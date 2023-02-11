#include "SelectionManuelleFacettes.h"

//(*InternalHeaders(SelectionManuelleFacettes)
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(SelectionManuelleFacettes)
const long SelectionManuelleFacettes::ID_STATICTEXT1 = wxNewId();
const long SelectionManuelleFacettes::ID_SPINCTRL1 = wxNewId();
const long SelectionManuelleFacettes::ID_STATICTEXT2 = wxNewId();
const long SelectionManuelleFacettes::ID_STATICTEXT3 = wxNewId();
const long SelectionManuelleFacettes::ID_STATICTEXT4 = wxNewId();
const long SelectionManuelleFacettes::ID_TEXTCTRL1 = wxNewId();
const long SelectionManuelleFacettes::ID_BUTTON1 = wxNewId();
const long SelectionManuelleFacettes::ID_BUTTON3 = wxNewId();
const long SelectionManuelleFacettes::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(SelectionManuelleFacettes,wxDialog)
	//(*EventTable(SelectionManuelleFacettes)
	//*)
END_EVENT_TABLE()

SelectionManuelleFacettes::SelectionManuelleFacettes(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SelectionManuelleFacettes)
	Create(parent, id, _T("Sélection manuelle des facettes"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(328,152));
	Move(wxDefaultPosition);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _T("Indice de l\'objet :"), wxPoint(8,12), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	SpinCtrl_NumObjet = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxPoint(192,8), wxSize(129,23), 0, 0, 100, 0, _T("ID_SPINCTRL1"));
	SpinCtrl_NumObjet->SetValue(_T("0"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _T("Nom :"), wxPoint(8,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText_NomObjet = new wxStaticText(this, ID_STATICTEXT3, _T("Nom Objet"), wxPoint(48,40), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText_Numeros = new wxStaticText(this, ID_STATICTEXT4, _T("Indices des facettes (séparés par espace, virgule ou - ) :"), wxPoint(8,64), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	TextCtrl_Numeros = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(8,88), wxSize(312,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	Button_Valider = new wxButton(this, ID_BUTTON1, _T("Valider"), wxPoint(16,120), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	Button_Valider->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Reset = new wxButton(this, ID_BUTTON3, _T("Reset"), wxPoint(120,120), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	Button_Reset->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	Button_Quitter = new wxButton(this, ID_BUTTON2, _T("Quitter"), wxPoint(224,120), wxSize(88,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	Button_Quitter->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));

	Connect(ID_SPINCTRL1,wxEVT_COMMAND_SPINCTRL_UPDATED,(wxObjectEventFunction)&SelectionManuelleFacettes::OnSpinCtrl_NumObjetChange);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleFacettes::OnButton_ValiderClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleFacettes::OnButton_ResetClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SelectionManuelleFacettes::OnButton_QuitterClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&SelectionManuelleFacettes::OnClose);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
}

SelectionManuelleFacettes::~SelectionManuelleFacettes()
{
	//(*Destroy(SelectionManuelleFacettes)
	//*)
}


void SelectionManuelleFacettes::OnButton_QuitterClick(wxCommandEvent& event)
{
// Boutton Quitter <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void SelectionManuelleFacettes::OnClose(wxCloseEvent& event)
{
    Hide();
}

void SelectionManuelleFacettes::OnButton_ValiderClick(wxCommandEvent& event)
{
//      Décomposition d'une chaine comme "1,4,5-10,23 45-48" en une liste "1 4 5 6 7 8 9 10 23 45 46 47 48"
//      Séparateurs "," ou " ". le "-" séparant 2 nombres a et b (b > a) signifie tous les entiers de a à b
//      La syntaxe 5  - 10 est équivalente à 5-10.

// Adapté de la version Tcl d'Ovni

    BddInter * Element=MAIN->Element;
    Face * Facette_courante;

	int indice = -1 ;
    int indice_facette, indice_facette_deb, objet, face ;
    char *ptr, c ;
    bool moins ;

    objet = SpinCtrl_NumObjet->GetValue();
    wxString wxChaine   = TextCtrl_Numeros->GetValue();
    wxCharBuffer buffer = wxChaine.mb_str();
    ptr = buffer.data();
//    printf("%s\n",ptr);
    indice_facette=face= 0 ;
    indice_facette_deb = 0 ;
    moins              = false ;
    while ((c=*ptr) != '\0') { // Récupère dans le caractère c les caractères successifs de la chaîne entrée
        indice = (int)c - (int)'0' ;
        if (indice >=0 && indice <= 9) indice_facette = 10*indice_facette + indice ; // Recompose les nombres entiers
        ptr++;
        if (*ptr == ',' || *ptr == ' ' || *ptr == '\0' || *ptr == '-') {
            if (moins) { // Génère les numéros intermédiaires de la syntaxe n1-n2
                for (indice=indice_facette_deb ; indice < indice_facette ; indice++) {
                    if (indice < (int)Element->Objetlist[objet].Nb_facettes-1) {
                        // N'ajouter que si non déjà présent dans la Liste
                        if (!Element->ToSelect.check_if_in_ListeSelect(objet,indice)) {
                            if (Element->verbose) printf("objet %d, facette %d\n",objet,indice);
                            Element->ToSelect.add_one(objet,indice);
//                            Element->colorface(objet,indice);
                            Element->Objetlist[objet].Facelist[indice].selected = true;
                        }
                    }
                }
                moins = false ;
            }
            // Sélectionner la facatte "indice_facette" de l'Objet "objet"
            if (indice_facette < (int)Element->Objetlist[objet].Nb_facettes-1) {
                // N'ajouter que si non déjà présent dans la Liste
                if (!Element->ToSelect.check_if_in_ListeSelect(objet,indice_facette)) {
                    if (Element->verbose) printf("objet %d, facette %d\n",objet,indice_facette);
                    Element->ToSelect.add_one(objet,indice_facette);
//                    Element->colorface(objet,indice_facette);
                    Element->Objetlist[objet].Facelist[indice_facette].selected = true;

                }
            }
            face = indice_facette;          // Sauvegarde dans face
            indice_facette_deb = face+1 ;   // déjà pris en compte, _deb an cas de - 1 cran plus loin
            if (*ptr == '-') moins = true ; // Singe - détectté
            indice_facette = 0;             // Raz du numéro d'indice facette pour reconstruire le suivant
        }
    } // Fin d'exploitation de la chaîne de caractères

    int len_liste = Element->ToSelect.ListeSelect.size();
    wxString str_loc;
    str_loc = wxString(Element->Objetlist[objet].GetName(), wxConvUTF8);
    StaticText_NomObjet->SetLabel(str_loc);
    MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(str_loc);    // Mise à jour du Nom de l'objet
    str_loc.Printf(_T("%d"),objet);
    MAIN->Selections_Panel->TextCtrl_NumObjet->SetValue(str_loc);       // Mise à jour du Numéro de l'objet

    if (len_liste == 0)
        str_loc.Printf(_T(""));
    else
        str_loc.Printf(_T("%d"),len_liste);
    MAIN->Selections_Panel->TextCtrl_Selection->SetValue(str_loc);      // Mise à jour du nombre de facettes sélectionnées

    str_loc.Printf(_T(""));
    MAIN->Selections_Panel->TextCtrl_NumGroupe  ->SetValue(str_loc);
    MAIN->Selections_Panel->TextCtrl_NumMateriau->SetValue(str_loc);
    if (len_liste == 1)
        str_loc.Printf(_T("%d"),face);
    else
        str_loc.Printf(_T("Sélection multiple"));
    MAIN->Selections_Panel->TextCtrl_NumFacette->SetValue(str_loc);     // Mise à jour du numéro de facette sélectionnée

    // Mise à jour des numéros de Groupe et de Matériau
    if(len_liste > 0) {
        bool groupes_egaux   = true;
        bool materiaux_egaux = true;

        objet = Element->ToSelect.ListeSelect[0].objet;
        face  = Element->ToSelect.ListeSelect[0].face_sommet;
        Facette_courante = &(Element->Objetlist[objet].Facelist[face]);
        int first_grp = Facette_courante->groupe;
        int first_mat = Facette_courante->codmatface;

        for (int i=1; i< len_liste; i++) {                  // boucle non utilisée si len_liste == 1
            objet = Element->ToSelect.ListeSelect[i].objet;
            face  = Element->ToSelect.ListeSelect[i].face_sommet;
            Facette_courante = &(Element->Objetlist[objet].Facelist[face]);
            if (Element->verbose) printf("%d %d %d %d %d\n", i, objet, face,
                                                             Facette_courante->groupe,
                                                             Facette_courante->codmatface);
            if (Facette_courante->groupe     != first_grp) groupes_egaux   = false;
            if (Facette_courante->codmatface != first_mat) materiaux_egaux = false;
        }

        if (groupes_egaux) {
            str_loc.Printf(_T("%d"),first_grp);
            MAIN->Selections_Panel->TextCtrl_NumGroupe  ->SetValue(str_loc);
        }
        if (materiaux_egaux) {
            str_loc.Printf(_T("%d"),first_mat);
            MAIN->Selections_Panel->TextCtrl_NumMateriau->SetValue(str_loc);
        }
    }

    Element->m_gllist = Element->glliste_select; //0;
    Element->Refresh();
}

void SelectionManuelleFacettes::OnSpinCtrl_NumObjetChange(wxSpinEvent& event)
{
    BddInter * Element=MAIN->Element;
    wxString str_loc;

    int NumeroObjet = SpinCtrl_NumObjet->GetValue();
    str_loc = wxString(Element->Objetlist[NumeroObjet].GetName(), wxConvUTF8);
    StaticText_NomObjet->SetLabel(str_loc);
//    MAIN->Selections_Panel->TextCtrl_NomObjet->ChangeValue(str_loc);  // A ne faire que si clic sur "Valider"
}

void SelectionManuelleFacettes::OnButton_ResetClick(wxCommandEvent& event)
{
    BddInter * Element=MAIN->Element;

    wxKeyEvent key_event;
    key_event.m_keyCode = 'S';
    Element->OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection de points
}
