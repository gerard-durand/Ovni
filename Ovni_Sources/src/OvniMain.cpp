/***************************************************************
 * Name:      OvniMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Gerard Durand (Gerard.Durand@onera.fr)
 * Created:   2011-02-10
 * Copyright: Gerard Durand ()
 * License:
 **************************************************************/

//#include <GL/glew.h>    // Pour test

#include "OvniMain.h"
#include "OvniApp.h"
#include <wx/settings.h>
#include <wx/app.h>
#include <wx/dcclient.h>
#include <wx/debug.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include "wx/wxprec.h"
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include <iomanip>
#include <fstream>

//(*InternalHeaders(OvniFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/tglbtn.h>

#include "version.h"

//using namespace std;  // N'est pas utile ici vu qu'on utilise explicitement std::

//helper functions : diverses infos affichées dans le menu Aide / A propos ...
enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format) {
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f ) {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }
#if _WIN32
#if _WIN64
    wxbuild << _T("\nVersion compilée en 64 bits");
#else
    wxbuild << _T("\nVersion compilée en 32 bits");
#endif // _WIN64
#endif // _WIN32

#ifndef WXUSINGDLL
    wxbuild << ", statique" ;
#endif // WXUSINGDLL

#ifdef DEBUG
    wxbuild << _T(" et en mode debug");
#endif // DEBUG

// Ajout des informations stockées dans version.h

    wxbuild << _T("\nBuild : ") << AutoVersion::FULLVERSION_STRING ;
    static const char RELEASE_SHORT[] = "r";
    if (AutoVersion::STATUS_SHORT[0] != RELEASE_SHORT[0]) {
            wxbuild << AutoVersion::STATUS_SHORT;
    }
    wxbuild << _T(", code source du ") << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR ;

// Récupération de la date et de l'heure de la compilation

    wxStructStat structStat;
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxStat(f.GetFullName(), &structStat);
    wxDateTime last_modified_time(structStat.st_mtime);
    wxString DateCompilation = last_modified_time.Format(wxT("%d/%m/%Y à %H:%M")); // pas besoin des secondes ! :%S
    wxbuild << _T("\nDate de compilation : ") << DateCompilation;

// Numéro de version du compilateur
    wxbuild << _T("\nVersion de gcc : ") << _T(__VERSION__);

    return wxbuild;
}

// Constantes d'identification pour les menus avec états activé/non activé (non gérés par wxSmith)
const long OvniFrame::ID_BUTTON7  = wxNewId();
const long OvniFrame::ID_BUTTON8  = wxNewId();
const long OvniFrame::ID_BUTTON9  = wxNewId();
const long OvniFrame::ID_BUTTON10 = wxNewId();
const long OvniFrame::ID_BUTTON11 = wxNewId();
const long OvniFrame::ID_BUTTON12 = wxNewId();
const long OvniFrame::ID_BUTTON13 = wxNewId();
const long OvniFrame::ID_BUTTON14 = wxNewId();
const long OvniFrame::ID_BUTTON15 = wxNewId();
const long OvniFrame::ID_BUTTON16 = wxNewId();
const long OvniFrame::ID_BUTTON17 = wxNewId();
const long OvniFrame::ID_BUTTON18 = wxNewId();
const long OvniFrame::ID_BUTTON19 = wxNewId();
const long OvniFrame::ID_BUTTON20 = wxNewId();

// Constantes d'identifications gérées par wxSmith
//(*IdInit(OvniFrame)
const long OvniFrame::ID_STATICTEXT1 = wxNewId();
const long OvniFrame::ID_BUTTON1 = wxNewId();
const long OvniFrame::ID_BUTTON2 = wxNewId();
const long OvniFrame::ID_STATICTEXT2 = wxNewId();
const long OvniFrame::ID_BUTTON3 = wxNewId();
const long OvniFrame::ID_BUTTON4 = wxNewId();
const long OvniFrame::ID_BUTTON5 = wxNewId();
const long OvniFrame::ID_BUTTON6 = wxNewId();
const long OvniFrame::ID_PANEL1 = wxNewId();
const long OvniFrame::ID_GLCANVAS = wxNewId();
const long OvniFrame::ID_SLIDER_X = wxNewId();
const long OvniFrame::ID_SLIDER_Y = wxNewId();
const long OvniFrame::ID_SLIDER_Z = wxNewId();
const long OvniFrame::ID_PANEL2 = wxNewId();
const long OvniFrame::idOpenFile = wxNewId();
const long OvniFrame::idReopenFile = wxNewId();
const long OvniFrame::idAddFile = wxNewId();
const long OvniFrame::idSaveFile = wxNewId();
const long OvniFrame::idSaveFileAs = wxNewId();
const long OvniFrame::idProperties = wxNewId();
const long OvniFrame::idPrefs = wxNewId();
const long OvniFrame::idHardware = wxNewId();
const long OvniFrame::idMenuQuit = wxNewId();
const long OvniFrame::menu_Affichage_Points = wxNewId();
const long OvniFrame::menu_Affichage_Filaire = wxNewId();
const long OvniFrame::menu_Affichage_Plein = wxNewId();
const long OvniFrame::menu_Affichage_Axes = wxNewId();
const long OvniFrame::menu_Affichage_Boite = wxNewId();
const long OvniFrame::menu_Affichage_Source = wxNewId();
const long OvniFrame::ID_MENUITEM1 = wxNewId();
const long OvniFrame::ID_MENUITEM2 = wxNewId();
const long OvniFrame::ID_MENUITEM3 = wxNewId();
const long OvniFrame::ID_MENUITEM4 = wxNewId();
const long OvniFrame::ID_MENUITEM5 = wxNewId();
const long OvniFrame::ID_MENUITEM6 = wxNewId();
const long OvniFrame::ID_MENUITEM7 = wxNewId();
const long OvniFrame::ID_MENUITEM8 = wxNewId();
const long OvniFrame::ID_MENUITEM32 = wxNewId();
const long OvniFrame::ID_MENUITEM33 = wxNewId();
const long OvniFrame::ID_MENUITEM34 = wxNewId();
const long OvniFrame::ID_MENUITEM35 = wxNewId();
const long OvniFrame::ID_MENUITEM36 = wxNewId();
const long OvniFrame::ID_MENUITEM37 = wxNewId();
const long OvniFrame::ID_MENUITEM38 = wxNewId();
const long OvniFrame::ID_MENUITEM51 = wxNewId();
const long OvniFrame::ID_MENUITEM39 = wxNewId();
const long OvniFrame::ID_MENUITEM9 = wxNewId();
const long OvniFrame::ID_MENUITEM10 = wxNewId();
const long OvniFrame::ID_MENUITEM11 = wxNewId();
const long OvniFrame::ID_MENUITEM12 = wxNewId();
const long OvniFrame::ID_MENUITEM13 = wxNewId();
const long OvniFrame::ID_MENUITEM14 = wxNewId();
const long OvniFrame::menu_reperage_couleurs_facettes = wxNewId();
const long OvniFrame::menu_reperage_couleurs_groupes = wxNewId();
const long OvniFrame::menu_reperage_couleurs_materiaux = wxNewId();
const long OvniFrame::ID_MENUITEM50 = wxNewId();
const long OvniFrame::ID_MENUITEM49 = wxNewId();
const long OvniFrame::ID_MENUITEM48 = wxNewId();
const long OvniFrame::ID_MENUITEM40 = wxNewId();
const long OvniFrame::ID_MENUITEM41 = wxNewId();
const long OvniFrame::ID_MENUITEM42 = wxNewId();
const long OvniFrame::ID_MENUITEM15 = wxNewId();
const long OvniFrame::ID_MENUITEM16 = wxNewId();
const long OvniFrame::ID_MENUITEM28 = wxNewId();
const long OvniFrame::ID_MENUITEM29 = wxNewId();
const long OvniFrame::ID_MENUITEM30 = wxNewId();
const long OvniFrame::ID_MENUITEM31 = wxNewId();
const long OvniFrame::ID_MENUITEM17 = wxNewId();
const long OvniFrame::ID_MENUITEM18 = wxNewId();
const long OvniFrame::ID_MENUITEM19 = wxNewId();
const long OvniFrame::ID_MENUITEM20 = wxNewId();
const long OvniFrame::ID_MENUITEM21 = wxNewId();
const long OvniFrame::ID_MENUITEM22 = wxNewId();
const long OvniFrame::ID_MENUITEM23 = wxNewId();
const long OvniFrame::ID_MENUITEM24 = wxNewId();
const long OvniFrame::ID_MENUITEM27 = wxNewId();
const long OvniFrame::ID_MENUITEM25 = wxNewId();
const long OvniFrame::ID_MENUITEM26 = wxNewId();
const long OvniFrame::ID_MENUITEM43 = wxNewId();
const long OvniFrame::ID_MENUITEM44 = wxNewId();
const long OvniFrame::ID_MENUITEM45 = wxNewId();
const long OvniFrame::ID_MENUITEM46 = wxNewId();
const long OvniFrame::ID_MENUITEM47 = wxNewId();
const long OvniFrame::idMenuHelp = wxNewId();
const long OvniFrame::idMenuAbout = wxNewId();
const long OvniFrame::ID_STATUSBAR1 = wxNewId();
const long OvniFrame::ID_TIMER1 = wxNewId();
//*)

// Constantes d'identifications pour le menu popup (clic droit à la souris)
const long OvniFrame::ID_POPUP_RAZ       = wxNewId();
const long OvniFrame::ID_POPUP_ETENDRE   = wxNewId();
const long OvniFrame::ID_POPUP_CENTRER   = wxNewId();
const long OvniFrame::ID_POPUP_MASQUER   = wxNewId();
const long OvniFrame::ID_POPUP_DEMASQUER = wxNewId();
const long OvniFrame::ID_POPUP_DELETE    = wxNewId();
const long OvniFrame::ID_POPUP_UNDELETE  = wxNewId();
const long OvniFrame::ID_POPUP_INVERSER_N= wxNewId();
const long OvniFrame::ID_POPUP_PARCOURS_I= wxNewId();
const long OvniFrame::ID_POPUP_RAZ_SELECT= wxNewId();
const long OvniFrame::ID_POPUP_NORM_F    = wxNewId();
const long OvniFrame::ID_POPUP_NORM_S    = wxNewId();
const long OvniFrame::ID_POPUP_FLAT      = wxNewId();
const long OvniFrame::ID_POPUP_NOT_FLAT  = wxNewId();

const long OvniFrame::idReopenFile3ds    = wxNewId();       // Fait manuellement car non créé via wxSmith

BEGIN_EVENT_TABLE(OvniFrame,wxFrame)
    //(*EventTable(OvniFrame)
    //*)
END_EVENT_TABLE()

OvniFrame::OvniFrame(wxWindow* parent,wxWindowID id) {
//    verbose = true;
    if (verbose) printf("Entree OvniFrame:OvniFrame\n");
    // Menus, boutons et autres contrôles gérés par wxSmith

    // Note mettre ci-dessous //_(* plutôt que //(* empèche la génération automatique de code par wxSmith. Utile pour pouvoir insérer du code dans une version quasi-définitive.
    // permet par exemple d'avoir 2 versions de GLCanvasAttributes_1[] conditionnées par un if (wxGLCanvas::IsDisplaySupported(test_GLCanvasAttributes_1)) testant la validité de
    // int test_GLCanvasAttributes_1[] = { WX_GL_SAMPLE_BUFFERS, 1, 0, 0 };     // Peut-être + général que #if wxCHECK_VERSION(3,0,0)
    // En cas de changement dans wxSmith sur la frame principale OvniFrame, il suffit de réactiver le //(*, faire les changements dans wxSmith, remettre en place le //_(* et
    // remettre en place le code qui a été supprimé par la génération automatique (voir l'exemple ci-dessous).

    int test_GLCanvasAttributes_1[] = { WX_GL_SAMPLE_BUFFERS, 1, WX_GL_SAMPLES, 4, 0, 0 };
    bool GLExtend = false;
    if (wxGLCanvas::IsDisplaySupported(test_GLCanvasAttributes_1)) GLExtend = true;
/*  Exemple de code à remplacer/ajouter :
    int GLCanvasAttributes_1[] = {
        WX_GL_RGBA,
        WX_GL_DOUBLEBUFFER,
        WX_GL_DEPTH_SIZE,      16,
        WX_GL_STENCIL_SIZE,    0,
    #if wxCHECK_VERSION(3,0,0)
        WX_GL_SAMPLE_BUFFERS,  1,
        WX_GL_SAMPLES,         4,
    #endif // wxCHECK_VERSION
        0, 0
        };
    if (!GLExtend) {
        printf("WX_GL_SAMPLE_BUFFERS non reconnu par la carte graphique et/ou son driver !\n");
        GLCanvasAttributes_1[6] = GLCanvasAttributes_1[7] = 0;
    }
*/
    //_(*Initialize(OvniFrame)
    Create(parent, wxID_ANY, _T("OVNI Version wxWidgets"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxFULL_REPAINT_ON_RESIZE, _T("wxID_ANY"));
    Hide();
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    Panel1->SetMinSize(wxSize(-1,25));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _T("Zoom :"), wxPoint(5,5), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    Button_ZoomPlus = new wxButton(Panel1, ID_BUTTON1, _T("+"), wxPoint(48,0), wxSize(45,24), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_ZoomMoins = new wxButton(Panel1, ID_BUTTON2, _T("-"), wxPoint(96,0), wxSize(45,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _T("Déplacements :"), wxPoint(152,5), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    Button_Gauche = new wxButton(Panel1, ID_BUTTON3, _T("Gauche"), wxPoint(240,0), wxSize(55,24), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Button_Droite = new wxButton(Panel1, ID_BUTTON4, _T("Droite"), wxPoint(294,0), wxSize(55,24), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Button_Haut = new wxButton(Panel1, ID_BUTTON5, _T("Haut"), wxPoint(348,0), wxSize(55,24), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Button_Bas = new wxButton(Panel1, ID_BUTTON6, _T("Bas"), wxPoint(402,0), wxSize(55,24), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer1->Add(Panel1, 0, wxALL|wxEXPAND, 0);
    int GLCanvasAttributes_1[] = {
        WX_GL_RGBA,
        WX_GL_DOUBLEBUFFER,
        WX_GL_DEPTH_SIZE,      16,
        WX_GL_STENCIL_SIZE,    0,
    #if wxCHECK_VERSION(3,0,0)
        WX_GL_SAMPLE_BUFFERS,  1,
        WX_GL_SAMPLES,         4,
    #endif // wxCHECK_VERSION
        0, 0
        };
    if (!GLExtend) {
        printf("WX_GL_SAMPLE_BUFFERS non reconnu par la carte graphique et/ou son driver !\n");
        GLCanvasAttributes_1[6] = GLCanvasAttributes_1[7] = 0;
    }
    #if wxCHECK_VERSION(3,0,0)
    	GLCanvas = new wxGLCanvas(this, ID_GLCANVAS, GLCanvasAttributes_1, wxDefaultPosition, wxSize(800,400), wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"));
    #else
    	GLCanvas = new wxGLCanvas(this, ID_GLCANVAS, wxDefaultPosition, wxSize(800,400), wxFULL_REPAINT_ON_RESIZE, _T("ID_GLCANVAS"), GLCanvasAttributes_1);
    #endif // wxCHECK_VERSION
    GLCanvas->SetBackgroundColour(wxColour(100,150,190));
    BoxSizer1->Add(GLCanvas, 1, wxALL|wxEXPAND, 0);
    Panel_Sliders = new wxPanel(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Slider_x = new wxSlider(Panel_Sliders, ID_SLIDER_X, 0, 0, 360, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_LABELS, wxDefaultValidator, _T("ID_SLIDER_X"));
    Slider_x->SetTickFreq(30);
    Slider_x->SetForegroundColour(wxColour(255,0,0));
    Slider_x->SetHelpText(_T("Axe Rouge"));
    BoxSizer2->Add(Slider_x, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    Slider_y = new wxSlider(Panel_Sliders, ID_SLIDER_Y, 0, 0, 360, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_LABELS, wxDefaultValidator, _T("ID_SLIDER_Y"));
    Slider_y->SetTickFreq(30);
    Slider_y->SetForegroundColour(wxColour(0,220,0));
    Slider_y->SetHelpText(_T("Axe Vert"));
    BoxSizer2->Add(Slider_y, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    Slider_z = new wxSlider(Panel_Sliders, ID_SLIDER_Z, 0, 0, 360, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_LABELS, wxDefaultValidator, _T("ID_SLIDER_Z"));
    Slider_z->SetTickFreq(30);
    Slider_z->SetForegroundColour(wxColour(0,0,255));
    Slider_z->SetHelpText(_T("Axe Bleu"));
    BoxSizer2->Add(Slider_z, 0, wxLEFT|wxRIGHT|wxEXPAND, 5);
    Panel_Sliders->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel_Sliders);
    BoxSizer2->SetSizeHints(Panel_Sliders);
    BoxSizer1->Add(Panel_Sliders, 0, wxALL|wxEXPAND, 0);
    SetSizer(BoxSizer1);
    MenuBar_Globale = new wxMenuBar();
    MenuFile = new wxMenu();
    Menu_Open = new wxMenuItem(MenuFile, idOpenFile, _T("Ouvrir"), _T("Ouvrir un fichier de bdd"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Open);
    Menu_ReOpen = new wxMenuItem(MenuFile, idReopenFile, _T("Réouvrir"), _T("Réouvre le fichier tel qu\'il est sur le disque"), wxITEM_NORMAL);
    MenuFile->Append(Menu_ReOpen);
    Menu_ReOpen->Enable(false);
    Menu_AddFile = new wxMenuItem(MenuFile, idAddFile, _T("Ajouter une Bdd"), _T("Ajoute un nouveau fichier Bdd à ceux déjà présents"), wxITEM_NORMAL);
    MenuFile->Append(Menu_AddFile);
    Menu_AddFile->Enable(false);
    Menu_Enregistrer = new wxMenuItem(MenuFile, idSaveFile, _T("Enregistrer"), _T("Enregistrement de la base de donnée"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Enregistrer);
    Menu_Enregistrer->Enable(false);
    Menu_Enregistrer_Sous = new wxMenuItem(MenuFile, idSaveFileAs, _T("Enregistrer sous..."), _T("Enregistrer la base de donnée sous un nouveau nom"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Enregistrer_Sous);
    Menu_Enregistrer_Sous->Enable(false);
    MenuFile->AppendSeparator();
    Menu_Proprietes = new wxMenuItem(MenuFile, idProperties, _T("Propriétés"), _T("Affiche les caractéristiques de la base de données"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Proprietes);
    MenuFile->AppendSeparator();
    Menu_Preferences = new wxMenuItem(MenuFile, idPrefs, _T("Préférences"), _T("Affiche une boîte de dialogue d\'options d\'utilisation"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Preferences);
    MenuFile->AppendSeparator();
    Menu_Hardware3D = new wxMenuItem(MenuFile, idHardware, _T("Hardware 3D"), _T("Affiche les caractéristiques de la carte 3D"), wxITEM_NORMAL);
    MenuFile->Append(Menu_Hardware3D);
    MenuFile->AppendSeparator();
    MenuItem_Quitter = new wxMenuItem(MenuFile, idMenuQuit, _T("Quitter\tAlt-F4"), _T("Quitte l\'application"), wxITEM_NORMAL);
    MenuFile->Append(MenuItem_Quitter);
    MenuBar_Globale->Append(MenuFile, _T("&Fichier"));
    Menu_Affichage = new wxMenu();
    Menu_Affichage_Points = new wxMenuItem(Menu_Affichage, menu_Affichage_Points, _T("Points"), _T("Affiche les points"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Points);
    Menu_Affichage_Filaire = new wxMenuItem(Menu_Affichage, menu_Affichage_Filaire, _T("Filaire"), _T("Affiche les arêtes"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Filaire);
    Menu_Affichage_Plein = new wxMenuItem(Menu_Affichage, menu_Affichage_Plein, _T("Plein"), _T("Affiche les surfaces"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Plein);
    Menu_Affichage_Plein->Check(true);
    Menu_Affichage->AppendSeparator();
    Menu_Affichage_Axes = new wxMenuItem(Menu_Affichage, menu_Affichage_Axes, _T("Axes"), _T("Affiche les axes X, Y et Z de la base de données"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Axes);
    Menu_Affichage_Axes->Check(true);
    Menu_Affichage_Boite = new wxMenuItem(Menu_Affichage, menu_Affichage_Boite, _T("Boîte englobante"), _T("Affiche la boîte englobante de la Bdd"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Boite);
    Menu_Affichage_Boite->Check(true);
    Menu_Affichage_Source = new wxMenuItem(Menu_Affichage, menu_Affichage_Source, _T("Source de lumière"), _T("Affiche la source de lumière (Soleil)"), wxITEM_CHECK);
    Menu_Affichage->Append(Menu_Affichage_Source);
    Menu_Affichage->AppendSeparator();
    Menu_Origine = new wxMenuItem(Menu_Affichage, ID_MENUITEM1, _T("Origine"), _T("Pour Bdd orientée selon la convention de Crira"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_Origine);
    Menu_VueDeFace = new wxMenuItem(Menu_Affichage, ID_MENUITEM2, _T("Vue de face"), _T("Pour Bdd orientée selon la convention de Crira"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_VueDeFace);
    Menu_VueDeProfil = new wxMenuItem(Menu_Affichage, ID_MENUITEM3, _T("Vue de profil"), _T("Pour Bdd orientée selon la convention de Crira"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_VueDeProfil);
    Menu_VueDeDessus = new wxMenuItem(Menu_Affichage, ID_MENUITEM4, _T("Vue de dessus"), _T("Pour Bdd orientée selon la convention de Crira"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_VueDeDessus);
    Menu_Affichage->AppendSeparator();
    Menu_PositionObservateur = new wxMenuItem(Menu_Affichage, ID_MENUITEM5, _T("Position Angulaire Observateur"), _T("Angles de Site et Azimut d\'observation"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_PositionObservateur);
    Menu_ZoomSpecifique = new wxMenuItem(Menu_Affichage, ID_MENUITEM6, _T("Zoom spécifique"), _T("Données de Zoom et d\'observation"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_ZoomSpecifique);
    Menu_CentreRotation = new wxMenuItem(Menu_Affichage, ID_MENUITEM7, _T("Centre de rotation"), _T("Coordonnées du centre de rotation des objets"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_CentreRotation);
    Menu_PositionSource = new wxMenuItem(Menu_Affichage, ID_MENUITEM8, _T("Position de la source"), _T("Coordonnées de la source de lumière"), wxITEM_NORMAL);
    Menu_Affichage->Append(Menu_PositionSource);
    MenuBar_Globale->Append(Menu_Affichage, _T("Affichage"));
    Menu_Primitive = new wxMenu();
    Menu_AjouteCone = new wxMenuItem(Menu_Primitive, ID_MENUITEM32, _T("Cône"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteCone);
    Menu_AjouteCube = new wxMenuItem(Menu_Primitive, ID_MENUITEM33, _T("Cube"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteCube);
    Menu_AjouteCylindre = new wxMenuItem(Menu_Primitive, ID_MENUITEM34, _T("Cylindre"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteCylindre);
    Menu_AjouteEllipsoide = new wxMenuItem(Menu_Primitive, ID_MENUITEM35, _T("Ellipsoïde"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteEllipsoide);
    Menu_AjouteFacette = new wxMenuItem(Menu_Primitive, ID_MENUITEM36, _T("Facette"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteFacette);
    Menu_AjouteSphere = new wxMenuItem(Menu_Primitive, ID_MENUITEM37, _T("Sphère"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteSphere);
    Menu_AjouteIcosaedre = new wxMenuItem(Menu_Primitive, ID_MENUITEM38, _T("Icosaèdre"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteIcosaedre);
    Menu_AjouteTore = new wxMenuItem(Menu_Primitive, ID_MENUITEM51, _T("Tore"), wxEmptyString, wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_AjouteTore);
    Menu_Primitive->AppendSeparator();
    Menu_SupprimerDerniere = new wxMenuItem(Menu_Primitive, ID_MENUITEM39, _T("Supprimer la dernière"), _T("Pour supprimer la dernière primitive ajoutée"), wxITEM_NORMAL);
    Menu_Primitive->Append(Menu_SupprimerDerniere);
    MenuBar_Globale->Append(Menu_Primitive, _T("Primitive"));
    Menu_Reperage = new wxMenu();
    Menu_ReperagePoint = new wxMenuItem(Menu_Reperage, ID_MENUITEM9, _T("Point"), wxEmptyString, wxITEM_NORMAL);
    Menu_Reperage->Append(Menu_ReperagePoint);
    Menu_ReperageFacette = new wxMenuItem(Menu_Reperage, ID_MENUITEM10, _T("Facette"), wxEmptyString, wxITEM_NORMAL);
    Menu_Reperage->Append(Menu_ReperageFacette);
    Menu_ReperageGroupe = new wxMenuItem(Menu_Reperage, ID_MENUITEM11, _T("Groupe"), wxEmptyString, wxITEM_NORMAL);
    Menu_Reperage->Append(Menu_ReperageGroupe);
    Menu_ReperageMateriau = new wxMenuItem(Menu_Reperage, ID_MENUITEM12, _T("Matériau"), wxEmptyString, wxITEM_NORMAL);
    Menu_Reperage->Append(Menu_ReperageMateriau);
    Menu_ReperageObjet = new wxMenuItem(Menu_Reperage, ID_MENUITEM13, _T("Objet"), wxEmptyString, wxITEM_NORMAL);
    Menu_Reperage->Append(Menu_ReperageObjet);
    Menu_Reperage->AppendSeparator();
    Menu_SensDesNormales = new wxMenuItem(Menu_Reperage, ID_MENUITEM14, _T("Sens des normales\t(n)"), _T("Colorise différemment les facettes mal orientées"), wxITEM_CHECK);
    Menu_Reperage->Append(Menu_SensDesNormales);
    Menu_Reperage->AppendSeparator();
    Menu_Reperage_Couleurs_Facettes = new wxMenuItem(Menu_Reperage, menu_reperage_couleurs_facettes, _T("Couleurs facettes"), wxEmptyString, wxITEM_CHECK);
    Menu_Reperage->Append(Menu_Reperage_Couleurs_Facettes);
    Menu_Reperage_Couleurs_Facettes->Enable(false);
    Menu_Reperage_Couleurs_Groupes = new wxMenuItem(Menu_Reperage, menu_reperage_couleurs_groupes, _T("Couleurs groupes"), wxEmptyString, wxITEM_CHECK);
    Menu_Reperage->Append(Menu_Reperage_Couleurs_Groupes);
    Menu_Reperage_Couleurs_Materiaux = new wxMenuItem(Menu_Reperage, menu_reperage_couleurs_materiaux, _T("Couleurs matériaux"), wxEmptyString, wxITEM_CHECK);
    Menu_Reperage->Append(Menu_Reperage_Couleurs_Materiaux);
    MenuBar_Globale->Append(Menu_Reperage, _T("Repérage"));
    Menu_Image = new wxMenu();
    MenuItem_ImageJpeg = new wxMenuItem(Menu_Image, ID_MENUITEM50, _T("Enregistrer au format jpeg"), wxEmptyString, wxITEM_NORMAL);
    Menu_Image->Append(MenuItem_ImageJpeg);
    MenuItem_ImagePng = new wxMenuItem(Menu_Image, ID_MENUITEM49, _T("Enregistrer au format png"), wxEmptyString, wxITEM_NORMAL);
    Menu_Image->Append(MenuItem_ImagePng);
    MenuItem_ImagePpm = new wxMenuItem(Menu_Image, ID_MENUITEM48, _T("Enregistrer au format ppm"), wxEmptyString, wxITEM_NORMAL);
    Menu_Image->Append(MenuItem_ImagePpm);
    MenuBar_Globale->Append(Menu_Image, _T("Image"));
    Menu_Outils = new wxMenu();
    Menu_RAZ_SelectionFacettes = new wxMenuItem(Menu_Outils, ID_MENUITEM40, _T("RAZ de sélection de facettes"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Menu_RAZ_SelectionFacettes);
    Menu_MasquerFacettes = new wxMenuItem(Menu_Outils, ID_MENUITEM41, _T("Masquer les facettes sélectionnées"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Menu_MasquerFacettes);
    Menu_SupprimerFacettes = new wxMenuItem(Menu_Outils, ID_MENUITEM42, _T("Supprimer les facettes sélectionnées"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Menu_SupprimerFacettes);
    Menu_Outils->AppendSeparator();
    Outils_Selec_Depl = new wxMenuItem(Menu_Outils, ID_MENUITEM15, _T("Sélections et déplacements"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_Selec_Depl);
    Outils_Modifications = new wxMenuItem(Menu_Outils, ID_MENUITEM16, _T("Modifications"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_Modifications);
    Outils_choix_afficher = new wxMenuItem(Menu_Outils, ID_MENUITEM28, _T("Choix des objets à afficher"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_choix_afficher);
    Outils_Reafficher = new wxMenuItem(Menu_Outils, ID_MENUITEM29, _T("Réafficher les éléments masqués"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_Reafficher);
    Outils_Supprimer_Masques = new wxMenuItem(Menu_Outils, ID_MENUITEM30, _T("Supprimer les éléments masqués"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_Supprimer_Masques);
    Outils_UnDelete = new wxMenuItem(Menu_Outils, ID_MENUITEM31, _T("Restituer les éléments supprimés"), wxEmptyString, wxITEM_NORMAL);
    Menu_Outils->Append(Outils_UnDelete);
    MenuBar_Globale->Append(Menu_Outils, _T("Outils"));
    Menu_Transformations = new wxMenu();
    MenuItem_SigneX = new wxMenuItem(Menu_Transformations, ID_MENUITEM17, _T("X => - X"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_SigneX);
    MenuItem_SigneY = new wxMenuItem(Menu_Transformations, ID_MENUITEM18, _T("Y => - Y"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_SigneY);
    MenuItem_SigneZ = new wxMenuItem(Menu_Transformations, ID_MENUITEM19, _T("Z => - Z"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_SigneZ);
    Menu_Transformations->AppendSeparator();
    MenuItem_PermXY = new wxMenuItem(Menu_Transformations, ID_MENUITEM20, _T("X <=> Y"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_PermXY);
    MenuItem_PermXZ = new wxMenuItem(Menu_Transformations, ID_MENUITEM21, _T("X <=> Z"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_PermXZ);
    MenuItem_PermYZ = new wxMenuItem(Menu_Transformations, ID_MENUITEM22, _T("Y <=> Z"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_PermYZ);
    Menu_Transformations->AppendSeparator();
    MenuItem_PermXYZ = new wxMenuItem(Menu_Transformations, ID_MENUITEM23, _T("X=>Y=>Z=>X"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(MenuItem_PermXYZ);
    Menu_Transformations->AppendSeparator();
    Inverser_Toutes_les_Normales = new wxMenuItem(Menu_Transformations, ID_MENUITEM24, _T("Inverser toutes les normales"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(Inverser_Toutes_les_Normales);
    Inverse_All_Selected_Normales = new wxMenuItem(Menu_Transformations, ID_MENUITEM27, _T("Inverser les normales Sélectionnées"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(Inverse_All_Selected_Normales);
    Menu_Transformations->AppendSeparator();
    Menu_DeplacerBdd = new wxMenuItem(Menu_Transformations, ID_MENUITEM25, _T("Déplacer la BDD"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(Menu_DeplacerBdd);
    Menu_FacteurEchelleBdd = new wxMenuItem(Menu_Transformations, ID_MENUITEM26, _T("Facteur d\'échelle"), wxEmptyString, wxITEM_NORMAL);
    Menu_Transformations->Append(Menu_FacteurEchelleBdd);
    MenuBar_Globale->Append(Menu_Transformations, _T("Transformations"));
    Menu_Options = new wxMenu();
    Menu_CouleurDesGroupes = new wxMenuItem(Menu_Options, ID_MENUITEM43, _T("Couleurs des groupes"), wxEmptyString, wxITEM_NORMAL);
    Menu_Options->Append(Menu_CouleurDesGroupes);
    Menu_RelirePalette = new wxMenuItem(Menu_Options, ID_MENUITEM44, _T("Relire une palette"), wxEmptyString, wxITEM_NORMAL);
    Menu_Options->Append(Menu_RelirePalette);
    Menu_EnregistrerPalette = new wxMenuItem(Menu_Options, ID_MENUITEM45, _T("Enregistrer la palette"), wxEmptyString, wxITEM_NORMAL);
    Menu_Options->Append(Menu_EnregistrerPalette);
    Menu_Options->AppendSeparator();
    Menu_ZoomAuto = new wxMenuItem(Menu_Options, ID_MENUITEM46, _T("Zoom automatique"), wxEmptyString, wxITEM_NORMAL);
    Menu_Options->Append(Menu_ZoomAuto);
    Menu_CentrageAuto = new wxMenuItem(Menu_Options, ID_MENUITEM47, _T("Centrage automatique"), wxEmptyString, wxITEM_NORMAL);
    Menu_Options->Append(Menu_CentrageAuto);
    MenuBar_Globale->Append(Menu_Options, _T("Options"));
    Menu_Aide = new wxMenu();
    MenuItem_Aide = new wxMenuItem(Menu_Aide, idMenuHelp, _T("Aide\tF1"), _T("Affiche une aide des commandes et des menus sous forme de fichier html"), wxITEM_NORMAL);
    Menu_Aide->Append(MenuItem_Aide);
    MenuItem_About = new wxMenuItem(Menu_Aide, idMenuAbout, _T("A Propos"), _T("Affiche quelques informations sur la génération de l\'application"), wxITEM_NORMAL);
    Menu_Aide->Append(MenuItem_About);
    MenuBar_Globale->Append(Menu_Aide, _T("Aide"));
    SetMenuBar(MenuBar_Globale);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    Timer_Save.SetOwner(this, ID_TIMER1);
    Timer_Save.Start(300000, false);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_ZoomPlusClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_ZoomMoinsClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_GaucheClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_DroiteClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_HautClick);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_BasClick);
    GLCanvas->Connect(wxEVT_PAINT,(wxObjectEventFunction)&OvniFrame::OnGLCanvasPaint,0,this);
    Connect(ID_SLIDER_X,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&OvniFrame::OnSlider_xCmdScroll);
    Connect(ID_SLIDER_X,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&OvniFrame::OnSlider_xCmdScroll);
    Connect(ID_SLIDER_Y,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&OvniFrame::OnSlider_yCmdScroll);
    Connect(ID_SLIDER_Y,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&OvniFrame::OnSlider_yCmdScroll);
    Connect(ID_SLIDER_Z,wxEVT_SCROLL_THUMBTRACK,(wxObjectEventFunction)&OvniFrame::OnSlider_zCmdScroll);
    Connect(ID_SLIDER_Z,wxEVT_COMMAND_SLIDER_UPDATED,(wxObjectEventFunction)&OvniFrame::OnSlider_zCmdScroll);
    Connect(idOpenFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_OpenSelected);
    Connect(idReopenFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReOpenSelected);
    Connect(idAddFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AddFileSelected);
    Connect(idSaveFile,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_EnregistrerSelected);
    Connect(idSaveFileAs,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Enregistrer_Sous);
    Connect(idProperties,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ProprietesSelected);
    Connect(idPrefs,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_PreferencesSelected);
    Connect(idHardware,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Hardware3DSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnQuit);
    Connect(menu_Affichage_Points,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_PointsSelected);
    Connect(menu_Affichage_Filaire,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_FilaireSelected);
    Connect(menu_Affichage_Plein,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_PleinSelected);
    Connect(menu_Affichage_Axes,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_AxesSelected);
    Connect(menu_Affichage_Boite,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_BoiteSelected);
    Connect(menu_Affichage_Source,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Affichage_SourceSelected);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_OrigineSelected);
    Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_VueDeFaceSelected);
    Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_VueDeProfilSelected);
    Connect(ID_MENUITEM4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_VueDeDessusSelected);
    Connect(ID_MENUITEM5,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_PositionObservateurSelected);
    Connect(ID_MENUITEM6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ZoomSpecifiqueSelected);
    Connect(ID_MENUITEM7,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_CentreRotationSelected);
    Connect(ID_MENUITEM8,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_PositionSourceSelected);
    Connect(ID_MENUITEM32,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteConeSelected);
    Connect(ID_MENUITEM33,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteCubeSelected);
    Connect(ID_MENUITEM34,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteCylindreSelected);
    Connect(ID_MENUITEM35,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteEllipsoideSelected);
    Connect(ID_MENUITEM36,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteFacetteSelected);
    Connect(ID_MENUITEM37,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteSphereSelected);
    Connect(ID_MENUITEM38,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteIcosaedreSelected);
    Connect(ID_MENUITEM51,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_AjouteToreSelected);
    Connect(ID_MENUITEM39,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_SupprimerDerniereSelected);
    Connect(ID_MENUITEM9,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReperagePointSelected);
    Connect(ID_MENUITEM10,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReperageFacetteSelected);
    Connect(ID_MENUITEM11,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReperageGroupeSelected);
    Connect(ID_MENUITEM12,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReperageMateriauSelected);
    Connect(ID_MENUITEM13,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReperageObjetSelected);
    Connect(ID_MENUITEM14,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_SensDesNormalesSelected);
    Connect(menu_reperage_couleurs_facettes,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Reperage_Couleurs_FacettesSelected);
    Connect(menu_reperage_couleurs_groupes,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Reperage_Couleurs_GroupesSelected);
    Connect(menu_reperage_couleurs_materiaux,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Reperage_Couleurs_MateriauxSelected);
    Connect(ID_MENUITEM50,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenuItem_ImageJpegSelected);
    Connect(ID_MENUITEM49,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenuItem_ImagePngSelected);
    Connect(ID_MENUITEM48,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenuItem_ImagePpmSelected);
    Connect(ID_MENUITEM40,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_RAZ_SelectionFacettesSelected);
    Connect(ID_MENUITEM41,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_MasquerFacettesSelected);
    Connect(ID_MENUITEM42,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_SupprimerFacettesSelected);
    Connect(ID_MENUITEM15,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_Selec_DeplSelected);
    Connect(ID_MENUITEM16,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_ModificationsSelected);
    Connect(ID_MENUITEM28,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_choix_afficherSelected);
    Connect(ID_MENUITEM29,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_ReafficherSelected);
    Connect(ID_MENUITEM30,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_Supprimer_MasquesSelected);
    Connect(ID_MENUITEM31,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_UnDeleteSelected);
    Connect(ID_MENUITEM17,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_XminusX);
    Connect(ID_MENUITEM18,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_YminusY);
    Connect(ID_MENUITEM19,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ZminusZ);
    Connect(ID_MENUITEM20,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_XtoY);
    Connect(ID_MENUITEM21,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_XtoZ);
    Connect(ID_MENUITEM22,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_YtoZ);
    Connect(ID_MENUITEM23,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_XtoYtoZ);
    Connect(ID_MENUITEM24,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Inverser_Toutes_les_NormalesSelected);
    Connect(ID_MENUITEM27,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Inverse_All_Selected_NormalesSelected);
    Connect(ID_MENUITEM25,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_DeplacerBddSelected);
    Connect(ID_MENUITEM26,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_FacteurEchelleBddSelected);
    Connect(ID_MENUITEM43,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_CouleurDesGroupesSelected);
    Connect(ID_MENUITEM44,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_RelirePaletteSelected);
    Connect(ID_MENUITEM45,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_EnregistrerPaletteSelected);
    Connect(ID_MENUITEM46,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ZoomAutoSelected);
    Connect(ID_MENUITEM47,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_CentrageAutoSelected);
    Connect(idMenuHelp,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenuItem_AideSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnAbout);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&OvniFrame::OnTimer_SaveTrigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&OvniFrame::OnClose);
    //*)

    // Initialement créés par wxSmith (pour wxWidgets 2.8.12) ainsi que les déclarations des ID_POPUP_*

#if wxCHECK_VERSION(3,0,0)
// wxCustomButton existe (généré via C::B + wxWidgets 3.0 et +) mais plantages. Utiliser plutôt wxBitmapToggleButton qui donne le même comportement
// mais n'existe pas sous wxWidget 2.8.12. De plus non géré pas wxSmith => génération manuelle.
    Button_Points  = new wxBitmapToggleButton(Panel1,ID_BUTTON7 ,wxBitmap(wxImage(_T("./Icones/points.png"))),    wxPoint(460,0),wxSize(24,24),0,wxDefaultValidator, _T("ID_BUTTON7"));
    Button_Filaire = new wxBitmapToggleButton(Panel1,ID_BUTTON8 ,wxBitmap(wxImage(_T("./Icones/filaire.png"))),   wxPoint(484,0),wxSize(24,24),0,wxDefaultValidator, _T("ID_BUTTON8"));
    Button_Plein   = new wxBitmapToggleButton(Panel1,ID_BUTTON9 ,wxBitmap(wxImage(_T("./Icones/plein.png"))),     wxPoint(508,0),wxSize(24,24),0,wxDefaultValidator, _T("ID_BUTTON9"));
    Button_Axes    = new wxBitmapToggleButton(Panel1,ID_BUTTON10,wxBitmap(wxImage(_T("./Icones/axes.png"))),      wxPoint(532,0),wxSize(24,24),0,wxDefaultValidator, _T("ID_BUTTON10"));
    Button_Boite   = new wxBitmapToggleButton(Panel1,ID_BUTTON11,wxBitmap(wxImage(_T("./Icones/boite.png"))),     wxPoint(556,0),wxSize(24,24),0,wxDefaultValidator, _T("ID_BUTTON11"));
    Button_Normale_Barycentre = new wxBitmapToggleButton(Panel1,ID_BUTTON12,wxBitmap(wxImage(_T("./Icones/B_Normales.png"))),wxPoint(580,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON12"));
    Button_Normales_Sommets   = new wxBitmapToggleButton(Panel1,ID_BUTTON13,wxBitmap(wxImage(_T("./Icones/S_Normales.png"))),wxPoint(604,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON13"));
    Button_Source  = new wxBitmapToggleButton(Panel1,ID_BUTTON14,wxBitmap(wxImage(_T("./Icones/sun.png"))),       wxPoint(628,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON14"));
    Button_Gouraud = new wxBitmapToggleButton(Panel1,ID_BUTTON15,wxBitmap(wxImage(_T("./Icones/gouraud.png"))),   wxPoint(652,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON15"));
    Button_Outils  = new wxBitmapToggleButton(Panel1,ID_BUTTON16,wxBitmap(wxImage(_T("./Icones/outils.png"))),    wxPoint(676,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON16"));
    Button_Modifs  = new wxBitmapToggleButton(Panel1,ID_BUTTON17,wxBitmap(wxImage(_T("./Icones/modifs.png"))),    wxPoint(700,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON17"));
    Button_Sliders = new wxBitmapToggleButton(Panel1,ID_BUTTON18,wxBitmap(wxImage(_T("./Icones/wxSlider16.png"))),wxPoint(724,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON18"));
    Button_Groupes = new wxBitmapToggleButton(Panel1,ID_BUTTON19,wxBitmap(wxImage(_T("./Icones/groupes.png"))),   wxPoint(748,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON19"));
    Button_Materiaux=new wxBitmapToggleButton(Panel1,ID_BUTTON20,wxBitmap(wxImage(_T("./Icones/materiau.png"))),  wxPoint(772,0),wxSize(24,24),0,wxDefaultValidator,_T("ID_BUTTON20"));
#else
    Button_Points = new wxCustomButton(Panel1,ID_BUTTON7,wxEmptyString,wxBitmap(wxImage(_T("./Icones/points.png"))),wxPoint(460,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_TOP,wxDefaultValidator,_T("ID_BUTTON7"));
    Button_Points->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/points.png"))));
    Button_Points->SetBitmapDisabled(Button_Points->CreateBitmapDisabled(Button_Points->GetBitmapLabel()));
    Button_Filaire = new wxCustomButton(Panel1,ID_BUTTON8,wxEmptyString,wxBitmap(wxImage(_T("./Icones/filaire.png"))),wxPoint(484,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_TOP,wxDefaultValidator,_T("ID_BUTTON8"));
    Button_Filaire->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/filaire.png"))));
    Button_Filaire->SetBitmapDisabled(Button_Filaire->CreateBitmapDisabled(Button_Filaire->GetBitmapLabel()));
    Button_Plein = new wxCustomButton(Panel1,ID_BUTTON9,wxEmptyString,wxBitmap(wxImage(_T("./Icones/plein.png"))),wxPoint(508,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON9"));
    Button_Plein->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/plein.png"))));
    Button_Plein->SetBitmapDisabled(Button_Plein->CreateBitmapDisabled(Button_Plein->GetBitmapLabel()));
    Button_Axes = new wxCustomButton(Panel1,ID_BUTTON10,wxEmptyString,wxBitmap(wxImage(_T("./Icones/axes.png"))),wxPoint(532,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON10"));
    Button_Axes->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/axes.png"))));
    Button_Axes->SetBitmapDisabled(Button_Axes->CreateBitmapDisabled(Button_Axes->GetBitmapLabel()));
    Button_Boite = new wxCustomButton(Panel1,ID_BUTTON11,wxEmptyString,wxBitmap(wxImage(_T("./Icones/boite.png"))),wxPoint(556,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON11"));
    Button_Boite->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/boite.png"))));
    Button_Boite->SetBitmapDisabled(Button_Boite->CreateBitmapDisabled(Button_Boite->GetBitmapLabel()));
    Button_Normale_Barycentre = new wxCustomButton(Panel1,ID_BUTTON12,wxEmptyString,wxBitmap(wxImage(_T("./Icones/B_Normales.png"))),wxPoint(580,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON12"));
    Button_Normale_Barycentre->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/B_Normales.png"))));
    Button_Normale_Barycentre->SetBitmapDisabled(Button_Normale_Barycentre->CreateBitmapDisabled(Button_Normale_Barycentre->GetBitmapLabel()));
    Button_Normales_Sommets = new wxCustomButton(Panel1,ID_BUTTON13,wxEmptyString,wxBitmap(wxImage(_T("./Icones/S_Normales.png"))),wxPoint(604,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON13"));
    Button_Normales_Sommets->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/S_Normales.png"))));
    Button_Normales_Sommets->SetBitmapDisabled(Button_Normales_Sommets->CreateBitmapDisabled(Button_Normales_Sommets->GetBitmapLabel()));
    Button_Source = new wxCustomButton(Panel1,ID_BUTTON14,wxEmptyString,wxBitmap(wxImage(_T("./Icones/sun.png"))),wxPoint(628,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON14"));
    Button_Source->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/sun.png"))));
    Button_Source->SetBitmapDisabled(Button_Source->CreateBitmapDisabled(Button_Source->GetBitmapLabel()));
    Button_Gouraud = new wxCustomButton(Panel1,ID_BUTTON15,wxEmptyString,wxBitmap(wxImage(_T("./Icones/gouraud.png"))),wxPoint(652,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON15"));
    Button_Gouraud->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/gouraud.png"))));
    Button_Gouraud->SetBitmapDisabled(Button_Gouraud->CreateBitmapDisabled(Button_Gouraud->GetBitmapLabel()));
    Button_Outils = new wxCustomButton(Panel1,ID_BUTTON16,wxEmptyString,wxBitmap(wxImage(_T("./Icones/outils.png"))),wxPoint(676,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON16"));
    Button_Outils->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/outils.png"))));
    Button_Outils->SetBitmapDisabled(Button_Outils->CreateBitmapDisabled(Button_Outils->GetBitmapLabel()));
    Button_Modifs = new wxCustomButton(Panel1,ID_BUTTON17,wxEmptyString,wxBitmap(wxImage(_T("./Icones/modifs.png"))),wxPoint(700,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON17"));
    Button_Modifs->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/modifs.png"))));
    Button_Modifs->SetBitmapDisabled(Button_Modifs->CreateBitmapDisabled(Button_Modifs->GetBitmapLabel()));
    Button_Sliders = new wxCustomButton(Panel1,ID_BUTTON18,wxEmptyString,wxBitmap(wxImage(_T("./Icones/wxSlider16.png"))),wxPoint(724,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON18"));
    Button_Sliders->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/wxSlider16.png"))));
    Button_Sliders->SetBitmapDisabled(Button_Sliders->CreateBitmapDisabled(Button_Sliders->GetBitmapLabel()));
    Button_Groupes = new wxCustomButton(Panel1,ID_BUTTON19,wxEmptyString,wxBitmap(wxImage(_T("./Icones/groupes.png"))),wxPoint(748,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON19"));
    Button_Groupes->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/groupes.png"))));
    Button_Groupes->SetBitmapDisabled(Button_Groupes->CreateBitmapDisabled(Button_Groupes->GetBitmapLabel()));
    Button_Materiaux = new wxCustomButton(Panel1,ID_BUTTON20,wxEmptyString,wxBitmap(wxImage(_T("./Icones/materiau.png"))),wxPoint(772,0),wxSize(24,24),wxCUSTBUT_TOGGLE|wxCUSTBUT_BOTTOM,wxDefaultValidator,_T("ID_BUTTON20"));
    Button_Materiaux->SetBitmapSelected(wxBitmap(wxImage(_T("./Icones/materiau.png"))));
    Button_Materiaux->SetBitmapDisabled(Button_Materiaux->CreateBitmapDisabled(Button_Materiaux->GetBitmapLabel()));
#endif // wxCHECK_VERSION

    Connect(ID_BUTTON7, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_PointsToggle);
    Connect(ID_BUTTON8, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_FilaireToggle);
    Connect(ID_BUTTON9, wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_PleinToggle);
    Connect(ID_BUTTON10,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_AxesToggle);
    Connect(ID_BUTTON11,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_BoiteToggle);
    Connect(ID_BUTTON12,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_Normale_BarycentreClick);
    Connect(ID_BUTTON13,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_Normales_SommetsClick);
    Connect(ID_BUTTON14,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_SourceToggle);
    Connect(ID_BUTTON15,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_GouraudToggle);
    Connect(ID_BUTTON16,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_OutilsToggle);
    Connect(ID_BUTTON17,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_ModifsToggle);
    Connect(ID_BUTTON18,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_SlidersToggle);
    Connect(ID_BUTTON19,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_GroupesToggle);
    Connect(ID_BUTTON20,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&OvniFrame::OnButton_MateriauxToggle);

// Popup du clic droit dans la fenètre OpenGL
    Connect(ID_POPUP_RAZ,       wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_RAZ_SelectionFacettesSelected);
    Connect(ID_POPUP_CENTRER,   wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Centrer_sur_SelectionSelected);
    Connect(ID_POPUP_ETENDRE,   wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Etendre_la_SelectionSelected);
    Connect(ID_POPUP_DELETE,    wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_SupprimerFacettesSelected);
    Connect(ID_POPUP_MASQUER,   wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_MasquerFacettesSelected);
    Connect(ID_POPUP_UNDELETE,  wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_UnDeleteSelected);
    Connect(ID_POPUP_DEMASQUER, wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnOutils_ReafficherSelected);
    Connect(ID_POPUP_INVERSER_N,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_Inverse_All_Selected_NormalesSelected);
    Connect(ID_POPUP_PARCOURS_I,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Reverse_ParcoursSelected);
    Connect(ID_POPUP_NORM_F,    wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Afficher_NormalesSelected);
    Connect(ID_POPUP_NORM_S,    wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Afficher_NormalesSommetsSelected);
    Connect(ID_POPUP_RAZ_SELECT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_Raz_Select_FSelected);
    Connect(ID_POPUP_FLAT,      wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_ForcerFacettesPlanesSelected);
    Connect(ID_POPUP_NOT_FLAT,  wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnPopup_ForcerFacettesNonPlanesSelected);

//    ID_POPUP_NORM_F_bis = ID_POPUP_NORM_F;
//    ID_POPUP_NORM_S_bis = ID_POPUP_NORM_S;

// Connexion manuelle pour la réouverture spécifique de fichiers 3ds (car non vréé via wxSmith)
    Connect(idReopenFile3ds,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OvniFrame::OnMenu_ReOpen3dsSelected);

// Affichage d'une aide lors du survol de boutons par la souris
    Button_Points->   SetToolTip(_T("Affichage des Points"));
    Button_Filaire->  SetToolTip(_T("Affichage des Arêtes"));
    Button_Plein->    SetToolTip(_T("Affichage des Facettes"));
    Button_Axes->     SetToolTip(_T("Affichage des Axes"));
    Button_Boite->    SetToolTip(_T("Affichage de la Boîte englobante"));
    Button_Normale_Barycentre->SetToolTip(_T("Affichage des Normales aux Barycentres des facettes sélectionnées"));
    Button_Normales_Sommets->  SetToolTip(_T("Affichage des Normales aux Sommets des facettes sélectionnées"));
    Button_Source->   SetToolTip(_T("Affichage de la Source de lumière"));
    Button_Gouraud->  SetToolTip(_T("Affichage de facettes planes ou lissage de Gouraud"));
    Button_Outils->   SetToolTip(_T("Sélections et déplacements"));
    Button_Modifs->   SetToolTip(_T("Modifications"));
    Button_Sliders->  SetToolTip(_T("Cacher/Afficher les curseurs de rotation"));
    Button_Groupes->  SetToolTip(_T("Coloriser les groupes"));
    Button_Materiaux->SetToolTip(_T("Coloriser les matériaux"));

//    wxSize* ClientSize = new wxSize(800,600);
//    this->SetMinSize(*ClientSize);
    //OpenGL initialisation code
    InitOpenGL();
    int argc=1;
    char *argv=(char*)"Ovni";
    glutInit(&argc,&argv);  // On pourrait récupérer les véritables arguments via wxGetApp().argc et wxGetApp.argv[*], mais peu d'intérêt ici pout glutInit, de + ce sont des wxString
//    glutSetOption(GLUT_MULTISAMPLE,4);    // à faire avant tracé de fenêtre Glut, mais fait aussi dans wxGLCanvas
//    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);   // Ajout 02/2020 pour test : serait déjà fait dans wxGLCanvas ?
    InitBoutons();
//    wxPrefs_pos=wxDefaultPosition;
//    wxModificationPanel_pos= wxDefaultPosition;
//    wxPropertiesPanel_pos  = wxDefaultPosition;
    CentreRotation_Panel            = new CentreRotation(this,wxID_ANY);
    ChangerEchelleBdd_Panel         = new ChangerEchelleBdd(this,wxID_ANY);
    ChoixAffichageObjets_Panel      = new ChoixAffichageObjets(this,wxID_ANY);
    Cone_Panel                      = new Cone(this,wxID_ANY);
    CouleursGroupes_Panel           = new CouleursGroupes(this,wxID_ANY);
    Cube_Panel                      = new Cube(this,wxID_ANY);
    Cylindre_Panel                  = new Cylindre(this,wxID_ANY);
    DeplacerBdd_Panel               = new DeplacerBdd(this,wxID_ANY);
    Ellipsoide_Panel                = new Ellipsoide(this,wxID_ANY);
    Facette_Panel                   = new Facette(this,wxID_ANY);
    Icosaedre_Panel                 = new Icosaedre(this,wxID_ANY);
    Manipulations_Panel             = new ManipulationsPanel(this,wxID_ANY);
    Modifications_Panel             = new ModificationPanel(this,wxID_ANY);
    PositionObsAzimutSite_Panel     = new PositionObs_AzimutSite(this,wxID_ANY);
    PositionSource_Panel            = new PositionSource(this,wxID_ANY);
    Preferences_Panel               = new Prefs_Dialog(this,wxID_ANY);//,wxPrefs_pos);
    Properties_Panel                = new PropertiesPanel(this,wxID_ANY);
    ReperageFacette_Panel           = new ReperageFacette(this,wxID_ANY);
    ReperageGroupe_Panel            = new ReperageGroupe(this,wxID_ANY);
    ReperageMateriau_Panel          = new ReperageMateriau(this,wxID_ANY);
    ReperageObjet_Panel             = new ReperageObjet(this,wxID_ANY);
    ReperagePoint_Panel             = new ReperagePoint(this,wxID_ANY);
    Rotation_Panel                  = new RotationPanel(this,wxID_ANY);
    Scale_Panel                     = new ScalePanel(this,wxID_ANY);
    Selections_Panel                = new SelectionPanel(this,wxID_ANY);
    Selections_Manuelles_Facettes   = new SelectionManuelleFacettes(this,wxID_ANY);
    Selections_Manuelles_Objets     = new SelectionManuelleObjets(this,wxID_ANY);
    Sphere_Panel                    = new Sphere(this,wxID_ANY);
    Tore_Panel                      = new Tore(this,wxID_ANY);
    Translation_Panel               = new TranslationPanel(this,wxID_ANY);
    ZoomSpecifique_Panel            = new ZoomSpecifique(this,wxID_ANY);
    AideHtml_Panel                  = new Aide_html(this,wxID_ANY);

    New_file = true;
//    s_extFileDef = wxEmptyString;

    int arg_c = wxGetApp().argc;
    wxString par_0(wxGetApp().argv[0]);         // Nom de la commande
    wxString par_1;
    if (arg_c >= 2) par_1=wxGetApp().argv[1];   // Nom du fichier passé éventuellement en ligne de commande

#ifdef __WXMSW__
    SetIcon(wxICON(avion)); // Pour afficher l'icône des fenètres : barre de tâche, en haut des fenètres,... Utile ou déjà fait en partie ? à ne faire que sous Windows ?
#endif

    wxString Version = AutoVersion::FULLVERSION_STRING ;
    this->SetTitle(this->GetTitle() + _T(" ") + Version.BeforeLast(_T('.')));   // On récupère le titre de la fenêtre et on y ajoute le numéro
                                                                                // de version : éliminer le dernier champ
    if (verbose) {
        printf("OvniFrame::OvniFrame : nb args  : %d\n",arg_c);
        printf("OvniFrame::OvniFrame : commande : %s\n",(const char *)par_0.mb_str());
    }

    if (Element == nullptr) {
        if (verbose) printf("Creation de BddInter Element\n");
        Element=new BddInter(dynamic_cast<wxWindow*>(this->GLCanvas), wxID_ANY, GLCanvasAttributes_1, wxDefaultPosition,
            wxSize(1, 1), wxSUNKEN_BORDER, verbose); // Taille minimale (1 pixel) car sera ajusté par la suite (+ évite une zone blanche si pas de fichier sélectionné !)
//          wxSize(300, 300), wxSUNKEN_BORDER);
///        this->GLCanvas = dynamic_cast<wxGLCanvas*>(Element); // Les this->GLCanvas commentés par /// ne semblent pas servir à quelque chose !!!
//        this->GLCanvas->SetSize(wxSize(860, 600));    // Sera fait plus tard ...
        Element->Numero_base = 0;
    }

    if (arg_c >= 2) {
        printf("fichier  : %s\n",(const char *)par_1.mb_str());
        Element->set_file(par_1);
        New_file = false;
    }

    wxString Fichier_svg = Element->wxWorkDir + Element->Fichier_Autosvg;
    if (wxFileExists(Fichier_svg)) {
        // Un fichier Autosave.bdd existe
        wxString wxMessage = _T("Un fichier de sauvegarde automatique ") + Element->Fichier_Autosvg + _T(" existe dans le répertoire de travail.\n\n");
        wxMessage         += _T("Voulez-vous l'utiliser ?\n\n");
        wxMessage         += _T("  *  Si Oui, il vaut mieux l'\"Enregistrer sous ...\" un autre nom dès que possible car elle\n");
        wxMessage         += _T("  *  sera écrasée à la prochaine sauvegarde automatique ou supprimée en sortie !");

        wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Sauvegarde automatique existante..."),
                                        wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION); // Note : avec wxWidgets > 3, sous Windows, plus de wxICON_QUESTION !

        int retour_Show = query->ShowModal();
        query->Destroy();
        if (retour_Show == wxID_YES) {
            Element->set_file(Fichier_svg);
            New_file = false;
        } else {
            // Le fichier Autosave ne sert pas => le supprimer maintenant, sinon il restera tant qu'une nouvelle sauvegarde n'aura pas été activée par le timer !
            wxRemoveFile(Fichier_svg);
        }
        // Note : ne pas mettre Element->exists_Autosave à true ici. Sera fait lors de la première activation du wxTimer Timer_Save.
    }

    Ouvrir_Fichier();
    Timer_Save.Stop();                                                          // Arrêt du timer par précaution
    if (Element->svg_time > 0) Timer_Save.Start(Element->svg_time*60000,false); // Lancer le timer si svg_time > 0

//    printf("Sortie OvniFrame, wxSize x:%d y:%d\n",ClientSize->GetX(), ClientSize->GetY());

    if (verbose) printf("Sortie OvniFrame::OvniFrame\n");
}

bool OvniFrame::OnInit() {
    if (verbose) printf("Entree OvniFrame::OnInit\n");
    const wxSize ClientSize = GetClientSize();
    ResizeOpenGL(ClientSize.x, ClientSize.y);
    if (verbose) printf("Sortie OvniFrame::OnInit") ; //, GL x:%d y:%d\n",ClientSize.x, ClientSize.y);
    return 1;
}

OvniFrame::~OvniFrame() {
    //(*Destroy(OvniFrame)
    //*)
    Timer_Save.Stop();          // Arrêt du timer en sortie de programme (peut être utile en cas de plantage car, apparemment, évite de laisser le timer actif après la sortie).
    Timer_Save.Disconnect();    // et déconnexion de précaution
    DeletePendingEvents();      // Utile ?
    wxTheApp->ExitMainLoop();   // Pour voir si utile en cas de crash dans Ovni
}

void OvniFrame::InitOpenGL(void) {
    if (verbose) printf("Entree OvniFrame::InitOpenGL\n");  // Ne semble plus très utile ... à voir ... et peut-être intégrer ailleurs
    //create opengl context
    m_glcontext = new wxGLContext(GLCanvas);
    //bind the context to the widget
    m_glcontext->SetCurrent(*GLCanvas);

    if (verbose) printf("Sortie OvniFrame::InitOpenGL\n");
}

/** Resize the view port
  * \param iWidth : the new width of the view port
  * \param iHeight: the new height of the view port
  */
void OvniFrame::ResizeOpenGL(int iWidth, int iHeight) {
    int iw, ih;

    if (verbose) printf("Entree OvniFrame::ResizeOpenGL\n");

    iw = iWidth;
    ih = iHeight;
    //to prevent division by 0
    if (ih <= 0) ih = 1;
//    printf("test x %d y %d\n",iw,ih);
    glViewport(0, 0, iw, ih);

	Element->Resize();

    if (verbose) printf("Sortie OvniFrame::ResizeOpenGL\n");
}

void OvniFrame::InitBoutons(void)
{
    if (verbose) printf("Entree OvniFrame::InitBoutons\n");
    if (Menu_Affichage_Points ->IsChecked()) Button_Points ->SetValue(true);    // IsChecked <=> Bouton enfoncé, sinon par défaut en position haute
    if (Menu_Affichage_Filaire->IsChecked()) Button_Filaire->SetValue(true);
    if (Menu_Affichage_Plein  ->IsChecked()) Button_Plein  ->SetValue(true);
    if (Menu_Affichage_Axes   ->IsChecked()) Button_Axes   ->SetValue(true);
    if (Menu_Affichage_Boite  ->IsChecked()) Button_Boite  ->SetValue(true);
    if (Menu_Affichage_Source ->IsChecked()) Button_Source ->SetValue(true);
    Button_Sliders->SetValue(true);     // Bouton enfoncé par défaut
    Slider_x->Enable();
    Slider_y->Enable();
    Slider_z->Enable();

    Panel_Sliders->Update();            // à ce niveau, pas encore affichés, donc ne sert à pas grand chose (idem 3 lignes Panel_Sliders)
    Panel_Sliders->Show(true);          // => Sliders affichés
    Panel_Sliders->Refresh(true);
    if (verbose) printf("Sortie OvniFrame::InitBoutons\n");
}

bool OvniFrame::OnBdd_modifiee() {
    bool retour = true;
    int  retour_Show;
    if (Element->Objetlist.size() == 0) return(true) ;  // Inutile de tester bdd_modifiee car il ne reste rien !
    else {
        int ok = Element->Objetlist.size();
        for (int i=0; i < (int)Element->Objetlist.size(); i++) if (Element->Objetlist[i].deleted) ok--;
        if (ok <= 0) return(true);                      // Idem car là aussi, il ne reste rien !
    }
    if (Element->bdd_modifiee) {
        wxString wxMessage = _T("La Base de Données 3D a été modifiée.\nVoulez-vous :\n");
        wxMessage         += _T("    1 : l'enregistrer puis Quitter . . . . . . . . . (Oui)\n");
#if wxCHECK_VERSION(3,0,0)
        wxMessage         += _T("    2 : quitter sans l'enregistrer . . . . . . . . . (Quitter)\n");
#else
        wxMessage         += _T("    2 : quitter sans l'enregistrer . . . . . . . . . (Non)\n");
#endif // wxCHECK_VERSION
        wxMessage         += _T("    3 : ne pas l'enregistrer ni Quitter Ovni (Annuler) ?");

        wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Question..."),
                                        wxYES_NO | wxYES_DEFAULT | wxCANCEL | wxICON_QUESTION); // Note : avec wxWidgets > 3, sous Windows, plus de wxICON_QUESTION !
#if wxCHECK_VERSION(3,0,0)
        query->SetYesNoCancelLabels(_T("Oui"),_T("Quitter"),_T("Annuler"));
#endif // wxCHECK_VERSION

        wxCommandEvent event_cmd;
        retour_Show = query->ShowModal();
        query->Destroy();
        if (retour_Show == wxID_YES) OnMenu_Enregistrer_Sous(event_cmd);
        if (retour_Show == wxID_CANCEL) retour = false;
    }
    return (retour);
}

void OvniFrame::OnPal_modifiee() {
    if (Element->pal_file_modified) {
        wxString wxMessage = _T("Le fichier de palette des couleurs a été modifié.\n");
        wxMessage         += _T("Voulez-vous l'enregistrer avant de Quitter ?");
        wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Question..."),
                                        wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

        wxCommandEvent event_cmd;
        if (query->ShowModal() == wxID_YES) OnMenu_EnregistrerPaletteSelected(event_cmd);
        query->Destroy();
    }
}

void OvniFrame::OnQuit(wxCommandEvent& event) {
    bool test;
    test = OnBdd_modifiee();
    if (!test) return;
    Element->bdd_modifiee = false;      // Forcer à false pour éviter une double interrogation lors du Close si on a refusé l'enregistrement sous...
    Element->Stocker_ini_file();
    OnPal_modifiee();
    Element->pal_file_modified = false; // Idem
    Close();
}

void OvniFrame::OnAbout(wxCommandEvent& event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _T("Ovni version wxWidgets...")); // <=> wxMessageDialog(this,msg,_T("Ovni version wxWidgets...")).ShowModal();
//    event.Skip();
}

void OvniFrame::OnClose(wxCloseEvent& event) {
    bool test;
    test = OnBdd_modifiee();
    if (!test) return;
    Element->Stocker_ini_file();
    OnPal_modifiee();

    // Suppression éventuelle du fichier de backup (fichier original recopié en *.bak)
    if (Element->CreerBackup && Element->SupprBackup && Element->OK_FichierCree) test = wxRemoveFile(Element->get_file() + _T(".bak")) ;  // Suppression du .bak

    // On est arrivé ici, donc tout s'est bien passé => supprimer la sauvegarde automatique (s'il y en a une)
    // Note : Un fichier Autosave déjà existant ne sera pas affecté si aucune sauvegarde n'a été effectuée lors de cette exécution d'Ovni
    //        Mais normalement, il a été supprimé lors de sa détection à l'ouverture d'Ovni.
    if (Element->exists_Autosave) {
        wxString Fichier_svg = Element->wxWorkDir + Element->Fichier_Autosvg;
        wxRemoveFile(Fichier_svg);
    }

    Destroy();
    exit(0);
}

void OvniFrame::OnGLCanvasPaint(wxPaintEvent& event) {
    if (verbose) printf("Entree OvniFrame::OnGLCanvasPaint\n");

    const wxSize ClientSize = Element->GetClientSize();
//    printf("ClientSize x/y %d %d\n",ClientSize.x, ClientSize.y);
    ResizeOpenGL(ClientSize.x, ClientSize.y);

//    m_glcontext->SetCurrent(*GLCanvas);
    event.Skip();                               /// Indispensable
    if (verbose) printf("Sortie OvniFrame::OnGLCanvasPaint\n");
}

void OvniFrame::OnMenu_NotImplemented(wxCommandEvent& event) {
    // Ne sert plus !
    wxMessageBox(_T("Non encore implémenté"),_T("To Do"));
}

void OvniFrame::OnMenu_Hardware3DSelected(wxCommandEvent& event) {

//! Affichage des caractéristiques de la carte graphique (Hardware 3D)

    char *cptr;
    int   depth=0 ;
    int   redBits=0, greenBits=0, blueBits=0;
    char* get_glstring;
    wxString msg, str;

    msg << _T("Caractéristiques de la carte Graphique :\n\n") ;
    msg << _T("OpenGL Renderer:  \t") ;

    get_glstring = (char *)glGetString(GL_RENDERER);
    cptr = get_glstring;
    while (*cptr != '\0') msg << (wxChar)btowc(*cptr++) ;

    msg << _T("\nOpenGL Vendor:\t\t") ;
    get_glstring = (char *)glGetString(GL_VENDOR);
    cptr = get_glstring;
    while (*cptr != '\0') msg << (wxChar)btowc(*cptr++) ;

    msg << _T("\nOpenGL Version:\t\t") ;
    get_glstring = (char *)glGetString(GL_VERSION);
    cptr = get_glstring;
    while (*cptr != '\0') msg << (wxChar)btowc(*cptr++) ;

    glGetIntegerv(GL_DEPTH_BITS, &depth);
    glGetIntegerv(GL_RED_BITS,   &redBits);
    glGetIntegerv(GL_GREEN_BITS, &greenBits);
    glGetIntegerv(GL_BLUE_BITS,  &blueBits);

    str.Printf(_T("\n\nProfondeur du Z-Buffer : \t\t%d bits\n"),depth);
    msg << str;

    str.Printf(_T("Profondeur couleur (R,G,B) détectée : \t%d, %d, %d\n"), redBits, greenBits, blueBits);
    msg << str ;

    wxMessageBox(msg,_T("Hardware 3D"));
}

void OvniFrame::OnButton_PleinToggle(wxCommandEvent& event) {

//! Affiche / Efface les surfaces des objets via un bouton

    if (Button_Plein->GetValue())
        Menu_Affichage_Plein->Check(true) ;
    else
        Menu_Affichage_Plein->Check(false);

    if (Element != nullptr) {
        Element->show_plein = Button_Plein->GetValue();
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_PleinSelected(wxCommandEvent& event) {

//! Affiche / Efface les surfaces des objets via un menu

    if (Menu_Affichage_Plein->IsChecked())
        Button_Plein->SetValue(true) ;
    else
        Button_Plein->SetValue(false);

    if (Element != nullptr) {
        Element->show_plein = Menu_Affichage_Plein->IsChecked();
        Element->Refresh();
    }
}

void OvniFrame::OnButton_AxesToggle(wxCommandEvent& event) {

//! Affiche / Efface les axes XYZ de la Bdd via un bouton

    if (Button_Axes->GetValue())
        Menu_Affichage_Axes->Check(true) ;
    else
        Menu_Affichage_Axes->Check(false);

    if (Element != nullptr) {
        Element->show_axes = Button_Axes->GetValue();
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_AxesSelected(wxCommandEvent& event) {

//! Affiche / Efface les axes XYZ de la Bdd via un menu

    if (Menu_Affichage_Axes->IsChecked())
        Button_Axes->SetValue(true) ;
    else
        Button_Axes->SetValue(false);

    if (Element != nullptr) {
        Element->show_axes = Menu_Affichage_Axes->IsChecked();
        Element->Refresh();
    }
}

void OvniFrame::OnButton_BoiteToggle(wxCommandEvent& event) {

//! Affiche / Efface la boîte englobante de la Bdd via un bouton

    if(Button_Boite->GetValue())
        Menu_Affichage_Boite->Check(true) ;
    else
        Menu_Affichage_Boite->Check(false);

    if (Element != nullptr) {
        Element->show_box = Button_Boite->GetValue();
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_BoiteSelected(wxCommandEvent& event) {

//! Affiche / Efface la boîte englobante de la Bdd via un menu

    if (Menu_Affichage_Boite->IsChecked())
        Button_Boite->SetValue(true) ;
    else
        Button_Boite->SetValue(false);

    if (Element != nullptr) {
        Element->show_box = Menu_Affichage_Boite->IsChecked() ;
        Element->Refresh();
    }
}

void OvniFrame::OnButton_SourceToggle(wxCommandEvent& event) {

//! Affiche / Efface la source de lumière via un bouton

    if (Button_Source->GetValue())
        Menu_Affichage_Source->Check(true) ;
    else
        Menu_Affichage_Source->Check(false);

    if(Element != nullptr) {
        Element->show_light = Button_Source->GetValue();
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_SourceSelected(wxCommandEvent& event) {

//! Affiche / Efface la source de lumière via le menu

    if (Menu_Affichage_Source->IsChecked())
        Button_Source->SetValue(true) ;
    else
        Button_Source->SetValue(false);

    if(Element != nullptr) {
        Element->show_light = Menu_Affichage_Source->IsChecked();
        Element->Refresh();
    }
}

void OvniFrame::OnButton_PointsToggle(wxCommandEvent& event) {

//! Affiche / Efface les points via un bouton

    if (Button_Points->GetValue())
        Menu_Affichage_Points->Check(true) ;
    else
        Menu_Affichage_Points->Check(false);

    if (Element != nullptr) {
        Element->show_points = Button_Points->GetValue();
        if ((Element->show_points) && (!Element->liste_sommets_OK)) {
            printf("Construction de la liste OpenGL de sommets\n");
            Element->m_gllist = Element->glliste_points;
        }
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_PointsSelected(wxCommandEvent& event) {

//! Affiche / Efface les points via le menu

    if (Menu_Affichage_Points->IsChecked())
        Button_Points->SetValue(true) ;
    else
        Button_Points->SetValue(false);

    if (Element != nullptr) {
        Element->show_points = Menu_Affichage_Points->IsChecked();
        if ((Element->show_points) && (!Element->liste_sommets_OK)) {
            printf("Construction de la liste OpenGL de sommets\n");
            Element->m_gllist = Element->glliste_points;
        }
        Element->Refresh();
    }
}

void OvniFrame::GenereListeAretes() {
    if (!Element->GenereTableauAretes_OK) {
        printf("Construction de GenereTableauAretes\n");
        for (unsigned int i=0; i<Element->Objetlist.size(); ++i) {
             Element->GenereTableauAretes(&(Element->Objetlist[i]));
        }
        Element->GenereTableauAretes_OK = true;
    }
    if (!Element->liste_aretes_OK) {
        printf("Construction de la liste OpenGL d'arêtes\n");
        Element->m_gllist = Element->glliste_lines;
    }
}

void OvniFrame::OnButton_FilaireToggle(wxCommandEvent& event) {

//! Affiche / Efface les arêtes via un bouton

    if (Button_Filaire->GetValue())
        Menu_Affichage_Filaire->Check(true) ;
    else
        Menu_Affichage_Filaire->Check(false);

    if(Element != nullptr) {
        Element->show_lines = Button_Filaire->GetValue();
        if (Element->show_lines) {
            GenereListeAretes();
        }
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Affichage_FilaireSelected(wxCommandEvent& event) {

//! Affiche / Efface les arêtes via le menu

    if (Menu_Affichage_Filaire->IsChecked())
        Button_Filaire->SetValue(true)  ;
    else
        Button_Filaire->SetValue(false) ;

    if (Element != nullptr) {
        Element->show_lines = Menu_Affichage_Filaire->IsChecked();
        if (Element->show_lines) {
            GenereListeAretes();
        }
        Element->Refresh();
    }
}

void OvniFrame::Toggle_Groupes(wxCommandEvent& event) {
    OnButton_GroupesToggle(event);
}

void OvniFrame::OnButton_GroupesToggle(wxCommandEvent& event) {
    if(Element != nullptr) {
        if (Button_Groupes->GetValue()) {
            Menu_Reperage_Couleurs_Groupes  ->Check(true);
            Menu_Reperage_Couleurs_Materiaux->Check(false);
            Menu_Reperage_Couleurs_Facettes ->Check(false);
            Element->groupes  = true;
            Element->materials= false;
        } else {
            Menu_Reperage_Couleurs_Groupes  ->Check(false);
            Element->groupes  = false;
        }
        if (Button_Materiaux->GetValue()) Button_Materiaux->SetValue(false);    // Options exclusives !
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void OvniFrame::Toggle_Materiaux(wxCommandEvent& event) {
    OnButton_MateriauxToggle(event);
}

void OvniFrame::OnButton_MateriauxToggle(wxCommandEvent& event) {
    if(Element != nullptr) {
        if (Button_Materiaux->GetValue()) {
            Menu_Reperage_Couleurs_Groupes  ->Check(false);
            Menu_Reperage_Couleurs_Materiaux->Check(true) ;
            Menu_Reperage_Couleurs_Facettes ->Check(false);
            Element->groupes  = false;
            Element->materials= true;
        } else {
            Menu_Reperage_Couleurs_Materiaux->Check(false);
            Element->materials= false;
        }
        if (Button_Groupes->GetValue()) Button_Groupes->SetValue(false);        // Options exclusives !
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void OvniFrame::OnMenu_Reperage_Couleurs_FacettesSelected(wxCommandEvent& event) {
    if (Menu_Reperage_Couleurs_Facettes->IsChecked()) {
        Button_Groupes  ->SetValue(false);
        Button_Materiaux->SetValue(false);
        Menu_Reperage_Couleurs_Groupes  ->Check(false);
        Menu_Reperage_Couleurs_Materiaux->Check(false);
    }
    // Code de colorisation facette par facette à écrire, mais pas forcément ici ! A l'usage, ne semble pas beaucoup servir !
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::OnMenu_Reperage_Couleurs_GroupesSelected(wxCommandEvent& event) {
    if (Menu_Reperage_Couleurs_Groupes->IsChecked()) {
        Button_Groupes  ->SetValue(true) ;
        Button_Materiaux->SetValue(false);
        Menu_Reperage_Couleurs_Materiaux->Check(false);
        Menu_Reperage_Couleurs_Facettes ->Check(false);
        Element->groupes  = true ;
        Element->materials= false;
    } else {
        Button_Groupes->SetValue(false);
        Element->groupes  = false;
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::OnMenu_Reperage_Couleurs_MateriauxSelected(wxCommandEvent& event) {
    if (Menu_Reperage_Couleurs_Materiaux->IsChecked()) {
        Button_Materiaux->SetValue(true) ;
        Button_Groupes  ->SetValue(false);
        Menu_Reperage_Couleurs_Groupes ->Check(false);
        Menu_Reperage_Couleurs_Facettes->Check(false);
        Element->groupes  = false;
        Element->materials= true ;
    } else {
        Button_Materiaux->SetValue(false);
        Element->materials= false;
    }
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::OnMenu_ReOpenSelected(wxCommandEvent& event)
{
    New_file = false;                           // => le nom du fichier est déjà connu (via get_file)
    Element->Numero_base = 0;                   // Réinitialisation complète
    Button_Points->SetValue(false);             // Forcer la suppression d'affichage des points (sinon l'affichage devient vide si c'était actif !)
    OnButton_PointsToggle(event);               // Faute de mieux car on n'a pas ce souci avec l'affichage des arêtes !
    Ouvrir_Fichier();
}

void OvniFrame::OnMenu_ReOpen3dsSelected(wxCommandEvent& event)
{
// Reopen spécifique pour fichiers 3ds
    this->Element->test_decalage3ds = !this->Element->test_decalage3ds; // Inverser le test...
                                                                        // Ici, on ne marque pas de changement du fichier init (ini_file_modified inchangé)
    if (this->Element->MPrefs->IsShown()) {                             // Afficher le nouveau status de la case à cocher si le menu préférences est affiché
        this->Element->MPrefs->CheckBox_TestDecalage3DS->SetValue(this->Element->test_decalage3ds);
    }
    OnMenu_ReOpenSelected(event) ;
}

void OvniFrame::OnMenu_OpenSelected(wxCommandEvent& event)
{
//    printf("Entree OpenSelected\n");
    New_file = true;
//    FilterIndex=0;
    Element->Numero_base = 0;
    Ouvrir_Fichier();

//    printf("Sortie OpenSelected\n");
}

void OvniFrame::OnMenu_AddFileSelected(wxCommandEvent& event)
{
    New_file = true;
    Element->Numero_base = Element->Objetlist.size();   // Pour continuer la numérotation (sans repartir de 0 dans "value" d'un Object)
    Ouvrir_Fichier();
}

void OvniFrame::Ouvrir_Fichier()
{
    static int FilterIndex=8;
    static bool first_time=true;

    if (verbose) printf("Entree OvniFrame::Ouvrir_Fichier\n");

    wxString Nom_Fichier ;
    static wxString s_extdef=_T("bdd;*.3ds");   // Initialiser avec un choix multiple (2 1ers de la liste complète suffisent)
    static wxString FullPath;                   // Pour conserver un répertoire de Bdd par défaut = celui courant

    if (New_file) {
// Si "Tous les fichiers 3D" est en 1ère position, c'est lui qui est systématiquement sélectionné !
        wxString Selector = _T("SDM Oktal (*.bdd)|*.bdd|");
        Selector += _T("Autodesk 3DS (*.3ds)|*.3ds|");
        Selector += _T("Wavefront (*.obj)|*.obj|");
        Selector += _T("XML G3D (*.g3d)|*.g3d|");
        Selector += _T("Stanford ou Niratam Geo (*.ply)|*.ply|");
        Selector += _T("Object File Format (*.off)|*.off|");
        Selector += _T("Milkshape 3D text (*.m3d)|*.m3d|");
        Selector += _T("Stéréolithographique (*.stl)|*.stl|");
        Selector += _T("Fichiers dxf (*.dxf)|*.dxf|");
        Selector += _T("Tous les fichiers 3D |*.bdd;*.3ds;*.obj;*.g3d;*.ply;*.off;*.m3d;*.stl;*.dxf|");
        Selector += _T("Tous les fichiers (*.*)|*.*");

// FilterIndex dans wxFilesSelectorEx permet bien de récupérer l'indice de la sélection (encore que ?), mais pas d'imposer un choix en entrée !
// ce qui permettrait de garder le choix précédent

/*
        printf("FilterIndex avant : %d\n",FilterIndex);
        Nom_Fichier = wxFileSelectorEx(_T("Choix du fichier 3D"), wxEmptyString, wxEmptyString, &FilterIndex, Selector);
        printf("FilterIndex apres : %d\n",FilterIndex);
*/
        Nom_Fichier = wxFileSelector(_T("Choix du fichier 3D"), FullPath, wxEmptyString, s_extdef, Selector);
    } else {
        Nom_Fichier = Element->get_file();
    }

    Slider_x->Enable();
    Slider_y->Enable();
    Slider_z->Enable();
    Element->Slider_x = Slider_x;
    Element->Slider_y = Slider_y;
    Element->Slider_z = Slider_z;
    Element->MPosCRot = CentreRotation_Panel;
    Element->MScale_0 = ChangerEchelleBdd_Panel;
    Element->MChoice_O= ChoixAffichageObjets_Panel;
    Element->MCone    = Cone_Panel;
    Element->MCGroup  = CouleursGroupes_Panel;
    Element->MCube    = Cube_Panel;
    Element->MCylindre= Cylindre_Panel;
    Element->MDeplacer= DeplacerBdd_Panel;
    Element->MEllips  = Ellipsoide_Panel;
    Element->MFacet   = Facette_Panel;
    Element->MIcosa   = Icosaedre_Panel;
    Element->MManip   = Manipulations_Panel;
    Element->MPanel   = Modifications_Panel;
    Element->MPosObs  = PositionObsAzimutSite_Panel;
    Element->MPosLight= PositionSource_Panel;
    Element->MPrefs   = Preferences_Panel;
    Element->MProps   = Properties_Panel;
    Element->MRepFacet= ReperageFacette_Panel;
    Element->MRepGrp  = ReperageGroupe_Panel;
    Element->MRepMat  = ReperageMateriau_Panel;
    Element->MRepObj  = ReperageObjet_Panel;
    Element->MRepPoint= ReperagePoint_Panel;
    Element->MRotation= Rotation_Panel;
    Element->MScale   = Scale_Panel;
    Element->MSelect  = Selections_Panel;
    Element->MSelFac  = Selections_Manuelles_Facettes;
    Element->MSelObj  = Selections_Manuelles_Objets;
    Element->MSphere  = Sphere_Panel;
    Element->MTrans   = Translation_Panel;
    Element->MZoomSpec= ZoomSpecifique_Panel;
    Element->MHelp    = AideHtml_Panel;

    bool Erreur_lecture = true;

    if (first_time) {                                                   // Pour afficher un écran vide à fond bleu, la première fois
        Show(true);
        Element->m_gllist = 0;
        Element->m_loaded = true;                                       // Faire comme si un fichier avait été chargé !
//        Element->Refresh(true);
        Update();
        first_time = false;                                             // Les fois suivantes, on ne passera plus par là
    }

    wxBeginBusyCursor();                                                // Active le curseur Busy

    if (!Nom_Fichier.IsEmpty()) {

        Erreur_lecture = false;

        s_extdef = Nom_Fichier.AfterLast(_T('.'));
        s_extdef = s_extdef.Lower();                                    // Forcer l'extension en minuscules

        FullPath = Nom_Fichier.BeforeLast(wxFILE_SEP_PATH) + wxFILE_SEP_PATH; //wxFileName::GetPathSeparator();

        printf("Chargement du fichier en cours...\n");

        if (New_file) Element->set_file(Nom_Fichier);
        if (Element->Numero_base == 0) Element->set_firstFile(Nom_Fichier); // En cas de fusion, on conserve le nom du 1er fichier choisi

        Element->create_bdd();

        if (Element->type < 0) Erreur_lecture = true;

        if (!Erreur_lecture) {

            Element->OK_ToSave      = true;                         // Le fichier ouvert peut être enregistré (ré-enregistré)
            Element->OK_FichierCree = false;                        // mais ne l'a pas encoré été !

            this->Menu_SensDesNormales->Enable() ;
            this->Menu_ReOpen->Enable();                            // Activer le menu "Réouvrir"

    // Si c'est un fichier 3ds, proposer dans le menu de le réouvrir en changeant les test de décalage

            if (s_extdef == _T("3ds")) {                            // Création d'une entrée de menu spécifique de réouverture pour les fichiers 3ds
                if (!this->Menu_ReOpen3ds) {                        // à ne faire que si le menu n'est pas déjà présent
                    this->Menu_ReOpen3ds = new wxMenuItem(this->MenuFile, this->idReopenFile3ds, _T("Réouvrir 3ds"),
                                                          _T("Réouvre le fichier tel qu\'il est sur le disque mais en changeant le test de décalage"), wxITEM_NORMAL);
                    if (Element->theme_b) {
                        this->Menu_ReOpen3ds->SetTextColour(Element->New_Forg);
                        this->Menu_ReOpen3ds->SetBackgroundColour(Element->New_Back);
                    }
                    this->MenuFile->Insert(2,this->Menu_ReOpen3ds); // à insérer en position 2
                    this->Menu_ReOpen3ds->Enable(true);             // Activer le menu "Réouvrir 3ds", mais l'est d'office dans ce cas
                }
                if (this->Element->test_decalage3ds)                // Proposer de réouvrir avec le test inverse de ce qui est en cours
                    this->Menu_ReOpen3ds->SetItemLabel(_T("Réouvrir le fichier 3ds sans décalages"));
                else
                    this->Menu_ReOpen3ds->SetItemLabel(_T("Réouvrir le fichier 3ds avec décalages"));

            } else { // Si ce n'est pas un fichier 3ds supprimer l'entrée de menu spécifique de réouverture
                if (this->Menu_ReOpen3ds) { // mais seulement si elle existe !
                    this->MenuFile->Delete(this->Menu_ReOpen3ds);   // Suppression du menu (Remove, Destroy ou Delete ? semble donner la même chose)
                    this->Menu_ReOpen3ds = nullptr;                 // et RAZ du pointeur
                }
            }

    ///        this->Menu_AddFile->Enable();         // Activer le menu "AddFile"
            this->Menu_Enregistrer->Enable();                   // Activer le menu "Enregistrer"
            this->Menu_Enregistrer_Sous->Enable();              // Activer le menu "Enregistrer sous..."
            Element->MPrefs->CheckBox_CreerBackup->Enable();    // Activer la case à cocher de création d'un fichier .bak
            if (Element->CreerBackup)
                Element->MPrefs->CheckBox_SupprBackup->Enable();// Activer la case à cocher de suppression du fichier .bak

            if (Element->Numero_base != 0) {
                Element->bdd_modifiee = true;                   // On a fait une fusion => marquer la bdd comme modifiée
                Menu_Enregistrer->Enable(false);                // On désactive "Enregistrer". Seul le choix "Enregistrer sous..." restera actif.
            }

            Element->Refresh(); // Le Refresh n'a de sens que si un fichier a été ouvert

        }
    }

    if (Erreur_lecture) { // ou Nom_Fichier.IsEmpty())

        //wxMessageBox(_T("Fichier vide !"));
        if (Element->Objetlist.size() == 0) {               // On a appuyé sur Annuler mais aucun objet n'est déjà présent
            Element->m_gllist = 0;
            Element->m_loaded = true;                           // Faire comme si un fichier avait été chargé !
            Update();                                           // Avec wxWidgets 3.1.4 remplace les 2 lignes ci-dessous ! Marche aussi avec 3.1.3
//            wxPaintEvent cmdPaint;                              // Pour déclencher un évênement OnPaint qui affichera alors un fond bleu uniforme, vide d'objet
//            Element->BddInter::Forcer_OnPaint(cmdPaint);        // Note : inutile avec wxWidgets 2.8, mais pas avec 3.1 et +
            this->Menu_Enregistrer_Sous->Enable();              // Activer tout de même le menu "Enregistrer sous..."
        } else {                                            // Objets(s) déjà affiché(s) => ne rien faire de plus...
            Element->SetFocus();                                // Donner le focus à la fenètre OpenGL ici car on n'est pas passé dans drawOpenGL
        }
    }

    this->Menu_AddFile->Enable();                         // Activer le menu "AddFile"

    wxSpinEvent cmd_spin;
    Preferences_Panel->SpinCtrl_PasSvg->SetValue(Element->svg_time);    // Initialiser la valeur du spinbutton
    Preferences_Panel->OnSpinCtrl_PasSvgChange_pub(cmd_spin);           // Simuler un clic pour déclencher le timer de sauvegardes automatiques

    wxEndBusyCursor();                                                  // Désactive le curseur Busy

    if (verbose) printf("Sortie OvniFrame::Ouvrir_Fichier\n");
}

void OvniFrame::OnMenu_PreferencesSelected(wxCommandEvent& event)
{
//    Preferences_Panel->ShowModal(); // Bloque l'interactivité d'autres options, boutons, menus, ...
    float val ;
    int   ival;
    bool  chkB;
    int   m_gllist_save;

    bool svg = Element->ini_file_modified;  // Sauvegarde, car il est modifié (via un des (ou les ?) SetValue et/ou SetLabel ?)

    if (Element->verbose) printf("Entree OnMenu_Preferences : m_gllist=%d\n",Element->m_gllist);

    Preferences_Panel->SpinCtrlDouble_axes->SetValue(Element->len_axe);
    Preferences_Panel->SpinCtrlDouble_norm->SetValue(Element->len_normales);
    Preferences_Panel->SpinCtrlDouble_src ->SetValue(Element->ray_sun);

    Preferences_Panel->CheckBox_AntialiasingSoft->SetValue(Element->antialiasing_soft);

    Preferences_Panel->CheckBox_1SeulObjet3D->SetValue(Element->Forcer_1_Seul_Objet);
    Preferences_Panel->CheckBox_LectureOptimisee->SetValue(Element->lect_obj_opt);
    if (Element->Forcer_1_Seul_Objet)
        Preferences_Panel->CheckBox_LectureOptimisee->Disable();
    else
        Preferences_Panel->CheckBox_LectureOptimisee->Enable();

    Preferences_Panel->CheckBox_TestDecalage3DS->SetValue(Element->test_decalage3ds);

    Preferences_Panel->CheckBox_CalculNormales ->SetValue(Element->CalculNormalesLectureBdd);

    chkB = Element->test_seuil_gouraud;
    Preferences_Panel->CheckBox_Seuillage->SetValue(chkB);
    Preferences_Panel->SpinCtrlDouble_SeuilGouraud ->SetValue(Element->angle_Gouraud);
    Preferences_Panel->SpinCtrlDouble_SeuilGouraud2->SetValue(Element->fmult_Gouraud);
    if (chkB) {
        Preferences_Panel->StaticText_Gouraud            ->Enable();
        Preferences_Panel->StaticText_Gouraud2           ->Enable();
        Preferences_Panel->SpinCtrlDouble_SeuilGouraud   ->Enable();
        Preferences_Panel->SpinCtrlDouble_SeuilGouraud2  ->Enable();
        Preferences_Panel->CheckBox_RecNormales_Seuillees->Enable();
    } else {
        Preferences_Panel->StaticText_Gouraud            ->Disable();
        Preferences_Panel->StaticText_Gouraud2           ->Disable();
        Preferences_Panel->SpinCtrlDouble_SeuilGouraud   ->Disable();
        Preferences_Panel->SpinCtrlDouble_SeuilGouraud2  ->Disable();
        Preferences_Panel->CheckBox_RecNormales_Seuillees->Disable();
    }
    Preferences_Panel->CheckBox_RecNormales_Seuillees->SetValue(Element->Enr_Normales_Seuillees);
    Preferences_Panel->CheckBox_NotFlat->SetValue(Element->NotFlat);

    ival = Element->svg_time;
    Preferences_Panel->SpinCtrl_PasSvg->SetValue(ival);

    if (ival > 0 && Element->bdd_modifiee) {
        Timer_Save.Start(ival*60000,false); // Ne démarre le timer que si le pas de sauvegarde est > 0 et si la Bdd a été modifiée
    } else {
        Timer_Save.Stop();                  // Sinon, arrêt du timer
    }

    Preferences_Panel->RadioBox_Triangulation->SetSelection(Element->methode_Triangulation);

    Preferences_Panel->RadioBox_Trackball->SetSelection(Element->m_gldata.mode_Trackball);

    Preferences_Panel->CheckBox_DisplayFps->SetValue(Element->viewFps);

    Preferences_Panel->CheckBox_CreerBackup->SetValue(Element->CreerBackup);
    Preferences_Panel->CheckBox_SupprBackup->SetValue(Element->SupprBackup);
    Preferences_Panel->CheckBox_SupprBackup->Enable(Element->CreerBackup) ;     // Conditionner le mode Enable au contenu de CreerBack

//    Preferences_Panel->TextCtrl_WorkDir->SetStyle(-1,-1,wxTextAttr(wxTE_RIGHT));
    Preferences_Panel->TextCtrl_WorkDir->SetLabel(Element->wxWorkDir);
//    Preferences_Panel->TextCtrl_WorkDir->SetStyle(-1,-1,wxTextAttr(wxTE_RIGHT)); // A revoir : Ne marche pas. L'idée est de cadrer à droite pour voir la fin du nom de répertoire
//    wxTextAttr* style=wxTE_RIGHT;

    Preferences_Panel->SpinCtrl_Threads->SetMax  (Element->nb_max_threads);
    Preferences_Panel->SpinCtrl_Threads->SetValue(Element->nb_threads);

    Element->ini_file_modified = svg;   // Restitution de la valeur qu'il y avait en entrée

    Preferences_Panel->Show();
    event.Skip();                       //N'a pas l'air utile

    if (Element->verbose) printf("Sortie OnMenu_Preferences : m_gllist=%d\n",Element->m_gllist);
}

void OvniFrame::OnSize(wxSizeEvent& event)
{
// Ne sert pas !
    printf("Hello de OvniFrame::OnSize\n");
    Element->Resize();
}

void OvniFrame::OnSlider_xCmdScroll(wxScrollEvent& event)
{
        float val = event.GetInt();
        Element->m_gldata.rotx = val;
        Element->m_gldata.roty = Slider_y->GetValue();  // C'est rotx qui a changé mais relire tout de même les autres sliders : permet de
        Element->m_gldata.rotz = Slider_z->GetValue();  // rafaichir des valeurs qui peuvent avoir été modifiées de 180° dans certains calculs
        if (PositionObsAzimutSite_Panel->IsShown()) SetAngles();
        if (ZoomSpecifique_Panel->IsShown())        SetAngles();
        Element->CalculMatriceRot();
//        printf("%d %d\n",event.GetInt(),Slider_x->GetValue()) ;
        Element->Refresh();
}

void OvniFrame::OnSlider_yCmdScroll(wxScrollEvent& event)
{
        float val = event.GetInt();
        Element->m_gldata.roty = val;
        Element->m_gldata.rotx = Slider_x->GetValue();  // Idem
        Element->m_gldata.rotz = Slider_z->GetValue();
        if (PositionObsAzimutSite_Panel->IsShown()) SetAngles();
        if (ZoomSpecifique_Panel->IsShown())        SetAngles();
        Element->CalculMatriceRot();
        Element->Refresh();
}

void OvniFrame::OnSlider_zCmdScroll(wxScrollEvent& event)
{
        float val = event.GetInt();
        Element->m_gldata.rotz = val;
        Element->m_gldata.rotx = Slider_x->GetValue();  // Idem
        Element->m_gldata.roty = Slider_y->GetValue();
        if (PositionObsAzimutSite_Panel->IsShown()) SetAngles();
        if (ZoomSpecifique_Panel->IsShown())        SetAngles();
        Element->CalculMatriceRot();
        Element->Refresh();
}

void OvniFrame::OnMenu_EnregistrerSelected(wxCommandEvent& event)
{
// Enregistre au format SDM bdd avec le nom d'origine et dans le répertoire d'origine
    bool test=true;
    wxString wxNomFichier = Element->get_firstFile().BeforeLast('.') + _T(".bdd");
    if (Element->CreerBackup) {
        if (wxFileExists(wxNomFichier)) {           // On n'ajoute le .bak que si le fichier .bdd existe déjà (fichier original de type SDM donc)
            wxString wxNomFichier_bak = wxNomFichier + _T(".bak") ;
            if (wxFileExists(wxNomFichier_bak)) test = wxRemoveFile(wxNomFichier_bak); // Si le fichier .bak existe déjà, le supprimer
            if (test) test = wxRenameFile(wxNomFichier, wxNomFichier_bak);
            if (test)
                printf("Sauvegarde du fichier original en ");
            else
                printf("Probleme a la sauvegarde de ");

            wxNomFichier_bak = wxFileNameFromPath(wxNomFichier_bak);    // Ne garder que le nom, enlever le Path
            printf("%s\n",(const char*)wxNomFichier_bak.mb_str());
        }
    }
    wxFileOutputStream output_stream(wxNomFichier);
    if (!output_stream.IsOk()) {
//        //wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
        printf("ERREUR d'ouverture du fichier %s !\n",(const char*)wxNomFichier.mb_str());
        Element->OK_FichierCree = false;
        return;
    }
    if (Element != nullptr) {
        Element->SaveBDD(wxNomFichier);
        Element->OK_FichierCree = true; // Cet indicateur n'a de sens que pour le menu "Enregistrer", pas "Enregistrer sous..."
    }
}

void OvniFrame::OnMenu_Enregistrer_Sous(wxCommandEvent& event)
{
// Enregistre au format SDM bdd en proposant le nom d'origine comme nom par défaut
    bool notOK = false;
    if (Element == nullptr)             notOK = true;
    if (Element->Objetlist.size() == 0) notOK = true;
    if (notOK) {   // Il n'y a rien à enregistrer
        Element->DisplayMessage(_T("Aucun objet à enregistrer !\n"),true);
        return;
    }
    // Préparer l'enregistrement sous...
    wxString Nom_svg = Element->get_firstFile().BeforeLast('.');
    wxString Type_fichiers = _T("SDM Oktal (*.bdd)|*.bdd|");                        // index = 0
    Type_fichiers +=         _T("Wavefront (*.obj)|*.obj|");                        // index = 1
    Type_fichiers +=         _T("XML G3D   (*.g3d)|*.g3d|");                        // index = 2
    Type_fichiers +=         _T("Object File Format (*.off)|*.off|");               // index = 3
    Type_fichiers +=         _T("Stéréolithographique Ascii   (*.stl)|*.stl|");     // index = 4
    Type_fichiers +=         _T("Stéréolithographique Binaire (*.stl)|*.stl");      // index = 5
    wxFileDialog saveFileDialog(this, _T("Enregistrer sous..."), _T(""), Nom_svg, Type_fichiers, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return; // L'utilisateur a changé d'idée finalement ...

    wxString wxNomFichier = saveFileDialog.GetPath();   // Récupérer le nom complet

//    printf("Index %d\n",saveFileDialog.GetFilterIndex());

    if(saveFileDialog.GetFilterIndex() >= 4) {          // Format stl => facettes triangulaires. Vérifier que c'est le cas avant d'ouvrir le fichier !
        Object * objet_courant;
        for (unsigned int o=0; o<Element->Objetlist.size(); o++) {
            objet_courant = &(Element->Objetlist[o]);
            if (objet_courant->deleted) continue ;

            for (unsigned int j=0; j<objet_courant->Facelist.size(); j++) {
                if(objet_courant->Facelist[j].deleted) continue;
                if(objet_courant->Facelist[j].getNb_Sommets_F() != 3) {
                    wxString wxMessage=_T("La base n'est pas triangulée. Stockage impossible en format .stl");
                    Element->DisplayMessage(wxMessage,true);
                    return;
                }
            }
        }
    }

    wxFileOutputStream output_stream(wxNomFichier);
    if (!output_stream.IsOk()) {
        printf("ERREUR d'ouverture du fichier %s !\n",(const char*)wxNomFichier.mb_str());
        return;
    }

    if (Element != nullptr) {
        Element->SaveTo(wxNomFichier,saveFileDialog.GetFilterIndex());
        Element->set_file(wxNomFichier);    // Mettre à jour le nom du fichier (au cas où il aurait changé, par exemple si fusion)
    }
}

void OvniFrame::OnOutils_Selec_DeplSelected(wxCommandEvent& event)
{
    if (Element != nullptr) {
        toggle_outils = !toggle_outils;
        if (toggle_outils) {
            Selections_Panel->RadioButton_Selection_Facettes->SetValue(true) ;
            Selections_Panel->RadioButton_Grp->SetValue(true)    ;          // 0 = Groupe
//            Selections_Panel->TextCtrl_NumerosUtilises->SetLabel(_T(""));   // Provisoire
//            Selections_Panel->RadioButton_TypeSelection_Both->SetValue(true) ;
//            Element->TypeSelection = 0;

            Selections_Panel->Show();
            Selections_Panel->Refresh();
        } else {
            Selections_Panel->Hide();
            Selections_Panel->Refresh();
        }
    } else {
        Button_Outils->SetValue(false); // Forcer à rester en position haute
        toggle_outils = false;
    }
}

void OvniFrame::OnOutils_ModificationsSelected(wxCommandEvent& event)
{
    str.Printf(_T("%7.1e"),Element->tolerance);
    Modifications_Panel->TextCtrl_Tolerance->SetValue(str);
    Modifications_Panel->Show();
    Modifications_Panel->Refresh();
}

void OvniFrame::Toggle_Sliders(wxCommandEvent& event)
{
    OnButton_SlidersToggle(event);
}

void OvniFrame::OnButton_SlidersToggle(wxCommandEvent& event)
{
    // Pour afficher ou masquer l'espace réservé aux Sliders

    int New_y_size;

    if (verbose) printf("\nEntree OvniFrame::OnButton_SlidersToggle\n");
    const wxSize ClientSize_Pa = Panel1->GetSize();

    const wxSize ClientSize_Sl = Panel_Sliders->GetSize();

    const wxSize ClientSize_GL = GLCanvas->GetSize();

    if (Panel_Sliders->IsShown()) {
        Panel_Sliders->Hide();
        New_y_size = ClientSize_GL.y + ClientSize_Sl.y;     // Récupérer la place occupée par les Sliders pour l'ajouter à la taille OpenGL
    } else {
        Panel_Sliders->Show();
        New_y_size = ClientSize_GL.y ;
    }
    ResizeOpenGL(ClientSize_GL.x, New_y_size);              // Redimensionner la fenètre OpenGL
    New_y_size += ClientSize_Pa.y;                          // Calculer la taille de la BoxSizer1 (OpenGL + Sliders + Panel 1)

//    BoxSizer1->SetSizeHints(this);
    BoxSizer1->SetDimension(0,0,ClientSize_GL.x, New_y_size);
    Element->Resize();
    Element->Refresh();

    if (verbose) printf("Sortie OvniFrame::OnButton_SlidersToggle\n\n");
}

void OvniFrame::Toggle_Gouraud(wxCommandEvent& event)
{
    OnButton_GouraudToggle(event);
}

void OvniFrame::OnButton_GouraudToggle(wxCommandEvent& event)
{
    unsigned int o;
    bool flat;

    if(Element != nullptr) {
        Element->smooth  = !Element->smooth;
        // On vérifie si tous les objets ne sont pas plats
        flat = true;
        for (o=0; o<Element->Objetlist.size(); o++) if (!Element->Objetlist[o].flat) flat = false;  // Au moins 1 objet n'est pas plat !
        if ((Element->smooth) && (flat)) {
            wxString Msg = _T("Lissage de Gouraud impossible.\nTous les objets sont plats : pas de normales aux sommets !\n");
            Msg +=         _T("Recalculer les normales ou relancer la lecture en cochant la case adéquate du dialogue \"Fichier/Préférences\"");
            Element->DisplayMessage(Msg,true);
            Button_Gouraud->SetValue(false);    // Simuler un nouveau clic sur le bouton pour le remettre à l'état initial
            Element->smooth = false;
            return; // On ne fait rien de plus !
        }
        Element->m_gldata.initialized=false;
        Element->m_gllist = 0;
        Element->Refresh();
//        Element->Toggle_Gouraud();
//        Button_Gouraud->SetValue(Element->smooth);  // Forcer l'état du bouton à être conforme à smooth
//        Refresh();                                  // Pour l'interface, surtout si smooth n'était pas conforme à l'état du bouton.
    }
}

void OvniFrame::OnButton_ModifsToggle(wxCommandEvent& event)
{
    if (Element != nullptr) {
        str.Printf(_T("%7.1e"),Element->tolerance);
        Modifications_Panel->TextCtrl_Tolerance->SetValue(str);
        if (toggle_modif == true) {
            toggle_modif  = false;
            Modifications_Panel->Hide();
            Modifications_Panel->Refresh();
        } else {
            toggle_modif  = true;
            Modifications_Panel->Show();
            Modifications_Panel->Refresh();
        }
    } else {
        Button_Modifs->SetValue(false); // Forcer à rester en position haute
        toggle_modif = false;
    }
}

void OvniFrame::OnMenu_SensDesNormalesSelected(wxCommandEvent& event)
{
    if (Element->show_CW_CCW == false) {
        Menu_SensDesNormales->Check(true);
        Element->show_CW_CCW = true;
    } else {
        Menu_SensDesNormales->Check(false);
        Element->show_CW_CCW = false;
    }
    Element->m_gllist = 0;
    Element->m_gldata.initialized=false;
    Element->Refresh();
}

void OvniFrame::OnModifsXYZ()
{
    // fonctions à appeler systématiquement en sortie de OnMenu_XminusX, YminusY, ...
        Element->m_gllist = 0;
        Element->searchMin_Max();
        Element->Refresh();
}

void OvniFrame::OnMenu_XminusX(wxCommandEvent& event)
{
    // Changer X en -X
    if (Element != nullptr) {
        Element->InverseX();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_YminusY(wxCommandEvent& event)
{
    // Changer Y en -Y
    if (Element != nullptr) {
        Element->InverseY();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_ZminusZ(wxCommandEvent& event)
{
    // Changer Z en -Z
    if (Element != nullptr) {
        Element->InverseZ();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_XtoY(wxCommandEvent& event)
{
    // Permuter les X et les Y
    if (Element != nullptr) {
        Element->XtoY();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_XtoZ(wxCommandEvent& event)
{
    // Permuter les X et les Z
    if (Element != nullptr) {
        Element->XtoZ();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_YtoZ(wxCommandEvent& event)
{
    // Permuter les Y et les Z
    if (Element != nullptr) {
        Element->YtoZ();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_XtoYtoZ(wxCommandEvent& event)
{
    // Permutation circulaire sur les X, Y et Z
    if (Element != nullptr) {
        Element->XtoYtoZ();
        OnModifsXYZ();
    }
}

void OvniFrame::OnMenu_Inverse_All_Selected_NormalesSelected(wxCommandEvent& event)
{
    // Inverser les normales sélectionnées
    if (Element != nullptr) {
        Element->Inverse_Selected_Normales();
//        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void OvniFrame::OnPopup_ForcerFacettesPlanesSelected(wxCommandEvent& event)
{
    // Forcer le mode facettes planes sur les facettes sélectionnées
    if (Element != nullptr) {
        Element->Flat_Selected_Facettes();
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void OvniFrame::OnPopup_ForcerFacettesNonPlanesSelected(wxCommandEvent& event)
{
    // Forcer le mode facettes non planes sur les facettes sélectionnées
    if (Element != nullptr) {
        Element->NotFlat_Selected_Facettes();
        Element->m_gllist = 0;
        Element->Refresh();
    }
}

void OvniFrame::OnPopup_Reverse_ParcoursSelected(wxCommandEvent& event)
{
    // Inverser le sens de parcours des facettes sélectionnées
    if (Element != nullptr) {
        Element->Inverser_Parcours_Selected();
        if (Element->show_CW_CCW) Element->m_gllist = 0;                  // Utile si Sens des normales est activé. Sans effet visuel sinon
        Element->Refresh();
    }
}

void OvniFrame::OnPopup_Raz_Select_FSelected(wxCommandEvent& event)
{   // Inverser le flag Raz_Selection_F (<=> Touche clavier 'Z'
    if (Element != nullptr) {
        Element->Raz_Selection_F   = !(Element->Raz_Selection_F);
        Element->ini_file_modified = true;                          // Ovni.ini à modifier
    }
}

void OvniFrame::OnMenu_Inverser_Toutes_les_NormalesSelected(wxCommandEvent& event)
{
    // Inverser toutes les normales
    if (Element != nullptr) {
        Element->Inverser_Toutes_les_Normales();
        Element->m_gllist = 0;
        Element->Refresh();
    }
}
//Zoom+
void OvniFrame::OnButton_ZoomPlusClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.zoom -= Element->m_gldata.zoom_step;//1.0;
        if (ZoomSpecifique_Panel->IsShown()) {
           str.Printf(_T("%4.3f"),Element->m_gldata.zoom);
           ZoomSpecifique_Panel->TextCtrl_FoV->SetValue(str);
        }
        Element->ResetProjectionMode();
        Element->Refresh();
    }
//    event.Skip();         // N'a pas l'air utile ! N'y est pas dans ZoomMoins !
}
//Zoom-
void OvniFrame::OnButton_ZoomMoinsClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.zoom += Element->m_gldata.zoom_step;//1.0;
        if (ZoomSpecifique_Panel->IsShown()) {
           str.Printf(_T("%4.3f"),Element->m_gldata.zoom);
           ZoomSpecifique_Panel->TextCtrl_FoV->SetValue(str);
        }
        Element->ResetProjectionMode();
        Element->Refresh();
    }
}
//gauche
void OvniFrame::OnButton_GaucheClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.posx -= Element->m_gldata.depl_xy;
        Element->ResetProjectionMode();
        Element->Refresh();
    }
}
//droit
void OvniFrame::OnButton_DroiteClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.posx += Element->m_gldata.depl_xy;
        Element->ResetProjectionMode();
        Element->Refresh();
    }
}
//haut
void OvniFrame::OnButton_HautClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.posy += Element->m_gldata.depl_xy;
        Element->ResetProjectionMode();
        Element->Refresh();
    }
}
//bas
void OvniFrame::OnButton_BasClick(wxCommandEvent& event)
{
    if (Element != nullptr) {
        Element->m_gldata.posy -= Element->m_gldata.depl_xy;
        Element->ResetProjectionMode();
        Element->Refresh();
    }
}

void OvniFrame::OnButton_OutilsToggle(wxCommandEvent& event)
{
    OnOutils_Selec_DeplSelected(event);
}

void OvniFrame::Redisplay_Proprietes(wxCommandEvent& event)
{
    OnMenu_ProprietesSelected(event);
}

void OvniFrame::OnMenu_ProprietesSelected(wxCommandEvent& event)
{
    // Affichages obtenus via le menu Propriétés de la Bdd

    wxString String_local;
    unsigned int taille;

    if (Element->Objetlist.size() == 0) {   // Les initialisations ont dans ce cas été court-circuitées, donc le faire ici.
        Element->nb_objets_reels = Element->nb_facettes = Element->nb_aretes = Element->nb_sommets = 0;
        Element->nb_3points = Element->nb_4points = Element->nb_sommets_max = Element->numero_objet_max = Element->numero_facette_max = 0;
    }
// Nom du/des fichier(s)
    Properties_Panel->StaticTextNomBdd->SetLabel(Element->wxNomsFichiers);

// Nombre d'objets
    String_local.Printf(_T("%d"),Element->nb_objets_reels);
    Properties_Panel->nb_objets->SetLabel(String_local);

// Nombre de facettes
    String_local.Printf(_T("%d"),Element->nb_facettes);
    Properties_Panel->nb_facettes->SetLabel(String_local);

// Nombre d'arêtes
    String_local.Printf(_T("%d"),Element->nb_aretes);
    Properties_Panel->nb_aretes->SetLabel(String_local);

// Nombre de groupes
    taille = Element->listeGroupes.size();
    String_local.Printf(_T("%d"),taille);
    Properties_Panel->nb_groupes->SetLabel(String_local);

// Nombre de matériaux
    taille = Element->listeMateriaux.size();
    String_local.Printf(_T("%d"),taille);
    Properties_Panel->nb_materiaux->SetLabel(String_local);

// Nombre de sommets
    String_local.Printf(_T("%d"),Element->nb_sommets);
    Properties_Panel->nb_points->SetLabel(String_local);

// Nombre de facettes à 3 sommets
    String_local.Printf(_T("%d"),Element->nb_3points);
    Properties_Panel->nb_3Points->SetLabel(String_local);

// Nombre de facettes à 4sommets ou plus
    String_local.Printf(_T("%d"),Element->nb_4points);
    Properties_Panel->nb_4Points->SetLabel(String_local);

// Nombre max de sommets dans les facettes
    String_local.Printf(_T("%d"),Element->nb_sommets_max);
    Properties_Panel->nb_maxPoints->SetLabel(String_local);

// Numéro de l'objet ou se trouve cette facette avec le nombre max de sommets
    String_local.Printf(_T("%d"),Element->numero_objet_max);
    Properties_Panel->numero_objet->SetLabel(String_local);

// Numéro de la facette dans cet objet avec le nombre max de sommets
    String_local.Printf(_T("%d"),Element->numero_facette_max);
    Properties_Panel->numero_facette->SetLabel(String_local);

// Dimensions de la boîte englobante
    float x_min,x_max,y_min,y_max,z_min,z_max;
    x_min = Element->x_min; x_max = Element->x_max;
    y_min = Element->y_min; y_max = Element->y_max;
    z_min = Element->z_min; z_max = Element->z_max;
    String_local.Printf(_T("X : %8.4f\t%8.4f\nY : %8.4f\t%8.4f\nZ : %8.4f\t%8.4f\n----------\n%5.3f*%5.3f*%5.3f"),
                                                x_min,x_max,
                                                y_min,y_max,
                                                z_min,z_max,
                                                x_max-x_min,
                                                y_max-y_min,
                                                z_max-z_min);

    Properties_Panel->Texte_Box->SetLabel(String_local);   // L'attribut wxALIGN_CENTRE est perdu !
//    Properties_Panel->Texte_Box->SetLabel(wxString(buffer[0]));
//    Properties_Panel->Texte_Box->CenterOnParent();
    Properties_Panel->Show();
}

void OvniFrame::SetAngles()
{
    Slider_x->SetValue(lround(Element->m_gldata.rotx));
    Slider_y->SetValue(lround(Element->m_gldata.roty));
    Slider_z->SetValue(lround(Element->m_gldata.rotz));

    if (PositionObsAzimutSite_Panel->IsShown()) {
        // ATTENTION : si roty != 0, les valeurs Site Azimut sont erronnées
        int val = Element->convert_rotx_LSI();
        PositionObsAzimutSite_Panel->SpinCtrl_LSI->SetValue(val);

        val = Element->convert_rotz_LAZ();
        PositionObsAzimutSite_Panel->SpinCtrl_LAZ->SetValue(val);
        if (lround(Element->m_gldata.roty) == 0) PositionObsAzimutSite_Panel->StaticText_Warning->Hide();
        else                                     PositionObsAzimutSite_Panel->StaticText_Warning->Show();
    }
    if (ZoomSpecifique_Panel->IsShown()) {
        // ATTENTION : si roty != 0, les valeurs Site Azimut sont erronnées
        int val = Element->convert_rotx_LSI();
        ZoomSpecifique_Panel->SpinCtrl_LSI->SetValue(val);

        val = Element->convert_rotz_LAZ();
        ZoomSpecifique_Panel->SpinCtrl_LAZ->SetValue(val);
        if (lround(Element->m_gldata.roty) == 0) ZoomSpecifique_Panel->StaticText_Warning->Hide();
        else                                     ZoomSpecifique_Panel->StaticText_Warning->Show();
    }
    Element->CalculMatriceRot();
    Element->Refresh();
}

void OvniFrame::OnMenu_OrigineSelected(wxCommandEvent& event)
{
    Element->m_gldata.rotx = 0.;
    Element->m_gldata.roty = 0.;
    Element->m_gldata.rotz = 0.;
    SetAngles();
}

void OvniFrame::OnMenu_VueDeFaceSelected(wxCommandEvent& event)
{
    // Vue de face pour un objet orienté comme dans CRIRA
    Element->m_gldata.rotx = 270.;
    Element->m_gldata.roty =   0.;
    Element->m_gldata.rotz =   0.;
    SetAngles();
}

void OvniFrame::OnMenu_VueDeProfilSelected(wxCommandEvent& event)
{
    // Vue de profil (gauche) pour un objet orienté comme dans CRIRA
    Element->m_gldata.rotx = 270.;
    Element->m_gldata.roty =   0.;
    Element->m_gldata.rotz = 270.;
    SetAngles();
}

void OvniFrame::OnMenu_VueDeDessusSelected(wxCommandEvent& event)
{
    // Vue de dessus pour un objet orienté comme dans CRIRA
    Element->m_gldata.rotx =   0.;
    Element->m_gldata.roty =   0.;
    Element->m_gldata.rotz = 270.;
    SetAngles();
}

void OvniFrame::OnMenu_PositionObservateurSelected(wxCommandEvent& event)
{
    Element->m_gldata.roty = 0 ;            // en toute rigueur, forcer cette valeur
    PositionObsAzimutSite_Panel->Show();    // Afficher d'abord, sinon SetAngles ne modifie pas les valeurs Site Azimut !
    SetAngles();
}

void OvniFrame::OnMenu_PositionSourceSelected(wxCommandEvent& event)
{
    PositionSource_Panel->SpinCtrlDouble_PosX->SetValue(Element->Light0Position[0]);
    PositionSource_Panel->SpinCtrlDouble_PosY->SetValue(Element->Light0Position[1]);
    PositionSource_Panel->SpinCtrlDouble_PosZ->SetValue(Element->Light0Position[2]);
    PositionSource_Panel->SpinCtrlDouble_PosW->SetValue(Element->Light0Position[3]);
    PositionSource_Panel->Show();
}

void OvniFrame::OnMenu_CentreRotationSelected(wxCommandEvent& event)
{
    if (Element->centrageRotAuto) {
        Element->centreRot = Element->centre_auto ;
        Element->Refresh();
    }
    CentreRotation_Panel->SpinCtrlDouble_X->SetValue(Element->centreRot[0]);
    CentreRotation_Panel->SpinCtrlDouble_Y->SetValue(Element->centreRot[1]);
    CentreRotation_Panel->SpinCtrlDouble_Z->SetValue(Element->centreRot[2]);
    CentreRotation_Panel->Show();
}

void OvniFrame::OnMenu_ReperageGroupeSelected(wxCommandEvent& event)
{
    ReperageGroupe_Panel->SpinButton1->SetValue(0);
    ReperageGroupe_Panel->SpinButton1->SetMax(Element->listeGroupes.size());
    str.Printf(_T("Aucun"));
    ReperageGroupe_Panel->TextCtrl1->SetValue(str);
    ReperageGroupe_Panel->Show();
}

void OvniFrame::OnMenu_ReperageMateriauSelected(wxCommandEvent& event)
{
    ReperageMateriau_Panel->SpinButton1->SetValue(0);
    ReperageMateriau_Panel->SpinButton1->SetMax(Element->listeMateriaux.size());
    str.Printf(_T("Aucun"));
    ReperageMateriau_Panel->TextCtrl1->SetValue(str);
    ReperageMateriau_Panel->Show();
}

void OvniFrame::ReperageObjet_activer()
{
    wxCommandEvent cmd_event;
    OnMenu_ReperageObjetSelected(cmd_event);
}

void OvniFrame::OnMenu_ReperageObjetSelected(wxCommandEvent& event)
{
    ReperageObjet_Panel->SpinButton_indice-> SetValue(-1);                          // SpinButton sur -1
    ReperageObjet_Panel->SpinButton_indice-> SetMax(Element->Objetlist.size());     // pour pouvoir boucler
    str.Printf(_T("Non sélectionné"));                                              // Afficher ce texte
    ReperageObjet_Panel->TextCtrl_NumObjet-> SetValue(str);
    str.Printf(_T(""));                                                             // Vider la case avec le nom de l'objet
    ReperageObjet_Panel->TextCtrl_NomObjet-> SetValue(str);
    ReperageObjet_Panel->TextCtrl_indice->   SetValue(str);
    ReperageObjet_Panel->CheckBox_masquer->  SetValue(false);                       // Case à cocher non cochée
    ReperageObjet_Panel->CheckBox_masquer->  Disable();                             // et désactivée
    ReperageObjet_Panel->CheckBox_supprimer->SetValue(false);                       // Case à cocher non cochée
    ReperageObjet_Panel->CheckBox_supprimer->Disable();                             // et désactivée
    ReperageObjet_Panel->Button_InverserNormales->Disable();
    ReperageObjet_Panel->Show();
}

void OvniFrame::OnOutils_choix_afficherSelected(wxCommandEvent& event)
{
    unsigned int i,n,indice_ListBox;
    wxString str_loc;
    n = ChoixAffichageObjets_Panel->CheckListBox1->GetCount();// Récupère le nombre d'items actuels
    for (i=0; i<n; i++)
        ChoixAffichageObjets_Panel->CheckListBox1->Delete(0); // Supprime ces "anciens" items

    indice_ListBox = 0; // Pour découpler l'indice d'un objet de celui de la CheckListBox (en cas d'objets supprimés)
    for (i=0; i<Element->Objetlist.size(); i++) {
        if (!Element->Objetlist[i].deleted) {
            str_loc = wxString(Element->Objetlist[i].GetName(), wxConvUTF8);    // si Bdd en utf8
//            str_loc = wxString(Element->Objetlist[i].GetName());
            ChoixAffichageObjets_Panel->CheckListBox1->Append(str_loc);    // Recrée tous les items (des objets non supprimés)
            ChoixAffichageObjets_Panel->CheckListBox1->Check(indice_ListBox,Element->Objetlist[i].afficher);            // Coche ceux qui sont à afficher
            indice_ListBox++;
 //           if (Element->Objetlist[i].deleted) ChoixAffichageObjets_Panel->CheckListBox1->Check(i,false);
        }
    }
    ChoixAffichageObjets_Panel->Show();
}

void OvniFrame::OnOutils_ReafficherSelected(wxCommandEvent& event)
{
    unsigned int i,j;
    for (i=0; i<Element->Objetlist.size(); i++) {
        Element->Objetlist[i].afficher = true;
        for (j=0; j<Element->Objetlist[i].Facelist.size(); j++) Element->Objetlist[i].Facelist[j].afficher = true;
    }
    Element->Elements_Masques = false;
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::OnOutils_Supprimer_MasquesSelected(wxCommandEvent& event)
{
    // Pour marquer les objets masqués comme "à supprimer" (deleted)
    // ATTENTION : les objets supprimés ne sont pas encore complètement gérés => continuent d'apparaître dans certains menus / dialogues
    unsigned int i,j;
    for (i=0; i<Element->Objetlist.size(); i++) {
        Element->Objetlist[i].deleted = !Element->Objetlist[i].afficher;
        if (Element->Objetlist[i].deleted) Element->Elements_Supprimes = true;
        for (j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
            Element->Objetlist[i].Facelist[j].deleted = !Element->Objetlist[i].Facelist[j].afficher;
            if (Element->Objetlist[i].Facelist[j].deleted) Element->Elements_Supprimes = true;
        }
    }
    Element->m_gllist = 0;  // Peut-être pas utile car les objets supprimés ne sont déjà plus affichés
    Element->Refresh();     // Idem
}

void OvniFrame::OnOutils_UnDeleteSelected(wxCommandEvent& event)
{
    // Pour réactiver les objets/facettes marqués comme supprimés (deleted), car en fait toujours là en mémoire.
    unsigned int i,j;
    for (i=0; i<Element->Objetlist.size(); i++) {
        Element->Objetlist[i].deleted  = false;
//        Element->Objetlist[i].afficher = true;
        for (j=0; j<Element->Objetlist[i].Facelist.size(); j++) {
            Element->Objetlist[i].Facelist[j].deleted  = false;  // Undelete
            Element->Objetlist[i].Facelist[j].afficher = true;   // + réafficher
        }
    }
    Element->Elements_Supprimes= false;
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::ReperagePoint_activer()
{
    wxCommandEvent cmd_event;
    OnMenu_ReperagePointSelected(cmd_event);
}

void OvniFrame::OnMenu_ReperagePointSelected(wxCommandEvent& event)
{
    wxString str;

// Init Objet
    ReperagePoint_Panel->SpinCtrl_IndiceObjet->SetMax(Element->Objetlist.size()-1);
    int IndiceObjet = 0;
    ReperagePoint_Panel->SpinCtrl_IndicePoint->SetMax(Element->Objetlist[IndiceObjet].Sommetlist.size() -1);
    ReperagePoint_Panel->SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    int num_obj = Element->Objetlist[IndiceObjet].GetValue();
    str.Printf(_T("%d"),num_obj);
    ReperagePoint_Panel->Text_NumeroObjet->SetValue(str);
//    str = Element->Objetlist[IndiceObjet].GetwxName();
    str = wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    ReperagePoint_Panel->Text_NomObjet->SetValue(str);

//Init point
    ReperagePoint_Panel->SpinCtrl_IndicePoint->SetValue(-1);
    str.Printf(_T(""));
    ReperagePoint_Panel->Text_ValeurX->SetValue(str);
    ReperagePoint_Panel->Text_ValeurY->SetValue(str);
    ReperagePoint_Panel->Text_ValeurZ->SetValue(str);
    Element->show_star = false;

    ReperagePoint_Panel->Show();
}

void OvniFrame::ReperageFacette_activer()
{
    wxCommandEvent cmd_event;
    OnMenu_ReperageFacetteSelected(cmd_event);
}

void OvniFrame::OnMenu_ReperageFacetteSelected(wxCommandEvent& event)
{
    wxString str;

// Init Objet
    ReperageFacette_Panel->SpinCtrl_IndiceObjet->SetMax(Element->Objetlist.size()-1);
    int IndiceObjet = 0;
    ReperageFacette_Panel->SpinCtrl_IndiceFacette->SetMax(Element->Objetlist[IndiceObjet].Facelist.size()); // indice max +1 pour boucler
    ReperageFacette_Panel->SpinCtrl_IndiceObjet->SetValue(IndiceObjet);
    int num_obj = Element->Objetlist[IndiceObjet].GetValue();
    str.Printf(_T("%d"),num_obj);
    ReperageFacette_Panel->Text_NumeroObjet->SetValue(str);
//    str = Element->Objetlist[IndiceObjet].GetwxName();
    str = wxString(Element->Objetlist[IndiceObjet].GetName(), wxConvUTF8);
    ReperageFacette_Panel->Text_NomObjet->SetValue(str);

// Init facette
    Element->Facette_Surlignee = false;
    ReperageFacette_Panel->SpinCtrl_IndiceFacette->SetValue(-1);
    str.Printf(_T("")) ;
    ReperageFacette_Panel->Text_NbSommets->SetValue(str);

    ReperageFacette_Panel->ButtonsDisable();    // Désactiver les boutons

    Element->Refresh();
    ReperageFacette_Panel->Show();
}

void OvniFrame::OnButton_Normale_BarycentreClick(wxCommandEvent& event)
{
    ReperageFacette_Panel->CheckBox_VisuNormale->SetValue(Button_Normale_Barycentre->GetValue());       // Synchroniser le bouton et la checkbox
    Element->AfficherNormaleFacette = Button_Normale_Barycentre->GetValue();                            // et la variable d'affichage
    Element->m_gllist = Element->glliste_select; //0   // pas très heureux ! Voir si on peut éviter de regénérer toutes les listes
    Element->Refresh();
}

void OvniFrame::OnButton_Normales_SommetsClick(wxCommandEvent& event)
{
    ReperageFacette_Panel->CheckBox_VisuNormales_Sommets->SetValue(Button_Normales_Sommets->GetValue());// Synchroniser le bouton et la checkbox
    Element->AfficherNormalesSommets = Button_Normales_Sommets->GetValue();                             // et la variable d'affichage
    Element->m_gllist = Element->glliste_select; //0    // pas très heureux ! Voir si on peut éviter de regénérer toutes les listes
    Element->Refresh();
}

void OvniFrame::OnMenu_AjouteConeSelected(wxCommandEvent& event)
{
    Cone_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Cone_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Cone_Panel->Show();
}

void OvniFrame::OnMenu_AjouteCubeSelected(wxCommandEvent& event)
{
    Cube_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Cube_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Cube_Panel->Show();
}

void OvniFrame::OnMenu_AjouteCylindreSelected(wxCommandEvent& event)
{
    Cylindre_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Cylindre_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Cylindre_Panel->Show();
}

void OvniFrame::OnMenu_AjouteEllipsoideSelected(wxCommandEvent& event)
{
    Ellipsoide_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Ellipsoide_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Ellipsoide_Panel->Show();
}

void OvniFrame::OnMenu_AjouteFacetteSelected(wxCommandEvent& event)
{
    Facette_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Facette_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Facette_Panel->Show();
}

void OvniFrame::OnMenu_AjouteSphereSelected(wxCommandEvent& event)
{
    Sphere_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Sphere_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Sphere_Panel->Show();
}

void OvniFrame::OnMenu_AjouteToreSelected(wxCommandEvent& event)
{
    Tore_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Tore_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Tore_Panel->Show();
}

void OvniFrame::OnMenu_AjouteIcosaedreSelected(wxCommandEvent& event)
{
    Icosaedre_Panel->SpinCtrl_Groupe  ->SetMax(maxGroupes-1);
    Icosaedre_Panel->SpinCtrl_Materiau->SetMax(maxGroupes-1);
    Icosaedre_Panel->Show();
}

void OvniFrame::OnMenu_SupprimerDerniereSelected(wxCommandEvent& event)
{

    bool test_delete = false;   // si false, objet marqué comme deleted, si true, tentative de destruction et de récupération de mémoire

    int indiceObjet = Element->Objetlist.size() -1;
    if (indiceObjet < 0) {      // Il ne reste rien !
        Element->OK_ToSave = false;
        return;
    }
    while (Element->Objetlist[indiceObjet].deleted) indiceObjet--;              // décompter les derniers objets s'ils sont déjà deleted !
    if ((Element->Objetlist[indiceObjet].primitive) && (indiceObjet >=0)) {     // Ne faire que sur les primitives

        if (test_delete) {
    // Semble OK une fois, mais plante ensuite.
    // Si on enlève les for ... jusqu'au printf (via test_delete), OK mais c'est toujours la primitive supprimée qui est réaffichée !!!
    // => l'objet n'est pas bien détruit ... à revoir

        // Récupérer la mémoire (pour limiter les soucis de fuite mémoire)
        for (unsigned int k=0; k<Element->Objetlist[indiceObjet].Nb_sommets; k++) {
            Element->Objetlist[indiceObjet].Sommetlist[k].point.pop_back();      // Supprimer les sommets
        }
        Element->Objetlist[indiceObjet].Sommetlist.clear();
        Element->Objetlist[indiceObjet].Sommetlist.pop_back();

        for (unsigned int j=0; j<Element->Objetlist[indiceObjet].Facelist.size(); j++)
            Element->Objetlist[indiceObjet].Facelist[j].F_sommets.pop_back();            // Supprimer les facettes
        Element->Objetlist[indiceObjet].Facelist.clear();
        Element->Objetlist[indiceObjet].Facelist.pop_back();

        if (Element->Objetlist[indiceObjet].Vecteurlist.size() >= 0) {
            for (unsigned int j=0; j<Element->Objetlist[indiceObjet].Vecteurlist.size(); j++)
                Element->Objetlist[indiceObjet].Vecteurlist[j].point.pop_back();        // Supprimer les normales aux sommets
            Element->Objetlist[indiceObjet].Vecteurlist.clear();
            Element->Objetlist[indiceObjet].Vecteurlist.pop_back();
        }

        // reste les Aspect_face ... si besoin !

        // Est-ce suffisant ?

        // fin de récupération
        printf("avant %d\n",(int)Element->Objetlist.size());
        Element->Objetlist[indiceObjet].deleted=true;
//        Element->Objetlist[indiceObjet].clear();
        Element->Objetlist.pop_back();
        printf("apres %d\n",(int)Element->Objetlist.size());
        }
        else
            Element->Objetlist[indiceObjet].deleted=true;   // Dans ce cas, on ne fait que marquer l'objet comme deleted !

        Element->m_gllist = 0;
        Element->searchMin_Max();
        Element->Refresh();
    } else {
        printf("Plus de primitives ajoutees !\n");
        if (indiceObjet < 0) Element->OK_ToSave = false;    // Il ne reste plus d'objets à enregistrer
    }
}

void OvniFrame::OnMenu_RAZ_SelectionFacettesSelected(wxCommandEvent& event)
{
// Efface la sélection de facettes effectuée à la souris <=> Touche S au clavier !
    wxKeyEvent key_event;
    key_event.m_keyCode = 'S';
    Element->OnKeyDown(key_event);              // Simule une pression sur la touche S au clavier
}

void OvniFrame::OnPopup_Centrer_sur_SelectionSelected(wxCommandEvent& event)
{
// Centre la rotation sur la sélection de facettes effectuée à la souris <=> Touche C au clavier !
    wxKeyEvent key_event;
    key_event.m_keyCode = 'C';
    Element->OnKeyDown(key_event);              // Simule une pression sur la touche C au clavier
}

void OvniFrame::OnPopup_Etendre_la_SelectionSelected(wxCommandEvent& event)
{
// Étendre la sélection de facettes effectuée à la souris <=> Touche X au clavier !
    wxKeyEvent key_event;
    key_event.m_keyCode = 'X';
    Element->OnKeyDown(key_event);              // Simule une pression sur la touche X au clavier
}


void OvniFrame::OnMenu_SupprimerFacettesSelected(wxCommandEvent& event)
{
// Efface la sélection de facettes effectuée à la souris <=> Touche S au clavier !
    wxKeyEvent key_event;
    key_event.m_keyCode = WXK_DELETE;           // Touche Suppr principale
    Element->OnKeyDown(key_event);              // Simule une pression sur la touche Suppr au clavier
}

void OvniFrame::OnMenu_MasquerFacettesSelected(wxCommandEvent& event)
{
// Efface la sélection de facettes effectuée à la souris <=> Touche S au clavier !
    wxKeyEvent key_event;
    key_event.m_keyCode = WXK_NUMPAD_DELETE;    // Touche Suppr du Numpad (via Maj su Verr Num activé)
    Element->OnKeyDown(key_event);              // Simule une pression sur la touche Suppr du clavier Numpad
}

void OvniFrame::OnGLCanvasRightUp(wxMouseEvent& event)
{
// Ne sert pas ???

//    printf("This : %d\n",this);
    printf("Right clic OvniFrame::OnGLCanvasRightUp\n");    // print pour voir quand cet évênement se produit
//    if (this == nullptr)
//        PopupMenu(&My_popupmenu);
//    else this->PopupMenu(&My_popupmenu);
}

void OvniFrame::OnPopup_Afficher_NormalesSommetsSelected(wxCommandEvent& event)
{
//    printf("clic sur Popup : Afficher les normales aux sommets\n");
    Button_Normales_Sommets->SetValue(!Button_Normales_Sommets->GetValue());        // Inverser l'état du bouton
    wxCommandEvent event_button(wxEVT_COMMAND_BUTTON_CLICKED);  // Simuler un clic sur le bouton d'affichage des normales aux sommets
    OnButton_Normales_SommetsClick(event_button);
}

void OvniFrame::OnPopup_Afficher_NormalesSelected(wxCommandEvent& event)
{
//    printf("clic sur Popup : Afficher les normales\n");
    Button_Normale_Barycentre->SetValue(!Button_Normale_Barycentre->GetValue());    // Inverser l'état du bouton
    wxCommandEvent event_button(wxEVT_COMMAND_BUTTON_CLICKED);  // Simuler un clic sur le bouton d'affichage des normales aux barycentres
    OnButton_Normale_BarycentreClick(event_button);
}

void OvniFrame::OnMenu_DeplacerBddSelected(wxCommandEvent& event)
{
    DeplacerBdd_Panel->Show();
}

void OvniFrame::OnMenu_FacteurEchelleBddSelected(wxCommandEvent& event)
{
    ChangerEchelleBdd_Panel->Show();
}

void OvniFrame::OnMenu_CentrageAutoSelected(wxCommandEvent& event)
{
    Element->centreRot = Element->centre_auto;
    Element->m_gllist  = 0;
    Element->Refresh();
}

void OvniFrame::OnMenu_ZoomAutoSelected(wxCommandEvent& event)
{
    Element->SetPosObs(true);
    Element->m_gllist = 0;
    Element->Refresh();
}

void OvniFrame::OnMenu_EnregistrerPaletteSelected(wxCommandEvent& event)
{
    int i,j;
    wxCharBuffer  buffer;
    wxString Nom_Fichier_Palette;

//    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
//    wxString appPath(f.GetPath());

//    printf("exe path : %s\n",(const char *)appPath.mb_str());
//    printf("current  : %s\n",(const char *)wxGetCwd().mb_str());

//    wxSetWorkingDirectory(appPath);

//    appPath += wxFILE_SEP_PATH; // Ajouter le séparateur de Path, sinon wxFileSelector ne l'utilise pas correctement

    wxString Selector   = _T("Tous les fichiers (*.*)|*.*|Fichiers palettes (*.pal)|*.pal");
    Nom_Fichier_Palette = wxFileSelector(_T("Enregistrer la palette"), Element->wxOvniPath, wxEmptyString, _T("pal"), Selector, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if (!Nom_Fichier_Palette.IsEmpty()) { // Si vide, on ne fait rien
        buffer=Nom_Fichier_Palette.mb_str();
        std::ofstream myfile;
        myfile.open (buffer.data());
        if (myfile.is_open()) {
            for (i=0; i<Element->nb_couleurs; i++) {
                myfile << std::setw(3) << i ;
                for (j=0; j<4; j++) myfile << " " << std::fixed << std::setprecision(4) << std::setw(6) << Element->MatAmbient_avionG[i][j];
                myfile << "\n";
                myfile << std::setw(3) << i ;
                for (j=0; j<4; j++) myfile << " " << std::fixed << std::setprecision(4) << std::setw(6) << Element->MatDiffuse_avionG[i][j];
                myfile << "\n";
            }
            myfile.close();

            Element->pal_file_modified = false;          // Marquer le fichier palette comme non modifié

            wxString Nom = wxFileNameFromPath(Nom_Fichier_Palette);
            buffer = Nom.mb_str();
            printf("\nEnregistrement de %s OK !\n",buffer.data());
        } else
            printf("Erreur d'ouverture du fichier %s\n",buffer.data());
    }
}

void OvniFrame::OnMenu_RelirePaletteSelected(wxCommandEvent& event)
{
    int i,j,k;
    wxCharBuffer  buffer;
    wxString Nom_Fichier_Palette;

    wxString Selector   = _T("Tous les fichiers (*.*)|*.*|Fichiers palettes (*.pal)|*.pal");
    Nom_Fichier_Palette = wxFileSelector(_T("Ouvrir un fichier de palette"), Element->wxOvniPath, wxEmptyString, _T("pal"), Selector, wxFD_OPEN);

    if (!Nom_Fichier_Palette.IsEmpty()) { // Si vide, c'est qu'on n'a pas choisi le fichier => on ne fait rien
        buffer=Nom_Fichier_Palette.mb_str();
        std::ifstream myfile;
        myfile.open (buffer.data());
        for (i=0; i<Element->nb_couleurs; i++) {
            myfile >> k;
            if (myfile.eof()) break;    // Fin de fichier atteinte trop tôt !
            for (j=0; j<4; j++) myfile >> Element->MatAmbient_avionG[i][j];
            myfile >> k;
            if (myfile.eof()) break;    // Fin de fichier atteinte trop tôt !
            for (j=0; j<4; j++) myfile >> Element->MatDiffuse_avionG[i][j];
        }
        if (myfile.eof()) printf("Seulement %d groupes dans ce fichier !!\n",i-1) ;

        myfile.close();

        if((Element->groupes) || (Element->materials)) {    // A ne faire que si l'un des modes affichage des groupes ou matériaux est actif
            Element->m_gllist = 0;                         // Mettre l'affichage à jour
            Element->Refresh();
        }
    }
}

void OvniFrame::OnMenu_CouleurDesGroupesSelected(wxCommandEvent& event)
{
    // Initialiser avec la couleur 0
    wxColor col;
    int Numero_couleur = CouleursGroupes_Panel->SpinCtrl_Groupes->GetValue();
    col.Set(lround(Element->MatAmbient_avionG[Numero_couleur][0]*255),
            lround(Element->MatAmbient_avionG[Numero_couleur][1]*255),
            lround(Element->MatAmbient_avionG[Numero_couleur][2]*255));
    CouleursGroupes_Panel->ColourPickerCtrl_Ambient->SetColour(col);
    col.Set(lround(Element->MatDiffuse_avionG[Numero_couleur][0]*255),
            lround(Element->MatDiffuse_avionG[Numero_couleur][1]*255),
            lround(Element->MatDiffuse_avionG[Numero_couleur][2]*255));
    CouleursGroupes_Panel->ColourPickerCtrl_Diffuse->SetColour(col);
//    CouleursGroupes_Panel->TextCtrl_Materiau->SetLabel(_T("Aucun")); // Inutile car soit déjà comme ça, soit changé et conforme à SpinCtrl_Groupes
    CouleursGroupes_Panel->Show();
}

void OvniFrame::OnMenuItem_ImagePpmSelected(wxCommandEvent& event)
{
/*!
/ fonction : enregistre une image au format PPM
/ entree :   aucune
/ sortie :   aucune
*/
	char *nom_fic_image ;
	char *p_work_dir;
	FILE *f ;
	int hauteur, largeur, larg3p;
	int i, j, xx;
//	int pix_offset=0;   // Provisoire : En version Tcl, sous Windows, il fallait ajouter 2 pixels (bug ???)

	wxCharBuffer buffer = Element->wxWorkDir.mb_str();
	p_work_dir = buffer.data();

    nom_fic_image = (char *)calloc(400, sizeof(char));
	strcpy(nom_fic_image,p_work_dir) ;
	if (strlen(nom_fic_image) > 0) strcat(nom_fic_image,"/") ;
	strcat(nom_fic_image,"Image_Ovni.ppm") ;

	f = fopen(nom_fic_image, "wb"); // ajout du b pour forcer l'écriture binaire (pour Windows).

	if (f == NULL) {
        wxMessageBox(_T("Erreur : Ouverture en écriture du fichier Image_Ovni.ppm impossible !"),_T("Avertissement"));
        return ;
	}

    Lire_Image(largeur, hauteur);
    larg3p = 3*largeur;

/*!définition du type d'image*/
	fprintf(f,"P6\n");
	fprintf(f,"# image Ovni-ONERA\n");
	fprintf(f,"%d %d\n",largeur,hauteur);
	fprintf(f,"# Format PPM \n");
	fprintf(f,"255\n");

    for (j=0; j<hauteur ; j++) {
//       glReadPixels (0, j, larg, 1, GL_RGB, GL_UNSIGNED_BYTE, pixelsRGB);     // Lecture ligne par ligne (moins performante)
//        xx = j*(larg3p+pix_offset) ;          // Tenir compte éventuellement des 2 octets supplémentaires/ligne
        xx = j*larg3p ;                         // Indice en début de ligne
        for (i=0; i<larg3p; i++,xx++) {
            fprintf(f,"%c",pixelsRGB[xx]);
        }
	}

	/*!Vide le buffer stdin*/
	fflush(stdin);

	/*!Fermeture du fichier*/
	fclose(f);

	printf("Fichier Image : Image_Ovni.ppm de taille %d*%d OK",largeur,hauteur);
	if (strlen(p_work_dir) > 0) printf(" dans\n%s\n", p_work_dir) ;

	// Libérer la mémoire allouée par malloc
	free(pixelsRGB) ;
	free(nom_fic_image) ;
}

void OvniFrame::OnMenuItem_ImageJpegSelected(wxCommandEvent& event)
{
	int largeur, hauteur;

	wxString wxNomFichier = Element->wxWorkDir;
	wxNomFichier += _T("Image_Ovni.jpg") ;

	Lire_Image(largeur, hauteur);

	wxImage *image = new wxImage(largeur, hauteur, pixelsRGB, false);
	image->SetOption(wxIMAGE_OPTION_QUALITY, _T("100"));    // Qualité du jpeg (100 = max)
	bool test = image->SaveFile(wxNomFichier, wxBITMAP_TYPE_JPEG);

    if (test) {
        printf("Fichier Image : Image_Ovni.jpg de taille %d*%d OK",largeur,hauteur);
        printf(" dans\n%s\n",(const char *)Element->wxWorkDir.mb_str()) ;
    } else {
        printf("Erreur dans l'enregistrement de Image_Ovni.jpg\n");
    }

    free(pixelsRGB) ;
}

void OvniFrame::OnMenuItem_ImagePngSelected(wxCommandEvent& event)
{
#include <wx/image.h>
	int largeur, hauteur;

	wxString wxNomFichier = Element->wxWorkDir;
	wxNomFichier += _T("Image_Ovni.png") ;

    Lire_Image(largeur, hauteur);

	wxImage *image = new wxImage(largeur, hauteur, pixelsRGB, false);
	bool test = image->SaveFile(wxNomFichier, wxBITMAP_TYPE_PNG);

    if (test) {
        printf("Fichier Image : Image_Ovni.png de taille %d*%d OK",largeur,hauteur);
        printf(" dans\n%s\n",(const char *)Element->wxWorkDir.mb_str()) ;
    } else {
        printf("Erreur dans l'enregistrement de Image_Ovni.png\n");
    }
//    image->Destroy();     // fait planter !

	free(pixelsRGB) ;
}

void OvniFrame::Lire_Image(int &largeur, int &hauteur)
{
// D'après un Copier-Coller de la version Tcl + C de : PPM Enregistre_image()

/* Modifs GD pour lire en mode RGB dans glReadPixels en une fois plutôt que
*  3 fois en modes RED, GREEN puis BLUE
*  Lecture ligne par ligne et stockage au fur et a mesure (sinon soucis de décalages)
*       ne marche pas toujours (pb sur certaines cartes graphiques au 2ème appel)
*  Si lecture globale (comme ici), il faut ajouter 2 octets par ligne sous TclTk
*/
	int viewport [4];

// Bug ?? avec TclTk et sous Windows, il faut pix_offset=2 octets de plus par ligne (n'existent pas avec glut seul !!)
//	int pix_offset=0;   // à supprimer ??

	GLsizei larg, haut;
	int larg3p;

	glGetIntegerv (GL_VIEWPORT, viewport);

	larg = viewport[2] -4;      // -4 pour éviter une marge noire de 4 pixels à droite et en haut des images lues sur l'écran.
	haut = viewport[3] -4;
	if (larg & 1) {             // Détecte une valeur impaire de la largeur image
        ++larg; //!Pour avoir des valeurs paires
//        printf("Largeur ajustee : %d\n",larg);
	}
//	if (haut & 1) ++haut;       // Ne semble pas nécessaire
	larg3p = 3*larg ;           // 1 pixel en largeur contient les 3 composantes R, G et B, soit 3 octets.

//	pixelsRGB = (char*) calloc((larg3p+pix_offset)*haut, sizeof(char));
	pixelsRGB = (unsigned char *) calloc(larg3p*haut, sizeof(unsigned char));

	glPixelStorei (GL_PACK_ALIGNMENT, 1);   // UNPACK dans la version Tcl ?

	glReadPixels (0, 0, larg, haut, GL_RGB, GL_UNSIGNED_BYTE, pixelsRGB);       // Lecture globale

	// Inverser haut/bas de l'image
	char mem_chr;
	int i, j, x1, x2;
	for (j=0; j<haut/2; j++) {
//        y2 = haut-1 -y;
//        x1 = y* (larg3p+pix_offset) ;
//        x2 = y2*(larg3p+pix_offset) ;
        x1 = j*larg3p ;             // Indice de début de ligne du bas
        x2 = (haut-1 -j)*larg3p ;   // Indice de début de ligne du haut
        for (i=0; i<larg3p; i++) {
            mem_chr         = pixelsRGB[x1];
            pixelsRGB[x1++] = pixelsRGB[x2];
            pixelsRGB[x2++] = mem_chr;
        }
	}

    largeur = larg;
    hauteur = haut;

	return ;
}

void OvniFrame::OnTimer_SaveTrigger(wxTimerEvent& event)
{
    wxString Fichier_svg;
    if (Element->bdd_modifiee) {                    // Sauvegarde automatique inutile si la bdd n'a pas changé
        printf("Sauvegarde automatique ...\n");
        Fichier_svg = Element->wxWorkDir + Element->Fichier_Autosvg;
//        printf("Enregistrement dans : %s\n",(const char *)Fichier_svg.mb_str()) ;
        Element->SaveTo(Fichier_svg,0);
        Element->bdd_modifiee = true;               // Garder/Forcer la valeur à true car on a enregistré la sauvegarde sous un autre nom (Autosave.bdd)
                                                    // et peut-être ailleurs que dans le répertoire d'origine du fichier bdd initial
        Element->exists_Autosave = true;            // Marquer que le fichier Autosave a été créé ici ! Mais ne détecte pas s'il y a eu une erreur dans SaveTo !
    }
}

void OvniFrame::OnMenu_ZoomSpecifiqueSelected(wxCommandEvent& event)
{
    Element->m_gldata.roty = 0 ;                    // en toute rigueur, forcer cette valeur à 0
    ZoomSpecifique_Panel->StaticText_Warning->Hide();
    float val = Element->m_gldata.zoom;
    wxString str_loc;
    str_loc.Printf(_T("%4.3f"),val);
    ZoomSpecifique_Panel->TextCtrl_FoV->SetLabel(str_loc);
    val = -(Element->m_gldata.posz +Element->centreRot[2])/Element->diagonale_save;
    str_loc.Printf(_T("%4.3f"),val);
    ZoomSpecifique_Panel->TextCtrl_Distance->SetLabel(str_loc);
    ZoomSpecifique_Panel->Show();                   // Afficher d'abord, sinon SetAngles ne modifie pas les valeurs Site Azimut !
    SetAngles();
}

void OvniFrame::OnMenuItem_AideSelected(wxCommandEvent& event)
{
    AideHtml_Panel->Show();
}
