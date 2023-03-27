#include "Cone.h"
#include "vector3d.h"

//(*InternalHeaders(Cone)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Cone)
const long Cone::ID_STATICTEXT1 = wxNewId();
const long Cone::ID_TEXTCTRL1 = wxNewId();
const long Cone::ID_STATICTEXT2 = wxNewId();
const long Cone::ID_TEXTCTRL2 = wxNewId();
const long Cone::ID_STATICTEXT3 = wxNewId();
const long Cone::ID_TEXTCTRL3 = wxNewId();
const long Cone::ID_STATICTEXT4 = wxNewId();
const long Cone::ID_TEXTCTRL4 = wxNewId();
const long Cone::ID_STATICTEXT5 = wxNewId();
const long Cone::ID_TEXTCTRL5 = wxNewId();
const long Cone::ID_STATICTEXT6 = wxNewId();
const long Cone::ID_TEXTCTRL6 = wxNewId();
const long Cone::ID_STATICTEXT7 = wxNewId();
const long Cone::ID_SPINCTRL1 = wxNewId();
const long Cone::ID_STATICTEXT8 = wxNewId();
const long Cone::ID_SPINCTRL2 = wxNewId();
const long Cone::ID_BUTTON1 = wxNewId();
const long Cone::ID_BUTTON2 = wxNewId();
const long Cone::ID_CHECKBOX1 = wxNewId();
const long Cone::ID_STATICTEXT9 = wxNewId();
const long Cone::ID_SPINCTRL3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Cone,wxDialog)
	//(*EventTable(Cone)
	//*)
END_EVENT_TABLE()

Cone::Cone(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Cone::Cone Création d'une forme générique en cône
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Cone)
	Create(parent, id, _T("Ajout d\'un cône"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(466,220));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Longueur :"), wxPoint(152,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Longueur = new wxTextCtrl(this, ID_TEXTCTRL1, _T("2.0"), wxPoint(224,8), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Rayon du cercle de base :"), wxPoint(80,35), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_Rayon = new wxTextCtrl(this, ID_TEXTCTRL2, _T("1.0"), wxPoint(224,32), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Coordonnées du centre du cône : X:"), wxPoint(4,59), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(192,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Y :"), wxPoint(272,59), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(288,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Z :"), wxPoint(368,59), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL5, _T("0.0"), wxPoint(384,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre de points générateurs du cercle :"), wxPoint(4,83), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	TextCtrl_NbPoints = new wxTextCtrl(this, ID_TEXTCTRL6, _T("16"), wxPoint(224,80), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Groupe :"), wxPoint(4,107), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(120,104), wxSize(96,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Matériau :"), wxPoint(232,107), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(360,104), wxSize(96,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(120,192), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(232,192), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_FermerCone = new wxCheckBox(this, ID_CHECKBOX1, _T("Fermer le cône à sa base"), wxPoint(8,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_FermerCone->SetValue(false);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Nombre de secteurs"), wxPoint(4,139), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	SpinCtrl_NbSecteurs = new wxSpinCtrl(this, ID_SPINCTRL3, _T("1"), wxPoint(120,136), wxSize(96,21), 0, 1, 20, 1, _T("ID_SPINCTRL3"));
	SpinCtrl_NbSecteurs->SetValue(_T("1"));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_MateriauChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cone::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cone::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_NbSecteursChange);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Cone::OnClose);
	//*)
//	this->Cone_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);

}

Cone::~Cone()
{
	//(*Destroy(Cone)
	//*)
}

void Cone::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Cone::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Cone::Genere_Sommets(BddInter* Element, Object *p_Objet)
{
    int i,j, numero;
    float angle,pas_angle;
    float Xp, XStep;
    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    float rayon_j;

    angle = 0.;
    pas_angle = 2.*M_PI/NbPoints;
    int NbSommets = n_secteurs*NbPoints +1;

    Element->str.clear();
    Element->N_elements = NbSommets;
    Element->make_sommet();

    p_Objet->Nb_sommets = NbSommets;

    // Sommets du/des cercles générateurs
    numero = 1;
    Xp = Xc - longueur/2.;	        // Premier plan
    XStep = longueur/n_secteurs;
    for (j=0; j<n_secteurs ; j++) {
        angle = 0.;
        rayon_j = (n_secteurs-j)*rayon/n_secteurs;
        for (i=0; i<NbPoints; i++) {
            Element->N_elements=numero; Element->Setxyz(Xp,Yc+rayon_j*cos(angle),Zc+rayon_j*sin(angle)); Element->make_1_sommet();
            angle += pas_angle;
            numero++;
        }
		Xp += XStep;                // Plans suivants
    }
    // Sommet du cône
    Element->N_elements=numero; Element->Setxyz(Xc+longueur/2,Yc,Zc); Element->make_1_sommet();

}

// Si TRUE secteur du sommet à 3 points, si FALSE, secteur du sommet à 4 points dont 1 doublé
#define Last_3Points TRUE
//#define Last_3Points FALSE

void Cone::Genere_Luminances(BddInter* Element)
{
    wxString str_loc;
    int numero,i1,i,j,i2;

    Element->str.clear();
    Element->N_elements = NbPoints*n_slices;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
#if Last_3Points
    i2 = NbPoints+1;
    bool last_j = false;
    int  last_secteur = n_secteurs-1;
#endif
    Element->make_luminance();
    numero= 1;

// Traité comme un cylindre, y compris sur les facettes contenant le sommet du cône si Last_3Points est FALSE, car considérées à 4 points (sommet doublé !)
// si Last_3Points est TRUE sedrnier secteurs à 3 points seulement

    for (j=0; j < n_secteurs ; j++) {
#if Last_3Points
        if (j == last_secteur) last_j = true;
#endif
        for (i=1; i <= NbPoints ; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1;
#if Last_3Points
            if (last_j)
                Element->str.Printf(_T("%d 3 %d %d %d"),numero,i,i1,i2++) ;   // ici, dernier secteur avec 3 points
            else
#endif
                Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i,i1,i1,i) ;
            Element->make_1_luminance();
            numero++;
        }
    }
    if (CheckBox_FermerCone->IsChecked()) {
        Element->str.Printf(_T("%d %d"),numero,NbPoints) ;
        str_loc.Printf(_T(" %d"),NbPoints+1);
        for (int j=0; j<NbPoints ; j++) Element->str += str_loc;
        Element->make_1_luminance();
    }
}

void Cone::Genere_Normales_Sommets(BddInter* Element, Object *p_Objet)
{
    // Normales aux sommets
    int numero;
    float angle,pas_angle;

    float hypotenuse = sqrt(longueur*longueur + rayon*rayon);
    float sin_sommet = rayon/hypotenuse;
    float cos_sommet = longueur/hypotenuse;

    pas_angle = 2.*M_PI/NbPoints;

    Element->str.clear();
    Element->N_elements = NbPoints;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
#if Last_3Points
    Element->N_elements += NbPoints;    // Place pour NbPoints normales aux sommets supplémentaires, égales aux normales au barycentre de chaque facette
#endif
    p_Objet->Nb_vecteurs = Element->N_elements;
    Element->make_vecteur();
    numero= 1;
    angle = 0.;
    for (int i=0; i<NbPoints; i++) {
        Element->N_elements=numero; Element->Setxyz(sin_sommet,cos_sommet*cos(angle),cos_sommet*sin(angle));
        Element->make_1_vecteur();
        angle += pas_angle;
        numero++;
	}
#if Last_3Points
    std::vector <float> Normale3;
    for (int i=0; i<NbPoints; i++) {
        Normale3 = p_Objet->Facelist[i].getNormale_b();         // Récupérer la normale au barycentre de la facette
        Element->N_elements=numero++;
        Element->Setxyz(Normale3[0],Normale3[1],Normale3[2]);   // En faire une normale au sommet du cône
        Element->make_1_vecteur();
    }
#endif
    if (CheckBox_FermerCone->IsChecked()) {
        Element->N_elements=numero++;
        Element->Setxyz(-1.,0.,0.);
        Element->make_1_vecteur();
    }
}

void Cone::Genere_Facettes(BddInter* Element, Object *p_Objet)
{
    wxString str_loc;
    int numero,i,i0,i1,i2,i3;
    int istep;

    Element->str.clear();
    int NbFacettes = NbPoints*n_secteurs;
    Element->N_elements = NbFacettes;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
    Element->make_face();

    p_Objet->Nb_facettes = Element->N_elements;
    numero = 1;

    // Facettes à 4 points (inter-secteurs)
    for (int j=0; j < n_secteurs-1 ; j++) {
        istep = j*NbPoints;
        for (i=1; i<=NbPoints; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1; i2=i1+NbPoints;
            i0 = i+istep;
            i3 = i0+NbPoints;
            i2 += istep ; i1 += istep;
            Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i0,i1,i2,i3); Element->make_1_face();
            numero++;
        }
    }
    // Facettes au sommet du cône
    istep = (n_secteurs-1)*NbPoints;
    i2    = NbFacettes +1;
    for (int i=1; i<=NbPoints; i++) {
        i1 = i+1 ; if (i1 > NbPoints) i1 = 1;
        i0 = i+istep;
        i1 += istep;
#if Last_3Points
        Element->str.Printf(_T("%d 3 %d %d %d"),numero,i0,i1,i2);  // Sommet non doublé => traiter les luminances pour 3 valeurs à la dernière itération en j
#else
        Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i0,i1,i2,i2);  // Sommet doublé : n'est pas optimal mais évite des soucis en cas de recalcul des normales
#endif
        Element->make_1_face();
        numero++;
	}
    if (CheckBox_FermerCone->IsChecked()) {
        str_loc.Printf(_T("%d %d"),numero,NbPoints); Element->str = str_loc;
        for (i=NbPoints; i>=1 ; i--) {
            str_loc.Printf(_T(" %d"),i); Element->str += str_loc;
        }
        Element->make_1_face();
    }
}

void Cone::genereCone()
{
    wxString num_obj;
    int new_num;
    Object *p_Objet;

    BddInter* Element = MAIN->Element;

//    int new_num = Element->Objetlist[(Element->Objetlist.size()-1)].GetValue() +1;
//  Récupérer plutôt le numéro du dernier objet au lieu de l'indice et l'incrémenter
    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetValue() +1;

    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Cone - ") + num_obj;
    Element->make_objet();
    Element->Objetlist.rbegin()->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->indiceObjet_courant; // C'est le dernier !

    printf("centre : %f %f %f\nlongueur  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],longueur);
    printf("rayon  : %f, Nombre de points : %d\n",rayon,NbPoints);
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
//    n_secteurs = 1; // Pour le moment !
    n_slices   = n_secteurs+1;
    int Nb_facettes = NbPoints*n_secteurs ;
    if (CheckBox_FermerCone->IsChecked()) Nb_facettes +=1;

    p_Objet = &(Element->Objetlist[indiceObjet]);

    Genere_Sommets (Element, p_Objet);
    Genere_Facettes(Element, p_Objet);

    Element->Genere_Normales_Facettes (p_Objet, Nb_facettes);
    Element->Genere_Attributs_Facettes(p_Objet, Nb_facettes, numeroGroupe, numeroMateriau);
    Genere_Normales_Sommets(Element, p_Objet);
    Genere_Luminances(Element);
    p_Objet->flat = false;

    Element->Genere_Tableau_Points_Facettes(p_Objet);
    Element->Genere_Tableau_Aretes_OK = true;
    Element->Genere_Tableau_Aretes(p_Objet);
    Element->Genere_Liste_Groupes_Materiaux(p_Objet);

    Element->bdd_modifiee = true;
}

void Cone::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter* Element = MAIN->Element;

    longueur = wxAtof(TextCtrl_Longueur->GetValue());
//    printf("size :%d\n",centre_cube.size());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    rayon    = wxAtof(TextCtrl_Rayon->GetValue());
    NbPoints = wxAtoi(TextCtrl_NbPoints->GetValue());
//    printf("centre : %f %f %f\narete  : %f\n",centre_cube[0],centre_cube[1],centre_cube[2],arete);
    numeroGroupe   = SpinCtrl_Groupe  ->GetValue() ;    // Par précaution, mais déjà fait !
    numeroMateriau = SpinCtrl_Materiau->GetValue() ;    // idem

    genereCone();

    Element->type = 1;  // Marquer comme si c'était un fichier .bdd
    Element->type_new = 1;
    Element->m_gllist = 0;

    Element->Search_Min_Max();
    Element->m_loaded = true;
    Element->OK_ToSave= true;
    Element->Refresh();
    Hide();
}

void Cone::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Cone::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

void Cone::OnSpinCtrl_NbSecteursChange(wxSpinEvent& event)
{
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
    n_slices   = n_secteurs +1 ;
}
