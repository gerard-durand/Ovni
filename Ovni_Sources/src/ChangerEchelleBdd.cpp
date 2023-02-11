#include "ChangerEchelleBdd.h"

//(*InternalHeaders(ChangerEchelleBdd)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ChangerEchelleBdd)
const long ChangerEchelleBdd::ID_STATICTEXT1 = wxNewId();
const long ChangerEchelleBdd::ID_STATICLINE1 = wxNewId();
const long ChangerEchelleBdd::ID_STATICTEXT2 = wxNewId();
const long ChangerEchelleBdd::ID_TEXTCTRL1 = wxNewId();
const long ChangerEchelleBdd::ID_STATICLINE2 = wxNewId();
const long ChangerEchelleBdd::ID_BUTTON1 = wxNewId();
const long ChangerEchelleBdd::ID_BUTTON2 = wxNewId();
const long ChangerEchelleBdd::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ChangerEchelleBdd,wxDialog)
	//(*EventTable(ChangerEchelleBdd)
	//*)
END_EVENT_TABLE()

ChangerEchelleBdd::ChangerEchelleBdd(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief ChangerEchelleBdd::ChangerEchelleBdd Gestion du changement d'échelle de la base de données 3D
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(ChangerEchelleBdd)
	Create(parent, id, _T("Changer l\'échelle de la Bdd"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(304,119));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Changement d\'échelle de la Base de Données"), wxPoint(32,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(0,32), wxSize(384,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Facteur à appliquer à la Bdd"), wxPoint(8,52), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_scale = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1"), wxPoint(184,48), wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxPoint(0,80), wxSize(400,2), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(8,88), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Inverser = new wxButton(this, ID_BUTTON2, _T("Inverser"), wxPoint(104,88), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Inverser->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON3, _T("Quitter"), wxPoint(200,88), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChangerEchelleBdd::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChangerEchelleBdd::OnButton_InverserClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ChangerEchelleBdd::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ChangerEchelleBdd::OnClose);
	//*)
//	this->ChangerEchelleBdd_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ChangerEchelleBdd::~ChangerEchelleBdd()
{
	//(*Destroy(ChangerEchelleBdd)
	//*)
}

void ChangerEchelleBdd::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ChangerEchelleBdd::OnClose(wxCloseEvent& event)
{
    Hide();
}

void ChangerEchelleBdd::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    float val;
    unsigned int o,i,j;
    Object * objet_courant;

    val = wxAtof(TextCtrl_scale->GetValue());                                               // Récupérer le facteur d'échelle
    for (o=0; o<Element->Objetlist.size(); o++) {
        objet_courant = &(Element->Objetlist[o]);
        unsigned int nbsom = objet_courant->Nb_sommets;
#pragma omp parallel for
        for (i=0;i<nbsom;i++) {                                                             // L'appliquer sur les 3 coordonnées des sommets
            if (objet_courant->Sommetlist[i].point.empty()) continue;                       // Ne pas tenir compte de points vides (non utilisés a priori !)
            for (j=0;j<3;j++) objet_courant->Sommetlist[i].point[j] *= val;
        }
    }
    for (j=0; j<3; j++) Element->centreRot[j] *= val;                                       // centre_auto mis à jour dans searchMin_Max;

    Element->m_gllist = 0;
    Element->searchMin_Max();
    Element->bdd_modifiee = true;
    Element->Refresh();
    if (MAIN->Properties_Panel->IsShown()) MAIN->OvniFrame::Redisplay_Proprietes(event);    // On met à jour la fenètre "Propriétés" si elle est affichée
}

void ChangerEchelleBdd::OnButton_InverserClick(wxCommandEvent& event)
{
    float val;
    wxString str;

    val = wxAtof(TextCtrl_scale->GetValue());                                               // Récupérer le facteur d'échelle
    if (val != 0.0) {                                                                       // L'inverser s'il n'est pas nul !
        val = 1.0/val;
        str.Printf(_T("%5.2f"),val);
        TextCtrl_scale->SetValue(str);                                                      // Modifier l'affichage
        OnButton_OKClick(event);                                                            // Lancer OKClick sur cette nouvelle valeur
    }
}
