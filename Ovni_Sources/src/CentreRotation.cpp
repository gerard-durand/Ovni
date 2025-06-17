#include "CentreRotation.h"

//(*InternalHeaders(CentreRotation)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(CentreRotation)
const wxWindowID CentreRotation::ID_STATICTEXT1 = wxNewId();
const wxWindowID CentreRotation::ID_STATICTEXT2 = wxNewId();
const wxWindowID CentreRotation::ID_STATICTEXT3 = wxNewId();
const wxWindowID CentreRotation::ID_STATICTEXT4 = wxNewId();
const wxWindowID CentreRotation::ID_SPINCTRLDOUBLE1 = wxNewId();
const wxWindowID CentreRotation::ID_SPINCTRLDOUBLE2 = wxNewId();
const wxWindowID CentreRotation::ID_SPINCTRLDOUBLE3 = wxNewId();
const wxWindowID CentreRotation::ID_BUTTON1 = wxNewId();
const wxWindowID CentreRotation::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CentreRotation,wxDialog)
	//(*EventTable(CentreRotation)
	//*)
END_EVENT_TABLE()

CentreRotation::CentreRotation(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief CentreRotation::CentreRotation Gestion du centre de rotation de la figure 3D
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(CentreRotation)
	Create(parent, id, _T("Centre de Rotation"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(200,166));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Centre de Rotation"), wxPoint(48,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("X :"), wxPoint(24,44), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText2->SetForegroundColour(wxColour(255,0,0));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Y :"), wxPoint(24,68), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText3->SetForegroundColour(wxColour(0,220,0));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Z :"), wxPoint(24,92), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText4->SetForegroundColour(wxColour(0,0,255));
	SpinCtrlDouble_X = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE1, _T("0"), wxPoint(56,40), wxSize(120,23), 0, -100, 100, 0, 0.1, _T("ID_SPINCTRLDOUBLE1"));
	SpinCtrlDouble_Y = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE2, _T("0"), wxPoint(56,64), wxSize(120,23), 0, -100, 100, 0, 0.1, _T("ID_SPINCTRLDOUBLE2"));
	SpinCtrlDouble_Z = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE3, _T("0"), wxPoint(56,88), wxSize(120,23), 0, -100, 100, 0, 0.1, _T("ID_SPINCTRLDOUBLE3"));
	Button_Defs = new wxButton(this, ID_BUTTON1, _T("Défaut"), wxPoint(8,136), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Defs->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_OK = new wxButton(this, ID_BUTTON2, _T("OK"), wxPoint(104,136), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRLDOUBLE1, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&CentreRotation::OnSpinCtrlDouble_XChange);
	Connect(ID_SPINCTRLDOUBLE2, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&CentreRotation::OnSpinCtrlDouble_YChange);
	Connect(ID_SPINCTRLDOUBLE3, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&CentreRotation::OnSpinCtrlDouble_ZChange);
	Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CentreRotation::OnButton_DefsClick);
	Connect(ID_BUTTON2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CentreRotation::OnButton_OKClick);
	Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction)&CentreRotation::OnInit);
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&CentreRotation::OnClose);
	//*)
//	this->Rot_Parent=parent;
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

CentreRotation::~CentreRotation()
{
	//(*Destroy(CentreRotation)
	//*)
}

void CentreRotation::OnInit(wxInitDialogEvent& event)
{
}

void CentreRotation::OnSpinCtrlDouble_XChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->centreRot[0] = SpinCtrlDouble_X->GetValue();
    Element->SetCentrageRotAuto(false);
    Element->SetPosObs(false);                        // Pour remettre le centre de rotation centré sur la fenètre OpenGL.
    Element->Refresh();
}

void CentreRotation::OnSpinCtrlDouble_YChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->centreRot[1] = SpinCtrlDouble_Y->GetValue();
    Element->SetCentrageRotAuto(false);
    Element->SetPosObs(false);
    Element->Refresh();
}

void CentreRotation::OnSpinCtrlDouble_ZChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->centreRot[2] = SpinCtrlDouble_Z->GetValue();
    Element->SetCentrageRotAuto(false);
    Element->SetPosObs(false);
    Element->Refresh();
}

void CentreRotation::OnButton_DefsClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->centreRot = Element->centre_auto;
    Element->SetCentrageRotAuto(true);
    SpinCtrlDouble_X->SetValue(Element->centreRot[0]);
    SpinCtrlDouble_Y->SetValue(Element->centreRot[1]);
    SpinCtrlDouble_Z->SetValue(Element->centreRot[2]);
    Element->SetPosObs(false);
    Element->Refresh();
}

void CentreRotation::OnButton_OKClick(wxCommandEvent& event)
{
// Boutton OK <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void CentreRotation::OnClose(wxCloseEvent& event)
{
    Hide();
}
