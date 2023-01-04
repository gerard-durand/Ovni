#include "ReperageObjet.h"

//(*InternalHeaders(ReperageObjet)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//#include "OvniMain.h"

//(*IdInit(ReperageObjet)
const long ReperageObjet::ID_STATICTEXT1 = wxNewId();
const long ReperageObjet::ID_TEXTCTRL1 = wxNewId();
const long ReperageObjet::ID_SPINBUTTON1 = wxNewId();
const long ReperageObjet::ID_TEXTCTRL3 = wxNewId();
const long ReperageObjet::ID_TEXTCTRL2 = wxNewId();
const long ReperageObjet::ID_STATICTEXT2 = wxNewId();
const long ReperageObjet::ID_BUTTON1 = wxNewId();
const long ReperageObjet::ID_CHECKBOX1 = wxNewId();
const long ReperageObjet::ID_CHECKBOX2 = wxNewId();
const long ReperageObjet::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ReperageObjet,wxDialog)
	//(*EventTable(ReperageObjet)
	//*)
END_EVENT_TABLE()

ReperageObjet::ReperageObjet(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ReperageObjet)
	Create(parent, id, _T("Repérage d\'un objet"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(308,144));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Objet indice/numéro :"), wxPoint(8,12), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_NumObjet = new wxTextCtrl(this, ID_TEXTCTRL1, _T("Non sélectionné"), wxPoint(128,8), wxSize(104,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	SpinButton_indice = new wxSpinButton(this, ID_SPINBUTTON1, wxPoint(232,8), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON1"));
	SpinButton_indice->SetValue(-1);
	SpinButton_indice->SetRange(-1, 100);
	SpinButton_indice->SetFocus();
	TextCtrl_indice = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0"), wxPoint(248,8), wxSize(48,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextCtrl_indice->Disable();
	TextCtrl_NomObjet = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxPoint(40,40), wxSize(256,21), wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Nom"), wxPoint(8,44), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("OK"), wxPoint(160,112), wxSize(136,26), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	CheckBox_masquer = new wxCheckBox(this, ID_CHECKBOX1, _T("Masquer l\'objet"), wxPoint(8,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_masquer->SetValue(false);
	CheckBox_supprimer = new wxCheckBox(this, ID_CHECKBOX2, _T("Supprimer l\'objet"), wxPoint(8,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_supprimer->SetValue(false);
	Button_InverserNormales = new wxButton(this, ID_BUTTON2, _T("Inverser les normales"), wxPoint(8,112), wxSize(136,26), 0, wxDefaultValidator, _T("ID_BUTTON2"));

	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&ReperageObjet::OnSpinButton_indiceChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageObjet::OnButton_OKClick);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ReperageObjet::OnCheckBox_masquerClick);
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ReperageObjet::OnCheckBox_supprimerClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ReperageObjet::OnButton_InverserNormalesClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ReperageObjet::OnClose);
	//*)
//	this->Obj_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ReperageObjet::~ReperageObjet()
{
	//(*Destroy(ReperageObjet)
	//*)
}

void ReperageObjet::OnSpinButton_indiceChange(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString str1,str2,str3;
    bool chkB;

    int num = SpinButton_indice->GetValue();
    if (num >= (int)Element->Objetlist.size()) {
        num = -1;                                   // Bouclage sur "Non sélectionné"
        SpinButton_indice->SetValue(num);
    }
    Element->SelectionObjet = num+1;                // 0 si aucun objet sélectionné
    if (num == -1) {
        str1.Printf(_T("Non sélectionné"));
        str2.Printf(_T(""));
        str3 = str2;
        TextCtrl_indice   ->SetValue(str2);         // Vide dans ce cas (pour ne pas mettre -1 !)
        CheckBox_masquer  ->SetValue(false);        // Désactiver les fonctions
        CheckBox_masquer  ->Disable();
        CheckBox_supprimer->SetValue(false);
        CheckBox_supprimer->Disable();
        Button_InverserNormales->Disable();
    } else {
        str1.Printf(_T("%d"),num);
        str3 = str1;
        TextCtrl_indice->SetValue(str1);
        int num_obj = Element->Objetlist[num].GetValue();
        str1.Printf(_T("%d"),num_obj);
        str2 =  wxString(Element->Objetlist[num].GetName(), wxConvUTF8);
        chkB = !Element->Objetlist[num].afficher;
        CheckBox_masquer  ->SetValue(chkB);
        CheckBox_masquer  ->Enable();
        chkB = Element    ->Objetlist[num].deleted;
        CheckBox_supprimer->SetValue(chkB);
        CheckBox_supprimer->Enable();
        Button_InverserNormales->Enable();
        if (chkB) CheckBox_masquer->Disable();  // Invalider la case masquer puisque l'objet est supprimé
    }
    TextCtrl_NumObjet->SetValue(str1);
    TextCtrl_NomObjet->SetValue(str2);
    if (MAIN->Selections_Panel->IsShown()) {
        MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(str2);
        if (num == -1) str1 = str2;
        MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(str1);  // ici c'est le numéro
        MAIN->Selections_Panel->TextCtrl_IndObjet->SetValue(str3);  // et là, l'indice de l'objet
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void ReperageObjet::OnCheckBox_masquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int num = SpinButton_indice->GetValue();
    if (num >= 0) {     // Ce devrait toujours être le cas car si -1, la case "Masquer" est désactivée
        bool chkB = !CheckBox_masquer->GetValue();
        Element->Objetlist[num].afficher = chkB;
        if (chkB) Button_InverserNormales->Enable();
        else      Button_InverserNormales->Disable();
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageObjet::OnCheckBox_supprimerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int num=SpinButton_indice->GetValue();
    if (num >= 0) {     // Ce devrait toujours être le cas car si -1, la case "Supprimer" est désactivée
        bool chkB = CheckBox_supprimer->GetValue();
        Element->Objetlist[num].deleted = chkB;
        if (chkB) {
            CheckBox_masquer->Disable();
            Button_InverserNormales->Disable();
        } else {
            CheckBox_masquer->Enable() ;
            if (CheckBox_masquer->GetValue())
                Button_InverserNormales->Disable();
            else
                Button_InverserNormales->Enable();
        }
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void ReperageObjet::OnButton_OKClick(wxCommandEvent& event)
{
// Boutton OK <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);

}

void ReperageObjet::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->SelectionObjet = 0;    // Raz de sélection d'objet en sortie
    for (unsigned int i=0; i<Element->Objetlist.size(); i++) Element->Objetlist[i].selected = false;
    Element->m_gllist = 0;
    Element->Refresh();
    Hide();
}

void ReperageObjet::OnButton_InverserNormalesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    int indiceObjet = SpinButton_indice->GetValue();
    if (indiceObjet >= 0) {     // Ce devrait toujours être le cas car si -1, le bouton "Inverser" est désactivé
        Element->Inverser_les_Normales_Objet(indiceObjet);
        Element->m_gllist = 0;
        Element->Refresh();
    }
}
