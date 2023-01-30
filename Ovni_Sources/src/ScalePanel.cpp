#include "ScalePanel.h"

//(*InternalHeaders(ScalePanel)
#include <wx/string.h>
//*)

//(*IdInit(ScalePanel)
const long ScalePanel::ID_STATICTEXT1 = wxNewId();
const long ScalePanel::ID_TEXTCTRL1 = wxNewId();
const long ScalePanel::ID_BUTTON1 = wxNewId();
const long ScalePanel::ID_STATICTEXT2 = wxNewId();
const long ScalePanel::ID_STATICTEXT3 = wxNewId();
const long ScalePanel::ID_STATICTEXT4 = wxNewId();
const long ScalePanel::ID_TEXTCTRL2 = wxNewId();
const long ScalePanel::ID_SPINBUTTON1 = wxNewId();
const long ScalePanel::ID_TEXTCTRL3 = wxNewId();
const long ScalePanel::ID_SPINBUTTON2 = wxNewId();
const long ScalePanel::ID_TEXTCTRL4 = wxNewId();
const long ScalePanel::ID_SPINBUTTON3 = wxNewId();
const long ScalePanel::ID_CHECKBOX1 = wxNewId();
const long ScalePanel::ID_BUTTON2 = wxNewId();
const long ScalePanel::ID_BUTTON3 = wxNewId();
//*)

wxString format_Sc=_T("%5.3f");

BEGIN_EVENT_TABLE(ScalePanel,wxDialog)
	//(*EventTable(ScalePanel)
	//*)
END_EVENT_TABLE()

ScalePanel::ScalePanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ScalePanel)
	Create(parent, id, _T("Mise à l\'échelle"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(285,240));
	Move(wxPoint(30,30));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Incrément :"), wxPoint(8,20), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	TextCtrl_Increment = new wxTextCtrl(this, ID_TEXTCTRL1, _T("0.010"), wxPoint(80,16), wxSize(112,24), wxTE_PROCESS_ENTER|wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_Valider = new wxButton(this, ID_BUTTON1, _T("Valider"), wxPoint(200,16), wxSize(80,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Mise à l\'échelle en X :"), wxPoint(16,76), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _T("Mise à l\'échelle en Y :"), wxPoint(16,108), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _T("Mise à l\'échelle en Z :"), wxPoint(16,140), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_ScaleX = new wxTextCtrl(this, ID_TEXTCTRL2, _T("1.000"), wxPoint(144,72), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	SpinButton_X = new wxSpinButton(this, ID_SPINBUTTON1, wxPoint(256,72), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON1"));
	SpinButton_X->SetRange(-100, 100);
	TextCtrl_ScaleY = new wxTextCtrl(this, ID_TEXTCTRL3, _T("1.000"), wxPoint(144,104), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	SpinButton_Y = new wxSpinButton(this, ID_SPINBUTTON2, wxPoint(256,104), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON2"));
	SpinButton_Y->SetRange(-100, 100);
	TextCtrl_ScaleZ = new wxTextCtrl(this, ID_TEXTCTRL4, _T("1.000"), wxPoint(144,136), wxSize(112,24), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	SpinButton_Z = new wxSpinButton(this, ID_SPINBUTTON3, wxPoint(256,136), wxSize(17,24), wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SPINBUTTON3"));
	SpinButton_Z->SetRange(-100, 100);
	CheckBox_ScaleUnique = new wxCheckBox(this, ID_CHECKBOX1, _T("Mettre la même échelle en X, Y et Z"), wxPoint(16,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	CheckBox_ScaleUnique->SetValue(true);
	Button_Appliquer = new wxButton(this, ID_BUTTON2, _T("Appliquer"), wxPoint(32,208), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Annuler = new wxButton(this, ID_BUTTON3, _T("Annuler"), wxPoint(152,208), wxSize(96,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));

	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&ScalePanel::OnTextCtrl_IncrementText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScalePanel::OnButton_ValiderClick);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ScalePanel::OnTextCtrl_ScaleXText);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&ScalePanel::OnSpinButton_XChangeUp);
	Connect(ID_SPINBUTTON1,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&ScalePanel::OnSpinButton_XChangeDown);
	Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ScalePanel::OnTextCtrl_ScaleYText);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&ScalePanel::OnSpinButton_YChangeUp);
	Connect(ID_SPINBUTTON2,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&ScalePanel::OnSpinButton_YChangeDown);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ScalePanel::OnTextCtrl_ScaleZText);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEUP,(wxObjectEventFunction)&ScalePanel::OnSpinButton_ZChangeUp);
	Connect(ID_SPINBUTTON3,wxEVT_SCROLL_LINEDOWN,(wxObjectEventFunction)&ScalePanel::OnSpinButton_ZChangeDown);
	Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&ScalePanel::OnCheckBox_ScaleUniqueClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScalePanel::OnButton_AppliquerClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScalePanel::OnButton_AnnulerClick);
	Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&ScalePanel::OnInit);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&ScalePanel::OnClose);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

ScalePanel::~ScalePanel()
{
	//(*Destroy(ScalePanel)
	//*)
}


void ScalePanel::OnClose(wxCloseEvent& event)
{
    BddInter *Element = MAIN->Element;

    Element->Changer_Echelle = false;
    ScaleX = ScaleY = ScaleZ = 1.0;
    wxTexte.Printf(format_Sc,ScaleX);
    CheckBox_ScaleUnique->SetValue(true);
    TextCtrl_ScaleX->SetValue(wxTexte);
    TextCtrl_ScaleY->SetValue(wxTexte);
    TextCtrl_ScaleZ->SetValue(wxTexte);
    Element->Scale_X = ScaleX;
    Element->Scale_Y = ScaleY;
    Element->Scale_Z = ScaleZ;
    Hide();
}

void ScalePanel::OnInit(wxInitDialogEvent& event)
{
//    printf("ScalePanel::OnInit\n");
    BddInter *Element = MAIN->Element;

    Incr = wxAtof(TextCtrl_Increment->GetValue());

    ScaleX = wxAtof(TextCtrl_ScaleX->GetValue());
    ScaleY = wxAtof(TextCtrl_ScaleY->GetValue());
    ScaleZ = wxAtof(TextCtrl_ScaleZ->GetValue());
    Element->Scale_X = ScaleX;
    Element->Scale_Y = ScaleY;
    Element->Scale_Z = ScaleZ;
    wxTexte.Printf(format_Sc,ScaleX);
    TextCtrl_ScaleX->SetValue(wxTexte);
    wxTexte.Printf(format_Sc,ScaleY);
    TextCtrl_ScaleY->SetValue(wxTexte);
    wxTexte.Printf(format_Sc,ScaleZ);
    TextCtrl_ScaleZ->SetValue(wxTexte);

    wxCommandEvent cmd_event;
    OnCheckBox_ScaleUniqueClick(cmd_event);
    Init_Centre_Scale();    // Initialisation du centre de mise à l'échelle (Barycentre de la sélection)
}

void ScalePanel::OnTextCtrl_IncrementText(wxCommandEvent& event)
{
    Incr = wxAtof(TextCtrl_Increment->GetValue());
}

void ScalePanel::OnButton_ValiderClick(wxCommandEvent& event)
{
    Incr = wxAtof(TextCtrl_Increment->GetValue());
}
/*
void ScalePanel::OnRadioBox_CentreSelect(wxCommandEvent& event)
{
}
*/

void ScalePanel::Init_Centre_Scale()
{
    unsigned int n_val,o,i,j, Nb_p;
    int indice;
    double cx1,cy1,cz1;
    Object *objet_courant;
    Sommet *sommet_courant;
    std::vector<int> NumerosSommets;

    BddInter *Element = MAIN->Element;

    // Barycentre du/des Objets/Facettes/Sommets
    cx1 = cy1 = cz1 = 0.0;
    Nb_p= 0;

    if (Element->mode_selection == Element->selection_objet) {
        n_val   = Element->listeObjets.size();
        auto it = Element->listeObjets.begin();
        for (i=0; i<n_val; i++, it++) {
            objet_courant = &(Element->Objetlist[*it]);
            if (objet_courant->deleted) continue ;                  // Ne pas traiter les objets supprimés
            for (j=0; j<objet_courant->Sommetlist.size(); j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                cx1 +=  sommet_courant->point[0];
                cy1 +=  sommet_courant->point[1];
                cz1 +=  sommet_courant->point[2];
                Nb_p++;
            }
        }

    } if (Element->mode_selection == Element->selection_facette) {

        for (o=0; o<Element->Objetlist.size(); o++) {
            objet_courant = &(Element->Objetlist[o]);
            if (objet_courant->deleted) continue ;                  // Ne pas traiter les objets supprimés
            Element->listePoints.clear();
            for (i=0; i<objet_courant->Nb_facettes; i++) {
                if (objet_courant->Facelist[i].selected) {          // Ne prendre en compte que les facettes sélectionnées
                    // Récupérer la liste des sommets de la facette. Ranger dans listePoints sans compter les sommets communs à plusieurs facettes
                    NumerosSommets = objet_courant->Facelist[i].getF_sommets();
                    for (j=0; j<NumerosSommets.size(); j++) {
                        indice = NumerosSommets[j] -1;
                        auto jt = std::find(Element->listePoints.begin(),Element->listePoints.end(),indice);    // Ce sommet est-il déjà dans la liste ?
                        if (jt == Element->listePoints.end() || Element->listePoints.empty()) {                     // Non
                            Element->listePoints.push_front(indice);                                                // L'ajouter à la liste des points
                            sommet_courant = &(objet_courant->Sommetlist[indice]);
                            cx1 += sommet_courant->point[0];        // Sommer les coordonnées des points sélectionnés
                            cy1 += sommet_courant->point[1];
                            cz1 += sommet_courant->point[2];
                            Nb_p++;
                        }
                    }
                }
            }
        }

    } else { // <=> Element->selection_point

        for (i=0; i<Element->Objetlist.size(); i++) {
            objet_courant = &(Element->Objetlist[i]);
            if (objet_courant->deleted) continue ;                  // Ne pas traiter les objets supprimés
            for (j=0; j<objet_courant->Sommetlist.size(); j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                if (sommet_courant->selected) {                     // Ne traiter que les sommets sélectionnés
                    cx1 += sommet_courant->point[0];
                    cy1 += sommet_courant->point[1];
                    cz1 += sommet_courant->point[2];
                    Nb_p++;
                }
            }
        }
    }
    if (Nb_p != 0) {
        Element->Centre_X = cx1/Nb_p; // ATTENTION : si Nb_p = 0, Centre_X _Y et _Z ne sont pas initialisés !!!!
        Element->Centre_Y = cy1/Nb_p;
        Element->Centre_Z = cz1/Nb_p;
    } else {
        Element->Centre_X = Element->Centre_Y = Element->Centre_Z = 0.0 ; // à vérifier si c'est un bon choix (plutôt barycentre global ? ou centre boîte englobante)
    }
}

void ScalePanel::OnTextCtrl_ScaleXText(wxCommandEvent& event)
{
    ScaleX = wxAtof(TextCtrl_ScaleX->GetValue());
}

void ScalePanel::OnTextCtrl_ScaleYText(wxCommandEvent& event)
{
    ScaleY = wxAtof(TextCtrl_ScaleY->GetValue());
}

void ScalePanel::OnTextCtrl_ScaleZText(wxCommandEvent& event)
{
    ScaleZ = wxAtof(TextCtrl_ScaleZ->GetValue());
}

void ScalePanel::OnCheckBox_ScaleUniqueClick(wxCommandEvent& event)
{
    Synchrones = CheckBox_ScaleUnique->IsChecked();
    if (Synchrones) {
        TextCtrl_ScaleY->Disable();
        TextCtrl_ScaleZ->Disable();
        SpinButton_Y   ->Disable();
        SpinButton_Z   ->Disable();
    } else {
        TextCtrl_ScaleY->Enable();
        TextCtrl_ScaleZ->Enable();
        SpinButton_Y   ->Enable();
        SpinButton_Z   ->Enable();
    }
}

void ScalePanel::OnSpinButton_XChangeUp(wxSpinEvent& event)
{
    ScaleX += Incr;
    OnSpinButton_XChange(event);
}

void ScalePanel::OnSpinButton_XChangeDown(wxSpinEvent& event)
{
    ScaleX -= Incr;
    OnSpinButton_XChange(event);
}

void ScalePanel::OnSpinButton_XChange(wxSpinEvent& event)
{
    SpinButton_X->SetValue(0);              // Raz : en principe évite les butées par valeur min et max du SpinButton
    wxTexte.Printf(format_Sc,ScaleX);
    TextCtrl_ScaleX->SetValue(wxTexte);     // Afficher la valeur calculée par incrémentation/décrémentation
    if (Synchrones) {
        TextCtrl_ScaleY->SetValue(wxTexte);
        TextCtrl_ScaleZ->SetValue(wxTexte);
    }
    Appliquer_Scale_Visuelle();
}

void ScalePanel::OnSpinButton_YChangeUp(wxSpinEvent& event)
{
    ScaleY += Incr;
    OnSpinButton_YChange(event);
}

void ScalePanel::OnSpinButton_YChangeDown(wxSpinEvent& event)
{
    ScaleY -= Incr;
    OnSpinButton_YChange(event);
}

void ScalePanel::OnSpinButton_YChange(wxSpinEvent& event)
{
    SpinButton_Y->SetValue(0);              // Raz
    wxTexte.Printf(format_Sc,ScaleY);
    TextCtrl_ScaleY->SetValue(wxTexte);     // Afficher la valeur calculée par incrémentation/décrémentation
    Appliquer_Scale_Visuelle();
}

void ScalePanel::OnSpinButton_ZChangeUp(wxSpinEvent& event)
{
    ScaleZ += Incr;
    OnSpinButton_ZChange(event);
}

void ScalePanel::OnSpinButton_ZChangeDown(wxSpinEvent& event)
{
    ScaleZ -= Incr;
    OnSpinButton_ZChange(event);
}

void ScalePanel::OnSpinButton_ZChange(wxSpinEvent& event)
{
    SpinButton_Z->SetValue(0);              // Raz
    wxTexte.Printf(format_Sc,ScaleZ);
    TextCtrl_ScaleZ->SetValue(wxTexte);     // Afficher la valeur calculée par incrémentation/décrémentation
    Appliquer_Scale_Visuelle();
}

void ScalePanel::Appliquer_Scale_Visuelle()
{
// Ici, c'est une application visuelle, sans modification de Bdd
    BddInter *Element = MAIN->Element;

    if (Synchrones) ScaleY = ScaleZ = ScaleX;
    Element->Scale_X = ScaleX;
    Element->Scale_Y = ScaleY;
    Element->Scale_Z = ScaleZ;

//    printf("Scale en X, Y et Z : %3.1f %3.1f %3.1f\n",ScaleX, ScaleY, ScaleZ);
    Element->Changer_Echelle = true;
    Element->m_gllist = 0;
    Element->Refresh();
}

void ScalePanel::OnButton_AppliquerClick(wxCommandEvent& event)
{
    BddInter *Element = MAIN->Element;

    unsigned int n_val, o, i, j, nb_fac;
    int indice;
    Object *objet_courant;
    Sommet *sommet_courant;
    Points *p_Point;
    std::vector<int> NumerosSommets;

    if (Element->mode_selection == Element->selection_objet) {
        n_val   = Element->listeObjets.size();
        auto it = Element->listeObjets.begin();

        for (i=0; i<n_val; i++, it++) {
            int o = *it;
//            printf("Objet : %d",o);
            objet_courant = &(Element->Objetlist[o]);
            unsigned int ns = objet_courant->Sommetlist.size();
//            printf(", Nb Sommets : %d\n",objet_courant->Sommetlist.size());
            // Recentrer l'objet en 0,0,0
            for (j=0; j<ns; j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                sommet_courant->point[0] -= Element->Centre_X;
                sommet_courant->point[1] -= Element->Centre_Y;
                sommet_courant->point[2] -= Element->Centre_Z;

            // Effectuer les 3 mises à l'échelle sur les 3 axes.
                sommet_courant->point[0] *= Element->Scale_X;
                sommet_courant->point[1] *= Element->Scale_Y;
                sommet_courant->point[2] *= Element->Scale_Z;

            // Remettre l'objet en place (translation inverse)
                sommet_courant->point[0] += Element->Centre_X;
                sommet_courant->point[1] += Element->Centre_Y;
                sommet_courant->point[2] += Element->Centre_Z;
            }

            if (!Synchrones) {  // Si les 3 Scale_* sont identiques, les normales ne changent pas
            // Calcul des normales aux barycentre des facettes
                nb_fac = objet_courant->Nb_facettes;
                for (j=0; j<nb_fac; j++) {
                    Element->Calcul_Normale_Barycentre(o,j);
    //              objet_courant->Facelist[j].flat = false;      // Ne pas changer a priori !
                }
            // Calcul des normales aux sommets
//                if (objet_courant->Nb_vecteurs != 0)              // Faut-il le conditionner à la préexistance des vecteurs ?
                    Element->GenereNormalesAuxSommets(o,ns);
            }
        }
    } else if(Element->mode_selection == Element->selection_facette) {

        for (o=0; o<Element->Objetlist.size(); o++) {               // Traiter les objets indépendamment
            Element->listePoints.clear();
            objet_courant = &(Element->Objetlist[o]);
            for (i=0; i<objet_courant->Nb_facettes; i++) {
                if (objet_courant->Facelist[i].selected) {          // Ne prendre en compte que les facettes sélectionnées
                    // Récupérer la liste des sommets de la facette. Ranger dans listePoints sans compter les sommets communs à plusieurs facettes
                    NumerosSommets = objet_courant->Facelist[i].getF_sommets();
                    for (j=0; j<NumerosSommets.size(); j++) {
                        indice = NumerosSommets[j] -1;
                        auto jt = std::find(Element->listePoints.begin(),Element->listePoints.end(),indice);    // Ce sommet est-il déjà dans la liste ?
                        if (jt == Element->listePoints.end() || Element->listePoints.empty()) {                     // Non
                            Element->listePoints.push_front(indice);                                                // L'ajouter à la liste des points
                        }
                    }
                }
            }
            // Établir en même temps la liste des facettes qui utilisent ces sommets
            n_val = Element->listePoints.size();
            if (n_val == 0) continue;   // Passer à l'objet suivant si la liste des sommets de l'objet courant est vide
            auto jt = Element->listePoints.begin();
            Element->listeFacettes.clear();
            for (j=0; j<n_val; j++, jt++) {
                sommet_courant = &(objet_courant->Sommetlist[*jt]);
                sommet_courant->point[0] -= Element->Centre_X;          // Recentrer les points de l'objet en 0,0,0
                sommet_courant->point[1] -= Element->Centre_Y;
                sommet_courant->point[2] -= Element->Centre_Z;

                sommet_courant->point[0] *= Element->Scale_X;           // Effectuer les 3 mises à l'échelle sur les 3 axes.
                sommet_courant->point[1] *= Element->Scale_Y;
                sommet_courant->point[2] *= Element->Scale_Z;

                sommet_courant->point[0] += Element->Centre_X;          // Remettre les points de l'objet en place (translation inverse)
                sommet_courant->point[1] += Element->Centre_Y;
                sommet_courant->point[2] += Element->Centre_Z;

                p_Point = &(objet_courant->Pointslist[*jt]);
                unsigned int n_fac = p_Point->IndicesFacettes.size();
                for (i=0; i<n_fac; i++) {
                    indice  = p_Point->IndicesFacettes[i];
                    auto ft = std::find(Element->listeFacettes.begin(),Element->listeFacettes.end(),indice);// La facette est-elle déjà dans la liste ?
                    if (ft == Element->listeFacettes.end() || Element->listeFacettes.empty()) {                 // Non
                        Element->listeFacettes.push_front(indice);                                              // L'ajouter à la liste des facettes
                    }
                }
            }
            // Calcul des normales aux barycentre des facettes modifiées
            nb_fac = Element->listeFacettes.size();
            auto ft= Element->listeFacettes.begin();
            for (j=0; j<nb_fac; j++,ft++) {
                Element->Calcul_Normale_Barycentre(o,*ft);
    //          objet_courant->Facelist[j].flat = false;      // Ne pas changer a priori !
            }

            if (objet_courant->Nb_vecteurs != 0) {
                // Réexplorer la liste des facettes et ajouter tous leurs sommets dans la liste des sommets
                ft = Element->listeFacettes.begin();
                for (i=0; i<nb_fac;i++,ft++) {
                    NumerosSommets = objet_courant->Facelist[*ft].getF_sommets();
                    for (j=0; j<NumerosSommets.size(); j++) {
                        indice = NumerosSommets[j] -1;
//                        objet_courant->Sommetlist[indice].selected = true;
                        auto jt = std::find(Element->listePoints.begin(),Element->listePoints.end(),indice);    // Ce sommet est-il déjà dans la liste ?
                        if (jt == Element->listePoints.end() || Element->listePoints.empty()) {                     // Non
                            Element->listePoints.push_front(indice);                                                // L'ajouter à la liste des points
                        }
                    }
                }
                jt    = Element->listePoints.begin();
                n_val = Element->listePoints.size();
                for (j=0; j<n_val; j++, jt++) Element->GenereNormale_1_Sommet(objet_courant,*jt,*jt) ;
            }

        }

//        ToDo();
//        OnButton_AnnulerClick(event); // mode de sélection facette : ne rien faire pour le moment !

    } else {
        // Ici on est en mode de sélection point
        for (o=0; o<Element->Objetlist.size(); o++) {               // Traiter les objets indépendamment
            Element->listeFacettes.clear();
            objet_courant = &(Element->Objetlist[o]);

            for (j=0; j<objet_courant->Nb_sommets; j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                if (sommet_courant->selected) {                         // Ne traiter que les sommets sélectionnés
                    sommet_courant->point[0] -= Element->Centre_X;          // Recentrer les points de l'objet en 0,0,0
                    sommet_courant->point[1] -= Element->Centre_Y;
                    sommet_courant->point[2] -= Element->Centre_Z;

                    sommet_courant->point[0] *= Element->Scale_X;           // Effectuer les 3 mises à l'échelle sur les 3 axes.
                    sommet_courant->point[1] *= Element->Scale_Y;
                    sommet_courant->point[2] *= Element->Scale_Z;

                    sommet_courant->point[0] += Element->Centre_X;          // Remettre les points de l'objet en place (translation inverse)
                    sommet_courant->point[1] += Element->Centre_Y;
                    sommet_courant->point[2] += Element->Centre_Z;

                    p_Point = &(objet_courant->Pointslist[j]);
                    unsigned int n_fac = p_Point->IndicesFacettes.size();
                    for (i=0; i<n_fac; i++) {
                        indice  = p_Point->IndicesFacettes[i];
                        auto ft = std::find(Element->listeFacettes.begin(),Element->listeFacettes.end(),indice);// La facette est-elle déjà dans la liste ?
                        if (ft == Element->listeFacettes.end() || Element->listeFacettes.empty()) {                 // Non
                            Element->listeFacettes.push_front(indice);                                              // L'ajouter à la liste des facettes
                        }
                    }
                }
            }
            // Calcul des normales au barycentre des facettes modifiées
            nb_fac = Element->listeFacettes.size();
            auto ft= Element->listeFacettes.begin();
            for (j=0; j<nb_fac; j++,ft++) {
                Element->Calcul_Normale_Barycentre(o,*ft);
    //          objet_courant->Facelist[j].flat = false;      // Ne pas changer a priori !
            }
        }

//        ToDo();
//        OnButton_AnnulerClick(event);
    }

    Element->bdd_modifiee    = true;
    Element->Changer_Echelle = false;
    Element->m_gllist = 0;
    Element->Refresh();
    wxCloseEvent close_event;
    OnClose(close_event);
}

void ScalePanel::OnButton_AnnulerClick(wxCommandEvent& event)
{
//    ToDo();
    ScaleX = ScaleY = ScaleZ = 1.0;
    wxTexte.Printf(format_Sc,ScaleX);
    CheckBox_ScaleUnique->SetValue(true);
    TextCtrl_ScaleX->SetValue(wxTexte);
    TextCtrl_ScaleY->SetValue(wxTexte);
    TextCtrl_ScaleZ->SetValue(wxTexte);
    Appliquer_Scale_Visuelle();
    wxCloseEvent close_event;
    OnClose(close_event);
}

//void ScalePanel::ToDo()
//{
//    MAIN->Element->DisplayMessage(_T("Pas encore complètement opérationnel\nSimulation interface OK"), true);
//}


