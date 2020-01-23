#include "ZoomSpecifique.h"

//(*InternalHeaders(ZoomSpecifique)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(ZoomSpecifique)
const long ZoomSpecifique::ID_STATICTEXT1 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT2 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT3 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT4 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT5 = wxNewId();
const long ZoomSpecifique::ID_SPINCTRL1 = wxNewId();
const long ZoomSpecifique::ID_SPINCTRL2 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT6 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT7 = wxNewId();
const long ZoomSpecifique::ID_TEXTCTRL1 = wxNewId();
const long ZoomSpecifique::ID_TEXTCTRL2 = wxNewId();
const long ZoomSpecifique::ID_BUTTON2 = wxNewId();
const long ZoomSpecifique::ID_BUTTON1 = wxNewId();
const long ZoomSpecifique::ID_BUTTON3 = wxNewId();
const long ZoomSpecifique::ID_STATICTEXT8 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ZoomSpecifique,wxDialog)
	//(*EventTable(ZoomSpecifique)
	//*)
END_EVENT_TABLE()

ZoomSpecifique::ZoomSpecifique(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ZoomSpecifique)
	Create(parent, id, _T("Zoom Spécifique"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(304,204));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Angles de visée de la Base de Données :"), wxPoint(0,8), wxSize(304,14), wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("LAZ :"), wxPoint(8,51), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("LSI :"), wxPoint(168,51), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Azimut"), wxPoint(64,32), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Site"), wxPoint(224,32), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	SpinCtrl_LAZ = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(40,48), wxSize(104,21), 0, -1, 360, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_LAZ->SetValue(_T("0"));
	SpinCtrl_LSI = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(192,48), wxSize(104,21), 0, -180, 180, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_LSI->SetValue(_T("0"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Distance de visée (exprimée en \nmultiple de la diagonale)"), wxPoint(8,92), wxSize(176,32), 0, _T("ID_STATICTEXT6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Champ de vue (degrés)"), wxPoint(8,132), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	TextCtrl_Distance = new wxTextCtrl(this, ID_TEXTCTRL1, _T("10.00"), wxPoint(192,96), wxSize(102,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl_FoV = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.00"), wxPoint(192,128), wxSize(102,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	Button_Reset = new wxButton(this, ID_BUTTON2, _T("Défaut"), wxPoint(8,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Appliquer = new wxButton(this, ID_BUTTON1, _T("Appliquer"), wxPoint(104,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Appliquer->SetFocus();
	Button_Quit = new wxButton(this, ID_BUTTON3, _T("OK"), wxPoint(200,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	StaticText_Warning = new wxStaticText(this, ID_STATICTEXT8, _T("Attention : Rotation en Y <> 0 !"), wxPoint(48,72), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	StaticText_Warning->Hide();
	StaticText_Warning->SetForegroundColour(wxColour(255,0,0));
	wxFont StaticText_WarningFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticText_Warning->SetFont(StaticText_WarningFont);

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ZoomSpecifique::OnSpinCtrl_LAZChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ZoomSpecifique::OnSpinCtrl_LSIChange);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ZoomSpecifique::OnTextCtrl_DistanceText);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&ZoomSpecifique::OnButton_AppliquerClick);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ZoomSpecifique::OnTextCtrl_FoVText);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&ZoomSpecifique::OnButton_AppliquerClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ZoomSpecifique::OnButton_ResetClick);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ZoomSpecifique::OnButton_AppliquerClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ZoomSpecifique::OnButton_QuitClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ZoomSpecifique::OnClose);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ZoomSpecifique::~ZoomSpecifique()
{
	//(*Destroy(ZoomSpecifique)
	//*)
}

void ZoomSpecifique::OnButton_QuitClick(wxCommandEvent& event)
{
// Boutton OK <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ZoomSpecifique::OnClose(wxCloseEvent& event)
{
    Hide();
}

void ZoomSpecifique::OnSpinCtrl_LAZChange(wxSpinEvent& event)
{
// Copier-coller du code équivallent dans PositionObs_AzimutSite.cpp
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
    if (Element->MPosObs->IsShown()) Element->MPosObs->SpinCtrl_LAZ->SetValue(lround(LAZ));

    int val = fmod(360-LAZ, 360.);
    if (val < 0) val += 360.;
    Element->m_gldata.rotz = val;
    Element->Slider_z->SetValue(val);
    Element->CalculMatriceRot();
    Element->Refresh();
}

void ZoomSpecifique::OnSpinCtrl_LSIChange(wxSpinEvent& event)
{
// Copier-coller du code équivallent dans PositionObs_AzimutSite.cpp
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
    if (Element->MPosObs->IsShown()) Element->MPosObs->SpinCtrl_LSI->SetValue(lround(LSI));

    int val = fmod(LSI+270, 360.);
    if (val < 0) val += 360. ;
    Element->m_gldata.rotx = val;
    Element->Slider_x->SetValue(val);
    Element->CalculMatriceRot();
    Element->Refresh();
}

void ZoomSpecifique::OnTextCtrl_DistanceText(wxCommandEvent& event)
{
    // Ne pas associer à EVT_TEXT ou Ne rien faire, sinon réagit dès qu'un caractère est entré !
}

void ZoomSpecifique::OnTextCtrl_FoVText(wxCommandEvent& event)
{
    // Idem
}

void ZoomSpecifique::OnButton_AppliquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->m_gldata.fmult_diag = wxAtof(TextCtrl_Distance->GetValue());
    Element->m_gldata.zoom       = wxAtof(TextCtrl_FoV->GetValue());
    Element->SetPosObs(false);
    Element->ResetProjectionMode();
    Element->Refresh();
}

void ZoomSpecifique::OnButton_ResetClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->m_gldata.fmult_diag = Element->fmult_diag_def;
    Element->m_gldata.rotx = Element->m_gldata.roty = Element->m_gldata.rotz = 0.0;
    StaticText_Warning->Hide(); // roty est à 0
    SpinCtrl_LSI->SetValue(90);
    if (Element->MPosObs->IsShown()) Element->MPosObs->SpinCtrl_LSI->SetValue(90);
    SpinCtrl_LAZ->SetValue(0);
    if (Element->MPosObs->IsShown()) Element->MPosObs->SpinCtrl_LAZ->SetValue(0);
    Element->reset_zoom    = true;
    Element->SetPosObs(Element->reset_zoom);
    Element->ResetProjectionMode();
    MAIN->SetAngles();

    float val = Element->m_gldata.zoom;
    wxString str_loc;
    str_loc.Printf(_T("%4.3f"),val);
    TextCtrl_FoV->SetLabel(str_loc);
    val = -(Element->m_gldata.posz +Element->centreRot[2])/Element->diagonale_save;
    str_loc.Printf(_T("%4.3f"),val);
    TextCtrl_Distance->SetLabel(str_loc);

    Element->Refresh();
}
