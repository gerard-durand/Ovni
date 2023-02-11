#include "Tore.h"

//(*InternalHeaders(Tore)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(Tore)
const long Tore::ID_STATICTEXT1 = wxNewId();
const long Tore::ID_TEXTCTRL1 = wxNewId();
const long Tore::ID_STATICTEXT2 = wxNewId();
const long Tore::ID_TEXTCTRL2 = wxNewId();
const long Tore::ID_STATICTEXT3 = wxNewId();
const long Tore::ID_STATICTEXT4 = wxNewId();
const long Tore::ID_STATICTEXT5 = wxNewId();
const long Tore::ID_TEXTCTRL3 = wxNewId();
const long Tore::ID_TEXTCTRL4 = wxNewId();
const long Tore::ID_TEXTCTRL5 = wxNewId();
const long Tore::ID_STATICTEXT6 = wxNewId();
const long Tore::ID_STATICTEXT7 = wxNewId();
const long Tore::ID_TEXTCTRL6 = wxNewId();
const long Tore::ID_TEXTCTRL7 = wxNewId();
const long Tore::ID_STATICTEXT8 = wxNewId();
const long Tore::ID_STATICTEXT9 = wxNewId();
const long Tore::ID_SPINCTRL1 = wxNewId();
const long Tore::ID_SPINCTRL2 = wxNewId();
const long Tore::ID_BUTTON1 = wxNewId();
const long Tore::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Tore,wxDialog)
	//(*EventTable(Tore)
	//*)
END_EVENT_TABLE()

Tore::Tore(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Tore::Tore Création d'une forme générique en Tore
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */
	//(*Initialize(Tore)
	Create(parent, id, _T("Ajout d\'un tore"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(473,167));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Rayons : Moyen :"), wxPoint(8,10), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_RayonMoyen = new wxTextCtrl(this, ID_TEXTCTRL1, _T("2.0"), wxPoint(104,8), wxSize(112,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("du CercIe générateur :"), wxPoint(232,10), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_RayonCercle = new wxTextCtrl(this, ID_TEXTCTRL2, _T("1.0"), wxPoint(352,8), wxSize(112,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Centre du tore : X :"), wxPoint(8,42), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Y :"), wxPoint(228,42), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Z :"), wxPoint(352,42), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(120,40), wxSize(96,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(248,40), wxSize(96,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL5, _T("0.0"), wxPoint(368,40), wxSize(96,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre de secteurs :"), wxPoint(8,74), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Nombre de points/cercle :"), wxPoint(248,74), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	TextCtrl_NbSecteurs = new wxTextCtrl(this, ID_TEXTCTRL6, _T("16"), wxPoint(144,72), wxSize(72,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	TextCtrl_NbPts = new wxTextCtrl(this, ID_TEXTCTRL7, _T("16"), wxPoint(392,72), wxSize(72,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Groupe :"), wxPoint(8,106), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Numéro de Matériau :"), wxPoint(248,106), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(146,104), wxSize(72,24), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(394,104), wxSize(72,24), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(136,136), wxSize(83,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_OK->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(248,136), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Tore::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Tore::OnSpinCtrl_MateriauChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Tore::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Tore::OnButton_AnnulerClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Tore::OnClose);
	//*)

    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

Tore::~Tore()
{
	//(*Destroy(Tore)
	//*)
}

void Tore::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Tore::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Tore::genereFacettes(BddInter* Element, Object *p_Objet)
{
    int i, j, istep;
    int i0,i1,i2,i3;
    std::vector<int> Numeros;

    int numero = 1;
    int Nb_facettes = n_secteurs*NbPoints;

    Element->str.clear();
    Element->N_elements = Nb_facettes;
    Element->makeface();
    Element->makeluminance();

    p_Objet->Nb_facettes = p_Objet->Nb_luminances = Nb_facettes;   // utile ?

    for (j=0 ; j<n_secteurs ; j++) {
        istep = j*NbPoints;
        for( i=1; i<=NbPoints; i++ ) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1; i2=i1+NbPoints;
            i0 = i+istep;
            i3 = i0+NbPoints;
            i2 += istep ; i1 += istep;
            if (i2 > Nb_facettes) i2 -= Nb_facettes;
            if (i3 > Nb_facettes) i3 -= Nb_facettes;
            Numeros = {i0, i1, i2, i3};             // Initialisation du vecteur des numéros de sommet (4 points)
            Element->Set_numeros(Numeros);
            Element->N_elements = numero;
            Element->make1face();
            Element->make1luminance();
            p_Objet->Facelist[numero-1].flat = false;
            numero++;
        }
    }
}

void Tore::genereSommets(BddInter* Element, Object *p_Objet)
{
// Génère les sommets, les normales aux sommets et les normales aux barycentres

    double phi, psi, dpsi, dphi, spsi, cpsi, sphi, cphi, psi_nb, phi_nb, spsi_nb, cpsi_nb, sphi_nb, cphi_nb ;
    double XX, YY, ZZ;
    int i, j;

    double Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    int Nb_sommets = n_secteurs*NbPoints;
    int numero = 1;;

    p_Objet->Nb_sommets = p_Objet->Nb_vecteurs = p_Objet->Nb_normales = Nb_sommets;   // utile ?

    Element->str.clear();
    Element->N_elements = Nb_sommets;
    Element->makesommet() ;
    Element->makevecteur();
    Element->makenormale();

    dpsi   =  2.0*M_PI/n_secteurs;
    dphi   = -2.0*M_PI/NbPoints  ;
    psi    =  0.0;
    psi_nb = dpsi/2.;

    for (j=0 ; j<n_secteurs ; j++) {
        cpsi = cos(psi);
        spsi = sin(psi);
        cpsi_nb = cos(psi_nb);
        spsi_nb = sin(psi_nb);
        phi     = 0.0;
        phi_nb  = dphi/2.;

        for( i=0; i<NbPoints; i++ ) {
            cphi = cos(phi);
            sphi = sin(phi);
            XX   = Xc + cpsi*(rayon_moyen + cphi*rayon_cercle);
            YY   = Yc + spsi*(rayon_moyen + cphi*rayon_cercle);
            ZZ   = Zc +                     sphi*rayon_cercle ;
            Element->N_elements = numero;
            Element->Setxyz(XX,YY,ZZ);
            Element->make1sommet();     // Sommet

            XX = cpsi*cphi;
            YY = spsi*cphi;
            ZZ =      sphi;
            Element->Setxyz(XX,YY,ZZ);
            Element->make1vecteur();    // Normale aux sommet

            cphi_nb = cos(phi_nb);
            sphi_nb = sin(phi_nb);
            XX = cpsi_nb*cphi_nb ;
            YY = spsi_nb*cphi_nb ;
            ZZ =         sphi_nb ;
            Element->Setxyz(XX,YY,ZZ);  // Normale au barycentre
            Element->make1normale();

            phi    += dphi;
            phi_nb += dphi;
            numero++;
        }
        psi    += dpsi;
        psi_nb += dpsi;
    }
}

void Tore::genereTore()
{
    wxString num_obj;
    int new_num;
    Object *p_Objet;

    BddInter* Element = MAIN->Element;

//  Récupérer plutôt le numéro du dernier objet au lieu de l'indice et l'incrémenter
    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetValue() +1;
    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Tore - ") + num_obj;
    Element->makeobjet();
    Element->Objetlist.rbegin()->primitive = true; // <=> (Element->Objetlist.end()-1)->primitive = true;
    int indiceObjet = Element->indiceObjet_courant; //Element->Objetlist.size() -1; // Car c'est le dernier ! MaJ dans makeobjet

    printf("centre : %f %f %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2]);
    printf("Rayons moyen/cercle  : %f / %f\n",rayon_moyen,rayon_cercle);
    printf("Nb secteurs/NbPoints : %d / %d\n",n_secteurs,NbPoints);
    printf("Numéros Groupe/Mat.  : %d / %d\n",numeroGroupe,numeroMateriau);

    p_Objet = &(Element->Objetlist[indiceObjet]);

    genereFacettes(Element, p_Objet);   // Générer les facettes avant les sommets !
    genereSommets (Element, p_Objet);   // car Facelist y est utilisé !
    int Nb_facettes = NbPoints*n_secteurs ;
    p_Objet->flat = false;
    Element->genereAttributsFacettes(p_Objet, Nb_facettes, numeroGroupe, numeroMateriau);

    Element->GenereTableauPointsFacettes(p_Objet);
    Element->GenereTableauAretes_OK = true;
    Element->GenereTableauAretes        (p_Objet);
    Element->GenereListeGroupesMateriaux(p_Objet);

    Element->bdd_modifiee = true;
}

void Tore::OnButton_OKClick(wxCommandEvent& event)
{
    wxString str;

    BddInter* Element = MAIN->Element;

    rayon_moyen  = wxAtof(TextCtrl_RayonMoyen ->GetValue());
    rayon_cercle = wxAtof(TextCtrl_RayonCercle->GetValue());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    n_secteurs    = wxAtoi(TextCtrl_NbSecteurs->GetValue());
    NbPoints      = wxAtoi(TextCtrl_NbPts->GetValue());
    numeroGroupe  = SpinCtrl_Groupe  ->GetValue() ;     // Par précaution, mais déjà fait via OnSpinCtrl !
    numeroMateriau= SpinCtrl_Materiau->GetValue() ;     // idem

    genereTore();

    Element->type = 1;  // Marquer comme si c'était un fichier .bdd
    Element->type_new = 1;
    Element->m_gllist = 0;

    Element->searchMin_Max();
    Element->m_loaded = true;
    Element->OK_ToSave= true;
    Element->Refresh();
    Hide();
}

void Tore::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Tore::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}
