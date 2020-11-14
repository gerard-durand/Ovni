#include "SelectionPanel.h"

//(*InternalHeaders(SelectionPanel)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(SelectionPanel)
const long SelectionPanel::ID_RADIOBOX1 = wxNewId();
const long SelectionPanel::ID_STATICTEXT1 = wxNewId();
const long SelectionPanel::ID_BUTTON1 = wxNewId();
const long SelectionPanel::ID_BUTTON2 = wxNewId();
const long SelectionPanel::ID_STATICTEXT2 = wxNewId();
const long SelectionPanel::ID_STATICTEXT3 = wxNewId();
const long SelectionPanel::ID_STATICTEXT4 = wxNewId();
const long SelectionPanel::ID_STATICTEXT5 = wxNewId();
const long SelectionPanel::ID_STATICTEXT6 = wxNewId();
const long SelectionPanel::ID_STATICTEXT7 = wxNewId();
const long SelectionPanel::ID_STATICTEXT8 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL1 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL2 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL9 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL3 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL10 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL4 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL5 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL6 = wxNewId();
const long SelectionPanel::ID_RADIOBOX2 = wxNewId();
const long SelectionPanel::ID_STATICTEXT9 = wxNewId();
const long SelectionPanel::ID_STATICTEXT10 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL7 = wxNewId();
const long SelectionPanel::ID_STATICTEXT11 = wxNewId();
const long SelectionPanel::ID_TEXTCTRL8 = wxNewId();
const long SelectionPanel::ID_BUTTON3 = wxNewId();
const long SelectionPanel::ID_CHECKBOX1 = wxNewId();
const long SelectionPanel::ID_BUTTON4 = wxNewId();
const long SelectionPanel::ID_BUTTON5 = wxNewId();
const long SelectionPanel::ID_BUTTON6 = wxNewId();
const long SelectionPanel::ID_BUTTON7 = wxNewId();
const long SelectionPanel::ID_BUTTON8 = wxNewId();
const long SelectionPanel::ID_BUTTON9 = wxNewId();
const long SelectionPanel::ID_BUTTON15 = wxNewId();
const long SelectionPanel::ID_BUTTON10 = wxNewId();
const long SelectionPanel::ID_BUTTON11 = wxNewId();
const long SelectionPanel::ID_BUTTON12 = wxNewId();
const long SelectionPanel::ID_RADIOBOX3 = wxNewId();
const long SelectionPanel::ID_STATICTEXT12 = wxNewId();
const long SelectionPanel::ID_BUTTON13 = wxNewId();
const long SelectionPanel::ID_BUTTON14 = wxNewId();
const long SelectionPanel::ID_BUTTON16 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SelectionPanel,wxDialog)
	//(*EventTable(SelectionPanel)
	//*)
END_EVENT_TABLE()

ListeSelected UndoListe;

SelectionPanel::SelectionPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SelectionPanel)
	Create(parent, wxID_ANY, _T("Sélection - Déplacements"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(280,717));
	Move(wxPoint(20,20));
	wxString __wxRadioBoxChoices_1[3] =
	{
		_T("Points           "),
		_T("Facettes           "),
		_T("Objets     ")
	};
	RadioBox_Selection = new wxRadioBox(this, ID_RADIOBOX1, wxEmptyString, wxPoint(0,8), wxSize(280,40), 3, __wxRadioBoxChoices_1, 1, wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX1"));
	RadioBox_Selection->SetSelection(1);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Mode de Sélection"), wxDefaultPosition, wxSize(288,16), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText1->SetForegroundColour(wxColour(255,255,255));
	StaticText1->SetBackgroundColour(wxColour(0,0,0));
	Button_SelectionManuelleFacettes = new wxButton(this, ID_BUTTON1, _T("Sélection manuelle"), wxPoint(8,48), wxSize(264,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_SelectionManuelleFacettes->Disable();
	Button_OuvrirReperage = new wxButton(this, ID_BUTTON2, _T("Ouvrir la fenêtre \"Repérage/Facettes\""), wxPoint(8,72), wxSize(264,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_OuvrirReperage->Disable();
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Informations"), wxPoint(0,104), wxSize(288,16), wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
	StaticText2->SetForegroundColour(wxColour(255,255,255));
	StaticText2->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Objet :"), wxPoint(8,132), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Numéro/Indice de l\'objet :"), wxPoint(8,156), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText_Fac = new wxStaticText(this, ID_STATICTEXT5, _T("Numéro/Indice de la facette"), wxPoint(8,180), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	StaticText_Selection = new wxStaticText(this, ID_STATICTEXT6, _T("Facettes sélectionnées :"), wxPoint(8,204), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText_Grp = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro du Groupe :"), wxPoint(8,228), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	StaticText_Mat = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro du Matériau :"), wxPoint(8,252), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	TextCtrl_NomObjet = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(56,128), wxSize(216,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl_NumObjet = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(160,152), wxSize(56,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextCtrl_IndObjet = new wxTextCtrl(this, ID_TEXTCTRL9, wxEmptyString, wxPoint(216,152), wxSize(56,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	TextCtrl_NumFacette = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxPoint(160,176), wxSize(56,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextCtrl_IndFacette = new wxTextCtrl(this, ID_TEXTCTRL10, wxEmptyString, wxPoint(216,176), wxSize(56,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	TextCtrl_Selection = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxPoint(160,200), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	TextCtrl_NumGroupe = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxPoint(160,224), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	TextCtrl_NumMateriau = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxPoint(160,248), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	wxString __wxRadioBoxChoices_2[2] =
	{
		_T("Groupe      "),
		_T("Matériau")
	};
	RadioBox_GrpMat = new wxRadioBox(this, ID_RADIOBOX2, wxEmptyString, wxPoint(56,284), wxSize(160,40), 2, __wxRadioBoxChoices_2, 1, wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX2"));
	RadioBox_GrpMat->SetSelection(0);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Modifier la sélection"), wxPoint(0,280), wxSize(288,16), wxALIGN_CENTRE, _T("ID_STATICTEXT9"));
	StaticText9->SetForegroundColour(wxColour(255,255,255));
	StaticText9->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	StaticText_NumerosUtilises = new wxStaticText(this, ID_STATICTEXT10, _T("Numéros des groupes utilisés"), wxPoint(56,328), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	TextCtrl_NumerosUtilises = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxPoint(0,344), wxSize(280,24), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	TextCtrl_NumerosUtilises->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR));
	StaticText_Changer = new wxStaticText(this, ID_STATICTEXT11, _T("Changer le numéro de groupe"), wxPoint(8,372), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	TextCtrl_GrpMat = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxPoint(184,368), wxSize(32,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	Button_Appliquer = new wxButton(this, ID_BUTTON3, _T("Appliquer"), wxPoint(216,368), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	CheckBox_ForcerFlat = new wxCheckBox(this, ID_CHECKBOX1, _T("Forcer le mode Faces planes"), wxPoint(8,400), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_ForcerFlat->SetValue(false);
	CheckBox_ForcerFlat->Disable();
	Button_InverserNormales = new wxButton(this, ID_BUTTON4, _T("Inverser les normales"), wxPoint(24,424), wxSize(160,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_UndoNormales = new wxButton(this, ID_BUTTON5, _T("Undo"), wxPoint(184,424), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button_UndoNormales->Disable();
	Button_Delete = new wxButton(this, ID_BUTTON6, _T("Supprimer la/les facette(s)"), wxPoint(24,448), wxSize(160,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button_UndoDelete = new wxButton(this, ID_BUTTON7, _T("Undo"), wxPoint(184,448), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	Button_UndoDelete->Disable();
	Button_InverserParcours = new wxButton(this, ID_BUTTON8, _T("Inversion du sens de parcours"), wxPoint(24,496), wxSize(232,24), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	Button_Masquer = new wxButton(this, ID_BUTTON9, _T("Masquer les facettes"), wxPoint(24,520), wxSize(160,24), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	Button_UndoMasquer = new wxButton(this, ID_BUTTON15, _T("Undo"), wxPoint(184,520), wxSize(72,24), 0, wxDefaultValidator, _T("ID_BUTTON15"));
	Button_UndoMasquer->Disable();
	Button_Reafficher = new wxButton(this, ID_BUTTON10, _T("Tout réafficher"), wxPoint(24,544), wxSize(232,24), 0, wxDefaultValidator, _T("ID_BUTTON10"));
	Button_Centrer = new wxButton(this, ID_BUTTON11, _T("Centrer la rotation sur la/les facettes"), wxPoint(24,568), wxSize(232,24), 0, wxDefaultValidator, _T("ID_BUTTON11"));
	Button_Manipulations = new wxButton(this, ID_BUTTON12, _T("Manipulations de l\'objet"), wxPoint(24,592), wxSize(232,24), 0, wxDefaultValidator, _T("ID_BUTTON12"));
	wxString __wxRadioBoxChoices_3[3] =
	{
		_T("Unique"),
		_T("Faces avant"),
		_T("Faces arrière")
	};
	RadioBox_TypeSelection = new wxRadioBox(this, ID_RADIOBOX3, wxEmptyString, wxPoint(0,624), wxSize(280,40), 3, __wxRadioBoxChoices_3, 1, wxRA_VERTICAL, wxDefaultValidator, _T("ID_RADIOBOX3"));
	RadioBox_TypeSelection->Disable();
	StaticText10 = new wxStaticText(this, ID_STATICTEXT12, _T("Type de Sélection"), wxPoint(0,624), wxSize(288,16), wxALIGN_CENTRE, _T("ID_STATICTEXT12"));
	StaticText10->SetForegroundColour(wxColour(255,255,255));
	StaticText10->SetBackgroundColour(wxColour(0,0,0));
	Button_Etendre = new wxButton(this, ID_BUTTON13, _T("Étendre la sélection"), wxPoint(64,664), wxSize(144,24), 0, wxDefaultValidator, _T("ID_BUTTON13"));
	Button_Quitter = new wxButton(this, ID_BUTTON14, _T("Fermer"), wxPoint(184,688), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON14"));
	Button_Fusionner = new wxButton(this, ID_BUTTON16, _T("Fusionner les objets sélectionnés"), wxPoint(24,472), wxSize(232,23), 0, wxDefaultValidator, _T("ID_BUTTON16"));
	Button_Fusionner->Disable();

	Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SelectionPanel::OnRadioBox_SelectionSelect);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_SelectionManuelleFacettesClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_OuvrirReperageClick);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&SelectionPanel::OnTextCtrl_NomObjetText);
	Connect(ID_RADIOBOX2,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SelectionPanel::OnRadioBox_GrpMatSelect);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_AppliquerClick);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnCheckBox_ForcerFlatClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_InverserNormalesClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_UndoNormalesClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_DeleteClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_UndoDeleteClick);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_InverserParcoursClick);
	Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_MasquerClick);
	Connect(ID_BUTTON15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_UndoMasquerClick);
	Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_ReafficherClick);
	Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_CentrerClick);
	Connect(ID_BUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_ManipulationsClick);
	Connect(ID_RADIOBOX3,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&SelectionPanel::OnRadioBox_TypeSelectionSelect);
	Connect(ID_BUTTON13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_EtendreClick);
	Connect(ID_BUTTON14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_QuitterClick);
	Connect(ID_BUTTON16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionPanel::OnButton_FusionnerClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&SelectionPanel::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SelectionPanel::OnClose);
	//*)

//	this->Selection_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

SelectionPanel::~SelectionPanel()
{
	//(*Destroy(SelectionPanel)
	//*)
}

void SelectionPanel::InitPanel()
{
    BddInter *Element = MAIN->Element;

    switch (RadioBox_Selection->GetSelection()) {
    case 0:
        Element->mode_selection = Element->selection_point;
        Element->show_points = true;
//        Element->style = GL_POINTS;
        Button_OuvrirReperage->SetLabel(_T("Ouvrir la fenêtre \"Repérage/Points\""));
        Button_OuvrirReperage->Enable();
        Button_SelectionManuelleFacettes->Disable();

        StaticText_Selection->SetLabel(_T("Points sélectionnés :"));
        StaticText_Fac->SetLabel(_T("Numéro/Indice du point"));
        StaticText_Fac->Enable();
        StaticText_Grp->Disable();
        StaticText_Mat->Disable();
        TextCtrl_NumFacette ->Enable();
        TextCtrl_IndFacette ->Enable();
        TextCtrl_NumGroupe  ->Enable();
        TextCtrl_NumMateriau->Disable();

        Button_InverserNormales->Disable();
        Button_UndoNormales    ->Disable();

        Button_Delete->SetLabel(_T("Supprimer la/les facette(s)"));
        Button_Delete->Disable();
        Button_UndoDelete->Disable();

        Button_InverserParcours->Disable();
        Button_Masquer->Disable();
        Button_Fusionner->Disable();

//        Button_Reafficher->Enable();

        Button_Centrer->Enable();
        Button_Centrer->SetLabel(_T("Centrer la rotation sur le(s) point(s)"));

        RadioBox_TypeSelection->Disable();
        Button_Etendre->Disable();
        CheckBox_ForcerFlat->Disable();
        CheckBox_ForcerFlat->SetValue(false);

        break;

    case 1:
        Element->mode_selection = Element->selection_facette;
        Element->show_points = false;
//        Element->style = GL_POLYGON;
        Button_OuvrirReperage->SetLabel(_T("Ouvrir la fenêtre \"Repérage/Facettes\""));
        Button_OuvrirReperage->Enable();
        Button_SelectionManuelleFacettes->Enable();

        StaticText_Selection->SetLabel(_T("Facettes sélectionnées :"));
        StaticText_Fac->SetLabel(_T("Numéro/Indice de la facette"));
        StaticText_Fac->Enable();
        StaticText_Grp->Enable();
        StaticText_Mat->Enable();
        TextCtrl_NumFacette ->Enable();
        TextCtrl_IndFacette ->Enable();
        TextCtrl_NumGroupe  ->Enable();
        TextCtrl_NumMateriau->Enable();

        Button_InverserNormales->Enable();
//        Button_UndoNormales    ->Enable();    // Changer ou pas ?

        Button_Delete->SetLabel(_T("Supprimer la/les facette(s)"));
        Button_Delete->Enable();
//        Button_UndoDelete->Enable();

        Button_InverserParcours->Enable();
        Button_Fusionner->Disable();


        Button_Masquer->SetLabel(_T("Masquer les facettes"));
        Button_Masquer->Enable();

//        Button_Reafficher->Enable();

        Button_Centrer->Enable();
        Button_Centrer->SetLabel(_T("Centrer la rotation sur la/les facette(s)"));

//        RadioBox_TypeSelection->Enable();
        Button_Etendre->Enable();
        CheckBox_ForcerFlat->Enable();
        CheckBox_ForcerFlat->SetValue(false);

        break;

    case 2:
        Element->mode_selection = Element->selection_objet;
        // On pourrait effacer une éventuelle sélection de facettes via key_event.m_keyCode = 's'; Element->OnKeyDown(key_event);
        Element->show_points = false;
//        Element->style = GL_POLYGON;
        Button_OuvrirReperage->SetLabel(_T("Ouvrir la fenêtre \"Repérage/Objets\""));
        Button_OuvrirReperage->Enable();    // Disable() en version Tcl. Pourquoi ?
        Button_SelectionManuelleFacettes->Enable();

        StaticText_Selection->SetLabel(_T("Objets sélectionnés :"));
        StaticText_Fac->SetLabel(_T("Numéro/Indice de la facette"));
        StaticText_Fac->Disable();
        StaticText_Grp->Disable();
        StaticText_Mat->Disable();
        TextCtrl_NumFacette ->Disable();
        TextCtrl_IndFacette ->Disable();
        TextCtrl_NumGroupe  ->Disable();
        TextCtrl_NumMateriau->Disable();

        Button_InverserNormales->Enable();
//        Button_UndoNormales    ->Enable();    // Changer ou pas ?
        if (Element->listeObjets.size() >= 2) { // à ce niveau, doit sans doute toujours être nul, donc test inutile ?
            Button_Fusionner->Enable();
        } else {
            Button_Fusionner->Disable();
        }

        Button_Delete->SetLabel(_T("Supprimer l'Objet"));
        Button_Delete->Enable();
//        Button_UndoDelete->Enable();

        Button_InverserParcours->Disable();

        Button_Masquer->SetLabel(_T("Masquer l'Objet"));
        Button_Masquer->Enable();

//        Button_Reafficher->Enable();
        Button_Centrer->Enable();
        Button_Centrer->SetLabel(_T("Centrer la rotation sur l'Objet/les Objets)"));

        RadioBox_TypeSelection->Disable();
        Button_Etendre->Disable();

        CheckBox_ForcerFlat->Enable(); // A voir ?
        CheckBox_ForcerFlat->SetValue(false);

        break;

    default:
        Element->mode_selection = Element->aucune;
        break;
    }

    if (RadioBox_GrpMat->GetSelection() == 0) {
        // Groupe sélectionné
        StaticText_Changer->SetLabel(_T("Changer le numéro de groupe"));
        StaticText_NumerosUtilises->SetLabel(_T("Numéros des groupes utilisés"));
    } else {
        // Matériau sélectionné
        StaticText_Changer->SetLabel(_T("Changer le numéro de matériau"));
        StaticText_NumerosUtilises->SetLabel(_T("Numéros des matériaux utilisés"));
    }

}

void SelectionPanel::OnInit(wxInitDialogEvent& event)
{
    InitPanel();
}

void SelectionPanel::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    Hide(); // Masquer la fenêtre

    MAIN->toggle_outils = false;
    MAIN->Button_Outils->SetValue(false);
    if (MAIN->Button_Points->GetValue())    // Si le bouton Afficher les points est activé, remettre show_points à true
        Element->show_points = true;
    else
        Element->show_points = false;   // sinon, false
    if (Element->mode_selection == Element->selection_objet) {
        Element->m_gllist=0;
    }
    Element->listeObjets.clear();
    Element->listePoints.clear();
    for (unsigned int i=0; i<Element->Objetlist.size(); i++) Element->Objetlist[i].selected=false;
    Element->SelectionObjet = 0;
    Element->mode_selection = Element->selection_facette;   // Remettre en mode sélection de facettes
    RadioBox_Selection->SetSelection(1);                    // aussi dans l'interface
    RadioBox_GrpMat->SetSelection(0);                       // Remettre le choix sur Groupe/Matériau sur Groupe
    wxCommandEvent new_event;
    OnRadioBox_GrpMatSelect(new_event); // Remettre en état les choix et labels groupe/matériau (par simulation d'un clic)
    Element->Refresh();
}

void SelectionPanel::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void SelectionPanel::OnRadioBox_SelectionSelect(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str_reset;
    str_reset.clear();                              // Vide pour forcer un affichage de case sans rien dedans
    unsigned int i;

    Element->listeObjets.clear();
    Element->listePoints.clear();
    for (i=0; i<Element->Objetlist.size(); i++) Element->Objetlist[i].selected = false;

    InitPanel();
    TextCtrl_NomObjet   ->ChangeValue(str_reset);
    TextCtrl_NumObjet   ->SetValue(str_reset);
    TextCtrl_IndObjet   ->SetValue(str_reset);
    TextCtrl_Selection  ->SetValue(str_reset);
    TextCtrl_NumFacette ->SetValue(str_reset);
    TextCtrl_IndFacette ->SetValue(str_reset);
    TextCtrl_NumGroupe  ->SetValue(str_reset);
    TextCtrl_NumMateriau->SetValue(str_reset);

    wxKeyEvent key_event;               // Raz de la liste de points/facettes sélectionnés
    key_event.m_keyCode = 'S';
    Element->OnKeyDown(key_event);      // Simule une pression sur la touche S au clavier

    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnRadioBox_GrpMatSelect(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int n_val, i;
    wxString str, str_grpmat;

    Element->listeGroupes.sort();
    Element->listeMateriaux.sort();
    auto it = Element->listeGroupes.begin();
    str_grpmat.clear();

    if (RadioBox_GrpMat->GetSelection() == 0) {
        // Groupe sélectionné
        StaticText_Changer->SetLabel(_T("Changer le numéro de groupe"));
        StaticText_NumerosUtilises->SetLabel(_T("Numéros des groupes utilisés"));
        // Construction de la liste des groupes à afficher
        n_val = Element->listeGroupes.size();
        str.Printf(_T("%d"), (int)*it++);
        str_grpmat += str;
        for (i=1; i<n_val; i++) {
            str.Printf(_T(", %d"), *it++);  // Formattage des valeurs séparées par une virgule
            str_grpmat += str;              // Concaténation des valeurs
        }
    } else {
        StaticText_Changer->SetLabel(_T("Changer le numéro de matériau"));
        StaticText_NumerosUtilises->SetLabel(_T("Numéros des matériaux utilisés"));
        it = Element->listeMateriaux.begin();
        // Construction de la liste des matériaux à afficher
        n_val = Element->listeMateriaux.size();
        str.Printf(_T("%d"), (int)*it++);
        str_grpmat += str;
        for (i=1; i<n_val; i++) {
            str.Printf(_T(", %d"), *it++);  // Formattage des valeurs séparées par une virgule
            str_grpmat += str;              // Concaténation des valeurs
        }
    }
    TextCtrl_NumerosUtilises->SetLabel(str_grpmat);
}

void SelectionPanel::OnButton_AppliquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    unsigned int i,j,n_val;

    int new_GrpMat = wxAtoi(TextCtrl_GrpMat->GetValue());

    if (Element->mode_selection == Element->selection_facette) {
        for (i=0; i<Element->Objetlist.size(); i++) {
            if (Element->Objetlist[i].deleted) continue ;                     // Ne pas traiter les objets supprimés
            for (j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
                if (Element->Objetlist[i].Facelist[j].deleted) continue;      // Ne pas traiter les facettes supprimées
                if (Element->Objetlist[i].Facelist[j].selected) {
                    if (RadioBox_GrpMat->GetSelection() == 0)
                        Element->Objetlist[i].Facelist[j].groupe     = new_GrpMat;
                    else
                        Element->Objetlist[i].Facelist[j].codmatface = new_GrpMat;
                }
            }
        }
    } else if (Element->mode_selection == Element->selection_objet) {
        auto it = Element->listeObjets.begin();
        n_val   = Element->listeObjets.size();
        for (i=0; i<n_val; i++, it++) {
            if (Element->Objetlist[*it].deleted) continue ;                   // Ne pas traiter les objets supprimés
            for (j=0; j<Element->Objetlist[*it].Facelist.size(); j++) {
                if (Element->Objetlist[*it].Facelist[j].deleted) continue;    // Ne pas traiter les facettes supprimées
                if (RadioBox_GrpMat->GetSelection() == 0)
                    Element->Objetlist[*it].Facelist[j].groupe       = new_GrpMat;
                else
                    Element->Objetlist[*it].Facelist[j].codmatface   = new_GrpMat;
            }
        }
    }
    Element->listeGroupes.clear();
    Element->listeMateriaux.clear();
    for (i=0; i<Element->Objetlist.size(); i++) {
        Element->GenereListeGroupesMateriaux(i);
    }
    OnRadioBox_GrpMatSelect(event) ; // Simuler un clic sur Groupe/Matériau pour regénérer la liste affichée dans l'interface

    Element->bdd_modifiee = true;
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnButton_InverserNormalesClick(wxCommandEvent& event)
{
// le mode_point n'est pas concerné
    BddInter *Element = MAIN->Element;

    unsigned int i;

    if (Element->mode_selection == Element->selection_facette) {
        Element->Inverse_Selected_Normales();
    } else if (Element->mode_selection == Element->selection_objet) {
        auto it = Element->listeObjets.begin();
        for (i=0; i<Element->listeObjets.size(); i++, it++) {
            Element->Inverser_les_Normales_Objet(*it);
        }
    }
    Button_UndoNormales->Enable();
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnButton_UndoNormalesClick(wxCommandEvent& event)
{
    OnButton_InverserNormalesClick(event);  // En fait <=> Inverser les normales
}

void SelectionPanel::OnButton_DeleteClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

// le mode_point n'est pas concerné
    if (Element->mode_selection == Element->selection_facette) {
        UndoListe = Element->ToSelect;              // Sauvegarder la liste ToSelect dans UndoListe
        wxKeyEvent key_event;
        key_event.m_keyCode = WXK_DELETE;           // Touche Suppr principale
        Element->OnKeyDown(key_event);              // Simule une pression sur la touche "Suppr" au clavier
    } else if (Element->mode_selection == Element->selection_objet) {
//        auto it = Element->listeObjets.begin();
//        for (unsigned int i=0; i<Element->listeObjets.size(); i++, it++) {
//            Element->Objetlist[*it].deleted = true;
//        }
        wxKeyEvent key_event;
        key_event.m_keyCode = WXK_DELETE;           // Touche Suppr principale
        Element->OnKeyDown(key_event);              // Simule une pression sur la touche "Suppr" au clavier
    }
    Button_UndoDelete->Enable();
//    Element->m_gllist = 0;    // Inutile car fait via OnKeyDown
//    Element->Refresh();       // ""
}

void SelectionPanel::OnButton_UndoDeleteClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

// le mode_point n'est pas concerné
    if (Element->mode_selection == Element->selection_facette) {
        Element->ToSelect = UndoListe;    // Remettre en place la liste ToSelect depuis UndoListe
        for (unsigned int i=0; i<Element->Objetlist.size(); i++) {
            if (Element->Objetlist[i].deleted) continue;                  // Ne rien faire sur les objets complètement supprimés
            for (unsigned int j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
                if (Element->ToSelect.check_if_in_ListeSelect(i,j)) {
                    Element->Objetlist[i].Facelist[j].deleted  = false;   // Undelete
                    Element->colorface(i,j);                              // coloriser la facette
                    Element->Objetlist[i].Facelist[j].selected = true;    // puis la marquer comme Sélectionnée
                    Element->Objetlist[i].Facelist[j].afficher = true;    // + réafficher
                }
            }
        }
    } else if (Element->mode_selection == Element->selection_objet) {
        auto it = Element->listeObjets.begin();
        for (unsigned int i=0; i<Element->listeObjets.size(); i++, it++) {
            Element->Objetlist[*it].deleted = false;
        }
    }
    Button_UndoDelete->Disable();
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnButton_PermutationClick(wxCommandEvent& event)
{
}

void SelectionPanel::OnButton_MasquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

// le mode_point n'est pas concerné
    if (Element->mode_selection == Element->selection_facette) {
        UndoListe = Element->ToSelect;              // Sauvegarder la liste ToSelect dans UndoListe
        wxKeyEvent key_event;
        key_event.m_keyCode = WXK_NUMPAD_DELETE;    // Touche Suppr du Numpad
        Element->OnKeyDown(key_event);              // Simule une pression sur la touche "Suppr" du clavier Numpad
    } else if (Element->mode_selection == Element->selection_objet) {
        wxKeyEvent key_event;
        key_event.m_keyCode = WXK_NUMPAD_DELETE;    // Touche Suppr du Numpad
        Element->OnKeyDown(key_event);              // Simule une pression sur la touche "Suppr" du clavier Numpad
//        auto it = Element->listeObjets.begin();
//        for (unsigned int i=0; i<Element->listeObjets.size(); i++, it++) {
//            Element->Objetlist[*it].afficher = false;
//        }
    }
    Button_UndoMasquer->Enable();
//    Element->m_gllist = 0;    // Inutile car fait via OnKeyDown
//    Element->Refresh();       // ""
}

void SelectionPanel::OnButton_UndoMasquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    if (Element->mode_selection == Element->selection_facette) {
        Element->ToSelect = UndoListe;    // Remettre en place la liste ToSelect depuis UndoListe
        for (unsigned int i=0; i<Element->Objetlist.size(); i++) {
            if (Element->Objetlist[i].deleted) continue;                  // Ne rien faire sur les objets complètement supprimés
            for (unsigned int j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
                if (Element->ToSelect.check_if_in_ListeSelect(i,j)) {
                    Element->colorface(i,j);                              // coloriser la facette
                    Element->Objetlist[i].Facelist[j].selected = true;    // puis la marquer comme Sélectionnée
                    Element->Objetlist[i].Facelist[j].afficher = true;    // + réafficher
                }
            }
        }
    } else if (Element->mode_selection == Element->selection_objet) {
        auto it = Element->listeObjets.begin();
        for (unsigned int i=0; i<Element->listeObjets.size(); i++, it++) {
            Element->Objetlist[*it].afficher = true;
        }
    }
    Button_UndoMasquer->Disable();
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::Reset_ListeObjets()
{
    BddInter *Element = MAIN->Element;
    unsigned int i,n,indice_ListBox;

    wxString str_loc;
    n = MAIN->Selections_Manuelles_Objets->CheckListBox1->GetCount();   // Récupère le nombre d'items actuels
    for (i=0 ; i<n ; i++)
        MAIN->Selections_Manuelles_Objets->CheckListBox1->Delete(0);    // Supprime ces "anciens" items

    indice_ListBox = 0; // Pour découpler l'indice d'un objet de celui de la CheckListBox (en cas d'objets supprimés)
    for (i=0; i<Element->Objetlist.size(); i++) {
        if (!Element->Objetlist[i].deleted) {
            str_loc = wxString(Element->Objetlist[i].GetName(), wxConvUTF8);
            MAIN->Selections_Manuelles_Objets->CheckListBox1->Append(str_loc);    // Recrée tous les items (des objets non supprimés)
            MAIN->Selections_Manuelles_Objets->CheckListBox1->Check(indice_ListBox,Element->Objetlist[i].selected); // Coche ceux qui sont déjà sélectionnés
            indice_ListBox++;
        }
    }
}

void SelectionPanel::OnButton_ReafficherClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    unsigned int i;

    for (i=0; i<Element->Objetlist.size(); i++) {
        Element->Objetlist[i].deleted = false;                        // Revalider les objets complètement supprimés
        Element->Objetlist[i].afficher= true;                         // Les réafficher
        for (unsigned int j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
            Element->Objetlist[i].Facelist[j].deleted  = false;       // Undelete des facettes
            Element->Objetlist[i].Facelist[j].afficher = true;        // + réafficher toutes les facettes
        }
    }

    if (MAIN->Selections_Manuelles_Objets->IsShown()) {                     // Mettre à jour si la fenêtre est affichée
        Reset_ListeObjets();
    }

    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnButton_CentrerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

// OK sur facette, points et objets
//    if ((Element->mode_selection == Element->selection_facette) ||
//        (Element->mode_selection == Element->selection_point)) {
        wxKeyEvent key_event;
        key_event.m_keyCode = 'C';
        Element->OnKeyDown(key_event);   // Simule une pression sur la touche "c" au clavier
//    }
}

void SelectionPanel::OnButton_ManipulationsClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool test;
    wxString wxMessage;

    test = false;
    if (Element->mode_selection == Element->selection_objet) {
        if (Element->listeObjets.empty()) {
            test      = true;
            wxMessage = _T("Aucun Objet sélectionné !");
        }
    } else if (Element->mode_selection == Element->selection_facette) {
        if (Element->ToSelect.ListeSelect.empty()) {
            test      = true;
            wxMessage = _T("Aucune Facette sélectionnée !");
        }
    }
    if (test) {
        wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Avertissement"),
                                                 wxOK | wxICON_INFORMATION ); // Avec l'icône wxICON_QUESTION, l'affichage reste silencieux (wxICON_INFORMATION + logique, mais bruyant !!)
        query->ShowModal();
        query->Destroy();
        return;
    }
    // Le bouton Rotation de Manipulation d'objet n'est actif que si on est en mode de sélection d'objet !
    if (Element->mode_selection == Element->selection_objet)
        Element->MManip->Button_Rotation->Enable();
    else
        Element->MManip->Button_Rotation->Disable();

    Element->MManip->Show();
}

void SelectionPanel::OnRadioBox_TypeSelectionSelect(wxCommandEvent& event)
{
}

void SelectionPanel::OnButton_EtendreClick(wxCommandEvent& event)
{
    wxKeyEvent key_event;
    key_event.m_keyCode = 'X';
    MAIN->Element->OnKeyDown(key_event);   // Simule une pression sur la touche "X" au clavier
}

void SelectionPanel::OnButton_InverserParcoursClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Inverser_Parcours_Selected();
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnTextCtrl_NomObjetText(wxCommandEvent& event)
{
// Pour entrer un nouveau nom d'objet, à condition qu'il n'y en ait qu'un seul de sélectionné
// NOTE : si wxTE_PROCESS_ENTER est activé dans le contrôle TextCtrl_NomObjet de SelectionPanel.wxs
// il faut appuyer sur la touche "Entrée" (ou "Return") pour valider. Ici, on valide à chaque caractère entré !

// On ne passe pas ici lors des TextCtrl_NomObjet->ChangeValue(wxNom); (pas d'event généré). Mais c'est le cas si TextCtrl_NomObjet->SetValue(wxNom);

    BddInter *Element = MAIN->Element;

    if (Element->mode_selection == Element->selection_objet) {
        if (Element->listeObjets.size() == 1) {
            auto it = Element->listeObjets.begin();
            wxString Nouveau_nom = TextCtrl_NomObjet->GetValue();
            Nouveau_nom = Nouveau_nom.utf8_str();   // Conversion de utf8 en string
            Element->Objetlist[*it].SetName(Nouveau_nom);
            printf("Nouveau nom de l'objet : %s\n",Element->Objetlist[*it].GetName());
            Element->bdd_modifiee = true;
        } else {
            if (Element->listeObjets.size() > 1) printf("Le changement de nom d'Objet ne fonctionne que sur la selection d'un seul objet !\n");
        }
    } else {
        printf("Le changement de nom d'Objet ne fonctionne qu'en mode \"Selection d'Objets\" !\n");
    }
}

void SelectionPanel::OnButton_SelectionManuelleFacettesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString Titre, Identification, str_loc;

    if (Element->mode_selection == Element->selection_objet) {
        Reset_ListeObjets();
        MAIN->Selections_Manuelles_Objets->Show();

    } else if (Element->mode_selection == Element->selection_facette) {
        unsigned int NumeroObjet = MAIN->Selections_Manuelles_Facettes->SpinCtrl_NumObjet->GetValue();
        MAIN->Selections_Manuelles_Facettes->SpinCtrl_NumObjet->SetMax(Element->Objetlist.size()-1);
        str_loc = wxString(Element->Objetlist[NumeroObjet].GetName(), wxConvUTF8);
        MAIN->Selections_Manuelles_Facettes->StaticText_NomObjet->SetLabel(str_loc);

// NOTE : on pourrait peut-être pré-remplir MAIN->Selections_Manuelles_Facettes->TextCtrl_Numeros avec le contenu des listes de Element->ToSelect,
//          soit des facettes déjà sélectionnées, si non vide. OK si dans 1 seul objet, plus difficile si dans plusieurs objets.

        MAIN->Selections_Manuelles_Facettes->Show();

    } else {
        Titre = _T("Sélection manuelle des points") ;   // Ne sert pas ?
        Identification = _T("Numéros des points");
    }

}

void SelectionPanel::OnButton_OuvrirReperageClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    if (Element->mode_selection == Element->selection_point) {
//        MAIN->ReperagePoint_Panel->Show();
        MAIN->ReperagePoint_activer();
    } else if (Element->mode_selection == Element->selection_facette) {
//        MAIN->ReperageFacette_Panel->Show();
        MAIN->ReperageFacette_activer();
    } else { // <=> selection_objet
//        MAIN->ReperageObjet_Panel->Show();
        MAIN->ReperageObjet_activer();
    }
}

void SelectionPanel::OnCheckBox_ForcerFlatClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;
    unsigned int i,j;

    if (Element->mode_selection == Element->selection_objet) {
        for (i=0;i<Element->Objetlist.size(); i++) {
            if (Element->Objetlist[i].selected) {                                                                           // Sur tous les objets sélectionnés,
                for (j=0;j<Element->Objetlist[i].Facelist.size(); j++) Element->Objetlist[i].Facelist[j].selected = true;   // Sélectionner toutes les facettes
            }
        }
    }

    if (CheckBox_ForcerFlat->GetValue())
        Element->Flat_Selected_Facettes();
    else
        Element->NotFlat_Selected_Facettes();

    if (Element->mode_selection == Element->selection_objet) {
        for (i=0;i<Element->Objetlist.size(); i++) {
            if (Element->Objetlist[i].selected) {                                                                           // Sur tous les objets sélectionnés,
                for (j=0;j<Element->Objetlist[i].Facelist.size(); j++) Element->Objetlist[i].Facelist[j].selected = false;  // Désélectionner toutes les facettes
            }
        }
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionPanel::OnButton_FusionnerClick(wxCommandEvent& event)
{
    unsigned int o, k, j, Nb_valeurs;
    unsigned int New_Nb_sommets, New_Nb_facettes, New_Nb_normales, New_Nb_aspects, New_Nb_luminances, New_Nb_vecteurs;
    unsigned int Offset_Nb_sommets, Offset_Nb_facettes, Offset_Nb_normales, Offset_Nb_aspects, Offset_Nb_luminances, Offset_Nb_vecteurs;
    int Numero, groupe, codmatface;
    std::vector<float> vecteur3D;
    std::vector<int> F_sommets, L_sommets;

    Object *Objet_courant, *Objet_base;
    Face    NewFace;
    Face   *Facette_courante;

    BddInter *Element = MAIN->Element;

    New_Nb_sommets = New_Nb_facettes = New_Nb_normales = New_Nb_aspects = New_Nb_luminances = New_Nb_vecteurs = 0;

    auto it = Element->listeObjets.end(); it--; // Pointer le dernier objet de la liste (c'est le premier séletionné et c'est là qu'on fusionne les autres)

    Offset_Nb_sommets = Offset_Nb_facettes = Offset_Nb_normales = Offset_Nb_aspects = Offset_Nb_luminances = Offset_Nb_vecteurs = 0;

    printf("Fusion des objets suivants dans %s :\n",Element->Objetlist[*it].GetName());
    for (o=0; o<Element->listeObjets.size(); ++o, it--) {
        Objet_courant = &(Element->Objetlist[*it]);
        printf("Objet %d, numero %d, %s\n",o,Objet_courant->GetValue(),Objet_courant->GetName());
        if (o == 0) {
            New_Nb_sommets    = Objet_courant->Nb_sommets;
            New_Nb_facettes   = Objet_courant->Nb_facettes;
            New_Nb_normales   = Objet_courant->Nb_normales;
            New_Nb_aspects    = Objet_courant->Nb_aspects;
            New_Nb_luminances = Objet_courant->Nb_luminances;
            New_Nb_vecteurs   = Objet_courant->Nb_vecteurs;
            Objet_base = Objet_courant;
        } else {
            New_Nb_sommets   += Objet_courant->Nb_sommets;
            New_Nb_facettes  += Objet_courant->Nb_facettes;
            New_Nb_normales  += Objet_courant->Nb_normales;
            New_Nb_aspects   += Objet_courant->Nb_aspects;
            New_Nb_luminances+= Objet_courant->Nb_luminances;
            New_Nb_vecteurs  += Objet_courant->Nb_vecteurs;

            Objet_base->Sommetlist.resize(New_Nb_sommets);
            for (k=0; k<Objet_courant->Nb_sommets; k++) {
                vecteur3D = Objet_courant->Sommetlist[k].getPoint();
                Numero    = k+1+Offset_Nb_sommets;
                Objet_base->Sommetlist[Numero-1] = Sommet(Numero, vecteur3D);
            }
            Objet_base->Nb_sommets = New_Nb_sommets;

            Objet_base->Vecteurlist.resize(New_Nb_vecteurs);
            for (k=0; k<Objet_courant->Nb_vecteurs; k++) {
                vecteur3D = Objet_courant->Vecteurlist[k].getPoint();
                Numero    = k+1+Offset_Nb_vecteurs;
                Objet_base->Vecteurlist[Numero-1] = Vecteur(Numero, vecteur3D);
            }
            Objet_base->Nb_vecteurs = New_Nb_vecteurs;

            Objet_base->Facelist.resize(New_Nb_facettes);
            for (k=0; k<Objet_courant->Nb_facettes; k++) {
                Facette_courante = &(Objet_courant->Facelist[k]);
                F_sommets  = Facette_courante->getF_sommets();
                L_sommets  = Facette_courante->getL_sommets();
                vecteur3D  = Facette_courante->getNormale_b();
                groupe     = Facette_courante->getGroupe();
                codmatface = Facette_courante->getCodmatface();
                Nb_valeurs = F_sommets.size();
                for (j=0; j<Nb_valeurs; j++) F_sommets[j] += Offset_Nb_sommets;
                Nb_valeurs = L_sommets.size();
                for (j=0; j<Nb_valeurs; j++) L_sommets[j] += Offset_Nb_vecteurs;

                Numero  = k+1+Offset_Nb_facettes;
                NewFace = Face(Numero,F_sommets);
                NewFace.setNormale_b(vecteur3D);
                NewFace.setGroupe(groupe);
                NewFace.setCodmatface(codmatface);
                NewFace.flat = Facette_courante->flat;  // Récupérer le booléen flat de la facette courante et le recopier dans la nouvelle facette
                NewFace.setLsommet(L_sommets);
                Objet_base->Facelist[Numero-1] = NewFace;
            }
            Objet_base->Nb_facettes  = New_Nb_facettes;
            Objet_base->Nb_normales  = New_Nb_normales;
            Objet_base->Nb_aspects   = New_Nb_aspects;
            Objet_base->Nb_luminances= New_Nb_luminances;

            if(!(Objet_courant->flat)) Objet_base->flat = false; // Au moins 1 des objets fusionnés n'est pas plat -> l'objet fusionné ne l'est pas non plus, du moins globalement

            Objet_courant->deleted   = true;    // Problème : un undelete peut le restituer. Voir comment le supprimer réellement de la mémoire
            Objet_courant->afficher  = false;   // ... pas suffisant
            Objet_courant->selected  = false;
        }
        Offset_Nb_sommets    = Objet_base->Nb_sommets;      // Conserver les offsets pour la fusion d'objet suivante
        Offset_Nb_facettes   = Objet_base->Nb_facettes;
        Offset_Nb_normales   = Objet_base->Nb_normales;
        Offset_Nb_aspects    = Objet_base->Nb_aspects;
        Offset_Nb_luminances = Objet_base->Nb_luminances;
        Offset_Nb_vecteurs   = Objet_base->Nb_vecteurs;
    }

    while (Element->listeObjets.size() > 1) {   // On ne garde que le dernier élément de la liste, soit objet_base (car la liste des Objets est construite par des push_front)
        it = Element->listeObjets.begin();
        Element->listeObjets.erase(it);
    }
    wxString str_loc;
    str_loc.clear();
    str_loc.Printf(_T("%d"),(int)Element->listeObjets.size());
    TextCtrl_Selection->SetValue(str_loc);

    Element->m_gllist = 0;
    Element->Refresh();
    Element->bdd_modifiee = true;
    Button_Fusionner->Disable();

    ToDo();
}

void SelectionPanel::ToDo()
{
    wxMessageDialog *query = new wxMessageDialog(NULL, _T("Pas encore complètement opérationnel...\nSimulation interface OK"), _T("Avertissement"),
                                                 wxOK | wxICON_INFORMATION ); // Avec l'icône wxICON_QUESTION, l'affichage reste silencieux (wxICON_INFORMATION + logique, mais bruyant !!)
    query->ShowModal();
    query->Destroy();
}
