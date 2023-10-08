#include "RotationPanel.h"

//(*InternalHeaders(RotationPanel)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(RotationPanel)
const long RotationPanel::ID_STATICTEXT1 = wxNewId();
const long RotationPanel::ID_TEXTCTRL1 = wxNewId();
const long RotationPanel::ID_BUTTON1 = wxNewId();
const long RotationPanel::ID_STATICBOX1 = wxNewId();
const long RotationPanel::ID_RADIOBOX1 = wxNewId();
const long RotationPanel::ID_STATICTEXT2 = wxNewId();
const long RotationPanel::ID_TEXTCTRL2 = wxNewId();
const long RotationPanel::ID_STATICTEXT3 = wxNewId();
const long RotationPanel::ID_TEXTCTRL3 = wxNewId();
const long RotationPanel::ID_STATICTEXT4 = wxNewId();
const long RotationPanel::ID_TEXTCTRL4 = wxNewId();
const long RotationPanel::ID_STATICTEXT5 = wxNewId();
const long RotationPanel::ID_STATICTEXT6 = wxNewId();
const long RotationPanel::ID_STATICTEXT7 = wxNewId();
const long RotationPanel::ID_BUTTON2 = wxNewId();
const long RotationPanel::ID_TEXTCTRL5 = wxNewId();
const long RotationPanel::ID_SPINBUTTON1 = wxNewId();
const long RotationPanel::ID_TEXTCTRL6 = wxNewId();
const long RotationPanel::ID_SPINBUTTON2 = wxNewId();
const long RotationPanel::ID_TEXTCTRL7 = wxNewId();
const long RotationPanel::ID_SPINBUTTON3 = wxNewId();
const long RotationPanel::ID_BUTTON3 = wxNewId();
const long RotationPanel::ID_BUTTON4 = wxNewId();
//*)

wxString format= _T("%4.1f");
//const double to_Rad=M_PI/180.0f;

BEGIN_EVENT_TABLE(RotationPanel,wxDialog)
	//(*EventTable(RotationPanel)
	//*)
END_EVENT_TABLE()

RotationPanel::RotationPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(RotationPanel)
	Create(parent, wxID_ANY, _T("Rotation"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("wxID_ANY"));
	SetClientSize(wxSize(360,371));
	Move(wxPoint(30,30));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Pas angulaire en degrés"), wxPoint(8,12), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_PasAngulaire = new wxTextCtrl(this, ID_TEXTCTRL1, _T("1.0"), wxPoint(152,8), wxDefaultSize, wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_ValiderPas = new wxButton(this, ID_BUTTON1, _T("Valider"), wxPoint(272,8), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_ValiderPas->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _T("Centre de Rotation"), wxPoint(2,40), wxSize(356,176), 0, _T("ID_STATICBOX1"));
	wxString __wxRadioBoxChoices_1[3] =
	{
	    _T("Barycentre de la sélection"),
	    _T("Boîte englobante de la sélection"),
	    _T("Point particulier à entrer (et valider) ci-dessous")
	};
	RadioBox_Centre = new wxRadioBox(this, ID_RADIOBOX1, wxEmptyString, wxPoint(32,56), wxSize(296,86), 3, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("X :"), wxPoint(8,156), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(32,152), wxSize(72,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	TextCtrl_X->Disable();
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Y :"), wxPoint(128,156), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(152,152), wxSize(72,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextCtrl_Y->Disable();
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Z :"), wxPoint(248,156), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(272,152), wxSize(80,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	TextCtrl_Z->Disable();
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Angle de rotation en X :"), wxPoint(40,228), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Angle de rotation en Y :"), wxPoint(40,260), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Angle de rotation en Z :"), wxPoint(40,292), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	Button_ValiderPoint = new wxButton(this, ID_BUTTON2, _T("Valider"), wxPoint(104,184), wxSize(152,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_ValiderPoint->Disable();
	Button_ValiderPoint->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	TextCtrl_AngleX = new wxTextCtrl(this, ID_TEXTCTRL5, _T("0"), wxPoint(200,224), wxSize(88,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	SpinButton_X = new wxSpinButton(this, ID_SPINBUTTON1, wxPoint(288,224), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON1"));
	SpinButton_X->SetRange(-360, 360);
	TextCtrl_AngleY = new wxTextCtrl(this, ID_TEXTCTRL6, _T("0"), wxPoint(200,256), wxSize(88,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	SpinButton_Y = new wxSpinButton(this, ID_SPINBUTTON2, wxPoint(288,256), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON2"));
	SpinButton_Y->SetRange(-360, 360);
	TextCtrl_AngleZ = new wxTextCtrl(this, ID_TEXTCTRL7, _T("0"), wxPoint(200,288), wxSize(88,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	SpinButton_Z = new wxSpinButton(this, ID_SPINBUTTON3, wxPoint(288,288), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON3"));
	SpinButton_Z->SetRange(-360, 360);
	Button_Appliquer = new wxButton(this, ID_BUTTON3, _T("Appliquer"), wxPoint(40,336), wxSize(120,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Appliquer->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON4, _T("Annuler"), wxPoint(200,336), wxSize(120,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_PasAngulaireText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RotationPanel::OnButton_ValiderPasClick);
	Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&RotationPanel::OnRadioBox_CentreSelect);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_XText);
	Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_YText);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_ZText);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RotationPanel::OnButton_ValiderPointClick);
	Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_AngleXText);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&RotationPanel::OnSpinButton_XChangeUp);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&RotationPanel::OnSpinButton_XChangeDown);
	Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_AngleYText);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&RotationPanel::OnSpinButton_YChangeUp);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&RotationPanel::OnSpinButton_YChangeDown);
	Connect(ID_TEXTCTRL7,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&RotationPanel::OnTextCtrl_AngleZText);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&RotationPanel::OnSpinButton_ZChangeUp);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&RotationPanel::OnSpinButton_ZChangeDown);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RotationPanel::OnButton_AppliquerClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&RotationPanel::OnButton_AnnulerClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&RotationPanel::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&RotationPanel::OnClose);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

RotationPanel::~RotationPanel()
{
	//(*Destroy(RotationPanel)
	//*)
}


void RotationPanel::OnClose(wxCloseEvent& event)
{
    MAIN->Element->Rotation_Objets = false;
    Hide();
}

void RotationPanel::OnInit(wxInitDialogEvent& event)
{
//    printf("RotationPanel::OnInit\n");
    Incr = wxAtof(TextCtrl_PasAngulaire->GetValue());
//    RotX = RotY = RotZ = 0.0;
    RotX = wxAtof(TextCtrl_AngleX->GetValue());
    RotY = wxAtof(TextCtrl_AngleY->GetValue());
    RotZ = wxAtof(TextCtrl_AngleZ->GetValue());
    wxTexte.Printf(format,RotX);
    TextCtrl_AngleX->SetValue(wxTexte);
    wxTexte.Printf(format,RotY);
    TextCtrl_AngleY->SetValue(wxTexte);
    wxTexte.Printf(format,RotZ);
    TextCtrl_AngleZ->SetValue(wxTexte);

    wxCommandEvent cmd_event;
    OnButton_ValiderPointClick(cmd_event);  // à faire en premier
    OnRadioBox_CentreSelect(cmd_event);
}

void RotationPanel::OnTextCtrl_PasAngulaireText(wxCommandEvent& event)
{
    Incr = wxAtof(TextCtrl_PasAngulaire->GetValue());
}

void RotationPanel::OnButton_ValiderPasClick(wxCommandEvent& event)
{
    Incr = wxAtof(TextCtrl_PasAngulaire->GetValue());
}

void RotationPanel::OnRadioBox_CentreSelect(wxCommandEvent& event)
{
    if (RadioBox_Centre->GetSelection() == 2) {
        TextCtrl_X->Enable();
        TextCtrl_Y->Enable();
        TextCtrl_Z->Enable();
        Button_ValiderPoint->Enable();
    } else {
        TextCtrl_X->Disable();
        TextCtrl_Y->Disable();
        TextCtrl_Z->Disable();
        Button_ValiderPoint->Disable();
    }
    Init_Centre_Rotation();
}

void RotationPanel::Init_Centre_Rotation()
{
    BddInter *Element = MAIN->Element;

    unsigned int n_val,i,j, Nb_p;
    Object  *objet_courant;

    n_val   = Element->listeObjets.size();
    auto it = Element->listeObjets.begin();
    double cx1,cx2,cy1,cy2,cz1,cz2,val;

    int selection = RadioBox_Centre->GetSelection();
//    printf("selection : %d\n",selection);
    switch (selection) {
    default:
    case 0:
    // Barycentre du/des Objets
        cx1 = cy1 = cz1 = 0.0;
        Nb_p= 0;
        for (i=0; i<n_val; i++, it++) {
            objet_courant = &(Element->Objetlist[*it]);
            if (objet_courant->deleted) continue ;                  // Ne pas traiter les objets supprimés
            for (j=0; j<objet_courant->Sommetlist.size(); j++) {    // Non parallélisable ? car somme cumulée des c*1 et nb_p (critical sans intérêt)
                cx1 +=  objet_courant->Sommetlist[j].point[0];
                cy1 +=  objet_courant->Sommetlist[j].point[1];
                cz1 +=  objet_courant->Sommetlist[j].point[2];
                Nb_p++;
            }
        }
        if (Nb_p != 0) {
            Element->Centre_X = cx1/Nb_p; // ATTENTION : si Nb_p = 0, Centre_X _Y et _Z ne sont pas initialisés !!!!
            Element->Centre_Y = cy1/Nb_p;
            Element->Centre_Z = cz1/Nb_p;
        } else {
            Element->Centre_X = Element->Centre_Y = Element->Centre_Z = 0.0 ; // à vérifier si c'est un bon choix
        }
//        printf("Centre_Rotation Barycentre : %f %f %f\n",Element->Centre_X,Element->Centre_Y,Element->Centre_Z);
        break;

    case 1:
    // Centre de la boîte englobante
        cx1 = cy1 = cz1 = DBL_MAX;
        cx2 = cy2 = cz2 = DBL_MIN;
        for (i=0; i<n_val; i++, it++) {
            objet_courant = &(Element->Objetlist[*it]);
            if (objet_courant->deleted) continue ;                   // Ne pas traiter les objets supprimés
            for (j=0; j<objet_courant->Sommetlist.size(); j++) {
                val = objet_courant->Sommetlist[j].point[0];
                cx1 = (val < cx1 ? val : cx1);
                cx2 = (val > cx2 ? val : cx2);
                val = objet_courant->Sommetlist[j].point[1];
                cy1 = (val < cy1 ? val : cy1);
                cy2 = (val > cy2 ? val : cy2);
                val = objet_courant->Sommetlist[j].point[2];
                cz1 = (val < cz1 ? val : cz1);
                cz2 = (val > cz2 ? val : cz2);
            }
        }
        Element->Centre_X = (cx1 + cx2)/2;
        Element->Centre_Y = (cy1 + cy2)/2;
        Element->Centre_Z = (cz1 + cz2)/2;
//        printf("Centre_Rotation Boite      : %f %f %f\n",Element->Centre_X,Element->Centre_Y,Element->Centre_Z);
        break;

    case 2:
    // Lire les valeurs
        wxCommandEvent cmd_event;
        OnButton_ValiderPointClick(cmd_event);
        break;
    }
}

void RotationPanel::OnTextCtrl_XText(wxCommandEvent& event)
{
    MAIN->Element->Centre_X = wxAtof(TextCtrl_AngleX->GetValue());
    printf("Centre_Rotation Entree X   : %f %f %f\n",MAIN->Element->Centre_X,MAIN->Element->Centre_Y,MAIN->Element->Centre_Z);
}

void RotationPanel::OnTextCtrl_YText(wxCommandEvent& event)
{
    MAIN->Element->Centre_Y = wxAtof(TextCtrl_AngleY->GetValue());
    printf("Centre_Rotation Entree Y   : %f %f %f\n",MAIN->Element->Centre_X,MAIN->Element->Centre_Y,MAIN->Element->Centre_Z);
}

void RotationPanel::OnTextCtrl_ZText(wxCommandEvent& event)
{
    MAIN->Element->Centre_Z = wxAtof(TextCtrl_AngleZ->GetValue());
    printf("Centre_Rotation Entree Z   : %f %f %f\n",MAIN->Element->Centre_X,MAIN->Element->Centre_Y,MAIN->Element->Centre_Z);
}

void RotationPanel::OnButton_ValiderPointClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Centre_X = wxAtof(TextCtrl_X->GetValue());   // Pas toujours nécessaire !
    Element->Centre_Y = wxAtof(TextCtrl_Y->GetValue());
    Element->Centre_Z = wxAtof(TextCtrl_Z->GetValue());
//    printf("Centre_Rotation Entree     : %f %f %f\n",MAIN->Element->Centre_X,MAIN->Element->Centre_Y,MAIN->Element->Centre_Z);
}

void RotationPanel::OnTextCtrl_AngleXText(wxCommandEvent& event)
{
    RotX = wxAtof(TextCtrl_AngleX->GetValue()); // Pour une entrée directe (sans le SpinButton)
    MAIN->Element->Rot_X = RotX;
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::OnSpinButton_XChangeUp(wxSpinEvent& event)
{
    RotX += Incr;
    if (RotX > 180.0) RotX -= 360.0;        // Pour boucler sur -180, +180°
    OnSpinButton_XChange(event);
}

void RotationPanel::OnSpinButton_XChangeDown(wxSpinEvent& event)
{
    RotX -= Incr;
    if (RotX < -180.0) RotX += 360.0;        // Pour boucler sur -180, +180°
    OnSpinButton_XChange(event);
}

void RotationPanel::OnSpinButton_XChange(wxSpinEvent& event)
{
    SpinButton_X->SetValue(0);              // Raz
    MAIN->Element->Rot_X = RotX;
    wxTexte.Printf(format, RotX);
    TextCtrl_AngleX->SetValue(wxTexte);     // Afficher la valeur calculée par incrémentation/décrémentation
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::OnTextCtrl_AngleYText(wxCommandEvent& event)
{
    RotY = wxAtof(TextCtrl_AngleY->GetValue());
    MAIN->Element->Rot_Y = RotY;
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::OnSpinButton_YChangeUp(wxSpinEvent& event)
{
    RotY += Incr;
    if (RotY > 180.0) RotY -= 360.0;
    OnSpinButton_YChange(event);
}

void RotationPanel::OnSpinButton_YChangeDown(wxSpinEvent& event)
{
    RotY -= Incr;
    if (RotY < -180.0) RotY += 360.0;
    OnSpinButton_YChange(event);
}

void RotationPanel::OnSpinButton_YChange(wxSpinEvent& event)
{
    SpinButton_Y->SetValue(0);  // Raz
    MAIN->Element->Rot_Y = RotY;
    wxTexte.Printf(format, RotY);
    TextCtrl_AngleY->SetValue(wxTexte);
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::OnTextCtrl_AngleZText(wxCommandEvent& event)
{
    RotZ = wxAtof(TextCtrl_AngleZ->GetValue());
    MAIN->Element->Rot_Z = RotZ;
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::OnSpinButton_ZChangeUp(wxSpinEvent& event)
{
    RotZ += Incr;
    if (RotZ > 180.0) RotZ -= 360.0;
    OnSpinButton_ZChange(event);
}

void RotationPanel::OnSpinButton_ZChangeDown(wxSpinEvent& event)
{
    RotZ -= Incr;
    if (RotZ < -180.0) RotZ += 360.0;
    OnSpinButton_ZChange(event);
}

void RotationPanel::OnSpinButton_ZChange(wxSpinEvent& event)
{
    SpinButton_Z->SetValue(0);  // Raz
    MAIN->Element->Rot_Z = RotZ;
    wxTexte.Printf(format, RotZ);
    TextCtrl_AngleZ->SetValue(wxTexte);
    Appliquer_Rotation_Visuelle();
}

void RotationPanel::Appliquer_Rotation_Visuelle()
{
    BddInter *Element = MAIN->Element;
// Ici, c'est une application visuelle, sans modification de Bdd
//    printf("Rotation en X, Y et Z : %3.1f %3.1f %3.1f\n",RotX, RotY, RotZ);
    Element->Rotation_Objets = true;
    Element->m_gllist = 0;
    Element->Refresh();

}

/* Rotation en X d'un objet */
void RotationPanel::Rotation_Objet_X(int indiceObjet, double angle)
{
	unsigned int j, ns;
	float yy, zz ;
	double cos_a, sin_a;
	Object *objet_courant;
	Sommet *sommet_courant;
/*	Matrice de rotation
	float rotx[] = { 1.0,    0.0    ,     0.0,
					 0.0, cos(angle), -sin(angle),
					 0.0, sin(angle),  cos(angle) }; */

    objet_courant = &(MAIN->Element->Objetlist[indiceObjet]);
    cos_a = cos(angle) ; sin_a = sin(angle);
    ns = objet_courant->Sommetlist.size();
#pragma omp parallel for private(sommet_courant,yy,zz)
	for(j=0; j<ns; j++) {
	    sommet_courant = &(objet_courant->Sommetlist[j]);
	    yy = sommet_courant->point[1] ;    // y
	    zz = sommet_courant->point[2] ;    // z
		sommet_courant->point[1] = cos_a*yy -sin_a*zz;
		sommet_courant->point[2] = sin_a*yy +cos_a*zz;
	}

}

/* Rotation en Y d'un objet */
void RotationPanel::Rotation_Objet_Y(int indiceObjet, double angle)
{
	unsigned int j, ns;
	float xx, zz ;
	double cos_a, sin_a;
	Object *objet_courant;
	Sommet *sommet_courant;
/*	Matrice de rotation
	float roty[] = { cos(angle), 0.0, sin(angle),
					    0.0,	 1.0,    0.0,
					-sin(angle), 0.0, cos(angle) };	*/

    objet_courant = &(MAIN->Element->Objetlist[indiceObjet]);
    cos_a = cos(angle) ; sin_a = sin(angle);
    ns = objet_courant->Sommetlist.size();
#pragma omp parallel for private(sommet_courant,xx,zz)
	for(j=0; j<ns; j++)	{
	    sommet_courant = &(objet_courant->Sommetlist[j]);
	    xx = sommet_courant->point[0] ;    // x
	    zz = sommet_courant->point[2] ;    // z
		sommet_courant->point[0] = cos_a*xx +sin_a*zz;
		sommet_courant->point[2] =-sin_a*xx +cos_a*zz;
	}

}

/* Rotation en Z d'un objet */
void RotationPanel::Rotation_Objet_Z(int indiceObjet, double angle)
{
	unsigned int j, ns;
	float xx, yy ;
	double cos_a, sin_a;
	Object *objet_courant;
	Sommet *sommet_courant;
/*	Matrice de rotation
	float rotz[] = { cos(angle), -sin(angle), 0.0,
					 sin(angle),  cos(angle), 0.0,
						0.0,	     0.0,	  1.0 }; */

    objet_courant = &(MAIN->Element->Objetlist[indiceObjet]);
    cos_a = cos(angle) ; sin_a = sin(angle);
    ns = objet_courant->Sommetlist.size();
#pragma omp parallel for private(sommet_courant,xx,yy)
	for(j=0; j<ns; j++)	{
	    sommet_courant = &(objet_courant->Sommetlist[j]);
	    xx = sommet_courant->point[0] ;    // x
	    yy = sommet_courant->point[1] ;    // y
		sommet_courant->point[0] = cos_a*xx -sin_a*yy;
		sommet_courant->point[1] = sin_a*xx +cos_a*yy;
	}
}

void RotationPanel::OnButton_AppliquerClick(wxCommandEvent& event)
{
// Ici, on modifie la Bdd pour les objets sélectionnés
    BddInter *Element = MAIN->Element;

    unsigned int n_val, i, j, nb_fac;
    Object  *objet_courant;
    Sommet  *sommet_courant;

    n_val   = Element->listeObjets.size();
    auto it = Element->listeObjets.begin();

    for (i=0; i<n_val; i++, it++) {
        int o = *it;
//        printf("Objet : %d",o);
        objet_courant = &(Element->Objetlist[o]);
        unsigned int ns = objet_courant->Sommetlist.size();
//        printf(", Nb Sommets : %d\n",objet_courant->Sommetlist.size());
        // Recentrer l'objet en 0,0,0
#pragma omp parallel for private(sommet_courant)
        for (j=0; j<ns; j++) {
            sommet_courant = &(objet_courant->Sommetlist[j]);
            sommet_courant->point[0] -= Element->Centre_X;
            sommet_courant->point[1] -= Element->Centre_Y;
            sommet_courant->point[2] -= Element->Centre_Z;
        }
        // Effectuer les 3 rotations dans l'ordre X, Y et Z (en mode graphique on a fait l'inverse Z, Y, X !!!) sinon on n'obtient pas la même chose au final.
        Rotation_Objet_X(*it,RotX*to_Rad);
        Rotation_Objet_Y(*it,RotY*to_Rad);
        Rotation_Objet_Z(*it,RotZ*to_Rad);
        // Remettre l'objet en place (translation inverse)
#pragma omp parallel for private(sommet_courant)
        for (j=0; j<ns; j++) {
            sommet_courant = &(objet_courant->Sommetlist[j]);
            sommet_courant->point[0] += Element->Centre_X;
            sommet_courant->point[1] += Element->Centre_Y;
            sommet_courant->point[2] += Element->Centre_Z;
        }

        // Calcul des normales aux barycentre des facettes
        nb_fac = objet_courant->Nb_facettes;
        for(j=0; j<nb_fac; j++) {
            Element->Calcul_Normale_Barycentre(o,j);
//            objet_courant->Facelist[j].flat = false;          // Ne pas changer a priori !
        }
        // Calcul des normales aux sommets
        unsigned int nv = objet_courant->Vecteurlist.size();    // Récupérer le nombre de vecteurs (peut être != ns)
        Element->Genere_Normales_Aux_Sommets(o,ns);
    }

    Element->bdd_modifiee    = true;
    Element->Rotation_Objets = false;
    Element->m_gllist = 0;
    Element->Refresh();

    OnButton_AnnulerClick(event);
}

void RotationPanel::OnButton_AnnulerClick(wxCommandEvent& event)
{
//    ToDo();
    RotX = RotY = RotZ = 0.0;
    wxTexte.Printf(format,RotX);
    TextCtrl_AngleX->SetValue(wxTexte);
    TextCtrl_AngleY->SetValue(wxTexte);
    TextCtrl_AngleZ->SetValue(wxTexte);
    wxCloseEvent close_event;
    OnClose(close_event);
}

//void RotationPanel::ToDo()
//{
//    DisplayMessage(_T("Pas encore complètement opérationnel\nSimulation interface OK"), false);
//}
