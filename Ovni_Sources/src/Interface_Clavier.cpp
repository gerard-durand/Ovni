#include "Interface.h"

// Gestion de l'interface de BddInter au Clavier
// *********************************************

//#include "OvniMain.h"
//#include "OvniApp.h"

void BddInter::OnKeyLeftRight(wxKeyEvent& event, int signe) {
    float spin_quat[4];
    float degres;
    bool  VerrNum;
    VerrNum = false;
    if (event.GetKeyCode() == 0x0148) VerrNum = true;   // Touche 4 pavé numérique + Verr Num enfoncée
    if (event.GetKeyCode() == 0x014a) VerrNum = true;   // Touche 6   "      "     "   "   "    "

    if (event.ShiftDown() || VerrNum) {                 // Si touche Shift enfoncée (ou VerrNum true) => Rotation
        if (m_gldata.mode_Trackball) {
            trackball(spin_quat, 0., 0., signe*pas_rad, 0.);
            add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
            Reset_Sliders();
        } else {
            m_gldata.rotz += signe*pas_deg;
            degres = fmod(m_gldata.rotz, 360) ;
            if (degres < 0) degres +=360;
            m_gldata.rotz = degres;                     // Utile ou fait dans les sliders ?
            Slider_z->SetValue(lround(degres));
        }
    } else {                                            // Sinon Déplacement
        m_gldata.posx += signe*m_gldata.depl_xy;//1.0f;
    }
}

void BddInter::OnKeyUpDown(wxKeyEvent& event, int signe) {
    float spin_quat[4];
    float degres;
    bool  VerrNum;
    VerrNum = false;
    if (event.GetKeyCode() == 0x0146) VerrNum = true;   // Touche 2 pavé numérique + Verr Num enfoncée
    if (event.GetKeyCode() == 0x014c) VerrNum = true;   // Touche 8   "      "     "   "   "    "

    if (event.ShiftDown() || VerrNum) {                 // Si touche Shift enfoncée ou Verr Num true => Rotation
        if (m_gldata.mode_Trackball) {
            trackball(spin_quat, 0., 0., 0., signe*pas_rad);
            add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
            Reset_Sliders();
        } else {
            m_gldata.rotx -= signe*pas_deg;
            degres = fmod(m_gldata.rotx, 360) ;
            if (degres < 0) degres +=360;
            m_gldata.rotx = degres;                     // Utile ou fait dans les sliders ?
            Slider_x->SetValue(lround(degres));
        }
    } else {                                            // Sinon Déplacement
        m_gldata.posy += signe*m_gldata.depl_xy;//1.0f;
    }
}

void BddInter::OnKeyDown(wxKeyEvent& event) {
    float spin_quat[4];
    float q[4],a[3];
    float degres;
    int test=0;
    std::vector<int>   Numeros;
    std::vector<int>   NumerosF, NumerosJ;
    std::vector<float> xyz_point;
    float X_m, Y_m, Z_m ;
    int   Nb_Valeurs;
    int   GrpMat;
    bool  Touche_Maj, liste_facettes_OK;
    Object  *Objet_courant;
    Face    *Facette_courante;
    Sommet  *Sommet_courant;
    wxString wxMessage;
//    static bool theme_b = false;
//    int next;
//    int nb_menus;

    wxMouseEvent   event_mouse;
    wxCommandEvent cmd_event;
    wxKeyEvent     key_event;

    wxMenu * menu;
    wxMenuItem * item;
    wxMenuItemList::compatibility_iterator node;
    int nb_menus, num_menu, nb;
    wxColour Backg;

    long evkey = event.GetKeyCode();    // pas de différence entre une minuscule et une majuscule à ce niveau
    if (evkey == 0) {
        return;
    }
    if(verbose) printf("Entree de BddInter::OnKeyDown\n");

// Décodage de la Touche
    switch (evkey) {

    case 0x016c:                    // Touche Verr Num
    break;

// Zoom -
    case WXK_NUMPAD_SUBTRACT:       // Touche - du Numpad ou 0x0186
    case WXK_NUMPAD_PAGEUP:         // Via la touche 9 du Numpad
    case WXK_PAGEUP:                // Touche PageUp
        Touche_Maj = true;                                      // Zoom accéléré
        if (evkey == WXK_NUMPAD_SUBTRACT) Touche_Maj = false;   // Zoom normal
        OnMouseZoom(event_mouse, 1, Touche_Maj);                // Simule un Zoom à la souris
        break;

    case 0x0036:                    // Touche 6 (ou -)
//        case '-':     // ne marche pas en fait !
        if (!event.ShiftDown()) {   // Ne retenir cette touche que si Shift NON enfoncée (<=> ignorer le 6)
            OnMouseZoom(event_mouse, +1, false);   // Signe inverse du Zoom à la souris, false => Zoom normal
        }
        break;

// Zoom +
    case WXK_NUMPAD_ADD:            // Touche + du Numpad ou 0x0184
    case WXK_NUMPAD_PAGEDOWN:       // Via la touche 3 du Numpad
    case WXK_PAGEDOWN:              // Touche PageDown
        Touche_Maj = true;                                  // Zoom accéléré
        if (evkey == WXK_NUMPAD_ADD) Touche_Maj = false;    // Zoom normal
        OnMouseZoom(event_mouse, -1, Touche_Maj);
        break;

    case 0x003d:                    // Touche + (ou = )
//        case '=':
        if (event.ShiftDown()) {    // Ne retenir cette touche que si Shift enfoncée (<=> ignorer le =)
            OnMouseZoom(event_mouse, -1, false);  // Effet OK ici, mais avec signe inverse / souris, false => Zoom normal
        }
        break;

// Déplacement ou Rotation
// Vers la droite
    case WXK_RIGHT:                 // Flèche droite ou 0x013c (défini dans defs.h, wxKeyCode : non unicode)
    case WXK_NUMPAD_RIGHT:          // Flèche droite du Numpad (si Num Lock non enfoncé) ou 0x017a
    case 0x014a:
        OnKeyLeftRight(event, +1);
        test=2;
        break;

// Vers la gauche
    case WXK_LEFT:                  // Flèche gauche ou 0x013a
    case WXK_NUMPAD_LEFT:           // Flèche gauche du Numpad (si Num Lock non enfoncé) ou 0x0178
    case 0x0148:
        OnKeyLeftRight(event, -1);
        test=2;
        break;

// Vers le haut
    case WXK_UP:                    // Flèche haut ou 0x013b
    case WXK_NUMPAD_UP:             // Flèche haut du Numpad (si Num Lock non enfoncé) ou 0x0179
    case 0x014c:
        OnKeyUpDown(event, +1);
        test=2;
        break;

// Vers le bas
    case WXK_DOWN:                  // Flèche bas ou 0x013d
    case WXK_NUMPAD_DOWN:           // Flèche bas du Numpad (si Num Lock non enfoncé) ou 0x017b
    case 0x0146:
        OnKeyUpDown(event, -1);
        test=2;
        break;

    case 'C':
// ou    case 'c':

        if (event.ShiftDown()) {    // C'est un 'C' majuscule
            MAIN_b->Toggle_Sliders(cmd_event);
            if (MAIN_b->Panel_Sliders->IsShown()) {         // Tester si le Panneau des sliders est visible
                MAIN_b->Button_Sliders->SetValue(true) ;    // Oui <=> Bouton enfoncé
            } else {
                MAIN_b->Button_Sliders->SetValue(false) ;   // Non <=> Bouton relevé
            }
            break;
        }

// Centrer la rotation sur la sélection

        X_m = Y_m = Z_m = 0.0;
        Nb_Valeurs= 0;

        if (mode_selection == selection_point) {    // En mode selection_point, ListeSelect contient un couple "objet,sommet" et non "objet,face"
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
#pragma omp parallel for private(xyz_point)
                for (unsigned int j=0; j < Objetlist[objet].Sommetlist.size(); j++) {
                    if (ToSelect.check_if_in_ListeSelect(objet,j)) {
                        xyz_point = this->Objetlist[objet].Sommetlist[j].getPoint();
#pragma omp critical
{
                        X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                        Nb_Valeurs++;
}
                    }
                }
            }
        } else if (mode_selection == selection_facette) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
#pragma omp parallel for private(Numeros,xyz_point)
                for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                    if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                        Numeros = this->Objetlist[objet].Facelist[face].getF_sommets();
                        for (unsigned int j=0; j < Numeros.size(); j++) {
                            xyz_point = this->Objetlist[objet].Sommetlist[Numeros[j] -1].getPoint();
#pragma omp critical
{
                            X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                            Nb_Valeurs++;
}
                        }
                    }
                }
            }
        } else if (mode_selection == selection_objet) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                if (!Objetlist[objet].selected) continue;
#pragma omp parallel for private(Numeros,xyz_point)
                for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                    Numeros = this->Objetlist[objet].Facelist[face].getF_sommets();
                    for (unsigned int j=0; j < Numeros.size(); j++) {
                        xyz_point = this->Objetlist[objet].Sommetlist[Numeros[j] -1].getPoint();
#pragma omp critical
{
                        X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                        Nb_Valeurs++;
}
                    }
                }
            }
        }

        if (Nb_Valeurs > 0) {    // Ne rien faire si nul !
            X_m /= Nb_Valeurs; Y_m /= Nb_Valeurs; Z_m /= Nb_Valeurs;
            centreRot = {X_m, Y_m, Z_m};    // <=> centreRot[0] = X_m ; centreRot[1] = Y_m ; centreRot[2] = Z_m;
            centrageRotAuto = false;
        } else {
            centreRot = centre_auto;
            centrageRotAuto = true;
        }
        SetPosObs(false);
        Refresh();
        break;

// Afficher les valeurs de position
    case 'D':
// ou    case 'd':
        printf("\nPosition :\n");
        printf("X   : %f\n",m_gldata.posx) ;
        printf("Y   : %f\n",m_gldata.posy) ;
        printf("Z   : %f\n",m_gldata.posz) ;
        printf("Centre de rotation :\n")   ;
        printf("X   : %f\n",centreRot[0])  ;
        printf("Y   : %f\n",centreRot[1])  ;
        printf("Z   : %f\n",centreRot[2])  ;
        printf("FoV : %f (zoom)\n",m_gldata.zoom) ;
        break;

    case 'F':
// Pour forcer la simplification des doublons d'arêtes
        forcer_simplification_doublons_aretes = !forcer_simplification_doublons_aretes;
        if (forcer_simplification_doublons_aretes)
            printf("Forcer la simplification des doublons d'arêtes\n");
        else
            printf("Ne pas forcer la simplification des doublons d'arêtes\n");

        break;

// Permutation circulaire entre Coloriser les groupes, Coloriser les matériaux ou Pas de colorisation
    case 'G':
// ou    case 'g':
        GrpMat = 0;
        if (MAIN_b->Button_Groupes  ->GetValue()) GrpMat = 1;
        if (MAIN_b->Button_Materiaux->GetValue()) GrpMat = 2;
        if (GrpMat == 0) {
            MAIN_b->Button_Groupes->SetValue(true);         // Active la colorisation des groupes
            MAIN_b->Toggle_Groupes(cmd_event);
        } else {
            if (GrpMat == 1) {
                MAIN_b->Button_Materiaux->SetValue(true);   // Active la colorisation des Matériaux
            } else {
                MAIN_b->Button_Materiaux->SetValue(false);  // Désactive toute colorisation
            }
            MAIN_b->Toggle_Materiaux(cmd_event);
        }
        break;

// Inverser les normales (aux barycentres) + sens de parcours des sommets de facettes sélectionnées
    case 'I':
// ou    case 'i':
        Inverse_Selected_Normales();
        if (Raz_Selection_F) {
            SELECTION old  = mode_selection;
            mode_selection = selection_facette;
            key_event.m_keyCode = 'S';
            OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection des facettes
            mode_selection = old;
            m_gllist = 0;           // Regénérer toutes les listes
            Refresh();
            break;
        }
//        m_gllist = glliste_select; // Initialisé dans Inverse_Selected_Normales (2 valeurs possibles)
        Refresh();
        break;

// Afficher la Matrice et les angles
    case 'M':
// ou    case 'm':

        if (event.ShiftDown()) {    // C'est un 'M' majuscule
            MAIN_b->Toggle_Gouraud(cmd_event);
            MAIN_b->Button_Gouraud->SetValue(smooth);
            break;
        }

        GLfloat m[4][4];            // C'est un m minuscule => affiche la matrice

        glGetFloatv(GL_MODELVIEW_MATRIX, *m);
        printf("Matrice GL 2D:\n");
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][0],m[1][0],m[2][0],m[3][0]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][1],m[1][1],m[2][1],m[3][1]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][2],m[1][2],m[2][2],m[3][2]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][3],m[1][3],m[2][3],m[3][3]) ;
        glGetFloatv(GL_MODELVIEW_MATRIX, *m);
        CalculAngles(&m[0][0],alpha,beta,gamma);
        printf("alpha %8.3f deg.\n",alpha);
        printf("beta  %8.3f deg.\n",beta);
        printf("gamma %8.3f deg.\n",gamma);
        break;

// Raccourci Clavier pour "Sens des normales"
    case 'N':
// ou    case 'n';
        show_CW_CCW = !show_CW_CCW;
        MAIN_b->Menu_SensDesNormales->Check(show_CW_CCW);   // Cocher/décocher le menu correspondant
        m_gllist = 0;
        Refresh();
        break;

// Inverser seulement le sens de parcours des sommets de facettes sélectionnées
    case 'P':
// ou    case 'p':
        Inverser_Parcours_Selected();
        if (Raz_Selection_F) {
            SELECTION old  = mode_selection;
            mode_selection = selection_facette;
            key_event.m_keyCode = 'S';
            OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection des facettes
            mode_selection = old;
            m_gllist = 0;           // Regénérer toutes les listes
            Refresh();
            break;  // m_gllist = 0; et Refresh() déjà fait via 'S' ???
        }
        if (show_CW_CCW) {
            m_gllist = 0;           // Utile si Sens des normales est activé. Sans effet visuel sinon
            Refresh();
        }
        break;

// Reset (valeurs d'initialisation) ou Forcer un Reset du tracé graphique
    case 'R':
//ou    case 'r':
        if (event.ControlDown()) {    // C'est un Ctrl-R ; un 'R' majuscule event.ShiftDown() ||
            if(verbose) printf("Touche Ctrl-R\n");
            m_gllist = 0;
            Refresh();
            break;
        }
        ResetData();
        test=1;
        break;

// Reset de la sélection d'objets, de facettes ou de points
    case 'S':
// ou    case 's':
        if (mode_selection == selection_objet) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) Objetlist[objet].selected = false;
            listeObjets.clear();
        }
        liste_facettes_OK = false;
        if ((mode_selection == selection_facette) && !ToSelect.ListeSelect.empty()){
            liste_facettes_OK= true;
        }
        // Les élements de ToSelect.Liste et ToSelect.ListeSelect vont être remis aux valeurs par défaut indépendamment du fait que ces listes soient des facettes ou des sommets
        while (this->ToSelect.ListeSelect.size() > 0) { // Faire tant que la liste ToSelect.ListeSelect n'est pas vide ...
            // On récupère le premier élément de la liste et on remet les valeurs de selected à false (par défaut)
            int objet = ToSelect.ListeSelect[0].objet;
            int numero= ToSelect.ListeSelect[0].face_sommet ;
            if (mode_selection == selection_point) {
                if (ifexist_sommet(objet,numero)) {
                    Sommet_courant = &(this->Objetlist[objet].Sommetlist[numero]);
                    Sommet_courant->selected = false;                   // si numero peut correspondre à un sommet, mettre selected à false
                }
            }
            if (mode_selection == selection_facette) {
                if (ifexist_facette(objet,numero)) {
                    Facette_courante = &(this->Objetlist[objet].Facelist[numero]);
                    Facette_courante->selected = false;                 // si numero peut correspondre à une facette, mettre selected à false, et reset de la couleur de la facette
                    Facette_courante->color[0] = Facette_courante->color[1] = Facette_courante->color[2] = gris_def;   // Ne sert plus ?
                }
            }
            ToSelect.ListeSelect.erase(ToSelect.ListeSelect.begin());   // On supprime le premier élément de la liste => la liste se décale d'un cran.
        }

        if (MManip->IsShown()) {
            MManip->Button_NewObjet->Disable();// Si besoin, désactiver le bouton Button_NewObjet
        }

        if (MSelect->IsShown()) {
            wxString str_reset;
            str_reset.clear();
            MSelect->TextCtrl_NomObjet   ->ChangeValue(str_reset);
            MSelect->TextCtrl_NumObjet   ->SetValue(str_reset);
            MSelect->TextCtrl_IndObjet   ->SetValue(str_reset);
            MSelect->TextCtrl_Selection  ->SetValue(str_reset);
            MSelect->TextCtrl_NumFacette ->SetValue(str_reset);
            MSelect->TextCtrl_IndFacette ->SetValue(str_reset);
            MSelect->TextCtrl_NumGroupe  ->SetValue(str_reset);
            MSelect->TextCtrl_NumMateriau->SetValue(str_reset);
            MSelect->Button_Fusionner    ->Disable();
            MSelect->RadioButton_TypeSelection_Both->SetValue(true);
        }

        if (mode_selection != selection_objet) {
            BuildAllFacettesSelected(); // Va supprimer la liste des facettes sélectionnées si elle existe
            buildAllPoints();           // Idem pour les points
            if ((mode_selection == selection_facette) && liste_facettes_OK) m_gllist = 0;   // ne faire que si la liste de facettes n'était pas déjà vide (2 'S' consécutifs)
            else                                                            m_gllist = -1;  //glliste_objets;  // suffisant ici ? pas de regénération de points, facettes ou lignes.
        } else
            m_gllist = 0;               // on est en mode selection_objet
        Refresh();
        break;

// Complément de la sélection de facettes dans les objets
    case 'K':
// ou    case 'k':
    if (mode_selection == selection_facette) {  // seulement en mode de sélection de facettes
        bool Inverse_Selected_Facettes = false;
        // balayer les objets
        for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
            // balayer les facettes de l'objet
            Objet_courant = &(this->Objetlist[objet]);
            unsigned int nbfacettes = Objet_courant->Facelist.size();
            bool facette_selected = false;
            for (unsigned int face=0; face < nbfacettes; face++) {
                // Rechercher si une facette est sélectionnée dans cet objet
                if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                    facette_selected = true;
                    Inverse_Selected_Facettes = true;
                    break;  // Inutile de poursuivre le balayage, on a trouvé au moins une facette sélectionnée dans cet objet
                }
            }
            if (facette_selected) { // Au moins une facette a été sélectionnée dans l'objet
                for (unsigned int face=0; face < nbfacettes; face++) {
                    if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                        ToSelect.erase_one(objet,face);     // La facette était sélectionnée => l'enlever de la liste
                    } else {
                        ToSelect.add_one(objet,face);       // La facette n'était pas sélectionnée => l'ajouter à la liste
                    }
                    Objet_courant->Facelist[face].selected = !Objet_courant->Facelist[face].selected;
                }
            }
        }
        if (Inverse_Selected_Facettes) {
            m_gllist = glliste_select;
            Refresh();
        }
    }
    break;


// Changer de mode Trackball <-> Direct
    case 'T':
// ou    case 't':
        m_gldata.mode_Trackball = 1 - m_gldata.mode_Trackball ;
        MPrefs->RadioBox_Trackball->SetSelection(m_gldata.mode_Trackball) ; // Pour changer le mode affiché dans le panneau Préférences (si présent à l'écran)
        if (m_gldata.mode_Trackball) {
            printf("Mode Trackball\n");

            CalculMatriceRot();

        } else {
            printf("Mode Rotation directe\n");
            GLfloat m[4][4];

            // Calcul des rotations à partir de la matrice MODELVIEW
            glGetFloatv(GL_MODELVIEW_MATRIX, *m);
            CalculAngles(*m, m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
 //           printf("Angles xyz : %f %f %f\n",m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
        }
        test=1; // Forcer un Refresh (ne devrait rien changer sur l'écran ....)
        break;

// Switch du mode verbeux
    case 'V':
// ou    case 'v':
        verbose = !verbose ;
        printf("Mode bavard : %s\n",verbose ? "true" : "false") ;
        break;

// Étendre la sélection de facettes aux facettes voisines et de normales compatibles
    case 'X':
// ou    case 'x':

        if (mode_selection == selection_point) break;   // X ne s'applique pas au mode de sélection des sommets

//        printf("On y va\n");
        wxBeginBusyCursor();                                               // Affiche un curseur animé par défaut (pour faire patienter !)
        bool extend_auto;
        int sz_numF, sz_numJ;

        extend_auto = false;                                               // Initialiser le marqueur d'ajouts de facettes
        // Du fait des boucles imbriquées ce test peut-être long (et même très long sur de grosses Bdds)
        // Paralléliser ? pas sûr ... mais ça semble fonctionner !
        for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
            Objet_courant = &(this->Objetlist[objet]);
            unsigned int nbfacettes = Objet_courant->Facelist.size();
            for (unsigned int face=0; face < nbfacettes; face++) {
                if (ToSelect.check_if_in_ListeSelect(objet,face)) {
//                    printf("objet : %d, facette : %d\n",objet,face);
                    NumerosF = Objet_courant->Facelist[face].getF_sommets();    // Récupérer les numéros de sommets de la facette "face" de l'objet "objet"
                    sz_numF  = NumerosF.size();
#pragma omp parallel for private(Facette_courante,NumerosJ,sz_numJ)
                    for (unsigned int j=0; j<nbfacettes; j++) {                 // Commencer directement à face+1 plutôt que 0 ? ... Non, ne suffit pas
                        if (j == face) continue;                                // Passer directement au suivant (inclus dans le test ci-dessous, mais + rapide ?)
                        if (ToSelect.check_if_in_ListeSelect(objet,j)) continue;// Si la facette j déjà dans la liste, passer à la suivante
                        Facette_courante = &(Objet_courant->Facelist[j]);
                        NumerosJ = Facette_courante->getF_sommets();            // Récupérer les numéros de sommets de la facette "j" de l'objet "objet"
                        sz_numJ  = NumerosJ.size();
                        for (int nn=0; nn<sz_numF; nn++) {
                            int Numeros_nn = NumerosF[nn];
                            for (int nj=0; nj<sz_numJ; nj++) {
                                if (Numeros_nn != NumerosJ[nj]) continue ;      // Ne faire la suite (comparaison des normales) que si 1 point commun entre les facettes j et face
                                if (Compare_Normales(objet,face,j)) {           // Test sur les normales
//                                   printf("OK  %d\n",j);
                                    if (!ToSelect.check_if_in_ListeSelect(objet,j)) {   // N'ajouter que si non déjà présent dans la Liste
#pragma omp critical
{
                                        ToSelect.add_one(objet,j);
}
                                        extend_auto = true;                             // Marquer qu'on a ajouté une facette dans la liste
                                        Facette_courante->selected = true;
//                                            colorface(objet,j);   // Maintenant <=> ligne ci-dessus
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        str.Printf(_T("%d"),(int)this->ToSelect.ListeSelect.size());
        MSelect->TextCtrl_Selection->SetValue(str); // Peut se faire dans tous les cas, même si SelectionPanel non affiché (MSelect->IsShown() est false)
        m_gllist = glliste_select; // 0;
        Refresh();
//        printf("C'est fini\n");
        if (wxIsBusy()) wxEndBusyCursor();                                                  // Supprime le curseur animé... C'est terminé !

        if (!extend_auto) {                                                 // La sélection n'a pas été modifiée : afficher un message
//            printf("Extension automatique : limite atteinte\n");
            if (ToSelect.ListeSelect.size() == 0)
                wxMessage = _T("La sélection de facettes est vide. Pas d'extension possible");
            else {
                wxMessage = _T("L'extension automatique de la sélection a atteind sa limite :\n");
                wxMessage+= _T("pas ou plus de points communs avec les facettes voisines");
            }
            DisplayMessage(wxMessage,false);
        }
        break;

// Supprimer les facettes sélectionnées
    case WXK_DELETE :

        if (mode_selection == selection_point) break;   // Delete ne s'applique pas au mode de sélection des sommets

        if (mode_selection == selection_objet) {
            auto it = listeObjets.begin();
            for (unsigned int i=0; i<listeObjets.size(); i++, it++) {
                Objetlist[*it].deleted = true;
            }
            if (MSelObj->IsShown()) {                   // Mettre à jour la fenêtre de Sélection Manuelle d'objets si elle est affichée
                unsigned int i,n,indice_ListBox;
                wxString str_loc;
                n = MSelObj->CheckListBox1->GetCount(); // Récupère le nombre d'items actuels
                for (i=0; i<n; i++)
                    MSelObj->CheckListBox1->Delete(0);  // Supprime ces "anciens" items

                indice_ListBox = 0; // Pour découpler l'indice d'un objet de celui de la CheckListBox (en cas d'objets supprimés)
                for (i=0; i<Objetlist.size(); i++) {
                    if (!Objetlist[i].deleted) {
                        str_loc = wxString(Objetlist[i].GetName(), wxConvUTF8);
                        MSelObj->CheckListBox1->Append(str_loc);                                // Recrée tous les items (des objets non supprimés)
                        MSelObj->CheckListBox1->Check(indice_ListBox,Objetlist[i].selected);    // Coche ceux qui sont déjà sélectionnés
                        indice_ListBox++;
                    }
                }
            }
            m_gllist = 0;
            Refresh();
            break;
        }

        if (mode_selection == selection_facette) {
            do {
    //            printf("\ntouche S : \nsize avant: %d\n",this->ToSelect.ListeSelect.size());
                for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                    for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                        if (ToSelect.check_if_in_ListeSelect(objet,face)) {
        //                    printf("%d %d %d\n",ToSelect.check_if_in_ListeSelect(objet,face),objet,face);
                            Facette_courante = &(this->Objetlist[objet].Facelist[face]);
                            Facette_courante->selected = false;
//                            Facette_courante->color[0] = Facette_courante->color[1] = Facette_courante->color[2] = gris_def;     // Ne sert plus ?
                            Facette_courante->deleted  = true;
                            Elements_Supprimes= true;              // Au moins une facette supprimée
                            ToSelect.erase_one(objet,face);
                        }
                    }
                }
    //            printf("size apres: %d\n",this->ToSelect.ListeSelect.size());     // Effet escompté mais la Liste n'est pas forcément vide !
            } while (this->ToSelect.ListeSelect.size() > 0);                        // Donc, on boucle ... jusqu'à ce que ...
            m_gllist = 0;
            Refresh();
        }
        break;

        // Masquer les facettes sélectionnées
    case WXK_NUMPAD_DELETE :

        if (mode_selection == selection_point) break;   // Masquer ne s'applique pas au mode de sélection des sommets

        if (mode_selection == selection_objet) {
            auto it = listeObjets.begin();
            for (unsigned int i=0; i<listeObjets.size(); i++, it++) {
                Objetlist[*it].afficher = false;
            }
            m_gllist = 0;
            Refresh();
            break;
        }

        if (mode_selection == selection_facette) {
            do {
    //            printf("\ntouche S : \nsize avant: %d\n",this->ToSelect.ListeSelect.size());
                for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                    for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                        if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                            Facette_courante = &(this->Objetlist[objet].Facelist[face]);
        //                    printf("%d %d %d\n",ToSelect.check_if_in_ListeSelect(objet,face),objet,face);
                            Facette_courante->selected = false;
//                            Facette_courante->color[0] = Facette_courante->color[1] = Facette_courante->color[2] = gris_def; // Ne sert plus ?
                            Facette_courante->afficher = false;
                            Elements_Masques  = true;               // Au moins une facette masquée
                            ToSelect.erase_one(objet,face);
                        }
                    }
                }
    //            printf("size apres: %d\n",this->ToSelect.ListeSelect.size());     // Effet escompté mais la Liste n'est pas forcément vide !
            } while (this->ToSelect.ListeSelect.size() > 0);                        // Donc, on boucle ... jusqu'à ce que ...
            m_gllist = 0;
            Refresh();
        }
        break;

// Quitter par les touches Q ou Escape (en plus de Alt-X déjà programmé et les icônes X de fermeture)
    case 'Q':
    case WXK_ESCAPE:
        wxTheApp->ExitMainLoop();
        break;

    case 'A':                                   // Bascule du mode Antialiasing OpenGL
        antialiasing_soft = !antialiasing_soft;
        if (MPrefs->IsShown()) {
            MPrefs->CheckBox_AntialiasingSoft->SetValue(antialiasing_soft); // Modifier la case à cocher si le dialogue Préférences est affiché
        }
        Refresh();
        ini_file_modified = true;               // Enregistrer ce changement dans le fichier init
        break;

    case 'Z':
        Raz_Selection_F   = !Raz_Selection_F;   // Inverse le mode de RAZ de sélection de facettes utilisé après une inversion de normales pour garder ou remettre à 0 la sélection
        ini_file_modified = true;               // Enregistrer ce changement dans le fichier init
        break ;

    case 'J':
        // Touche J pour divers tests provisoires ... A supprimer donc !
        test_rectangle_selection = !test_rectangle_selection;
        printf("Mode Test ");
        if (test_rectangle_selection) printf("on\n");
        else                          printf("off\n");
//        Refresh();
        break;

    case 'W':
        theme_b = !theme_b;                     // Test pour Basculer du thème standard vers un thème sombre... Pas très fiable, plante parfois !!!
#if wxCHECK_VERSION(3,3,0)
//        if (theme_b) wxGetApp().MSWEnableDarkMode(1);// wxApp::DarkMode_Always
//        else         wxGetApp().MSWEnableDarkMode(0);// wxApp::DarkMode_Auto
        if (theme_b) wxTheApp->MSWEnableDarkMode(wxApp::DarkMode_Always, new MySettings); // Et si MySettings a déjà été créé ?
        else         wxTheApp->MSWEnableDarkMode(0);
        Switch_theme_wx33(theme_b);
#else
        Switch_theme(theme_b);
#endif
        darkmode = (int)theme_b*2 -1;   // darkmode = -1 pour thème clair standard, darkmode = +1 pour thème sombre (0, pour suivre le thème système, non disponible ici)
        ini_file_modified = true;
        this->MAIN_b ->Refresh();
        break;

// Touche non reconnue : rien de spécial à faire (sauf l'afficher)
    default:
        if (evkey != 0x0132 && evkey != 0x0133 && evkey != 0x0134 ) { // on passe si Shift, Alt ou Ctrl
            printf("code : %3d, %c, %#06x\n", (int)evkey,char(evkey),(int)evkey);
        }
        break;
    }

    if (test != 0) { // Quelque chose à changé (rotation, déplacement, Zoom). Il faut redessiner la fenêtre OpenGL
        if (test == 1) {
            ResetProjectionMode();
        }
        if (test == 2) {
            if (MPosObs->IsShown()) {
//                    printf("Pos Obs est affiche\n");
            // Même code que dans SetAngles (mais accès + direct)
            // ATTENTION : si roty != 0, les valeurs Site Azimut sont erronnées
                int val = Convert_Rotx_LSI() ;
                MPosObs->SpinCtrl_LSI->SetValue(val);

                val = Convert_Rotz_LAZ();
                MPosObs->SpinCtrl_LAZ->SetValue(val);
            }
            if (MZoomSpec->IsShown()) {
                int val = Convert_Rotx_LSI();
                MZoomSpec->SpinCtrl_LSI->SetValue(val);

                val = Convert_Rotz_LAZ();
                MZoomSpec->SpinCtrl_LAZ->SetValue(val);
            }
        }
        Refresh(false);
    }

//    event.Skip();   // Avec ou sans ne semble rien changer !!!
    if(verbose) printf("Sortie de BddInter::OnKeyDown\n");
}
