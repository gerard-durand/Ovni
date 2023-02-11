#include "PropertiesPanel.h"

//(*InternalHeaders(PropertiesPanel)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(PropertiesPanel)
const long PropertiesPanel::ID_STATICTEXT1 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT2 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT3 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT4 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT5 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT6 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT7 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT8 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT9 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT10 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT11 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT12 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT13 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT14 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT15 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT16 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT17 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT18 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT19 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT20 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT21 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT22 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT23 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT24 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT25 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT26 = wxNewId();
const long PropertiesPanel::ID_STATICTEXT27 = wxNewId();
const long PropertiesPanel::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PropertiesPanel,wxDialog)
	//(*EventTable(PropertiesPanel)
	//*)
END_EVENT_TABLE()

PropertiesPanel::PropertiesPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PropertiesPanel)
	Create(parent, id, _T("Propriétés de la Bdd"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(335,463));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Fichier(s) de base de données :"), wxPoint(0,1), wxSize(336,20), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText1->SetMinSize(wxSize(325,20));
	StaticText1->SetMaxSize(wxSize(325,20));
	StaticText1->SetForegroundColour(wxColour(255,255,255));
	StaticText1->SetBackgroundColour(wxColour(0,0,0));
	StaticTextNomBdd = new wxStaticText(this, ID_STATICTEXT2, _T("Nom du fichier"), wxPoint(0,20), wxSize(335,20), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
	StaticTextNomBdd->SetForegroundColour(wxColour(255,255,255));
	StaticTextNomBdd->SetBackgroundColour(wxColour(0,0,0));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _T("Nombre d\'objets"), wxPoint(32,56), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT4, _T("Nombre de points"), wxPoint(32,72), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT5, _T("Nombre de facettes"), wxPoint(32,88), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre d\'arêtes"), wxPoint(32,104), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT7, _T("Nombre de groupes"), wxPoint(32,120), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT8, _T("Nombre de matériaux"), wxPoint(32,136), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	nb_objets = new wxStaticText(this, ID_STATICTEXT9, _T("nnnnn"), wxPoint(216,56), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT9"));
	nb_points = new wxStaticText(this, ID_STATICTEXT10, _T("nnnnn"), wxPoint(216,72), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT10"));
	nb_facettes = new wxStaticText(this, ID_STATICTEXT11, _T("nnnnn"), wxPoint(216,88), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT11"));
	nb_aretes = new wxStaticText(this, ID_STATICTEXT12, _T("nnnnn"), wxPoint(216,104), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT12"));
	nb_groupes = new wxStaticText(this, ID_STATICTEXT13, _T("nnnnn"), wxPoint(216,120), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT13"));
	nb_materiaux = new wxStaticText(this, ID_STATICTEXT14, _T("nnnnn"), wxPoint(216,136), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT14"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT15, _T("\nStatistiques"), wxPoint(0,160), wxSize(335,40), wxALIGN_CENTRE, _T("ID_STATICTEXT15"));
	StaticText8->SetForegroundColour(wxColour(255,255,255));
	StaticText8->SetBackgroundColour(wxColour(0,0,0));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT16, _T("Nombre de facettes à 3 points"), wxPoint(32,208), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	StaticText10 = new wxStaticText(this, ID_STATICTEXT17, _T("Nombre de facettes à 4 points et +"), wxPoint(32,224), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	StaticText11 = new wxStaticText(this, ID_STATICTEXT18, _T("Nombre maximum de points"), wxPoint(32,240), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	StaticText12 = new wxStaticText(this, ID_STATICTEXT19, _T(".... dans la facette numéro"), wxPoint(32,256), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	StaticText13 = new wxStaticText(this, ID_STATICTEXT20, _T(".... de l\'objet numéro"), wxPoint(32,272), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	nb_3Points = new wxStaticText(this, ID_STATICTEXT21, _T("nnnnn"), wxPoint(216,208), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT21"));
	nb_4Points = new wxStaticText(this, ID_STATICTEXT22, _T("nnnnn"), wxPoint(216,224), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT22"));
	nb_maxPoints = new wxStaticText(this, ID_STATICTEXT23, _T("nnnnn"), wxPoint(216,240), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT23"));
	numero_facette = new wxStaticText(this, ID_STATICTEXT24, _T("nnnnn"), wxPoint(216,256), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT24"));
	numero_objet = new wxStaticText(this, ID_STATICTEXT25, _T("nnnnn"), wxPoint(216,272), wxSize(60,16), wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT25"));
	StaticText14 = new wxStaticText(this, ID_STATICTEXT26, _T("\nBoite Englobante"), wxPoint(0,296), wxSize(335,40), wxALIGN_CENTRE, _T("ID_STATICTEXT26"));
	StaticText14->SetForegroundColour(wxColour(255,255,255));
	StaticText14->SetBackgroundColour(wxColour(0,0,0));
	Texte_Box = new wxStaticText(this, ID_STATICTEXT27, _T("Texte\n1\n2\n3\n4"), wxPoint(0,344), wxSize(335,88), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT27"));
	BoutonOK = new wxButton(this, ID_BUTTON1, _T("OK"), wxPoint(128,432), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoutonOK->SetFocus();
	BoutonOK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PropertiesPanel::OnBoutonOKClick);
	//*)
}

PropertiesPanel::~PropertiesPanel()
{
	//(*Destroy(PropertiesPanel)
	//*)
}

void PropertiesPanel::OnBoutonOKClick(wxCommandEvent& event)
{
    Hide();
}

void PropertiesPanel::OnInit(wxInitDialogEvent& event)
{
}
