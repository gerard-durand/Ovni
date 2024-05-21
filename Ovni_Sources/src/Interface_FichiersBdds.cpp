#include "Interface.h"

// Lecture et écriture de divers formats de BDDs
// *********************************************

/* Fonctions principales :
   create_bdd   Lecture de BDDs et création de la base de données interne
        LoadBDD     Chargement d'un fichier SDM Oktal (Format de base pour Crira) : extension .bdd
        LoadOBJ     Chargement d'un fichier Wavefront           : extension .obj
        Load3DS     Chargement d'un fichier Autodesk            : extension .3ds
        LoadOFF     Chargement d'un fichier Object File Format  : extension .off
        LoadPLY     Chargement d'un fichier Niratam ou Stanford Polygon File (détection automatique) : extension .ply
        LoadM3D     Chargement d'un fichier Milkshape 3D Texte  : extension .m3d
        LoadG3D     Chargement d'un fichier Xml Groupe 3D Onera : extension .g3d
        LoadSTL     Chargement d'un fichier Standard Triangle Language (Ascii ou binaire) : extension .stl
        Load        Chargement d'un fichier .dxf (support partiel en lecture, seulement affichage, pas de transformation possible)
        LoadDXF     Version améliorée de Load qui permet d'enregistrer dans un autre format via les initialisations dans RenderDXF (comme .bdd d'Oktal)

   SaveTo       Enregistrement de Bdds sous différents formats
        SaveBDD     Enregistrement au format SDM Oktal .bdd (par défaut)
        SaveOBJ     Enregistrement au format Wavefront .obj
        SaveOFF     Enregistrement au format Object File Format .off
        SaveG3D     Enregistrement au format Xml Groupe 3D .g3d
        SaveSTL     Enregistrement au format Standard Triangle Language .stl (Ascii ou binaire)
*/

#include "utf8.h"
#include "ply.h"

FILE* f;                                    // Doit être ici pour pouvoir être utilisé aussi dans la lecture des fichiers G3D (hors BddInter)

#define XML_FMT_INT_MOD "l"
#define XML_BUFFSIZE    8192

char XML_Buff[XML_BUFFSIZE];
int  XML_Depth;
int  i_objetXML_courant=0;                  // Utilisé seulement en lecture de fichier G3D. Donner une valeur initiale
static int id_timer=1000;

char s1[666]; //,buffer[1000] ;			    // chaines de caractères. Déclarée ici car utilisée aussi dans la lecture des fichiers G3D (hors BddInter)


wxString BddInter::get_file() {
    return this->file;
}

wxString BddInter::get_firstFile() {
    return this->first_file;
}

void BddInter::set_file(wxString file) {
    this->file=file;
}

void BddInter::set_firstFile(wxString file) {
    this->first_file=file;
}

void BddInter::clearall() {

    if(verbose) printf("Entree de BddInter::clearall\n");
    Objetlist.clear();
    wxStringlist.clear();

    type_fichier    =   1;      // <=> .bdd
    reset_zoom      = true;
    m_loaded        = false;
    diagonale_save  = 0.0;
    glDeleteLists( 1, 100);     // ?? pourquoi 100 et pas plus ? que se passe t-il si plus de 100 listes ? 8 doit être suffisant en fait (glliste_* de 1 à 8)
    glClear(GL_COLOR_BUFFER_BIT);
    m_gldata.initialized = false;
}

// Appel à la lecture puis la création de BDDs

void BddInter::create_bdd() {
    Object *objet_courant;
    Face   *facette_courante;

    unsigned int indice_premierObjet;   // indice du premier objet de la nouvelle Bdd à charger

    if(verbose) printf("Entree de BddInter::create_bdd\n");

//    traiter_doublons_aretes = traiter_doublons_aretes_def;              // réinitialiser à chaque nouvelle lecture de fichier

    if (Numero_base == 0) {                                             // Ouvrir ou réouvrir un fichier
        clearall();                                                         // RAZ global
        wxNomsFichiers = wxFileNameFromPath(get_file());                    // N'extraire que le nom du fichier (pas le path !)
    } else {                                                            // Fusion avec la bdd déjà en mémoire
        Numero_base = this->Objetlist.rbegin()->GetValue();                 // Retourne la "value" du dernier objet de la liste
        Numero_base = ((Numero_base + 10)/10)*10;                           // Pour calculer un offset de numérotation et commencer à la dizaine supérieure
        wxNomsFichiers += _T(" + ") + wxFileNameFromPath(get_file());       // Concaténer avec le nom du/des fichier(s) précédent(s)
    }
    indice_premierObjet = this->Objetlist.size();                       // Utile pour les fusions
    wxString str_nom = this->file;
    str_nom.MakeLower();    // Force le nom à être en minuscules (simplifie les tests de détection d'extension ci-dessous)

    wxCharBuffer buffer=str_nom.mb_str();
    printf("Chargement de : %s\n",buffer.data());
    if(str_nom.EndsWith(_T(".bdd"))) {
        type_fichier = 1;
    } else if(str_nom.EndsWith(_T(".3ds"))) {
        type_fichier = 2;
    } else if(str_nom.EndsWith(_T(".g3d"))) {
        type_fichier = 3;
    } else if(str_nom.EndsWith(_T(".obj"))) {
        type_fichier = 4;
    } else if(str_nom.EndsWith(_T(".ply"))) {
        type_fichier = 5;
    } else if(str_nom.EndsWith(_T(".off"))) {
        type_fichier = 6;
    } else if(str_nom.EndsWith(_T(".m3d"))) {
        type_fichier = 7;
    } else if(str_nom.EndsWith(_T(".stl"))) {
        type_fichier = 8;
    } else if(str_nom.EndsWith(_T(".dxf"))) {
        type_fichier = 0;
    }
    printf("create_bdd : type_fichier=%d\n",type_fichier);

    Dialog_Titre   = wxS("Lecture du fichier");
    Dialog_Comment = wxFileNameFromPath(get_file());
    Dialog_Delay   = CLOCKS_PER_SEC/2;      // 1/2 seconde

    wxFileInputStream stream(str_nom);
    mtllib_OK = false;                      // Ne sert que si une ligne mtllib existe dans un fichier .obj
    if(type_fichier != -1) {
        switch(type_fichier) {
        case 0:
            printf("\nChargement d'un fichier .dxf !!!\n");
            printf("ATTENTION : Le support des fichiers dxf n'est que partiel !\n");
            sprintf(Message,"            Seuls les fichiers ayant un bloc ENTITIES contenant les mots clés 3DFACE et LINE sont traités.\n");
            printf("%s",utf8_To_ibm(Message));
            printf("            Rendu visuel via RenderDXF(), sauvegarde possible en format SDM .bdd, .obj,... via SaveTo()\n");
            sprintf(Message,"            Mais pas de manipulations directes, sélections/affichages de facettes, points, arêtes ... etc.\n");
            printf("%s",utf8_To_ibm(Message));
//            m_renderer.Load(stream);              // lecture incompatible de BddInter !! Ancienne lecture via DXFRenderer.cpp
            this->LoadDXF(stream);                  // lecture maintenant compatible de BddInter !!
//            this->m_loaded = m_renderer.IsOk();   //true; // Récupère le m_loaded interne à m_renderer
            this->m_gldata.zoom = 4;                // ces valeurs sont compatibles de penguin.dxf ! Pour généraliser, il faut récupérer des valeurs comme min et max en x, y et z
            this->m_gldata.zoom_step = 0.5;
            this->m_gldata.posz = -150.;
            this->m_gldata.zNear= abs(this->m_gldata.posz) -10.;    // Objet recadré sur -5,+5 dans BddInter::NormalizeEntitiesDXF
            this->m_gldata.zFar = abs(this->m_gldata.posz) +10.;
            this->m_gldata.posx = 0.;
            this->m_gldata.posy = 0.;
            centre_auto = {0.0f, 0.0f, 0.0f};   // <=> centre_auto[0] = centre_auto[1] = centre_auto[2] = 0.0;
            centreRot = centre_auto;
            diagonale_save = 10.*sqrt(3.);
            x_min = y_min = z_min = -5.;        // Objet recadré sur -5,+5 dans BddInter::NormalizeEntitiesDXF
            x_max = y_max = z_max = +5.;
//            Search_Min_Max();
//            ResetData();
            this->Refresh(true);
            this->SetFocus();
            this->finishdraw=true;
            type_dxf = true;    // Pour appeler RenderDXF plutôt que DrawOpenGL
            break;
        case 1:
            printf("\nChargement d'un fichier .bdd !!!\n");
            this->LoadBDD();
//            printf("Nb groupes   : %d\n",listeGroupes.size());
//            for (int n : listeGroupes) std::cout << n << ' '; std::cout << '\n' ;
//            printf("Nb materiaux : %d\n",listeMateriaux.size());
//            for (int n : listeMateriaux) std::cout << n << ' '; std::cout << '\n' ;
            break;
        case 2:
            printf("\nChargement d'un fichier Autodesk .3ds !!! \n");
            this->Load3DS();
            break;
        case 3:
            printf("\nChargement d'un fichier Xml .g3d !!! \n");
            this->LoadG3D();
            break;
        case 4:
            printf("\nChargement d'un fichier Wavefront .obj !!! \n");
            this->LoadOBJ();
            break;
        case 5:
            printf("\nChargement d'un fichier .ply Niratam ou Stanford Polygon File !!! \n");
            this->LoadPLY();
            break;
        case 6:
            printf("\nChargement d'un fichier .off !!! \n");
            this->LoadOFF();
            break;
        case 7:
            printf("\nChargement d'un fichier .m3d !!! \n");
            this->LoadM3D();
            break;
        case 8:
            printf("\nChargement d'un fichier .stl !!! \n");
            this->LoadSTL();
            break;
        }
    }
    if (type_fichier > 0) {         // On ne passe pas dans ce if pour les fichiers dxf
        type_dxf     = false;
        bdd_modifiee = false;       // Pas sûr que ce soit ici le meilleur endroit, surtout si on fusionne des bdd !

        bool Normales_sommets_presentes = false;
        unsigned int i;
        unsigned int nb_facettes_loc = 0;
        for (i=indice_premierObjet; i<this->Objetlist.size(); ++i) {
            nb_facettes_loc += this->Objetlist[i].Nb_facettes;
        }
        if (nb_facettes_loc < nb_facettes_test) // Si trop de facettes, éviter de le faire ici car c'est long et pas toujours utile
            Genere_Tableau_Aretes_OK = true;
        else
            Genere_Tableau_Aretes_OK = false;
        for (i=indice_premierObjet; i<this->Objetlist.size(); ++i) {
            objet_courant = &(this->Objetlist[i]);
            if ((objet_courant->Nb_vecteurs != 0) && (objet_courant->Nb_luminances != 0)) Normales_sommets_presentes = true;
            Genere_Tableau_Points_Facettes(objet_courant);
            if(Genere_Tableau_Aretes_OK)                              // Ne pas générer maintenant si beaucoup de facettes, mais test mal adapté et
                Genere_Tableau_Aretes(objet_courant);                 // non compatible à ce qu'il faudrait dans OvniFrame::OnButton_FilaireToggle
        }

        this->Search_Min_Max();

// Etablir une liste de numéros de groupes et de matériaux
        listeGroupes.clear();       // à faire ailleurs ? (surtout si fusion de plusieurs Bdds)
        listeMateriaux.clear();
        listeObjets.clear();
        listePoints.clear();
        for (i=0; i<this->Objetlist.size(); ++i) {
            Genere_Liste_Groupes_Materiaux(i);
        }
        listeGroupes.sort();                                            // Trier les listes
        listeMateriaux.sort();
        printf("Liste des groupes   : ");
//        std::list<int>::iterator it = listeGroupes.begin();
        auto it = listeGroupes.begin();
        for (i=0; i<listeGroupes.size();   i++) {
            printf("%3d ", *it++); // ou it = listeGroupes.begin(); std::advance(it, i); ou encore std::list<int>::iterator it = std::next(listeGroupes.begin(), i);
        }
        printf("\n");
        it = listeMateriaux.begin();
        sprintf(Message,"Liste des matériaux : ");
        printf("%s",utf8_To_ibm(Message));
        for (i=0; i<listeMateriaux.size(); i++,it++) {
            printf("%3d ", *it);
        }
        printf("\n");

// Ici on force le calcul des normales aux sommets même si elles sont données dans le fichier, mais après confirmation

        if (CalculNormalesLectureBdd) {
            bool Forcer_calcul=false;
            for (i=indice_premierObjet; i<this->Objetlist.size(); ++i) {
                objet_courant = &(this->Objetlist[i]);
                if (Normales_sommets_presentes) {
                    if (i == 0) {
                        wxString wxMessage = _T("Les Vecteurs et Luminances sont déjà présents dans la BDD.\n");
                        wxMessage         += _T("Le calcul refait à la lecture du fichier va les remplacer...\n");
                        wxMessage         += _T("Est-ce bien ce que vous voulez ?");

                        long retour_Display = DisplayMessage(wxMessage, _T("Question"),
                                                             wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
                        if (retour_Display == wxID_YES) {
                            Forcer_calcul = true;
                            bdd_modifiee  = true;
                        }
                    }
                }
                else Forcer_calcul = true;
                if (Forcer_calcul) {
                    indiceObjet_courant = i;
                    unsigned int nb_p = objet_courant->Nb_sommets;
                    this->N_elements  = nb_p;
                    this->str.clear();
                    make_vecteur();

                    unsigned int nb_fac = objet_courant->Nb_facettes;
                    this->N_elements    = nb_fac;
                    make_luminance();

                    // Recopie des numéros de sommets des facettes dans luminances
#pragma omp parallel for private(facette_courante)
                    for (unsigned int nfac=0; nfac < nb_fac ; nfac++) {
                        facette_courante = &(this->Objetlist[i].Facelist[nfac]);
//                        NumerosSommets   = facette_courante->getF_sommets();
//                        this->N_elements = facette_courante->getNumero();
//                        make_1_luminance();
                        make_1_luminance(facette_courante->getNumero(),facette_courante->getF_sommets());
                        facette_courante->flat = false ;
                    }

                    Genere_Normales_Aux_Sommets(i, nb_p);
                    objet_courant->flat=false;
                }
            }
        }
    }
//    this->Refresh(true);  // Sert à quoi ?
    if(verbose) printf("Sortie de BddInter::create_bdd\n");
}

// Fichiers G3D, de type XML

//****************************************************************************
static void XMLCALL start_XML_Element(void *data, const char *el, const char **attr)
{
    int i,n,i1;
    unsigned int tmp, tmp2 ;
    int o;
    static unsigned int ind_xyz, ind_fac ;
    BddInter *Element=(BddInter *)data;
    float vx,vy,vz;
    char *cptr;
//    int valp[500];  // Attention, stockage provisoire limité à 500 valeurs (nb sommets ou normales / facette)
    int  val_id=0 ;
    static int nb_val_id=0;
    char sc[100];
    int  nc ;
    static float vxyz[3];
    wxString wxNumeros;

    Face   *Face_ij;
    Object *Objet_courant;
    std::vector<int> Numeros_L;

// Profondeur et nombre d'attributs détectés
/*  printf("Depth=%2d",XML_Depth) ;
    for (i = 0; attr[i]; i += 2) {
    }
    printf(", nb_atts=%2d ",i/2);
// *****************************************

    for (i = 0; i < XML_Depth; i++)
        printf("  ");

    printf("%s", el); */

    o = i_objetXML_courant;

    for (i = 0; attr[i]; i += 2) {
//    printf(" %s='%s'", attr[i], attr[i + 1]);

        if (XML_Depth == 1) {
            if (!strcmp(el,"objets")) {
                if (!strcmp(attr[i],"nbr")) {
                    o = atoi(attr[i+1]);
                    printf("Nb objets : attribut nbr= %d\n",o);
//                    o = i_objetXML_courant = -1;
                }
            } else if(!strcmp(el,"types_valeurs")) {
                if (!strcmp(attr[i],"nbr")) {
                    nb_val_id = atoi(attr[i+1]);
                }
            }
        } else if (XML_Depth == 2) {
            if (!strcmp(el,"objet")) {
                if (i == 0) {
                    Element->str = _T("<OBJET> 0 Test_Init"); // Donner un numéro et un nom provisoire
                    Element->make_objet();
                    // Initialisations pour un objet
//                    o=++i_objetXML_courant ;
                    o = i_objetXML_courant = Element->indiceObjet_courant; //Objetlist.size() -1;
                    Objet_courant = &(Element->Objetlist[o]);
                    Objet_courant->afficher = true;
                    Objet_courant->deleted  = false;
                    Objet_courant->flat     = true;
//                    Element->indiceObjet_courant   = o;
                }
                if (!strcmp(attr[i],"id")) {
                    Element->Objetlist[o].SetValue(atoi(attr[i+1])+Element->Numero_base);   // En cas de fusion, on ajoute Numero_base en offset
                    printf("Indice objet : %d balise id+offset : %d\n",i_objetXML_courant,Element->Objetlist[o].GetValue()); // atoi(attr[i+1])+Element->Numero_base
                }
                if (!strcmp(attr[i],"nom")) {

#if wxCHECK_VERSION(3,0,0)
                    wxString wxNom = wxString(attr[i+1]);
#else
                    wxString wxNom = wxString::FromAscii(attr[i+1]);
#endif // wxCHECK_VERSION

                    Element->Objetlist[o].SetName(wxNom);
                    printf("Indice objet : %d Nom lu dans la balise nom : %s\n",i_objetXML_courant,attr[i+1]);
                }
            } else if(!strcmp(el,"type_materiau")) {
                if (!strcmp(attr[i],"id")) {
                    val_id = atoi(attr[i+1]);
                } else if (!strcmp(attr[i],"nom")) { // pour OVNI, seul le matériau "optique" est reconnu. Le matériau thermique est ignoré
                    if (!strcmp(attr[i+1],"optique")  ) codemateriau= val_id; // n° d'Identification du mot clé matériau optique
//                    if (!strcmp(attr[i+1],"thermique")) codemateriau= val_id; // idem pour le numéro de matériau thermique
                }
            } else if(!strcmp(el,"type_valeur")) {
                if (!strcmp(attr[i],"id")) {
                    val_id = atoi(attr[i+1]);
                } else if (!strcmp(attr[i],"nom")) { // pour OVNI, actuellement, tous les mots clés sont équivallents à "groupe". codmatface devrait être <=> codemateriau mais conflit ?
                    if (!strcmp(attr[i+1],"groupe")    ) codegroupe = val_id; // n° d'Identification du mot clé groupe
                    if (!strcmp(attr[i+1],"codmatface")) codegroupe = val_id; // idem pour le numéro de matériau
                    if (!strcmp(attr[i+1],"code Crira")) codegroupe = val_id; // idem pour le numéro de matériau (synonyme de codmatface dans ce format de fichier ?)
                    printf("type_valeur id:%d/%d nom:%s\n",val_id,nb_val_id,attr[i+1]);
                }
            }
        } else if (XML_Depth == 5) {
            Objet_courant = &(Element->Objetlist[o]);
            if (!strcmp(el,"sommets")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_sommets = tmp;
//                    printf("Indice objet : %d, sommets   , balise nbr :%d\n",o,tmp);
                // Création du tableau des points
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->make_sommet();
                }
            } else if (!strcmp(el,"normales_s")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_vecteurs = tmp;
//                    printf("Indice objet : %d, normales_s, balise nbr :%d\n",o,tmp);
                // Création du tableau des normales aux sommets
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->make_vecteur();
                }
            } else if (!strcmp(el,"facettes")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_facettes = tmp;
//                    printf("Indice objet : %d, facettes,   balise nbr :%d\n",o,tmp);
                // Création du tableau des facettes
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->make_face();
                    Element->make_normale();
                    Element->make_aspect_face();
//                    if (Objet_courant->Nb_vecteurs != 0)
//                        Element->make_luminance();
                }
            }
        } else if (XML_Depth == 6) {
            if (!strcmp(el,"sommet")) {
                if (!strcmp(attr[i],"id")) {
                    // Ne marchera bien que si id est en 0
                    ind_xyz = atoi(attr[i+1]);              // Indice courant du sommet
                } else if (!strcmp(attr[i],"xyz")) {        // lecture des x,y,z des sommets
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz);
                    Element->str.clear();
                    Element->N_elements = ind_xyz;
                    Element->Setxyz(vx,vy,vz);
                    Element->make_1_sommet();
                }
            } else if (!strcmp(el,"normale_s")) {
                if (!strcmp(attr[i],"id")) {
                    ind_xyz = atoi(attr[i+1]);              // Indice courant de la normale au sommet
                } else if (!strcmp(attr[i],"xyz")) {        // lecture des x,y,z des normales au sommet
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz);
                    Element->str.clear();
                    Element->N_elements = ind_xyz;
                    Element->Setxyz(vx,vy,vz);
                    Element->make_1_vecteur();
                }
            } else if (!strcmp(el,"facette")) {
                if (!strcmp(attr[i],"id")) {
                    ind_fac = atoi(attr[i+1]);              // Indice courant de la facette
                    // Initialiser quelques valeurs de la facette
                    Element->Objetlist[o].Facelist[ind_fac-1].flat = true;
                }
            }
        } else if (XML_Depth == 7) {
            Face_ij = &(Element->Objetlist[o].Facelist[ind_fac-1]);
            if (!strcmp(el,"normale_b")) {
                if (!strcmp(attr[i],"xyz")) {
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz); // Lire x,y,z de la normale au barycentre
                    Element->str.clear();
                    Element->N_elements = ind_fac;
                    Element->Setxyz(vx,vy,vz);
                    // Garder les 3 valeurs dans un tableau statique. On en aura besoin si le mot clé sommets est lu après normale_b
                    vxyz[0] = vx ; vxyz[1] = vy ; vxyz[2] = vz ;
                    Element->make_1_normale();
                }
            } else if (!strcmp(el,"sommets")) {
                if (!strcmp(attr[i],"ref")) {
//                    i1=sscanf(attr[i+1],"%d",&tmp);
//                    printf("%d %d\n",i1,tmp);
                    cptr = (char *)attr[i+1] ;
                    i1=0;                                               // Pour compter le nombre de sommets
                    while (*cptr == ' ') cptr++ ;                       // Eliminer les blancs de tête
                    do {
                        i1++;
//                        sscanf(cptr,"%d",&valp[i1]) ;                   // Stockage provisoire, mais valp ne sert plus ici
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
                        while (*cptr == ' ') cptr++ ;                   // Passer les blancs
                        nc=sscanf(cptr,"%s",sc) ;
                    } while (nc != EOF) ;
//                    printf("%d sommets dans cette facette\n",i1);
                    n=i1;
                    Element->str.Printf(_T("%d %d "),ind_fac,n);
                    wxNumeros = wxString::FromAscii(attr[i+1]) ;        // plutôt que les valp on met l'intégralité de la chaîne attr
                    Element->str += wxNumeros;
                    Element->make_1_face();
                // make_1_face remet à 0 la normale au barycentre
                // => la restituer via vxyz (intérêt si sommets lu après normale_b)
//                    Element->Objetlist[Element->indiceObjet_courant].Facelist[ind_fac-1].setNormale_b(vxyz);
//                    Element->Objetlist[Element->indiceObjet_courant].Facelist[ind_fac-1].flat = true ; // Facette plane par défaut => normales_s lu après !
                    Face_ij->setNormale_b(vxyz);
//                    Face_ij->flat = true ; // Facette plane par défaut => normales_s doit être lu après !
                }
            } else if (!strcmp(el,"normales_s")) {
                Objet_courant = &(Element->Objetlist[o]);
                if (!strcmp(attr[i],"ref")) {
//                    i1=sscanf(attr[i+1],"%d",&tmp);
//                    printf("%d %d\n",i1,tmp);
                    cptr = (char *)attr[i+1] ;
                    i1=0;                                               // Pour compter le nombre de normales aux sommets
                    while (*cptr == ' ') cptr++ ;                       // Eliminer les blancs de tête
                    do {
                        i1++;
 //                       sscanf(cptr,"%d",&valp[i1]) ;                   // Stockage provisoire
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
                        while (*cptr == ' ') cptr++ ;                   // Passer les blancs
                        nc=sscanf(cptr,"%s",sc) ;
                    } while (nc != EOF) ;
//                    printf("%d sommets dans cette facette\n",i1);
                    n=i1;

                    if (Objet_courant->Nb_luminances == 0) {
                        Element->N_elements = Objet_courant->Nb_facettes;
                        Element->str.clear();
                        Element->make_luminance();
                    }
                    Element->str.Printf(_T("%d %d "),ind_fac,n);
                    wxNumeros = wxString::FromAscii(attr[i+1]) ;
                    Element->str += wxNumeros;
                    Element->make_1_luminance();
                    Face_ij->flat = false ;    // La facette n'est a priori pas plane
                    Objet_courant->flat = false;                         // Du coup, idem pour l'objet.
                    // Vérifier que la facette n'est pas plane => même numéro de vecteur sur tous les sommets
                    Numeros_L = Face_ij->getL_sommets();
                    int Num_0 = Numeros_L[0];
                    bool Facette_plane = true;
                    for (unsigned int k=1; k<Numeros_L.size(); k++) {
                        if (Numeros_L[k] != Num_0) Facette_plane = false;
                    }
                    if (Facette_plane) Face_ij->flat = true ;    // La facette est donc plane
                }
            } else if (!strcmp(el,"materiaux")) {
                if (!strcmp(attr[i],"ref")) {
                    sscanf(attr[i+1],"%d%d",&tmp,&tmp2);
                    // Affecter groupe ou materiau suivant ce qui a été donné dans types_valeurs
                    // ATTENTION : ci dessous ne marche que si codmatface est après codegroupe dans le fichier g3d
                    //             de même, le mot clé materiaux (ou valeurs) doit être situé après le mot clé sommets sinon make_1_face réinitialise groupe de codmatface !
//                    if (tmp==codegroupe  )
//                        Element->str.Printf(_T("%d <GROUPE>  %d"),ind_fac,tmp2)    ; Element->make_1_aspect_face();
//                    if (tmp==codemateriau)
//                        Element->str.Printf(_T("%d <CODMATFACE> %d"),ind_fac,tmp2); Element->make_1_aspect_face(); // ind_fac,
                        Face_ij->codmatface = tmp2;
                }
            } else if (!strcmp(el,"valeurs")) {
                if (!strcmp(attr[i],"val")) {
                    sscanf(attr[i+1],"%d%f",&tmp,&vx);
                    // Affecter groupe ou materiau suivant ce qui a été donné dans types_valeurs
//                    if (tmp==codegroupe  )
//                        Element->str.Printf(_T("%d <GROUPE>  %d"),ind_fac,(int)vx)     ; Element->make_1_aspect_face();
                        Face_ij->groupe = (int)vx;
//                    if (tmp==codemateriau)
//                        Element->str.Printf(_T("%d <CODMATFACE> %d"),ind_fac,(int)vx) ; Element->make_1_aspect_face(); //ind_fac,
                }
            }
        }
    }

//  printf("\n");
    XML_Depth++;
}

static void XMLCALL end_XML_Element(void *data, const char *el)
{
    XML_Depth--;
}

//******************************************************************************

void BddInter::LectureXML_G3d (FILE *f)
{
   /*
    *  Lecture d'un Fichier de polygones au format XML G3d Version 2.
    */
    int done;
    int len;
    int o;
    int nb_T_points, nb_T_norml, nb_T_facettes ;
    XML_Parser p;
    unsigned int indice_premierObjet;

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    rewind(f);

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    indice_premierObjet = this->Objetlist.size();   // Pour utilisation en cas de fusion

    sprintf(Message,"\nNon complètement testé : pas assez de fichiers de test ...\n");
    printf("%s",utf8_To_ibm(Message));
//    printf("\nNon completement teste : pas assez de fichiers de test ...\n");

    codemateriau = -1;
    codegroupe   = -1;

    p = XML_ParserCreate(nullptr);
    if (!p) {
        fprintf(stderr, "Allocation memoire impossible pour le parseur\n");
        exit(-1);
    }

    XML_SetElementHandler(p, start_XML_Element, end_XML_Element);
    XML_SetUserData(p, this);

    for (;;) {

        len = (int)fread(XML_Buff, 1, XML_BUFFSIZE, f);
        if (ferror(f)) {
            fprintf(stderr, "Erreur de lecture du fichier XML\n");
            exit(-1);
        }
        Update_Dialog((long long)ftello64(f), fichierBdd_length);
        done = feof(f);

        if (XML_Parse(p, XML_Buff, len, done) == XML_STATUS_ERROR) {
            fprintf(stderr, "Erreur de Parseur ligne : %" XML_FMT_INT_MOD "u:\n%s\n",
                    XML_GetCurrentLineNumber(p),
                    XML_ErrorString(XML_GetErrorCode(p)));
#ifdef WIN32
            system("pause") ;
#endif
            Fermer_progress_dialog();

            exit(-1);
        }

        if (done)
            break;
    }
    XML_ParserFree(p);

//    printf("Nombre d'objets                : %d\n",xnb_obj);
    int nb_objets = this->Objetlist.size();
    printf("\nNombre d'objets : %d\n",nb_objets -indice_premierObjet);
    /*    printf("Nombre total de points         : %d\n",xnb_pts);
        printf("Nombre de normales aux sommets : %d\n",xnb_norm);
        printf("Nombre total de facettes       : %d\n",xnb_fac); */

    nb_T_points = nb_T_norml = nb_T_facettes = 0 ;

//    if (indice_premierObjet != 0) printf("Recapitulatif de tous les objets :\n");

    for (o=indice_premierObjet; o< nb_objets; o++) {
		indiceObjet_courant = o;
        sprintf(Message,"\nNuméro de l'objet %2d                 : %d\n",o,this->Objetlist[indiceObjet_courant].GetValue());
        printf("%s",utf8_To_ibm(Message));
        wxCharBuffer buf_nom = this->Objetlist[o].GetName();    // Normalement déjà en utf8 dans le format g3d
        printf("Nom de l'objet %2d                    : %s\n",o,buf_nom.data());
        printf("Nombre de points de l'objet %2d       : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_sommets);
        nb_T_points += Objetlist[o].Nb_sommets;
        printf("Nombre de normales_s de l'objet %2d   : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_vecteurs);
        nb_T_norml += this->Objetlist[o].Nb_vecteurs ;
        printf("Nombre de facettes de l'objet %2d     : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_facettes);
        nb_T_facettes += this->Objetlist[o].Nb_facettes ;
    }
    printf("\n");
    printf("Nombre total de points               : %d\n",nb_T_points  );
    printf("Nombre total de normales aux sommets : %d\n",nb_T_norml   );
    printf("Nombre total de facettes             : %d\n",nb_T_facettes);

    Fermer_progress_dialog();

    sprintf(Message,"Non encore totalement opérationnel/testé\n\n");
    printf("%s",utf8_To_ibm(Message));
/*#ifdef WIN32
    system("pause") ;
#endif
    exit(9); */
}

//******************************************************************************
// NOTE : dans les fonctions Load* la parallélisation des boucles for n'est probablement pas faisable en général car la lecture des fichiers est forcément séquentielle
//        à moins que l'attribut ordered puisse être utilisé, mais ce n'est pas sûr ... à tester

void BddInter::LoadG3D()
{
    if(verbose) printf("Entree de BddInter::LoadG3D\n");
    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f = fopen(buffer.data(),"r");	//ouverture du fichier
    fgets(s1,160,f) ;
    if (!strncmp(s1,"<?xml ",6)) {
    // Lire la ligne suivante pour identifier g3d version 2
        fgets(s1,160,f);
        if (!strncmp(s1,"<g3d version=\"2.",16)) {           // Vérifie que la seconde ligne commence par <g3d version="2.
            sprintf(Message,"Fichier de type : XML Format g3d Version 2\n") ;
            printf("%s",utf8_To_ibm(Message)) ;
//            printf("Fichier de type : XML Format g3d Version 2\n") ;
//           printf("%x %x\n",cel,&cel);
            LectureXML_G3d(f); //cel,f,entete,svg_entete) ;
            fclose(f) ;
//            if (svg_entete && entete != nullptr) fclose(entete) ;
//            init_liste_Materiaux(cel) ;
            m_loaded = true;
            m_gllist = 0;

            return; // (0) ;
        } else {
            sprintf(Message,"Fichier de type XML mais n'est pas du g3d Version 2\n") ;
            printf("%s",utf8_To_ibm(Message)) ;
            type_fichier = -1; // Erreur
        }
    }

    fclose(f);
    if(verbose) printf("Sortie de BddInter\n");
}

// Pour fichiers Waveforont obj

void BddInter::Optimiser_Obj_Sommets(Object *objet_courant, int o, bool &msg_optim)
{
    int i, j;
    int indice_min, indice_max, numero_sommet;
    int NbFacettes;
    Face *facette_courante;

    NbFacettes = objet_courant->Nb_facettes;

    if (NbFacettes <= 0) return;    // Pas de facettes, sortir immédiatement car rien à faire sur cet objet

    if (msg_optim) {
        printf("Optimisation du fichier .obj : Traitement des sommets de facettes et des vecteurs\n");  // à n'écrire que la première fois
        msg_optim = false;
    }
    indice_max = -1;
    indice_min = objet_courant->Nb_sommets;                     // On traite d'abord les sommets de facettes
#pragma omp parallel for private(facette_courante,numero_sommet)
    for (i=0; i<NbFacettes; i++) {                              // Recherche des numéros de sommets min et max utilisés dans l'objet
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_F; j++) {
            numero_sommet = facette_courante->F_sommets[j];
#pragma omp critical
            {
            indice_min = std::min(indice_min, numero_sommet);
            indice_max = std::max(indice_max, numero_sommet);
            }
        }
    }
    printf("Objet %3d, Sommets  : indice_min=%5d, indice_max=%5d, Nouvelles valeurs [1,%d]\n",o,indice_min,indice_max,indice_max-indice_min+1);
    // Diminuer la taille du tableau des sommets en enlevant le début -> indice_min-1 et la fin à partir de indice_max+1
//                    if (indice_max < (int)objet_courant->Nb_sommets)              // Effacer d'abord la fin
//                        objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin()+indice_max,objet_courant->Sommetlist.end());
    objet_courant->Sommetlist.resize(indice_max);                   // Effacer d'abord la fin : un resize suffit <=> ligne précédente !
    if (indice_min > 1)                                             // Puis le début
        objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin(),objet_courant->Sommetlist.begin()+indice_min-1);
    objet_courant->Nb_sommets = indice_max - indice_min +1;         // Mettre à jour Nb_sommets
    if (objet_courant->Nb_sommets != objet_courant->Sommetlist.size()) printf("Oups tailles != Nb_sommets\n");  // Au cas où ...
    // Changer les numéros de sommets des facettes
    indice_min--;
#pragma omp parallel for private(facette_courante)
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (int j=0; j < facette_courante->Nb_Sommets_F; j++) {
            facette_courante->F_sommets[j] -= indice_min;           // Renumérotation des sommets utilisés dans chaque facette
        }
    }
    // En toute rigueur, il faudrait aussi changer les objet_courant->Sommetlist[*].Numero en les décalant de indice_min (idem sur les vecteurs)
}

void BddInter::Optimiser_Obj_Vecteurs(Object *objet_courant, int o)
{
    int i, j;
    int indice_min, indice_max, numero_sommet;
    int NbFacettes;
    Face *facette_courante;

    NbFacettes = objet_courant->Nb_facettes;

    if (NbFacettes <= 0) return;    // Pas de facettes, sortir immédiatement car rien à faire sur cet objet

    indice_max = -1;
    indice_min = objet_courant->Nb_vecteurs;                        // On traite maintenant les normales aux sommets de facettes
    objet_courant->Nb_vecteurs = 0;                                 // Réinitialiser Nb_vecteurs
    if (objet_courant->Facelist[0].Nb_Sommets_L == 0) {
        objet_courant->Nb_luminances = 0;                           // et Nb_luminances par précaution ici !
        return;                                                     // Passer à l'objet suivant car pas de normales aux sommets des facettes
    }
#pragma omp parallel for private(facette_courante) // pb avec indice_* si 2 threads le modifient en même temps => tester avec omp critical ?
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_L; j++) {
            numero_sommet = facette_courante->L_sommets[j];
#pragma omp critical
            {
            indice_min = std::min(indice_min, numero_sommet);
            indice_max = std::max(indice_max, numero_sommet);
            }
        }
    }
    printf("Objet %3d, Vecteurs : indice_min=%5d, indice_max=%5d, Nouvelles valeurs [1,%d]\n",o,indice_min,indice_max,indice_max-indice_min+1);

    objet_courant->Vecteurlist.resize(indice_max);                  // Effacer d'abord la fin : un resize suffit
    if (indice_min > 1)                                             // Puis le début
        objet_courant->Vecteurlist.erase(objet_courant->Vecteurlist.begin(),objet_courant->Vecteurlist.begin()+indice_min-1);
    objet_courant->Nb_vecteurs = indice_max - indice_min +1;        // Mettre à jour Nb_vecteurs
    if (objet_courant->Nb_vecteurs != objet_courant->Vecteurlist.size()) printf("Oups tailles != Nb_vecteurs\n");
    // Changer les numéros de sommets des facettes
    indice_min--;
#pragma omp parallel for private(facette_courante)
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_L; j++) {
            facette_courante->L_sommets[j] -= indice_min;
        }
    }
    // En toute rigueur, il faudrait aussi changer les objet_courant->Vecteurlist[*].Numero en les décalant de indice_min (idem sur les sommets)
}

void BddInter::LoadOBJ()
{
/*
 *  Lecture d'un Fichier de polygones au format Wavefront OBJ
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  NOTE : la lecture se fait en 3 passes car les numéros de sommets peuvent être utilisés dans des objets/facettes avant d'être définis.
 *         Tous les sommets de tous les objets vont être définis dans le premier objet. Le tri et l'optimisation seront faits plus tard.
 */
//    FILE* f;                                      // Déjà déclaré au niveau général, donc autant l'utiliser
    char sc[100];//,s1[666],buffer[1000] ;			//chaines de caractères
    char nom_obj[512];
    char newline='\\' ;
    unsigned int i, n, nfac, npoint, nb_fac, nb_p, first, npoint_courant ;
//    unsigned int nfac_t;
    unsigned int nb_norm,  nnorm ;
    unsigned int npoint_t, nnorm_t ;
    unsigned int o=0, oo=0, found=0;		        // pour compter le nombre d'objets
    int nc;
    int valp[100],valn[100] ;

    #define nb_mat_max 4000
    char *tab_nom[nb_mat_max]; // Provisoire en attendant de mettre directement dans la structure d'objet

    int num_mat=-123 ;
//    bool Forcer_1_Seul_Objet=false ; // mis un niveau au dessus ! Si false, on crée le nombre d'objets trouvés dans la bdd, si true, on met tout dans 1 seul objet

    unsigned int num_min, num_max, num_tot, num_cur ;
    unsigned int *full_ind_sommets=nullptr, *new_ind_sommets=nullptr, *inv_ind_sommets=nullptr, *p_uint ;
    int indice, num_min1, num_min2;
    int indice_min,indice_max;
    unsigned norm_point_fac_existe, nb_normp_fac;
    int  compt=0, speed=200 ;

    unsigned int Nb_objets, indice_premierObjet;
    float vx,vy,vz;

    bool nom_utf8;
    wxString str_loc;

    std::vector<int> Numeros;

    Face *Face_ij;

    // A mettre ailleurs ??
    const char delimiters[] = "/\\" ;
    char *cptr, *token, *nom_fichier;           //Pointeur de chaîne de caractères

    if(verbose) printf("Entree de BddInter::LoadOBJ\n");

    wxCharBuffer buffer=this->file.mb_str();

    // A mettre ailleurs ??
    cptr  = strdup(buffer.data());
    token = strtok(cptr,delimiters) ;
    do {
        cptr=token;
        token = strtok(nullptr,delimiters) ;
    } while (token != nullptr) ;
    printf("Nom du fichier : %s\n", cptr);
    nom_fichier = cptr;                         // a simplifier ?

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f = fopen(buffer.data(),"r");	//ouverture du fichier

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    fgets(s1,160,f) ;
    if (!strncmp(s1,"#",1) || !strncmp(s1,"mtllib",6) || !strncmp(s1,"g ",2) || !strncmp(s1,"v ",2)) {
        if (s1[0] == '#') {
            for (i=2; i<100; i++) {
                if (fgets(s1,160,f) != nullptr) {
                    if(!strncmp(s1,"mtllib",6) || !strncmp(s1,"g ",2) || !strncmp(s1,"v ",2)) {
                        if(!strncmp(s1,"mtllib",6)) {
                            sprintf(Message,"Fichier Wavefront OBJ avec fichier matériau : %s",s1+7) ;
                            mtllib_OK  = true;
                            mtllib_nom = strdup(s1);    // Sauvegarde de la ligne mtllib
//                            printf("Fichier Wavefront OBJ avec fichier materiau : %s",s1+7) ;
                        } else {
                            sprintf(Message,"Fichier Wavefront OBJ : %s\n",cptr) ;
                            mtllib_OK  = false;
//                            printf("Fichier Wavefront OBJ : %s\n",cptr) ;
                        }
                        printf("%s",utf8_To_ibm(Message)) ;
                        found = 1 ;
                        break ;
                    }
                } else {
                    break;
                }
            }
        } else found = 1;
        if (found) {
//            printf("%d ligne\n",i) ;
//            LectureOBJ(cel,f,entete,svg_entete,cptr) ;
            nb_p   = 0;
            nb_fac = 0;
            nb_norm= 0;
            nb_mat = 0;
            o      = 0;
            tab_nom[0] = strdup(buffer.data()) ;
            rewind(f) ;

            while (fgets(s1,660,f) != nullptr) {
//                if (!strncmp(s1,"mtllib",6)) mtllib_OK = true; // Déjà fait plus tôt
                if (!strncmp(s1,"v ", 2)) nb_p++;
                if (!strncmp(s1,"f ", 2)) nb_fac++;
                if (!strncmp(s1,"fo ",3)) nb_fac++; // f et fo synonymes
                if (!strncmp(s1,"g ", 2)) {
                    tab_nom[o]=strdup(s1+2) ;
                    o++;
                    if (o == nb_mat_max) {
                      sprintf(Message,"ATTENTION : Trop d'objets dans ce fichier .obj augmenter nb_mat_max > %u\n",nb_mat_max);
                      printf("%s",utf8_To_ibm(Message));
//                      --nb_mat ; // On utilisera le dernier indice pour tous les suivants...
#ifdef WIN32
                        system("pause");
#endif
                    }
                }
                if (!strncmp(s1,"vn ",3)) nb_norm++;
                if (!strncmp(s1,"usemtl ",7)) {
                    if (nb_mat == 0) {
                        tab_mat[nb_mat++] = strdup(s1+7);
                    } else {
                        found=0;
                        for (i=0; i< nb_mat; i++) {
                            if (!strncmp(s1+7,tab_mat[i],strlen(tab_mat[i]))) {
                                found = 1;  // Ce matériau est déjà dans la liste
                                break;
                            }
                        }
                        if(!found) {        // Ajouter le nouveau matériau
                            tab_mat[nb_mat++] = strdup(s1+7);
                            if (nb_mat >= nb_mat_max) {
                                sprintf(Message,"ATTENTION : Trop de matériaux dans ce fichier .obj nb_mat > %u\n",nb_mat_max);
                                printf("%s",utf8_To_ibm(Message));
//                                printf("ATTENTION : Trop de materiaux dans ce fichier .obj nb_mat > %u\n",nb_mat_max);
                                --nb_mat ; // On utilisera le dernier indice pour tous les suivants...
#ifdef WIN32
                                system("pause");
#endif
                            }
                        }
                    }
                }
            }
            if (o == 0) o=1; // pas de "g ", mais tout de même un objet
            printf("Nombre total d'objets    : %d\n",o) ;
            if (o) {
                sprintf(Message,"Liste des nom lus:\n");
                printf("%s",utf8_To_ibm(Message));
//                printf("Liste des nom lus:\n");
                for (i=0; i< o; i++) {
                    str_loc  = wxString(tab_nom[i]);
                    nom_utf8 = is_utf8(tab_nom[i]) ;
                    if (!nom_utf8) str_loc = str_loc.utf8_str();    // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
                    sprintf(Message,"Numéro: %3d, %s",i+1,(const char *)str_loc.data());//tab_nom[i]); // Pas besoin de \n, il est déjà dans tab_nom
                    printf("%s",utf8_To_ibm(Message));
//                    printf("Numero: %3d, %s",i+1,tab_nom[i]); // Pas besoin de \n, il est déjà dans tab_nom
                }
                printf("\n") ;
            }
            printf("Nombre total de points   : %d\n",nb_p) ;
            printf("Nombre total de normales : %d\n",nb_norm) ;
            printf("Nombre total de facettes : %d\n",nb_fac)  ;
            sprintf(Message,"Nombre total de matériaux: %d\n",nb_mat);
            printf("%s",utf8_To_ibm(Message));
//            printf("Nombre total de materiaux: %d\n",nb_mat);
            if (nb_mat) {
                sprintf(Message,"Liste des matériaux :\n");
                printf("%s",utf8_To_ibm(Message));
//                printf("Liste des materiaux:\n");
                for (i=0; i< nb_mat; i++) {
                    str_loc  = wxString(tab_mat[i]);
                    nom_utf8 = is_utf8(tab_mat[i]);
                    if (!nom_utf8) str_loc = str_loc.utf8_str();    // Le nom du matériau n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
                    sprintf(Message,"Numéro: %3d, %s",i+1,(const char *)str_loc.data());//tab_mat[i]); // Pas besoin de \n, il est déjà dans tab_mat
                    printf("%s",utf8_To_ibm(Message));
//                    printf("Numero: %3d, %s",i+1,tab_mat[i]); // Pas besoin de \n, il est déjà dans tab_mat
                }
                printf("\n") ;
            }

            if(Forcer_1_Seul_Objet) o=1 ;
            Nb_objets = o;

// Création des tableaux de points et de normales (tous identiques, avec tous les points de tous les objets)
// Ici, ne créer que les tableaux pour l'objet 1 et y faire pointer ceux des autres objets.

            indice_premierObjet = this->Objetlist.size();   // indice du premier objet de l'objet à charger
            for (o=0; o<Nb_objets; o++) {
                this->str.Printf(_T("<OBJET> %d Test_%d"),o,o);    // Nom provisoire
                this->make_objet();
//                indiceObjet_courant = this->Objetlist.size() -1;

// ?? ne faire que si o != 0 ?
                this->N_elements = nb_p;
                this->str.clear();
                this->make_sommet();
                this->Objetlist[indiceObjet_courant].Nb_sommets = nb_p; // Ici tous les sommets

                this->N_elements = nb_norm;
//                if (nb_norm == 0) this->N_elements = nb_p ;  // Pas de normales dans le fichier => il faudra les calculer
//                if (nb_norm != 0) this->make_vecteur();
                this->make_vecteur();
                this->Objetlist[indiceObjet_courant].Nb_vecteurs = this->N_elements; // Ici toutes les normales aux sommets
            }

//
//            if (svg_entete && temp != nullptr) {
//                fprintf(temp,"#### Fichier original de type Wavefront OBJ ####\n\n") ;
//            }

// Seconde analyse pour remplir les tableaux de points et de normales aux sommets incluses dans le fichier et créer la structure
// pour les facettes
            rewind(f) ;
            o=indice_premierObjet ;
            indiceObjet_courant = o;
            npoint = 0 ;
            nfac   = 0 ;
            nnorm  = 0 ;
            first  = 1 ;
            while (fgets(s1,660,f) != nullptr) {   // Jusqu'à la fin de fichier
                if (!strncmp(s1,"v ", 2)) {     // Lecture des points (Les mettre tous dans l'objet 1)
                    npoint++;
                    sscanf(s1+2,"%f%f%f", &vx,&vy,&vz);
                    this->str.clear();
                    this->N_elements = npoint;
                    this->Setxyz(vx,vy,vz);
                    this->make_1_sommet();
                    continue;
                }
                if (!strncmp(s1,"vn ", 3)) {            // Lecture des normales aux sommets
                    nnorm++;
                    sscanf(s1+3,"%f%f%f", &vx,&vy,&vz);
                    this->str.clear();
                    this->N_elements = nnorm;
                    this->Setxyz(vx,vy,vz);
                    this->make_1_vecteur();
                    continue;
                }
                if (!strncmp(s1,"f ", 2) || !strncmp(s1,"fo ", 3)) {
                    nfac++;
                    continue;
                }
                if (!strncmp(s1,"g ", 2) && !Forcer_1_Seul_Objet) {
                    if (first) {
                        first = 0 ;
                    } else {

// Création du tableau des facettes de l'objet précédent
                        this->Objetlist[o].Nb_facettes = nfac ;                  // Nombre de facettes de l'objet
                        this->N_elements = nfac;
                        str.clear();
                        indiceObjet_courant = o;    // Pour le make_face et make_luminance, il faut l'indice réel dans indiceObjet_courant
                        make_face();
                        if (nb_norm != 0) make_luminance();
                        o++;
                        indiceObjet_courant = indice_premierObjet;    // Pour forcer à écrire tous les sommets et vecteurs dans le 1er objet
                        nfac = 0;
                    }
                    continue;
                }
            }

// Création du tableau des facettes du dernier objet
            this->Objetlist[o].Nb_facettes = nfac ;                  // Nombre de facettes de l'objet
            this->N_elements = nfac;
            str.clear();
            indiceObjet_courant = o;    // Pour le make_face et make_luminance, donner la bonne valeur à indiceObjet_courant
            make_face();
//            make_normale();
            if (nb_norm != 0) make_luminance();
//            make_aspect_face();

// Troisième analyse : remplissage des tableaux de facettes

            rewind(f) ;
//    printf("OK2...\n");
            npoint_courant = 1 ;
            i=0 ;
            if (mtllib_OK) {
                do {
                    i++;
                    fgets(s1,660,f) ;
                } while (strncmp(s1,"mtllib",6)) ;
                strcpy(nom_obj, Lire_chaine(s1+7)) ;      // Récupérer ce qui suit mtllib
        //        if ((cptr=strstr(nom_obj,".mtl")) != nullptr) *cptr='\0' ; // Supprime l'extension .mtl : ici capte aussi une chaine .mtl qui ne serait pas une extension (en fin de nom_obj)
                if ((cptr=strstr(nom_obj,".mtl")) == ((char *)&nom_obj+strlen(nom_obj)-4)) *cptr='\0' ; // Supprime l'extension .mtl
            } else {
                strcpy(nom_obj,nom_fichier) ;
                if ((cptr=strstr(nom_obj,".obj")) == ((char *)&nom_obj+strlen(nom_obj)-4)) *cptr='\0' ; // Supprime l'extension .obj
            }
            printf("\nNom de l'objet : %s\n",nom_obj) ;   // c'est le Nom par défaut (si pas de "g ")

#if wxCHECK_VERSION(3,0,0)
            wxString wxNom=wxString(nom_obj);
#else
            wxString wxNom=wxString::FromAscii(nom_obj);
#endif
            nom_utf8 = is_utf8(wxNom.data());
            if (!nom_utf8) wxNom = wxNom.utf8_str();        // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
            this->Objetlist[indice_premierObjet].SetName(wxNom);
        //    printf("OK3...\n");
            nfac    = 0 ;
//            nfac_t  = 0 ;
            nnorm   = 0 ;
            o = indice_premierObjet ;
            indiceObjet_courant = o;
            first = 1 ;
            while (fgets(s1,660,f) != nullptr) { // Jusqu'à la fin de fichier
        //        printf("%s",s1) ;
                Update_Dialog((long long)ftello64(f), fichierBdd_length);

                if (!strncmp(s1,"g ",2) && !Forcer_1_Seul_Objet) {
                    if (first) {
                        first = 0 ;
                    } else {
                        o++ ;
//                        nfac_t   += nfac;
                        nfac  =0 ;
                    }

#if wxCHECK_VERSION(3,0,0)
                    wxNom = wxString(Lire_chaine(s1+2));                    // Nom de l'objet
#else
                    wxNom = wxString::FromAscii(Lire_chaine(s1+2));
#endif // wxCHECK_VERSION
                    nom_utf8 = is_utf8(wxNom.data());
                    if (!nom_utf8) wxNom = wxNom.utf8_str();                // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8

                    this->Objetlist[o].SetName(wxNom);                      // ! A vérifier car actuellement 1 seul objet !
                    indiceObjet_courant = o;
                    continue;
                }
                if (!strncmp(s1,"usemtl ",7)) {         // Lecture du nom de matériau
                    for (i=0; i< nb_mat; i++) {
                        if (!strncmp(s1+7,tab_mat[i],strlen(tab_mat[i]))) {
                            num_mat = i+1;  // Le matériau est déjà dans la liste => num_mat = rang dans le tableau pour commencer à 1
                            // Cas particulier où le nom est group_* (conversion en .obj par Ovni)
                            if (!strncmp(s1+7,"group_",6)) {
                                num_mat = atoi(s1+13);  // Dans ce cas, ce qu'il y a derrière "group_" est le numéro de groupe original
                            }
                            break;
                        }
                    }
                    continue;
                }
                if (!strncmp(s1,"vn ", 3)) {            // comptage des normales aux sommets ... au cas où
                    nnorm++;
                    continue;
                }
                if (!strncmp(s1,"v ", 2)) {             // comptage des sommets ...
                    npoint_courant++;
                    continue;
                }
                if (!strncmp(s1,"f ",2) || !strncmp(s1,"fo ",3)) {
                    nfac++;
                    i=0 ;
                    cptr = s1+2 ;
                    nb_normp_fac = 0;
        //        printf("%s",s1);
                    do {
                        i++;
                        while (*cptr == ' ' || *cptr == '/') cptr++ ;   // Eliminer les blancs et les slashs
                        sscanf(cptr,"%d",&valp[i]) ;
                        if (valp[i] < 0) {                              // Ici ce sont des valeurs relatives / Point courant
                            valp[i] += npoint_courant ;
                            cptr++ ; // Sauter le signe -
                        }

        //        printf("lue %d\n",valp[i]) ;
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
        //        printf("%s",cptr) ;
                        norm_point_fac_existe = 0;
                        if (*cptr == '/') {                             // Atteindre le / suivant (ou l'espace ou la fin de ligne)
        //        printf("%s",cptr) ;
                            cptr++;
        //        printf("%s",cptr) ;
                            while (*cptr != '/' && *cptr != ' ' && *cptr != '\n') {
                                cptr++;
        //        printf("%s",cptr) ;
        //        system("pause");
                            }
                            if (*cptr == '/') norm_point_fac_existe = 1;
                            if (*cptr != '\n') cptr++;
                        }
                        if (nb_norm != 0 && norm_point_fac_existe) {
                            while (*cptr == ' ' || *cptr == '/') cptr++ ;
                            if (*cptr != '\n') {                            // Pourrait arriver si // en fin de ligne et rien derrière
                                sscanf(cptr,"%d",&valn[i]);
                                ++nb_normp_fac;
                                if (valn[i] < 0 ) {                         // Ici ce sont des valeurs relatives / normale courante
                                    valn[i] += (nnorm+1) ;
                                    cptr++ ; // Sauter le signe -
                                }
                                while (*cptr >= '0' && *cptr <= '9') cptr++ ; // Passer les chiffres
                            }
                        }
                        nc=sscanf(cptr,"%s",sc) ;
                        if (nc == 1 && sc[0] == newline) {
                            fgets(s1,660,f);
                            cptr = s1;
                        }
                    } while (nc != EOF) ;
        //            system("pause") ;
                    n = i ;

                    Numeros.resize(n);

                    for (i=0; i<n ; i++) {
                        Numeros[i] = valp[i+1];
                    }
                    str.clear();                        // Important à garder, sinon souci par la suite dans make_1_normale!
                    this->N_elements = nfac;            // Idem
//                    this->Set_numeros(Numeros);
//                    this->make_1_face();                    this->make_1_face(nfac,Numeros);

                    this->Setxyz(1.,0.,0.);
                    this->make_1_normale();             // Normale bidon. Sera mise à jour plus tard.

// Ci-dessous : OK si dans tous les cas autant de normales que de sommets. Si nnorm==0 pas de pb, sinon, si à la lecture
// certaines facettes n'ont pas de normales aux sommets => soucis ! N'arrive dans aucun fichier testé mais ....
// après introduction de nb_normp_fac, c'est un peu mieux, mais à vérifier ...

                    Numeros.resize(nb_normp_fac);
                    if (nb_normp_fac != 0 && nb_normp_fac != n) {
                        fprintf(stderr,"Attention : la facette %d a un nombre de points (%d) et de normales aux sommets (%d) different !\n",nfac,n,nb_normp_fac);
                        // Non this->Objetlist[o].Nb_normales = 0 ;    // Forcer à 0 car il y a certainement une erreur !!
                    }
                    for (i=0; i<n; i++) {
                        if(nb_norm != 0 && nb_normp_fac == n) Numeros[i]=valn[i+1];// - nnorm_t;
                        else                                  Numeros[i]=valp[i+1];// - npoint_t;
                    }
//                    printf("%d %d\n",nb_normp_fac,n);
                    Face_ij = &(this->Objetlist[o].Facelist[nfac-1]);
                    if (nb_normp_fac == n) {            // Pas sûr que ce soit le bon test !!!
//                        this->str.clear();
//                        this->N_elements = nfac;
//                        this->Set_numeros(Numeros);
//                        this->make1luminance();
                        this->make_1_luminance(nfac,Numeros);
                        Face_ij->flat = false;              // Facette non plane
                        this->Objetlist[o].flat = false;    // donc l'objet aussi
                    } else {
                        Face_ij->flat = true;               // Facette plane
                    }
                    Face_ij->groupe     = num_mat;
                    Face_ij->codmatface = num_mat;
                    continue;
                }

//                Update_Dialog(ftello64(f), fichierBdd_length); // pas efficace ici à cause des continue
            }
//            nfac_t += nfac;     // Dernière mise à jour ici, car en dehors de la boucle des objets !

            Fermer_progress_dialog();

// Et maintenant utiliser les tableaux de sommets et vecteurs de l'objet numéro 0
// ATTENTION : ainsi, on fait des copies. Il faudrait plutôt, à ce niveau, pointer sur les tableaux/vectors de l'objet 0
//             pour éviter de dupliquer ces (gros) tableaux.
//             Si lecture optimisée des fichiers .obj, on éliminera ces sommets/vecteurs par la suite.
            Object *PremierObjet = &(this->Objetlist[indice_premierObjet]);
            Object *objet_courant;
            bool msg_optim = true;

            for (o=1+indice_premierObjet; o<Nb_objets+indice_premierObjet; o++) {   // On ne commence que sur le 2ème objet les copies

                objet_courant= &(this->Objetlist[o]);
                objet_courant->Sommetlist  = PremierObjet->Sommetlist ;
                objet_courant->Vecteurlist = PremierObjet->Vecteurlist;

                if (lect_obj_opt) { // Suite inutile c'est forcément le cas ... && !Forcer_1_Seul_Objet && (Nb_objets > 1)) {
                    // Le but est d'éliminer dans chaque objet tous les points et vecteurs inutiles et renuméroter en conséquence
                    // les numéros de sommets utilisés dans les facettes et les luminances
                    // Lancer après coup une simplification de Bdd peut aussi faire le job
                    Optimiser_Obj_Sommets (objet_courant, o, msg_optim);
                    Optimiser_Obj_Vecteurs(objet_courant, o);
                    if (objet_courant->Nb_facettes == 0) {
                        objet_courant->deleted = true;
                        objet_courant->Sommetlist.clear();        // Réinitialiser les vecteurs pour libérer de la mémoire
                        objet_courant->Vecteurlist.clear();
                        objet_courant->Nb_sommets = objet_courant->Nb_vecteurs = 0;
                    }
                }
            }

            for (o=indice_premierObjet; o<Nb_objets+indice_premierObjet; o++) {
                for (i=0; i<this->Objetlist[o].Nb_facettes; i++) Calcul_Normale_Barycentre(o,i);
            }

            // Optimisation du fichier .obj à la lecture : élimination des sommets et vecteurs non utilisés
            // Cas particulier du 1er objet, celui contenant la liste des sommets et vecteurs de référence

            if (lect_obj_opt && !Forcer_1_Seul_Objet && (Nb_objets > 1)) {

                for (o=indice_premierObjet; o<=indice_premierObjet; o++) { // En fait, on ne passe qu'une seule fois dans cette boucle
                    objet_courant = &(this->Objetlist[o]);
                    Optimiser_Obj_Sommets (objet_courant, o, msg_optim);
                    Optimiser_Obj_Vecteurs(objet_courant, o);
                }

                if (PremierObjet->Nb_facettes == 0) {   // Cet objet est en fait vide, pas de facettes, seulement des sommets et des vecteurs
                    // éliminer les sommets et vecteurs du 1er objet (objet 0 si 1 seule Bdd) car ne servira plus.
                    PremierObjet->Sommetlist.resize(0);
                    PremierObjet->Vecteurlist.resize(0);
                    PremierObjet->Nb_sommets = PremierObjet->Nb_vecteurs = 0;
                    PremierObjet->deleted    = true;    // Ceinture et bretelles !!!! :-)
                }
            }

//****************************************
//            if (svg_entete && entete != nullptr) fclose(entete) ;
//            init_liste_Materiaux(cel) ;
            m_loaded = true;
            m_gllist = 0;
            return ;
        }
        printf("Fichier .obj non conforme Wavefront\n") ;
        type_fichier = -1;
    }
    fclose(f);

    Fermer_progress_dialog();           // au cas où ...

    if(verbose) printf("Sortie de BddInter::LoadOBJ\n");
}

//******************************************************************************

// Pour fichiers Milkshape 3D Ascii

void BddInter::LoadM3D()
{
/*
 *  Lecture d'un Fichier de polygones au format MilkShape 3D Ascii
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  Une création de groupes faite ?????????????????????????????????
 */

    Object *Objet_courant;
    char nom_obj[80];
    int sommet[3] ;
    int normal[3] ;
    char *cptr ;
    unsigned int n,norm ;
    unsigned int i, nfac, npoint, nb_fac, nb_p ;
    unsigned int NbVecteurs;
    unsigned int o=0;		            // pour compter le nombre d'objets
    unsigned int Nb_objets;
    float vx,vy,vz;
    std::vector<int> Numeros;
    bool test_flat;

    if(verbose) printf("Entree de BddInter::LoadM3D\n");

    wxCharBuffer buffer=this->file.mb_str();

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f = fopen(buffer.data(),"r");   //ouverture du fichier

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    fgets(s1,160,f);
    if (!strncmp(s1+3,"MilkShape 3D ASCII",18)) {
        printf("Fichier de type : MilkShape 3D en Ascii\n") ;
    } else {
        printf("Fichier .m3d mais pas reconnu comme de type MilkShape 3D en Ascii\n") ;
        type_fichier = -1;
        fclose(f);
        return;
    }

    Numeros.resize(3); // Toujours 3 points dans ce format

//    fgets(s1,160,f);

    for (i=2; i<=6 ; i++) {
        fgets(s1,100,f) ;
    }
    if (strncmp(s1,"Meshes:",7)) {
        sprintf(Message,"Erreur dans le fichier : chaîne \"Meshes\" non trouvée\n") ;
        printf("%s",utf8_To_ibm(Message)) ;
//        printf("Erreur dans le fichier : chaine \"Meshes\" non trouvee\n") ;
#ifdef WIN32
        system("pause") ;
#endif
        type_fichier = -1;
        exit(2) ;
    }
    sscanf(s1+7,"%d",&Nb_objets);
    printf("%d objets\n",Nb_objets);

//#ifdef WIN32
//        system("pause") ;
//#endif
//        exit(9);
//    }
//    if (svg_entete && temp != nullptr) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Fichier MilkShape 3D Ascii ####\n\n") ;
//    }

    for (o=0; o< Nb_objets; o++) {
        fgets(s1,100,f) ;
        strcpy(nom_obj, Lire_chaine(s1)+1) ;        //+1 pour enlever le " de début
        cptr = strchr(nom_obj,'\0') ;               // Pointe la fin de chaîne
        while(*--cptr != '"') {*cptr = '\0' ;}      // élimine la fin de chaîne.
        *cptr = '\0' ;                              // Puis éliminer ce " de fin
        printf("Objet :%4d, Nom : %s\n",o,nom_obj);

#if wxCHECK_VERSION(3,0,0)
        wxString wxNom=wxString(nom_obj);
#else
        wxString wxNom=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        this->str.Printf(_T("<OBJET> %d "),o);
        this->str += wxNom;
        this->make_objet();
//        indiceObjet_courant = this->Objetlist.size()-1;//o;

        Objet_courant = &(this->Objetlist[indiceObjet_courant]);

        // Lecture du nombre de points
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&nb_p) ;

        this->str.clear();
        this->N_elements = nb_p;

        this->make_sommet();
        // Lectures des points
        for (npoint = 1; npoint <= nb_p; npoint++) {
            fgets(s1,200,f) ;
            // Lecture des x, y et z
            sscanf(s1,"%d%f%f%f", &i, &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make_1_sommet();
        }

        Update_Dialog((long long)ftello64(f), fichierBdd_length);

        // Lecture du nombre de Normales aux sommets
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&NbVecteurs) ;

        this->N_elements = NbVecteurs;
        this->make_vecteur();

        // Lectures des normales
        for (npoint = 1; npoint <= NbVecteurs; npoint++) {
            fgets(s1,200,f) ;
            // Lecture des x, y et z
            sscanf(s1,"%f%f%f", &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make_1_vecteur();
        }

        Update_Dialog((long long)ftello64(f), fichierBdd_length);

        // Lecture du nombre de facettes
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&nb_fac) ;
        this->N_elements = nb_fac;
        Objet_courant->Nb_facettes = nb_fac;

        this->make_face();
        this->make_normale();
        this->make_luminance();
        this->make_aspect_face();

        // Lecture des facettes
        for (nfac=1; nfac <= nb_fac; nfac++) {
            this->str.clear();
            fgets(s1,200,f) ;
            for (i=0; i<3; i++) normal[i] = 0;
            sscanf(s1,"%d%d%d%d%d%d%d", &norm, &sommet[0], &sommet[1],&sommet[2], &normal[0], &normal[1] , &normal[2]) ;

            for (i=0; i<3; i++) Numeros[i] = sommet[i]+1;// Ajouter 1 au n°, car on commence à 1 et non 0
//            this->N_elements = nfac;
//            this->Set_numeros(Numeros);
//            this->make_1_face();
            this->make_1_face(nfac,Numeros);

            test_flat = false;
            for (i=0; i<3; i++) {
                n          = normal[i]+1;
                Numeros[i] = n;
                if (n > NbVecteurs) test_flat = true ; // incompatibilité de numéro => forcer cette facette à être plane
            }
//            this->Set_numeros(Numeros);
//            this->make_1_luminance();
            this->make_1_luminance(nfac,Numeros);
            Objet_courant->Facelist[nfac-1].flat = test_flat; // Facette non plane si le numéro de vecteurs est compatible

            Calcul_Normale_Barycentre(indiceObjet_courant,nfac-1);

            Objet_courant->Facelist[nfac-1].groupe     = groupe_def;
            Objet_courant->Facelist[nfac-1].codmatface = codmatface_def;

            Update_Dialog((long long)ftello64(f), fichierBdd_length);
        }
        Objet_courant->Nb_facettes  = nb_fac;
        Objet_courant->Nb_normales  = nb_fac;
        Objet_courant->Nb_luminances= nb_fac;
        Objet_courant->flat         = false;    // Objet a priori avec normales aux sommets, mais peut être invalidé facette/facette
    }
    m_loaded = true;
    m_gllist = 0;
    fclose(f);

    Fermer_progress_dialog();

    sprintf(Message,"\nFin de la lecture des données.\n");
    printf("%s",utf8_To_ibm(Message));
    if(verbose) printf("Sortie de BddInter::LoadM3D\n");
}

//******************************************************************************

void BddInter::buildGroupes(unsigned int &nb_groupes, int &index_grp, bool &i_warn, int materiau)
{
// Construit un tableau de numéros de groupes à partir de différentes valeurs de matériaux. Utilisé par LoadPLY et RenderDXF
// Les numéros uitilisés seront en fait l'indice dans le tableau (index_grp)

    int i0;

// Différentiation des groupes
    if (nb_groupes == 0) {
        nb_groupes = 1 ;
        index_grp  = 1 ;
        Groupe_num[1] = materiau ;
    } else {
        i0 = 0 ;
        for (unsigned int i=1; i <= nb_groupes; i++) {
            if (materiau == Groupe_num[i]) {
                i0 = 1 ;
                index_grp = i ;
            }
        }
        if (!i0) {
            if (nb_groupes == maxGroupes) {
                if (!i_warn) {
//                        printf("Warning : Nombre de valeurs ""materiau"" differentes > %d\n",maxGroupes) ;
                    sprintf(Message,"Warning : Nombre de valeurs ""materiau"" différentes > %d\n",maxGroupes) ;
                    printf("%s",utf8_To_ibm(Message)) ;
                    i_warn = true ;
                }
            } else nb_groupes++;
            index_grp = nb_groupes ;
            Groupe_num[nb_groupes] = materiau ;
        }
    }
}

// Pour fichiers Niratam polygon file ply
void BddInter::LoadPLY()
{
/*
*  Lecture d'un Polygon file, format Niratam - GEO
*  Identifie éventuellement plusieurs objets, crée des normales.
*  Une création de groupes faite (d'après les infos de materi)
*/

    char ident[80] ;			//chaînes de caractères  s1[666],
    char nom_obj[80], nom_prec[80] ;
    char   *cptr = nullptr ;
    Object *Objet_courant;
    int n_OK, ittem ;
    unsigned int i, nfac, npoint, nb_fac, nb_p, n, indice_premierObjet ;
    int materi ;
    int index_grp= 0;
    bool i_warn  = false ;
    int id1, id2, id3 ;
    int o=0;		            // pour compter le nombre d'objets

    int o_dim=51, o_inc=10 ;
    int *o_nfac, *o_npoint ;
    int code_facette=0;     //flag pour tester s'il y a des facettes dégénérées (< 3 points)

    float vx, vy, vz;

    std::vector<int> Numeros_Sommets;

    if(verbose) printf("Entree de BddInter::LoadPLY\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f = fopen(buffer.data(),"r");	//ouverture du fichier

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    fgets(s1,160,f);
    if (strncmp(s1,"GEO",3)) {
        if (strncmp(s1,"ply",3)) {
            printf("Fichier .ply mais pas de type GEO Niratam, ni Stanford Polygon file !\n");
            fclose(f);
            type_fichier = -1;
            return ;
        }
        fclose(f);
        this->LoadPLY_Stanford();
        return;
    }

    printf("Fichier GEO - Niratam : %s",s1) ;
    unsigned int nb_groupes = 0 ;

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    for (i=0; i <= 4; i++) {
        fgets(s1,100,f) ;    // Le nom de l'avion est à la seconde ligne, donc pour i=0
        printf("%s",s1) ;
        if (i == 0) strcpy(ident,s1) ;
        if ((cptr=strstr(s1,"ITTEM =")) != nullptr) break ;
        if (i == 4) {
            sprintf(Message,"Erreur dans le Polygon File : chaîne \"ITTEM =\" non trouvée\n") ;
            printf("%s",utf8_To_ibm(Message)) ;
//            printf("Erreur dans le Polygon File : chaine \"ITTEM =\" non trouvee\n") ;
#ifdef WIN32
            system("pause") ;
#endif
            exit(2) ;
        }
    }
    sscanf(cptr+7,"%d",&ittem) ;
    for (i=0; i <= 10; i++) {
        fgets(s1,100,f)   ;
        printf("%s",s1) ;
        if ((cptr=strstr(s1,"Start")) != nullptr) break ;
        if (i == 10) {
            sprintf(Message,"Erreur dans le Polygon File : chaîne \"Start\" non trouvée\n") ;
            printf("%s",utf8_To_ibm(Message)) ;
//            printf("Erreur dans le Polygon File : chaine \"Start\" non trouvee\n") ;
#ifdef WIN32
            system("pause") ;
#endif
            exit(2) ;
        }
    }
//  Comptage des facettes, des objets et des points...
    fscanf(f,"%d",&n) ;
    nfac     = 0 ;
    npoint   = 0 ;
    nb_fac   = 0 ;
    nb_p     = 0 ;
    o_nfac   = (int *)malloc(o_dim*sizeof(int)) ;     // Pour conserver les valeurs nfac et npoint lues
    o_npoint = (int *)malloc(o_dim*sizeof(int)) ;
    strcpy(nom_prec,"testxxx") ;
    while (n != 0) {  // La fin du fichier est atteinte quand on lit n à 0
        fgets(s1,100,f) ;
        fgets(s1,100,f) ;
        fscanf(f,"%d", &i);
        fgets( s1,100,f)  ;

        strcpy(nom_obj, Lire_chaine(s1)) ;
        n = (n-2)/3 ;
        if (strstr(nom_obj,nom_prec) == nullptr) {
            if (nb_fac != 0) {
                printf(" avec %5d facettes et %5d points\n", nfac, npoint) ;
                if (o == o_dim) {
                    o_dim   += o_inc ;
                    o_nfac   = (int *)realloc(o_nfac,  o_dim*sizeof(int)) ; // Tableaux initiaux trops petits
                    o_npoint = (int *)realloc(o_npoint,o_dim*sizeof(int)) ; // Les agrandir....
                }
                o_nfac[o]   = nfac ;
                o_npoint[o] = npoint ;
            }
            o++ ;
            printf("%-30s",nom_obj) ;
            strcpy(nom_prec, nom_obj) ;
            npoint = 0 ;
            nfac   = 0 ;
        }
        for (i=1; i <= n; i++) fgets(s1,100,f) ;
        npoint = npoint + n ;
        nb_p   = nb_p + n ;
        nfac ++ ;
        nb_fac++;

        fscanf(f,"%d",&n) ;
    }
    printf(" avec %5d facettes et %5d points\n", nfac, npoint) ;
    if (o == o_dim) {
        o_dim += o_inc ;
        o_nfac   = (int *)realloc(o_nfac,  o_dim*sizeof(int)) ;
        o_npoint = (int *)realloc(o_npoint,o_dim*sizeof(int)) ;
    }
    o_nfac[o]   = nfac ;
    o_npoint[o] = npoint ;
    printf("Total :\n%8d Objets\n%8d Facettes\n%8d Points\n",o,nb_fac,nb_p) ;

// Ici on constate que pour certains fichiers, un même nom d'objet apparaît
// plusieurs fois a différents endroits (faut-il les regrouper ?)

    /* retour au debut du fichier pour commencer la lecture des données */
    rewind(f);

    /* Lire jusqu'à Start */
    do {
        fgets(s1,100,f) ;
    } while (strstr(s1,"Start") == nullptr) ;
// printf("%s",s1) ;
//    if (svg_entete && temp != nullptr ) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Polygon File Niratam - GEO ####\n\n") ;
//        fprintf(temp,"#### %s\n\n", ident) ;
//    }

// On est au début des polygones/facettes

    int Nb_objets = o;
    o=-1 ;
    indice_premierObjet = this->Objetlist.size();   // peut-être != 0 en cas de fusions d'objets

    strcpy(nom_prec,"test_abcd_xxyyzz") ; // Nom bidon, initialisation...

    fscanf(f,"%d",&n) ;
    while (n != 0) {  // La fin du fichier est atteinte quand on lit n à 0
        if (ittem == 5) {
            fgets(s1,100,f) ;   // Passer le cr
            fgets(s1,100,f) ;   // Lire la ligne
//          printf("%s",s1) ;
            materi = 0 ;
//       s1 = toupper(s1) ;
            if (!strncasecmp(s1,"  SKN",5)) materi = 1 ; // On pourrait différentier en analysant
            if (!strncasecmp(s1,"  ITO",5)) materi = 2 ; // la chaîne suivante sur la ligne
            if (!strncasecmp(s1,"  ITI",5)) materi = 3 ; // Genre Paint-A, Paint-B,...
            if (!strncasecmp(s1,"  JCV",5)) materi = 4 ;
        } else {
            fscanf(f,"%d",&materi) ; // Décripter materi pour obtenir des groupes ...
            fgets(s1,100,f) ;   // Passer le cr
        }

        Update_Dialog((long long)ftello64(f), fichierBdd_length);

        buildGroupes(nb_groupes, index_grp, i_warn, materi);

        if(materi == 0) {
            // A faire
            printf("Warning : Fichier .ply : cas materi = 0 a coder ?\n");
        }

//      fgets(s1,100,f)     ; // Lire la ligne (contient un numéro et un identifiant d'objet)
//      sscanf(s1,"%d", &i) ; //

        fscanf(f,"%d", &i);     // Lire un numéro (serait à comparer à nfac)
        fgets( s1,100,f)  ;     // et un identifiant d'objet)
        strcpy(nom_obj, Lire_chaine(s1)) ;

        if (strstr(nom_obj,nom_prec) == nullptr) {
            o++ ;                                   // On change d'objet dès que l'identifiant change
            strcpy(nom_prec, nom_obj) ;
            str.Printf(_T("<OBJET> %d "),o);

#if wxCHECK_VERSION(3,0,0)
            wxString wxNom=wxString(nom_obj);
#else
            wxString wxNom=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

            str += wxNom;
            make_objet();
//            indiceObjet_courant = this->Objetlist.size()-1; //o;
            Objet_courant = &(this->Objetlist[indiceObjet_courant]);
            Objet_courant->Nb_facettes = o_nfac[o+1];
            Objet_courant->Nb_sommets  = o_npoint[o+1];

            this->N_elements = Objet_courant->Nb_facettes;
            str.clear();
            make_face();
            make_normale();
//            make_aspect_face();

            this->N_elements = Objet_courant->Nb_sommets;
            make_sommet();

            nfac   = 0;
            npoint = 0;
        }
        ++nfac ;
        n = (n-2)/3 ; // Nombre de sommets à lire
        if (n < 3) code_facette = 1 ;

        Objet_courant = &(this->Objetlist[indiceObjet_courant]);    // Par précaution, car c'est normalement déjà initialisé

        Numeros_Sommets.resize(n);
        this->str.clear();
        for (i=0; i < n; i++) {
            ++npoint ;                // Il serait intéressant de vérifier ici si le point n'existe pas déjà (limiter les doublons)
            fscanf(f,"%f",&vx);		// Lecture du x du point
            fscanf(f,"%f",&vy);		// Lecture du y du point
            fscanf(f,"%f",&vz);		// Lecture du z du point
            this->Setxyz(vx,vy,vz);
            this->N_elements = npoint;
            this->make_1_sommet();
            Numeros_Sommets[i] = npoint;
        }
        N_elements = nfac;
//        this->Set_numeros(Numeros_Sommets);
//        make_1_face();
        this->make_1_face(nfac, Numeros_Sommets);
        Objet_courant->Facelist[nfac-1].flat = true;  // provisoire
        this->Setxyz(1.,0.,0.); // Provisoire
        make_1_normale();

        Objet_courant->Facelist[nfac-1].groupe     = index_grp;
        Objet_courant->Facelist[nfac-1].codmatface = index_grp;

        fscanf(f,"%d",&n) ;

// Vérifications de précaution : des fois que ...
        if (npoint > nb_p) {
            n = 0 ;
            printf("Warning : Trop de points dans le fichier ( > %d)\n",npoint) ;
        }
        if (nfac > nb_fac) {
            n = 0 ;
            printf("Warning : Trop de facettes dans le fichier ( > %d)\n",nfac) ;
        }

        Update_Dialog((long long)ftello64(f), fichierBdd_length);
    }

    Fermer_progress_dialog();

    for (o=0; o<Nb_objets; o++) {
        indiceObjet_courant = indice_premierObjet + o;
        Objet_courant = &(this->Objetlist[indiceObjet_courant]);
        for (i=0; i < Objet_courant->Facelist.size(); i++) Calcul_Normale_Barycentre(indiceObjet_courant,i);
    }
    free(o_nfac)   ; // Libérer la mémoire obtenue par malloc/realloc ...
    sprintf(Message,"Nombre de groupes identifiés : %d\nIndice Materi\n",nb_groupes) ;
    printf("%s",utf8_To_ibm(Message)) ;

    for (i=1; i<=nb_groupes; i++) printf("  %2d %8d\n",i,Groupe_num[i]) ;
    free(o_npoint) ;

    m_loaded = true;
    m_gllist = 0;

    if (code_facette == 1) {
        printf("La bdd contient des facettes de 2 points ou moins !!\n") ;
        printf("Utiliser la Simplification de Bdd sinon risques de plantage\n");
        printf("avec certaines cartes graphiques!\n");
    }
    sprintf(Message,"\nFin de la lecture des données.\n");
    printf("%s",utf8_To_ibm(Message));

//******************************************************************************

    fclose(f);

    if(verbose) printf("Sortie de BddInter::LoadPLY\n");
}

//float float_swap(float value){
//       int temp =  __builtin_bswap32(*(unsigned int*)&value);
//       return *(float*)&temp;
//};

// Pour fichiers ply Stanford

void BddInter::LoadPLY_Stanford()
{
/*
 *  Lecture d'un Fichier de polygones au format PLY de Stanford
 *  Un seul objet, crée les normales aux barycentres, lecture éventuelle des normales aux sommets (si présentes)
 *
 *  Largement inspiré/copié de headply et ply2iv, des programmes écrits par Greg Turk (auteur original du format ply).
 *  headply.c : Read the header from a PLY file and print it out
 *  ply2iv.c  : Convert a PLY file to an Inventor file.
 *
 *  Greg Turk

 * -----------------------------------------------------------------------

 *  Copyright (c) 2020, Georgia Institute of Technology
 *  All rights reserved.

 */

    int compteur = 0;
    int i,j,k;
    int elem_count;

    char *buf;
    char *elem_name;
    PlyFile *in_ply;

    std::vector<int> Numeros;
    Face   *facette_courante;
    Object *objet_courant;

//    bool verbose = true;  // Version locale de verbose

/* vertex and face definitions for a polygonal object */

    typedef struct PlyVertex {
        float x,y,z;
        float r,g,b;
        float nx,ny,nz;
        void *other_props;       /* other properties */
    } PlyVertex;

    typedef struct PlyFace {
        unsigned char nverts;    /* number of vertex indices in list */
//        unsigned int nverts;    /* number of vertex indices in list */
        int *verts;              /* vertex index list */
        void *other_props;       /* other properties */
    } PlyFace;

        typedef struct PlyTris {
        unsigned int npts;    /* number of vertex indices in list */
        int *verts;              /* vertex index list */
        void *other_props;       /* other properties */
    } PlyTris;

    const char *elem_names[] = { /* list of the elements in the object */
        "vertex", "face", "tristrips"
    };

    PlyProperty vert_props[] = { /* list of property information for a vertex */
        {(char*)"x",  Float32, Float32, offsetof(PlyVertex,x),  0, 0, 0, 0},
        {(char*)"y",  Float32, Float32, offsetof(PlyVertex,y),  0, 0, 0, 0},
        {(char*)"z",  Float32, Float32, offsetof(PlyVertex,z),  0, 0, 0, 0},
        {(char*)"r",  Float32, Float32, offsetof(PlyVertex,r),  0, 0, 0, 0},
        {(char*)"g",  Float32, Float32, offsetof(PlyVertex,g),  0, 0, 0, 0},
        {(char*)"b",  Float32, Float32, offsetof(PlyVertex,b),  0, 0, 0, 0},
        {(char*)"nx", Float32, Float32, offsetof(PlyVertex,nx), 0, 0, 0, 0},
        {(char*)"ny", Float32, Float32, offsetof(PlyVertex,ny), 0, 0, 0, 0},
        {(char*)"nz", Float32, Float32, offsetof(PlyVertex,nz), 0, 0, 0, 0},
    };

    PlyProperty face_props[] = { /* list of property information for a face */
        {(char*)"vertex_indices", Int32, Int32, offsetof(PlyFace,verts), 1, Uint8, Uint8, offsetof(PlyFace,nverts)},
        {(char*)"vertex_index"  , Int32, Int32, offsetof(PlyFace,verts), 1, Uint8, Uint8, offsetof(PlyFace,nverts)},
    };

    PlyProperty tris_props[] = { /* list of property information for a tristrips */
        {(char*)"vertex_indices", Int32, Int32, offsetof(PlyTris,verts), 1, Uint32, Uint32, offsetof(PlyTris,npts)},
        {(char*)"vertex_index"  , Int32, Int32, offsetof(PlyTris,verts), 1, Uint32, Uint32, offsetof(PlyTris,npts)},
    };

// En fait, "vertex_indices" et "vertex_index" sont synonymes. La norme actuelle dit "vertex_indices", mais l'autre forme semble exister dans d'anciens fichiers.

    /*** the PLY object ***/

    int nverts,nfaces;
    PlyVertex *vlist;
    PlyFace   *flist;
    PlyTris   *tlist;

    PlyOtherProp *vert_other,*face_other;

    int per_vertex_color = 0;
    int has_normals = 0;

    int nelems, file_type;
    float version;
//    char **liste;

    if(verbose)
        printf("Entrée de BddInter::LoadPLY_Stanford\n");
    printf("... En cours d'implémentation ...\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f = fopen(buffer.data(),"r");	    //ouverture du fichier

// Lecture et affichage du header

// Ci-dessous, garder les fgets ... pour détection de problèmes de header, mais on peut mettre en commentaire les 2 printf suivants (ou via if(verbose)...)

    fgets(s1,160,f);
    printf ("%s", s1);                  // le \n est déjà dans s1

    while (1) {
        buf = fgets(s1,160,f);
        if (buf == nullptr) {
            fprintf (stderr, "Fin de fichier atteinte ... Abandon de la lecture !\n");
            type_fichier = -1;
            break;
        }
        printf ("%s", s1);              // le \n est déjà dans s1

        if (strncmp(s1, "comment", 7)) compteur++;  // Ne pas compter les lignes "comment"

        if (compteur > 50) {
            fprintf (stderr, "Le Header semble trop long ... Abandon de la lecture !\n");
            type_fichier = -1;
            break;
        }

        if (strncmp(s1, "end_header", 10) == 0)
            break;
    }

    if (type_fichier < 0) {
        fclose(f);
        printf("Sortie en Erreur de BddInter::LoadPLY_Stanford\n");
        return;
    }

    rewind(f);  // Repositionner le fichier à son début

    /*** Read in the original PLY object ***/
    in_ply  = read_ply (f);                 // Lit l'entête et prépare les structures et tableaux de données

    if (in_ply->file_type != PLY_ASCII) {   // C'est un fichier binaire (Little Endian ou Big Endian) => le réouvrir autrement
        close_ply(in_ply);
        free_ply (in_ply) ;
        f = fopen(buffer.data(),"rb");
        in_ply  = read_ply (f);
    }

    if (in_ply == nullptr) {
        type_fichier = -1;
        printf("Sortie en Erreur de BddInter::LoadPLY_Stanford\n");
        return;
    }

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    for (i = 0; i < in_ply->num_elem_types; i++) {
        elem_name = setup_element_read_ply (in_ply, i, &elem_count);
    }

    // Créer un seul objet

    str.Printf(_T("<OBJET> %d "),0+Numero_base);
    str += wxFileName(buffer.data()).GetName();     // Récupérer le nom du fichier sans l'extension .ply ni le path comme nom d'objet
    make_objet();

    objet_courant = &(this->Objetlist[indiceObjet_courant]);

    if (verbose) printf("num_elem_types %d\n",in_ply->num_elem_types);

    for (i = 0; i < in_ply->num_elem_types; i++) {

        /* prepare to read the i'th list of elements */
        elem_name = setup_element_read_ply (in_ply, i, &elem_count);

        if (equal_strings ((char*)"vertex", elem_name)) {

            /* create a vertex list to hold all the vertices */
            nverts = elem_count;

            if(verbose) {
                printf("nprops         %d\n",in_ply->elems[i]->nprops);
                printf("nverts         %d\n",nverts);
            }

            // Créer les sommets

            objet_courant->Nb_sommets = nverts;
            this->N_elements = nverts;
            str.clear();
            make_sommet();
            if (has_normals) make_vecteur();

            /* set up for getting vertex elements */

            setup_property_ply (in_ply, &vert_props[0]);
            setup_property_ply (in_ply, &vert_props[1]);
            setup_property_ply (in_ply, &vert_props[2]);

            for (j = 0; j < in_ply->elems[i]->nprops; j++) {
                PlyProperty *prop;
                prop = in_ply->elems[i]->props[j];
                if (equal_strings ((char*)"r", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[3]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"g", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[4]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"b", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[5]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"nx", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[6]);
                    has_normals = 1;
                }
                if (equal_strings ((char*)"ny", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[7]);
                    has_normals = 1;
                }
                if (equal_strings ((char*)"nz", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[8]);
                    has_normals = 1;
                }
            }

            vert_other = get_other_properties_ply (in_ply, offsetof(PlyVertex, other_props));

            /* grab all the vertex elements */

            for (j = 0; j < elem_count; j++) {
                vlist = (PlyVertex *) malloc (sizeof (PlyVertex));
                vlist->r = 1;
                vlist->g = 1;
                vlist->b = 1;
                get_element_ply (in_ply, (void *) vlist);

                this->N_elements = j+1;
                this->Setxyz(vlist->x, vlist->y, vlist->z);
                make_1_sommet();
                if (has_normals) {
                    this->Setxyz(vlist->nx, vlist->ny, vlist->nz);
                    make_1_vecteur();
                }
                free(vlist); // Libérer la mémoire de chaque vertex

                Update_Dialog((long long)ftello64(f), fichierBdd_length);     // mieux car utilise la taille réelle et la position
            }

            if (vert_other == nullptr || face_other == nullptr) {
                // Ignorer ? faire quelque-chose ? Permet d'éviter un warning de compilation sur "variable set but not used" !
            }
            if (per_vertex_color) {
                // colorisier un groupe peut-être, mais comme c'est par sommet et non par facettes ??? permet d'éviter un warning de compilation
            }

        } else if (equal_strings ((char*)"face", elem_name)) {

            nfaces = elem_count;

            if (verbose) printf("nfaces         %d\n",nfaces);

            /* set up for getting face elements */

            // Créer les facettes

            this->N_elements = nfaces;
            make_face();
            make_normale();
            make_aspect_face();

            if (has_normals) {
                make_luminance();
                objet_courant->flat = false;
        //        objet_courant->Nb_luminances= nfaces; // déjà fait via make_luminance
        //        objet_courant->Nb_vecteurs  = nverts; //  ""       via make_vecteur
            } else {
                objet_courant->flat = true;
                objet_courant->Nb_luminances= 0;
                objet_courant->Nb_vecteurs  = 0;
            }

            str.clear();

            PlyProperty *propE;
            propE = in_ply->elems[i]->props[0];
            if (equal_strings ((char*)"vertex_indices", propE->name)) {
                setup_property_ply (in_ply, &face_props[0]);
            } else {
//            if (equal_strings ((char*)"vertex_index", propE->name)) { // vertex_indices et vertex_index (version ply antérieure ?) sont en fait synonymes
                setup_property_ply (in_ply, &face_props[1]);
            }
            face_other = get_other_properties_ply (in_ply, offsetof(PlyFace, other_props));

            /* grab all the face elements */
            for (j = 0; j < elem_count; j++) {
                flist = (PlyFace *) malloc (sizeof (PlyFace));
                get_element_ply (in_ply, (void *) flist);

                int nb_som = flist->nverts;
                Numeros.resize(nb_som);
                for (k=0; k<nb_som; k++) Numeros[k]=(flist->verts[k]) +1;
//                this->N_elements = j+1;
//                this->Set_numeros(Numeros) ;
//                make_1_face();
                make_1_face(j+1,Numeros);
                facette_courante = &(objet_courant->Facelist[j]);
                facette_courante->groupe     = groupe_def;
                facette_courante->codmatface = codmatface_def;
                if (has_normals) {
//                    make_1_luminance();
                    make_1_luminance(j+1,Numeros);
                    facette_courante->flat = false;
                } else {
                    facette_courante->flat = true;
                }
                Calcul_Normale_Barycentre(indiceObjet_courant,j);
                free(flist);     // Libérer la mémoire de chaque Ply facettes (malloc de flist)

                Update_Dialog((long long)ftello64(f), fichierBdd_length);
            }

        } else if (equal_strings ((char*)"tristrips", elem_name)) {

            PlyProperty *propE;
            propE = in_ply->elems[i]->props[0];
            if (equal_strings ((char*)"vertex_indices", propE->name)) {
                setup_property_ply (in_ply, &tris_props[0]);
            } else {
//            if (equal_strings ((char*)"vertex_index", propE->name)) { // vertex_indices et vertex_index (version ply antérieure ?) sont en fait synonymes
                setup_property_ply (in_ply, &tris_props[1]);
            }

            for (j = 0; j < elem_count; j++) {
                tlist = (PlyTris *) malloc (sizeof (PlyTris));
                get_element_ply (in_ply, (void *) tlist);

                int nb_som = tlist->npts;
                printf("nb sommets tristrips = %d\n",nb_som);
                nfaces = nb_som-2;                              // provisoire et surdimensionné !
                printf("nfaces provisoire    = %d\n",nfaces);

                this->N_elements = nfaces;
                make_face();
                make_normale();
                make_aspect_face();

                if (has_normals) {
                    make_luminance();
                    objet_courant->flat = false;
                } else {
                    objet_courant->flat = true;
                    objet_courant->Nb_luminances= 0;
                    objet_courant->Nb_vecteurs  = 0;
                }

                str.clear();

                Numeros.resize(3);  // Seulement des triangles
                // Algorithme issu de https://github.com/cnr-isti-vclab/vcglib/blob/master/wrap/io_trimesh/import_ply.h
                int remainder=0;
                int k,kpts;
                kpts   = nb_som-2;
                nfaces = 0;
                for (k=0; k<kpts; ++k) {
                    if (tlist->verts[k+2] == -1) {
                        k+=2;
						if(k%2) remainder=0;
						else    remainder=1;
						continue;
                    }
                    if((k+remainder)%2) {
                        Numeros[0] = tlist->verts[k+1] +1;  // <=> swap 0 <-> 1
                        Numeros[1] = tlist->verts[k+0] +1;
                    } else {
                        Numeros[0] = tlist->verts[k+0] +1;
                        Numeros[1] = tlist->verts[k+1] +1;
                    }
                    Numeros[2] = tlist->verts[k+2] +1;

//                    this->N_elements = nfaces+1;
//                    this->Set_numeros(Numeros) ;
//                    make_1_face();
                    make_1_face(nfaces+1,Numeros);
                    facette_courante = &(objet_courant->Facelist[nfaces]);
                    facette_courante->groupe     = groupe_def;
                    facette_courante->codmatface = codmatface_def;
                    if (has_normals) {
//                        make_1_luminance();
                        make_1_luminance(nfaces+1,Numeros);
                        facette_courante->flat = false;
                    } else {
                        facette_courante->flat = true;
                    }
                    Calcul_Normale_Barycentre(indiceObjet_courant,nfaces);
                    nfaces++;
                }
                printf("nfaces réel          = %d\n",nfaces);
                objet_courant->Facelist.resize(nfaces); // Ajuster à la taille réelle
                objet_courant->Nb_facettes = objet_courant->Nb_normales = objet_courant->Nb_aspects = nfaces;
                if (has_normals) objet_courant->Nb_luminances = nfaces;
                printf("Liste des facettes redimmensionnée\n");
                free(tlist);     // Libérer la mémoire de chaque Ply tristrips (malloc de tlist)

                Update_Dialog((long long)ftello64(f), fichierBdd_length);
            }

        }/*else
            get_other_element_ply (in_ply); */  // Inutile de lire les autres propriétés (fin du fichier) mais du coup impose que vertex et face soient en 1 et/ou 2ème position
    }

    close_ply(in_ply);
    free_ply (in_ply) ;

//    type_fichier = -1;

    m_loaded = true;
    m_gllist = 0;

    Fermer_progress_dialog();

    if(verbose)
        printf("Sortie de BddInter::LoadPLY_Stanford\n");

}

// Pour fichiers .off

void BddInter::LoadOFF()
{
/*
 *  Lecture d'un Fichier de polygones au format OFF
 *  Un seul objet, crée les normales aux barycentres.
 */

    char nom_obj[80];
    int n;
    unsigned int i, nfac, npoint, nb_fac, nb_p, nsommets ;
    unsigned int o=0;		            // pour compter le nombre d'objets
    float vx,vy,vz;
    unsigned int Nb_objets;
    std::vector<int>   Numeros;
    std::vector<float> xyz_point;
    char   *cptr;        //Pointeur de chaîne de caractères
    Object *objet_courant;

    if(verbose) printf("Entree de BddInter::LoadOFF\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f=fopen(buffer.data(),"r");	//ouverture du fichier

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    fgets(s1,160,f) ;
    if (strncmp(s1,"OFF",3)) {
        printf("Fichier .off, mais pas de type Object File Format !\n");    // Doit obligatoirement commencer par OFF en 1ère ligne
        fclose(f);
        type_fichier = -1;
        return ;
    }

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    printf("Fichier de type Object File Format : %s",s1) ;
    Nb_objets = 1 ;     // A priori, 1 seul objet par fichier
//    if (svg_entete && temp != nullptr) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Fichier OFF ####\n\n") ;
//    }

    for (o=0; o< Nb_objets; o++) {
        sprintf(nom_obj, "Objet OFF - %d",o) ;

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),o);//+Numero_base);
        str += NomObj;
        make_objet();
//        indiceObjet_courant = this->Objetlist.size() -1; //o;
        objet_courant = &(this->Objetlist[indiceObjet_courant]);

        // Lecture du nombre de sommets et de facettes
        fgets(s1,100,f) ;
        while (s1[0] == '#') fgets(s1,100,f);   // Sauter les lignes commençant par # ... Il faudrait aussi sauter les lignes blanches (vides ou que des espaces)
        sscanf(s1,"%d%d",&nb_p,&nb_fac) ;

        objet_courant->Nb_sommets = nb_p;
        this->N_elements = nb_p;
        str.clear();
        make_sommet();

        // Lecture des points
        str.clear();
        for (npoint = 1; npoint <= nb_p; npoint++) {
            // Lecture des x, y et z
            fgets(s1,100,f) ;
            while (s1[0] == '#') fgets(s1,100,f);                           // élimine les lignes de commentaires
            cptr = strrchr(s1,'\n') ; if (cptr != nullptr) *cptr = '\0' ;   // Suppression des \n de fin de ligne
            cptr = strrchr(s1,'\r') ; if (cptr != nullptr) *cptr = '\0' ;   // Suppression des \r de fin de ligne
            while (strlen(s1) == 0) {                                       // élimine les lignes vides
                fgets(s1,100,f);
                while (s1[0] == '#') fgets(s1,100,f);
                cptr = strrchr(s1,'\n') ; if (cptr != nullptr) *cptr = '\0' ;
                cptr = strrchr(s1,'\r') ; if (cptr != nullptr) *cptr = '\0' ;
            }
            sscanf(s1,"%f%f%f", &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make_1_sommet();

            Update_Dialog((long long)ftello64(f), fichierBdd_length);
        }

        this->N_elements = nb_fac;
        make_face();
        make_normale();
        make_aspect_face();

        // Lecture des facettes
        str.clear();
        for (nfac=1; nfac <= nb_fac; nfac++) {
            fscanf(f,"%d",&nsommets);
            Numeros.resize(nsommets);
            for (i=0; i< nsommets; i++) {
                fscanf(f,"%d", &n) ;
                Numeros[i] = n+1; // Ajouter 1 au n°, car on commence à 1 et non 0
            }
//            str.clear();
//            this->N_elements = nfac;
//            this->Set_numeros(Numeros) ;
//            make_1_face();
            make_1_face(nfac,Numeros);
            objet_courant->Facelist[nfac-1].flat = true; // provisoire

            fgets(s1,200,f) ; //Terminer la lecture de cette ligne. Il reste parfois des données (normales ? autres ?)

            objet_courant->Facelist[nfac-1].groupe     = groupe_def;
            objet_courant->Facelist[nfac-1].codmatface = codmatface_def;

            Update_Dialog((long long)ftello64(f), fichierBdd_length);
        }
        objet_courant->Nb_facettes  = nb_fac;
        objet_courant->Nb_normales  = nb_fac;
        objet_courant->Nb_luminances= 0;        // Dans le format OFF, il n'y a pas de normales aux sommets
        objet_courant->Nb_vecteurs  = 0;        // enregistrées dans le fichier de Bdd
        objet_courant->flat=true;               // A la lecture, on considère que l'objet n'est composé que de facettes planes
        for (i=0; i<nb_fac; i++) Calcul_Normale_Barycentre(indiceObjet_courant,i);

        // Il n'y a pas de normales aux sommets dans ce format.
        // Le calcul sera fait éventuellement dans create_bdd si la case adéquate ("Calcul des normales dès la lecture") est cochée dans le dialogue "Préférences".

    }

    m_loaded = true;
    m_gllist = 0;

    fclose(f);

    Fermer_progress_dialog();

    if(verbose) printf("Sortie de BddInter::LoadOFF\n");
}

// Pour fichiers STL

void BddInter::LoadSTL() {
/*
 * Lecture d'un fichier STL (STéréoLithographique, ou encore Standard Triangle Language ou encore Standard Tessellation Language)
 * 2 formats reconnus : Ascii et Binaire
 * 1 seul objet, que des facettes triangulaires, coordonnées des sommets dans chaque facette => plusieurs fois le même point => simplification possible
 */

    bool binary;            // Pour identifier/différentier un fichier .stl de type Ascii ou Binaire
    char *cptr, *nom_obj;
    unsigned int i,j,ilen;
    unsigned int numero_facette;
    unsigned int numero_sommet,numero_sommetB;
    std::vector<int> Numeros;
    float vx,vy,vz;
    Object *objet_courant;
    Face   *facette_courante;
    unsigned int nb_facettes_loc=0;
    unsigned int nb_sommets_loc=0;

    if(verbose) printf("Entree de BddInter::LoadSTL\n");

    wxCharBuffer buffer=this->file.mb_str();

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f = fopen(buffer.data(),"r");   //ouverture du fichier

    fstat64(fileno(f), &s_statfichier);
    fichierBdd_length = s_statfichier.st_size;
    printf("Taille : %lld\n",fichierBdd_length);

    cptr=fgets(s1,81,f) ;           // 80 caractères +1 pour permettre un \n
    s1[81] = '\0';                  // Simple précaution
    printf("longueur de l'entête : %d\n",(int)strlen(s1));
    if (cptr == nullptr) {
        printf("Fichier vide !\n");
        fclose(f);
        type_fichier = -1;
        return;
    }

    binary = true;
    cptr   = s1;
    for (i=0; i<80; i++, cptr++) {
        if (*cptr == 0x0A) {
            binary = false;     // Si le caractère LF est trouvé dans les 80 premiers du fichier, c'est que ce n'est pas un fichier binaire, mais plutôt Ascii !
            break;              // => Inutile de poursuivre la recherche
        }
    }

    if (!binary && strncmp(s1,"solid ",6)) {    // Fichier Ascii, mais ne contenant pas la chaîne "solid " sur les 6 premiers caractères !
        printf("Fichier .stl, mais pas de type Ascii stéréolithographique !\n");
        fclose(f);
        type_fichier = -1;
        return ;
    }

    if (binary) {
        printf("Fichier .stl binaire !\n");
        if (!strncmp(s1,"solid ",6)) {      // Débute par "solid ", mais c'est un fichier binaire
            nom_obj = strdup(s1+6);         // Le nom commence donc au 7ème caractère
        } else {
            nom_obj = strdup(s1);           // Retenir toute la chaîne comme nom
        }
    } else {
        printf("Fichier .stl Ascii !\n");
        nom_obj = strdup(s1+6);             // Le nom commence donc au 7ème caractère
    }
    ilen = strlen(nom_obj);
    cptr = nom_obj + ilen-1;
    while (*cptr == '\n') {                 // éliminer un \n de fin (ou evt. plusieurs)
        *cptr = '\0';
        cptr--;
    }
    while (*cptr == ' ') {                  // éliminer les espaces de fin
        *cptr = '\0';
        cptr--;
    }

    printf("Nom de l'objet : %s\n",nom_obj);

    if (!binary) {

// Lecture d'un fichier STL Ascii

        // Compter le nombre de facettes et sommets (1ère lecture)
        rewind(f);
        fgets(s1,160,f) ;                   // Les lignes contiennent moins de 160 caractères (plutôt 60). On prévoie large donc.
        while (strncmp(s1,"endsolid ",9)) { // Lire tant que la chaîne endsolid n'a pas été trouvée. ERREUR : Que se passe t-il si elle n'existe pas ???
            cptr=fgets(s1,160,f) ;          // Peut-il y avoir des lignes vides ?
            if (cptr == nullptr) {
                printf("Erreur\n");         // Ligne "endsolid " non trouvée ?
                return;
            }
            cptr = s1;
            while (*cptr == ' ') cptr++;    // Pour éliminer les espaces de début
            if (!strncmp(cptr,"facet normal ",13)) nb_facettes_loc++;
            if (!strncmp(cptr,"vertex ",7))        nb_sommets_loc++ ;
        }
        rewind(f);

        // Remplir l'objet (2ème lecture)

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),0+Numero_base);    // Voir si Numero_base est utile (en cas de lecture de plusieurs fichiers ?)
        str += NomObj;
        make_objet();
        objet_courant = &(this->Objetlist[indiceObjet_courant]);

        // Création des sommets et des facettes

        objet_courant->Nb_sommets = nb_sommets_loc;
        this->N_elements = nb_sommets_loc;
        str.clear();
        make_sommet();

        this->N_elements = nb_facettes_loc;
        make_face();
        make_normale();
        make_aspect_face();

        fgets(s1,160,f) ;
        numero_sommet = 1;
        numero_sommetB= 1;
        numero_facette= 1;
        Numeros.resize(3);

        while (strncmp(s1,"endsolid ",9)) {
            fgets(s1,160,f) ;
            cptr = s1;
            while (*cptr == ' ') cptr++;
            if (!strncmp(cptr,"facet normal ",13)) {
                cptr+=13;
                for (i=0; i< 3; i++) {
                    Numeros[i] = numero_sommet++;
                }
                str.clear();                        // Important pour le make_1_normale suivant
                this->N_elements = numero_facette;  // Idem
//                this->Set_numeros(Numeros) ;
//                make_1_face();
                make_1_face(numero_facette,Numeros);
                facette_courante = &(objet_courant->Facelist[numero_facette-1]);
                facette_courante->flat       = false;   // Pas de normales aux sommets dans un STL => activer la possibilité de les calculer
                facette_courante->groupe     = groupe_def;
                facette_courante->codmatface = codmatface_def;

                sscanf(cptr,"%f%f%f", &vx, &vy, &vz);   // 3 composantes de la normale à la facette
                this->Setxyz(vx,vy,vz);
                make_1_normale();

                numero_facette++;
            }
            if (!strncmp(cptr,"vertex ",7)) {           // Normalement 3 lignes vertex consécutives => 3 sommets d'un triangle
                cptr+=7;
                str.clear();
                sscanf(cptr,"%f%f%f", &vx, &vy, &vz);
                this->N_elements = numero_sommetB;
                this->Setxyz(vx,vy,vz);
                make_1_sommet();

                numero_sommetB++;
            }

            Update_Dialog((long long)ftello64(f), fichierBdd_length);
        }

    } else {

// Lecture d'un fichier STL Binaire
        bool erreur_couleurs = false;
        int index=0;
        UINT32 nb_triangles;            // C'est un Unsigned Integer sur 32 bits (en fait <=> int sur gcc 32 ou 64 bits)
        UINT16 Attribute ;              // Unsigned Integer sur 16 bits (<=> unsigned short en gcc 32 ou 64 bits)

        fclose(f);                      // Fermer le fichier car ouvert précédemment en mode Ascii
        f = fopen(buffer.data(),"rb");  // Le réouvrir en mode lecture+binaire
        fseek(f,80,0);                  // passer les 80 premiers octets d'entête (déjà lus et décodés) : ici fseek suffit, mais on aurait pu mettre fseeko64

        fread(&nb_triangles,sizeof(UINT32),1,f);
        printf("Nombre de triangles : %d\n",nb_triangles);

        // Remplir l'objet

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),0+Numero_base);    // Numero_base ?? voir plus haut.
        str += NomObj;
        make_objet();

        objet_courant = &(this->Objetlist[indiceObjet_courant]);

        // Création des sommets et des facettes

        nb_sommets_loc = nb_triangles*3;                // Facettes triangulaires, donc 3 sommets
        nb_facettes_loc= nb_triangles;

        objet_courant->Nb_sommets = nb_sommets_loc;
        this->N_elements = nb_sommets_loc;
        str.clear();
        make_sommet();

        this->N_elements = nb_facettes_loc;
        make_face();
        make_normale();
        make_aspect_face();

        numero_sommet = 1;
        numero_sommetB= 1;
        numero_facette= 1;
        Numeros.resize(3);

        listeMateriaux.clear();

        for (i=1; i<=nb_facettes_loc; i++) {
            for (j=0; j< 3; j++) {
                Numeros[j] = numero_sommet++;
            }
            str.clear();                        // Important pour le make_1_normale suivant
            this->N_elements = numero_facette;  // Idem
//            this->Set_numeros(Numeros) ;
//            make_1_face();
            make_1_face(numero_facette,Numeros);
            facette_courante = &(objet_courant->Facelist[numero_facette-1]);
            facette_courante->flat       = false;   // Pas de normales aux sommets dans un STL => activer la possibilité de les calculer
            facette_courante->groupe     = groupe_def;
            facette_courante->codmatface = codmatface_def;

            fread(xyz,sizeof(float),3,f);               // Trois composantes de la normale. Ce doit être des float32 (OK sous gcc GNU 32 et 64)
            make_1_normale();

            numero_facette++;

            for (j=0; j<3; j++) {                       // 3 sommets d'un triangle
                fread(xyz,sizeof(float),3,f);           // 3*3 coordonnées des sommets
                this->N_elements = numero_sommetB;
                make_1_sommet();
                numero_sommetB++;
            }

            fread(&Attribute,sizeof(UINT16),1,f);       // Entier 16 bits de réserve (très peu utilisé apparemment)

            if (Attribute > 0 || !listeMateriaux.empty()) {
                int materiau = Attribute;
                auto it = std::find(listeMateriaux.begin(),listeMateriaux.end(),materiau);  // Est-il déjà dans la liste ?
                if (it == listeMateriaux.end() || listeMateriaux.empty()) {                 // Non
                    listeMateriaux.push_back(materiau);                                     // L'ajouter à la liste des matériaux
                    // Les 6 lignes suivantes permettent de coder materiau avec la couleur trouvée dans Attribute
                    index = listeMateriaux.size();
                    if (index < nb_couleurs) {
                        MatDiffuse_avionG[index][0] = MatAmbient_avionG[index][0] = (materiau % 32) / 31.;          // Bits  0 à 4  -> rouge entre 0. et 1.
                        MatDiffuse_avionG[index][1] = MatAmbient_avionG[index][1] = ((materiau >>  5) % 32) / 31.;  // Bits  5 à 9  -> vert     "
                        MatDiffuse_avionG[index][2] = MatAmbient_avionG[index][2] = ((materiau >> 10) % 32) / 31.;  // Bits 10 à 14 -> bleu     "
                    } else erreur_couleurs = true;
                }
                int rang = 1;
                for (it = listeMateriaux.begin(); it != listeMateriaux.end(); ++it,++rang) {
                    if (*it == materiau) break;         // materiau identifié => sortir du for pour obtenir son rang
                }
                facette_courante->codmatface = rang;    // Coder le matériau pas son rang dans la liste
                facette_courante->groupe     = rang-1;  // Pour donner une valeur différente du matériau
            }

            Update_Dialog((long long)ftello64(f), fichierBdd_length);
        }
        if (erreur_couleurs) {
            // les tableaux MatAmbient_avionG et MatDiffuse_avionG sont trops petits
            wxString wxMessage;
            wxMessage.clear();
            wxMessage.Printf("Trop de couleurs/valeurs Attribute différentes dans ce ficher .stl binaire.\n%d valeurs trouvées, maximum : %d",index,nb_couleurs-1);
            wxMessage  += _T("\nAgrandir MatAmbient_avionG et MatDiffuse_avionG");
            DisplayMessage(wxMessage,true);
        }
    }

    objet_courant->Nb_luminances= 0;   // Dans le format STL, il n'y a pas de normales aux sommets
    objet_courant->Nb_vecteurs  = 0;   // enregistrées dans le fichier de Bdd
    objet_courant->flat = true;        // A la lecture, l'objet n'est composé que de facettes planes => Il faudra recalculer toutes les normales

    m_loaded = true ; //true;
    m_gllist = 0;

    fclose(f);
//    type_fichier = -1;  // Pour le moment
//    Update();

    Fermer_progress_dialog();

    if(verbose) printf("Sortie de BddInter::LoadSTL\n");
}

//static long
//fileio_tell_func(void *self) {
//    FILE *f = (FILE*)self;
//    return(ftello64(f));
//}

// Pour fichier au format Autodesk 3DS

void BddInter::Load3DS()
{
/*
 *  Lecture d'un Fichier de polygones au format Autodesk 3DS
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  Une création de groupes faite ?????????????????????????????????
 *  Utilise la librairie lib3ds (d'après l'exemple 3ds2m et 3dsplay)
 *  Certains fichiers sont mal lus, y compris par 3dsplay alors que DeepExploration s'en sort.
 *  Ouvrir donc par DeepExploration et réenregistrer en 3ds en y ajoutant _b au nom par exemple
 */
    int im ;
    Lib3dsNode *p, *node ;
    int meshes = 0;
    int nnodes = 0;
    int i;
    bool make_nodes = false;
//    int indiceObjet;

    if(verbose) printf("Entree de BddInter::Load3DS\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f3ds = lib3ds_file_open(buffer.data());
    if (!f3ds) {
        printf("Le fichier %s n'est pas un fichier 3ds\n",buffer.data());
        type_fichier = -1;
        return;
    }
    fichierBdd_length = std::filesystem::file_size(buffer.data());
    printf("Taille : %lld\n",fichierBdd_length);

    /* Pas de nodes ?  En fabriquer pour favoriser l'affichage (cf 3dsplay.c). */
    if (!f3ds->nodes ) {
        make_nodes = true;                  // pas de nodes du tout !
    } else {
        o_3ds = 0;
        for (p = f3ds->nodes; p != 0; p = p->next) {
            compter_nodes_mesh(p);
        }
        if (o_3ds == 0) make_nodes = true;  // Il y a des nodes mais pas de nodes de type mesh => en construire
    }
//    if( !f3ds->nodes ) {
    if( make_nodes ) {
//        Lib3dsNode *node;

        sprintf(Message,"Création de Nodes, car ce fichier .3ds n'en a pas !\n") ;
        printf("%s",utf8_To_ibm(Message)) ;

        for (i = 0; i < f3ds->nmeshes; ++i) {
            Lib3dsMesh *mesh = f3ds->meshes[i];
            node = lib3ds_node_new(LIB3DS_NODE_MESH_INSTANCE);
            strcpy(node->name, mesh->name);
            lib3ds_file_insert_node(f3ds, node, nullptr);
        }
        sprintf(Message,"%d meshes lus et autant de nodes créés\n",f3ds->nmeshes);
        printf("%s",utf8_To_ibm(Message)) ;
    }

    lib3ds_file_eval(f3ds, 0.0f);   // Indispensable pour configurer proprement les matrices des nodes

    // Code pas très utile ! Sert juste à vérifier 2 méthodes de comptage qui ne donnent pas toujours la même chose ! Différents types : meshes, cameras, ...
//    for (node=f3ds->nodes; node!=nullptr; node=node->next ) {
//        nnodes++;                               // Compter le nombre de nodes (noeuds)
//        printf("Node : %s\n",node->name);
//        for (p=node->childs; p!=nullptr; p=p->next) {
//            nnodes++;
//            printf("Node : %s\n",p->name);
//        }
//        /*       if (nnode) {
//                    if (strcmp(nnode, p->name)!=0) {
//                        continue;
//                    }
//                }*/
//    }
//    printf("Nodes : %d\n",nnodes);

// Appel de compter_nodes_mesh plutôt que faire nb_objets = nnodes
    o_3ds = 0;
    for (p = f3ds->nodes; p != 0; p = p->next) {   // Pas forcément utile car objets construits au fur et à mesure. Laissé par précaution
        compter_nodes_mesh(p);
    }
    printf("sortie de compter_nodes_mesh : o_3ds = %d\n",o_3ds);
//    if (o_3ds != nnodes) {
//        sprintf(Message,"Nombre de nodes retenus : %d, différent du nombre initial : %d\n",o_3ds,nnodes);
//        printf(utf8_To_ibm(Message)) ;
////        printf("Nombre de nodes retenus : %d, different du nombre initial : %d\n",o_3ds,nnodes);
//    }
//    cel->nb_objet = o_3ds ; //meshes;
    meshes = o_3ds;
    printf("%d objets\n",meshes);

    if (meshes == 0) {
        sprintf(Message,"Erreur dans le fichier 3DS : ne contient pas d'objets (meshes)\n") ;
        printf("%s",utf8_To_ibm(Message)) ;
#ifdef WIN32
        system("pause") ;
#endif
//        exit(2) ;
        return;
    }

//    io = new Lib3dsIo();
//    lib3ds_io_setup(io);
//    f = fopen(buffer.data(),"rb");
//    if (!f) printf("fopen ne marche pas\n");
//    io->self = f;
//    io->tell_func = fileio_tell_func;

// Seconde analyse des objets avec appel récursif des nodes
    time_deb_dialog = clock();
    o_3ds      = 0 ; //indiceObjet_courant = o_3ds;
    nb_mat_3ds = 0 ;
    progress_dialog_en_cours = false;
    long cpt;
    for (p = f3ds->nodes,cpt=0; p != 0; p = p->next,cpt++) {
        decoder_node(p);
        Update_Dialog((long long)cpt,(long long)meshes);                          // Faute d'avoir accès à la position en lecture dans f3ds (ou f)
    }

//    numObjet_suiv += ((o_3ds+10)/10)*10 ; // Pour arrondir à la dizaine supérieure
    printf("\nnb_mat = %d\n",nb_mat_3ds) ;
    if (nb_mat_3ds >= 1) {
        sprintf(Message,"Liste des matériaux du fichier 3ds\n") ;
        printf("%s",utf8_To_ibm(Message));
//        printf("Liste des materiaux du fichier 3ds\n") ;
        for (im=0; im < nb_mat_3ds ; im++) {
            sprintf(Message,"%4d %s\n",im+1, tab_mat[im]);
            printf("%s",utf8_To_ibm(Message));
        }
    }
    sprintf(Message,"\nFin de la lecture des données.\n");
    printf("%s",utf8_To_ibm(Message));

    lib3ds_file_free(f3ds);
//    fclose(f);
//    delete io;
    Fermer_progress_dialog();

    m_loaded = true;
    m_gllist = 0;

    if(verbose) printf("Sortie de BddInter::Load3DS\n");
}

int BddInter::mesh_count_smoothing(Lib3dsMesh *mesh)
{
    int i;
    int compteur_smoothing = 0;

    if (!mesh->nfaces) {
        return (0);
    }

    for (i = 0; i < mesh->nfaces; ++i) {
        Lib3dsFace *f = &mesh->faces[i];
        if (f->smoothing_group) compteur_smoothing++;
    }
    return (compteur_smoothing);
}

int BddInter::compter_nodes_mesh (Lib3dsNode *node)
{
    Lib3dsNode *p ;

    for (p = node->childs; p != 0; p = p->next) {
        compter_nodes_mesh(p);
    }

    if (node->type == LIB3DS_NODE_MESH_INSTANCE) {
        if (strcmp(node->name,"$$$DUMMY") == 0 ) {
            return (0);
        }
        o_3ds++;
    }
    return (1);
}

char* BddInter::Lire_chaine( char st [])
{
// élimine les blancs de début et de fin de chaîne

    char *cptr;          //Pointeur de chaîne de caractères
    char  c ;

    cptr = strrchr(st,'\n') ; // élimination du terminateur de ligne.
    if (cptr != nullptr) {
        *cptr = '\0' ;
    }
    cptr = strrchr(st,'\r') ; // élimination d'un éventuel cr.
    if (cptr != nullptr) {
        *cptr = '\0' ;
    }
    c     = st[0] ;
    st[0] = 'x'   ;                         // Pour limiter le -- dans le while suivant
    cptr = strchr(st,'\0') ;                // Pointe la fin de chaîne
	while(*--cptr == ' ') {*cptr = '\0' ;}  // élimine les blancs de fin de chaîne.
    cptr = st ;                             // Pointe le début de chaîne
    st[0] = c ;                             // Restituer le caractère
    while(*cptr != '\0') {
        if (*cptr == '\t') *cptr = ' ' ;    // Remplace les tab par des espaces dans la chaîne
        cptr++ ;
    }
    cptr = st ;
	while(*cptr == ' ')  {cptr++ ;}         // élimine les blancs de début de chaîne.
    if (*cptr == '\0')
        cptr = strcpy(st, " ") ;            // Laisser au moins un blanc
    return (cptr) ;
}

void BddInter::Affiche_Matrice(float M[4][4]) {
    int i,j;
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) printf("%12.5f ",M[j][i]);
        printf("\n");
    }
    printf("\n");
}

int BddInter::decoder_node (Lib3dsNode *node)
{
/*  Lecture d'un Fichier de polygones au format Autodesk 3DS
 *  Identifie éventuellement plusieurs objets, crée des normales aux barycentres des facettes.
 *  Une création de groupes faite ?????????????????????????????????
 *  Utilise la librairie lib3ds (d'après l'exemple 3ds2m ou 3Dsplay, mais quelques soucis, voir ci-dessus ! )
 */
    char nom_obj[80];
    int im;
    unsigned int i,k, nfac, npoint, nb_fac, nb_p, found ;
    unsigned int o=0;		            // pour compter le nombre d'objets
    int num_mat ;
    int index;
    float matrice[4][4], inv_matrice[4][4] ;
    float matrice_w[4][4];
    Object *objet_courant;
    Face   *facette_courante;

    Lib3dsMesh *mesh;
    float pos[3],pos2[3];
    Lib3dsMaterial *mat3ds = 0;
    Lib3dsMeshInstanceNode *n;
    char nom_mat_tmp[80];
    std::vector<int>   Numeros;
    std::vector<float> xyz_point;
    int N0,N1,N2;

    Numeros.resize(3);
//    printf("%ld\n",lib3ds_io_tell(io));

    {
        Lib3dsNode *p ;
        for (p = node->childs; p != 0; p = p->next) {
            decoder_node(p);
        }
    }

    if (node->type == LIB3DS_NODE_MESH_INSTANCE) {

        n = (Lib3dsMeshInstanceNode*)node;

        if (strcmp(node->name,"$$$DUMMY") == 0) {
            return (0);
        }

        index = lib3ds_file_mesh_by_name(f3ds, n->instance_name);
        if( index < 0 )
            index = lib3ds_file_mesh_by_name(f3ds, node->name);     // 2ème essai, si pas trouvé par instance_name
        if (index < 0) {
            return (0);                                             // Si toujours pas trouvé, sortir
        }

        mesh = f3ds->meshes[index];

//        printf("user_id : %d\n",mesh->user_id);

        strcpy(nom_obj, Lire_chaine(mesh->name)) ;
//        printf("Objet :%4d, Nom : %s\n",o,nom_obj);

        wxString wx_num_obj;
        wx_num_obj.Printf(_T("%d "),(int)Objetlist.size());
        str.Printf(_T("<OBJET> "));
        str += wx_num_obj;

#if wxCHECK_VERSION(3,0,0)
        wxString wxNom = wxString(nom_obj);
#else
        wxString wxNom = wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        // Récupération du nombre de points
        nb_p = mesh->nvertices ;
        if (nb_p == 0) {
            printf("\nObjet :%4d, Nom : %s ==> VIDE. Non pris en compte !\n",o,nom_obj);
            return (0);   // Pas de points => objet vide donc l'ignorer
        }

        str += wxNom;
        make_objet();

        o_3ds = o = Objetlist.size() -1;
//      Plus simple que o_3ds à écrire dans les indices de tableaux

        printf("Objet :%4d, Nom : %s\n",o,nom_obj);
//        indiceObjet_courant = o;

        str.clear();
        N_elements = nb_p;
        make_sommet();       // Construire les sommets
        objet_courant = &(this->Objetlist[o]) ;
        objet_courant->Nb_sommets = nb_p ;

        N_elements = 3*mesh->nfaces;
        make_vecteur();  // Ici 3 vecteurs par facette. On ne tient pas compte des vecteurs communs à plusieurs facettes
        objet_courant->Nb_vecteurs = N_elements;

// Allocation de mémoire pour le calcul des normales aux sommets
        float (*normal_S)[3] = (float(*)[3])malloc(3 * 3 * sizeof(float) * mesh->nfaces);
// Calcul des normales aux sommets par la fonction de la lib3ds
        lib3ds_mesh_calculate_vertex_normals(mesh, normal_S);
        int compteur_smoothing = mesh_count_smoothing(mesh);
// Allocation de mémoire pour le calcul des normales au barycentre des facettes
        float (*normal_B)[3] = (float(*)[3])malloc(3 * sizeof(float) * mesh->nfaces);
// Calcul des normales aux barycentres par la fonction de la lib3ds
        lib3ds_mesh_calculate_face_normals(mesh, normal_B);

// Dans lib3ds, il y a aussi lib3ds_mesh_calculate_face_normals (à voir ...)

        if (test_decalage3ds) {
            printf("\nObjet : Nom : %s\n",nom_obj);
            lib3ds_matrix_copy(inv_matrice,mesh->matrix);
//            Affiche_Matrice(inv_matrice);
            lib3ds_matrix_inv(inv_matrice);
//            Affiche_Matrice(inv_matrice);
            lib3ds_matrix_copy(matrice_w,node->matrix);
//            Affiche_Matrice(matrice_w);
//            lib3ds_matrix_copy(matrice_w,  mesh->matrix);
//            lib3ds_matrix_mult(matrice,matrice_w,inv_matrice);    // Est ça ou (matrice,inv_matrice,matrice_w)
//            Affiche_Matrice(matrice_w);
//            printf("pivot : %f %f %f \n", n->pivot[0], n->pivot[1], n->pivot[2]);
//            printf("diff  : %f %f %f \n", matrice_w[3][0] +n->pivot[0], matrice_w[3][1] +n->pivot[1], matrice_w[3][2] +n->pivot[2]);
            lib3ds_matrix_translate(matrice_w, -n->pivot[0], -n->pivot[1], -n->pivot[2]);
//            Affiche_Matrice(matrice_w);
            lib3ds_matrix_mult(matrice,matrice_w,inv_matrice);
//            lib3ds_matrix_mult(matrice,inv_matrice,matrice_w);
//            Affiche_Matrice(matrice);
//            lib3ds_matrix_translate(matrice, -n->pivot[0], -n->pivot[1], -n->pivot[2]);
        }

        // Lecture et transformation des points
        for (i = 0; i < nb_p; i++) {
            if (test_decalage3ds) {
                lib3ds_vector_copy(pos2, mesh->vertices[i]);
                lib3ds_vector_transform(pos,matrice,pos2);
            } else {
                lib3ds_vector_copy(pos, mesh->vertices[i]);
            }

            // Lecture des x, y et z
            N_elements = i+1;
            Setxyz(pos[0],pos[1],pos[2]);
            make_1_sommet();
        }

// Modification éventuelle de matrice de transformation
        if (test_decalage3ds) { // Pour la rotation des normales : matrice de rotation pure ! => ne garder que [0], [1] et [2] dans les 2 dimensions
            matrice[0][3] = matrice[3][0] =  matrice[1][3] = matrice[3][1] =  matrice[2][3] = matrice[3][2] = 0.;
            matrice[3][3] = 1;
        }

        // Récupération du nombre de facettes
        nb_fac = mesh->nfaces ;
        objet_courant->Nb_facettes = nb_fac ;
        objet_courant->Nb_normales = nb_fac;

        str.clear();
        N_elements = nb_fac;
        make_face();
        make_luminance();
        make_normale();
        make_aspect_face();

        // Lecture des facettes
        int n_vect=1;   // Numéro du 1er vecteur calculé
        for (i=0; i < nb_fac; ++i) {
            nfac = i+1 ;

            Numeros[0] = N0 = mesh->faces[i].index[0];  // Garder ces indices dans N0,... car ils vont reservir
            Numeros[1] = N1 = mesh->faces[i].index[1];
            Numeros[2] = N2 = mesh->faces[i].index[2];
            for (k=0; k<3; k++) Numeros[k]++;           // Ajouter 1 au n°, car on commence à 1 et non 0
//            this->N_elements = nfac;
//            Set_numeros(Numeros);
//            printf("Numeros Sommets : %d %d %d\n",Numeros[0],Numeros[1],Numeros[2]);
            str.clear();
//            make_1_face();
            make_1_face(nfac,Numeros);

// Calcul de la normale au barycentre de la facette via lib3ds_mesh_calculate_face_normals
            lib3ds_vector_copy(this->xyz, normal_B[i]);
            if (test_decalage3ds) {
                lib3ds_vector_transform(this->xyz, matrice, normal_B[i]);
            }
            this->N_elements=nfac; this->make_1_normale();

// Normales aux sommets
            for (int nn = 0; nn<3 ; nn++) {
                int nnp = 3*i + nn;
                lib3ds_vector_copy(this->xyz, normal_S[nnp]);
                if (test_decalage3ds) {
                    lib3ds_vector_transform(this->xyz, matrice, normal_S[nnp]); // On prend les normales qui ont été calculées, même si = normal_B correspondante
                }
                this->N_elements = n_vect++;
                str.clear();
                this->make_1_vecteur();
            }
            Numeros[0]=Numeros[1]=Numeros[2] = 3*i+1;  Numeros[1] += 1 ; Numeros[2] += 2;
//            Set_numeros(Numeros);
//            str.clear();
//            this->N_elements = nfac;
//            make_1_luminance();
            make_1_luminance(nfac,Numeros);

// Identification des matériaux (s'il y en a)
            num_mat = -123 ;
//            mat3ds->name[0] = 0;
//            printf("i= %d, material= %d\n",i,mesh->faces[i].material);

            if (mesh->faces[i].material >= 0) {
                mat3ds = f3ds->materials[mesh->faces[i].material];
                strcpy(nom_mat_tmp,mat3ds->name);
            } else {
                sprintf(nom_mat_tmp,"Mat_Null"); // Forcer un nom de matériau
            }

            if (nb_mat_3ds == 0) {
                if (strlen(nom_mat_tmp) > 0) {
                    tab_mat[nb_mat_3ds++] = strdup(nom_mat_tmp) ;
                    num_mat = 1 ;
                }
            } else {
                found = 0;
                for (im=0; im< nb_mat_3ds; im++) {
                    if (!strncmp(nom_mat_tmp,tab_mat[im],strlen(tab_mat[im]))) {
                        found = 1;      // Ce matériau est déjà dans la liste
                        break;
                    }
                }
                if(found) num_mat = im+1 ;
                else {                  // Ajouter le nouveau matériau
                    if (strlen(nom_mat_tmp) > 0) {
                        tab_mat[nb_mat_3ds++] = strdup(nom_mat_tmp);
                        num_mat = nb_mat_3ds ;
                    }
                    if (nb_mat_3ds >= nb_mat_max) {
                        sprintf(Message,"ATTENTION : Trop de matériaux dans ce fichier .3ds nb_mat > %u\n",nb_mat_max);
                        printf("%s",utf8_To_ibm(Message));
//                        printf("ATTENTION : Trop de materiaux dans ce fichier .3ds nb_mat > %u\n",nb_mat_max);
                    }
                }
            }
            facette_courante = &(objet_courant->Facelist[i]);
            facette_courante->groupe     = num_mat;
            facette_courante->codmatface = num_mat;
            facette_courante->afficher   = true;       // Pour afficher la facette
            facette_courante->deleted    = false;
            facette_courante->flat       = false;      // En réalité, la facette peut être plane : les 3 normales calculées aux sommets = normale au barycentre/
//            printf("%ld\n",ftello64(f));                 // retourne toujours 0 avec f = fopen(buffer.data(),"rb"); fait dans Load_3DS

        }

// Désallocation de la mémoire de calcul des normales
        free(normal_S);
        free(normal_B);

        if (compteur_smoothing)
            objet_courant->flat = false;    // Si compteur_smoothing != 0, les normales aux sommets sont utilisables car != normales aux barycentres
        else
            objet_courant->flat = true;     // Objet sans normales aux sommets => forcer les facettes planes de cet objet

        objet_courant->afficher = true;
        objet_courant->deleted  = false;
    }
    return (0);
}

// Pour fichier SDM Oktal

void BddInter::LoadBDD() {

// ATTENTION : la lecture originale via tfile pose problème s'il y a des caractères accentués dans un fichier ANSI => Plante car agit comme si on avait trouvé un Eof !
//             Pas de soucis, si le fichier comportant des caractères accentués est encodé en UTF8.
// Autres soucis : tfile semble lire tout le fichier en mémoire dès le début => s'il y a un caractère erronné dedans (comme dans f16_celar.bdd, ligne 11052)
//                 c'est comme ci-dessus => le prend pour un Eof et le fichier n'est pas décodé.
//             OK avec wxFileInputStream + wxTextInputStream c'est mieux car la ligne est certes mal lue, mais ça n'empêche pas de lire la suite !

//             Revoir le calcul des Luminances => plante sur certains fichiers celar à cause d'erreur d'indicage (variable compteur >= 96 sur 1er objet
//             de EuroFigh_8.bdd par exemple !

// On pourrait décoder d'autres mots comme OMBRAGE / SHADING (ce qui permettrait d'imposer des facettes planes ou avec lissage de Gouraud/Phong ou ...)

    Object *objet_courant=nullptr;
//    int x1 = 0;
    unsigned int i;
    int mode_lecture= 0;
    int non_retenus = 0;
    unsigned int NbFacettes   = 0;
    unsigned int NbSommets    = 0;
    unsigned int NbNormales   = 0;
    unsigned int NbAspectFaces= 0;
    unsigned int NbLuminances = 0;
    unsigned int NbVecteurs   = 0;
    unsigned int indiceObjet  = 0;
    unsigned int NbMatrices   = 0;

    int N_elements_prec;

#define notFound std::string::npos      // Définition sous forme de macro compatible 32 / 64 bits (notFound ne peut pas être une variable car le type diffère en 32 et 64 bits)

    char  c_ligne[300],mot_cle[300];
    int j, k, Nb;
    double matrice[16];

    bool ligne_utf8;

    if(verbose) printf("Entree de BddInter::LoadBdd\n");

    wxCharBuffer buffer=this->file.mb_str();
    fichierBdd.open (buffer.data());

    fichierBdd.seekg(0,fichierBdd.end);     // Se positionner en fin de fichier
    fichierBdd_length = fichierBdd.tellg(); // Récupérer la position dans le fichier, donc ici sa taille
    fichierBdd.seekg(0,fichierBdd.beg);     // Revenir au début du fichier
    printf("Taille : %lld\n",fichierBdd_length);

//    wxRect rect;
//    this->MAIN_b->GetStatusBar()->GetFieldRect(0, rect);              // Récupérer la taille et position du premier champ de la StatusBar

//    if (m_gauge) wxDELETE(m_gauge);
// PROBLEME : si pas de timer, il faut mettre une valeur max < 100, mais le timer ne fonctionne pas ici. Pourquoi ?
//    m_gauge = new wxGauge(this->MAIN_b->GetStatusBar(), wxID_ANY, 100, rect.GetPosition(), rect.GetSize(), wxGA_HORIZONTAL|wxNO_BORDER );
//    m_gauge->SetSize(rect.x + margin, rect.y + margin, rect.width - 2*margin, rect.height - 2*margin);  // Changer position et taille pour laisser une petite marge autour
//    m_gauge->SetSize(rect.x + margin, rect.y + margin, 300, rect.height - 2*margin);  // Changer position et taille pour laisser une petite marge autour
//    m_gauge->SetValue(0);
// Divers tests d'init du timer ...
//    if (!m_timer) m_timer = new wxTimer();//(this->MAIN_b,wxID_ANY);   // Créer un timer
////    id_timer= m_timer->GetId();
////    m_timer->SetOwner(this->MAIN_b,id_timer);
//////    Connect(wxID_ANY,wxEVT_TIMER,(wxObjectEventFunction)&BddInter::OnTimer_Bdd);
////    printf("id_timer : %d\n",m_timer->GetId());
////    m_timer->Bind(wxEVT_TIMER, &BddInter::OnTimer_Bdd, this);
//    if (m_timer->IsRunning())   printf("Timer On\n") ;
//    else                        printf("Timer Off\n");
////    m_timer->Start(100);
//    if (m_timer->IsRunning())   printf("Timer On\n") ;
//    else                        printf("Timer Off\n");
////    timer_bis = false;
////    m_gauge->Update();

    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    if(m_gllist > 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    while(getline(fichierBdd, ligne)) { // Lit ligne par ligne jusqu'à la fin du fichier (si EoF getline retourne eof())
        str.clear();
        while (ligne.back() == ' ') ligne.pop_back();   // Eliminer les éventuels espaces de fin de ligne
        if (ligne.empty())   continue;                  // Si la ligne est vide, passer à la suivante
        if (ligne[0] == '#') continue;                  // Si c'est une ligne de commentaires, passer à la suivante

#if wxCHECK_VERSION(3,0,0)
        str = wxString(ligne);
#else
        str = wxString::FromAscii(ligne.c_str());
#endif // wxCHECK_VERSION

        if((ligne.find("<OBJET>") != notFound) || (ligne.find("<OBJECT>") != notFound)) { // La ligne contient <OBJET> (ou <OBJECT> en version SDM anglaise)
//            printf("<OBJET> %d %d %d\n",ligne.find("<OBJET>"),ligne.find("<OBJECT>"),std::string::npos);
//            printf("taille %d\n",Objetlist.size());
            if (Objetlist.size() != 0) {
//                printf("Nb_facettes : %d, Nb_sommets : %d\n",Objetlist[Objetlist.size()-1].Nb_facettes,(Objetlist.end()-1)->Nb_sommets);
//                printf("Nb_facettes : %d\n",(Objetlist.end()-1)->Nb_facettes);
                if ((Objetlist.end()-1)->Nb_facettes == 0) {
                    printf("L'objet %s est vide => non pris en compte\n",(Objetlist.end()-1)->GetName());
                    Objetlist.erase(Objetlist.end()-1); // On supprime cet objet s'il est vide (car sans facettes)
                    non_retenus++;
                }
            }

            ligne_utf8 = is_utf8(ligne.data());     // La ligne est-elle en utf8 ?
            if(!ligne_utf8) str = str.utf8_str();   // Ligne non utf8 (donc probablement Ansi) : la convertir en utf8

            make_objet();
            mode_lecture = 0;
            indiceObjet  = Objetlist.size() -1;      // Numéro d'indice de l'objet à lire
//            indiceObjet_courant = indiceObjet;      // Enregistrer ce numéro d'indice dans BddInter
            objet_courant = &(this->Objetlist[indiceObjet]);
        } else if((ligne.find("<FACE>") != notFound) || (ligne.find("<POLYGON>") != notFound)) {
            make_face();
            mode_lecture = 1;
            NbFacettes   = objet_courant->Nb_facettes;
            for (i=0; i<NbFacettes; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();    // Eliminer les éventuels espaces de fin de ligne
                while (ligne.empty()) {
                    getline(fichierBdd,ligne);
                    while (ligne.back() == ' ') ligne.pop_back();
                }
                str.clear();
                std::istringstream ss(ligne);
                ss >> N_elements;   // en fait ici, numéro de la facette
                ss >> Nb;           // ici, nombre de sommets de la facette
                NumerosSommets.resize(Nb);
                for (j=0; j<Nb; j++) ss >> NumerosSommets[j];
                make_1_face();
            }
            mode_lecture = -1;
        } else if((ligne.find("<SOMMET>") != notFound) || (ligne.find("<VERTEX>") != notFound)) {
            make_sommet();
            mode_lecture = 2;
            NbSommets = objet_courant->Nb_sommets;
            str.clear();
            N_elements_prec = 0;
            for (i=0; i<NbSommets; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne);
                    while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
                make_1_sommet();
                if (N_elements - N_elements_prec != 1)
                    printf("==> Erreur possible :\n    Sommet %d (indice %d, Objet d'indice %d) rupture de sequence\n",N_elements,i,indiceObjet);

                N_elements_prec = N_elements;
            }
            mode_lecture = -1;
        } else if((ligne.find("<NORMALE>") != notFound) || (ligne.find("<POLY_NORMAL>") != notFound)) {
            make_normale();
            mode_lecture = 3;
            NbNormales   = objet_courant->Nb_normales;
            str.clear();
            for (i=0; i<NbNormales; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                   getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
//                make_1_normale();
                int indiceFacette = N_elements-1;
                objet_courant->Facelist[indiceFacette].setNormale_b(xyz);
            }
            mode_lecture = -1;
        } else if((ligne.find("<ASPECT_FACE>") != notFound) || (ligne.find("<POLY_ATTR>") != notFound)) {
            make_aspect_face();
            mode_lecture = 4;
            NbAspectFaces = objet_courant->Nb_aspects;
            for (i=0; i<NbAspectFaces;) { //; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back(); // Passer la(les) ligne(s) vide(s)
                }
                while (!ligne.empty()) {    // Lire et décoder jusqu'à ce qu'on trouve une ligne vide (ATTENTION s'il n'y en a pas !!!)
                    str.clear();
                    str=wxString::FromAscii(ligne.c_str());
                    make_1_aspect_face();   // A supprimer par la suite ??? Pour lire les lignes  <GROUPE> et <CODMATFACE> avec le même indice i
                                            // Mais laisser pour l'instant car décodage et rangement faits dans make_1_aspect_face à revoir
                                            // C'est le SEUL endroit ou make_1_aspect_face est encore actif (mais utile si pas de ligne blanche intermédiaire !)
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                    if (New_aspect_face) i++;   // Incrémenter le compteur i seulement si 3 valeurs dans la ligne
                }
            }
            mode_lecture = -1;
        } else if((ligne.find("<VECTEUR>") != notFound) || (ligne.find("<VERT_NORMAL>") != notFound)) {
            make_vecteur();
            mode_lecture = 5;
            NbVecteurs   = objet_courant->Nb_vecteurs;
            str.clear();
            N_elements_prec = 0;
            for (i=0; i<NbVecteurs; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
                make_1_vecteur();
                if (N_elements - N_elements_prec != 1)
                    printf("==> Erreur possible :\n    Vecteur %d (indice %d, Objet d'indice %d) rupture de sequence\n",N_elements,i,indiceObjet);

                N_elements_prec = N_elements;
            }
            mode_lecture = -1;
        } else if(ligne.find("<LUMINANCE>") != notFound) {
            make_luminance();
            mode_lecture = 6;
            objet_courant->flat = false;                  // L'objet peut être tracé en mode smooth car <LUMINANCE> est présent
            unsigned int NbLuminances_a_lire = objet_courant->Nb_luminances;
            if (NbLuminances_a_lire < NbFacettes) {                    // C'est le cas de F16.bdd original
                printf("-> Nombre de luminances a lire : %d, Nouvelle dimension : %d\n",NbLuminances_a_lire, NbFacettes);
                objet_courant->Nb_luminances = NbFacettes;
                for (unsigned int nfac=0; nfac < NbFacettes; nfac++) objet_courant->Facelist[nfac].flat = true; // Tout initialiser à plat
            }
            for (i=0; i<NbLuminances_a_lire; i++) {                     // Mais ne lire que les NbLuminances données
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                str.clear();
                std::istringstream ss(ligne);
                ss >> N_elements;   // en fait ici, numéro de la facette
                ss >> Nb;           // ici, nombre de sommets de la facette
                NumerosSommets.resize(Nb);
                for (j=0; j<Nb ; j++) ss >> NumerosSommets[j];
                make_1_luminance();   // peut être à changer en make1luminance(Nb,Numeros_Sommets) avec un tableau local Numeros_Sommet plutôt que global NumerosSommets
                objet_courant->Facelist[N_elements-1].flat = false; // Smooth possible uniquement pour les facettes avec Luminances données
                // Test de vérification de facette plane (tous numéros égaux)
                bool Facette_plane = true;
                int Num_0 = NumerosSommets[0];
                for (j=1; j<Nb ; j++) {
                    if (NumerosSommets[j] != Num_0) Facette_plane = false;
                }
                if (Facette_plane) objet_courant->Facelist[N_elements-1].flat = true;
            }
            mode_lecture = -1;
        } else if((ligne.find("<POSITION>") != notFound) || (ligne.find("<PLACEMENT>") != notFound)) {
            make_position(); // Ne sert pas à grand chose semble t-il !
            mode_lecture = 7;
            objet_courant = &(this->Objetlist[indiceObjet]);        // Déjà comme ça, mais évite un warning
            NbMatrices = objet_courant->Nb_matrices;
            printf("Nombre de matrices a lire : %d\n",NbMatrices);  // Il ne devrait y en avoir qu'une seule par objet ! ATTENTION ici seulement le type MAT_POSITION !
            for (i=0; i<NbMatrices; i++) {                          // Toutefois, on va toutes les lire (au cas où ...) mais ne garder que la dernière
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();        // Lire 1 ligne mais supprimer tous les espaces de fin
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();    // Passer la(les) ligne(s) vide(s)
                }
                Nb = 0;
//                while (!ligne.empty()) {    // Lire et décoder jusqu'à ce qu'on trouve une ligne vide (ATTENTION s'il n'y en a pas !!!)
                for (j = 0; j<4; j++) {     // Lire et décoder 4 lignes.
                    str.clear();
                    str=wxString::FromAscii(ligne.c_str());
                    strcpy(c_ligne,ligne.c_str());
                    if (j == 0) {
                        sscanf(c_ligne,"%d%s%lf%lf%lf%lf",&k,&mot_cle[0],&matrice[0],&matrice[1],&matrice[2],&matrice[3]);  // La 1ère contient un n° de matrice, <MAT_POSITION> et 4 doubles
                        // En toute rigueur, il faudrait vérifier que le mot_cle est bien <MAT_POSITION> car d'autres transformations sont possibles : Translation, Rotation, Scale.
                        // et du coup adapter la lecture ... car elles n'ont pas les mêmes valeurs à lire !
//                        printf("%d %12.8f %12.8f %12.8f %12.8f\n",Nb,matrice[0],matrice[1],matrice[2],matrice[3]);
                    } else {
                        sscanf(c_ligne,"%lf%lf%lf%lf",&matrice[Nb],&matrice[Nb+1],&matrice[Nb+2],&matrice[Nb+3]);           // Les 3 suivantes contiennent 4 doubles
//                        printf("%d %12.8f %12.8f %12.8f %12.8f\n",Nb,matrice[Nb],matrice[Nb+1],matrice[Nb+2],matrice[Nb+3]);
                    }
                    Nb += 4;
                    getline(fichierBdd,ligne); // while (ligne.back() == ' ') ligne.pop_back();    // Pour supprimer les espaces de fin
                }
            }
            for (i=0; i<16; i++) objet_courant->matrice[i] = matrice[i];          // Enregistrer la matrice dans l'objet
            // Vérification
//            for (i=0; i<16; i++) {
//                printf("%12.8f ",objet_courant->matrice[i]);
//                if (i%4 == 3) printf("\n");
//            }
            mode_lecture = -1;
        } else {
                if (mode_lecture > 0) printf("mode_lecture = %d : POURQUOI passer par ici ????\n",mode_lecture);
        }
//        x1++;
//        printf("ligne : %d\n",x1);

//        int val  = m_gauge->GetValue();
//        val = dialog.GetValue();
//        pos = fichierBdd.tellg();
//        new_val = round(pos*100./fichierBdd_length);// +0.5;
////        printf("%d %d\n",val,new_val);
////        if (val < 100) {
//            if ((val != new_val) && (new_val < 100)) {
/////                m_gauge->SetValue(new_val);
/////                m_gauge->Update();
//                dialog.Update(new_val);
////                val = new_val;
////                MAIN_b->UpdateWindowUI();
////                m_gauge->HandleDisplayChange();
////                m_gauge->Refresh();
//            }
////        }
        Update_Dialog((long long)fichierBdd.tellg(), fichierBdd_length);
    }
//    m_gauge->SetValue(100);
//    m_gauge->Update();
//    MAIN_b->UpdateWindowUI();

    if (Objetlist.size() == 0) {
        printf("Le fichier ne comporte aucun objet !\n");
        m_gllist = 0;
        fichierBdd.close();
        wxString wxMessage = _T("Le fichier ne comporte aucun objet !");
        DisplayMessage(wxMessage,true);
        return;
    }
    if ((Objetlist.end()-1)->Nb_facettes == 0) {
        printf("L'objet %s est vide => non pris en compte\n",(Objetlist.end()-1)->GetName());
        Objetlist.erase(Objetlist.end()-1); // On supprime cet objet s'il est vide (car sans facettes)
        non_retenus++;
    }
    if (Objetlist.size() == 1)
        printf("\n%d objet",(int)Objetlist.size());
    else
        printf("\n%d objets\n",(int)Objetlist.size());
    if (non_retenus != 0)
        printf("%d objets(s) non pris en compte\n",non_retenus);

    printf("\nNb objets      %d\n",(int)this->Objetlist.size());

    for (i=0; i<this->Objetlist.size(); ++i) {
        printf("Objet %2d, numero %2d, Nb_facettes : %4d\n",i,this->Objetlist[i].GetValue(), this->Objetlist[i].Nb_facettes);

        if (this->Objetlist[i].mat_position) TraiterMatricePosition(i) ;
    }

//    if (this->Objetlist[indiceObjet].Vecteurlist.size()==0) {
//        printf("\nCalcul Vecteur !!!\n");
//        Calcul_All_Vecteurs();
//    }

    m_loaded = true;
    m_gllist = 0;
    fichierBdd.close();

//    Sleep(1000);

//    if (wxTheApp && m_pendingEvents && !m_pendingEvents->IsEmpty()) ProcessPendingEvents();   // provoque un affichage debug mais pourtant semble finir l'affichage de m_gauge !!!
//    MAIN_b->Refresh();
//    m_gauge->UpdateWindowUI();
//    Update();
//    UpdateWindowUI();
//    MAIN_b->StatusBar1->UpdateWindowUI();
//    RefreshRect(rect);
//    if (m_gauge) wxDELETE(m_gauge);
//    if (m_timer) wxDELETE(m_timer);

    Fermer_progress_dialog();    // Le plus tard possible car fermeture automatique de progress_dialog via le wxPD_AUTO_HIDE

    if(verbose) printf("Sortie de BddInter::LoadBdd\n");
}

void BddInter::TraiterMatricePosition(unsigned int i)
{
    // Il y a une matrice position pour cet objet => transformer d'abord les points puis les normales

    Vector3D V1, V2, New_Norm;  // Vecteurs 3D initialisés à 0.
    std::vector<float> Normale;
    std::vector<float> xyz_point;
    double matrice[16];
    Object *objet_courant;
    unsigned int j, NbSommets, NbFacettes, NbVecteurs;

    objet_courant = &(this->Objetlist[i]);

    for (j=0; j<16 ; j++) matrice[j] = objet_courant->matrice[j] ; // Copie locale de la matrice

    // Traitement des sommets/points par la matrice de position

    NbSommets = objet_courant->Nb_sommets;
#pragma omp parallel for private(xyz_point)
    for (j=0 ; j<NbSommets ; j++) {
        xyz_point = objet_courant->Sommetlist[j].getPoint();
        Vector3D P1(xyz_point[0],xyz_point[1],xyz_point[2]);
        Vector3D P2 = P1.MultPoint4dMatrice4x4(matrice);
//        xyz_point[0] = P2.X ; xyz_point[1] = P2.Y ; xyz_point[2] = P2.Z ;
        xyz_point = {(float)P2.X, (float)P2.Y, (float)P2.Z} ;          // cast (float) sinon Warnings car P2 en double
        objet_courant->Sommetlist[j].setPoint(xyz_point);
    }

    // Modification de la matrice locale pour y supprimer les translations (sans objet sur des normales) ;
    matrice[12] = matrice[13] = matrice[14] = 0.0;

    // Traitement des normales au barycentres des facettes

/*          prendre deux vecteurs (non colinéaires) V1 et V2 passant par le sommet (ou barycentre) étudié (ramené en 0,0,0),
    les modifier par la matrice M puis faire le produit vectoriel des 2 vecteurs modifiés => Nouvelle normale
    équation générale ax + by + cz + d = 0, normale au plan : (a, b, c)
    passe par 0,0,0 => d=0
    cas c != 0 : on fixe x et y, on calcule z
            si x = 1, y = 0 => z = -a/c => V1 = (1, 0, -a/c)
            si x = 0, y = c => z = -b   => V2 = (0, c, -b)
            ainsi le produit vectoriel V1^V2 = (a, b, c) est bien égal à la normale
    cas c = 0 (=> division par 0 ou V1 et V2 ci-dessus vecteurs colinéaires)
    cas b != 0 : on fixe x et z, on calcule y
            si x = -b, z = 0 => y = a => V1 = (-b, a, 0)
            si x = 0,  z = 1 => y = 0 => V2 = ( 0, 0, 1)
            on obtient le produit vectoriel V1^V2 = (a, b, 0)
    cas b = 0
            C'est le plan x = 0 (a est forcément != 0 )
            V1 = (0,a,0)
            V2 = (0,0,1) en fait n'est pas différent du cas b != 0
*/

    NbFacettes = objet_courant->Nb_facettes;
#pragma omp parallel for private(Normale,V1,V2,New_Norm)
    for (j=0; j<NbFacettes; j++){
        Normale = objet_courant->Facelist[j].getNormale_b();
        if (Normale[2] != 0.0) {
            V1 = Vector3D(1.0,        0.0, -Normale[0]/Normale[2]);  // <=> V1.X = 1.0 ; V1.Y = 0.0 ; V1.Z = -Normale[0]/Normale[2];
            V2 = Vector3D(0.0, Normale[2], -Normale[1]);
        } else {
            V1 = Vector3D(-Normale[1], Normale[0], 0.0);
            V2 = Vector3D(        0.0,        0.0, 1.0);
        }
        V1 = V1.MultPoint4dMatrice4x4(matrice);     // Transformation de V1
        V2 = V2.MultPoint4dMatrice4x4(matrice);     // Transformation de V2
        New_Norm = V1.crossProduct(V2);             // Normale à V1 et V2 : V1^V2
        New_Norm.normalize();                       // Normalisation à 1.

        Normale = {(float)New_Norm.X, (float)New_Norm.Y, (float)New_Norm.Z} ;
        objet_courant->Facelist[j].setNormale_b(Normale);
    }

    // Traitement des normales aux sommets
    NbVecteurs = objet_courant->Nb_vecteurs;
#pragma omp parallel for private(Normale,V1,V2,New_Norm)
    for (j=0; j<NbVecteurs; j++){
        Normale = objet_courant->Vecteurlist[j].getPoint();
        if (Normale[2] != 0.0) {
            V1 = Vector3D(1.0,        0.0, -Normale[0]/Normale[2]);
            V2 = Vector3D(0.0, Normale[2], -Normale[1]);
        } else {
            V1 = Vector3D(-Normale[1], Normale[0], 0.0);
            V2 = Vector3D(        0.0,        0.0, 1.0);
        }
        V1 = V1.MultPoint4dMatrice4x4(matrice);     // Transformation de V1
        V2 = V2.MultPoint4dMatrice4x4(matrice);     // Transformation de V2
        New_Norm = V1.crossProduct(V2);             // Normale à V1 et V2 : V1^V2
        New_Norm.normalize();                       // Normalisation à 1.

        Normale = {(float)New_Norm.X, (float)New_Norm.Y, (float)New_Norm.Z} ;
        objet_courant->Vecteurlist[j].setPoint(Normale);
    }
}

// Recopie/adaptation de DXFRenderer

// Conversion table from AutoCAD ACI colours to RGB values
static const struct { unsigned char r, g, b; } aci_to_rgb[256] = {
/*   0 */ {255, 255, 255},
/*   1 */ {255,   0,   0},
/*   2 */ {255, 255,   0},
/*   3 */ {  0, 255,   0},
/*   4 */ {  0, 255, 255},
/*   5 */ {  0,   0, 255},
/*   6 */ {255,   0, 255},
/*   7 */ {255, 255, 255},
/*   8 */ {128, 128, 128},
/*   9 */ {192, 192, 192},
/*  10 */ {255,   0,   0},
/*  11 */ {255, 127, 127},
/*  12 */ {204,   0,   0},
/*  13 */ {204, 102, 102},
/*  14 */ {153,   0,   0},
/*  15 */ {153,  76,  76},
/*  16 */ {127,   0,   0},
/*  17 */ {127,  63,  63},
/*  18 */ { 76,   0,   0},
/*  19 */ { 76,  38,  38},
/*  20 */ {255,  63,   0},
/*  21 */ {255, 159, 127},
/*  22 */ {204,  51,   0},
/*  23 */ {204, 127, 102},
/*  24 */ {153,  38,   0},
/*  25 */ {153,  95,  76},
/*  26 */ {127,  31,   0},
/*  27 */ {127,  79,  63},
/*  28 */ { 76,  19,   0},
/*  29 */ { 76,  47,  38},
/*  30 */ {255, 127,   0},
/*  31 */ {255, 191, 127},
/*  32 */ {204, 102,   0},
/*  33 */ {204, 153, 102},
/*  34 */ {153,  76,   0},
/*  35 */ {153, 114,  76},
/*  36 */ {127,  63,   0},
/*  37 */ {127,  95,  63},
/*  38 */ { 76,  38,   0},
/*  39 */ { 76,  57,  38},
/*  40 */ {255, 191,   0},
/*  41 */ {255, 223, 127},
/*  42 */ {204, 153,   0},
/*  43 */ {204, 178, 102},
/*  44 */ {153, 114,   0},
/*  45 */ {153, 133,  76},
/*  46 */ {127,  95,   0},
/*  47 */ {127, 111,  63},
/*  48 */ { 76,  57,   0},
/*  49 */ { 76,  66,  38},
/*  50 */ {255, 255,   0},
/*  51 */ {255, 255, 127},
/*  52 */ {204, 204,   0},
/*  53 */ {204, 204, 102},
/*  54 */ {153, 153,   0},
/*  55 */ {153, 153,  76},
/*  56 */ {127, 127,   0},
/*  57 */ {127, 127,  63},
/*  58 */ { 76,  76,   0},
/*  59 */ { 76,  76,  38},
/*  60 */ {191, 255,   0},
/*  61 */ {223, 255, 127},
/*  62 */ {153, 204,   0},
/*  63 */ {178, 204, 102},
/*  64 */ {114, 153,   0},
/*  65 */ {133, 153,  76},
/*  66 */ { 95, 127,   0},
/*  67 */ {111, 127,  63},
/*  68 */ { 57,  76,   0},
/*  69 */ { 66,  76,  38},
/*  70 */ {127, 255,   0},
/*  71 */ {191, 255, 127},
/*  72 */ {102, 204,   0},
/*  73 */ {153, 204, 102},
/*  74 */ { 76, 153,   0},
/*  75 */ {114, 153,  76},
/*  76 */ { 63, 127,   0},
/*  77 */ { 95, 127,  63},
/*  78 */ { 38,  76,   0},
/*  79 */ { 57,  76,  38},
/*  80 */ { 63, 255,   0},
/*  81 */ {159, 255, 127},
/*  82 */ { 51, 204,   0},
/*  83 */ {127, 204, 102},
/*  84 */ { 38, 153,   0},
/*  85 */ { 95, 153,  76},
/*  86 */ { 31, 127,   0},
/*  87 */ { 79, 127,  63},
/*  88 */ { 19,  76,   0},
/*  89 */ { 47,  76,  38},
/*  90 */ {  0, 255,   0},
/*  91 */ {127, 255, 127},
/*  92 */ {  0, 204,   0},
/*  93 */ {102, 204, 102},
/*  94 */ {  0, 153,   0},
/*  95 */ { 76, 153,  76},
/*  96 */ {  0, 127,   0},
/*  97 */ { 63, 127,  63},
/*  98 */ {  0,  76,   0},
/*  99 */ { 38,  76,  38},
/* 100 */ {  0, 255,  63},
/* 101 */ {127, 255, 159},
/* 102 */ {  0, 204,  51},
/* 103 */ {102, 204, 127},
/* 104 */ {  0, 153,  38},
/* 105 */ { 76, 153,  95},
/* 106 */ {  0, 127,  31},
/* 107 */ { 63, 127,  79},
/* 108 */ {  0,  76,  19},
/* 109 */ { 38,  76,  47},
/* 110 */ {  0, 255, 127},
/* 111 */ {127, 255, 191},
/* 112 */ {  0, 204, 102},
/* 113 */ {102, 204, 153},
/* 114 */ {  0, 153,  76},
/* 115 */ { 76, 153, 114},
/* 116 */ {  0, 127,  63},
/* 117 */ { 63, 127,  95},
/* 118 */ {  0,  76,  38},
/* 119 */ { 38,  76,  57},
/* 120 */ {  0, 255, 191},
/* 121 */ {127, 255, 223},
/* 122 */ {  0, 204, 153},
/* 123 */ {102, 204, 178},
/* 124 */ {  0, 153, 114},
/* 125 */ { 76, 153, 133},
/* 126 */ {  0, 127,  95},
/* 127 */ { 63, 127, 111},
/* 128 */ {  0,  76,  57},
/* 129 */ { 38,  76,  66},
/* 130 */ {  0, 255, 255},
/* 131 */ {127, 255, 255},
/* 132 */ {  0, 204, 204},
/* 133 */ {102, 204, 204},
/* 134 */ {  0, 153, 153},
/* 135 */ { 76, 153, 153},
/* 136 */ {  0, 127, 127},
/* 137 */ { 63, 127, 127},
/* 138 */ {  0,  76,  76},
/* 139 */ { 38,  76,  76},
/* 140 */ {  0, 191, 255},
/* 141 */ {127, 223, 255},
/* 142 */ {  0, 153, 204},
/* 143 */ {102, 178, 204},
/* 144 */ {  0, 114, 153},
/* 145 */ { 76, 133, 153},
/* 146 */ {  0,  95, 127},
/* 147 */ { 63, 111, 127},
/* 148 */ {  0,  57,  76},
/* 149 */ { 38,  66,  76},
/* 150 */ {  0, 127, 255},
/* 151 */ {127, 191, 255},
/* 152 */ {  0, 102, 204},
/* 153 */ {102, 153, 204},
/* 154 */ {  0,  76, 153},
/* 155 */ { 76, 114, 153},
/* 156 */ {  0,  63, 127},
/* 157 */ { 63,  95, 127},
/* 158 */ {  0,  38,  76},
/* 159 */ { 38,  57,  76},
/* 160 */ {  0,  63, 255},
/* 161 */ {127, 159, 255},
/* 162 */ {  0,  51, 204},
/* 163 */ {102, 127, 204},
/* 164 */ {  0,  38, 153},
/* 165 */ { 76,  95, 153},
/* 166 */ {  0,  31, 127},
/* 167 */ { 63,  79, 127},
/* 168 */ {  0,  19,  76},
/* 169 */ { 38,  47,  76},
/* 170 */ {  0,   0, 255},
/* 171 */ {127, 127, 255},
/* 172 */ {  0,   0, 204},
/* 173 */ {102, 102, 204},
/* 174 */ {  0,   0, 153},
/* 175 */ { 76,  76, 153},
/* 176 */ {  0,   0, 127},
/* 177 */ { 63,  63, 127},
/* 178 */ {  0,   0,  76},
/* 179 */ { 38,  38,  76},
/* 180 */ { 63,   0, 255},
/* 181 */ {159, 127, 255},
/* 182 */ { 51,   0, 204},
/* 183 */ {127, 102, 204},
/* 184 */ { 38,   0, 153},
/* 185 */ { 95,  76, 153},
/* 186 */ { 31,   0, 127},
/* 187 */ { 79,  63, 127},
/* 188 */ { 19,   0,  76},
/* 189 */ { 47,  38,  76},
/* 190 */ {127,   0, 255},
/* 191 */ {191, 127, 255},
/* 192 */ {102,   0, 204},
/* 193 */ {153, 102, 204},
/* 194 */ { 76,   0, 153},
/* 195 */ {114,  76, 153},
/* 196 */ { 63,   0, 127},
/* 197 */ { 95,  63, 127},
/* 198 */ { 38,   0,  76},
/* 199 */ { 57,  38,  76},
/* 200 */ {191,   0, 255},
/* 201 */ {223, 127, 255},
/* 202 */ {153,   0, 204},
/* 203 */ {178, 102, 204},
/* 204 */ {114,   0, 153},
/* 205 */ {133,  76, 153},
/* 206 */ { 95,   0, 127},
/* 207 */ {111,  63, 127},
/* 208 */ { 57,   0,  76},
/* 209 */ { 66,  38,  76},
/* 210 */ {255,   0, 255},
/* 211 */ {255, 127, 255},
/* 212 */ {204,   0, 204},
/* 213 */ {204, 102, 204},
/* 214 */ {153,   0, 153},
/* 215 */ {153,  76, 153},
/* 216 */ {127,   0, 127},
/* 217 */ {127,  63, 127},
/* 218 */ { 76,   0,  76},
/* 219 */ { 76,  38,  76},
/* 220 */ {255,   0, 191},
/* 221 */ {255, 127, 223},
/* 222 */ {204,   0, 153},
/* 223 */ {204, 102, 178},
/* 224 */ {153,   0, 114},
/* 225 */ {153,  76, 133},
/* 226 */ {127,   0,  95},
/* 227 */ {127,  63, 111},
/* 228 */ { 76,   0,  57},
/* 229 */ { 76,  38,  66},
/* 230 */ {255,   0, 127},
/* 231 */ {255, 127, 191},
/* 232 */ {204,   0, 102},
/* 233 */ {204, 102, 153},
/* 234 */ {153,   0,  76},
/* 235 */ {153,  76, 114},
/* 236 */ {127,   0,  63},
/* 237 */ {127,  63,  95},
/* 238 */ { 76,   0,  38},
/* 239 */ { 76,  38,  57},
/* 240 */ {255,   0,  63},
/* 241 */ {255, 127, 159},
/* 242 */ {204,   0,  51},
/* 243 */ {204, 102, 127},
/* 244 */ {153,   0,  38},
/* 245 */ {153,  76,  95},
/* 246 */ {127,   0,  31},
/* 247 */ {127,  63,  79},
/* 248 */ { 76,   0,  19},
/* 249 */ { 76,  38,  47},
/* 250 */ { 51,  51,  51},
/* 251 */ { 91,  91,  91},
/* 252 */ {132, 132, 132},
/* 253 */ {173, 173, 173},
/* 254 */ {214, 214, 214},
/* 255 */ {255, 255, 255}
};

// convert an AutoCAD ACI colour to wxWidgets RGB colour
inline wxColour BddInter::ACIColourToRGB(int col)
{
    wxASSERT(col >= 0 && col <= 255);
    return wxColour(aci_to_rgb[col].r, aci_to_rgb[col].g, aci_to_rgb[col].b);
}

// deallocate all the dynamic data
void BddInter::ClearDXF()
{
    m_loaded = false;
    m_layers.clear();
    m_objets.clear();
    m_entities.clear();
}

int BddInter::GetLayerColourDXF(const wxString& layer) const
{
    for (const auto& current : m_layers)
    {
        if (current.name == layer)
            return current.colour;
    }
    return 7;   // white
}

// read two sequential lines
inline void BddInter::GetLines(wxTextInputStream& text, wxString& line1, wxString& line2)
{
    line1 = text.ReadLine().Trim().Trim(false);
    line2 = text.ReadLine().Trim().Trim(false);
}

// parse header section: just skip everything
bool BddInter::ParseHeaderDXF(wxInputStream& stream)
{
    if (verbose) printf("Entree de BddInter::ParseHeaderDXF\n");

    wxTextInputStream text(stream);
    wxString line1, line2;
    while (stream.CanRead())
    {
        GetLines(text, line1, line2);
        if (line1 == "0" && line2 == "ENDSEC") {
            if (verbose) printf("Sortie de BddInter::ParseHeaderDXF en true\n");
            return true;
        }
    }
    if (verbose) printf("Sortie de BddInter::ParseHeaderDXF en false\n");
    return false;
}

// parse tables section: save layers name and colour
bool BddInter::ParseTablesDXF(wxInputStream& stream)
{
// TABLES est lu mais non utilisé par Ovni.

    if (verbose) printf("Entree de BddInter::ParseTablesDXF\n");

    wxTextInputStream text(stream);
    wxString line1, line2;
    bool inlayer=false;
    DXFLayer layer;
    while (stream.CanRead())
    {
        GetLines(text, line1, line2);
        if (line1 == "0" && inlayer)
        {
            // flush layer
            if (!layer.name.IsEmpty() && layer.colour != -1)
            {
                DXFLayer p;
                p.name = layer.name;
                p.colour = layer.colour;
                m_layers.push_back(p);
            }
            layer = DXFLayer();
            inlayer = false;
        }
        if (line1 == "0" && line2 == "ENDSEC") {
            if (verbose) printf("Sortie de BddInter::ParseTablesDXF en true\n");
            return true;
        }
        else if (line1 == "0" && line2 == "LAYER")
            inlayer = true;
        else if (inlayer)
        {
            if (line1 == "2") // layer name
                layer.name = line2;
            else if (line1 == "62") // ACI colour
            {
                long l;
                line2.ToLong(&l);
                layer.colour = l;
            }
        }
    }
    if (verbose) printf("Sortie de BddInter::ParseTablesDXF en false\n");
    return false;
}

// This method is used instead of numStr.ToDouble(d) because the latter
// (wxString::ToDouble()) takes the systems proper locale into account,
// whereas the implementation below works with the default locale.
// (Converting numbers that are formatted in the default locale can fail
//  with system locales that use e.g. the comma as the decimal separator.)
double BddInter::ToDoubleDXF(const wxString& numStr)
{
    double             d;
    std::string        numStr_(numStr.c_str());
    std::istringstream iss(numStr_);

    iss >> d;

    return d;
}

// parse entities section: save 3DFACE and LINE entities
bool BddInter::ParseEntitiesDXF(wxInputStream& stream)
{
// Tel que c'est fait ici, on rassemble tout dans un seul objet.
// On pourrait différentier les objets à chaque nouveau layer, et pour chacun compter les 3DFACE et nombre de sommets pour chacun d'entre eux.
// Peut-être à stocker dans des tableaux ou plutôt des vecteurs dont la taille augmente à chaque nouveau layer.

    wxTextInputStream text(stream);
    wxString line1, line2;
    wxString layer, old_layer;
    int state = 0;  // 0: none, 1: 3DFACE, 2: LINE
    DXFVector v[4];
    DXFObjet p_objets;
    bool EnCours      = false;
    bool type_return  = true;
    int  Nvector      = 3;              // Par défaut, facettes triangulaires
    int  nb3DFACE     = 0;
    int  nbLINE       = 0;
    int  nbLayer      = 0;
    int  nbVector     = 0;
//    int  Nlignes      = 0;              // Compteur de lignes
//    int  Nlignes_test = 10000;          // Toutes les Nlignes_test afficher un . histoire de faire patienter et montrer que ça tourne !
    int  colour       = -1;

    if (verbose) printf("Entree de BddInter::ParseEntitiesDXF\n");

    fichierBdd_length = stream.GetSize();
    printf("Taille du fichier %lld octets\n",fichierBdd_length);
    time_deb_dialog   = clock();
    progress_dialog_en_cours = false;

    old_layer = "";

    while (stream.CanRead())
    {
        GetLines(text, line1, line2);

        Update_Dialog((long long)stream.TellI(), fichierBdd_length);   // Utilise la position courante dans le stream et sa longueur totale pour afficher la barre de progression

//        Nlignes++;
//        if ((Nlignes%Nlignes_test) == 0) {                  // Cette marque de progression à la console n'est plus très utile...
//            EnCours = true;
//            printf("."); fflush(stdout);
//        }
        if (line1 == "0" && state > 0)
        {
            // flush entity : tant que state == 0, la lecture d'une 3DFACE ou LINE n'est pas terminée
            if (state == 1) // 3DFACE est complet
            {
                nb3DFACE++;
                std::unique_ptr<DXFFace> p(new DXFFace);
                p->v0 = v[0];
                p->v1 = v[1];
                p->v2 = v[2];
                p->v3 = v[3];
                p->Nb_DXFVector = Nvector;  // Nombre de vecteurs lus (3 ou 4)
                nbVector += Nvector;
                p->CalculateNormal();
                if (colour != -1)
                    p->colour = colour;
                else
                    p->colour = GetLayerColourDXF(layer);
                m_entities.push_back(std::move(p));
                colour = -1; layer.clear();
                v[0] = v[1] = v[2] = v[3] = DXFVector();
                state = 0;
            }
            else if (state == 2) // LINE est complet
            {
                nbLINE++;
                std::unique_ptr<DXFLine> p(new DXFLine);
                p->v0 = v[0];
                p->v1 = v[1];
                if (colour != -1)
                    p->colour = colour;
                else
                    p->colour = GetLayerColourDXF(layer);
                m_entities.push_back(std::move(p));
                colour = -1; layer.clear();
                v[0] = v[1] = v[2] = v[3] = DXFVector();
                state = 0;
            }
        }
        if (line1 == "0" && line2 == "ENDSEC") {    // Fin normale d'une section ENTITIES d'un fichier dxf
            type_return = true;
            goto Sortie;
        }
        else if (line1 == "0" && line2 == "3DFACE") // initialisation d'une nouvelle 3DFACE
            state = 1;
        else if (line1 == "0" && line2 == "LINE")   // initialisation d'une nouvelle LINE
            state = 2;
        else if (state > 0)                         // Ici on lit les propriétés successives des 3DFACE ou LINE (coordonnées de points en x,y,z, couleur, layer)
        {
            const float d = float(ToDoubleDXF(line2));

            Nvector = 3;                            // Facettes triangulaires
            if (line1 == "10")
                v[0].x = d;
            else if (line1 == "20")
                v[0].y = d;
            else if (line1 == "30")
                v[0].z = d;
            else if (line1 == "11")
                v[1].x = d;
            else if (line1 == "21")
                v[1].y = d;
            else if (line1 == "31")
                v[1].z = d;
            else if (line1 == "12")
                v[2].x = d;
            else if (line1 == "22")
                v[2].y = d;
            else if (line1 == "32")
                v[2].z = d;
            else if (line1 == "13")
                v[3].x = d;
            else if (line1 == "23")
                v[3].y = d;
            else if (line1 == "33") {
                v[3].z = d;
                Nvector= 4;                     // Facettes à 4 sommets (en fait il faudrait que "13", "23" et "33" soient tous 3 vus dans des line1 successifs)
            }
            else if (line1 == "8") {            // layer
                layer = line2;
                if (layer != old_layer && state == 1) { // Pour créer un nouvel objet, basé sur layer, mais seulement avec des 3DFACE, pas des LINE
                    p_objets.name       = layer;
                    p_objets.num_3DFACE = nb3DFACE;
                    p_objets.num_Vector = nbVector;
                    m_objets.push_back(p_objets);
                    old_layer = layer;          // Mémoriser layer dans old_layer pour la comparaison suivante
                    nbLayer++;                  // Incrémenter nbLayer (comptage du nombre d'objets)
//                    printf("\n%4d 3DFACE : %6d num_deb : %6d nom : %s ", nbLayer, nb3DFACE, nbVector, layer.mb_str().data());
                }
            }
            else if (line1 == "62")             // colour
            {
                long l;
                line2.ToLong(&l);
                colour = l;
            }
        }
    }
    type_return = false;    // On ne devrait pas sortir par ici. Il manque quelquechose en fin de fichier ? Un ENDSEC non trouvé ?

Sortie:
    Update_Dialog((long long)stream.TellI(), fichierBdd_length);   // Ici, on ne devrait pas être loin de la fin du fichier

//    if (EnCours)
//        printf("\n");
    if (verbose) printf("Sortie de BddInter::ParseEntitiesDXF     : nb3DFACE : %d, nbLINE : %d, nbLayer : %d, nbVector : %d en %s\n",nb3DFACE,nbLINE,nbLayer,nbVector,(type_return ? "true" : "false"));
    Fermer_progress_dialog();
    nb3DFACE_DXF = nb3DFACE;
    nbLINE_DXF   = nbLINE;
    nbPoints_DXF = nbVector;
    return type_return;
}

// parse and load a DXF file
// currently pretty limited, but knows enough to handle 3DFACEs and LINEs
bool BddInter::LoadDXF(wxInputStream& stream)
{
    wxString line1, line2;

    if (verbose) printf("Entree de BddInter::LoadDXF\n");
    ClearDXF();
    wxTextInputStream text(stream);

    this->clearall(); this->type_fichier = 0;   // Raz des objets => pas d'ajouts/fusions d'objets possibles, remettre type_fichier à la bonne valeur car modifiée dans clearall().

    while (stream.CanRead())
    {
        GetLines(text, line1, line2);
        if (line1 == "999") // comment
            continue;
        else if (line1 == "0" && line2 == "SECTION")
        {
            GetLines(text, line1, line2);
            if (line1 == "2")
            {
                if (line2 == "HEADER")
                {
                    if (!ParseHeaderDXF(stream))
                        return false;
                }
                else if (line2 == "TABLES")
                {
                    if (!ParseTablesDXF(stream))
                        return false;
                }
                else if (line2 == "ENTITIES")
                {
                    if (!ParseEntitiesDXF(stream))
                        return false;
                }
            }
        }
    }
    Message[0] = '\0';          // Par précaution, vider Message
    NormalizeEntitiesDXF();     // Normaliser et recentrer dans [-5,+5]
    m_loaded = true;
    m_gllist = 0;
    if (verbose) printf("Sortie de BddInter::LoadDXF\n");
    return true;
}

inline float mymin(float x, float y) { return x < y ? x : y; } // On pourrait utiliser std::min et std::max (mais pas forcément plus efficace)
inline float mymax(float x, float y) { return x > y ? x : y; }

// Scale object boundings to [-5,5]
void BddInter::NormalizeEntitiesDXF()
{
    // calculate current min and max boundings of object
    DXFVector minv( 10e20f,  10e20f,  10e20f);
    DXFVector maxv(-10e20f, -10e20f, -10e20f);
	int nbvec,compteur=0;

	if (verbose) printf("Entree de BddInter::NormalizeEntitiesDXF\n");

    for (auto& entity : m_entities)
    {
        if (entity->type == DXFEntity::Line)
        {
            compteur++;
            DXFLine *line = (DXFLine *)entity.get();
            const DXFVector *v[2] = { &line->v0, &line->v1 };
            for (int i = 0; i < 2; ++i)
            {
                minv.x = mymin(v[i]->x, minv.x);
                minv.y = mymin(v[i]->y, minv.y);
                minv.z = mymin(v[i]->z, minv.z);
                maxv.x = mymax(v[i]->x, maxv.x);
                maxv.y = mymax(v[i]->y, maxv.y);
                maxv.z = mymax(v[i]->z, maxv.z);
            }
        } else if (entity->type == DXFEntity::Face)
        {
            compteur++;
            DXFFace *face = (DXFFace *)entity.get();
            const DXFVector *v[4] = { &face->v0, &face->v1, &face->v2, &face->v3 };
            nbvec = 3;
            if (face->Nb_DXFVector != 3) nbvec = 4; // 3 points pour des facettes triangulaires, sinon 4
            for (int i = 0; i < nbvec; ++i)
            {
                minv.x = mymin(v[i]->x, minv.x);
                minv.y = mymin(v[i]->y, minv.y);
                minv.z = mymin(v[i]->z, minv.z);
                maxv.x = mymax(v[i]->x, maxv.x);
                maxv.y = mymax(v[i]->y, maxv.y);
                maxv.z = mymax(v[i]->z, maxv.z);
            }
        }
    }

    // rescale object down to [-5,5]
    DXFVector span(maxv.x - minv.x, maxv.y - minv.y, maxv.z - minv.z);
    float factor = mymin(mymin(10.0f / span.x, 10.0f / span.y), 10.0f / span.z);

    if (compteur == 0) {
        printf("DXFRenderer::m_entities est vide\n");
        goto Sortie;
    }

    sprintf(Message,"Recadré sur [-5,+5] avec un facteur de %8.3e\n",factor) ;
    printf("%s",utf8_To_ibm(Message)) ;

    for (auto& entity : m_entities)
    {
        if (entity->type == DXFEntity::Line)
        {
            DXFLine *line = (DXFLine *)entity.get();
            DXFVector *v[2] = { &line->v0, &line->v1 };
            for (int i = 0; i < 2; ++i)
            {
                v[i]->x -= minv.x + span.x/2; v[i]->x *= factor;
                v[i]->y -= minv.y + span.y/2; v[i]->y *= factor;
                v[i]->z -= minv.z + span.z/2; v[i]->z *= factor;
            }
        } else if (entity->type == DXFEntity::Face)
        {
            DXFFace *face = (DXFFace *)entity.get();
            DXFVector *v[4] = { &face->v0, &face->v1, &face->v2, &face->v3 };
            nbvec = 3;
            if (face->Nb_DXFVector != 3) nbvec = 4; // 3 points pour des facettes triangulaires, sinon 4
            for (int i = 0; i < nbvec; ++i)
            {
                v[i]->x -= minv.x + span.x/2; v[i]->x *= factor;
                v[i]->y -= minv.y + span.y/2; v[i]->y *= factor;
                v[i]->z -= minv.z + span.z/2; v[i]->z *= factor;
            }
        }
    }

Sortie:
    if (verbose) printf("Sortie de BddInter::NormalizeEntitiesDXF\n");
}

// OpenGL renderer for DXF entities : Note si Render() const , comme dans le sample wxwidgets penguin, on a une erreur d'affectation sur m_gllist
// Le stockage dans BddInter est fait dans cette version (sortie de la classe DXFRenderer et intégrée à BddInter)

// Compatibilité avec chargement d'autres fichiers dans d'autres formats (en cas de fusion/ajout d'une bdd) à voir ...
// On peut avoir plusieurs objets, 1 à chaque nouveau layer (quand line1 == "8" cf ParseEntitiesDXF plus haut). Piloté par Forcer_1_Seul_Objet
// Il faut impérativement passer dans RenderDXF lors de la lecture d'un .dxf car c'est là qu'on initilise la compatibilité avec BddInter

void BddInter::RenderDXF()
{
    Object *Objet_courant;
    int nfac, npoint;
    unsigned int i, o;
    std::vector<int> Numeros_Sommets;
    unsigned int nb_groupes = 0;
    unsigned int nb_facettes_loc;
    int index_grp = 0;
    bool i_warn   = false ;

    if (!m_loaded) return; // Base non chargée => ne rien faire d'autre

    unsigned Nb_objets = m_objets.size();
//    unsigned int indice_premierObjet = this->Objetlist.size();   // indice du premier objet de l'objet à charger, ici 0 pour l'instant

//    bool Forcer_1_Seul_Objet = false;    // En commun avec celui pour les obj
    if(Forcer_1_Seul_Objet) Nb_objets = 1 ;

    if( m_gllist == 0 ) {
        m_gllist = glliste_objets; //glGenLists( 1 ) ;
        glNewList( m_gllist, GL_COMPILE_AND_EXECUTE );
        int nb3DFACE_debut  = 0;
        int nbPoints_debut  = 0;
        indiceObjet_courant = 0;

        nfac = npoint = 0;
        o = 1;  // Numéro d'objet (ici, on commence la numérotation à 1)

        for (const auto& entity : m_entities)
        {
            // On traitera la couleur entity->colour pour en faire des groupes (comme dans LoadPLY via buildGroupes) mais seulement pour des facettes
            wxColour c = ACIColourToRGB(entity->colour);
            if (entity->type == DXFEntity::Line)    // Pour l'instant, affichage via RenderDXF, mais pas de stockage des points/sommets dans la BddInter
            {
                DXFLine *line = (DXFLine *)entity.get();
                glBegin(GL_LINES);
                glColor3f(c.Red()/255.0f, c.Green()/255.0f, c.Blue()/255.0f);
                glVertex3f(line->v0.x, line->v0.y, line->v0.z);
                glVertex3f(line->v1.x, line->v1.y, line->v1.z);
                glEnd();
            }
            else if (entity->type == DXFEntity::Face)
            {
                if (nfac == 0) {
                    str.Printf(_T("<OBJET> %d ") + wxFileName::StripExtension(wxFileNameFromPath(get_file())),o);
                    if (o > 1) str.Append("_" + m_objets[o-1].name);
//        printf("indiceObjet_courant avant = %d\n",indiceObjet_courant);
                    make_objet();                                                                       // Pour le moment, 1 seul objet
//        printf("indiceObjet_courant apres = %d\n",indiceObjet_courant);
                    Objet_courant = &(this->Objetlist[indiceObjet_courant]);    // indiceObjet_courant vaut quoi ici ?
                    nb3DFACE_debut = m_objets[o-1].num_3DFACE;
                    nbPoints_debut = m_objets[o-1].num_Vector;
                    if (o == Nb_objets) {
                        Objet_courant->Nb_facettes = nb_facettes_loc = nb3DFACE_DXF - nb3DFACE_debut;
                        Objet_courant->Nb_sommets                    = nbPoints_DXF - nbPoints_debut;
                    } else {
                        Objet_courant->Nb_facettes = nb_facettes_loc = m_objets[o].num_3DFACE - nb3DFACE_debut;
                        Objet_courant->Nb_sommets                    = m_objets[o].num_Vector - nbPoints_debut;
                    }

                    this->N_elements = Objet_courant->Nb_facettes;
                    str.clear();
                    make_face();
                    make_normale();

                    this->N_elements = Objet_courant->Nb_sommets;
                    make_sommet();
                }
                buildGroupes(nb_groupes, index_grp, i_warn, entity->colour);    // Ne construire les groupes que pour les facettes (3DFACE)
                DXFFace *face = (DXFFace *)entity.get();
                Numeros_Sommets.resize(face->Nb_DXFVector);
                this->str.clear();
                if (face->Nb_DXFVector == 3) {
                    glBegin(GL_TRIANGLES);
                    glColor3f(c.Red()/255.0f, c.Green()/255.0f, c.Blue()/255.0f);
                    glNormal3f(face->n.x,  face->n.y,  face->n.z);
                    glVertex3f(face->v0.x, face->v0.y, face->v0.z);
                    glVertex3f(face->v1.x, face->v1.y, face->v1.z);
                    glVertex3f(face->v2.x, face->v2.y, face->v2.z);
                    glEnd();
			    } else {
			        glBegin(GL_POLYGON);            // GL_QUADS conviendrait aussi
                    glColor3f(c.Red()/255.0f, c.Green()/255.0f, c.Blue()/255.0f);
                    glNormal3f(face->n.x,  face->n.y,  face->n.z);
                    glVertex3f(face->v0.x, face->v0.y, face->v0.z);
                    glVertex3f(face->v1.x, face->v1.y, face->v1.z);
                    glVertex3f(face->v2.x, face->v2.y, face->v2.z);
                    glVertex3f(face->v3.x, face->v3.y, face->v3.z);
                    glEnd();
                }
                ++npoint;                       // 1er sommet
                this->Setxyz(face->v0.x, face->v0.y, face->v0.z); this->N_elements = npoint; this->make_1_sommet(); Numeros_Sommets[0] = npoint;
                ++npoint;                       // 2nd sommet
                this->Setxyz(face->v1.x, face->v1.y, face->v1.z); this->N_elements = npoint; this->make_1_sommet(); Numeros_Sommets[1] = npoint;
                ++npoint;                       // 3ème sommet
                this->Setxyz(face->v2.x, face->v2.y, face->v2.z); this->N_elements = npoint; this->make_1_sommet(); Numeros_Sommets[2] = npoint;
                if (face->Nb_DXFVector != 3) {
                   ++npoint;                    // 4ème sommet (Nb_DXFVector devrait donc valoir 4 normalement)
                   this->Setxyz(face->v3.x, face->v3.y, face->v3.z); this->N_elements = npoint; this->make_1_sommet(); Numeros_Sommets[3] = npoint;
                }
                N_elements = ++nfac;
                this->Set_numeros(Numeros_Sommets);             // Créer la facette
                make_1_face();
                this->Setxyz(face->n.x, face->n.y, face->n.z);  // Créer la Normale au barycentre de la facette
                make_1_normale();

            // Affectation d'une numéro de groupe et/ou de matériau à la facette
                Objet_courant->Facelist[nfac-1].groupe     = index_grp; // Comme dans LoadPLY, mais on pourrait y mettre la valeur par défaut groupe_def
                Objet_courant->Facelist[nfac-1].codmatface = index_grp; //    "        "       "       "       "       "       "       "      codmatface_def

                if (nfac == (int)nb_facettes_loc) { // On a fini avec cet objet. Passer à l'objet suivant
                    nfac = npoint = nb3DFACE_debut = nbPoints_debut = 0;
                    o++;
                }
			}
        }
        glEndList();

        if (this->Objetlist.size() == 0) {
            sprintf(Message,"Ce fichier ne peut pas être décodé correctement par DXFRenderer : pas de mots clés ENTITIES/3DFACE ni LINE\n");
            printf("%s",utf8_To_ibm(Message)) ;
        } else {
            printf("%s",utf8_To_ibm(Message)) ; // Rappel du Message précédent concernant le facteur d'échelle

            sprintf(Message,"Nombre de groupes identifiés : %d\nIndice CouleurDXF\n",nb_groupes) ;
            printf("%s",utf8_To_ibm(Message)) ;
            for (i=1; i<=nb_groupes; i++) printf("  %2d  %8d\n",i,Groupe_num[i]) ;

            for (i=0; i<this->Objetlist.size(); ++i) Genere_Liste_Groupes_Materiaux(i);
            this->Search_Min_Max();     // Indispensable pour initialiser correctement certaines valeurs de BddInter
            this->bdd_modifiee = false; // Initialisation de précaution
//        printf("Nb_Objets : %lld\n",m_objets.size());
//        for (i=0; i<m_objets.size(); i++) printf("%4d %6d %6d nom : %s\n",i+1,m_objets[i].num_3DFACE,m_objets[i].num_Vector,m_objets[i].name.mb_str().data());
        }
    } else {
        glCallList(m_gllist);
    }
}

//////////////////////////////////////////////////
// *** Enregistrement de divers formats de BDD ***
//////////////////////////////////////////////////

void BddInter::SaveTo(wxString str, int index) {

// Pour enregistrer sous différents formats, le format .bdd est celui par défaut (tous les formats lus ne sont pas accessibles en écriture)

    if(OK_ToSave) {
        int type_saveTo = -1;
        wxString local_str = str ;
        local_str.MakeLower();                          // Forcer le nom de fichier à être en minuscules

        if (local_str.EndsWith(_T(".bdd"))) {           // Teste si l'extension est .bdd
            type_saveTo = 1;
        } else if (local_str.EndsWith(_T(".obj"))) {    // Teste si l'extension est .obj
            type_saveTo = 2;
        } else if (local_str.EndsWith(_T(".g3d"))) {    // Teste si l'extension est .g3d
            type_saveTo = 3;
        } else if (local_str.EndsWith(_T(".off"))) {    // Teste si l'extension est .off
            type_saveTo = 4;
        } else if (local_str.EndsWith(_T(".stl"))) {    // Teste si l'extension est .stl
            type_saveTo = index+1;                            // Via index on sait si c'est Ascii ou Binaire
        } else if (local_str.EndsWith(_T(".dxf"))) {    // Ne devrait jamais arriver !!!
            type_saveTo = 0;
        }
        if (type_saveTo == -1) {
            // Extension absente ou non reconnue => forcer l'ajout de .bdd (pas forcément utile car fait via le dialogue saveFileDialog dans OvniFrame::OnMenu_Enregistrer_Sous
            str += _T(".bdd");
            type_saveTo = 1;
        }

//        printf("saving type                        %d\n",type_saveTo);

        if(type_saveTo > 0) {
            buffer = str.utf8_str();
            sprintf(Message,"Enregistrement de :\n%s\n",buffer.data());
            printf("%s",utf8_To_ibm(Message));
            switch(type_saveTo) {
            case 0:                         // En réserve, mais ne peut pas se produire ici !
                //m_renderer.Save(stream);
                break;
            case 1:
                SaveBDD(str);
                break;
            case 2:
                SaveOBJ(str);
                break;
            case 3:
                SaveG3D(str);
                break;
            case 4:
                SaveOFF(str);
                break;
            case 5:
                SaveSTL(str, true);     // Appel en mode Ascii
                break;
            case 6:
                SaveSTL(str, false);    // Appel en mode binaire
                break;
            default:
                wxMessageBox(_T("Type de fichier non reconnu!"),_T("Error"));
                break;
            }
        } else {
            wxMessageBox(_T("Type de fichier non valide!"),_T("Error"));
        }

    }
}

// Les boucles for des fonctions Save* ne sont a priori pas parallélisables : les fichiers créés doivent être séquentiels

void BddInter::WarningAccess(wxString str) {
    wxString Msg = _T("Écriture dans le fichier ") + str + _(" impossible !"); //wxNomsFichiers est OK sauf si sauvegarde automatique
    DisplayBox(Msg,"Avertissement");//        wxMessageBox(Msg,"Avertissement");
}

void BddInter::SaveBDD(wxString str) {
    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur = 0;
    unsigned int compteur_sommets = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;

    wxString str_loc = wxFileNameFromPath(str); // Traitement particulier pour la sauvegarde automatique dans Autosave.bdd

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        WarningAccess(str_loc);                 // wxNomsFichiers est OK sauf si sauvegarde automatique
        return;
    }

    printf("\nNombre d'objets : %d\n",(int)this->Objetlist.size());

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) continue;                            // Objet sans facettes => l'ignorer

        compteur_sommets = 0;
        NewVecteurs.clear();
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
//            if(objet_courant->Sommetlist[j].show == true) {   // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
//                compteur++;                                 // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
//                objet_courant->Sommetlist[j].Numero = compteur; // Pourquoi ??? car modifie la base, mais important si soudures ?
//            }
        }
        myfile << "<OBJET> ";
        myfile << objet_courant->GetValue();
        myfile << " ";
        myfile << objet_courant->GetName();                           // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//        wxString mystring = this->Objetlist[i].GetwxName();                 // Récupère le nom wxString
//        std::string stlstring = std::string(mystring.mb_str(wxConvUTF8));   // et convertion en UTF8
//        myfile << stlstring;
//        wxCharBuffer buffer = mystring.ToUTF8();                            // Revient au même que les 2 lignes précédentes
//        myfile << buffer.data();                                            // mais écriture plus claire !
        myfile << "\n";
        myfile << "\n";

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        myfile << "\t<FACE>\t";
        myfile << compteur ;
        myfile << "\n";
        myfile << "\n";

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;   // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // => on ne travaille que sur les facettes non supprimées !
            numeros_Sommets = Face_ij->F_sommets; // <=> Face_ij->getF_sommets();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t";
            myfile << std::setw(2) << numeros_Sommets.size();
            for (k=0; k<numeros_Sommets.size(); k++) {
                myfile << "\t";
//                myfile << std::setw(5) << objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero;     // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << std::setw(5) << numeros_Sommets[k];
            }
            myfile << "\n";
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        unsigned int compteur_facettes = compteur;   // A conserver car resert pour les normales, aspect_faces, luminances
//        printf("\n Face end");
        compteur = 0;
        myfile << "\n\t<SOMMET>\t";
        myfile << compteur_sommets;
        myfile << "\n";
        myfile << "\n";
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {      // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                compteur++;
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                myfile << "\t";
                myfile << std::setw(5) << compteur;
//                myfile << std::setw(5) << objet_courant->Sommetlist[j].Numero;
                for (k=0; k<xyz_sommet.size(); k++) {
                    myfile << "\t";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
////            }
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        myfile << "\n\t<NORMALE>\t";
        myfile << std::setw(5) << compteur_facettes;
        myfile << "\n";
        myfile << "\n";
//        for (j=0; j<compteur_facettes; j++) {                  // NON car en cas de soudure, on saute des facettes
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;
            compteur++;
            xyz_sommet = Face_ij->getNormale_b();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            for (k=0; k<xyz_sommet.size(); k++) {
                myfile << "\t";
                myfile << std::fixed << std::setprecision(8)<< std::setw(11) << xyz_sommet[k];
            }
            myfile << "\n";
        }
        compteur = 0;
        myfile << "\n\t<ASPECT_FACE>\t";
        myfile << compteur_facettes ;
        myfile << "\n";
        myfile << "\n";
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;
            compteur++;
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t<GROUPE>\t\t";
            myfile << Face_ij->getGroupe();
            myfile << "\n";
            if(Face_ij->getCodmatface() != codmatface_def) {
                myfile << "\t\t\t";
                myfile << "<CODMATFACE>\t";
                myfile << Face_ij->getCodmatface();
                myfile << "\n";
            }
            myfile << "\n"; // Forcer une ligne blanche après GROUPE, CODMATFACE ou les 2 groupés
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        numeros_Sommets.clear();
//        if (test_seuil_gouraud && Enr_Normales_Seuillees) {
            nouvel_indice = objet_courant->Nb_vecteurs +1;
//        }
        unsigned compteur_luminances = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
           if (!objet_courant->Facelist[j].deleted) {
                numeros_Sommets = objet_courant->Facelist[j].getL_sommets();
                compteur_luminances += numeros_Sommets.size();
            }
            // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
            if (compteur_luminances != 0) break;
        }

        if (compteur_luminances == 0) continue ;        // Pas de luminances => objet à facettes planes => pas de vecteurs => passer à l'objet suivant

        myfile << "\n\t<LUMINANCE>\t";
        myfile << compteur_facettes;                    // Le nombre de luminances est égal au nombre de facettes
        myfile << "\n";
        myfile << "\n";
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) {
//                printf("j=%d ",j);  // Bug : si on le met, OK quand souder a été fait, sinon plante (du moins avec g++ 8.1, car semble OK en 9.3 de Msys2 !)
                continue ;
            }
            compteur++;
//            if (compteur == 1) printf("\n");    // Bug : OK si souder et exe normal, mais enlever en mode Debug !!!!!
            numeros_Sommets = Face_ij->getL_sommets();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t";
            myfile << std::setw(2) << numeros_Sommets.size();
            bool test_composite = (test_seuil_gouraud && Enr_Normales_Seuillees) || (Face_ij->flat) ;
            if (test_composite) {
                NormaleFacette = Face_ij->getNormale_b();
            }
            for (k=0; k<numeros_Sommets.size(); k++) {
                myfile << "\t";
                if (test_composite) {
                    if (Face_ij->flat) {
                        NormaleSommet = NormaleFacette; // Dans ce cas, on remplace la normale au sommet par la normale au barycentre de la facette
                        test_np = true;
                    } else {
                        NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                        test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                    }
                    if (test_np) {
                    // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                        int test   = 0;
                        bool found = false;
                        for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                            test = 0;
                            if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                            if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                            if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                            if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                myfile << std::fixed << std::setw(5) << objet_courant->Nb_vecteurs+ 1 +jj;
                                found = true;
                                break;  // Abandon de l'exploration du for jj
                            }
                        }
                        if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                            myfile << std::fixed << std::setw(5) << nouvel_indice++;
                            New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                            NewVecteurs.push_back(New_vecteur);
                        }
                    } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                        myfile << std::fixed << std::setw(5) << numeros_Sommets[k];
                    }
                } else { // Pas d'enregistrement de normales seuillées.
                    myfile << std::fixed << std::setw(5) << numeros_Sommets[k];
                }
            }
            myfile << "\n";
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        myfile << "\n\t<VECTEUR>\t";
//        int NbVecteurs = objet_courant->Nb_vecteurs;
        int NbVecteurs = objet_courant->Vecteurlist.size();     // mieux mais soucis en cas de soudures !
//        printf("%d\n",NbVecteurs);
        NbVecteurs += NewVecteurs.size();
//        printf("%d\n",NbVecteurs);
//        fflush(stdout);
        myfile << std::setw(5) << NbVecteurs;
        myfile << "\n";
        myfile << "\n";
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
//            if(objet_courant->Vecteurlist[j].show == true) {   // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                compteur++;
                NormaleSommet = objet_courant->Vecteurlist[j].point;
                myfile << "\t";
                myfile << std::setw(5) << compteur;
                for (k=0; k<3; k++) {
                    myfile << "\t";
                    myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                }
                myfile << "\n";
                myfile.flush(); // Forcer l'écriture sur le fichier
//            }
        }
        for (j=0; j<NewVecteurs.size(); j++) {
            compteur++;
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
            myfile << "\n";
        }
        myfile << "\n";
        myfile.flush(); // Forcer l'écriture sur le fichier
    }
    myfile << "\n";
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm  *timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveOBJ(wxString str) {

// A vérifier ...

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<int>   numeros_Sommets_L;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur   = 0;
    int compteur_o = 0;
    int offset_vertices = 0;
    int offset_normales = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;
    int last_groupe = codmatface_def;
    int current_groupe;

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        WarningAccess(wxNomsFichiers);
        return;
    }

    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    myfile << "# Fichier Wavefront obj créé par Ovni\n";

    if (mtllib_OK) myfile << "\n" << mtllib_nom ;               // Restitution de la ligne mtllib sauvegardée (entre 2 lignes blanches)

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }
        compteur_o++;

        NewVecteurs.clear();
        unsigned int compteur_sommets = 0;
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
////                compteur++;                                         // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
////                objet_courant->Sommetlist[j].Numero = compteur;     // Pourquoi ??? car modifie la base
////            }
        }
        myfile << "\ng ";
        myfile << objet_courant->GetName();                         // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//        wxString mystring = objet_courant->GetwxName();
//        wxCharBuffer buffer = mystring.ToUTF8();
//        myfile << buffer.data();
        myfile << "\n";

        myfile << "# " << compteur_sommets << " Sommets\n";

        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                myfile << "v";
                for (k=0; k<xyz_sommet.size(); k++) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
////            }
        }

//        compteur = 0;
        numeros_Sommets.clear();
//        if (test_seuil_gouraud && Enr_Normales_Seuillees) {
            nouvel_indice = objet_courant->Nb_vecteurs +1 + offset_normales ;   // Indice de départ pour stocker les nouvelles normales seuillées
//        }
        unsigned compteur_luminances = 0;
        if (objet_courant->Nb_luminances != 0) {
            for (j=0; j<objet_courant->Facelist.size(); j++) {
                Face_ij = &(objet_courant->Facelist[j]);
                if(!(Face_ij->deleted || Face_ij->flat)) {
                    numeros_Sommets = Face_ij->getL_sommets();
                    compteur_luminances += numeros_Sommets.size();
                }
                // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
                if (compteur_luminances != 0) break;
            }
        }
        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(!objet_courant->Facelist[j].deleted) {   // Test original sur .show, mais une facette masquée ne l'est qu'à l'affichage !
                compteur++;                             // Donc compter toutes les facettes non supprimées (.show original != .afficher !)
            }
        }

        myfile << "\n# " << compteur << " Elements\n";

        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            current_groupe = Face_ij->getGroupe();
            if (current_groupe != last_groupe) {
                if ((current_groupe <= 0) || (nb_mat == 0)) {
                    myfile << "usemtl group_";
                    if (current_groupe < 10) myfile << "0";             // Pour forcer un nom comme group_01, ... group_09, group_10,...
                    if (current_groupe <= 0) current_groupe = 0;        // pour éviter un codage comme group_0-123 par exemple et y mettre group_00
                    myfile << current_groupe << "\n";
                    last_groupe = current_groupe;
                } else {
                    myfile << "usemtl ";//group_";
                    myfile << tab_mat[current_groupe -1];               // ne donne pas tout à fait le résultat escompté mais semble OK (?)
                    last_groupe = current_groupe;
                }
            }

            numeros_Sommets = Face_ij->F_sommets;
            myfile << "f";
            if (compteur_luminances != 0) {                         // Il y a des des normales aux sommets
                numeros_Sommets_L = Face_ij->getL_sommets();
                if (test_seuil_gouraud && Enr_Normales_Seuillees) {
                    NormaleFacette = Face_ij->getNormale_b();
                }
            }
            for (k=0; k<numeros_Sommets.size(); k++) {
                myfile << " ";
//                myfile << (objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero + offset_vertices); // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << (numeros_Sommets[k] + offset_vertices);
                if(Face_ij->flat) continue;                         // Facette plane, ne pas enregistrer les normales aux sommets
                if (compteur_luminances != 0) {                     // Il y des des normales aux sommets
                    if (numeros_Sommets_L.size() == 0) continue;    // Si pas de normales aux sommets sur cette facette en particulier, passer à la suivante
                    myfile << "//";                                 // le champ entre les 2 / est réservé aux textures. Non utilisé dans Ovni
                    if (test_seuil_gouraud && Enr_Normales_Seuillees) {
                        NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                        test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                        if (test_np) {
                        // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                            int  test  = 0;
                            bool found = false;
                            for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                                test   = 0;
                                if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                                if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                                if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                                if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                    myfile << objet_courant->Nb_vecteurs + 1 +jj + offset_normales;
                                    found = true;
                                    break;  // Abandon de l'exploration du for jj
                                }
                            }
                            if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                                myfile << nouvel_indice++;
                                New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                                NewVecteurs.push_back(New_vecteur);
                            }
                        } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                            myfile << numeros_Sommets_L[k] + offset_normales;
                        }
                    } else { // Pas d'enregistrement de normales seuillées.
                        myfile << numeros_Sommets_L[k] + offset_normales;
                    }
                }
            }
            myfile << "\n";
        }

        int NbVecteurs = objet_courant->Vecteurlist.size();         // mieux mais soucis possibles en cas de soudures !
        NbVecteurs += NewVecteurs.size();

        if (NbVecteurs != 0) {                                      // Enregistrer les normales aux sommets
            myfile << "\n# " << NbVecteurs << " Normales aux sommets\n";

            for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
                    NormaleSommet = objet_courant->Vecteurlist[j].point;
                    myfile << "vn";
                    for (k=0; k<3; k++) {
                        myfile << " ";
                        myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                    }
                    myfile << "\n";
    //            }
            }
            for (j=0; j<NewVecteurs.size(); j++) {
                myfile << "vn";
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
                myfile << "\n";
            }
            myfile << "\n";
        }
        offset_vertices += compteur_sommets;
        offset_normales += NbVecteurs;

    }
    if ((int)this->Objetlist.size() != compteur_o)
        printf("Nombre d'objets retenus  : %d\n",compteur_o);

    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm  *timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveOFF(wxString str) {

// A vérifier ...

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur = 0;
    int total_vertices = 0;
    int total_facettes = 0;
    int offset_vertices= -1;    // Dans ce format, on commence la numérotation des sommets à 0 et non 1 !
    unsigned int compteur_sommets;
    unsigned o,j,k ;
    bool commentaires  = false; // Pour ajouter des commentaires (lignes commençant par #). Mais pose problèmes avec certain logiciels !

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        WarningAccess(wxNomsFichiers);
        return;
    }
    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    myfile << "OFF\n";
// ATTENTION : certain logiciels (par ex Deep Exploration) n'acceptent pas les lignes de commentaires
    if (commentaires) myfile << "# Fichier Object File Format off créé par Ovni\n";

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }

        compteur_sommets = 0;
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
                compteur_sommets++;
////            }
        }
        total_vertices += compteur_sommets;

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Test original sur .show, mais une facette masquée ne l'est qu'à l'affichage !
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        total_facettes += compteur;
    }
    myfile << total_vertices << " " << total_facettes << " 0\n";

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        if (commentaires) {
// ATTENTION : certains logiciels (par ex Deep Exploration) n'acceptent pas les lignes de commentaires
            myfile << "# Objet initial : ";
            myfile << objet_courant->GetName();                         // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//            wxString mystring = objet_courant->GetwxName();
//            wxCharBuffer buffer = mystring.ToUTF8();
//            myfile << buffer.data();
            myfile << "\n";
        }

        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                for (k=0; k<xyz_sommet.size(); k++) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
////            }
        }
    }
    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            numeros_Sommets = Face_ij->F_sommets;
            myfile << numeros_Sommets.size();
            for (k=0; k<numeros_Sommets.size(); k++) {
                myfile << " ";
//                myfile << (objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero + offset_vertices); // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << (numeros_Sommets[k] + offset_vertices);
            }
            myfile << "\n";
        }

        compteur_sommets = 0;
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
                compteur_sommets++;
////            }
        }

        offset_vertices += compteur_sommets;
    }
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm  *timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveSTL(wxString str, bool ascii) {

// Construit à partir de SaveOFF
// si ascii est true => mode Ascii, sinon mode binaire (fusion de SaveSTL_Ascii et SaveSTL_Binary)

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    UINT32 compteur = 0;
    UINT16 Attribute= 0;
    unsigned int o,i,j,k,len ;

    buffer=str.mb_str();
    std::ofstream myfile;
    if (ascii) {
        myfile.open (buffer.data());
    } else {
        myfile.open(buffer.data(), std::ofstream::binary);
    }
    if (!myfile.is_open()) {
        WarningAccess(wxNomsFichiers);
        return;
    }

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;
            compteur++;                                         // Compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }
    }

    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    if (ascii) {
        myfile << "solid " << this->Objetlist[0].GetName() << "\n";
    } else {
        len = strlen(this->Objetlist[0].GetName())    ;
        myfile.write(this->Objetlist[0].GetName(),len);
        for (i=len+1; i<=80; i++) myfile.write(" ",1)    ;
        myfile.write((char *)&compteur,sizeof(UINT32));
    }

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        for (i=0; i<objet_courant->Facelist.size(); i++) {
            Face_ij = &(objet_courant->Facelist[i]);
            if(Face_ij->deleted) continue;
            if (ascii) myfile << "  facet normal";
            xyz_sommet = Face_ij->getNormale_b();
            for (j=0; j<3; j++) {
                if (ascii) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[j] ;
                } else {
                    myfile.write((char *)&xyz_sommet[j], sizeof(float)) ;
                }
            }
            if (ascii) myfile << "\n" << "    outer loop\n";
            numeros_Sommets = Face_ij->F_sommets;
            for (j=0; j<3; j++) {    // numeros_Sommets.size() doit être = 3
                xyz_sommet = objet_courant->Sommetlist[numeros_Sommets[j]-1].getPoint();
                if (ascii) myfile << "      vertex";
                for (k=0; k<3; k++) {
                    if (ascii) {
                        myfile << " ";
                        myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                    } else {
                        myfile.write((char *)&xyz_sommet[k],sizeof(float));
                    }
                }
                if (ascii) myfile << "\n";
            }
            if (ascii) {
                myfile << "    endloop\n";
                myfile << "  endfacet\n";
            } else {
                myfile.write((char *)&Attribute,sizeof(UINT16));
            }
        }
    }

    if (ascii) myfile << "endsolid " << this->Objetlist[0].GetName() << "\n";

    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm  *timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveG3D(wxString str) {
    // En cours d'écriture ...
    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<int>   numeros_Sommets_L;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Sommet *Sommet_ij;
    Object *objet_courant;

    int compteur = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm  *timeinfo;
    char buffer_time [16];

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        WarningAccess(wxNomsFichiers);
        return;
    }

    printf("\nNombre d'objets originaux : %d\n",(int)this->Objetlist.size());

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer_time,16,"%d/%m/%Y",timeinfo);              // date du jour courant (format à la française)

    myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";   // Marquer l'encodage comme UTF-8 plutôt que ISO-8859-1, sinon soucis possibles avec caractères accentués !
    myfile << "<g3d version=\"2.0\">\n";
	myfile << "\t<document version=\"1.0\" date=\"";
	myfile << buffer_time ;
	myfile << "\" auteur=\"ONERA\" commentaire=\"Fichier graphique\">\n";
	myfile << "\t\t<source format=\"CRIRA\">\n";
	myfile << "\t\t\t<entete>\n";
	myfile << "\t\t\t\tFichier g3d créé par Ovni\n";
	myfile << "\t\t\t</entete>\n";
	myfile << "\t\t</source>\n";
	myfile << "\t</document>\n";
	unsigned int nb_mat = listeMateriaux.size();
	if (nb_mat != 0) {
        auto it = listeMateriaux.begin();
        myfile << "\t<types_materiaux nbr=\"1\">\n";
        myfile << "\t\t<type_materiau id=\"1\" nom=\"optique\" nbr=\"";
        myfile << nb_mat << "\">\n";
        for (j=0; j<nb_mat; j++,it++) {
            myfile << "\t\t\t<materiau id=\"" << j+1 << "\" nom=\"materiau_";
            if (*it == codmatface_def) {
                myfile << "def";
            } else {
                if (*it < 10) myfile << "0";
                myfile << *it ;
            }
            myfile << "\"/>\n";
        }
        myfile << "\t\t</type_materiau>\n";
        myfile << "\t</types_materiaux>\n";
	}
	myfile << "\t<types_valeurs nbr=\"2\">\n";
    myfile << "\t\t<type_valeur id=\"1\" nom=\"groupe\"/>\n";
    myfile << "\t\t<type_valeur id=\"2\" nom=\"codmatface\"/>\n";
	myfile << "\t</types_valeurs>\n";

	compteur = 0;
	for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        int compteur_F = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur_F++;                                       // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur_F == 0) objet_courant->deleted = true;     // Objet sans facettes => le marquer comme supprimé

	    if (!objet_courant->deleted) compteur++ ;
	}

    if ((int)this->Objetlist.size() != compteur)
        printf("Nombre d'objets retenus   : %d\n",compteur);

    myfile << "\t\t<objets nbr=\"" << compteur << "\">\n";

    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        NewVecteurs.clear();
        unsigned int compteur_sommets = 0;
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
////            if(objet_courant->Sommetlist[j].show == true) {       // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
////                compteur++;                                     // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
////                objet_courant->Sommetlist[j].Numero = compteur; // Pourquoi ??? car modifie la base
////            }
        }

        numeros_Sommets_L.clear();
        nouvel_indice = objet_courant->Nb_vecteurs +1;          // Premier indice libre au-dessus de NB_vecteurs pour l'objet courant

        unsigned compteur_luminances = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(!Face_ij->deleted) {
                numeros_Sommets_L = Face_ij->getL_sommets();
                compteur_luminances += numeros_Sommets_L.size();
            }
            // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
            if (compteur_luminances != 0) break;
        }

        myfile << "\t\t<objet id=\"";
        myfile << objet_courant->GetValue();
        myfile << "\" nom=\"";
//        wxString mystring = objet_courant->GetwxName();                         // Récupère le nom wxString
//        std::string stlstring = std::string(mystring.mb_str(wxConvUTF8));   // et convertion en UTF8
//        myfile << stlstring;
//        wxCharBuffer buffer = mystring.ToUTF8();                                // Revient au même que les 2 lignes précédentes
//        myfile << buffer.data();                                                // mais écriture plus claire !
        myfile << objet_courant->GetName();                                   // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
        myfile << "\" unite=\"\">\n";
        myfile << "\t\t\t<maillages nbr=\"1\">\n";
        myfile << "\t\t\t\t<maillage lod=\"1\">\n";

////        compteur = 0;
        myfile << "\t\t\t\t\t<sommets nbr=\"";
        myfile << compteur_sommets;
        myfile << "\">\n";
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            Sommet_ij = &(objet_courant->Sommetlist[j]);
////            if(objet_courant->Sommetlist[j].show == true) {         // A vérifier.
//                compteur++;
                xyz_sommet = Sommet_ij->getPoint();
                myfile << "\t\t\t\t\t\t<sommet id=\"";
                myfile << (j+1) ; //Sommet_ij->Numero; // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << "\" xyz=\"";
                for (k=0; k<xyz_sommet.size(); k++) {
                    myfile << "\t";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << " \"/>\n";
////            }
        }
        myfile << "\t\t\t\t\t</sommets>\n";
        myfile.flush();

        compteur=0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;        // Test original sur .show
            compteur++;                                             // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        myfile << "\t\t\t\t\t<facettes nbr=\"";
        myfile << compteur ;
        myfile << "\">\n";

        compteur = 0;
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            compteur++;
            myfile << "\t\t\t\t\t\t<facette id=\"";
            myfile << compteur;
            myfile << "\">\n";

            xyz_sommet = Face_ij->getNormale_b();
            myfile << "\t\t\t\t\t\t\t<normale_b xyz=\"";
            for (k=0; k<xyz_sommet.size(); k++) {
                myfile << std::fixed << std::setprecision(8)<< std::setw(11) << xyz_sommet[k];
                myfile << " ";
            }
            myfile << "\"/>\n";
            myfile.flush();

            myfile << "\t\t\t\t\t\t\t<sommets ref=\"";
            numeros_Sommets = Face_ij->F_sommets;
            for (k=0; k<numeros_Sommets.size(); k++) {
//                myfile << objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero;     // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << numeros_Sommets[k];
                myfile << " ";
            }
            myfile << "\"/>\n";
            myfile.flush();

            numeros_Sommets_L=Face_ij->getL_sommets();
            if ((numeros_Sommets_L.size() != 0) || !Face_ij->flat) {
                bool test_composite = (test_seuil_gouraud && Enr_Normales_Seuillees) || (Face_ij->flat) ;
                if (test_composite) {
                    NormaleFacette = Face_ij->getNormale_b();
                }
                myfile << "\t\t\t\t\t\t\t<normales_s ref=\"";
                for (k=0; k<numeros_Sommets_L.size(); k++) {
                    if (test_composite) {
                        if (Face_ij->flat) {
                            NormaleSommet = NormaleFacette; // Dans ce cas, on remplace la normale au sommet par la normale au barycentre de la facette
                            test_np = true;
                        } else {
                            NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                            test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                        }
                        if (test_np) {
                        // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                            int test = 0;
                            bool found = false;
                            for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                                test = 0;
                                if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                                if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                                if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                                if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                    myfile << objet_courant->Nb_vecteurs+ 1 +jj;
                                    found = true;
                                    break;  // Abandon de l'exploration du for jj
                                }
                            }
                            if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                                myfile << nouvel_indice++;
                                New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                                NewVecteurs.push_back(New_vecteur);
                            }
                        } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                              myfile << numeros_Sommets_L[k];
                        }
                    } else { // Pas d'enregistrement de normales seuillées.
                          myfile << numeros_Sommets_L[k];
                    }
                    myfile << " ";
                }
                myfile << "\"/>\n";
            }
            myfile.flush();

            if(Face_ij->getCodmatface() != codmatface_def) {
                myfile << "\t\t\t\t\t\t\t<materiaux ref=\"1 ";
                myfile << Face_ij->getCodmatface();
                myfile << "\"/>\n";
            }
            myfile.flush();

            myfile << "\t\t\t\t\t\t\t<valeurs val=\"2 ";
            myfile << Face_ij->getGroupe();
            myfile << ".0\"/>\n";   // Ici, le format attend un flottant

            myfile << "\t\t\t\t\t\t</facette>\n";
            myfile.flush();
        }
        myfile << "\t\t\t\t\t</facettes>\n";
        myfile.flush();

//        unsigned int compteur_facettes = compteur;   // A conserver car resert pour les normales, aspect_faces, luminances

        if (compteur_luminances != 0) {        // Pas de luminances => objet à facettes planes => pas de vecteurs => passer à l'objet suivant

            compteur=0;
            myfile << "\t\t\t\t\t<normales_s nbr=\"";
    //        int NbVecteurs = objet_courant->Nb_vecteurs;
            int NbVecteurs = objet_courant->Vecteurlist.size();     // mieux mais soucis en cas de soudures !
            NbVecteurs += NewVecteurs.size();
            myfile << NbVecteurs;
            myfile << "\">\n";
            for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
    //            if(objet_courant->Vecteurlist[j].show == true) {   // A vérifier.
                    compteur++;
                    NormaleSommet = objet_courant->Vecteurlist[j].point;
                    myfile << "\t\t\t\t\t\t<normale_s id=\"";
                    myfile << compteur;
                    myfile << "\" xyz=\"";
                    for (k=0; k<3; k++) {
                        myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                        myfile << " ";
                    }
                    myfile << "\"/>\n";
    //            }
            }
            for (j=0; j<NewVecteurs.size(); j++) {
                compteur++;
                myfile << "\t\t\t\t\t\t<normale_s id=\"";
                myfile << compteur;
                myfile << "\" xyz=\"";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
                myfile << " ";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
                myfile << " ";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
                myfile << " \"/>\n";
            }
            myfile << "\t\t\t\t\t</normales_s>\n";
        }
        myfile << "\t\t\t\t</maillage>\n";
		myfile << "\t\t\t</maillages>\n";
		myfile << "\t\t</objet>\n";

    }
    myfile << "\t</objets>\n";
    myfile << "</g3d>\n";
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);                          // Reprendre cette info déjà récupérée en début de SaveG3D
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data()); // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}
