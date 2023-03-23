#include "ReperageGroupe.h"

//(*InternalHeaders(ReperageGroupe)
#include <wx/settings.h>
#include <wx/string.h>
//*)

#include "OvniMain.h"

//(*IdInit(ReperageGroupe)
const long ReperageGroupe::ID_STATICTEXT1 = wxNewId();
const long ReperageGroupe::ID_TEXTCTRL1 = wxNewId();
const long ReperageGroupe::ID_BUTTON1 = wxNewId();
const long ReperageGroupe::ID_BUTTON2 = wxNewId();
const long ReperageGroupe::ID_SPINBUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ReperageGroupe,wxDialog)
	//(*EventTable(ReperageGroupe)
	//*)
END_EVENT_TABLE()

ReperageGroupe::ReperageGroupe(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ReperageGroupe)
	Create(parent, id, _T("Repérage d\'un groupe"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(230,75));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Numéro du groupe :"), wxPoint(8,20), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _T("Aucun"), wxPoint(128,16), wxSize(80,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Valider"), wxPoint(8,48), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->Disable();
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quit = new wxButton(this, ID_BUTTON2, _T("Quitter"), wxPoint(128,48), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Quit->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	SpinButton1 = new wxSpinButton(this, ID_SPINBUTTON1, wxPoint(208,16), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON1"));
	SpinButton1->SetRange(-1, 100);

	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageGroupe::OnButton_QuitClick);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ReperageGroupe::OnSpinButton1Change);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ReperageGroupe::OnClose);
	//*)
//	this->Grp_Parent=parent;
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ReperageGroupe::~ReperageGroupe()
{
	//(*Destroy(ReperageGroupe)
	//*)
}


void ReperageGroupe::OnSpinButton1Change(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str;

    int num = SpinButton1->GetValue();

    if (num < 0) {
        num = Element->listeGroupes.size();
        SpinButton1->SetValue(num);
    }
    if (num > (int)Element->listeGroupes.size()) {
        num = 0;
        SpinButton1->SetValue(num);
    }
    if (num == 0) {
        str.Printf(_T("Aucun"));
        Element->GroupeMateriau[0]=0;
    } else {
        auto it = Element->listeGroupes.begin();
        for (int n=1; n<num; n++, it++) ;   // Pointer la num_ième valeur
        str.Printf(_T("%d"),(int)*it);      // Récupérer la num_ième valeur numérique de la liste
        Element->GroupeMateriau[0]=1  ;
        Element->GroupeMateriau[1]=*it;
    }
    TextCtrl1->SetValue(str);
    Element->m_gllist = 0;
    Element->Refresh();
}

void ReperageGroupe::OnButton_QuitClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ReperageGroupe::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->GroupeMateriau[0]=0;
    Element->m_gllist = 0;
    Element->Refresh();
    Hide();
}
