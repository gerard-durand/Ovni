#include "ManipulationsPanel.h"

//(*InternalHeaders(ManipulationsPanel)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ManipulationsPanel)
const long ManipulationsPanel::ID_STATICTEXT1 = wxNewId();
const long ManipulationsPanel::ID_BUTTON1 = wxNewId();
const long ManipulationsPanel::ID_BUTTON2 = wxNewId();
const long ManipulationsPanel::ID_BUTTON3 = wxNewId();
const long ManipulationsPanel::ID_STATICTEXT2 = wxNewId();
const long ManipulationsPanel::ID_CHECKBOX1 = wxNewId();
const long ManipulationsPanel::ID_CHECKBOX2 = wxNewId();
const long ManipulationsPanel::ID_CHECKBOX3 = wxNewId();
const long ManipulationsPanel::ID_BUTTON4 = wxNewId();
const long ManipulationsPanel::ID_BUTTON5 = wxNewId();
const long ManipulationsPanel::ID_STATICLINE1 = wxNewId();
const long ManipulationsPanel::ID_BUTTON6 = wxNewId();
const long ManipulationsPanel::ID_BUTTON7 = wxNewId();
const long ManipulationsPanel::ID_STATICLINE2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ManipulationsPanel,wxDialog)
	//(*EventTable(ManipulationsPanel)
	//*)
END_EVENT_TABLE()

ManipulationsPanel::ManipulationsPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ManipulationsPanel)
	Create(parent, id, _T("Manipulations d\'Objet"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE|wxCLOSE_BOX, _T("id"));
	SetClientSize(wxSize(254,271));
	Move(wxPoint(20,20));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Modifications de l\'Objet"), wxPoint(0,0), wxSize(256,16), wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	StaticText1->SetForegroundColour(wxColour(255,255,255));
	StaticText1->SetBackgroundColour(wxColour(0,0,0));
	Button_Translation = new wxButton(this, ID_BUTTON1, _T("Translation"), wxPoint(32,24), wxSize(192,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Translation->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Rotation = new wxButton(this, ID_BUTTON2, _T("Rotation"), wxPoint(32,48), wxSize(192,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Rotation->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Scale = new wxButton(this, ID_BUTTON3, _T("Mise à l\'échelle"), wxPoint(32,72), wxSize(192,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Scale->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Création d\'un objet par symétrie / Plan"), wxPoint(0,104), wxSize(256,16), wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
	StaticText2->SetForegroundColour(wxColour(255,255,255));
	StaticText2->SetBackgroundColour(wxColour(0,0,0));
	CheckBox_X = new wxCheckBox(this, ID_CHECKBOX1, _T("X"), wxPoint(32,128), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_X->SetValue(false);
	CheckBox_X->SetForegroundColour(wxColour(255,0,0));
	CheckBox_Y = new wxCheckBox(this, ID_CHECKBOX2, _T("Y"), wxPoint(112,128), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	CheckBox_Y->SetValue(false);
	CheckBox_Y->SetForegroundColour(wxColour(0,255,0));
	CheckBox_Z = new wxCheckBox(this, ID_CHECKBOX3, _T("Z"), wxPoint(192,128), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	CheckBox_Z->SetValue(false);
	CheckBox_Z->SetForegroundColour(wxColour(0,0,255));
	Button_Creer = new wxButton(this, ID_BUTTON4, _T("Créer l\'objet"), wxPoint(16,152), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	Button_Creer->Disable();
	Button_Creer->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Raz = new wxButton(this, ID_BUTTON5, _T("Remise à zéro"), wxPoint(128,152), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button_Raz->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxPoint(0,180), wxSize(256,4), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	Button_Quitter = new wxButton(this, ID_BUTTON6, _T("Fermer"), wxPoint(72,240), wxSize(104,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_NewObjet = new wxButton(this, ID_BUTTON7, _T("Facettes sélectionnées => Nouvel Objet"), wxPoint(8,192), wxSize(234,24), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	Button_NewObjet->Disable();
	Button_NewObjet->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxPoint(0,224), wxSize(256,4), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_TranslationClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_RotationClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_ScaleClick);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnCheckBox_XClick);
	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnCheckBox_YClick);
	Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnCheckBox_ZClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_CreerClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_RazClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_QuitterClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ManipulationsPanel::OnButton_NewObjetClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&ManipulationsPanel::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ManipulationsPanel::OnClose);
	//*)

    this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ManipulationsPanel::~ManipulationsPanel()
{
	//(*Destroy(ManipulationsPanel)
	//*)
}

void ManipulationsPanel::OnInit(wxInitDialogEvent& event)
{
    CheckBox_XYZ = false;
    CheckBox_X->SetValue(false);
    CheckBox_Y->SetValue(false);
    CheckBox_Z->SetValue(false);
    Button_Creer->Disable();
    MAIN->Element->Symetrie_Objets = false;
}

void ManipulationsPanel::OnClose(wxCloseEvent& event)
{
    if (CheckBox_XYZ) {
        CheckBox_XYZ = false;
        CheckBox_X->SetValue(false);
        CheckBox_Y->SetValue(false);
        CheckBox_Z->SetValue(false);
        Button_Creer->Disable();
        MAIN->Element->Symetrie_Objets = false;
        MAIN->Element->Refresh();
    }
    Hide();
}

void ManipulationsPanel::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ManipulationsPanel::OnButton_TranslationClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    wxString wxTexte;

    Element->MTrans->SpinButton_XG->SetValue(0);        // Tout remettre à 0
    Element->MTrans->SpinButton_YG->SetValue(0);
    Element->MTrans->SpinButton_ZG->SetValue(0);
    Element->MTrans->SpinButton_XF->SetValue(0);
    Element->MTrans->SpinButton_YF->SetValue(0);
    Element->MTrans->SpinButton_ZF->SetValue(0);
    wxTexte.Printf(_T("%2.1f"),0.0);                    // En toute rigueur devrait être identique à format_1 dans TranslationPanel.cpp, mais le OnInit repasse par dessus
    Element->MTrans->TextCtrl_XG->SetValue(wxTexte);
    Element->MTrans->TextCtrl_YG->SetValue(wxTexte);
    Element->MTrans->TextCtrl_ZG->SetValue(wxTexte);
    Element->MTrans->TextCtrl_XF->SetValue(wxTexte);    // Devrait utiliser format_2, mais idem ci-dessus
    Element->MTrans->TextCtrl_YF->SetValue(wxTexte);
    Element->MTrans->TextCtrl_ZF->SetValue(wxTexte);
    Element->MTrans->Show();
}

void ManipulationsPanel::OnButton_RotationClick(wxCommandEvent& event)
{
    MAIN->Element->MRotation->Show();
}

void ManipulationsPanel::OnButton_ScaleClick(wxCommandEvent& event)
{
    MAIN->Element->MScale->Show();
}

void ManipulationsPanel::OnCheckBox_XClick(wxCommandEvent& event)
{
    CheckBox_XYZ = (CheckBox_X->GetValue() || CheckBox_Y->GetValue() || CheckBox_Z->GetValue());
    MAIN->Element->Symetrie_Objets = false;
    if (CheckBox_XYZ) {
        Button_Creer->Enable();
        CreerBoiteObjet();
    } else Button_Creer->Disable();
    MAIN->Element->Refresh();
}

void ManipulationsPanel::OnCheckBox_YClick(wxCommandEvent& event)
{
    CheckBox_XYZ = (CheckBox_X->GetValue() || CheckBox_Y->GetValue() || CheckBox_Z->GetValue());
    MAIN->Element->Symetrie_Objets = false;
    if (CheckBox_XYZ) {
        Button_Creer->Enable();
        CreerBoiteObjet();
    } else Button_Creer->Disable();
    MAIN->Element->Refresh();
}

void ManipulationsPanel::OnCheckBox_ZClick(wxCommandEvent& event)
{
    CheckBox_XYZ = (CheckBox_X->GetValue() || CheckBox_Y->GetValue() || CheckBox_Z->GetValue());
    MAIN->Element->Symetrie_Objets = false;
    if (CheckBox_XYZ) {
        Button_Creer->Enable();
        CreerBoiteObjet();
    } else Button_Creer->Disable();
    MAIN->Element->Refresh();
}

void ManipulationsPanel::CreerBoiteObjet()
{
    BddInter *Element = MAIN->Element;

    unsigned int n_val, i, j, nb_fac;
    bool first;
    Object   *objet_courant;
    Sommet   *sommet_courant;

    x1b1=x2b1=y1b1=y2b1=z1b1=z2b1=0.0;

    n_val   = Element->listeObjets.size();
    auto it = Element->listeObjets.begin();

    first = true;
    for (i=0; i<n_val; i++, it++) {
        int o = *it;
//        printf("Objet : %d",o);
        objet_courant = &(Element->Objetlist[o]);
        unsigned int ns = objet_courant->Sommetlist.size();

        for (j=0; j<ns; j++) {
            sommet_courant = &(objet_courant->Sommetlist[j]);
            if (sommet_courant->point.empty()) continue; // Ne pas traiter un éventuel point vide (non initialisé)
            if (first) {
                x1b1 = x2b1 = sommet_courant->point[0];
                y1b1 = y2b1 = sommet_courant->point[1];
                z1b1 = z2b1 = sommet_courant->point[2];
                first= false;
            } else {
                x1b1 = std::min(x1b1,(double)sommet_courant->point[0]); // min en x
                x2b1 = std::max(x2b1,(double)sommet_courant->point[0]); // max en x
                y1b1 = std::min(y1b1,(double)sommet_courant->point[1]); // min en y
                y2b1 = std::max(y2b1,(double)sommet_courant->point[1]); // max en y
                z1b1 = std::min(z1b1,(double)sommet_courant->point[2]); // min en z
                z2b1 = std::max(z2b1,(double)sommet_courant->point[2]); // max en z
            }
        }
    }
    Element->Symetrie_Objets = true;
    Element->x1_b1 = Element->x1_b2 = x1b1; // Recopie de la boîte originale dans la boîte symétrisée (initialisation)
    Element->x2_b1 = Element->x2_b2 = x2b1;
    Element->y1_b1 = Element->y1_b2 = y1b1;
    Element->y2_b1 = Element->y2_b2 = y2b1;
    Element->z1_b1 = Element->z1_b2 = z1b1;
    Element->z2_b1 = Element->z2_b2 = z2b1;

    if (CheckBox_X->GetValue()) {       // Symétrise la boîte en X
        Element->x1_b2 *= -1.;
        Element->x2_b2 *= -1.;
    }
    if (CheckBox_Y->GetValue()) {       // Symétrise la boîte en Y
        Element->y1_b2 *= -1.;
        Element->y2_b2 *= -1.;
    }
    if (CheckBox_Z->GetValue()) {       // Symétrise la boîte en Z
        Element->z1_b2 *= -1.;
        Element->z2_b2 *= -1.;
    }
}

void ManipulationsPanel::OnButton_CreerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    bool     symetries_impaires;
    unsigned int n_val, i, j, nb_fac;
    Object   *objet_courant, *objet_nouveau;
    Sommet   *sommet_courant;
    Vecteur  *vecteur_courant;
    Face     *facette_courante;
    unsigned int indiceObjet, new_numero, ns;
    int numero_max;
    std::vector<int> NumerosSommets;
    std::vector<int> ReverseSommets;
    int nb_symetries = 0;

    bool Symetriser_X = CheckBox_X->GetValue();
    bool Symetriser_Y = CheckBox_Y->GetValue();
    bool Symetriser_Z = CheckBox_Z->GetValue();

    if (Symetriser_X) nb_symetries++;
    if (Symetriser_Y) nb_symetries++;
    if (Symetriser_Z) nb_symetries++;
    symetries_impaires = (bool)(nb_symetries % 2);              // Parité du nombre de symétries

    n_val   = Element->listeObjets.size();
    auto it = Element->listeObjets.begin();

    indiceObjet = Element->Objetlist.size() -1;                 // Numéro d'indice du dernier objet
    numero_max  = 0;
    for (i=0; i<=indiceObjet; i++) numero_max = std::max(numero_max,Element->Objetlist[i].GetValue());
//    printf("\nNumero maximal : %d\n",numero_max);
    new_numero = ((numero_max/10) +1)*10;

    for (i=0; i<n_val; i++, it++) {
        int o = *it;
        Element->Objetlist.push_back(Element->Objetlist[o]);    // Push_back d'une copie de l'objet initial
        objet_courant = &(Element->Objetlist[o]);
        indiceObjet = Element->Objetlist.size() -1;             // Numéro d'indice de l'objet créé
        Element->indiceObjet_courant = indiceObjet;             // Enregistrer ce numéro d'indice dans BddInter

        objet_nouveau = &(Element->Objetlist[indiceObjet]);
        objet_nouveau->SetValue(new_numero);                    // Nouveau numéro pour le nouvel objet
        wxString Ancien_nom  = objet_courant->GetwxName();
        wxString Nouveau_nom = objet_courant->GetwxName() + _T(" - par symetrie");  // Ajouter "par symetrie" au nom de l'objet original
        objet_nouveau->SetName(Nouveau_nom);
        objet_courant->SetName(Ancien_nom);                     // Bizzarement objet_courant a perdu son nom. Restitution ici !

        // Changer certaines valeurs selon les options de symétrisation X, Y et/ou Z
        // 1) Changer les coordonnées des sommets
        ns = objet_nouveau->Sommetlist.size();
        for (j=0; j<ns; j++) {
            sommet_courant = &(objet_nouveau->Sommetlist[j]);
            if (sommet_courant->point.empty()) continue;        // Ne pas traiter un éventuel point vide (non initialisé)
            if (Symetriser_X) sommet_courant->point[0] *= -1;
            if (Symetriser_Y) sommet_courant->point[1] *= -1;
            if (Symetriser_Z) sommet_courant->point[2] *= -1;
        }
        // 2) Changer les coordonnées des normales aux barycentres et éventuellement inverser le sens de parcours des sommets de la facette (si le nombre de symétries est impair)
        nb_fac = objet_nouveau->Facelist.size();
        for (j=0; j<nb_fac; j++) {
            facette_courante = &(objet_nouveau->Facelist[j]);
            if (Symetriser_X) facette_courante->normale_b[0] *= -1;
            if (Symetriser_Y) facette_courante->normale_b[1] *= -1;
            if (Symetriser_Z) facette_courante->normale_b[2] *= -1;
            if (symetries_impaires) {
                NumerosSommets = facette_courante->getF_sommets();
                ReverseSommets.clear();
                for (auto it2=NumerosSommets.crbegin(); it2 != NumerosSommets.crend(); ++it2) ReverseSommets.push_back(*it2);
                facette_courante->setFsommet(ReverseSommets);
                NumerosSommets = facette_courante->getL_sommets();
                if (NumerosSommets.size() > 0) {                // Ne pas faire s'il n'y a pas de normales aux sommets
                    ReverseSommets.clear();
                    for (auto it2=NumerosSommets.crbegin(); it2 != NumerosSommets.crend(); ++it2) ReverseSommets.push_back(*it2);
                    facette_courante->setLsommet(ReverseSommets);
                }
            }
        }
        // 3) Changer les coordonnées des normales aux sommets
        ns = objet_courant->Vecteurlist.size();
        for (j=0; j<ns; j++) {
            vecteur_courant = &(objet_nouveau->Vecteurlist[j]);
            if (Symetriser_X) vecteur_courant->point[0] *= -1;
            if (Symetriser_Y) vecteur_courant->point[1] *= -1;
            if (Symetriser_Z) vecteur_courant->point[2] *= -1;
        }
        objet_nouveau->selected = false;    // Ne pas marquer le nouvel objet comme étant sélectionné
    }
//    Element->Symetrie_Objets = false; // En commentaire pour laisser affichée la boîte englobante du/des objet(s) sélectionnés
    Element->m_gllist = 0;
    Element->Search_Min_Max();          // Mettre à jour les min-max pour la boîte englobante
    Element->bdd_modifiee = true;
    Element->Refresh();
//    ToDo();
}

void ManipulationsPanel::OnButton_RazClick(wxCommandEvent& event)
{
    CheckBox_XYZ = false;
    CheckBox_X->SetValue(false);
    CheckBox_Y->SetValue(false);
    CheckBox_Z->SetValue(false);
    Button_Creer->Disable();
    MAIN->Element->Symetrie_Objets = false;
    MAIN->Element->Refresh();
}

void ManipulationsPanel::NewObjet_SelectedFacets(wxCommandEvent& event){    // Pour appeler depuis OvniFrame::OnPopup_CreerObjetFacettesSelected
    OnButton_NewObjetClick(event) ;                                         // ManipulationsPanel::OnButton_NewObjetClick qui est private ici
}

void ManipulationsPanel::OnButton_NewObjetClick(wxCommandEvent& event)
{
    Object   *objet_courant, *objet_nouveau;
    BddInter *Element = MAIN->Element;
    static bool premiere_fois = true;

    int i, indice_objet, numero_max, new_numero;
    long long unsigned int i_obj, i_fac, new_indice;
    bool OK = true;

    int nb_select = Element->ToSelect.ListeSelect.size();       // Nombre de facettes sélectionnées
    if (nb_select <= 0) return;                                 // Aucune, retour direct (peut arriver si ManipulationPanel était déjà ouvert)
    indice_objet  = Element->ToSelect.ListeSelect[0].objet;     // Indice de l'objet de la première facette sélectionnée

    for (i=1; i<nb_select; i++) {                               // On vérifie que toutes les facettes sélectionnées appartiennent au même objet
        if (Element->ToSelect.ListeSelect[i].objet != indice_objet) OK = false;
    }
    if (!OK) {
        DisplayMessage(_T("Ne fonctionne pas sur des facettes\nappartenant à plusieurs objets"), true);
        return;
    }

    printf("Objet à traiter : indice %d, nom  : %s\n",indice_objet,Element->Objetlist[indice_objet].GetName());
    printf("Nombre de facettes sélectionnées : %d\n" ,nb_select);

    numero_max = 0;
    new_indice = Element->Objetlist.size();
    for (i_obj =0; i_obj<new_indice; i_obj++) numero_max = std::max(numero_max,Element->Objetlist[i_obj].GetValue());
    if (premiere_fois)
        new_numero = ((numero_max/10) +1)*10;   // Pour bien différentier ce qu'on sépare, la première fois
    else
        new_numero = ++numero_max;              // ensuite, incrémenter simplement de 1

    // Provisoire : OK mais pas optimisé en mémoire car dédouble l'objet

    Element->Objetlist.push_back(Element->Objetlist[indice_objet]);         // Push_back d'une copie de l'objet initial en entier
    objet_courant = &(Element->Objetlist[indice_objet]);
    Element->indiceObjet_courant = new_indice;                              // Enregistrer le numéro d'indice dans BddInter
    objet_nouveau = &(Element->Objetlist[new_indice]);
    objet_nouveau->SetValue(new_numero);
//    wxString Ancien_nom  = objet_courant->GetwxName();
    wxString Nouveau_nom = objet_courant->GetwxName() + _T(" - Extrait");   // Modifier le nom (ou bien le demander à l'écran)
    objet_nouveau->SetName(Nouveau_nom);
//    objet_courant->SetName(Ancien_nom);   // Inutile ici, pourtant il a fallu le faire dans OnButton_CreerClick !

    for (i_fac=0;i_fac<objet_courant->Facelist.size();i_fac++) {
        objet_nouveau->Facelist[i_fac].selected = false;    // Dans objet_nouveau, mettre l'attribut selected sur toutes les facettes à false
        if (objet_courant->Facelist[i_fac].selected) {
            objet_courant->Facelist[i_fac].deleted = true;  // Facette sélectionnée dans objet_courant     => la supprimer de objet_courant
        } else {
            objet_nouveau->Facelist[i_fac].deleted = true;  // Facette non sélectionnée dans objet_courant => la supprimer de objet_nouveau
        }
    }

    // Désélectionner les facettes dans objet_courant !
    wxKeyEvent key_event;
    key_event.m_keyCode = 'S';
    Element->OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection de facettes

    Element->bdd_modifiee = true;

    // En sortie, peut-être forcer (simuler) un clic sur Quitter, mais peut généer si on fait plusieurs fois de suite cette opération

/*  En toute rigueur, il faut simplifier la bdd : le faire d'office ou le suggérer ?
    Le faire d'office peut s'avérer long sur de grosses bdd, surtout si on le fait plusieurs fois de suite.
    Autant le faire une seule fois, à la demande. Le risque est toutefois de prendre temporairement trop de place en mémoire,
    car les facettes ne sont que marquées "deleted", mais pas supprimées, et les points/normales/vecteurs non utilisés restent en mémoire.
*/
    if (premiere_fois)
        DisplayMessage(_T("Il faudrait simplifier la Bdd de préférence\navant de l'enregistrer"), true);
    premiere_fois = false;  // Un seul avertissement la première fois... Vous avez été prévenus non ?
}

//void ManipulationsPanel::ToDo()
//{
//    DisplayMessage(_T("Pas encore complètement opérationnel"), true);
//}

