#include "Prefs_Dialog.h"

//(*InternalHeaders(Prefs_Dialog)
#include <wx/settings.h>
#include <wx/string.h>
//*)

#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

const float to_Deg=180.0f/M_PI;
const float to_Rad=1.0f/to_Deg;

//(*IdInit(Prefs_Dialog)
const long Prefs_Dialog::ID_STATICTEXT1 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE1 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT2 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRLDOUBLE1 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT3 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRLDOUBLE2 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE4 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT4 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRLDOUBLE3 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE5 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX1 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE3 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX8 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX2 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX3 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE2 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX4 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT5 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRLDOUBLE4 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT6 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRLDOUBLE5 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX6 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX5 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX11 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX12 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE6 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT7 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT8 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRL1 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE7 = wxNewId();
const long Prefs_Dialog::ID_RADIOBOX1 = wxNewId();
const long Prefs_Dialog::ID_RADIOBOX2 = wxNewId();
const long Prefs_Dialog::ID_RADIOBOX3 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE8 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX7 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE10 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX9 = wxNewId();
const long Prefs_Dialog::ID_CHECKBOX10 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE11 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT9 = wxNewId();
const long Prefs_Dialog::ID_TEXTCTRL10 = wxNewId();
const long Prefs_Dialog::ID_BUTTON1 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE9 = wxNewId();
const long Prefs_Dialog::ID_STATICTEXT10 = wxNewId();
const long Prefs_Dialog::ID_SPINCTRL2 = wxNewId();
const long Prefs_Dialog::ID_STATICLINE12 = wxNewId();
const long Prefs_Dialog::ID_BUTTON2 = wxNewId();
const long Prefs_Dialog::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Prefs_Dialog,wxDialog)
	//(*EventTable(Prefs_Dialog)
	//*)
END_EVENT_TABLE()

Prefs_Dialog::Prefs_Dialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Prefs_Dialog)
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer3;

	Create(parent, wxID_ANY, _T("Préférences de l\'application"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	Hide();
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Valeurs = coefficient par rapport à la diagonale de la boîte englobante "), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	BoxSizer1->Add(StaticText1, 0, wxALL|wxEXPAND, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	BoxSizer1->Add(StaticLine1, 0, wxALL|wxEXPAND, 0);
	FlexGridSizer1 = new wxFlexGridSizer(2, 2, -2, wxDLG_UNIT(this,wxSize(0,0)).GetWidth());
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Longueur des axes"), wxDefaultPosition, wxSize(260,15), 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlDouble_axes = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE1, _T("0,25"), wxDefaultPosition, wxSize(80,23), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE1"));
	SpinCtrlDouble_axes->SetValue(_T("0,25"));
	FlexGridSizer1->Add(SpinCtrlDouble_axes, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Longueur des normales"), wxDefaultPosition, wxSize(260,16), 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlDouble_norm = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE2, _T("0,05"), wxDefaultPosition, wxSize(80,23), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE2"));
	SpinCtrlDouble_norm->SetValue(_T("0,05"));
	FlexGridSizer1->Add(SpinCtrlDouble_norm, 1, wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 1);
	StaticLine4 = new wxStaticLine(this, ID_STATICLINE4, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE4"));
	BoxSizer1->Add(StaticLine4, 0, wxALL|wxEXPAND, 0);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Rayon de la source"), wxDefaultPosition, wxSize(260,16), 0, _T("ID_STATICTEXT4"));
	BoxSizer4->Add(StaticText4, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlDouble_src = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE3, _T("0,03"), wxDefaultPosition, wxSize(80,23), 0, 0, 1, 0, 0.01, _T("ID_SPINCTRLDOUBLE3"));
	SpinCtrlDouble_src->SetValue(_T("0,03"));
	BoxSizer4->Add(SpinCtrlDouble_src, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxLEFT|wxRIGHT|wxEXPAND, 1);
	StaticLine5 = new wxStaticLine(this, ID_STATICLINE5, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE5"));
	BoxSizer1->Add(StaticLine5, 0, wxALL|wxEXPAND, 0);
	CheckBox_AntialiasingSoft = new wxCheckBox(this, ID_CHECKBOX1, _T("Anti-aliasing géré par OpenGL"), wxDefaultPosition, wxSize(200,20), 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_AntialiasingSoft->SetValue(false);
	BoxSizer1->Add(CheckBox_AntialiasingSoft, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	BoxSizer1->Add(StaticLine3, 0, wxALL|wxEXPAND, 0);
	CheckBox_1SeulObjet3D = new wxCheckBox(this, ID_CHECKBOX8, _T("Forcer la lecture des fichiers .obj dans 1 seul Objet 3D"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
	CheckBox_1SeulObjet3D->SetValue(false);
	BoxSizer1->Add(CheckBox_1SeulObjet3D, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	CheckBox_LectureOptimisee = new wxCheckBox(this, ID_CHECKBOX2, _T("Lecture optimisée des tableaux de points des fichiers de type .obj"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_LectureOptimisee->SetValue(false);
	BoxSizer1->Add(CheckBox_LectureOptimisee, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	CheckBox_TestDecalage3DS = new wxCheckBox(this, ID_CHECKBOX3, _T("Lecture du décalage éventuel des objets dans les fichiers .3ds"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_TestDecalage3DS->SetValue(false);
	BoxSizer1->Add(CheckBox_TestDecalage3DS, 0, wxALL|wxEXPAND, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	BoxSizer1->Add(StaticLine2, 0, wxALL|wxEXPAND, 0);
	CheckBox_Seuillage = new wxCheckBox(this, ID_CHECKBOX4, _T("Calculs et visualisation avec seuillage des normales"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	CheckBox_Seuillage->SetValue(false);
	BoxSizer1->Add(CheckBox_Seuillage, 0, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer3 = new wxFlexGridSizer(2, 2, -2, 0);
	StaticText_Gouraud = new wxStaticText(this, ID_STATICTEXT5, _T("Seuil de comparaison des normales (degrés) :"), wxDefaultPosition, wxSize(260,16), 0, _T("ID_STATICTEXT5"));
	FlexGridSizer3->Add(StaticText_Gouraud, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlDouble_SeuilGouraud = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE4, _T("35,0"), wxDefaultPosition, wxSize(80,23), 0, 0, 90, 0, 0.5, _T("ID_SPINCTRLDOUBLE4"));
	SpinCtrlDouble_SeuilGouraud->SetValue(_T("35,0"));
	FlexGridSizer3->Add(SpinCtrlDouble_SeuilGouraud, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText_Gouraud2 = new wxStaticText(this, ID_STATICTEXT6, _T("Coefficient multiplicateur du 2ème angle :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer3->Add(StaticText_Gouraud2, 0, wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrlDouble_SeuilGouraud2 = new wxSpinCtrlDouble(this, ID_SPINCTRLDOUBLE5, _T("1,6"), wxDefaultPosition, wxSize(80,23), 0, 0, 3, 0, 0.1, _T("ID_SPINCTRLDOUBLE5"));
	SpinCtrlDouble_SeuilGouraud2->SetValue(_T("1,6"));
	FlexGridSizer3->Add(SpinCtrlDouble_SeuilGouraud2, 0, wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer3, 0, wxALL|wxEXPAND, 1);
	CheckBox_RecNormales_Seuillees = new wxCheckBox(this, ID_CHECKBOX6, _T("Enregistrement dans la Bdd des normales aux sommets seuillées"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	CheckBox_RecNormales_Seuillees->SetValue(false);
	CheckBox_RecNormales_Seuillees->Disable();
	BoxSizer1->Add(CheckBox_RecNormales_Seuillees, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	CheckBox_CalculNormales = new wxCheckBox(this, ID_CHECKBOX5, _T("Calcul des normales aux sommets dès la lecture de la Bdd"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	CheckBox_CalculNormales->SetValue(false);
	BoxSizer1->Add(CheckBox_CalculNormales, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	CheckBox_NotFlat = new wxCheckBox(this, ID_CHECKBOX11, _T("Forcer toutes les facettes à être NON planes"), wxPoint(0,0), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX11"));
	CheckBox_NotFlat->SetValue(false);
	BoxSizer1->Add(CheckBox_NotFlat, 0, wxALL, 5);
	CheckBox_TraiterDoublonsAretes = new wxCheckBox(this, ID_CHECKBOX12, _T("Supprimer les doublons d\'arêtes des facettes"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX12"));
	CheckBox_TraiterDoublonsAretes->SetValue(false);
	BoxSizer1->Add(CheckBox_TraiterDoublonsAretes, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5);
	StaticLine6 = new wxStaticLine(this, ID_STATICLINE6, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE6"));
	BoxSizer1->Add(StaticLine6, 0, wxALL|wxEXPAND, 0);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Intervalle de temps des sauvegardes automatiques en minutes :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	BoxSizer1->Add(StaticText7, 0, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("0 <=> Pas de sauvegarde"), wxDefaultPosition, wxSize(260,16), 0, _T("ID_STATICTEXT8"));
	BoxSizer5->Add(StaticText8, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl_PasSvg = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxSize(80,23), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_PasSvg->SetValue(_T("0"));
	SpinCtrl_PasSvg->SetMaxSize(wxSize(78,0));
	BoxSizer5->Add(SpinCtrl_PasSvg, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer5, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine7 = new wxStaticLine(this, ID_STATICLINE7, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE7"));
	BoxSizer1->Add(StaticLine7, 0, wxALL|wxEXPAND, 0);
	wxString __wxRadioBoxChoices_1[3] =
	{
	    _T("Méthode 1     "),
	    _T("Méthode 2     "),
	    _T("Méthode 3     ")
	};
	RadioBox_Triangulation = new wxRadioBox(this, ID_RADIOBOX1, _T("Méthode de triangulation"), wxDefaultPosition, wxSize(382,48), 3, __wxRadioBoxChoices_1, 1, wxRA_SPECIFY_ROWS, wxDefaultValidator, _T("ID_RADIOBOX1"));
	RadioBox_Triangulation->SetSelection(1);
	BoxSizer1->Add(RadioBox_Triangulation, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	wxString __wxRadioBoxChoices_2[2] =
	{
	    _T("Directe            "),
	    _T("Trackball")
	};
	RadioBox_Trackball = new wxRadioBox(this, ID_RADIOBOX2, _T("Mode de rotation à la souris"), wxDefaultPosition, wxSize(382,48), 2, __wxRadioBoxChoices_2, 1, wxRA_SPECIFY_ROWS, wxDefaultValidator, _T("ID_RADIOBOX2"));
	RadioBox_Trackball->SetSelection(1);
	BoxSizer1->Add(RadioBox_Trackball, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	wxString __wxRadioBoxChoices_3[4] =
	{
	    _T("16 x16           "),
	    _T("24 x 24            "),
	    _T("32 x 32            "),
	    _T("48 x 48")
	};
	RadioBox_IconSize = new wxRadioBox(this, ID_RADIOBOX3, _T("Taille des icônes de la barre d\'outils"), wxDefaultPosition, wxSize(382,48), 4, __wxRadioBoxChoices_3, 1, wxRA_SPECIFY_ROWS, wxDefaultValidator, _T("ID_RADIOBOX3"));
	RadioBox_IconSize->SetSelection(0);
	BoxSizer1->Add(RadioBox_IconSize, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine8 = new wxStaticLine(this, ID_STATICLINE8, wxDefaultPosition, wxSize(380,2), wxLI_HORIZONTAL, _T("ID_STATICLINE8"));
	BoxSizer1->Add(StaticLine8, 0, wxALL|wxEXPAND, 0);
	CheckBox_DisplayFps = new wxCheckBox(this, ID_CHECKBOX7, _T("Affichage du nombre de frames OpenGL par secondes (fps)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
	CheckBox_DisplayFps->SetValue(true);
	BoxSizer1->Add(CheckBox_DisplayFps, 0, wxALL|wxEXPAND, 5);
	StaticLine10 = new wxStaticLine(this, ID_STATICLINE10, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE10"));
	BoxSizer1->Add(StaticLine10, 0, wxALL|wxEXPAND, 0);
	CheckBox_CreerBackup = new wxCheckBox(this, ID_CHECKBOX9, _T("Créer un fichier backup *.bdd.bak lors des opérations Enregistrer"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
	CheckBox_CreerBackup->SetValue(false);
	CheckBox_CreerBackup->Disable();
	CheckBox_CreerBackup->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	BoxSizer1->Add(CheckBox_CreerBackup, 0, wxALL|wxEXPAND, 5);
	CheckBox_SupprBackup = new wxCheckBox(this, ID_CHECKBOX10, _T("Supprimer le fichier de backup *.bdd.bak en quittant Ovni"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
	CheckBox_SupprBackup->SetValue(false);
	CheckBox_SupprBackup->Disable();
	CheckBox_SupprBackup->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	BoxSizer1->Add(CheckBox_SupprBackup, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine11 = new wxStaticLine(this, ID_STATICLINE11, wxDefaultPosition, wxSize(380,1), wxLI_HORIZONTAL, _T("ID_STATICLINE11"));
	BoxSizer1->Add(StaticLine11, 0, wxALL|wxEXPAND, 0);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Répertoire des fichiers temporaires de travail"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT9"));
	BoxSizer1->Add(StaticText9, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	TextCtrl_WorkDir = new wxTextCtrl(this, ID_TEXTCTRL10, _T("C:"), wxDefaultPosition, wxSize(294,21), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	BoxSizer2->Add(TextCtrl_WorkDir, 0, wxLEFT|wxRIGHT|wxALIGN_BOTTOM, 5);
	Button_tmp_rep = new wxButton(this, ID_BUTTON1, _T("..."), wxDefaultPosition, wxSize(40,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(Button_tmp_rep, 0, wxALIGN_BOTTOM, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine9 = new wxStaticLine(this, ID_STATICLINE9, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE9"));
	BoxSizer1->Add(StaticLine9, 0, wxALL|wxEXPAND, 0);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT10, _T("Nombre de threads OpenMP (0 <=> auto)"), wxDefaultPosition, wxSize(260,16), 0, _T("ID_STATICTEXT10"));
	BoxSizer6->Add(StaticText5, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL, 5);
	SpinCtrl_Threads = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxSize(80,23), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Threads->SetValue(_T("0"));
	SpinCtrl_Threads->SetMaxSize(wxSize(78,0));
	BoxSizer6->Add(SpinCtrl_Threads, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer6, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticLine12 = new wxStaticLine(this, ID_STATICLINE12, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE12"));
	BoxSizer1->Add(StaticLine12, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 2);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	Button_Reset = new wxButton(this, ID_BUTTON2, _T("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer3->Add(Button_Reset, 1, wxALL|wxALIGN_BOTTOM, 2);
	Button_OK = new wxButton(this, ID_BUTTON3, _T("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer3->Add(Button_OK, 1, wxALL|wxALIGN_BOTTOM, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_SPINCTRLDOUBLE1,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrlDouble_axesChange);
	Connect(ID_SPINCTRLDOUBLE2,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrlDouble_normChange);
	Connect(ID_SPINCTRLDOUBLE3,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrlDouble_srcChange);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_AntialiasingSoftClick);
	Connect(ID_CHECKBOX8,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_1SeulObjet3DClick);
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_LectureOptimiseeClick);
	Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_TestDecalage3DSClick);
	Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_SeuillageClick);
	Connect(ID_SPINCTRLDOUBLE4,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrlDouble_SeuilGouraudChange);
	Connect(ID_SPINCTRLDOUBLE5,wxEVT_SPINCTRLDOUBLE,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrlDouble_SeuilGouraud2Change);
	Connect(ID_CHECKBOX6,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_RecNormales_SeuilleesClick);
	Connect(ID_CHECKBOX5,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_CalculNormalesClick);
	Connect(ID_CHECKBOX11,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_NotFlatClick);
	Connect(ID_CHECKBOX12,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_TraiterDoublonsAretesClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrl_PasSvgChange);
	Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&Prefs_Dialog::OnRadioBox_TriangulationSelect);
	Connect(ID_RADIOBOX2,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&Prefs_Dialog::OnRadioBox_TrackballSelect);
	Connect(ID_RADIOBOX3,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&Prefs_Dialog::OnRadioBox_IconSizeSelect);
	Connect(ID_CHECKBOX7,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_DisplayFpsClick);
	Connect(ID_CHECKBOX9,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_CreerBackupClick);
	Connect(ID_CHECKBOX10,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnCheckBox_SupprBackupClick);
	Connect(ID_TEXTCTRL10,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&Prefs_Dialog::OnTextCtrl_WorkDirText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnButton_tmp_repClick);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Prefs_Dialog::OnSpinCtrl_ThreadsChange);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnButton_ResetClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Prefs_Dialog::OnButton_OKClick);
	//*)
//	this->Prefs_Parent=parent;
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Prefs_Dialog::~Prefs_Dialog()
{
	//(*Destroy(Prefs_Dialog)
	//*)
}

void Prefs_Dialog::OnButton_OKClick(wxCommandEvent& event)
{
    Hide();
}

void Prefs_Dialog::OnSpinCtrlDouble_axesChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->len_axe = SpinCtrlDouble_axes->GetValue();
    Element->buildRepereOXYZ();
    Element->ini_file_modified = true ;
    Element->Refresh();
}

void Prefs_Dialog::OnSpinCtrlDouble_normChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->len_normales = SpinCtrlDouble_norm->GetValue();
    Element->ini_file_modified = true ;
    Element->m_gllist = Element->glliste_select; //0;      // Ne regénére que la liste des facettes sélectionnées => les normales des facettes
    Element->Refresh();
}

void Prefs_Dialog::OnSpinCtrlDouble_srcChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->ray_sun = SpinCtrlDouble_src->GetValue();
    Element->ini_file_modified = true ;
    Element->Refresh();
}

void Prefs_Dialog::OnCheckBox_SeuillageClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    CheckBox_RecNormales_Seuillees->SetValue(MAIN->Element->Enr_Normales_Seuillees);
    bool chkB = CheckBox_Seuillage->GetValue();
    if (chkB) {
        StaticText_Gouraud            ->Enable() ;
        StaticText_Gouraud2           ->Enable() ;
        SpinCtrlDouble_SeuilGouraud   ->Enable() ;
        SpinCtrlDouble_SeuilGouraud2  ->Enable() ;
        CheckBox_RecNormales_Seuillees->Enable();
    } else {
        StaticText_Gouraud            ->Disable() ;
        StaticText_Gouraud2           ->Disable() ;
        SpinCtrlDouble_SeuilGouraud   ->Disable();
        SpinCtrlDouble_SeuilGouraud2  ->Disable();
        CheckBox_RecNormales_Seuillees->Disable();
//        CheckBox_RecNormales_Seuillees->SetValue(chkB);
    }

    Element->test_seuil_gouraud = chkB;
    Element->ini_file_modified  = true ;
    Element->m_gllist = 0;
    Element->Refresh();
}

void Prefs_Dialog::OnSpinCtrlDouble_SeuilGouraudChange(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    float val = SpinCtrlDouble_SeuilGouraud->GetValue();
    Element->angle_Gouraud  = val;
    float seuil = cos(val*to_Rad);
    if (val >= 179.9) seuil = -1.0f;
    Element->seuil_Gouraud  = seuil;

    val *= MAIN->Element->fmult_Gouraud;
    if (val > 90.0) val = 180.0;
    seuil = cos(val*to_Rad);
    if (val >= 179.9) seuil    = -1.0f;
    Element->seuil_Gouraud2    = seuil;
    Element->ini_file_modified = true ;
    Element->m_gllist = 0;
    Element->Refresh();
}

void Prefs_Dialog::OnSpinCtrlDouble_SeuilGouraud2Change(wxSpinDoubleEvent& event)
{
    BddInter *Element = MAIN->Element;

    float val = SpinCtrlDouble_SeuilGouraud2->GetValue();
    Element->fmult_Gouraud = val;
    val *= MAIN->Element->angle_Gouraud;
    if (val > 90.0) val = 180.0;    // Forcer les valeurs > 90° (comme en version Tcl)
    float seuil= cos(val*to_Rad);
    if (val >= 179.9)   seuil  = -1.0f;
    Element->seuil_Gouraud2    = seuil;
    Element->ini_file_modified = true ;
    Element->m_gllist = 0;
    Element->Refresh();
}

void Prefs_Dialog::OnSpinCtrl_PasSvgChange_pub(wxSpinEvent& event)
{
// Ici, fonction publique pour être appelée depuis l'extérieur de la classe Prefs_Dialog
//printf("OK spin\n");
    bool svg;
    svg = MAIN->Element->ini_file_modified; // Sauvegarder la valeur
    OnSpinCtrl_PasSvgChange(event);
    MAIN->Element->ini_file_modified = svg; // puis la restituer
}

void Prefs_Dialog::OnSpinCtrl_PasSvgChange(wxSpinEvent& event)
{
    int svg_time_local;
    MAIN->Element->svg_time = svg_time_local = SpinCtrl_PasSvg->GetValue();
    MAIN->Timer_Save.Stop();                                    // Arrêt du timer
    if (svg_time_local > 0) {                                   // Puis le relancer si svg_time > 0
        MAIN->Timer_Save.Start(svg_time_local*60000,false);     // Convertir des minutes en millisecondes et démarrer le timer
    }
    MAIN->Element->ini_file_modified= true ;  // En commentaire pour l'instant, la sauvegarde automatique n'est pas encore implémentée
}

void Prefs_Dialog::OnButton_tmp_repClick(wxCommandEvent& event)
{
    defaultPath = MAIN->Element->wxWorkDir;
    wxDirDialog  Path_Dlg(this,_T("Choisir un répertoire de travail ..."),defaultPath);
    int retour = Path_Dlg.ShowModal();
    if (retour == wxID_CANCEL) return;
    wxString New_Path = Path_Dlg.GetPath();
    if (New_Path != _T("")) {
        New_Path += wxFILE_SEP_PATH ; // Ajouter le séparateur de path à la fin
        TextCtrl_WorkDir->ChangeValue(New_Path);
        defaultPath = MAIN->Element->wxWorkDir = New_Path ;
        MAIN->Element->ini_file_modified = true ;
    }
//    Path_Dlg.Destroy();   // Provoque une erreur avec wxWidgets >= 3.1.4
}

void Prefs_Dialog::OnTextCtrl_WorkDirText(wxCommandEvent& event)
{
    MAIN->Element->wxWorkDir = TextCtrl_WorkDir->GetLabel();
    wxString Last_Char = wxFILE_SEP_PATH ;      // <=> wxFileName::GetPathSeparator()
    if(!MAIN->Element->wxWorkDir.EndsWith(Last_Char)) {            // Si séparateur de path absent à la fin, l'ajouter
        MAIN->Element->wxWorkDir += wxFILE_SEP_PATH;
        TextCtrl_WorkDir->SetLabel(MAIN->Element->wxWorkDir);       // et mettre à jour dans TextCtrl_WorkDir
    }
    MAIN->Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_AntialiasingSoftClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->antialiasing_soft = CheckBox_AntialiasingSoft->GetValue();//Antialiasing_Soft;
//    MAIN->Element->m_gldata.initialized=false;
//    MAIN->Element->ResetProjectionMode();
    Element->buildAllLines();
    Element->buildBoite();
    Element->buildRepereOXYZ();
    Element->ini_file_modified = true ;
    Element->Refresh();
}

void Prefs_Dialog::OnRadioBox_TrackballSelect(wxCommandEvent& event)
{
//    printf("Mode Trackball : %d\n",RadioBox_Trackball->GetSelection());
//    MAIN->Element->m_gldata.mode_Trackball = RadioBox_Trackball->GetSelection();
    wxKeyEvent k_event;
    k_event.m_keyCode = 'T';
    MAIN->Element->OnKeyDown(k_event);   // Simule une pression sur la touche T au clavier
    MAIN->Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnButton_ResetClick(wxCommandEvent& event)
{
    float val ;
    bool  chkB, chkB2;
    int   ival;
    BddInter *Element = MAIN->Element;

// Longueur des axes
    val = Element->len_axe = Element->len_axe_def;
    SpinCtrlDouble_axes->SetValue(val) ;

// Longueur des normales
    val = Element->len_normales = Element->len_normales_def;
    SpinCtrlDouble_norm->SetValue(val) ;

// Rayon du "Soleil"
    val = Element->ray_sun = Element->ray_sun_def;
    SpinCtrlDouble_src->SetValue(val) ;

// Case à cocher Antialiasing Soft
    chkB = Element->antialiasing_soft = Element->antialiasing_soft_def;
    CheckBox_AntialiasingSoft->SetValue(chkB);

// Case à cocher des fichiers .obj
    chkB  = Element->Forcer_1_Seul_Objet = Element->Forcer_1_Seul_Objet_def;
    CheckBox_1SeulObjet3D->SetValue(chkB);
    chkB2 = Element->lect_obj_opt = Element->lect_obj_opt_def;
    CheckBox_LectureOptimisee->SetValue(chkB2);
    if (chkB)
        CheckBox_LectureOptimisee->Disable() ;
    else
        CheckBox_LectureOptimisee->Enable();

// Case à cocher "Lecture décalage éventuel 3ds
    chkB = Element->test_decalage3ds = Element->test_decalage3ds_def;
    CheckBox_TestDecalage3DS->SetValue(chkB);

// Case à cocher calcul des normales aux sommets dès la lecture
    chkB = Element->CalculNormalesLectureBdd = Element->CalculNormalesLectureBdd_def;
    CheckBox_CalculNormales->SetValue(chkB);

// Case à cocher du traitement des doublons d'arêtes
    chkB = Element->traiter_doublons_aretes = Element->traiter_doublons_aretes_def;
    CheckBox_TraiterDoublonsAretes->SetValue(chkB);

// Case à cocher de Test du seuillage de Gouraud et activation/désactivation des valeurs de seuil
    chkB = Element->test_seuil_gouraud = Element->test_seuil_gouraud_def;
    CheckBox_Seuillage->SetValue(chkB);
    val = Element->angle_Gouraud_def;
    float cos_val = cos(val*to_Rad);
    if (val >= 179.9) cos_val = -1.0f;
    Element->seuil_Gouraud = cos_val;
    Element->angle_Gouraud = val;
    SpinCtrlDouble_SeuilGouraud->SetValue(val);

    val = Element->fmult_Gouraud = Element->fmult_Gouraud_def;
    SpinCtrlDouble_SeuilGouraud2->SetValue(val);
    val *= Element->angle_Gouraud_def ;
    cos_val = cos(val*to_Rad);
    if (val >= 179.9) cos_val = -1.0f;
    Element->seuil_Gouraud2 = cos_val;
    ival = Element->Enr_Normales_Seuillees = Element->Enr_Normales_Seuillees_def;
    CheckBox_RecNormales_Seuillees->SetValue(ival);
    if (chkB) {
        StaticText_Gouraud            ->Enable() ;
        StaticText_Gouraud2           ->Enable() ;
        SpinCtrlDouble_SeuilGouraud   ->Enable() ;
        SpinCtrlDouble_SeuilGouraud2  ->Enable() ;
        CheckBox_RecNormales_Seuillees->Enable();
    } else {
        StaticText_Gouraud            ->Disable() ;
        StaticText_Gouraud2           ->Disable() ;
        SpinCtrlDouble_SeuilGouraud   ->Disable();
        SpinCtrlDouble_SeuilGouraud2  ->Disable();
        CheckBox_RecNormales_Seuillees->Disable();
//        CheckBox_RecNormales_Seuillees->SetValue(chkB);
    }
    chkB = Element->NotFlat = Element->NotFlat_def;
    CheckBox_NotFlat->SetValue(chkB);

// Pas temporel des sauvegardes
    ival = Element->svg_time = Element->svg_time_def;
    SpinCtrl_PasSvg->SetValue(ival);
    wxSpinEvent cmd_spin;
    OnSpinCtrl_PasSvgChange(cmd_spin);    // Simule un clic pour réactualiser/réinitialiser le timer

// Mode de triangulation
    ival = Element->methode_Triangulation = Element->methode_Triangulation_def;
    RadioBox_Triangulation->SetSelection(ival);

// Mode Trackball ou Rotation directe
    ival = Element->m_gldata.mode_Trackball = Element->mode_Trackball_def;
    RadioBox_Trackball->SetSelection(ival);

// Case à cocher pour l'affichage des FPS
    chkB = Element->viewFps = Element->viewFps_def;
    CheckBox_DisplayFps->SetValue(chkB);

// Case à cocher pour la création d'un fichier .bak
    chkB = Element->CreerBackup = Element->CreerBackup_def;
    CheckBox_CreerBackup->SetValue(chkB);
    CheckBox_SupprBackup->Enable(chkB);     // N'activer la case à cocher de suppression du .bak que si sa création a été demandée

// Case à cocher pour la suppression du fichier .bak en sortie de programme
    chkB = Element->SupprBackup = Element->SupprBackup_def;
    CheckBox_SupprBackup->SetValue(chkB);

// Reset du répertoire de travail
//    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
//    wxString appPath(f.GetPath());
    Element->wxWorkDir = Element->wxOvniPath;
    char Message[512];
    sprintf(Message,"Répertoire de travail :\n%s\n",(const char*)Element->wxOvniPath.utf8_str());
    printf("%s",utf8_To_ibm(Message));
    TextCtrl_WorkDir->SetLabel(Element->wxOvniPath);

    Element->nb_threads      = Element->nb_threads_def;
    SpinCtrl_Threads->SetValue(Element->nb_threads);
    omp_set_dynamic(1);

    Element->icon_index = Element->icon_index_def;
    Element->icon_size  = Element->icon_sizes[Element->icon_index];
    RadioBox_IconSize->SetSelection(Element->icon_index);
    OnRadioBox_IconSizeSelect(event);   // Simule un changement de taille d'icône

    Element->ini_file_modified = true ;
    Element->m_gllist = 0;
    Element->Refresh();
}

void Prefs_Dialog::OnCheckBox_DisplayFpsClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->viewFps = CheckBox_DisplayFps->GetValue();
    Element->ini_file_modified = true ;
    Element->Refresh();
}

void Prefs_Dialog::OnCheckBox_TestDecalage3DSClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->test_decalage3ds = CheckBox_TestDecalage3DS->GetValue();
    Element->ini_file_modified= true ;
}

void Prefs_Dialog::OnCheckBox_LectureOptimiseeClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->lect_obj_opt = CheckBox_LectureOptimisee->GetValue();
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_RecNormales_SeuilleesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Enr_Normales_Seuillees = CheckBox_RecNormales_Seuillees->GetValue();
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_1SeulObjet3DClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool chkB;

    chkB = CheckBox_1SeulObjet3D->GetValue();
    Element->Forcer_1_Seul_Objet = chkB;
    if (chkB) {
        CheckBox_LectureOptimisee->Disable();
    } else {
        CheckBox_LectureOptimisee->Enable();
    }
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_CalculNormalesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->CalculNormalesLectureBdd = CheckBox_CalculNormales->GetValue();
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnRadioBox_TriangulationSelect(wxCommandEvent& event)
{
    MAIN->Element->methode_Triangulation = RadioBox_Triangulation->GetSelection();
//    printf("Methode de Triangulation dans Preference : %d\n",MAIN->Element->methode_Triangulation);
}

void Prefs_Dialog::OnCheckBox_CreerBackupClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool ChkB = CheckBox_CreerBackup->GetValue();
    Element->CreerBackup = ChkB;
    CheckBox_SupprBackup->Enable(ChkB);
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_SupprBackupClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->SupprBackup = CheckBox_SupprBackup->GetValue();
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_NotFlatClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->NotFlat = CheckBox_NotFlat->GetValue();
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnInit(wxInitDialogEvent& event)
{
}

void Prefs_Dialog::OnSpinCtrl_ThreadsChange(wxSpinEvent& event)
{
    BddInter *Element = MAIN->Element;

    int ival = SpinCtrl_Threads->GetValue();
    if ((ival <= 0) || (ival >= Element->nb_max_threads)) {
        omp_set_dynamic(1);         // Nombre de threads automatique, soit le maximum
        if (ival >= Element->nb_max_threads) ival = Element->nb_max_threads;
        omp_set_num_threads(Element->nb_max_threads);                       // Par précaution, mais pas indispensable !
    } else {
        omp_set_dynamic(0);         // Forcer le nombre de threads
        omp_set_num_threads(ival);
    }
    Element->nb_threads        = ival;
    Element->ini_file_modified = true ;
}

void Prefs_Dialog::OnCheckBox_TraiterDoublonsAretesClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->traiter_doublons_aretes = CheckBox_TraiterDoublonsAretes->GetValue();
    Element->ini_file_modified       = true ;
}

void Prefs_Dialog::OnRadioBox_IconSizeSelect(wxCommandEvent& event)
{
    BddInter *Element  = MAIN->Element;

    Element->icon_size = Element->icon_sizes[RadioBox_IconSize->GetSelection()];    // soit 16, 24, 32 ou 48 car 4 choix proposés à l'affichage dans Préférences
    int new_width      = MAIN->SetNewIcons(Element->icon_size) ;
    int new_height     = Element->icon_size+9;
    MAIN->Panel1->SetMinSize(wxSize(new_width,new_height));
    MAIN->Panel1->SetSize   (wxSize(new_width,new_height));

    wxRect rect          = MAIN->GetRect();                                         // récupère position et taille de la fenêtre
    wxSize* ClientSizeXY = new wxSize(new_width,rect.height);                       // Ajuster la taille de la fenêtre
    MAIN->SetSize(*ClientSizeXY);
    Element->ini_file_modified = true ;
    delete ClientSizeXY;
}
