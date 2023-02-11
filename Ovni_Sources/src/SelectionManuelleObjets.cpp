#include "SelectionManuelleObjets.h"

//(*InternalHeaders(SelectionManuelleObjets)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(SelectionManuelleObjets)
const long SelectionManuelleObjets::ID_CHECKLISTBOX1 = wxNewId();
const long SelectionManuelleObjets::ID_BUTTON1 = wxNewId();
const long SelectionManuelleObjets::ID_BUTTON2 = wxNewId();
const long SelectionManuelleObjets::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SelectionManuelleObjets,wxDialog)
	//(*EventTable(SelectionManuelleObjets)
	//*)
END_EVENT_TABLE()

SelectionManuelleObjets::SelectionManuelleObjets(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SelectionManuelleObjets)
	Create(parent, id, _T("Choix des objets à sélectionner"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(240,232));
	Move(wxDefaultPosition);
	CheckListBox1 = new wxCheckListBox(this, ID_CHECKLISTBOX1, wxPoint(16,16), wxSize(208,176), 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
	CheckListBox1->Append(_T("Objet1"));
	Button_Aucun = new wxButton(this, ID_BUTTON1, _T("Aucun"), wxPoint(16,200), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Aucun->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Tous = new wxButton(this, ID_BUTTON2, _T("Tous"), wxPoint(88,200), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Tous->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON3, _T("Quitter"), wxPoint(160,200), wxSize(64,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_CHECKLISTBOX1,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,(wxObjectEventFunction)&SelectionManuelleObjets::OnCheckListBox1Toggled);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleObjets::OnButton_AucunClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleObjets::OnButton_TousClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleObjets::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SelectionManuelleObjets::OnClose);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

SelectionManuelleObjets::~SelectionManuelleObjets()
{
	//(*Destroy(SelectionManuelleObjets)
	//*)
}


void SelectionManuelleObjets::OnClose(wxCloseEvent& event)
{
    Hide();
}

void SelectionManuelleObjets::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void SelectionManuelleObjets::OnButton_AucunClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int n = CheckListBox1->GetCount();
    unsigned int indice_objet = 0;
    Element->listeObjets.clear();
    for (int i=0; i<n; i++) {
        CheckListBox1->Check(i,false);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;
        Element->Objetlist[indice_objet].selected = false;
        indice_objet++;
    }
    Element->SelectionObjet = 0;

// Il faut aussi mettre à jour le contenu des items dans SelectionPanel

    wxString str_loc;
    str_loc.Printf(_T(""));
    MAIN->Selections_Panel->TextCtrl_Selection->SetValue(str_loc);
    MAIN->Selections_Panel->TextCtrl_NomObjet ->ChangeValue(_T(""));    //ChangeValue plutôt que SetValue pour éviter la génération d'un event de texte
    MAIN->Selections_Panel->TextCtrl_NumObjet ->SetValue(_T(""));
    MAIN->Selections_Panel->Button_Fusionner  ->Disable();

    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionManuelleObjets::OnButton_TousClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool chkB;
    int chk_compteur=0;

    int n = CheckListBox1->GetCount();
    unsigned int indice_objet = 0;  // Pour découpler l'indice d'objet avec celui de la CheckBoxList (en cas d'objets supprimés)
    Element->listeObjets.clear();
    Element->SelectionObjet = 0;
    for (int i=0; i<n; i++) {
        CheckListBox1->Check(i);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;
        Element->Objetlist[indice_objet].selected = true;
        Element->listeObjets.push_back(i);
        indice_objet++;
        chk_compteur++;
    }
    Element->SelectionObjet = -1;

    wxString str_loc;
    str_loc.Printf(_T("%d"),n);
    MAIN->Selections_Panel->TextCtrl_Selection->SetValue(str_loc);
    MAIN->Selections_Panel->TextCtrl_NomObjet ->ChangeValue(_T("Sélection multiple"));
    MAIN->Selections_Panel->TextCtrl_NumObjet ->SetValue(_T(""));
    if (chk_compteur > 1)
         MAIN->Selections_Panel->Button_Fusionner->Enable();
    else MAIN->Selections_Panel->Button_Fusionner->Disable();

    Element->m_gllist = 0;
    Element->Refresh();
}

void SelectionManuelleObjets::OnCheckListBox1Toggled(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool chkB;

    int n = CheckListBox1->GetCount();
    int compteur = 0;
    unsigned int indice_objet = 0;  // Pour découpler l'indice d'objet avec celui de la CheckBoxList (en cas d'objets supprimés)
    Element->listeObjets.clear();
    for (int i=0; i<n; i++) {
        chkB = CheckListBox1->IsChecked(i);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;    // Passer les objets supprimés (incrémenter l'indice)
        Element->Objetlist[indice_objet].selected = chkB;
        if (chkB) {
            Element->SelectionObjet=indice_objet;
            Element->listeObjets.push_back(indice_objet);
            compteur++;
        }
        indice_objet++; // Peut-être vérifier que l'indice est <= Objetlist.size ? encore plus dans le while ... au cas où !!!
    }

    wxString str_loc;
    str_loc.Printf(_T("%d"),compteur);
    if (compteur == 0) {
        MAIN->Selections_Panel->TextCtrl_Selection->SetValue(_T(""));
        MAIN->Selections_Panel->TextCtrl_NomObjet ->ChangeValue(_T(""));
        MAIN->Selections_Panel->TextCtrl_NumObjet ->SetValue(_T(""));
        MAIN->Selections_Panel->Button_Fusionner  ->Disable();
    } else {
        MAIN->Selections_Panel->TextCtrl_Selection->SetValue(str_loc);
        if (compteur > 1) {
            MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(_T("Sélection multiple"));
            MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(_T(""));
            MAIN->Selections_Panel->Button_Fusionner->Enable();
        } else {
            str_loc = wxString(Element->Objetlist[Element->SelectionObjet].GetName(), wxConvUTF8);
            MAIN->Selections_Panel->TextCtrl_NomObjet ->ChangeValue(str_loc);
            str_loc.Printf(_T("%d"),Element->SelectionObjet);
            MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(str_loc);
            MAIN->Selections_Panel->Button_Fusionner ->Disable();
        }
    }
    if (compteur == 0)
        Element->SelectionObjet =  0;
    else
        Element->SelectionObjet = -1;

    Element->m_gllist = 0;
    Element->Refresh();
}
