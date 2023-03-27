#include "Facette.h"
#include "vector3d.h"

//(*InternalHeaders(Facette)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Facette)
const long Facette::ID_STATICTEXT1 = wxNewId();
const long Facette::ID_TEXTCTRL1 = wxNewId();
const long Facette::ID_STATICTEXT2 = wxNewId();
const long Facette::ID_TEXTCTRL2 = wxNewId();
const long Facette::ID_STATICTEXT3 = wxNewId();
const long Facette::ID_TEXTCTRL3 = wxNewId();
const long Facette::ID_STATICTEXT4 = wxNewId();
const long Facette::ID_TEXTCTRL4 = wxNewId();
const long Facette::ID_STATICTEXT5 = wxNewId();
const long Facette::ID_TEXTCTRL5 = wxNewId();
const long Facette::ID_STATICTEXT6 = wxNewId();
const long Facette::ID_TEXTCTRL6 = wxNewId();
const long Facette::ID_STATICTEXT7 = wxNewId();
const long Facette::ID_TEXTCTRL7 = wxNewId();
const long Facette::ID_STATICTEXT8 = wxNewId();
const long Facette::ID_TEXTCTRL8 = wxNewId();
const long Facette::ID_STATICTEXT9 = wxNewId();
const long Facette::ID_TEXTCTRL9 = wxNewId();
const long Facette::ID_STATICTEXT10 = wxNewId();
const long Facette::ID_SPINCTRL1 = wxNewId();
const long Facette::ID_STATICTEXT11 = wxNewId();
const long Facette::ID_SPINCTRL2 = wxNewId();
const long Facette::ID_BUTTON1 = wxNewId();
const long Facette::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Facette,wxDialog)
	//(*EventTable(Facette)
	//*)
END_EVENT_TABLE()

Facette::Facette(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Facette::Facette Création d'une forme générique de Facette
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Facette)
	Create(parent, id, _T("Ajout d\'un objet Facette"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(449,142));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Coordonnées de P1 :  X:"), wxPoint(8,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_P1X = new wxTextCtrl(this, ID_TEXTCTRL1, _T("0.0"), wxPoint(136,8), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Y:"), wxPoint(224,11), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_P1Y = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(240,8), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Z :"), wxPoint(336,11), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_P1Z = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(360,8), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Coordonnées de P2 :  X:"), wxPoint(8,35), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_P2X = new wxTextCtrl(this, ID_TEXTCTRL4, _T("1.0"), wxPoint(136,32), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Y :"), wxPoint(224,35), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_P2Y = new wxTextCtrl(this, ID_TEXTCTRL5, _T("0.0"), wxPoint(240,32), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Z :"), wxPoint(336,35), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	TextCtrl_P2Z = new wxTextCtrl(this, ID_TEXTCTRL6, _T("0.0"), wxPoint(360,32), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Coordonnées de P3 :  X:"), wxPoint(8,59), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	TextCtrl_P3X = new wxTextCtrl(this, ID_TEXTCTRL7, _T("0.0"), wxPoint(136,56), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Y :"), wxPoint(224,59), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	TextCtrl_P3Y = new wxTextCtrl(this, ID_TEXTCTRL8, _T("1.0"), wxPoint(240,56), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Z :"), wxPoint(336,59), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	TextCtrl_P3Z = new wxTextCtrl(this, ID_TEXTCTRL9, _T("0.0"), wxPoint(360,56), wxSize(80,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _T("Numéro de Groupe :"), wxPoint(8,83), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(136,80), wxSize(80,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _T("Numéro de Matériau :"), wxPoint(232,83), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(360,80), wxSize(80,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(128,112), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(240,112), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Facette::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Facette::OnSpinCtrl_MateriauChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Facette::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Facette::OnButton_AnnulerClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Facette::OnClose);
	//*)
//	this->Facette_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Facette::~Facette()
{
	//(*Destroy(Facette)
	//*)
}

void Facette::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Facette::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Facette::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Facette::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

void Facette::genereFacette()
{
    wxString num_obj, str_loc;
    int new_num;
    Object *p_Objet;

    BddInter* Element = MAIN->Element;

// Objet Facette
    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetValue() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Facette - ") + num_obj;
    Element->make_objet();
    Element->Objetlist.rbegin()->primitive = true;
    int indiceObjet = Element->indiceObjet_courant;

    p_Objet = &(Element->Objetlist[indiceObjet]);

// Facette
    Element->str.clear();
    Element->N_elements = 1;
    Element->make_face();
    p_Objet->Nb_facettes = 1;
    Element->str = _T("1 3 1 2 3"); Element->make_1_face();

    p_Objet->Facelist[0].flat = true;   // On force le mode facette plane

// Sommets
    Element->str.clear();
    Element->N_elements = 3;
    Element->make_sommet();
    p_Objet->Nb_sommets = 3;
    float X, Y, Z;
    X = wxAtof(TextCtrl_P1X->GetValue()); Y = wxAtof(TextCtrl_P1Y->GetValue()); Z = wxAtof(TextCtrl_P1Z->GetValue());
    Vector3D P1(X,Y,Z);
    Element->N_elements=1; Element->Setxyz(X,Y,Z); Element->make_1_sommet();
    X = wxAtof(TextCtrl_P2X->GetValue()); Y = wxAtof(TextCtrl_P2Y->GetValue()); Z = wxAtof(TextCtrl_P2Z->GetValue());
    Vector3D P2(X,Y,Z);
    Element->N_elements=2; Element->Setxyz(X,Y,Z); Element->make_1_sommet();
    X = wxAtof(TextCtrl_P3X->GetValue()); Y = wxAtof(TextCtrl_P3Y->GetValue()); Z = wxAtof(TextCtrl_P3Z->GetValue());
    Vector3D P3(X,Y,Z);
    Element->N_elements=3; Element->Setxyz(X,Y,Z); Element->make_1_sommet();

// Normale à la facette
    Vector3D v1 = P2 - P1;
    Vector3D v2 = P3 - P1;

    Vector3D vn = v1.crossProduct(v2);
    vn.normalize();

    Element->str.clear();
    Element->N_elements = 1;
    Element->make_normale();
    p_Objet->Nb_normales = 1;
    Element->N_elements = 1; Element->Setxyz( vn.X, vn.Y, vn.Z); Element->make_1_normale();

// Attributs de la facette
    Element->Genere_Attributs_Facettes(p_Objet, 1, numeroGroupe, numeroMateriau);

// Luminance
    p_Objet->flat = true; // En fait serait de toutes façons plat avec les normales aux sommets ci-dessous
    p_Objet->Nb_vecteurs   = 0;
    p_Objet->Nb_luminances = 0;

    Element->Genere_Tableau_Points_Facettes(p_Objet);
    Element->Genere_Tableau_Aretes_OK = true;
    Element->Genere_Tableau_Aretes(p_Objet);
    Element->Genere_Liste_Groupes_Materiaux(p_Objet);

    Element->bdd_modifiee = true;
}

void Facette::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter* Element = MAIN->Element;

    numeroGroupe   = SpinCtrl_Groupe  ->GetValue() ;    // Par précaution, mais déjà fait !
    numeroMateriau = SpinCtrl_Materiau->GetValue() ;    // idem

    genereFacette();

    Element->type = 1;  // Marquer comme si c'était un fichier .bdd
    Element->type_new = 1;
    Element->m_gllist = 0;

    Element->Search_Min_Max();
    Element->m_loaded = true;
    Element->OK_ToSave= true;
    Element->Refresh();
    Hide();
}


