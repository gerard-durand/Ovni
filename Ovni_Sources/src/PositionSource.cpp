#include "PositionSource.h"

//(*InternalHeaders(PositionSource)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(PositionSource)
const long PositionSource::ID_STATICTEXT5 = wxNewId();
const long PositionSource::ID_STATICTEXT1 = wxNewId();
const long PositionSource::ID_STATICTEXT2 = wxNewId();
const long PositionSource::ID_STATICTEXT3 = wxNewId();
const long PositionSource::ID_STATICTEXT4 = wxNewId();
const long PositionSource::ID_SPINCTRLDOUBLE1 = wxNewId();
const long PositionSource::ID_SPINCTRLDOUBLE2 = wxNewId();
const long PositionSource::ID_SPINCTRLDOUBLE3 = wxNewId();
const long PositionSource::ID_SPINCTRLDOUBLE4 = wxNewId();
const long PositionSource::ID_BUTTON2 = wxNewId();
const long PositionSource::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PositionSource,wxDialog)
	//(*EventTable(PositionSource)
	//*)
END_EVENT_TABLE()

PositionSource::PositionSource(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PositionSource)
	Create(parent, id, _T("Position de la Source de Lumière"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(238,168));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT5, _T("En coordonnées Écran (œuil)"), wxPoint(40,8), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	Pos_X = new wxStaticText(this, ID_STATICTEXT1, _T("X :"), wxPoint(60,35), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	Pos_X->SetForegroundColour(wxColour(255,0,0));
	Pos_Y = new wxStaticText(this, ID_STATICTEXT2, _T("Y :"), wxPoint(60,59), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	Pos_Y->SetForegroundColour(wxColour(0,220,0));
	Pos_Z = new wxStaticText(this, ID_STATICTEXT3, _T("Z :"), wxPoint(60,83), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	Pos_Z->SetForegroundColour(wxColour(0,0,255));
	Pos_W = new wxStaticText(this, ID_STATICTEXT4, _T("W :"), wxPoint(60,107), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	SpinCtrlDouble_PosX = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE1, _T("0"), wxPoint(88,32), wxSize(96,23), 0, -100, 100, 0, 0.5, _T("ID_SPINCTRLDOUBLE1"));
	SpinCtrlDouble_PosX->SetValue(_T("0"));
	SpinCtrlDouble_PosY = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE2, _T("0"), wxPoint(88,56), wxSize(96,23), 0, -100, 100, 0, 0.5, _T("ID_SPINCTRLDOUBLE2"));
	SpinCtrlDouble_PosY->SetValue(_T("0"));
	SpinCtrlDouble_PosZ = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE3, _T("0"), wxPoint(88,80), wxSize(96,23), 0, -100, 100, 0, 0.5, _T("ID_SPINCTRLDOUBLE3"));
	SpinCtrlDouble_PosZ->SetValue(_T("0"));
	SpinCtrlDouble_PosW = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE4, _T("0"), wxPoint(88,104), wxSize(96,23), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE4"));
	SpinCtrlDouble_PosW->SetValue(_T("0"));
	Button_Quitter = new wxButton(this, ID_BUTTON2, _T("Quitter"), wxPoint(128,136), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Defaut = new wxButton(this, ID_BUTTON3, _T("Défaut"), wxPoint(32,136), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Defaut->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRLDOUBLE1, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&PositionSource::OnSpinCtrlDouble_PosXChange);
	Connect(ID_SPINCTRLDOUBLE2, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&PositionSource::OnSpinCtrlDouble_PosYChange);
	Connect(ID_SPINCTRLDOUBLE3, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&PositionSource::OnSpinCtrlDouble_PosZChange);
	Connect(ID_SPINCTRLDOUBLE4, wxEVT_SPINCTRLDOUBLE, (wxObjectEventFunction)&PositionSource::OnSpinCtrlDouble_PosWChange);
	Connect(ID_BUTTON2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PositionSource::OnButton_QuitterClick);
	Connect(ID_BUTTON3, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PositionSource::OnButton_DefautClick);
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&PositionSource::OnClose);
	//*)

//    this->Pos_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

PositionSource::~PositionSource()
{
	//(*Destroy(PositionSource)
	//*)
}

void PositionSource::OnSpinCtrlDouble_PosXChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Light0Position[0]=SpinCtrlDouble_PosX->GetValue();
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void PositionSource::OnSpinCtrlDouble_PosYChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Light0Position[1]=SpinCtrlDouble_PosY->GetValue();
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void PositionSource::OnSpinCtrlDouble_PosZChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Light0Position[2]=SpinCtrlDouble_PosZ->GetValue();
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void PositionSource::OnSpinCtrlDouble_PosWChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Light0Position[3]=SpinCtrlDouble_PosW->GetValue();
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void PositionSource::OnButton_DefautClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    for (int i=0;i<4;i++) {
        Element->Light0Position[i] = Element->Light0Position_def[i];
    }
    SpinCtrlDouble_PosX->SetValue(Element->Light0Position[0]);
    SpinCtrlDouble_PosY->SetValue(Element->Light0Position[1]);
    SpinCtrlDouble_PosZ->SetValue(Element->Light0Position[2]);
    SpinCtrlDouble_PosW->SetValue(Element->Light0Position[3]);
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void PositionSource::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void PositionSource::OnClose(wxCloseEvent& event)
{
    Hide();
}
