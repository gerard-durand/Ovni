#include "ReperageFacette.h"

//(*InternalHeaders(ReperageFacette)
#include <wx/font.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ReperageFacette)
const long ReperageFacette::ID_STATICTEXT1 = wxNewId();
const long ReperageFacette::ID_STATICTEXT2 = wxNewId();
const long ReperageFacette::ID_SPINCTRL1 = wxNewId();
const long ReperageFacette::ID_SPINCTRL2 = wxNewId();
const long ReperageFacette::ID_TEXTCTRL1 = wxNewId();
const long ReperageFacette::ID_STATICTEXT3 = wxNewId();
const long ReperageFacette::ID_TEXTCTRL2 = wxNewId();
const long ReperageFacette::ID_STATICTEXT4 = wxNewId();
const long ReperageFacette::ID_TEXTCTRL3 = wxNewId();
const long ReperageFacette::ID_CHECKBOX4 = wxNewId();
const long ReperageFacette::ID_SPINCTRL3 = wxNewId();
const long ReperageFacette::ID_TEXTCTRL4 = wxNewId();
const long ReperageFacette::ID_CHECKBOX1 = wxNewId();
const long ReperageFacette::ID_CHECKBOX2 = wxNewId();
const long ReperageFacette::ID_BUTTON1 = wxNewId();
const long ReperageFacette::ID_BUTTON3 = wxNewId();
const long ReperageFacette::ID_BUTTON4 = wxNewId();
const long ReperageFacette::ID_BUTTON5 = wxNewId();
const long ReperageFacette::ID_BUTTON6 = wxNewId();
const long ReperageFacette::ID_BUTTON7 = wxNewId();
const long ReperageFacette::ID_CHECKBOX3 = wxNewId();
const long ReperageFacette::ID_BUTTON8 = wxNewId();
const long ReperageFacette::ID_BUTTON9 = wxNewId();
const long ReperageFacette::ID_STATICTEXT5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ReperageFacette,wxDialog)
	//(*EventTable(ReperageFacette)
	//*)
END_EVENT_TABLE()

ReperageFacette::ReperageFacette(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ReperageFacette)
	Create(parent, id, _T("Repérage d\'une facette"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(312,395));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Indice de la facette :"), wxPoint(8,59), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Indice/Numéro de l\'objet :"), wxPoint(8,11), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	SpinCtrl_IndiceFacette = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(128,56), wxSize(98,21), 0, -2, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_IndiceFacette->SetValue(_T("0"));
	SpinCtrl_IndiceObjet = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(160,8), wxSize(80,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_IndiceObjet->SetValue(_T("0"));
	Text_NumeroObjet = new wxTextCtrl(this, ID_TEXTCTRL1, _T("nnn"), wxPoint(240,8), wxSize(64,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Text_NumeroObjet->Disable();
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Nom :"), wxPoint(8,35), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	Text_NomObjet = new wxTextCtrl(this, ID_TEXTCTRL2, _T("Texte"), wxPoint(48,32), wxSize(256,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	Text_NomObjet->Disable();
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Nombre de sommets :"), wxPoint(8,91), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	Text_NbSommets = new wxTextCtrl(this, ID_TEXTCTRL3, _T("nnn"), wxPoint(144,88), wxSize(160,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	Text_NbSommets->Disable();
	CheckBox_VisuSommets = new wxCheckBox(this, ID_CHECKBOX4, _T("Visualisation des sommets de la facette"), wxPoint(24,116), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBox_VisuSommets->SetValue(false);
	SpinCtrl_IndiceSommet = new wxSpinCtrl(this, ID_SPINCTRL3, _T("0"), wxPoint(40,136), wxSize(122,21), 0, -1, 100, 0, _T("ID_SPINCTRL3"));
	SpinCtrl_IndiceSommet->SetValue(_T("0"));
	SpinCtrl_IndiceSommet->Disable();
	TextCtrl_NumeroSommet = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxPoint(176,136), wxSize(128,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	TextCtrl_NumeroSommet->Disable();
	CheckBox_VisuNormale = new wxCheckBox(this, ID_CHECKBOX1, _T("Visualisation de la normale à la facette"), wxPoint(24,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_VisuNormale->SetValue(false);
	CheckBox_VisuNormales_Sommets = new wxCheckBox(this, ID_CHECKBOX2, _T("Visualisation des normales aux sommets"), wxPoint(24,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_VisuNormales_Sommets->SetValue(false);
	Button_InvNormale = new wxButton(this, ID_BUTTON1, _T("Inverser la normale"), wxPoint(24,216), wxSize(264,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_InvNormale->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_DelFacette = new wxButton(this, ID_BUTTON3, _T("Supprimer la facette"), wxPoint(24,240), wxSize(152,23), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_DelFacette->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_UndoDel = new wxButton(this, ID_BUTTON4, _T("Undo"), wxPoint(184,240), wxSize(104,23), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_UndoDel->Disable();
	Button_UndoDel->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Masquer = new wxButton(this, ID_BUTTON5, _T("Masquer la facette"), wxPoint(24,264), wxSize(264,23), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button_Masquer->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Permuter = new wxButton(this, ID_BUTTON6, _T("Permutation circulaire des sommets"), wxPoint(24,288), wxSize(264,23), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button_Permuter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_CentrerRotation = new wxButton(this, ID_BUTTON7, _T("Centrer la rotation sur la facette"), wxPoint(24,312), wxSize(264,23), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	Button_CentrerRotation->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_Laisser = new wxCheckBox(this, ID_CHECKBOX3, _T("Laisser la facette affichée en quittant"), wxPoint(24,344), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_Laisser->SetValue(false);
	Button_Reset = new wxButton(this, ID_BUTTON8, _T("Reset"), wxPoint(24,368), wxSize(104,23), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	Button_Reset->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON9, _T("Quitter"), wxPoint(184,368), wxSize(104,23), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText_Warning = new wxStaticText(this, ID_STATICTEXT5, _T("Supprimée"), wxPoint(232,58), wxSize(64,16), 0, _T("ID_STATICTEXT5"));
	StaticText_Warning->Hide();
	StaticText_Warning->SetForegroundColour(wxColour(255,0,0));
	wxFont StaticText_WarningFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	StaticText_Warning->SetFont(StaticText_WarningFont);

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ReperageFacette::OnSpinCtrl_IndiceFacetteChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ReperageFacette::OnSpinCtrl_IndiceObjetChange);
	Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnCheckBox_VisuSommetsClick);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&ReperageFacette::OnSpinCtrl_IndiceSommetChange);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnCheckBox_VisuNormaleClick);
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnCheckBox_VisuNormales_SommetsClick);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_InvNormaleClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_DelFacetteClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_UndoDelClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_MasquerClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_PermuterClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_CentrerRotationClick);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_ResetClick);
	Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageFacette::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ReperageFacette::OnClose);
	//*)
//	this->Facette_Parent=parent;
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ReperageFacette::~ReperageFacette()
{
	//(*Destroy(ReperageFacette)
	//*)
}


void ReperageFacette::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ReperageFacette::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();

    if (CheckBox_Laisser->IsChecked() && (IndiceFacette >= 0))
        Element->Facette_Surlignee = true;
    else
        Element->Facette_Surlignee = false;
    Element->Refresh();
    Hide();
}

void ReperageFacette::ButtonsEnable()
{
// Activer les boutons
    Button_InvNormale->Enable();
    Button_DelFacette->Enable();
//    Button_UndoDel ->Enable();    // Ne pas activer ici !
    Button_Masquer ->Enable();
    Button_Permuter->Enable();
    Button_CentrerRotation->Enable();
    CheckBox_VisuSommets ->Enable();
    if (CheckBox_VisuSommets->IsChecked())
        SpinCtrl_IndiceSommet->Enable();
    else
        SpinCtrl_IndiceSommet->Disable();
}

void ReperageFacette::ButtonsDisable()
{
// Désactiver tous les boutons
    Button_InvNormale->Disable();
    Button_DelFacette->Disable();
    Button_UndoDel   ->Disable();
    Button_Masquer   ->Disable();
    Button_Permuter  ->Disable();
    Button_CentrerRotation->Disable();
    CheckBox_VisuSommets  ->Disable();
    SpinCtrl_IndiceSommet ->Disable();
}

void ReperageFacette::OnSpinCtrl_IndiceObjetChange(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;
    wxString str_nom, str_num, str;

    int IndiceObjet = SpinCtrl_IndiceObjet->GetValue();
    SpinCtrl_IndiceFacette->SetMax(Element->Objetlist[IndiceObjet].Facelist.size());
    int num_obj = Element->Objetlist[IndiceObjet].GetValue();
    str_num.Printf(_T("%d"),num_obj);
    Text_NumeroObjet->SetValue(str_num);
//    str_nom = Element->Objetlist[IndiceObjet].GetwxName();
    str_nom =  wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    Text_NomObjet->SetValue(str_nom);
    Element->Facette_Surlignee = false;
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette == -2) {
        IndiceFacette = SpinCtrl_IndiceFacette->GetMax() -1;        // si -2, on boucle sur l'indice max, soit Facelist.size() -1
        SpinCtrl_IndiceFacette->SetValue(IndiceFacette);
    } else if (IndiceFacette == SpinCtrl_IndiceFacette->GetMax()) {
        IndiceFacette = -1;                                         // si indice trop grand, boucler sur -1 (pas de facette sélectionnée)
        SpinCtrl_IndiceFacette->SetValue(IndiceFacette);
    }
    StaticText_Warning->Hide();
    if (IndiceFacette >= 0) {
        ButtonsEnable();
        if (!Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].afficher) {
            StaticText_Warning->SetLabel(_T("Masquée"));
            StaticText_Warning->Show();
        }
        if (Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].deleted) {
            Button_UndoDel->Enable();                                       // Facette supprimée => pouvoir la restaurer
            StaticText_Warning->SetLabel(_T("Supprimée"));
            StaticText_Warning->Show();
        } else
            Button_UndoDel->Disable();
        int Nb_points = Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].Nb_Sommets_F;
        SpinCtrl_IndiceSommet->SetMax(Nb_points);                           // Mettre 1 de plus pour permettre le bouclage
        str.Printf(_T("%d"),Nb_points);
        Element->Facette_Surlignee        = true;
        Element->Numero_Objet_a_Surligner = IndiceObjet;
        Element->Numero_Facette_Surlignee = IndiceFacette;
        if (Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].afficher == true)
            Button_Masquer->SetLabel(_T("Masquer la facette"));
        else
            Button_Masquer->SetLabel(_T("Réafficher la facette"));
        wxSpinEvent event_spin(wxEVT_COMMAND_SPINCTRL_UPDATED);             // Simuler un changement SpinEvent de indiceSommet
        OnSpinCtrl_IndiceSommetChange(event_spin);
    } else {
        str.Printf(_T("")) ;
        ButtonsDisable();
        TextCtrl_NumeroSommet->SetValue(str);
    }
    Text_NbSommets->SetValue(str);

    if (MAIN->Selections_Panel->IsShown()) {                                // Mise à jour de SelectionPanel s'il est affiché
        MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(str_nom);
        if (IndiceObjet < 0) str_num.Printf(_T(""));                        // Laisser vide
        MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(str_num);       // ici c'est le numéro

        str.Printf(_T("%d"),IndiceObjet);
        if (IndiceObjet < 0) str.Printf(_T(""));
        MAIN->Selections_Panel->TextCtrl_IndObjet->SetValue(str);           // et là l'indice de l'objet

        IndiceFacette = SpinCtrl_IndiceFacette->GetValue();                 // Pas forcément utile, c'est déjà le cas !
        str.Printf(_T("%d"),IndiceFacette);
        if (IndiceFacette < 0) str.Printf(_T(""));
        MAIN->Selections_Panel->TextCtrl_IndFacette->SetValue(str);
        str.Printf(_T("%d"),IndiceFacette+1);
        if (IndiceFacette < 0) str.Printf(_T(""));
        MAIN->Selections_Panel->TextCtrl_NumFacette->SetValue(str);

        MAIN->Selections_Panel->TextCtrl_Selection->SetValue(_T(""));       // Vider le nombre d'élements sélectionnés (objets, facettes, points)

        if (IndiceFacette >= 0)
            str.Printf(_T("%d"),Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].groupe);
        else
            str.Printf(_T(""));
        MAIN->Selections_Panel->TextCtrl_NumGroupe->SetValue(str);
        if (IndiceFacette >= 0)
            str.Printf(_T("%d"),Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].codmatface);
        MAIN->Selections_Panel->TextCtrl_NumMateriau->SetValue(str);
    }

    Element->Refresh();
}

void ReperageFacette::OnSpinCtrl_IndiceFacetteChange(wxSpinEvent& event)
{
    ReperageFacette::OnSpinCtrl_IndiceObjetChange(event);
//    // On peut faire exactement le même traitement pour OnSpinCtrl_NumeroFacetteChange et OnSpinCtrl_IndiceObjetChange

}

void ReperageFacette::OnCheckBox_VisuSommetsClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int IndiceObjet   = SpinCtrl_IndiceObjet  ->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        if (CheckBox_VisuSommets->IsChecked()) {
            SpinCtrl_IndiceSommet->Enable();
            SpinCtrl_IndiceSommet->SetMax(wxAtoi(Text_NbSommets->GetValue()));  // Mettre un de plus pour pouvoir boucler si max atteind !
            wxSpinEvent event_spin(wxEVT_COMMAND_SPINCTRL_UPDATED);             // Simuler un changement SpinEvent de indiceSommet
            OnSpinCtrl_IndiceSommetChange(event_spin);
            Element->Sommets_Surlignes = true;
            Element->Numero_Sommet_a_Surligner = wxAtoi(TextCtrl_NumeroSommet->GetValue());
        } else {
            SpinCtrl_IndiceSommet->Disable();
            Element->Sommets_Surlignes = false;
        }
    } else {
        SpinCtrl_IndiceSommet->Disable();
        Element->Sommets_Surlignes = false;
    }
    Element->Refresh();
}

void ReperageFacette::OnSpinCtrl_IndiceSommetChange(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;
    std::vector<int> NumerosSommets;
    wxString str;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        NumerosSommets = Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].getF_sommets();
        if (SpinCtrl_IndiceSommet->GetValue()  ==  SpinCtrl_IndiceSommet->GetMax()) SpinCtrl_IndiceSommet->SetValue(0) ; // Pour boucler
        if (SpinCtrl_IndiceSommet->GetValue() < 0) SpinCtrl_IndiceSommet->SetValue( SpinCtrl_IndiceSommet->GetMax() -1); // Idem
        int NumeroCourant = NumerosSommets[SpinCtrl_IndiceSommet->GetValue()];
        str.Printf(_T("%d"),NumeroCourant);
        TextCtrl_NumeroSommet->SetValue(str);
        Element->Numero_Sommet_a_Surligner = wxAtoi(TextCtrl_NumeroSommet->GetValue());
    } else TextCtrl_NumeroSommet->SetValue(_T(""));

    Element->Refresh();
}

void ReperageFacette::OnButton_CentrerRotationClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    std::vector<int>   NumerosSommets;
    std::vector<float> xyz_sommet;
    float centre_facette[3]={0.,0.,0.}; // On pourrait peut-être en faire un vector, mais il faudrait que les opérations + - / += ... soient OK
    int i,k ;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        NumerosSommets = Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].getF_sommets();
//        centre_facette[0]=centre_facette[1]=centre_facette[2]=0. ;
        int ns = NumerosSommets.size();
        // Sommer les 3 composantes de tous les ns sommets de la facette
        for(k=0; k<ns; k++) {
            xyz_sommet = Element->Objetlist[IndiceObjet].Sommetlist[NumerosSommets[k]-1].getPoint();
            for (i=0; i<3; i++) centre_facette[i] += xyz_sommet[i];
        }
        Element->centrageRotAuto = false;
        // Imposer le point central de la facette comme nouveau centre de rotation
        for (i=0; i<3; i++) Element->centreRot[i] = centre_facette[i]/ns;
        Element->SetPosObs(false);    // Reset de la position observée (centreRot) mais sans changer le zoom
        Element->Refresh();
    }
}

void ReperageFacette::OnButton_MasquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        if (Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].afficher) {
            Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].afficher=false;
            Element->Elements_Masques = true;                 // Indique au niveau global qu'une facette a été masquée
            Button_Masquer->SetLabel(_T("Réafficher la facette"));  // Changer l'intitulé du bouton
        } else {
            Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].afficher=true;
            Button_Masquer->SetLabel(_T("Masquer la facette"));     // Ne pas changer Elements_Masques car on ne travaille une seule facette
        }
        wxSpinEvent cmd_spin;
        OnSpinCtrl_IndiceObjetChange(cmd_spin); // Simule un spin button pour rafraichir l'interface
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageFacette::OnButton_ResetClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    wxString str;

    Element->centrageRotAuto = true;
    Element->centreRot = Element->centre_auto;
    Element->SetPosObs(false);

// Init Objet
    SpinCtrl_IndiceObjet->SetMax(Element->Objetlist.size()-1);
    int IndiceObjet = 0;
    SpinCtrl_IndiceFacette->SetMax(Element->Objetlist[IndiceObjet].Facelist.size() -1);
    SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    int num_obj = Element->Objetlist[IndiceObjet].GetValue();
    str.Printf(_T("%d"),num_obj);
    Text_NumeroObjet->SetValue(str);
    str = wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    Text_NomObjet->SetValue(str);

// Init facette
    SpinCtrl_IndiceFacette->SetValue(-1);
    str.Printf(_T("")) ;
    Text_NbSommets->SetValue(str);
    Element->Facette_Surlignee = false;
    Element->Sommets_Surlignes = false;
    Button_Masquer->SetLabel(_T("Masquer la facette"));
    ButtonsDisable();

    CheckBox_VisuNormale->SetValue(false);
    MAIN->Button_Normale_Barycentre->SetValue(false);
    CheckBox_VisuNormales_Sommets  ->SetValue(false);
    MAIN->Button_Normales_Sommets  ->SetValue(false);

    CheckBox_VisuSommets->SetValue(false);
    CheckBox_Laisser    ->SetValue(false);

    StaticText_Warning->Hide();

    Element->Refresh();
}

void ReperageFacette::OnButton_DelFacetteClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].deleted=true;
        Button_UndoDel->Enable();
        Element->m_gllist = 0;
        wxSpinEvent cmd_spin;
        OnSpinCtrl_IndiceObjetChange(cmd_spin); // Simule un spin button pour rafraichir l'interface
        Element->Elements_Supprimes = true;     // Indique au niveau global qu'une facette a été supprimée
        Element->Refresh();
    }
}

void ReperageFacette::OnButton_UndoDelClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        Element->Objetlist[IndiceObjet].Facelist[IndiceFacette].deleted=false; // Ne pas changer Elements_Supprimes car on ne travaille que sur 1 seule facette
        Button_UndoDel->Disable();
        wxSpinEvent cmd_spin;
        OnSpinCtrl_IndiceObjetChange(cmd_spin); // Simule un spin button pour rafraichir l'interface
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageFacette::OnButton_InvNormaleClick(wxCommandEvent& event)
{
// Ici (pour l'instant !), on inverse la normale à la facette, mais on ne recalcule pas l'influence sur les normales aux sommets !

    BddInter *Element = MAIN->Element;
    Object * objet_courant;
    std::vector<float> NormaleFacette;
    std::vector<int>   NumerosSommets;
    std::vector<int>   ReverseSommets;
    int i;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    objet_courant = &(Element->Objetlist[IndiceObjet]);
    if (IndiceFacette >= 0) {
        NormaleFacette = objet_courant->Facelist[IndiceFacette].getNormale_b();      // Récupérer la normale
        for (i=0; i<3; i++) NormaleFacette[i] *= -1;                                                        // Inverser les 3 composantes
        objet_courant->Facelist[IndiceFacette].setNormale_b(NormaleFacette);         // Remettre la normale inversée en place
        // Inverser le sens de parcours des sommets de la facette
        NumerosSommets = objet_courant->Facelist[IndiceFacette].getF_sommets();
        ReverseSommets.clear();
        for (auto it=NumerosSommets.crbegin(); it != NumerosSommets.crend(); ++it) ReverseSommets.push_back(*it);
        objet_courant->Facelist[IndiceFacette].setFsommet(ReverseSommets);
        // Idem pour normales aux sommets
        if (objet_courant->Facelist[IndiceFacette].L_sommets.size() != 0) {  // doit être la même chose !!!
            NumerosSommets = objet_courant->Facelist[IndiceFacette].getL_sommets();
            ReverseSommets.clear();
            for (auto it=NumerosSommets.crbegin(); it != NumerosSommets.crend(); ++it) ReverseSommets.push_back(*it);
            objet_courant->Facelist[IndiceFacette].setLsommet(ReverseSommets);
        }
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageFacette::OnButton_PermuterClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    Object * objet_courant;
    std::vector<int> NumerosSommets;
    int last;

    int IndiceObjet   = SpinCtrl_IndiceObjet->GetValue();
    objet_courant     = &(Element->Objetlist[IndiceObjet]);
    int IndiceFacette = SpinCtrl_IndiceFacette->GetValue();
    if (IndiceFacette >= 0) {
        NumerosSommets = objet_courant->Facelist[IndiceFacette].getF_sommets();
        last = NumerosSommets.back();                       // Récupérer le dernier élément
        NumerosSommets.pop_back();                          // Le supprimer du vecteur
        NumerosSommets.insert(NumerosSommets.begin(),last); // Le dernier devient le premier !
        objet_courant->Facelist[IndiceFacette].setFsommet(NumerosSommets);
        // Permuter aussi les numéros des normales aux sommets (seulement s'ils existent), pas forcément les mêmes numéros que pour les facettes
        if (objet_courant->Facelist[IndiceFacette].L_sommets.size() != 0) {
            NumerosSommets = objet_courant->Facelist[IndiceFacette].getL_sommets();  // En toute rigueur NumerosVecteurs
            last = NumerosSommets.back();
            NumerosSommets.pop_back();
            NumerosSommets.insert(NumerosSommets.begin(),last);
            objet_courant->Facelist[IndiceFacette].setLsommet(NumerosSommets);
        }
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageFacette::OnCheckBox_VisuNormaleClick(wxCommandEvent& event)
{
    MAIN->Button_Normale_Barycentre->SetValue(CheckBox_VisuNormale->GetValue());        // Synchoniser le bouton avec la checkbox
    MAIN->Element->AfficherNormaleFacette = CheckBox_VisuNormale->GetValue();
    MAIN->Element->Refresh();
}

void ReperageFacette::OnCheckBox_VisuNormales_SommetsClick(wxCommandEvent& event)
{
    MAIN->Button_Normales_Sommets->SetValue(CheckBox_VisuNormales_Sommets->GetValue()); // Synchoniser le bouton avec la checkbox
    MAIN->Element->AfficherNormalesSommets = CheckBox_VisuNormales_Sommets->GetValue();
    MAIN->Element->Refresh();
}
