#include "ReperageMateriau.h"

//(*InternalHeaders(ReperageMateriau)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "OvniMain.h"

//(*IdInit(ReperageMateriau)
const long ReperageMateriau::ID_STATICTEXT1 = wxNewId();
const long ReperageMateriau::ID_TEXTCTRL1 = wxNewId();
const long ReperageMateriau::ID_BUTTON1 = wxNewId();
const long ReperageMateriau::ID_BUTTON2 = wxNewId();
const long ReperageMateriau::ID_SPINBUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ReperageMateriau,wxDialog)
	//(*EventTable(ReperageMateriau)
	//*)
END_EVENT_TABLE()

ReperageMateriau::ReperageMateriau(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ReperageMateriau)
	Create(parent, id, _T("Repérage d\'un matériau"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(230,75));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Numéro du matériau :"), wxPoint(8,20), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _T("Aucun"), wxPoint(128,16), wxSize(80,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Valider"), wxPoint(8,48), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->Disable();
	Button_Quit = new wxButton(this, ID_BUTTON2, _T("Quitter"), wxPoint(128,48), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	SpinButton1 = new wxSpinButton(this, ID_SPINBUTTON1, wxPoint(208,16), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON1"));
	SpinButton1->SetRange(0, 100);

	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageMateriau::OnButton_QuitClick);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ReperageMateriau::OnSpinButton1Change);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ReperageMateriau::OnClose);
	//*)
//	this->Mat_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);

}

ReperageMateriau::~ReperageMateriau()
{
	//(*Destroy(ReperageMateriau)
	//*)
}


void ReperageMateriau::OnSpinButton1Change(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str;
    int num=SpinButton1->GetValue();
    if (num == 0) {
        str.Printf(_T("Aucun"));
        Element->GroupeMateriau[0]=0;
    } else {
        auto it = Element->listeMateriaux.begin();
        for (int n=1; n<num; n++, it++) ;   // Pointer la num_ième valeur
        str.Printf(_T("%d"),(int)*it);      // Récupérer la num_ième valeur numérique de la liste
        Element->GroupeMateriau[0]=2  ;
        Element->GroupeMateriau[1]=*it;
    }
    TextCtrl1->SetValue(str);
    Element->m_gllist = 0;
    Element->Refresh();
}

void ReperageMateriau::OnButton_QuitClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ReperageMateriau::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->GroupeMateriau[0]=0;
    Element->m_gllist = 0;
    Element->Refresh();
    Hide();
}
