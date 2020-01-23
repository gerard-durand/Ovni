#include "Sphere.h"

//(*InternalHeaders(Sphere)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Sphere)
const long Sphere::ID_STATICTEXT1 = wxNewId();
const long Sphere::ID_TEXTCTRL1 = wxNewId();
const long Sphere::ID_STATICTEXT2 = wxNewId();
const long Sphere::ID_TEXTCTRL2 = wxNewId();
const long Sphere::ID_STATICTEXT3 = wxNewId();
const long Sphere::ID_TEXTCTRL3 = wxNewId();
const long Sphere::ID_STATICTEXT4 = wxNewId();
const long Sphere::ID_TEXTCTRL4 = wxNewId();
const long Sphere::ID_STATICTEXT5 = wxNewId();
const long Sphere::ID_TEXTCTRL5 = wxNewId();
const long Sphere::ID_STATICTEXT6 = wxNewId();
const long Sphere::ID_TEXTCTRL6 = wxNewId();
const long Sphere::ID_BUTTON1 = wxNewId();
const long Sphere::ID_BUTTON2 = wxNewId();
const long Sphere::ID_STATICTEXT7 = wxNewId();
const long Sphere::ID_SPINCTRL1 = wxNewId();
const long Sphere::ID_STATICTEXT8 = wxNewId();
const long Sphere::ID_SPINCTRL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Sphere,wxDialog)
	//(*EventTable(Sphere)
	//*)
END_EVENT_TABLE()

Sphere::Sphere(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Sphere)
	Create(parent, id, _T("Ajout d\'une sphère"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(409,164));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Rayon :"), wxPoint(144,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Rayon = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1.0"), wxPoint(200,8), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Centre de la sphère : X :"), wxPoint(5,43), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(128,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Y :"), wxPoint(208,43), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(224,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Z :"), wxPoint(312,43), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(328,40), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Nombre de Méridiens :"), wxPoint(8,75), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_NbMeridiens = new wxTextCtrl(this, ID_TEXTCTRL5, _T("20"), wxPoint(128,72), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre de Parallèles :"), wxPoint(208,75), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	TextCtrl_NbParalleles = new wxTextCtrl(this, ID_TEXTCTRL6, _T("10"), wxPoint(328,72), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(104,136), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(208,136), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Groupe :"), wxPoint(8,107), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(128,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Matériau :"), wxPoint(208,107), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(328,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Sphere::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Sphere::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Sphere::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Sphere::OnSpinCtrl_MateriauChange);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Sphere::OnClose);
	//*)
//	this->Sphere_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Sphere::~Sphere()
{
	//(*Destroy(Sphere)
	//*)
}

void Sphere::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Sphere::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Sphere::genereSphere()
{
    wxString num_obj;
    int new_num;

    BddInter* Element = MAIN->Element;

//  Récupérer plutôt le numéro du dernier objet au lieu de l'indice et l'incrémenter
    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetValue() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Sphere - ") + num_obj;
    Element->makeobjet();
    Element->Objetlist.rbegin()->primitive = true; // <=> (Element->Objetlist.end()-1)->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->indiceObjet_courant; //Element->Objetlist.size() -1; // Car c'est le dernier ! MaJ dans makeobjet
//    Element->indiceObjet_courant = indiceObjet;

    printf("centre : %f %f %f\nrayon  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],rayon);
    printf("Nombre de meridiens  : %d\n",Nb_Meridiens) ;
    printf("Nombre de paralleles : %d\n",Nb_Paralleles);
    int Nb_facettes = 2*Nb_Meridiens*Nb_Paralleles;

    Element->genereFacettesSphere(Nb_Meridiens, Nb_Paralleles);
    Element->genereSommetsSphere (Nb_Meridiens, Nb_Paralleles, centre_primitive, rayon, 1.0, 1.0, 1.0);
    Element->genereNormalesFacettes (indiceObjet, Nb_facettes);
    Element->genereAttributsFacettes(indiceObjet, Nb_facettes, numeroGroupe, numeroMateriau);
    Element->genereLuminances(indiceObjet, Nb_facettes);
    Element->Objetlist[indiceObjet].flat = false;
    Element->genereNormalesSommetsSphere(Nb_Meridiens, Nb_Paralleles, 1.0, 1.0, 1.0);

    Element->GenereTableauPointsFacettes(&Element->Objetlist[indiceObjet]);
    Element->GenereTableauAretes(&Element->Objetlist[indiceObjet]);
    Element->GenereListeGroupesMateriaux(indiceObjet);

    Element->bdd_modifiee = true;
}

void Sphere::OnButton_OKClick(wxCommandEvent& event)
{
    wxString str;

    BddInter* Element = MAIN->Element;

    rayon = wxAtof(TextCtrl_Rayon->GetValue());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    Nb_Meridiens  = wxAtoi(TextCtrl_NbMeridiens ->GetValue());
    Nb_Paralleles = wxAtoi(TextCtrl_NbParalleles->GetValue());
    numeroGroupe  = SpinCtrl_Groupe  ->GetValue() ;     // Par précaution, mais déjà fait !
    numeroMateriau= SpinCtrl_Materiau->GetValue() ;     // idem

    genereSphere();

    Element->type = 1;  // Marquer comme si c'était un fichier .bdd
    Element->type_new = 1;
    Element->m_gllist = 0;

    Element->searchMin_Max();
    Element->m_loaded = true;
    Element->OK_ToSave= true;
    Element->Refresh();
    Hide();
}


void Sphere::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Sphere::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

