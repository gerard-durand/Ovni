#include "CouleursGroupes.h"

//(*InternalHeaders(CouleursGroupes)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(CouleursGroupes)
const long CouleursGroupes::ID_STATICTEXT1 = wxNewId();
const long CouleursGroupes::ID_STATICLINE1 = wxNewId();
const long CouleursGroupes::ID_STATICTEXT2 = wxNewId();
const long CouleursGroupes::ID_SPINCTRL1 = wxNewId();
const long CouleursGroupes::ID_STATICTEXT3 = wxNewId();
const long CouleursGroupes::ID_TEXTCTRL1 = wxNewId();
const long CouleursGroupes::ID_STATICTEXT4 = wxNewId();
const long CouleursGroupes::ID_STATICTEXT5 = wxNewId();
const long CouleursGroupes::ID_COLOURPICKERCTRL1 = wxNewId();
const long CouleursGroupes::ID_COLOURPICKERCTRL2 = wxNewId();
const long CouleursGroupes::ID_BUTTON1 = wxNewId();
const long CouleursGroupes::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CouleursGroupes,wxDialog)
	//(*EventTable(CouleursGroupes)
	//*)
END_EVENT_TABLE()

CouleursGroupes::CouleursGroupes(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(CouleursGroupes)
	Create(parent, id, _T("Couleurs des Groupes et Matériaux"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(248,166));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Modification des couleurs"), wxPoint(0,4), wxSize(272,16), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(0,24), wxSize(304,1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Numéro de groupe :"), wxPoint(16,32), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	SpinCtrl_Groupes = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(144,32), wxSize(90,23), 0, -1, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupes->SetValue(_T("0"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Numéro de matériau :"), wxPoint(16,64), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_Materiau = new wxTextCtrl(this, ID_TEXTCTRL1, _T("Aucun"), wxPoint(144,64), wxSize(88,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl_Materiau->Disable();
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Lumière Ambiante"), wxPoint(16,96), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Lumière Diffuse"), wxPoint(136,96), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	ColourPickerCtrl_Ambient = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL1, wxColour(0,0,0), wxPoint(16,112), wxSize(104,25), 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL1"));
	ColourPickerCtrl_Diffuse = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL2, wxColour(0,0,0), wxPoint(128,112), wxSize(104,25), 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL2"));
	Button_Reset = new wxButton(this, ID_BUTTON1, _T("Reset"), wxPoint(16,136), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Reset->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON3, _T("Quitter"), wxPoint(128,136), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&CouleursGroupes::OnSpinCtrl_GroupesChange);
	Connect(ID_COLOURPICKERCTRL1,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&CouleursGroupes::OnColourPickerCtrl_AmbientColourChanged);
	Connect(ID_COLOURPICKERCTRL2,wxEVT_COMMAND_COLOURPICKER_CHANGED,(wxObjectEventFunction)&CouleursGroupes::OnColourPickerCtrl_DiffuseColourChanged);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CouleursGroupes::OnButton_ResetClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CouleursGroupes::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&CouleursGroupes::OnClose);
	//*)
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
    // Ci-dessous, mis explicitement car ne fonctionne pas dans wxColourPickerCtrl sous wxWidgets 2.8.12 et à condition de mettre wxDefaultSize ou wxSize(104,25)
    ColourPickerCtrl_Ambient->SetSize(104,24);
    ColourPickerCtrl_Diffuse->SetSize(104,24);      // Semble OK avec wxWidgets 3.1.1 (encore que !!!) ou 3.1.2 (là c'est toujours OK)
}

CouleursGroupes::~CouleursGroupes()
{
	//(*Destroy(CouleursGroupes)
	//*)
}

void CouleursGroupes::OnClose(wxCloseEvent& event)
{
    Hide();
}

void CouleursGroupes::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void CouleursGroupes::OnSpinCtrl_GroupesChange(wxSpinEvent& event)
{
    BddInter* Element = MAIN->Element;

    Numero_Groupe = SpinCtrl_Groupes->GetValue();
    if (Numero_Groupe >= Element->nb_couleurs) {        // Pour reboucler à 0 si on dépasse le max
        Numero_Groupe = 0;
        SpinCtrl_Groupes->SetValue(Numero_Groupe);
    }
    if (Numero_Groupe < 0) {                            // Pour reboucler au numéro de groupe max si devient < 0
        Numero_Groupe = Element->nb_couleurs-1;
        SpinCtrl_Groupes->SetValue(Numero_Groupe);
    }
    wxColor col;
    col.Set(lround(Element->MatAmbient_avionG[Numero_Groupe][0]*255),
            lround(Element->MatAmbient_avionG[Numero_Groupe][1]*255),
            lround(Element->MatAmbient_avionG[Numero_Groupe][2]*255));
    ColourPickerCtrl_Ambient->SetColour(col);                           // Afficher la couleur dans le bouton
    col.Set(lround(Element->MatDiffuse_avionG[Numero_Groupe][0]*255),
            lround(Element->MatDiffuse_avionG[Numero_Groupe][1]*255),
            lround(Element->MatDiffuse_avionG[Numero_Groupe][2]*255));
    ColourPickerCtrl_Diffuse->SetColour(col);                           // Afficher la couleur dans le bouton

// NOTE : changer aussi le texte du numéro de matériau (intérêt ?)

    int nb_mat;
    nb_mat = Element->listeMateriaux.size();
    TextCtrl_Materiau->SetLabel(_T("Aucun"));
    if ((Numero_Groupe <= nb_mat) && (Numero_Groupe != 0)) {
        wxString label;
        auto it= Element->listeMateriaux.begin();
        it--;
        for (int i=0; i<Numero_Groupe; i++,it++);
        label.Printf(_T("%d"),(int)*it);
        TextCtrl_Materiau->SetLabel(label);
    }
}

void CouleursGroupes::OnColourPickerCtrl_AmbientColourChanged(wxColourPickerEvent& event)
{
    BddInter* Element = MAIN->Element;

    wxColor col = ColourPickerCtrl_Ambient->GetColour();
//    printf("%d %d %d\n",col.Red(),col.Green(),col.Blue());
    Element->MatAmbient_avionG[Numero_Groupe][0] = col.Red()  /255.;
    Element->MatAmbient_avionG[Numero_Groupe][1] = col.Green()/255.;
    Element->MatAmbient_avionG[Numero_Groupe][2] = col.Blue() /255.;
    Element->pal_file_modified = true;              // Marquer le fichier palette comme modifié

    if (Element->groupes || Element->materials) {
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void CouleursGroupes::OnColourPickerCtrl_DiffuseColourChanged(wxColourPickerEvent& event)
{
    BddInter* Element = MAIN->Element;

    wxColor col = ColourPickerCtrl_Diffuse->GetColour();
    Element->MatDiffuse_avionG[Numero_Groupe][0] = col.Red()  /255.;
    Element->MatDiffuse_avionG[Numero_Groupe][1] = col.Green()/255.;
    Element->MatDiffuse_avionG[Numero_Groupe][2] = col.Blue() /255.;
    Element->pal_file_modified = true;              // Marquer le fichier palette comme modifié

    if (Element->groupes || Element->materials) {
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void CouleursGroupes::OnButton_ResetClick(wxCommandEvent& event)
{
// Reset aux couleurs par défaut de palette (<=> relire default.pal)
    BddInter* Element = MAIN->Element;

    for (int j=0; j<4; j++) {
        for (int i=0; i< Element->nb_couleurs; i++) {
            Element->MatAmbient_avionG[i][j] = Element->MatAmbient_avionG_def[i][j];
            Element->MatDiffuse_avionG[i][j] = Element->MatDiffuse_avionG_def[i][j];
        }
    }
    wxSpinEvent cmd_event;                      // Créer en évênement de type spinbutton
    OnSpinCtrl_GroupesChange(cmd_event);        // Simule un changement de numéro de groupe => réactualise les couleurs dans les wxColourPickerCtrl
    if (Element->groupes || Element->materials) {
        Element->m_gllist = 0;
        Element->Refresh();
    }
    Element->pal_file_modified = false;         // Marquer le fichier palette comme non modifié
}
