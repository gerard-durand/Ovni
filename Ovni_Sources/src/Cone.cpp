#include "Cone.h"
#include "vector3d.h"

//(*InternalHeaders(Cone)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Cone)
const long Cone::ID_STATICTEXT1 = wxNewId();
const long Cone::ID_TEXTCTRL1 = wxNewId();
const long Cone::ID_STATICTEXT2 = wxNewId();
const long Cone::ID_TEXTCTRL2 = wxNewId();
const long Cone::ID_STATICTEXT3 = wxNewId();
const long Cone::ID_TEXTCTRL3 = wxNewId();
const long Cone::ID_STATICTEXT4 = wxNewId();
const long Cone::ID_TEXTCTRL4 = wxNewId();
const long Cone::ID_STATICTEXT5 = wxNewId();
const long Cone::ID_TEXTCTRL5 = wxNewId();
const long Cone::ID_STATICTEXT6 = wxNewId();
const long Cone::ID_TEXTCTRL6 = wxNewId();
const long Cone::ID_STATICTEXT7 = wxNewId();
const long Cone::ID_SPINCTRL1 = wxNewId();
const long Cone::ID_STATICTEXT8 = wxNewId();
const long Cone::ID_SPINCTRL2 = wxNewId();
const long Cone::ID_BUTTON1 = wxNewId();
const long Cone::ID_BUTTON2 = wxNewId();
const long Cone::ID_CHECKBOX1 = wxNewId();
const long Cone::ID_STATICTEXT9 = wxNewId();
const long Cone::ID_SPINCTRL3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Cone,wxDialog)
	//(*EventTable(Cone)
	//*)
END_EVENT_TABLE()

Cone::Cone(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Cone)
	Create(parent, id, _T("Ajout d\'un cône"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(466,220));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Longueur :"), wxPoint(152,11), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Longueur = new wxTextCtrl(this, ID_TEXTCTRL1, _T("2.0"), wxPoint(224,8), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Rayon du cercle de base :"), wxPoint(80,35), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	TextCtrl_Rayon = new wxTextCtrl(this, ID_TEXTCTRL2, _T("1.0"), wxPoint(224,32), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Coordonnées du centre du cône : X:"), wxPoint(4,59), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	TextCtrl_X = new wxTextCtrl(this, ID_TEXTCTRL3, _T("0.0"), wxPoint(192,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Y :"), wxPoint(272,59), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Y = new wxTextCtrl(this, ID_TEXTCTRL4, _T("0.0"), wxPoint(288,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _T("Z :"), wxPoint(368,59), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	TextCtrl_Z = new wxTextCtrl(this, ID_TEXTCTRL5, _T("0.0"), wxPoint(384,56), wxSize(72,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _T("Nombre de points générateurs du cercle :"), wxPoint(4,83), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	TextCtrl_NbPoints = new wxTextCtrl(this, ID_TEXTCTRL6, _T("16"), wxPoint(224,80), wxSize(96,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _T("Numéro de Groupe :"), wxPoint(4,107), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	SpinCtrl_Groupe = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(120,104), wxSize(96,21), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_Groupe->SetValue(_T("0"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _T("Numéro de Matériau :"), wxPoint(232,107), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	SpinCtrl_Materiau = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxPoint(360,104), wxSize(96,21), 0, 0, 100, 0, _T("ID_SPINCTRL2"));
	SpinCtrl_Materiau->SetValue(_T("0"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Accepter"), wxPoint(120,192), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Annuler = new wxButton(this, ID_BUTTON2, _T("Annuler"), wxPoint(232,192), wxSize(96,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	CheckBox_FermerCone = new wxCheckBox(this, ID_CHECKBOX1, _T("Fermer le cône à sa base"), wxPoint(8,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_FermerCone->SetValue(false);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _T("Nombre de secteurs"), wxPoint(4,139), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	SpinCtrl_NbSecteurs = new wxSpinCtrl(this, ID_SPINCTRL3, _T("1"), wxPoint(120,136), wxSize(96,21), 0, 1, 20, 1, _T("ID_SPINCTRL3"));
	SpinCtrl_NbSecteurs->SetValue(_T("1"));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_GroupeChange);
	Connect(ID_SPINCTRL2,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_MateriauChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cone::OnButton_OKClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Cone::OnButton_AnnulerClick);
	Connect(ID_SPINCTRL3,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&Cone::OnSpinCtrl_NbSecteursChange);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Cone::OnClose);
	//*)
//	this->Cone_Parent=parent;
    this->MAIN = dynamic_cast<OvniFrame*>(parent);

}

Cone::~Cone()
{
	//(*Destroy(Cone)
	//*)
}

void Cone::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Cone::OnButton_AnnulerClick(wxCommandEvent& event)
{
// Boutton Annuler <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Cone::genereSommets(BddInter* Element, int indiceObjet)
{
    int i,j, numero;
    float angle,pas_angle;
    float Xp, XStep;
    float Xc = centre_primitive[0] , Yc = centre_primitive[1] , Zc = centre_primitive[2];
    float rayon_j;

    angle = 0.;
    pas_angle = 2.*M_PI/NbPoints;
    int NbSommets = n_secteurs*NbPoints +1;

    Element->str.clear();
    Element->N_elements = NbSommets;
    Element->makesommet();

    Element->Objetlist[indiceObjet].Nb_sommets = NbSommets;

    // Sommets du/des cercles générateurs
    numero = 1;
    Xp = Xc - longueur/2.;	        // Premier plan
    XStep = longueur/n_secteurs;
    for (j=0; j<n_secteurs ; j++) {
        angle = 0.;
        rayon_j = (n_secteurs-j)*rayon/n_secteurs;
        for (i=0; i<NbPoints; i++) {
            Element->N_elements=numero; Element->Setxyz(Xp,Yc+rayon_j*cos(angle),Zc+rayon_j*sin(angle)); Element->make1sommet();
            angle += pas_angle;
            numero++;
        }
		Xp += XStep;                // Plans suivants
}
    // Sommet du cône
    Element->N_elements=numero; Element->Setxyz(Xc+longueur/2,Yc,Zc); Element->make1sommet();

}

//void Cone::genereNormalesFacettes(BddInter* Element)
//{
//// N'est pas adapté à n_slices > 2
//
//// Revient au même si on utilise la fonction BddInter::genereNormalesFacettes
//// ATTENTION : n'utilise que les 3 premiers points de chaque facette
//
//    int numero;
//    std::vector<float> xyz_cercle, sommet_cone;
//    std::vector<int>   NumerosSommets;
//
//    int indiceObjet = Element->Objetlist.size() -1;
//
//    // Normales aux facettes
//    Element->str.clear();
//    Element->N_elements = NbPoints;
//    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
//    Element->makenormale();
//
//    sommet_cone = Element->Sommet1list[indiceObjet][NbPoints].getPoint();
////    sommet_cone = Element->Sommet1list[indiceObjet][NbPoints+1].getPoint();
//    Vector3D Sommet(sommet_cone[0],sommet_cone[1],sommet_cone[2]);
//    numero = 1;
//    for (int i=0; i<NbPoints; i++) {
//        NumerosSommets = Element->Face1list[indiceObjet][i].getF_sommets();
//        xyz_cercle = Element->Sommet1list[indiceObjet][NumerosSommets[0]-1].getPoint();
////        xyz_cercle = Element->Sommet1list[indiceObjet][NumerosSommets[0]].getPoint();
//        Vector3D P1(xyz_cercle[0],xyz_cercle[1],xyz_cercle[2]);
//        xyz_cercle = Element->Sommet1list[indiceObjet][NumerosSommets[1]-1].getPoint();
////        xyz_cercle = Element->Sommet1list[indiceObjet][NumerosSommets[1]].getPoint();
//        Vector3D P2(xyz_cercle[0],xyz_cercle[1],xyz_cercle[2]);
//        P1 -= Sommet;
//        P2 -= Sommet;
//        Vector3D Vn = P1.crossProduct(P2);
//        Vn.normalize();
//        Element->N_elements=numero; Element->Setxyz(Vn.X,Vn.Y,Vn.Z); Element->make1normale();
//        numero++;
//    }
//    if (CheckBox_FermerCone->IsChecked()) {
//        Element->N_elements=numero;
//        Element->Setxyz(-1.,0.,0.);
//        Element->make1normale();
//    }
//}

void Cone::genereLuminances(BddInter* Element, int indiceObjet)
{
    wxString str_loc;
    int numero,i1,i,j;

    Element->str.clear();
    Element->N_elements = NbPoints*n_slices;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
//    (Element->Objetlist.end()-1)->Nb_luminances = Element->N_elements;
//    Element->Objetlist[indiceObjet].Nb_luminances = Element->N_elements;
    Element->makeluminance();
    numero= 1;

// Traité comme un cylindre, y compris sur les facettes contenant le sommet du cône, car considérées à 4 points (sommet doublé !)
    for (j=0; j < n_secteurs ; j++) {
        for (i=1; i <= NbPoints ; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1;
            Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i,i1,i1,i) ; Element->make1luminance();
            numero++;
        }
    }
    if (CheckBox_FermerCone->IsChecked()) {
        Element->str.Printf(_T("%d %d"),numero,NbPoints) ;
        str_loc.Printf(_T(" %d"),NbPoints+1);
        for (int j=0; j<NbPoints ; j++) Element->str += str_loc;
        Element->make1luminance();
    }
}

void Cone::genereNormalesSommets(BddInter* Element, int indiceObjet)
{
    // Normales aux sommets
    int numero;
    float angle,pas_angle;

    float hypotenuse = sqrt(longueur*longueur + rayon*rayon);
    float sin_sommet = rayon/hypotenuse;
    float cos_sommet = longueur/hypotenuse;

    pas_angle = 2.*M_PI/NbPoints;

    Element->str.clear();
    Element->N_elements = NbPoints;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;

    Element->Objetlist[indiceObjet].Nb_vecteurs = Element->N_elements;
    Element->makevecteur();
    numero= 1;
    angle = 0.;
    for (int i=0; i<NbPoints; i++) {
        Element->N_elements=numero; Element->Setxyz(sin_sommet,cos_sommet*cos(angle),cos_sommet*sin(angle));
        Element->make1vecteur();
        angle += pas_angle;
        numero++;
	}
    if (CheckBox_FermerCone->IsChecked()) {
        Element->N_elements=numero;
        Element->Setxyz(-1.,0.,0.);
        Element->make1vecteur();
    }
}

//void Cone::genereFacettes(OvniFrame* MAIN, int indiceObjet)
void Cone::genereFacettes(BddInter* Element, int indiceObjet)
{
    wxString str_loc;
    int numero,i,i0,i1,i2,i3;
    int istep;

    Element->str.clear();
    int NbFacettes = NbPoints*n_secteurs;
    Element->N_elements = NbFacettes;
    if (CheckBox_FermerCone->IsChecked()) Element->N_elements +=1;
    Element->makeface();

    Element->Objetlist[indiceObjet].Nb_facettes = Element->N_elements;
    numero = 1;

    // Facettes à 4 points (inter-secteurs)
    for (int j=0; j < n_secteurs-1 ; j++) {
        istep = j*NbPoints;
        for (i=1; i<=NbPoints; i++) {
            i1 = i+1 ; if (i1 > NbPoints) i1=1; i2=i1+NbPoints;
            i0 = i+istep;
            i3 = i0+NbPoints;
            i2 += istep ; i1 += istep;
            Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i0,i1,i2,i3); Element->make1face();
            numero++;
        }
    }
    // Facettes au sommet du cône
    istep = (n_secteurs-1)*NbPoints;
    i2    = NbFacettes +1;
    for (int i=1; i<=NbPoints; i++) {
        i1 = i+1 ; if (i1 > NbPoints) i1 = 1;
        i0 = i+istep;
        i1 += istep;
        Element->str.Printf(_T("%d 4 %d %d %d %d"),numero,i0,i1,i2,i2);  // Sommet doublé : n'est pas optimal mais évite des soucis en cas de recalcul des normales
        Element->make1face();
        numero++;
	}
    if (CheckBox_FermerCone->IsChecked()) {
        str_loc.Printf(_T("%d %d"),numero,NbPoints); Element->str = str_loc;
        for (i=NbPoints; i>=1 ; i--) {
            str_loc.Printf(_T(" %d"),i); Element->str += str_loc;
        }
        Element->make1face();
    }
}

void Cone::genereCone()
{
    wxString num_obj;
    int new_num;

    BddInter* Element = MAIN->Element;

//    int new_num = Element->Objetlist[(Element->Objetlist.size()-1)].GetValue() +1;
//  Récupérer plutôt le numéro du dernier objet au lieu de l'indice et l'incrémenter
    if (Element->Objetlist.size() == 0)
        new_num = 1;
    else
        new_num = Element->Objetlist.rbegin()->GetValue() +1;

    num_obj.Printf(_T("%d"),new_num);
    Element->str = _T("<OBJET> ") + num_obj + _T(" Cone - ") + num_obj;
    Element->makeobjet();
    Element->Objetlist.rbegin()->primitive = true;
//    printf("size : %d\n",Element->Objetlist.size());
    int indiceObjet = Element->indiceObjet_courant; // C'est le dernier !

    printf("centre : %f %f %f\nlongueur  : %f\n",centre_primitive[0],centre_primitive[1],centre_primitive[2],longueur);
    printf("rayon  : %f, Nombre de points : %d\n",rayon,NbPoints);
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
//    n_secteurs = 1; // Pour le moment !
    n_slices   = n_secteurs+1;
    int Nb_facettes = NbPoints*n_secteurs ;
    if (CheckBox_FermerCone->IsChecked()) Nb_facettes +=1;

    genereFacettes(Element, indiceObjet);
    genereSommets (Element, indiceObjet);
//    genereNormalesFacettes(MAIN);
    Element->genereNormalesFacettes (indiceObjet, Nb_facettes);
    Element->genereAttributsFacettes(indiceObjet, Nb_facettes, numeroGroupe, numeroMateriau);
    genereLuminances(Element, indiceObjet);
    Element->Objetlist[indiceObjet].flat = false;
    genereNormalesSommets(Element, indiceObjet);

    Element->GenereTableauPointsFacettes(&Element->Objetlist[indiceObjet]);
    Element->GenereTableauAretes(&Element->Objetlist[indiceObjet]);
    Element->GenereListeGroupesMateriaux(indiceObjet);

    Element->bdd_modifiee = true;
}

void Cone::OnButton_OKClick(wxCommandEvent& event)
{
    BddInter* Element = MAIN->Element;

    longueur = wxAtof(TextCtrl_Longueur->GetValue());
//    printf("size :%d\n",centre_cube.size());
    centre_primitive[0] = wxAtof(TextCtrl_X->GetValue());
    centre_primitive[1] = wxAtof(TextCtrl_Y->GetValue());
    centre_primitive[2] = wxAtof(TextCtrl_Z->GetValue());
    rayon    = wxAtof(TextCtrl_Rayon->GetValue());
    NbPoints = wxAtoi(TextCtrl_NbPoints->GetValue());
//    printf("centre : %f %f %f\narete  : %f\n",centre_cube[0],centre_cube[1],centre_cube[2],arete);
    numeroGroupe   = SpinCtrl_Groupe  ->GetValue() ;    // Par précaution, mais déjà fait !
    numeroMateriau = SpinCtrl_Materiau->GetValue() ;    // idem

    genereCone();

    Element->type = 1;  // Marquer comme si c'était un fichier .bdd
    Element->type_new = 1;
    Element->m_gllist = 0;

    Element->searchMin_Max();
    Element->m_loaded = true;
    Element->OK_ToSave= true;
    Element->Refresh();
    Hide();
}

void Cone::OnSpinCtrl_GroupeChange(wxSpinEvent& event)
{
    numeroGroupe  = SpinCtrl_Groupe->GetValue();
}

void Cone::OnSpinCtrl_MateriauChange(wxSpinEvent& event)
{
    numeroMateriau= SpinCtrl_Materiau->GetValue();
}

void Cone::OnSpinCtrl_NbSecteursChange(wxSpinEvent& event)
{
    n_secteurs = SpinCtrl_NbSecteurs->GetValue();
    n_slices   = n_secteurs +1 ;
}
