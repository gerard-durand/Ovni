#include "PositionObs_AzimutSite.h"

//(*InternalHeaders(PositionObs_AzimutSite)
#include <wx/font.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(PositionObs_AzimutSite)
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT1 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT2 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT3 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT4 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT5 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_SPINCTRL1 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_SPINCTRL2 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_BUTTON3 = wxNewId();
const wxWindowID PositionObs_AzimutSite::ID_STATICTEXT6 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PositionObs_AzimutSite,wxDialog)
	//(*EventTable(PositionObs_AzimutSite)
	//*)
END_EVENT_TABLE()

PositionObs_AzimutSite::PositionObs_AzimutSite(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PositionObs_AzimutSite)
	Create(parent, id, _T("Position Angulaire Observateur"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(304,119));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Angles de visée de la Base de Données :"), wxPoint(0,8), wxSize(304,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("LAZ :"), wxPoint(8,51), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("LSI :"), wxPoint(168,51), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Azimut"), wxPoint(64,32), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Site"), wxPoint(224,32), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	SpinCtrl_LAZ = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(40,48), wxSize(104,21), 0, -1, 360, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_LSI = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(192,48), wxSize(104,21), 0, -180, 180, 0, _T("ID_SPINCTRL2"));
	Button_Quit = new wxButton(this, ID_BUTTON3, _T("OK"), wxPoint(112,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quit->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText_Warning = new wxStaticText(this, ID_STATICTEXT6, _T("Attention : Rotation en Y <> 0 !"), wxPoint(56,72), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText_Warning->Hide();
	StaticText_Warning->SetForegroundColour(wxColour(255,0,0));
	wxFont StaticText_WarningFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticText_Warning->SetFont(StaticText_WarningFont);

	Connect(ID_SPINCTRL1, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&PositionObs_AzimutSite::OnSpinCtrl_LAZChange);
	Connect(ID_SPINCTRL2, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&PositionObs_AzimutSite::OnSpinCtrl_LSIChange);
	Connect(ID_BUTTON3, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PositionObs_AzimutSite::OnButton_QuitClick);
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&PositionObs_AzimutSite::OnClose);
	//*)
//	printf("Entree PositionObs_AzimutSite\n");
//	this->Pos_Parent=parent;
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

PositionObs_AzimutSite::~PositionObs_AzimutSite()
{
	//(*Destroy(PositionObs_AzimutSite)
	//*)
}

void PositionObs_AzimutSite::OnSpinCtrl_LAZChange(wxSpinEvent& event)
{
// Même code code dans ZoomSpecifique.cpp
    BddInter *Element = MAIN->Element;

    // Amplitude du SpinCtrl : [0, 360°]
    float LAZ = SpinCtrl_LAZ->GetValue();
    if (lround(LAZ) >= 360) {
        LAZ = 0. ;
        SpinCtrl_LAZ->SetValue(0);
    } else if (lround(LAZ) < 0) {
        LAZ = 360. + LAZ ;
        SpinCtrl_LAZ->SetValue(lround(LAZ));
    }
    if (Element->MZoomSpec->IsShown()) Element->MZoomSpec->SpinCtrl_LAZ->SetValue(lround(LAZ));

    int val = fmod(360-LAZ, 360.);
    if (val < 0) val += 360.;
    Element->m_gldata.rotz = val;
    Element->Slider_z->SetValue(val);
    Element->CalculMatriceRot();
    Element->Refresh();
}

void PositionObs_AzimutSite::OnSpinCtrl_LSIChange(wxSpinEvent& event)
{
// Même code code dans ZoomSpecifique.cpp

    BddInter *Element = MAIN->Element;

    // Amplitude du SpinCtrl : [-180, +180°]
    float LSI = SpinCtrl_LSI->GetValue();//*0.1 ;
//    str.Printf(_T("%4.3f"), val);
//    TextCtrl1->SetValue(str);
//    printf("Valeur lue : %4.3f\n", LSI);
    if (lround(LSI) == 180) {
        LSI = -180;
        SpinCtrl_LSI->SetValue(LSI);
    } else if (lround(LSI) == -180) {
        LSI = 180;
        SpinCtrl_LSI->SetValue(LSI);
    }
    if (Element->MZoomSpec->IsShown()) Element->MZoomSpec->SpinCtrl_LSI->SetValue(lround(LSI));

    int val = fmod(LSI+270, 360.);
    if (val < 0) val += 360. ;
    Element->m_gldata.rotx = val;
    Element->Slider_x->SetValue(val);
    Element->CalculMatriceRot();
    Element->Refresh();
}

void PositionObs_AzimutSite::OnButton_QuitClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void PositionObs_AzimutSite::OnClose(wxCloseEvent& event)
{
    Hide();
}


