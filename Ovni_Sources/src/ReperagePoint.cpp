#include "ReperagePoint.h"

//(*InternalHeaders(ReperagePoint)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ReperagePoint)
const wxWindowID ReperagePoint::ID_STATICTEXT1 = wxNewId();
const wxWindowID ReperagePoint::ID_SPINCTRL1 = wxNewId();
const wxWindowID ReperagePoint::ID_STATICTEXT2 = wxNewId();
const wxWindowID ReperagePoint::ID_SPINCTRL2 = wxNewId();
const wxWindowID ReperagePoint::ID_TEXTCTRL1 = wxNewId();
const wxWindowID ReperagePoint::ID_STATICTEXT3 = wxNewId();
const wxWindowID ReperagePoint::ID_TEXTCTRL2 = wxNewId();
const wxWindowID ReperagePoint::ID_STATICTEXT4 = wxNewId();
const wxWindowID ReperagePoint::ID_STATICTEXT5 = wxNewId();
const wxWindowID ReperagePoint::ID_STATICTEXT6 = wxNewId();
const wxWindowID ReperagePoint::ID_TEXTCTRL3 = wxNewId();
const wxWindowID ReperagePoint::ID_TEXTCTRL4 = wxNewId();
const wxWindowID ReperagePoint::ID_TEXTCTRL5 = wxNewId();
const wxWindowID ReperagePoint::ID_CHECKBOX1 = wxNewId();
const wxWindowID ReperagePoint::ID_CHECKBOX2 = wxNewId();
const wxWindowID ReperagePoint::ID_CHECKBOX3 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON1 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON2 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON3 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON4 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON5 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON6 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON7 = wxNewId();
const wxWindowID ReperagePoint::ID_CHECKBOX4 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON8 = wxNewId();
const wxWindowID ReperagePoint::ID_BUTTON9 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ReperagePoint,wxDialog)
	//(*EventTable(ReperagePoint)
	//*)
END_EVENT_TABLE()

ReperagePoint::ReperagePoint(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ReperagePoint)
	Create(parent, id, _T("Repérage d\'un point"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(296,253));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Indice/Numéro de l\'objet :"), wxPoint(8,11), wxSize(136,16), 0, _T("ID_STATICTEXT1"));
	SpinCtrl_IndicePoint = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(112,56), wxSize(72,21), 0, -1, 100, 0, _T("ID_SPINCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Nom :"), wxPoint(8,35), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	SpinCtrl_IndiceObjet = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(152,8), wxSize(72,21), 0, -1, 100, 0, _T("ID_SPINCTRL2"));
	Text_NumeroObjet = new wxTextCtrl(this, ID_TEXTCTRL1, _T("nnn"), wxPoint(224,8), wxSize(64,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Text_NumeroObjet->Disable();
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Indice du point :"), wxPoint(8,59), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	Text_NomObjet = new wxTextCtrl(this, ID_TEXTCTRL2, _T("Texte"), wxPoint(48,32), wxSize(240,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	Text_NomObjet->Disable();
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("X :"), wxPoint(8,92), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Y :"), wxPoint(8,116), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Z :"), wxPoint(8,140), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	Text_ValeurX = new wxTextCtrl(this, ID_TEXTCTRL3, _T("xxx"), wxPoint(24,88), wxSize(128,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	Text_ValeurX->Disable();
	Text_ValeurY = new wxTextCtrl(this, ID_TEXTCTRL4, _T("yyy"), wxPoint(24,112), wxSize(128,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	Text_ValeurY->Disable();
	Text_ValeurZ = new wxTextCtrl(this, ID_TEXTCTRL5, _T("zzz"), wxPoint(24,136), wxSize(128,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	Text_ValeurZ->Disable();
	CheckBox_X = new wxCheckBox(this, ID_CHECKBOX1, wxEmptyString, wxPoint(160,92), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_X->SetValue(false);
	CheckBox_Y = new wxCheckBox(this, ID_CHECKBOX2, wxEmptyString, wxPoint(160,116), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_Y->SetValue(false);
	CheckBox_Z = new wxCheckBox(this, ID_CHECKBOX3, wxEmptyString, wxPoint(160,140), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_Z->SetValue(false);
	Button_ModifierX = new wxButton(this, ID_BUTTON1, _T("Modifier"), wxPoint(176,88), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_ModifierX->Disable();
	Button_ModifierX->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_ModifierY = new wxButton(this, ID_BUTTON2, _T("Modifier"), wxPoint(176,112), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_ModifierY->Disable();
	Button_ModifierY->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_ModifierZ = new wxButton(this, ID_BUTTON3, _T("Modifier"), wxPoint(176,136), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_ModifierZ->Disable();
	Button_ModifierZ->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_UndoX = new wxButton(this, ID_BUTTON4, _T("Undo"), wxPoint(232,88), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_UndoX->Disable();
	Button_UndoX->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_UndoY = new wxButton(this, ID_BUTTON5, _T("Undo"), wxPoint(232,112), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button_UndoY->Disable();
	Button_UndoY->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_UndoZ = new wxButton(this, ID_BUTTON6, _T("Undo"), wxPoint(232,136), wxSize(56,23), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button_UndoZ->Disable();
	Button_UndoZ->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Centrer = new wxButton(this, ID_BUTTON7, _T("Centrer la rotation sur le point"), wxPoint(32,168), wxSize(232,23), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	Button_Centrer->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_Laisser = new wxCheckBox(this, ID_CHECKBOX4, _T("Laisser l\'étoile affichée en quittant"), wxPoint(32,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBox_Laisser->SetValue(false);
	Button_Reset = new wxButton(this, ID_BUTTON8, _T("Reset"), wxPoint(32,224), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	Button_Reset->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON9, _T("Quitter"), wxPoint(176,224), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRL1, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&ReperagePoint::OnSpinCtrl_IndicePointChange);
	Connect(ID_SPINCTRL2, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&ReperagePoint::OnSpinCtrl_IndiceObjetChange);
	Connect(ID_CHECKBOX1, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnCheckBox_XClick);
	Connect(ID_CHECKBOX2, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnCheckBox_YClick);
	Connect(ID_CHECKBOX3, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnCheckBox_ZClick);
	Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_ModifierXClick);
	Connect(ID_BUTTON2, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_ModifierYClick);
	Connect(ID_BUTTON3, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_ModifierZClick);
	Connect(ID_BUTTON4, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_UndoXClick);
	Connect(ID_BUTTON5, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_UndoYClick);
	Connect(ID_BUTTON6, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_UndoZClick);
	Connect(ID_BUTTON7, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_CentrerClick);
	Connect(ID_BUTTON8, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_ResetClick);
	Connect(ID_BUTTON9, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ReperagePoint::OnButton_QuitterClick);
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&ReperagePoint::OnClose);
	//*)
//	this->Point_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ReperagePoint::~ReperagePoint()
{
	//(*Destroy(ReperagePoint)
	//*)
}


void ReperagePoint::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ReperagePoint::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

   int IndicePoint = SpinCtrl_IndicePoint->GetValue();

    if (CheckBox_Laisser->IsChecked() && (IndicePoint >= 0))
        Element->SetShowStar(true);
    else
        Element->SetShowStar(false);
    Element->Refresh();
    Hide();
}

void ReperagePoint::OnSpinCtrl_IndicePointChange(wxSpinEvent& event)
{
    ReperagePoint::OnSpinCtrl_IndiceObjetChange(event);
// On peut faire exactement le même traitement pour OnSpinCtrl_IndicePointChange et OnSpinCtrl_IndiceObjetChange

}

void ReperagePoint::OnSpinCtrl_IndiceObjetChange(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str_nom, str_num, str;
    std::vector<float> xyz_sommet;

    int IndiceObjet = SpinCtrl_IndiceObjet->GetValue();
    if (IndiceObjet < 0) {
        IndiceObjet = Element->Objetlist.size() -1;
        SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    }
    if (IndiceObjet >= (int)Element->Objetlist.size()) {
        IndiceObjet = 0;
        SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    }
    SpinCtrl_IndicePoint->SetMax(Element->Objetlist[IndiceObjet].Sommetlist.size() -1);
    int num_obj = Element->Objetlist[IndiceObjet].GetNumero();
    str_num.Printf(_T("%d"),num_obj);
    Text_NumeroObjet->SetValue(str_num);
//    str_nom = Element->Objetlist[IndiceObjet].GetwxName();
    str_nom =  wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    Text_NomObjet->SetValue(str_nom);
    int IndicePoint = SpinCtrl_IndicePoint->GetValue();                     // Ici, le numéro de point peut avoir changé si > max !
    if (IndicePoint < 0) {
        str.Printf(_T(""));
        Text_ValeurX->SetValue(str);
        Text_ValeurY->SetValue(str);
        Text_ValeurZ->SetValue(str);
        Element->SetShowStar(false);
    } else {
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        if (xyz_sommet.empty()) {                                           // Point qui n'a pas été initialisé ???
            str.Printf(_T(""));
            Text_ValeurX->SetValue(str);
            Text_ValeurY->SetValue(str);
            Text_ValeurZ->SetValue(str);
            Element->SetShowStar(false);
        } else {
            str.Printf(_T("%f"),xyz_sommet[0]);
            Text_ValeurX->SetValue(str);
            str.Printf(_T("%f"),xyz_sommet[1]);
            Text_ValeurY->SetValue(str);
            str.Printf(_T("%f"),xyz_sommet[2]);
            Text_ValeurZ->SetValue(str);
            Element->SetShowStar(true);
            Element->SetPointStar(xyz_sommet);
        }
    }
    if (MAIN->Selections_Panel->IsShown()) {                                // Mise à jour de SelectionPanel s'il est affiché
        MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(str_nom);
        if (IndiceObjet < 0) str_num.Printf(_T(""));                        // Laisser vide
        MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(str_num);       // ici c'est le numéro
        str.Printf(_T("%d"),IndiceObjet);
        if (IndiceObjet < 0) str.Printf(_T(""));
        MAIN->Selections_Panel->TextCtrl_IndObjet->SetValue(str);           // et là l'indice de l'objet
        if (IndicePoint < 0)
            str.Printf(_T(""));
        else
            str.Printf(_T("%d"),IndicePoint);
        MAIN->Selections_Panel->TextCtrl_IndFacette->SetValue(str);
        if (IndicePoint < 0)
            str.Printf(_T(""));
        else
            str.Printf(_T("%d"),IndicePoint+1);
        MAIN->Selections_Panel->TextCtrl_NumFacette->SetValue(str);
    }
    Element->Refresh();
}

void ReperagePoint::OnButton_CentrerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;

    int IndiceObjet = SpinCtrl_IndiceObjet->GetValue();
    int IndicePoint = SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        Element->SetCentrageRotAuto(false);
        Element->centreRot = xyz_sommet;  // Imposer le point comme nouveau centre de rotation
        Element->SetPosObs(false);        // Reset de la position observée (centreRot) mais sans changer le zoom
        Element->Refresh();
    }
}

void ReperagePoint::OnButton_ResetClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str;

    wxCommandEvent event_check(wxEVT_COMMAND_CHECKBOX_CLICKED);//, ID_CHECKBOX1);
    wxCommandEvent event_button(wxEVT_COMMAND_BUTTON_CLICKED) ;

    CheckBox_X->SetValue(false);                                        // Pour simuler un décochage de la CheckBox en X
    if (Button_UndoX->IsEnabled()) OnButton_UndoXClick(event_button);   // Simuler un UndoX si besoin
    OnCheckBox_XClick(event_check);                                     // puis décochage

    CheckBox_Y->SetValue(false);                                        // Idem en Y
    if (Button_UndoY->IsEnabled()) OnButton_UndoYClick(event_button);
    OnCheckBox_YClick(event_check);

    CheckBox_Z->SetValue(false);                                        // Idem en Z
    if (Button_UndoZ->IsEnabled()) OnButton_UndoZClick(event_button);
    OnCheckBox_ZClick(event_check);

    Element->SetCentrageRotAuto(true);
    Element->centreRot = Element->centre_auto ;
    Element->SetPosObs(false);

    int IndiceObjet = 0;
    SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    int num_obj = Element->Objetlist[IndiceObjet].GetNumero();
    str.Printf(_T("%d"),num_obj);
    Text_NumeroObjet->SetValue(str);
    str = wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    Text_NomObjet->SetValue(str);

    int IndicePoint=-1;
    SpinCtrl_IndicePoint->SetValue(IndicePoint);
    str.Printf(_T(""));
    Text_ValeurX->SetValue(str);
    Text_ValeurY->SetValue(str);
    Text_ValeurZ->SetValue(str);

    Element->SetShowStar(false);
    Element->Refresh();
}

void ReperagePoint::OnCheckBox_XClick(wxCommandEvent& event)
{
    if (CheckBox_X->IsChecked()) {
        Button_ModifierX->Enable();
        Text_ValeurX->Enable();
    } else {
        Button_ModifierX->Disable();
        Button_UndoX->Disable();
        Text_ValeurX->Disable();
    }
}

void ReperagePoint::OnCheckBox_YClick(wxCommandEvent& event)
{
    if (CheckBox_Y->IsChecked()) {
        Button_ModifierY->Enable();
        Text_ValeurY->Enable();
    } else {
        Button_ModifierY->Disable();
        Button_UndoY->Disable();
        Text_ValeurY->Disable();
    }
}

void ReperagePoint::OnCheckBox_ZClick(wxCommandEvent& event)
{
    if (CheckBox_Z->IsChecked()) {
        Button_ModifierZ->Enable();
        Text_ValeurZ->Enable();
    } else {
        Button_ModifierZ->Disable();
        Button_UndoZ->Disable();
        Text_ValeurZ->Disable();
    }
}

void ReperagePoint::OnButton_ModifierXClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;

    int IndicePoint = SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet=SpinCtrl_IndiceObjet->GetValue();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        if (!Button_UndoX->IsEnabled()) Old_X = xyz_sommet[0];  // Si Undo déjà actif, ne pas changer Old_X
        Button_UndoX->Enable();
        xyz_sommet[0] = wxAtof(Text_ValeurX->GetValue());
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    } else Button_UndoX->Disable();
}

void ReperagePoint::OnButton_ModifierYClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;

    int IndicePoint=SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet=SpinCtrl_IndiceObjet->GetValue();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        if (!Button_UndoY->IsEnabled()) Old_Y = xyz_sommet[1];  // Si Undo déjà actif, ne pas changer Old_Y
        Button_UndoY->Enable();
        xyz_sommet[1] = wxAtof(Text_ValeurY->GetValue());
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    } else Button_UndoY->Disable();
}

void ReperagePoint::OnButton_ModifierZClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;

    int IndicePoint=SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet=SpinCtrl_IndiceObjet->GetValue();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        if (!Button_UndoZ->IsEnabled()) Old_Z = xyz_sommet[2];  // Si Undo déjà actif, ne pas changer Old_Z
        Button_UndoZ->Enable();
        xyz_sommet[2] = wxAtof(Text_ValeurZ->GetValue());
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    } else Button_UndoZ->Disable();
}

void ReperagePoint::OnButton_UndoXClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;
    wxString str;

    int IndicePoint=SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet=SpinCtrl_IndiceObjet->GetValue();
        Button_UndoX->Disable();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        xyz_sommet[0] = Old_X;
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        str.Printf(_T("%f"),Old_X);
        Text_ValeurX->SetValue(str);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    }
}

void ReperagePoint::OnButton_UndoYClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;
    wxString str;

    int IndicePoint=SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet=SpinCtrl_IndiceObjet->GetValue();
        Button_UndoY->Disable();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        xyz_sommet[1] = Old_Y;
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        str.Printf(_T("%f"),Old_Y);
        Text_ValeurY->SetValue(str);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    }
}

void ReperagePoint::OnButton_UndoZClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    std::vector<float> xyz_sommet;
    wxString str;

    int IndicePoint = SpinCtrl_IndicePoint->GetValue();
    if (IndicePoint >= 0) {
        int IndiceObjet = SpinCtrl_IndiceObjet->GetValue();
        Button_UndoZ->Disable();
        xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].getPoint();
        xyz_sommet[2] = Old_Z;
        Element->Objetlist[IndiceObjet].Sommetlist[IndicePoint].setPoint(xyz_sommet);
        str.Printf(_T("%f"),Old_Z);
        Text_ValeurZ->SetValue(str);
        Element->SetPointStar(xyz_sommet);
        Element->m_gllist   = 0;
        Element->Refresh();
    }
}
