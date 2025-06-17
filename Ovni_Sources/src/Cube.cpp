#include "Cube.h"

//(*InternalHeaders(Cube)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Cube)
const long Cube::ID_STATICTEXT1 = wxNewId();
const long Cube::ID_TEXTCTRL1 = wxNewId();
const long Cube::ID_STATICLINE1 = wxNewId();
const long Cube::ID_STATICTEXT2 = wxNewId();
const long Cube::ID_STATICTEXT3 = wxNewId();
const long Cube::ID_TEXTCTRL2 = wxNewId();
const long Cube::ID_STATICTEXT4 = wxNewId();
const long Cube::ID_TEXTCTRL3 = wxNewId();
const long Cube::ID_STATICTEXT5 = wxNewId();
const long Cube::ID_TEXTCTRL4 = wxNewId();
const long Cube::ID_BUTTON1 = wxNewId();
const long Cube::ID_BUTTON2 = wxNewId();
const long Cube::ID_STATICTEXT6 = wxNewId();
const long Cube::ID_SPINCTRL1 = wxNewId();
const long Cube::ID_STATICTEXT7 = wxNewId();
const long Cube::ID_SPINCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Cube,wxDialog)
	//(*EventTable(Cube)
	//*)
END_EVENT_TABLE()

Cube::Cube(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Cube::Cube Création d'une forme générique en cube ou parallélépipède
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Cube)
	Create(parent, id, _T("Ajout d\'un cube"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(400,151));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Arête du cube :"), wxPoint(88,3), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Arete = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1.0"), wxPoint(184,0), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(0,28), wxSize(400,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Coordonnées du centre du cube :"), wxPoint(112,32), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("X :"), wxPoint(8,59), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(24,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Y :"), wxPoint(136,59), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(152,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Z :"), wxPoint(264,59), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(280,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(88,120), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(208,120), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Numéro de Groupe :"), wxPoint(8,91), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(120,88), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Matériau :"), wxPoint(200,91), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(320,88), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cube::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cube::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cube::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cube::OnSpinCtrl_MateriauChange);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&Cube::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Cube::OnClose);
	//*)
//	this->Cube_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Cube::~Cube()
{
	//(*Destroy(Cube)
	//*)
}


void Cube::OnInit(wxInitDialogEvent& event)
{
//    centre_cube.resize(3);
}

void Cube::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Cube::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Cube::Genere_Sommets(BddInter *Element, Object *p_Objet) //int indiceObjet)
{
    int Nb_sommets = 8 ;
    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    float demi = arete/2;

    Element->str.clear();
    Element->N_elements = 8;
    Element->make_sommet();

//    Element->Objetlist[indiceObjet].Nb_sommets = Nb_sommets;
    p_Objet->Nb_sommets = Nb_sommets;

    Element->N_elements=1; Element->Setxyz(Xc-demi,Yc+demi,Zc+demi); Element->make_1_sommet();
    Element->N_elements=2; Element->Setxyz(Xc+demi,Yc+demi,Zc+demi); Element->make_1_sommet();
    Element->N_elements=3; Element->Setxyz(Xc+demi,Yc-demi,Zc+demi); Element->make_1_sommet();
    Element->N_elements=4; Element->Setxyz(Xc-demi,Yc-demi,Zc+demi); Element->make_1_sommet();
    Element->N_elements=5; Element->Setxyz(Xc-demi,Yc+demi,Zc-demi); Element->make_1_sommet();
    Element->N_elements=6; Element->Setxyz(Xc+demi,Yc+demi,Zc-demi); Element->make_1_sommet();
    Element->N_elements=7; Element->Setxyz(Xc+demi,Yc-demi,Zc-demi); Element->make_1_sommet();
    Element->N_elements=8; Element->Setxyz(Xc-demi,Yc-demi,Zc-demi); Element->make_1_sommet();
}

//void Cube::Genere_Luminances(BddInter* Element, int indiceObjet)
//{
//    int Nb_facettes = 6 ;
//    Element->str.clear();
//    Element->N_elements = Nb_facettes;
//    Element->make_luminance();
//
//    for (int n=1; n <= 6; n++) {
//        Element->str.Printf(_T("%d 4 %d %d %d %d"),n,n,n,n,n); Element->make_1_luminance();
//    }
//}

//void Cube::Genere_Normales_Sommets(BddInter* Element, int indiceObjet)
//{
//    Element->str.clear();
//    Element->N_elements = 6;
//    Element->Objetlist[indiceObjet].Nb_vecteurs = 6;
//    Element->make_vecteur();
//
//    Element->N_elements=1; Element->Setxyz( 0.0, 0.0, 1.0); Element->make_1_vecteur();
//    Element->N_elements=2; Element->Setxyz( 1.0, 0.0, 0.0); Element->make_1_vecteur();
//    Element->N_elements=3; Element->Setxyz( 0.0, 0.0,-1.0); Element->make_1_vecteur();
//    Element->N_elements=4; Element->Setxyz(-1.0, 0.0, 0.0); Element->make_1_vecteur();
//    Element->N_elements=5; Element->Setxyz( 0.0, 1.0, 0.0); Element->make_1_vecteur();
//    Element->N_elements=6; Element->Setxyz( 0.0,-1.0, 0.0); Element->make_1_vecteur();
//}

void Cube::Genere_Facettes(BddInter *Element, Object *p_Objet) //int indiceObjet)
{
//    Object *p_Objet;
    int Nb_facettes = 6 ;
    Element->str.clear();
    Element->N_elements = Nb_facettes;
    Element->make_face();

//    p_Objet = &(Element->Objetlist[indiceObjet]);
    p_Objet->Nb_facettes = Nb_facettes;

    Element->str = _T("1 4 4 3 2 1"); Element->make_1_face();
    Element->str = _T("2 4 3 7 6 2"); Element->make_1_face();
    Element->str = _T("3 4 7 8 5 6"); Element->make_1_face();
    Element->str = _T("4 4 8 4 1 5"); Element->make_1_face();
    Element->str = _T("5 4 1 2 6 5"); Element->make_1_face();
    Element->str = _T("6 4 8 7 3 4"); Element->make_1_face();

    for (int j=0; j<Nb_facettes ; j++) p_Objet->Facelist[j].flat = true ; // Force le mode facettes planes
}


void Cube::Genere_Cube()
{
    wxString num_obj;
    int new_num;
    Object *p_Objet;

    BddInter *Element = MAIN->Element;

    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetNumero() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Cube - ") + num_obj;
    Element->make_objet();
    Element->Objetlist.rbegin()->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->GetIndiceObjetCourant();

    printf("centre : %f %f %f\narete  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],arete);

    p_Objet = &(Element->Objetlist[indiceObjet]);

    Genere_Facettes(Element, p_Objet);//indiceObjet);
    Genere_Sommets (Element, p_Objet);//indiceObjet);

    Element->Genere_Normales_Facettes (p_Objet, 6);
    Element->Genere_Attributs_Facettes(p_Objet, 6, numeroGroupe, numeroMateriau);
    p_Objet->flat = true; // En fait serait plat de toutes façons avec les normales aux sommets calculées

////    forcer le nombre de Luminances à 0, mais ne suffit pas !
//    Element->str.clear();
//    Element->N_elements = 0;
//    Element->make_luminance();

    Element->Genere_Tableau_Points_Facettes(p_Objet);
    Element->SetGenereTableauAretes_OK(true);
    Element->Genere_Tableau_Aretes(p_Objet);
    Element->Genere_Liste_Groupes_Materiaux(p_Objet);

    Element->SetBddModifiee(true);;
}

void Cube::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    arete = wxAtof(TextCtrl_Arete->GetValue());
//    printf("size :%d\n",centre_cube.size());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
//    printf("centre : %f %f %f\narete  : %f\n",centre_cube[0],centre_cube[1],centre_cube[2],arete);
    numeroGroupe   = SpinCtrl_Groupe  ->GetValue() ;    // Par précaution, mais déjà fait !
    numeroMateriau = SpinCtrl_Materiau->GetValue() ;    // idem

    Genere_Cube();

    Element->SetTypeFichier(1);     // Marquer comme si c'était un fichier .bdd
    Element->SetTypeDxf(false);
    Element->m_gllist = 0;

    Element->Search_Min_Max();
    Element->SetFileLoaded(true);
    Element->SetOK_ToSave (true);

    Element->Refresh();
    Hide();
}

void Cube::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Cube::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

