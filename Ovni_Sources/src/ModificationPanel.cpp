#include "ModificationPanel.h"

//(*InternalHeaders(ModificationPanel)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ModificationPanel)
const wxWindowID ModificationPanel::ID_STATICTEXT1 = wxNewId();
const wxWindowID ModificationPanel::ID_TOGGLEBUTTON1 = wxNewId();
const wxWindowID ModificationPanel::ID_SPINCTRL1 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT2 = wxNewId();
const wxWindowID ModificationPanel::ID_TOGGLEBUTTON2 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL2 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT3 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT4 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT5 = wxNewId();
const wxWindowID ModificationPanel::ID_SPINCTRL2 = wxNewId();
const wxWindowID ModificationPanel::ID_SPINCTRL3 = wxNewId();
const wxWindowID ModificationPanel::ID_TOGGLEBUTTON4 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON4 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON5 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON6 = wxNewId();
const wxWindowID ModificationPanel::ID_CHECKBOX2 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL4 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT6 = wxNewId();
const wxWindowID ModificationPanel::ID_TOGGLEBUTTON3 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON8 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICLINE1 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL6 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT7 = wxNewId();
const wxWindowID ModificationPanel::ID_STATICTEXT8 = wxNewId();
const wxWindowID ModificationPanel::ID_TEXTCTRL1 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON9 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON10 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON11 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON12 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON1 = wxNewId();
const wxWindowID ModificationPanel::ID_CHECKBOX1 = wxNewId();
const wxWindowID ModificationPanel::ID_CHECKBOX3 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL8 = wxNewId();
const wxWindowID ModificationPanel::ID_BUTTON13 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL9 = wxNewId();
const wxWindowID ModificationPanel::ID_PANEL10 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ModificationPanel,wxDialog)
	//(*EventTable(ModificationPanel)
	//*)
END_EVENT_TABLE()

ModificationPanel::ModificationPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ModificationPanel)
	Create(parent, id, _T("Modifications"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(243,591));
	Move(wxPoint(20,20));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Ajout de points dans une arête"), wxPoint(0,0), wxSize(240,20), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText1->SetForegroundColour(wxColour(255,255,255));
	StaticText1->SetBackgroundColour(wxColour(0,0,0));
	Panel2 = new wxPanel(this, ID_PANEL2, wxPoint(0,20), wxSize(240,64), wxTRANSPARENT_WINDOW|wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	ToggleButton_Ajouter = new wxToggleButton(Panel2, ID_TOGGLEBUTTON1, _T("Ajouter sous le pointeur"), wxPoint(32,4), wxSize(176,24), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));
	ToggleButton_Ajouter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	SpinCtrl_NbSegments = new wxSpinCtrl(Panel2, ID_SPINCTRL1, _T("2"), wxPoint(88,32), wxSize(46,24), 0, 2, 100, 2, _T("ID_SPINCTRL1"));
	SpinCtrl_NbSegments->Disable();
	StaticText2 = new wxStaticText(Panel2, ID_STATICTEXT2, _T("segments"), wxPoint(144,32), wxDLG_UNIT(Panel2,wxSize(32,9)), wxALIGN_CENTRE|wxFULL_REPAINT_ON_RESIZE, _T("ID_STATICTEXT2"));
	ToggleButton_Diviser = new wxToggleButton(Panel2, ID_TOGGLEBUTTON2, _T("Diviser en"), wxPoint(8,32), wxSize(72,24), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON2"));
	ToggleButton_Diviser->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Création de facettes"), wxPoint(0,162), wxSize(240,18), wxALIGN_CENTRE, _T("ID_STATICTEXT3"));
	StaticText3->SetForegroundColour(wxColour(255,255,255));
	StaticText3->SetBackgroundColour(wxColour(0,0,0));
	Panel4 = new wxPanel(this, ID_PANEL4, wxPoint(0,180), wxSize(240,144), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	StaticText4 = new wxStaticText(Panel4, ID_STATICTEXT4, _T("Avec le numéro de groupe"), wxPoint(8,2), wxSize(144,16), 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(Panel4, ID_STATICTEXT5, _T("Avec le numéro de matériau"), wxPoint(8,22), wxSize(152,16), 0, _T("ID_STATICTEXT5"));
	SpinCtrl_Groupe = new wxSpinCtrl(Panel4, ID_SPINCTRL2, _T("0"), wxPoint(168,0), wxSize(46,20), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau = new wxSpinCtrl(Panel4, ID_SPINCTRL3, _T("0"), wxPoint(168,20), wxSize(46,20), 0, 0, 15, 0, _T("ID_SPINCTRL3"));
	ToggleButton_CreerFacette = new wxToggleButton(Panel4, ID_TOGGLEBUTTON4, _T("Créer"), wxPoint(16,40), wxSize(88,24), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON4"));
	ToggleButton_CreerFacette->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(Panel4, ID_BUTTON4, _T("Annuler la création en cours"), wxPoint(16,68), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_Annuler->Disable();
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_SupprimerFacette = new wxButton(Panel4, ID_BUTTON5, _T("Supprimer les facettes créées"), wxPoint(16,92), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button_SupprimerFacette->Disable();
	Button_SupprimerFacette->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_InverserNormale = new wxButton(Panel4, ID_BUTTON6, _T("Inverser les normales"), wxPoint(16,116), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button_InverserNormale->Disable();
	Button_InverserNormale->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_FacettePlane = new wxCheckBox(Panel4, ID_CHECKBOX2, _T("Plane"), wxPoint(120,44), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_FacettePlane->SetValue(false);
	CheckBox_FacettePlane->Disable();
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Souder deux points"), wxPoint(0,82), wxSize(240,20), wxALIGN_CENTRE, _T("ID_STATICTEXT6"));
	StaticText6->SetForegroundColour(wxColour(255,255,255));
	StaticText6->SetBackgroundColour(wxColour(0,0,0));
	Panel6 = new wxPanel(this, ID_PANEL6, wxPoint(0,102), wxSize(240,64), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	ToggleButton_Souder = new wxToggleButton(Panel6, ID_TOGGLEBUTTON3, _T("Souder"), wxPoint(64,4), wxSize(120,24), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON3"));
	ToggleButton_Souder->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Undo = new wxButton(Panel6, ID_BUTTON8, _T("Undo"), wxPoint(16,32), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	Button_Undo->Disable();
	Button_Undo->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticLine1 = new wxStaticLine(Panel6, ID_STATICLINE1, wxPoint(0,32), wxSize(240,0), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Divers"), wxPoint(0,322), wxSize(240,18), wxALIGN_CENTRE, _T("ID_STATICTEXT7"));
	StaticText7->SetForegroundColour(wxColour(255,255,255));
	StaticText7->SetBackgroundColour(wxColour(0,0,0));
	Panel8 = new wxPanel(this, ID_PANEL8, wxPoint(0,340), wxSize(240,208), wxTAB_TRAVERSAL, _T("ID_PANEL8"));
	StaticText8 = new wxStaticText(Panel8, ID_STATICTEXT8, _T("Tolérance d\'égalité %"), wxPoint(16,8), wxSize(128,16), 0, _T("ID_STATICTEXT8"));
	TextCtrl_Tolerance = new wxTextCtrl(Panel8, ID_TEXTCTRL1, _T("0"), wxPoint(152,8), wxSize(72,20), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_InverserTout = new wxButton(Panel8, ID_BUTTON9, _T("Inverser toutes les normales"), wxPoint(16,32), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	Button_InverserTout->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Trianguler = new wxButton(Panel8, ID_BUTTON10, _T("Trianguler la BDD"), wxPoint(16,56), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON10"));
	Button_Trianguler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_RecalculerNormales = new wxButton(Panel8, ID_BUTTON11, _T("Recalculer toutes les normales"), wxPoint(16,104), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON11"));
	Button_RecalculerNormales->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Simplification = new wxButton(Panel8, ID_BUTTON12, _T("Simplification de la BDD"), wxPoint(16,128), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON12"));
	Button_Simplification->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_RecalculerAretes = new wxButton(Panel8, ID_BUTTON1, _T("Recalculer les arêtes"), wxPoint(16,152), wxSize(208,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_RecalculerAretes->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_Transparence = new wxCheckBox(Panel8, ID_CHECKBOX1, _T("Activer la transparence"), wxPoint(16,182), wxSize(176,16), 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_Transparence->SetValue(false);
	CheckBox_NotFlat = new wxCheckBox(Panel8, ID_CHECKBOX3, _T("Forcer facettes à NON planes"), wxPoint(16,84), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_NotFlat->SetValue(false);
	Panel10 = new wxPanel(this, ID_PANEL10, wxPoint(0,544), wxSize(240,48), wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	Button_Quitter = new wxButton(Panel10, ID_BUTTON13, _T("Quitter"), wxPoint(80,16), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON13"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Panel9 = new wxPanel(Panel10, ID_PANEL9, wxPoint(0,0), wxSize(240,8), wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	Panel9->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));

	Connect(ID_TOGGLEBUTTON1, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnToggleButton_AjouterToggle);
	Connect(ID_SPINCTRL1, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&ModificationPanel::OnSpinCtrl_NbSegmentsChange);
	Connect(ID_TOGGLEBUTTON2, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnToggleButton_DiviserToggle);
	Connect(ID_SPINCTRL2, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&ModificationPanel::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL3, wxEVT_COMMAND_SPINCTRL_UPDATED, (wxObjectEventFunction)&ModificationPanel::OnSpinCtrl_MateriauChange);
	Connect(ID_TOGGLEBUTTON4, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnToggleButton_CreerFacetteToggle);
	Connect(ID_BUTTON4, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_AnnulerClick);
	Connect(ID_BUTTON5, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_SupprimerFacetteClick);
	Connect(ID_BUTTON6, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_InverserNormaleClick);
	Connect(ID_CHECKBOX2, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnCheckBox_FacettePlaneClick);
	Connect(ID_TOGGLEBUTTON3, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnToggleButton_SouderToggle);
	Connect(ID_BUTTON8, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_UndoClick);
	Connect(ID_TEXTCTRL1, wxEVT_COMMAND_TEXT_UPDATED, (wxObjectEventFunction)&ModificationPanel::OnTextCtrl_ToleranceText);
	Connect(ID_BUTTON9, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_InverserToutClick);
	Connect(ID_BUTTON10, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_TriangulerClick);
	Connect(ID_BUTTON11, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_RecalculerNormalesClick);
	Connect(ID_BUTTON12, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_SimplificationClick);
	Connect(ID_BUTTON1, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_RecalculerAretesClick);
	Connect(ID_CHECKBOX1, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnCheckBox_TransparenceClick);
	Connect(ID_CHECKBOX3, wxEVT_COMMAND_CHECKBOX_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnCheckBox_NotFlatClick);
	Connect(ID_BUTTON13, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ModificationPanel::OnButton_QuitterClick);
	Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction)&ModificationPanel::OnInit);
	Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, (wxObjectEventFunction)&ModificationPanel::OnClose);
	//*)

//	this->Mod_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ModificationPanel::~ModificationPanel()
{
	//(*Destroy(ModificationPanel)
	//*)
}

void ModificationPanel::OnInit(wxInitDialogEvent& event)
{
    unsigned int o,i;
    BddInter *Element  = MAIN->Element;
    Object   *objet_courant;

    Element->SetDetectionSurvolPoint(false);        // Par précaution car c'est déjà le cas normalement
    Element->SetDetectionSurvolArete(false);
    old_modeGL         = (int)Element->modeGL;      // Pour pouvoir restituer l'état initial en quittant ModificationPanel
    old_mode_selection = Element->mode_selection;
    old_show_points    = Element->GetShowPoints();
    old_show_lines     = Element->GetShowLines();
    CheckBox_NotFlat->SetValue(Element->GetNotFlat());
    aretes_calculees   = true;  // Si le flag n'a pas été changé, c'est donc qu'il n'est pas utile de les recalculer
//    printf("mode_selection : %d %d\n",old_mode_selection,MAIN->Element->mode_selection);

// RAZ de l'attribut "selected" des facettes et sommets en entrée
    for (o=0; o<Element->Objetlist.size(); o++) {
        objet_courant = &(Element->Objetlist[o]);
        for (i=0; i<objet_courant->Facelist.size()  ; i++) objet_courant->Facelist[i].selected  = false;
        for (i=0; i<objet_courant->Sommetlist.size(); i++) objet_courant->Sommetlist[i].selected= false;
    }
}

void ModificationPanel::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ModificationPanel::OnClose(wxCloseEvent& event)
{
//    printf("ModificationPanel: Close\n");
    BddInter *Element = MAIN->Element;
    unsigned  int o,i;

    MAIN->SetToggleModifs(false);
    MAIN->Button_Modifs->SetValue(false);
    if (ToggleButton_CreerFacette->GetValue()) {    // Le bouton Créer une facette est toujours enfoncé !
        wxCommandEvent cmd_event;
        OnButton_AnnulerClick(cmd_event);           // Simuler un clic sur "Annuler" pour forcer à vider la sélection (au cas où !)
    }
    if (ToggleButton_Ajouter->GetValue() || ToggleButton_Diviser->GetValue() ||
        ToggleButton_Souder ->GetValue() || ToggleButton_CreerFacette->GetValue() ) {
        ToggleButton_Ajouter->SetValue(false);
        ToggleButton_Diviser->SetValue(false);
        ToggleButton_Souder ->SetValue(false);
        ToggleButton_CreerFacette->SetValue(false);
        CheckBox_FacettePlane->Disable();   // Mais on garde son état coché/décoché
        Button_Annuler->Disable();
        Button_InverserNormale->Disable();
        Bool_diviser = false;
        Bool_souder  = false;

        Element->SetShowLines (false);
        Element->SetShowPoints(false);
        Element->Smemory     = nullptr;
    }
    Element->modeGL          = (BddInter::MODE)old_modeGL;
    Element->mode_selection  = (BddInter::SELECTION)old_mode_selection;
    Element->SetCreationFacette(false);
    Element->SetDetectionSurvolPoint(false);
    Element->SetDetectionSurvolArete(false);

    Element->SetTolerance(wxAtof(TextCtrl_Tolerance->GetValue()));
    if (old_show_points) Element->SetShowPoints(true);                   // Dans ce cas, forcer à true
    if (old_show_lines)  Element->SetShowLines (true);                   // idem
//    printf("mode_selection : %d %d\n",old_mode_selection,Element->mode_selection);

// RAZ l'attribut "selected" des facettes en sortie (par précaution)
    for (o=0; o<Element->Objetlist.size(); o++) {
        for (i=0; i<Element->Objetlist[o].Facelist.size(); i++) Element->Objetlist[o].Facelist[i].selected = false;
    }
    Hide();
    // Si show_lines est à true (mais pas seulement !), il faudrait peut-être actualiser Genere_Tableau_Points_Facettes et Genere_Tableau_Aretes
    // Mais cette opération est longue sur certaines grosses Bdd. Peut-être à proposer en option via le bouton "Recalculer les arêtes".
    Element->Refresh();

    if (!aretes_calculees) {
        char Message[128];
        sprintf(Message,"===> ATTENTION : Les arêtes n'ont pas été recalculées\n");
        printf("%s",utf8_To_ibm(Message));
    }
}

//souder
void ModificationPanel::OnToggleButton_SouderToggle(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Bool_souder = ToggleButton_Souder->GetValue();
    if (Bool_souder) {
        Element->SetDetectionSurvolPoint(true);                             // Activer la détection en jaune du point/sommet survolé
//        printf("Souder YES\n");
        ToggleButton_Ajouter->SetValue(false);
        ToggleButton_Diviser->SetValue(false);
        old_modeGL = (int)Element->modeGL;
        Element->modeGL = Element->points;
        if (Element->GetPointsSelected()) Element->SetListeSommets_OK(false); // Force à regénérer la liste des points/sommets, car un point a été précedemment sélectionné
                                                                        //(pour éviter l'apparition de points en vert d'une éventuelle liste précédente)
    } else {
//        printf("Souder NO\n");
        wxKeyEvent key_event;
        key_event.m_keyCode = 'S';      // Pour forcer à effacer une éventuelle liste de points
        Element->OnKeyDown(key_event);  // Simule une pression sur la touche S au clavier => Reset de la sélection
        delete Element->Smemory;        // Utile ??
        Element->Smemory = nullptr;     // ""
        Element->modeGL  = (BddInter::MODE)old_modeGL;
        key_event.m_keyCode = 'S';      // Pour forcer à effacer une éventuelle liste de facettes
        Element->OnKeyDown(key_event);  // Simule une pression sur la touche S au clavier => Reset de la sélection
        Element->SetDetectionSurvolPoint(false);

    }
    Bool_diviser = false;
    Element->SetShowPoints(Bool_souder);
//    if (Bool_souder && !Element->GetListeSommets_OK() {
    if (!Element->GetListeSommets_OK() || Element->GetPointsSelected()) {
        printf("Re-construction de la liste de sommets\n");
        Element->m_gllist = Element->glliste_points;
    }
    Element->SetShowLines(false);
    Element->Refresh();
}

//ajouter sous le pointeur un point à une arête
void ModificationPanel::OnToggleButton_AjouterToggle(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    division=-1;
    Bool_diviser = ToggleButton_Ajouter->GetValue();
    if (ToggleButton_Ajouter->GetValue()) {
//        printf("Ajouter YES\n");
        Element->SetDetectionSurvolArete(true);     // Activer la détection en vert de l'arête survolée
        ToggleButton_Diviser->SetValue(false);
        ToggleButton_Souder ->SetValue(false);
        SpinCtrl_NbSegments ->Disable();
        Element->modeGL = Element->aretes;
    } else {
//        printf("Ajouter NO\n");
        Element->SetDetectionSurvolArete(false);
        Element->modeGL = (BddInter::MODE)old_modeGL;
    }
    Bool_souder         = false;
    Element->SetShowLines (Bool_diviser);
    Element->SetShowPoints(false);

    if (Bool_diviser && !Element->GetListeAretes_OK()) {
        printf("Construction de la liste d'arêtes\n");
        Element->m_gllist = Element->glliste_lines;
    }
    Element->Refresh();
}

//division d'arrête en N segments d'égale longueur
void ModificationPanel::OnToggleButton_DiviserToggle(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Bool_diviser = ToggleButton_Diviser->GetValue();
    if (Bool_diviser) {
//        printf("Diviser YES\n");
        Element->SetDetectionSurvolArete(true);     // Activer la détection en vert de l'arête survolée
        ToggleButton_Ajouter->SetValue(false);
        ToggleButton_Souder ->SetValue(false);
        SpinCtrl_NbSegments ->Enable();
        Element->modeGL = Element->aretes;
        division = SpinCtrl_NbSegments->GetValue();
    } else {
//        printf("Diviser NO\n");
        Element->SetDetectionSurvolArete(false);
        SpinCtrl_NbSegments->Disable();
        Element->modeGL = (BddInter::MODE)old_modeGL;
    }
    Bool_souder         = false;
    Element->SetShowLines (Bool_diviser);
    Element->SetShowPoints(false);
    Element->m_gllist   = Element->glliste_lines;
    Element->Refresh();
}

void ModificationPanel::OnSpinCtrl_NbSegmentsChange(wxSpinEvent& event)
{
    division = SpinCtrl_NbSegments->GetValue();
//    printf("Division : %d\n",division);
}

//undo
void ModificationPanel::OnButton_UndoClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->UNDO_ONE();
//    Element->modeGL = (BddInter::MODE)old_modeGL;    // Pas ici !!!!
//    delete Element->Smemory;      // Utile ?
//    Element->Smemory = nullptr;   //                 Par contre, peut-être ajouter un reset de sélection pour éviter un point vert restant ?
//    Element->m_gllist = 0; // inutile ? et plutôt glliste_points ou glliste_lines
    Element->Refresh();
}

void ModificationPanel::OnButton_InverserNormaleClick(wxCommandEvent& event)
{
    wxKeyEvent key_event;
    key_event.m_keyCode = 'I';
    MAIN->Element->OnKeyDown(key_event);   // Simule une pression sur la touche i au clavier => inverser les normales des facettes sélectionnées
    // Il faudrait peut-être aussi recalculer les normales aux sommets !
}

void ModificationPanel::OnButton_RecalculerNormalesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Calcul_All_Normales();
    Element->m_gllist = 0;
    Element->Refresh();
}

void ModificationPanel::OnButton_SimplificationClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Simplification_BDD();
    Element->m_gllist = 0;
    Element->Refresh();
}

void ModificationPanel::OnButton_TriangulerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    Object   *objet_courant;
    Face     *facette_courante, *facette_nouvelle;
    unsigned int o,i,j,k;
    unsigned int nb_facettes_loc, nb_sommets_loc, new_numero_facette, new_indice_facette;
    int jj, jj1, jj2, jj3, jj3v;
    unsigned int new_triangles = 0;
    bool parite=false;
    bool facette_plane;
    float xyz[3];
    std::vector<float> xyz_pt;
    std::vector<int>   NumerosSommets;
    std::vector<int>   NumerosVecteurs;
    std::list<int>     IndicesFacettes;

    int methode_triangulation, new_points, new_numero;
    unsigned int nb_new;
    char Message[512];

    bool verbose = Element->GetVerbose();

    NumerosSommets.resize(3);
    NumerosVecteurs.resize(3);
    Element->str.clear();

    methode_triangulation = Element->GetMethodeTriangulation();
    printf("Methode de Triangulation numero %d\n",methode_triangulation+1);
    for (o=0; o<Element->Objetlist.size(); o++) {
        objet_courant = &(Element->Objetlist[o]);
        if (objet_courant->deleted) continue;   // Objet supprimé mais encore en mémoire => passer au suivant
        nb_facettes_loc = objet_courant->Facelist.size();

/* On commence par vérifier s'il y a des facettes a plus de 3 sommets */

        nb_new     = 0;
        new_points = 0;
        new_numero_facette = nb_facettes_loc ;
        IndicesFacettes.clear();
        for (i=0; i<nb_facettes_loc; i++) {
            if (objet_courant->Facelist[i].deleted) continue;       // Facette supprimée ... passer à la suivante
            nb_sommets_loc = objet_courant->Facelist[i].Nb_Sommets_F;
            if (nb_sommets_loc > 3) {
                if (methode_triangulation == 2) {
                    nb_new += nb_sommets_loc-1;
                    new_points++;
                } else {
                    nb_new += (nb_sommets_loc -3);
                }
            }
        }
        if (nb_new > 0) {        ;      // Si oui ...
            new_triangles+=nb_new;      // Nombre total de nouvelles facettes (triangles)
            sprintf(Message,"%4d facettes a creer dans l'objet d'indice %2d : %s\n",nb_new,o,objet_courant->GetName());
            printf("%s",Message);
            if (new_points != 0) {
                sprintf(Message,"%4d points a ajouter dans cet objet\n",new_points);
                printf("%s",Message);
            }

            Element->SetIndiceObjetCourant(o);

            new_numero_facette = nb_facettes_loc;

            for (i=0; i<nb_facettes_loc; i++) {
                nb_sommets_loc = objet_courant->Facelist[i].Nb_Sommets_F;
                if (nb_sommets_loc <= 3) continue ;                 // On ne change rien, c'est déjà un triangle

                if (objet_courant->Facelist[i].Nb_Sommets_L == 0) {
                        facette_plane = true;
                } else  facette_plane = false;

                facette_courante = &(objet_courant->Facelist[i]);
                if (facette_courante->deleted) continue;        // Facette supprimée ... passer à la suivante
                if (verbose) {
                    printf("Facette originale :\nSommets de     %3d :",i+1);         // i+1 = Numéro
                    for (j=0; j<(unsigned int)facette_courante->Nb_Sommets_F; j++) printf(" %3d",facette_courante->F_sommets[j]);
                    printf("\n");
                    printf("Vecteurs de    %3d :",i+1);
                    for (j=0; j<(unsigned int)facette_courante->Nb_Sommets_L; j++) printf(" %3d",facette_courante->L_sommets[j]);
                    printf("\n");
                }
                jj1 = 0;
                jj2 = 1;    // évite un warning en mode Release
                jj3 = 2;
                jj3v= 0;
                IndicesFacettes.push_back(i);   // Facette originale

                if (methode_triangulation == 2) {
                // Créer un nouveau point au centre de la facette
                    std::fill_n(xyz,3,0.0); // <=> xyz[0] = xyz[1] = xyz[2] = 0.0;
                    for (j=0; j<nb_sommets_loc; j++) {
                        jj      = facette_courante->F_sommets[j]-1; // jj : indice = numero-1
                        xyz_pt  = objet_courant->Sommetlist[jj].getPoint();
                        xyz[0] += xyz_pt[0]; xyz[1] += xyz_pt[1]; xyz[2] += xyz_pt[2];
                    }
                    xyz[0] /= nb_sommets_loc; xyz[1] /= nb_sommets_loc; xyz[2] /= nb_sommets_loc;

                    new_numero = objet_courant->Nb_sommets +1;      // Nouveau numéro = ancienne taille +1
//                    for (k=0; k<3; k++) Element->xyz[k] = xyz[k];   // Recopie du xyz local dans le vecteur xyz de Element
                    Element->SetXYZ(xyz[0], xyz[1], xyz[2]);        // Recopie du xyz local dans le vecteur xyz de Element
                    Element->str.clear();
                    Element->N_elements = new_numero;
                    Element->make_1_sommet();                         // Ici on va augmenter la taille de Sommetlist via push_back
                    if (!facette_plane) {
                        new_numero = objet_courant->Nb_vecteurs +1;
                        Element->SetXYZ(0., 0., 1.);                  // Initialiser le vecteur
                        Element->N_elements = new_numero;
                        Element->make_1_vecteur();
                    }
                    nb_new = nb_sommets_loc-1;  // 1 de moins car on réutilise la facette de base (facette_courante)
                    jj3 = objet_courant->Nb_sommets;                        // Indice du nouveau point central
                    if (!facette_plane) jj3v= objet_courant->Nb_vecteurs;   // Indice du vecteur sur le point central

                    if (verbose) {
                        printf("Originale modifiee :\n");
                        printf("Sommets de     %3d :",i+1);  // Numéro de la facette courante qui sera modifiée en place pour n'avoir plus que 3 sommets
                        for (k=0; k<2; k++) printf(" %3d",facette_courante->F_sommets[k]);
                        printf(" %3d\n",jj3);
                        if (!facette_plane) {
                            printf("Vecteurs de    %3d :",i+1);
                            for (k=0; k<2; k++) printf(" %3d",facette_courante->L_sommets[k]);
                            printf(" %3d\n",jj3v);
                        }
                    }

                    // Et pour la normale au barycentre ... cf version TCL, interface.c ligne 5921 et suivantes

                } else {
                    if (verbose) {
                        printf("Originale modifiee :\n");
                        printf("Sommets de     %3d :",i+1);  // Numéro de la facette courante qui sera modifiée en place pour n'avoir plus que 3 sommets
                        for (k=0; k<3; k++) printf(" %3d",facette_courante->F_sommets[k]);
                        printf("\n");
                        if (!facette_plane) {
                            printf("Vecteurs de    %3d :",i+1);
                            for (k=0; k<3; k++) printf(" %3d",facette_courante->L_sommets[k]);
                            printf("\n");
                        }
                    }
                    nb_new = nb_sommets_loc -3;
                    jj1 = 0;
                    jj2 = 1;
                    jj3 = 2;
                    parite = false;
                }
                if (verbose) printf("Nouvelles facettes:\n");
                for (j=0; j<nb_new; j++) {
                    new_numero_facette++;
                    if (methode_triangulation == 0) {       // Méthode la + simple 0,1,2 puis 0,2,3, ... et 0, n-2, n-1
                        jj1 = 0 ;
                        jj2 = j+2 ;
                        jj3 = jj2+1 ;
                    } else if (methode_triangulation == 1) {
                        parite = !parite ;
                        if (parite) {                       // Méthode 0,1,2 puis 0,2,n-1 puis n-1,2,3 puis n-1,3,n-2 ...
                            jj2 = jj3 ;
                            jj3 = jj1-1;
                            if(jj3 < 0) jj3 = nb_sommets_loc-1;
                        } else {
                            jj1 = jj3 ;
                            jj3 = jj2+1 ;
                        }
                    } else { // <=>  if (methode_triangulation == 2)
                        jj1 = j+1 ;
                        jj2 = jj1+1;
                        if(jj2 >= (int)nb_sommets_loc) jj2=0;
                    }
                    NumerosSommets[0]  = facette_courante->F_sommets[jj1];
                    NumerosSommets[1]  = facette_courante->F_sommets[jj2];
                    if (!facette_plane) {
                        NumerosVecteurs[0] = facette_courante->L_sommets[jj1];
                        NumerosVecteurs[1] = facette_courante->L_sommets[jj2];
                    }
                    if (methode_triangulation != 2) {
                        NumerosSommets[2]  = facette_courante->F_sommets[jj3];
                        if (!facette_plane) NumerosVecteurs[2] = facette_courante->L_sommets[jj3];
                    } else {
                        NumerosSommets[2] = jj3;
                        if (!facette_plane) NumerosVecteurs[2]= jj3v;
                    }
                    if (verbose) {
                        printf("New sommets  : %3d :",new_numero_facette);
                        for (k=0; k<3; k++) printf(" %3d",NumerosSommets[k]);
                        printf("\n");
                        if (!facette_plane) {
                            printf("New vecteurs : %3d :",new_numero_facette);
                            for (k=0; k<3; k++) printf(" %3d",NumerosVecteurs[k]);
                            printf("\n");
                        }
                    }
                    // Création de la nouvelle facette
                    new_indice_facette      = new_numero_facette-1;
                    Element->N_elements     = new_numero_facette;
                    Element->NumerosSommets = NumerosSommets;
                    Element->make_1_face();
                    if (!facette_plane) {
                        Element->NumerosSommets = NumerosVecteurs;
                        Element->make_1_luminance();
//                        Element->make_1_luminance(new_numero_facette,NumerosVecteurs);
                    }
                    facette_courante = &(objet_courant->Facelist[i]);   // Facelist a changé donc peut avoir changé de place en mémoire => réinit du pointeur facette_courante
                    facette_nouvelle = &(objet_courant->Facelist[new_indice_facette]);
                    facette_nouvelle->groupe     = facette_courante->groupe;
                    facette_nouvelle->codmatface = facette_courante->codmatface;
                    facette_nouvelle->flat       = facette_courante->flat;
                    facette_nouvelle->normale_b  = facette_courante->normale_b; // On réutilise la même normale que celle de base : facette_courante
                    // En toute rigueur, la recalculer car la facette de base n'est pas toujours plane !
                    Element->Calcul_Normale_Barycentre(o,new_indice_facette);

                    // Pour l'instant les facettes sont planes => recréer les normales aux sommets
                    IndicesFacettes.push_back(new_indice_facette);

                }
                if (verbose)  printf("\n");

// ATTENTION : la facette_courante de base doit aussi changer (nombre de sommets ....) tailles ....
                if (methode_triangulation == 2) {
                    facette_courante->F_sommets[2] = jj3;   // Modification du 3ème sommet de la facette de base
                }

                facette_courante->F_sommets.resize(3);
                facette_courante->Nb_Sommets_F = 3;
                if (!facette_plane) {
                    facette_courante->L_sommets.resize(3);
                    facette_courante->Nb_Sommets_L = 3;
                }
                Element->Calcul_Normale_Barycentre(o,i);    // Recalculer la normale au barycentre de la facette originale
            }

            if (objet_courant->Temps_Calcul_Aretes <= tempo_s*CLOCKS_PER_SEC) {
                Element->Genere_Tableau_Points_Facettes(objet_courant);
                Element->Genere_Tableau_Aretes(objet_courant);    // Peut-être long sur de grosses Bdd (recherche des doublons !) est-ce à faire systématiquement ?
                Element->buildAllLines();
                Refresh();
                aretes_calculees    = true;
            } else aretes_calculees = false;

//  ATTENTION : le nombre de vecteurs dans la Bdd originale peut être différent du nombre de sommets car plusieurs normales sont possibles pour 1 seul sommet

//            // on regénère les normales aux sommets, seulement pour les facettes qui ont été changées
//            auto it = IndicesFacettes.begin();
//            IndicesSommets.clear();
//            // Etablir la liste des sommets/vecteurs concernés en explorant les facettes modifiées
//            for (i=0; i<IndicesFacettes.size();i++,it++) {
//                objet_courant->Facelist[*it].flat = false;
//                for (j=0; j<3; j++) {
//                    k = objet_courant->Facelist[*it].L_sommets[j];
//                    auto kt=std::find(IndicesSommets.begin(),IndicesSommets.end(),k);
//                    if (kt == IndicesSommets.end() || IndicesSommets.empty()) {
//                        IndicesSommets.push_front(k);
//                    }
//                }
//            }
//            auto kt=IndicesSommets.begin();
//            if (methode_triangulation == 2) {   // Il faut redimensionner les vecteurs car de nouveaux sommets ont été créés et donc il va y avoir de nouveaux vecteurs !
//                printf("avant :");
//                for (k=0; k<IndicesSommets.size(); k++,kt++) printf(" %d",*kt);
//                printf("\napres :");
//                IndicesSommets.sort();
//                kt=IndicesSommets.begin();
//                for (k=0; k<IndicesSommets.size(); k++, kt++) printf(" %d",*kt);
//                printf("\n");
//                kt=IndicesSommets.end();
//                kt--;   // Pointer la dernière valeur correcte
//                objet_courant = &(Element->Objetlist[o]); // Utile à redéfinir, par précaution ???
//                printf("dernier : %d, Vecteurlist.size : %d\n",*kt,objet_courant->Vecteurlist.size()); // OUPS ! marche seulement si Vecteurlist et Sommetlist même taille
////                if (*kt > (int)objet_courant->Vecteurlist.size()) { // ce devrait toujours être vrai avec methode_triangulation = 2
//                int new_size = objet_courant->Vecteurlist.size() + new_points;
//                objet_courant->Vecteurlist.resize(new_size);
//                objet_courant->Nb_vecteurs = objet_courant->Vecteurlist.size();
////                }
//                kt=IndicesSommets.begin();
//            }
//            for (k=0; k<IndicesVecteurs.size(); k++, kt++) Element->Genere_Normale_1_Sommet(objet_courant,*kt,*kt);
//            Element->Search_Min_Max();   // Pour mettre à jour les infos de la Bdd (nombre de points, facettes, ....)
        }
    }

// Tous les objets ont été analysés et traités
    if (new_triangles == 0) {
        DisplayMessage(_T("La base de données est déjà triangulée !"),true);
    } else {
        Element->Calcul_All_Normales();     // Ici, le + simple : recalculer toutes les normales (y compris celles aux barycentres !)
        Element->Search_Min_Max();          // Pour mettre à jour les infos de la Bdd (nombre de points, facettes, ....)
        Element->SetBddModifiee(true);
        Element->m_gllist = 0;              // Retracer la nouvelle Bdd modifiée
        Element->Smemory  = nullptr;        // Raz
        Button_Undo->Disable();             // Désactiver le bouton
        Element->Refresh();
    }
}

void ModificationPanel::OnCheckBox_TransparenceClick(wxCommandEvent& event)
{
    activer_transparence = CheckBox_Transparence->GetValue();
    MAIN->Element->m_gllist = 0;
    MAIN->Element->Refresh();
}

void ModificationPanel::OnTextCtrl_ToleranceText(wxCommandEvent& event)
{
    MAIN->Element->SetTolerance(fabs(wxAtof(TextCtrl_Tolerance->GetValue())));
}

void ModificationPanel::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    NumeroGroupe = SpinCtrl_Groupe->GetValue();
}

void ModificationPanel::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    NumeroMateriau = SpinCtrl_Materiau->GetValue();
}

void ModificationPanel::OnToggleButton_CreerFacetteToggle(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    unsigned int o,i;
    static bool   old_detection_survol_point = false;

    Element->SetShowPoints(ToggleButton_CreerFacette->GetValue());
    Element->SetCreationFacette(Element->GetShowPoints());
    if (Element->GetShowPoints()) {
        Element->mode_selection = Element->selection_point;
        Button_Annuler->Enable();
        CheckBox_FacettePlane->Enable();
        old_detection_survol_point = Element->GetDetectionSurvolPoint();
        Element->SetDetectionSurvolPoint(true);                             // Activer la détection en jaune du point/sommet survolé
//        Button_InverserNormale->Enable(); // Fait dans processHits quand une facette est créée
    } else {
        Element->mode_selection = (BddInter::SELECTION)old_mode_selection;
        Element->modeGL         = (BddInter::MODE)old_modeGL;
        if (old_show_points) Element->SetShowPoints(true);                      // Dans ce cas, ne pas mettre à false
        Button_Annuler->Disable();
        CheckBox_FacettePlane ->Disable();                                      // On garde son état coché/décoché
        Button_InverserNormale->Disable();
        Element->SetDetectionSurvolPoint(old_detection_survol_point);           // Désactiver/remettre en état la détection en jaune du point/sommet survolé
        // RAZ de l'attribut "selected" des facettes en sortie de Créer facette
        for (o=0; o<Element->Objetlist.size(); o++) {
            for (i=0; i<Element->Objetlist[o].Facelist.size(); i++) Element->Objetlist[o].Facelist[i].selected=false;
        }
    }
//    Element->m_gllist = 0;    // Inutile ici ?

    Element->Refresh();
}

void ModificationPanel::OnButton_AnnulerClick(wxCommandEvent& event)
{
    wxKeyEvent key_event;
    key_event.m_keyCode = 'S';
    MAIN->Element->OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection de points
}

void ModificationPanel::OnButton_SupprimerFacetteClick(wxCommandEvent& event)
{
    Object   *Objet_courant;
    BddInter *Element = MAIN->Element;

    unsigned int indice = Element->GetIndiceObjetCourant();
    Objet_courant = &(Element->Objetlist[indice]);
    unsigned int Nb_facettes_original = Objet_courant->Nb_facettes_original;
    printf("Nb_facettes_old = %d, Nb_facettes = %d\n",Nb_facettes_original,Objet_courant->Nb_facettes);
    Objet_courant->Facelist.pop_back();
    Objet_courant->Nb_facettes = Objet_courant->Facelist.size();
    if (Objet_courant->Nb_facettes == Nb_facettes_original) {
        Button_SupprimerFacette->Disable();
        Objet_courant->Nb_facettes_original = 0;
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void ModificationPanel::OnButton_InverserToutClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Inverser_Toutes_les_Normales();
    Element->m_gllist = 0;
    Element->Refresh();
}

void ModificationPanel::OnCheckBox_FacettePlaneClick(wxCommandEvent& event)
{
    FacetteCreeePlane = CheckBox_FacettePlane->GetValue();
}

void ModificationPanel::OnButton_RecalculerAretesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    Object   *objet_courant;
    unsigned int o;

    for (o=0; o<Element->Objetlist.size(); o++) {
        objet_courant = &(Element->Objetlist[o]);
        if (objet_courant->deleted) continue;   // Objet supprimé mais encore en mémoire => passer au suivant
        Element->Genere_Tableau_Points_Facettes(objet_courant);
        Element->Genere_Tableau_Aretes(objet_courant);
        aretes_calculees = true;
    }
    Element->buildAllLines();
    Element->Refresh();
}

void ModificationPanel::OnCheckBox_NotFlatClick(wxCommandEvent& event)
{
    MAIN->Element->SetNotFlat(CheckBox_NotFlat->GetValue());
}
