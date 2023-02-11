#include "ChoixAffichageObjets.h"

//(*InternalHeaders(ChoixAffichageObjets)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ChoixAffichageObjets)
const long ChoixAffichageObjets::ID_STATICTEXT1 = wxNewId();
const long ChoixAffichageObjets::ID_CHECKLISTBOX1 = wxNewId();
const long ChoixAffichageObjets::ID_BUTTON1 = wxNewId();
const long ChoixAffichageObjets::ID_BUTTON2 = wxNewId();
const long ChoixAffichageObjets::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ChoixAffichageObjets,wxDialog)
	//(*EventTable(ChoixAffichageObjets)
	//*)
END_EVENT_TABLE()

ChoixAffichageObjets::ChoixAffichageObjets(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief ChoixAffichageObjets::ChoixAffichageObjets Gestion du choix d'affichage des éléments de la base de données 3D
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(ChoixAffichageObjets)
	Create(parent, id, _T("Choix des objets à afficher"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(269,200));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Choisir les objets à afficher"), wxPoint(64,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	CheckListBox1 = new wxCheckListBox(this, ID_CHECKLISTBOX1, wxPoint(16,32), wxSize(240,128), 0, 0, 0, wxDefaultValidator, _T("ID_CHECKLISTBOX1"));
	CheckListBox1->Check(CheckListBox1->Append(_T("Objet 1")));
	Button_Aucun = new wxButton(this, ID_BUTTON1, _T("Aucun"), wxPoint(16,168), wxSize(80,26), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Aucun->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Tous = new wxButton(this, ID_BUTTON2, _T("Tous"), wxPoint(96,168), wxSize(80,26), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Tous->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON3, _T("Quitter"), wxPoint(176,168), wxSize(80,26), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_CHECKLISTBOX1,wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,(wxObjectEventFunction)&ChoixAffichageObjets::OnCheckListBox1Toggled);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChoixAffichageObjets::OnButton_AucunClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChoixAffichageObjets::OnButton_TousClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChoixAffichageObjets::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&ChoixAffichageObjets::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ChoixAffichageObjets::OnClose);
	//*)
//	this->Chk_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ChoixAffichageObjets::~ChoixAffichageObjets()
{
	//(*Destroy(ChoixAffichageObjets)
	//*)
}


void ChoixAffichageObjets::OnCheckListBox1Toggled(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool chkB;
    int n = CheckListBox1->GetCount();
    unsigned int indice_objet = 0;  // Pour découpler l'indice d'objet avec celui de la CheckBoxList (en cas d'objets supprimés)

    for (int i=0; i<n; i++) {
        chkB = CheckListBox1->IsChecked(i);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;    // Passer les objets supprimés (incrémenter l'indice)
        Element->Objetlist[indice_objet].afficher = chkB;
//        if (Element->Objetlist[i].deleted) {
//            CheckListBox1->Check(i,false); // Décocher la case si l'objet a été supprimé
//        }
        indice_objet++; // Peut-être vérifier que l'indice est <= Objetlist.size ? encore plus dans le while ... au cas où !!!
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void ChoixAffichageObjets::OnButton_AucunClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int n = CheckListBox1->GetCount();
    unsigned int indice_objet = 0;
    for (int i=0; i<n; i++) {
        CheckListBox1->Check(i,false);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;
        Element->Objetlist[indice_objet].afficher = false;
        indice_objet++;
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void ChoixAffichageObjets::OnButton_TousClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int n = CheckListBox1->GetCount();
    unsigned int indice_objet = 0;
    for (int i=0; i<n; i++) {
        CheckListBox1->Check(i);
        while (Element->Objetlist[indice_objet].deleted) indice_objet++;
        Element->Objetlist[indice_objet].afficher = true;
        indice_objet++;
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void ChoixAffichageObjets::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ChoixAffichageObjets::OnInit(wxInitDialogEvent& event)
{
}

void ChoixAffichageObjets::OnClose(wxCloseEvent& event)
{
    Hide();
}
