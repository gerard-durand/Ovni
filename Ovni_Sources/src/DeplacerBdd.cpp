#include "DeplacerBdd.h"

//(*InternalHeaders(DeplacerBdd)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(DeplacerBdd)
const long DeplacerBdd::ID_STATICTEXT1 = wxNewId();
const long DeplacerBdd::ID_STATICLINE1 = wxNewId();
const long DeplacerBdd::ID_STATICTEXT2 = wxNewId();
const long DeplacerBdd::ID_STATICTEXT3 = wxNewId();
const long DeplacerBdd::ID_STATICTEXT4 = wxNewId();
const long DeplacerBdd::ID_TEXTCTRL1 = wxNewId();
const long DeplacerBdd::ID_TEXTCTRL2 = wxNewId();
const long DeplacerBdd::ID_TEXTCTRL3 = wxNewId();
const long DeplacerBdd::ID_BUTTON1 = wxNewId();
const long DeplacerBdd::ID_BUTTON2 = wxNewId();
const long DeplacerBdd::ID_BUTTON3 = wxNewId();
const long DeplacerBdd::ID_BUTTON4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(DeplacerBdd,wxDialog)
	//(*EventTable(DeplacerBdd)
	//*)
END_EVENT_TABLE()

DeplacerBdd::DeplacerBdd(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(DeplacerBdd)
	Create(parent, id, _T("Déplacer la Bdd"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(256,199));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Déplacement de la Base de Données"), wxPoint(24,4), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(0,24), wxSize(384,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticLine1->Disable();
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Delta X :"), wxPoint(24,44), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText2->SetForegroundColour(wxColour(255,0,0));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Delta Y :"), wxPoint(24,76), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText3->SetForegroundColour(wxColour(0,220,0));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Delta Z :"), wxPoint(24,108), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText4->SetForegroundColour(wxColour(0,0,255));
	TextCtrl_DeplX = new wxTextCtrl(this, ID_TEXTCTRL1, _T("0"), wxPoint(96,40), wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl_DeplY = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0"), wxPoint(96,72), wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextCtrl_DeplZ = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0"), wxPoint(96,104), wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	Button_Centrer = new wxButton(this, ID_BUTTON1, _T("Centrage Auto sur Min-Max de la Bdd"), wxPoint(8,136), wxSize(240,26), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK = new wxButton(this, ID_BUTTON2, _T("Accepter"), wxPoint(8,168), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Inverser = new wxButton(this, ID_BUTTON3, _T("Inverser"), wxPoint(88,168), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quitter = new wxButton(this, ID_BUTTON4, _T("Quitter"), wxPoint(168,168), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DeplacerBdd::OnButton_CentrerClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DeplacerBdd::OnButton_OKClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DeplacerBdd::OnButton_InverserClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&DeplacerBdd::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&DeplacerBdd::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&DeplacerBdd::OnClose);
	//*)
//	this->Deplacer_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

DeplacerBdd::~DeplacerBdd()
{
	//(*Destroy(DeplacerBdd)
	//*)
}

void DeplacerBdd::OnInit(wxInitDialogEvent& event)
{
}

void DeplacerBdd::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void DeplacerBdd::OnClose(wxCloseEvent& event)
{
    Hide();
}

void DeplacerBdd::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    float dx,dy,dz;
    unsigned int o,i;
    Object *  objet_courant;
    Sommet1 * sommet_i;

    dx = wxAtof(TextCtrl_DeplX->GetValue());
    dy = wxAtof(TextCtrl_DeplY->GetValue());
    dz = wxAtof(TextCtrl_DeplZ->GetValue());
//    OvniFrame* MAIN=dynamic_cast<OvniFrame*>(Deplacer_Parent);
    for (o=0; o<Element->Objetlist.size(); o++) {
        objet_courant = &(Element->Objetlist[o]);
        for (i=0; i<objet_courant->Nb_sommets; i++) {
            sommet_i = &(objet_courant->Sommetlist[i]);
            if (sommet_i->point.empty()) continue;      // Ne pas tenir compte de points vides (non utilisés a priori !)
            sommet_i->point[0] += dx;
            sommet_i->point[1] += dy;
            sommet_i->point[2] += dz;
        }
    }
    Element->centreRot[0] += dx; Element->centreRot[1] += dy; Element->centreRot[2] += dz; // centre_auto mis à jour dans searchMin_Max

    Element->m_gllist = 0;
    Element->searchMin_Max();
    Element->SetPosObs(true);
    Element->ResetProjectionMode();
    Element->bdd_modifiee = true;
    Element->Refresh();
}

void DeplacerBdd::OnButton_InverserClick(wxCommandEvent& event)
{
    float val;
    wxString str;

    val = -wxAtof(TextCtrl_DeplX->GetValue());  // Récupérer le Delta X et le changer de signe
    str.Printf(_T("%5.3f"),val);                // Formater la nouvelle valeur en texte
    TextCtrl_DeplX->SetValue(str);              // Mettre à jour l'affichage

    val = -wxAtof(TextCtrl_DeplY->GetValue());  // Idem en Y
    str.Printf(_T("%5.3f"),val);
    TextCtrl_DeplY->SetValue(str);

    val = -wxAtof(TextCtrl_DeplZ->GetValue());  // Idem en Z
    str.Printf(_T("%5.3f"),val);
    TextCtrl_DeplZ->SetValue(str);

    OnButton_OKClick(event);                    // Lancer OKClick avec ces 3 nouvelles valeurs
}

void DeplacerBdd::OnButton_CentrerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

/// Ne fonctionne pas toujours bien ... revoir l'ordre des opérations peut-être ...
/// + souci avec zNear zFar ... (modifiés dans SetPosObs et utilisés dans ResetProjectionMode)

    Element->centrageRotAuto = true;
    Element->m_gllist = 0;
    Element->searchMin_Max();
    Element->SetPosObs(true);
    Element->ResetProjectionMode();
    Element->Refresh();
}
