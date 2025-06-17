#include "Icosaedre.h"
#include "vector3d.h"

//(*InternalHeaders(Icosaedre)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Icosaedre)
const long Icosaedre::ID_STATICTEXT1 = wxNewId();
const long Icosaedre::ID_TEXTCTRL1 = wxNewId();
const long Icosaedre::ID_STATICTEXT2 = wxNewId();
const long Icosaedre::ID_TEXTCTRL2 = wxNewId();
const long Icosaedre::ID_STATICTEXT3 = wxNewId();
const long Icosaedre::ID_TEXTCTRL3 = wxNewId();
const long Icosaedre::ID_STATICTEXT4 = wxNewId();
const long Icosaedre::ID_TEXTCTRL4 = wxNewId();
const long Icosaedre::ID_STATICTEXT5 = wxNewId();
const long Icosaedre::ID_SPINCTRL3 = wxNewId();
const long Icosaedre::ID_BUTTON1 = wxNewId();
const long Icosaedre::ID_BUTTON2 = wxNewId();
const long Icosaedre::ID_STATICTEXT7 = wxNewId();
const long Icosaedre::ID_SPINCTRL1 = wxNewId();
const long Icosaedre::ID_STATICTEXT8 = wxNewId();
const long Icosaedre::ID_SPINCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Icosaedre,wxDialog)
	//(*EventTable(Icosaedre)
	//*)
END_EVENT_TABLE()

Icosaedre::Icosaedre(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Icosaedre::Icosaedre Création d'une forme générique en Icosaedre (peut servir de base à une sphère)
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Icosaedre)
	Create(parent, id, _T("Ajout d\'un Icosaèdre"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(410,164));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Rayon :"), wxPoint(144,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Rayon = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1.0"), wxPoint(200,8), wxSize(104,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Centre de l\'Icosaèdre X :"), wxPoint(5,43), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(128,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Y :"), wxPoint(208,43), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(224,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Z :"), wxPoint(312,43), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(328,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Nombre de Subdivisions :"), wxPoint(80,76), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	SpinCtrl_Subdiv = new wxSpinCtrl(this, ID_SPINCTRL3, _T("0"), wxPoint(224,72), wxSize(74,23), 0, 0, 8, 0, _T("ID_SPINCTRL3"));
	SpinCtrl_Subdiv->SetValue(_T("0"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(104,136), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(208,136), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Groupe :"), wxPoint(8,107), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(128,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Matériau :"), wxPoint(208,107), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(328,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Icosaedre::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Icosaedre::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Icosaedre::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Icosaedre::OnSpinCtrl_MateriauChange);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&Icosaedre::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Icosaedre::OnClose);
	//*)
//	this->Icosaedre_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Icosaedre::~Icosaedre()
{
	//(*Destroy(Icosaedre)
	//*)
}

void Icosaedre::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Icosaedre::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Icosaedre::Genere_Normales_Sommets(BddInter *Element, Object *p_Objet)
{
    int i, numero;
    std::vector<float> xyz_sommet;

    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    int NbPoints = (Element->Objetlist.end()-1)->Nb_sommets;
    Element->str.clear();
    Element->N_elements = NbPoints;
    p_Objet->Nb_normales = NbPoints;
    Element->make_vecteur();

    Element->N_elements = 3;
    numero = 1;
    for (i=0; i<NbPoints ; i++) {
        xyz_sommet = p_Objet->Sommetlist[i].getPoint();
        Vector3D Vs(xyz_sommet[0]-Xc, xyz_sommet[1]-Yc, xyz_sommet[2]-Zc);
        Vs.normalize();
        Element->N_elements = numero;
        Element->Setxyz(Vs.X, Vs.Y, Vs.Z);
        Element->make_1_vecteur();
        numero++;
    }

}

void Icosaedre::Genere_Sommets(BddInter *Element, Object *p_Objet)
{
// Génère un icosaèdre, via des points sur une sphère de rayon donné
// NOTE : cette figure peut servir de point de départ à une génération de sphère en subdivisant les arêtes de façon récursive
//        intérêt :      moins de directions privilégiées / sphère classique
//        inconvénient : plus de réels méridiens => plus de traitement par quartiers

//NOTE : avec ces sommets, on peut générer un icosaèdre comme une sphère à 2 parallèles et 5 méridiens.

	float theta, d_theta, phi, s_phi, c_phi;
    int i, numero;

    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    int NbPoints = 12;

    Element->str.clear();
    Element->N_elements = NbPoints;
    Element->make_sommet();

    p_Objet->Nb_sommets = NbPoints;

    // Pôle Nord
    numero = 1;
    Element->N_elements = numero;
    Element->Setxyz(Xc, Yc+rayon, Zc);
    Element->make_1_sommet();

    phi   = M_PI/2 - atan(0.5) ;    // Phi n'est pas la latitude ici : latitude = +- artan(1/2) M_PI_2 = M_PI/2 mais seulement pour Gnu C
    s_phi = sin(phi);
    c_phi = cos(phi);

    // Parcours des points (5 points par parallèle)
    theta  = -M_PI/10.;      // 1er point -18° (compatible de la version Tcl)
    d_theta=  M_PI/5.*2.;    // Pas de 72°
    for (i=0; i<= 9; i++) {
        numero++;
        Element->N_elements = numero;
        Element->Setxyz(Xc+rayon*cos(theta)*s_phi,
                        Yc+rayon*c_phi,
                        Zc+rayon*sin(theta)*s_phi);
        Element->make_1_sommet();

        if (i == 4) {
            c_phi *= -1 ;           // <=> phi = M_PI - phi ;
            theta += d_theta/2.;    // 2ème parallèle, se décaler d'1/2 pas, soit 36°
        }
        theta += d_theta;
    }
    // Pôle Sud
    numero = NbPoints;
    Element->N_elements = numero;
    Element->Setxyz(Xc, Yc-rayon, Zc);
    Element->make_1_sommet();

}

void Icosaedre::genereIcosaedre()
{
    BddInter *Element = MAIN->Element;

    wxString num_obj;
    int new_num;
    Object *p_Objet;

    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetNumero() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Icosaedre - ") + num_obj;
    Element->make_objet();
    Element->Objetlist.rbegin()->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->GetIndiceObjetCourant();

    printf("centre : %f %f %f\nrayon  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],rayon);
    int Nb_Meridiens = 5 ;
    int Nb_Paralleles= 2;
    int Nb_facettes  = 2*Nb_Meridiens*Nb_Paralleles;

    bool New_typeSphere = false;
    Element->Genere_Facettes_Sphere(Nb_Meridiens, Nb_Paralleles, New_typeSphere);

    p_Objet = &(Element->Objetlist[indiceObjet]);

    Genere_Sommets(Element, p_Objet);
    Element->Genere_Normales_Facettes (p_Objet, Nb_facettes);
    Element->Genere_Attributs_Facettes(p_Objet, Nb_facettes, numeroGroupe, numeroMateriau);
    Element->Genere_Luminances(p_Objet, Nb_facettes);
    p_Objet->flat = false; // Si true, vrai Icosaèdre à facettes planes
    Genere_Normales_Sommets(Element, p_Objet);

    Element->Genere_Tableau_Points_Facettes(p_Objet);
    Element->SetGenereTableauAretes_OK(true);
    Element->Genere_Tableau_Aretes(p_Objet);
    Element->Genere_Liste_Groupes_Materiaux(p_Objet);

    Element->SetBddModifiee(true);
}

void Icosaedre::genereSubdivisions(int NbSubdiv)
{
// Génère une sphère en subdivisant de façon itérative les triangles d'origine de l'icosaèdre.
// Segments divisés par 2 => 1 triangle d'origine en donne 3 nouveaux.
// NbSubdiv     Nombre de sommets   avec simplification
/*      0               12                  12
        1               72                  42
        2              312                 162
        3             1272                 642
        4             5112                2562
        5            20472               10242
        6            81912               40962
        7           327672              163842
        ... Pas d'intérêt au-delà, de plus devient long à gérer et peut planter à cause de soucis de gestion de la mémoire.
*/
    BddInter *Element = MAIN->Element;

    Object *objet_courant;
    Face   *facette_courante;
    Face   *facette_nouvelle;
    Sommet *sommet_courant;
    int isubdiv;
    unsigned int i,j,indiceFacette;
    std::vector<int> NumerosSommets,NewNumeros;
    std::vector<float> xyz_sommet;

    if (Element->GetVerbose()) printf("NbSubdiv = %d\n",NbSubdiv);
    int indiceObjet = Element->Objetlist.size() -1; // Car c'est le dernier !
    Element->SetIndiceObjetCourant(indiceObjet);
    objet_courant = &(Element->Objetlist[indiceObjet]);
    Vector3D Centre(centre_primitive[0],centre_primitive[1],centre_primitive[2]);
    Vector3D Pn, Vn;

    Element->str.clear();

    for (isubdiv=0; isubdiv<NbSubdiv; isubdiv++) {
        unsigned int Nb_sommets  = objet_courant->Sommetlist.size();
        unsigned int Nb_facettes = objet_courant->Facelist.size();
        unsigned int newFacette  = Nb_facettes;
        int groupe     = objet_courant->Facelist[0].groupe ;        // groupe et codmatface identiques sur tout l'objet
        int codmatface = objet_courant->Facelist[0].codmatface ;
        for (i=0; i<Nb_facettes; i++) {
            facette_courante = &(objet_courant->Facelist[i]);
            NumerosSommets = facette_courante->getF_sommets();
//            printf("%d %2d %2d %2d\n",NumerosSommets.size(),NumerosSommets[0],NumerosSommets[1],NumerosSommets[2]);
            sommet_courant = &(objet_courant->Sommetlist[NumerosSommets[0]-1]);
            xyz_sommet = sommet_courant->getPoint();
            Vector3D P0(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            sommet_courant = &(objet_courant->Sommetlist[NumerosSommets[1]-1]);
            xyz_sommet = sommet_courant->getPoint();
            Vector3D P1(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            sommet_courant = &(objet_courant->Sommetlist[NumerosSommets[2]-1]);
            xyz_sommet = sommet_courant->getPoint();
            Vector3D P2(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);

// Génération de 3 points, milieu des segments (Pour l'instant, on ne traite pas les points communs à 2 facettes)
// => en gros, 2 fois trop de points générés
            Pn = (P0+P1)/2. - Centre;
            Vn = Pn.normalize();                // Vecteur de la normale au sommet
            Pn = Vn*rayon + Centre;             // Point sur la surface de la sphère
            Element->N_elements = Nb_sommets+1;
            Element->Setxyz(Pn.X, Pn.Y, Pn.Z);
            Element->make_1_sommet();
            Element->Setxyz(Vn.X, Vn.Y, Vn.Z);
            Element->make_1_vecteur();

            Pn = (P1+P2)/2. - Centre;
            Vn = Pn.normalize();
            Pn = Vn*rayon + Centre;
            Element->N_elements = Nb_sommets+2;
            Element->Setxyz(Pn.X, Pn.Y, Pn.Z);
            Element->make_1_sommet();
            Element->Setxyz(Vn.X, Vn.Y, Vn.Z);
            Element->make_1_vecteur();

            Pn = (P2+P0)/2. - Centre;
            Vn = Pn.normalize();
            Pn = Vn*rayon + Centre;
            Element->N_elements = Nb_sommets+3;
            Element->Setxyz(Pn.X, Pn.Y, Pn.Z);
            Element->make_1_sommet();
            Element->Setxyz(Vn.X, Vn.Y, Vn.Z);
            Element->make_1_vecteur();

            NewNumeros = NumerosSommets;
            NewNumeros[1] = Nb_sommets+1;
            NewNumeros[2] = Nb_sommets+3;
            facette_courante->setFsommet(NewNumeros);           // Facette originale modifiée
            Element->Calcul_Normale_Barycentre(indiceObjet,i);  // Recalculer une nouvelle normale au barycentre
            facette_courante->setLsommet(NewNumeros);           // Sommets en Luminances = Sommets des Facettes

// Création des 3 nouvelles facettes
            Element->NumerosSommets.resize(3);

            indiceFacette = newFacette++;
            Element->N_elements = newFacette;
            Element->NumerosSommets = {(int)Nb_sommets+1, NumerosSommets[1], (int)Nb_sommets+2};
            Element->make_1_face();
            objet_courant = &(Element->Objetlist[indiceObjet]);
            facette_nouvelle = &(objet_courant->Facelist[indiceFacette]);
            facette_nouvelle->groupe       = groupe;
            facette_nouvelle->codmatface   = codmatface;
            Element->Calcul_Normale_Barycentre(indiceObjet,indiceFacette);
            facette_nouvelle->L_sommets    = facette_nouvelle->F_sommets;
            facette_nouvelle->Nb_Sommets_L = 3;
            facette_nouvelle->flat = false;

            indiceFacette++;
            Element->N_elements = ++newFacette;
            Element->NumerosSommets = {(int)Nb_sommets+1, (int)Nb_sommets+2, (int)Nb_sommets+3};
            Element->make_1_face();
            facette_nouvelle = &(objet_courant->Facelist[indiceFacette]);
            facette_nouvelle->groupe       = groupe;
            facette_nouvelle->codmatface   = codmatface;
            Element->Calcul_Normale_Barycentre(indiceObjet,indiceFacette);
            facette_nouvelle->L_sommets    = facette_nouvelle->F_sommets;
            facette_nouvelle->Nb_Sommets_L = 3;
            facette_nouvelle->flat = false;

            indiceFacette++;
            Element->N_elements = ++newFacette;
            Element->NumerosSommets = {(int)Nb_sommets+3, (int)Nb_sommets+2, NumerosSommets[2]};
            Element->make_1_face();
            facette_nouvelle = &(objet_courant->Facelist[indiceFacette]);
            facette_nouvelle->groupe       = groupe;
            facette_nouvelle->codmatface   = codmatface;
            Element->Calcul_Normale_Barycentre(indiceObjet,indiceFacette);
            facette_nouvelle->L_sommets    = facette_nouvelle->F_sommets;
            facette_nouvelle->Nb_Sommets_L = 3;
            facette_nouvelle->flat = false;

            Nb_sommets = objet_courant->Sommetlist.size();

        }
    }
    Element->Genere_Tableau_Points_Facettes(objet_courant);
    Element->Genere_Tableau_Aretes(objet_courant);
    printf("\nResultat des subdivisions :\n");
    printf("Nombre de points   : %d\n",objet_courant->Nb_sommets) ;
    printf("Nombre de facettes : %d\n",objet_courant->Nb_facettes);
    printf("Nombre d'aretes    : %d\n",objet_courant->Nb_aretes) ;

}

void Icosaedre::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str;

    rayon = wxAtof(TextCtrl_Rayon->GetValue());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    numeroGroupe  = SpinCtrl_Groupe  ->GetValue() ;     // Par précaution, mais déjà fait !
    numeroMateriau= SpinCtrl_Materiau->GetValue() ;     // idem

    genereIcosaedre();

    int NbSubdiv = SpinCtrl_Subdiv->GetValue(); // 6 ou 7 au max. à 8, ça plante car dépassements de mémoire sans doute : trop de points et/ou facettes.
    if (NbSubdiv != 0) genereSubdivisions(NbSubdiv);

    Element->SetTypeFichier(1);     // Marquer comme si c'était un fichier .bdd
    Element->SetTypeDxf(false);
    Element->m_gllist = 0;

    Element->Search_Min_Max();
    Element->SetFileLoaded(true);
    Element->SetOK_ToSave (true);
    Element->Refresh();
    Hide();
}


void Icosaedre::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Icosaedre::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}


void Icosaedre::OnInit(wxInitDialogEvent& event)
{
}
