#include "Cylindre.h"

//(*InternalHeaders(Cylindre)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Cylindre)
const long Cylindre::ID_STATICTEXT1 = wxNewId();
const long Cylindre::ID_TEXTCTRL1 = wxNewId();
const long Cylindre::ID_STATICTEXT2 = wxNewId();
const long Cylindre::ID_TEXTCTRL2 = wxNewId();
const long Cylindre::ID_STATICTEXT3 = wxNewId();
const long Cylindre::ID_TEXTCTRL3 = wxNewId();
const long Cylindre::ID_STATICTEXT4 = wxNewId();
const long Cylindre::ID_TEXTCTRL4 = wxNewId();
const long Cylindre::ID_STATICTEXT5 = wxNewId();
const long Cylindre::ID_TEXTCTRL5 = wxNewId();
const long Cylindre::ID_TEXTCTRL6 = wxNewId();
const long Cylindre::ID_STATICTEXT6 = wxNewId();
const long Cylindre::ID_BUTTON1 = wxNewId();
const long Cylindre::ID_BUTTON2 = wxNewId();
const long Cylindre::ID_CHECKBOX1 = wxNewId();
const long Cylindre::ID_STATICTEXT7 = wxNewId();
const long Cylindre::ID_SPINCTRL1 = wxNewId();
const long Cylindre::ID_STATICTEXT8 = wxNewId();
const long Cylindre::ID_SPINCTRL2 = wxNewId();
const long Cylindre::ID_STATICTEXT9 = wxNewId();
const long Cylindre::ID_SPINCTRL3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Cylindre,wxDialog)
	//(*EventTable(Cylindre)
	//*)
END_EVENT_TABLE()

Cylindre::Cylindre(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Cylindre::Cylindre Création d'une forme générique en Cylindre
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Cylindre)
	Create(parent, id, _T("Ajout d\'un cylindre"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(407,217));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Longueur :"), wxPoint(120,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Arete = new wxTextCtrl(this, ID_TEXTCTRL1, _T("2.0"), wxPoint(192,8), wxSize(104,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Centre du cylindre:  X :"), wxPoint(8,35), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL2, _T("0.0"), wxPoint(128,32), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Y :"), wxPoint(204,35), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(224,32), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Z :"), wxPoint(304,35), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(328,32), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Rayon :"), wxPoint(120,59), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_Rayon = new wxTextCtrl(this, ID_TEXTCTRL5, _T("1.0"), wxPoint(192,56), wxSize(104,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	TextCtrl_NbPoints = new wxTextCtrl(this, ID_TEXTCTRL6, _T("10"), wxPoint(192,80), wxSize(104,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre de points/cercle :"), wxPoint(40,83), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(96,192), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(200,192), wxSize(88,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	CheckBox_FermerCylindre = new wxCheckBox(this, ID_CHECKBOX1, _T("Fermer le cylindre par 2 facettes aux extrémités"), wxPoint(8,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_FermerCylindre->SetValue(false);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Groupe :"), wxPoint(8,107), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(120,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Matériau :"), wxPoint(208,107), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(328,104), wxSize(72,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Nombre de secteurs"), wxPoint(8,139), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	SpinCtrl_NbSecteurs = new wxSpinCtrl(this, ID_SPINCTRL3, _T("1"), wxPoint(120,136), wxSize(72,21), 0, 1, 20, 1, _T("ID_SPINCTRL3"));
	SpinCtrl_NbSecteurs->SetValue(_T("1"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cylindre::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cylindre::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cylindre::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cylindre::OnSpinCtrl_MateriauChange);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cylindre::OnSpinCtrl_NbSecteursChange);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Cylindre::OnClose);
	//*)
//	this->Cylindre_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Cylindre::~Cylindre()
{
	//(*Destroy(Cylindre)
	//*)
}

void Cylindre::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Cylindre::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Cylindre::Genere_Sommets(BddInter *Element, Object *p_Objet)
{
    int i,j, numero;
    float angle,pas_angle;
    float Xp, XStep;
    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];

    angle = 0.;
    pas_angle = 2.*M_PI/NbPoints;
    int NbSommets = n_slices*NbPoints;

    Element->str.clear();
    Element->N_elements = NbSommets;
    Element->make_sommet();

    p_Objet->Nb_sommets = NbSommets;

    // Sommets
    numero = 1;
    Xp = Xc+arete/2;                // Premier plan
    XStep = arete/n_secteurs;
    for (j=0; j<n_slices ; j++) {
        angle = 0.;
        for (i=0; i<NbPoints; i++) {
            Element->N_elements=numero; Element->Setxyz(Xp,Yc+rayon*cos(angle),Zc+rayon*sin(angle)); Element->make_1_sommet();
            angle += pas_angle;
            numero++;
        }
        Xp -= XStep;                // Plans suivants
    }
}

void Cylindre::Genere_Luminances(BddInter* Element)
{
    wxString str_loc;
    int numero,i1,i,j;

    Element->str.clear();
    Element->N_elements = NbPoints*n_slices;
    if (CheckBox_FermerCylindre->IsChecked()) Element->N_elements +=2;

    Element->make_luminance();
    numero= 1;

    for (j=0; j < n_secteurs ; j++) {
        for (i=1; i <= NbPoints ; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1;
            Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i,i,i1,i1) ; Element->make_1_luminance();
            numero++;
        }
    }
    if (CheckBox_FermerCylindre->IsChecked()) {
        Element->str.Printf(_T("%d %d"),numero,NbPoints) ;
        str_loc.Printf(_T(" %d"),NbPoints+1);
        for (int j=0; j<NbPoints ; j++) Element->str += str_loc;
        Element->make_1_luminance();

        numero++;
        Element->str.Printf(_T("%d %d"),numero,NbPoints) ;
        str_loc.Printf(_T(" %d"),NbPoints+2);
        for (int j=0; j<NbPoints ; j++) Element->str += str_loc;
        Element->make_1_luminance();
    }
}

void Cylindre::Genere_Normales_Sommets(BddInter *Element, Object *p_Objet)
{
    // Normales aux sommets
    int numero;
    float angle,pas_angle;

    pas_angle = 2.*M_PI/NbPoints;

    Element->str.clear();
    Element->N_elements = NbPoints;                                      // Autant que de sommets sur 1 seul cercle générateur
    if (CheckBox_FermerCylindre->IsChecked()) Element->N_elements +=2;   // et 2 de plus en cas de fermeture du cylindre par 2 facettes

    p_Objet->Nb_vecteurs = Element->N_elements;
    Element->make_vecteur();
    numero= 1;
    angle = 0.;
    for (int i=0; i<NbPoints; i++) {
        Element->N_elements=numero; Element->Setxyz(0.,cos(angle),sin(angle)); Element->make_1_vecteur();
        angle += pas_angle;
        numero++;
    }
    if (CheckBox_FermerCylindre->IsChecked()) {
        Element->N_elements=numero;
        Element->Setxyz(1.,0.,0.);
        Element->make_1_vecteur();
        numero++;
        Element->N_elements=numero;
        Element->Setxyz(-1.,0.,0.);
        Element->make_1_vecteur();
    }
}

void Cylindre::Genere_Facettes(BddInter *Element, Object *p_Objet)
{
    wxString str_loc;
    int numero,i,i0,i1,i2,i3;
    int istep;

    Element->str.clear();
    int NbFacettes = NbPoints*n_secteurs;
    Element->N_elements = NbFacettes;
    if (CheckBox_FermerCylindre->IsChecked()) Element->N_elements +=2;
    Element->make_face();

    p_Objet->Nb_facettes = Element->N_elements;
    numero = 1;

    for (int j=0; j < n_secteurs ; j++) {
        istep = j*NbPoints;
        for (i=1; i<=NbPoints; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1; i2=i1+NbPoints;
            i0 = i+istep;
            i3 = i0+NbPoints;
            i2 += istep ; i1 += istep;
            Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i0,i3,i2,i1); Element->make_1_face();
            numero++;
        }
    }
    if (CheckBox_FermerCylindre->IsChecked()) {
        str_loc.Printf(_T("%d %d"),numero,NbPoints); Element->str = str_loc;
        for (i=1; i<=NbPoints ; i++) {
            str_loc.Printf(_T(" %d"),i);
            Element->str += str_loc;
        }
        Element->make_1_face();
        numero++;
        str_loc.Printf(_T("%d %d"),numero,NbPoints); Element->str = str_loc;
        i0 = n_slices*NbPoints;
        i1 = i0 - NbPoints;
        for (i=i0; i>i1 ; i--) {
            str_loc.Printf(_T(" %d"),i); Element->str += str_loc;
        }
        Element->make_1_face();
    }
}

void Cylindre::genereCylindre()
{
    wxString num_obj;
    int new_num;
    Object *p_Objet;

    BddInter *Element = MAIN->Element;

    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetNumero() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Cylindre - ") + num_obj;
    Element->make_objet();
    Element->Objetlist.rbegin()->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->GetIndiceObjetCourant();

    printf("centre : %f %f %f\narete  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],arete);
    printf("rayon  : %f, Nombre de points : %d\n",rayon,NbPoints);
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
    n_slices   = n_secteurs+1;
    int Nb_facettes = NbPoints*n_secteurs ;
    if (CheckBox_FermerCylindre->IsChecked()) Nb_facettes +=2;

    p_Objet = &(Element->Objetlist[indiceObjet]);

    Genere_Sommets (Element, p_Objet);
    Genere_Facettes(Element, p_Objet);

    Element->Genere_Normales_Facettes (p_Objet, Nb_facettes);
    Element->Genere_Attributs_Facettes(p_Objet, Nb_facettes, numeroGroupe, numeroMateriau);
    Genere_Normales_Sommets(Element, p_Objet);
    Genere_Luminances(Element);
    p_Objet->flat = false;

    Element->Genere_Tableau_Points_Facettes(p_Objet);
    Element->SetGenereTableauAretes_OK(true);
    Element->Genere_Tableau_Aretes(p_Objet);
    Element->Genere_Liste_Groupes_Materiaux(p_Objet);

    Element->SetBddModifiee(true);
}

void Cylindre::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    arete = wxAtof(TextCtrl_Arete->GetValue());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    rayon    = wxAtof(TextCtrl_Rayon->GetValue());
    NbPoints = wxAtoi(TextCtrl_NbPoints->GetValue());
    numeroGroupe   = SpinCtrl_Groupe   ->GetValue() ;   // Par précaution, mais déjà fait !
    numeroMateriau = SpinCtrl_Materiau ->GetValue() ;   // idem

    genereCylindre();

    Element->SetTypeFichier(1);     // Marquer comme si c'était un fichier .bdd
    Element->SetTypeDxf(false);
    Element->m_gllist = 0;

    Element->Search_Min_Max();
    Element->SetFileLoaded(true);
    Element->SetOK_ToSave (true);
    Element->Refresh();
    Hide();
}

void Cylindre::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Cylindre::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

void Cylindre::OnSpinCtrl_NbSecteursChange(wxSpinEvent& event)
{
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
    n_slices   = n_secteurs+1;
}
