#ifndef OVNIMAIN_H
#define OVNIMAIN_H

/***************************************************************
 * Name:      OvniMain.h
 * Purpose:   Defines Application Frame
 * Author:    Gerard Durand (Gerard.Durand@onera.fr)
 * Created:   2011-02-10
 * Copyright: Gerard Durand ()
 * License:
 **************************************************************/

//(*Headers(OvniFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
//*)

#if !wxCHECK_VERSION(3,0,0)
    #include <wx/things/toggle.h>   // Pour avoir wxCustomButton dans wxWidgets 2.8.12
//    #include <wx/things/spinctld.h>
#endif

//#include <wx/defs.h>
#include "Interface.h"

#include "Prefs_Dialog.h"
#include "ModificationPanel.h"
#include "TranslationPanel.h"
#include "RotationPanel.h"
#include "ScalePanel.h"
#include "SelectionPanel.h"
#include "PropertiesPanel.h"
#include "PositionObs_AzimutSite.h"
#include "PositionSource.h"
#include "CentreRotation.h"
#include "ReperagePoint.h"
#include "ReperageFacette.h"
#include "ReperageGroupe.h"
#include "ReperageMateriau.h"
#include "ReperageObjet.h"
#include "CouleursGroupes.h"
#include "Cone.h"
#include "Cube.h"
#include "Cylindre.h"
#include "Facette.h"
#include "Sphere.h"
#include "Icosaedre.h"
#include "Ellipsoide.h"
#include "Tore.h"
#include "ChoixAffichageObjets.h"
#include "DeplacerBdd.h"
#include "ChangerEchelleBdd.h"
#include "ManipulationsPanel.h"
#include "SelectionManuelleFacettes.h"
#include "SelectionManuelleObjets.h"
#include "ZoomSpecifique.h"
#include "Aide_html.h"

#include <math.h>

#define tempo_s 2       //

// Pour test du darkmode sous Windows à partir de wxWidgets 3.3.0 (beta)
#if defined(__WXMSW__)
#if wxCHECK_VERSION(3,3,0)
#include <wx/msw/darkmode.h>
class MySettings : public wxDarkModeSettings
{
public:
    virtual wxColour GetMenuColour(wxMenuColour which)
    {
        if ( which == wxMenuColour::StandardFg )
            return *wxCYAN;

        return wxDarkModeSettings::GetMenuColour(which);
    }

    wxColour GetColour(wxSystemColour index) override
    {
    switch ( index )
        {
        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_HIGHLIGHTTEXT:
        case wxSYS_COLOUR_BTNTEXT:
        case wxSYS_COLOUR_INFOTEXT:
        case wxSYS_COLOUR_LISTBOXTEXT:
        case wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT:
            // Default colour used here is 0xe0e0e0.
            return *wxCYAN;

        default:
            return wxDarkModeSettings::GetColour(index);
        }
    }
};
#endif // wxCHECK_VERSION
#endif // __WXMSW__

class OvniFrame: public wxFrame
{
    public:
         //(*Declarations(OvniFrame)
         wxBoxSizer* BoxSizer1;
         wxBoxSizer* BoxSizer2;
         wxButton* Button_Bas;
         wxButton* Button_Droite;
         wxButton* Button_Gauche;
         wxButton* Button_Haut;
         wxButton* Button_ZoomMoins;
         wxButton* Button_ZoomPlus;
         wxGLCanvas* GLCanvas;
         wxMenu* MenuFile;
         wxMenu* Menu_Affichage;
         wxMenu* Menu_Aide;
         wxMenu* Menu_Image;
         wxMenu* Menu_Options;
         wxMenu* Menu_Outils;
         wxMenu* Menu_Primitive;
         wxMenu* Menu_Reperage;
         wxMenu* Menu_Transformations;
         wxMenuBar* MenuBar_Globale;
         wxMenuItem* Inverse_All_Selected_Normales;
         wxMenuItem* Inverser_Toutes_les_Normales;
         wxMenuItem* MenuItem_About;
         wxMenuItem* MenuItem_Aide;
         wxMenuItem* MenuItem_ImageJpeg;
         wxMenuItem* MenuItem_ImagePng;
         wxMenuItem* MenuItem_ImagePpm;
         wxMenuItem* MenuItem_PermXY;
         wxMenuItem* MenuItem_PermXYZ;
         wxMenuItem* MenuItem_PermXZ;
         wxMenuItem* MenuItem_PermYZ;
         wxMenuItem* MenuItem_Quitter;
         wxMenuItem* MenuItem_SigneX;
         wxMenuItem* MenuItem_SigneY;
         wxMenuItem* MenuItem_SigneZ;
         wxMenuItem* Menu_AddFile;
         wxMenuItem* Menu_Affichage_Axes;
         wxMenuItem* Menu_Affichage_Boite;
         wxMenuItem* Menu_Affichage_Filaire;
         wxMenuItem* Menu_Affichage_Plein;
         wxMenuItem* Menu_Affichage_Points;
         wxMenuItem* Menu_Affichage_Source;
         wxMenuItem* Menu_AjouteCone;
         wxMenuItem* Menu_AjouteCube;
         wxMenuItem* Menu_AjouteCylindre;
         wxMenuItem* Menu_AjouteEllipsoide;
         wxMenuItem* Menu_AjouteFacette;
         wxMenuItem* Menu_AjouteIcosaedre;
         wxMenuItem* Menu_AjouteSphere;
         wxMenuItem* Menu_AjouteTore;
         wxMenuItem* Menu_CentrageAuto;
         wxMenuItem* Menu_CentreRotation;
         wxMenuItem* Menu_CouleurDesGroupes;
         wxMenuItem* Menu_DeplacerBdd;
         wxMenuItem* Menu_Enregistrer;
         wxMenuItem* Menu_EnregistrerPalette;
         wxMenuItem* Menu_Enregistrer_Sous;
         wxMenuItem* Menu_FacteurEchelleBdd;
         wxMenuItem* Menu_Hardware3D;
         wxMenuItem* Menu_MasquerFacettes;
         wxMenuItem* Menu_Open;
         wxMenuItem* Menu_Origine;
         wxMenuItem* Menu_PositionObservateur;
         wxMenuItem* Menu_PositionSource;
         wxMenuItem* Menu_Preferences;
         wxMenuItem* Menu_Proprietes;
         wxMenuItem* Menu_RAZ_SelectionFacettes;
         wxMenuItem* Menu_ReOpen;
         wxMenuItem* Menu_RelirePalette;
         wxMenuItem* Menu_ReperageFacette;
         wxMenuItem* Menu_ReperageGroupe;
         wxMenuItem* Menu_ReperageMateriau;
         wxMenuItem* Menu_ReperageObjet;
         wxMenuItem* Menu_ReperagePoint;
         wxMenuItem* Menu_Reperage_Couleurs_Facettes;
         wxMenuItem* Menu_Reperage_Couleurs_Groupes;
         wxMenuItem* Menu_Reperage_Couleurs_Materiaux;
         wxMenuItem* Menu_Retracer3D;
         wxMenuItem* Menu_SensDesNormales;
         wxMenuItem* Menu_SupprimerDerniere;
         wxMenuItem* Menu_SupprimerFacettes;
         wxMenuItem* Menu_VueDeDessus;
         wxMenuItem* Menu_VueDeFace;
         wxMenuItem* Menu_VueDeProfil;
         wxMenuItem* Menu_ZoomAuto;
         wxMenuItem* Menu_ZoomSpecifique;
         wxMenuItem* Outils_Modifications;
         wxMenuItem* Outils_Reafficher;
         wxMenuItem* Outils_Selec_Depl;
         wxMenuItem* Outils_Supprimer_Masques;
         wxMenuItem* Outils_UnDelete;
         wxMenuItem* Outils_choix_afficher;
         wxPanel* Panel1;
         wxPanel* Panel_Sliders;
         wxSlider* Slider_x;
         wxSlider* Slider_y;
         wxSlider* Slider_z;
         wxStaticText* StaticText1;
         wxStaticText* StaticText2;
         wxStatusBar* StatusBar1;
         wxTimer Timer_Save;
         //*)

#if wxCHECK_VERSION(3,0,0)
         wxToggleButton* Button_Axes;
         wxToggleButton* Button_Boite;
         wxToggleButton* Button_Filaire;
         wxToggleButton* Button_Gouraud;
         wxToggleButton* Button_Groupes;
         wxToggleButton* Button_Materiaux;
         wxToggleButton* Button_Modifs;
         wxToggleButton* Button_Normale_Barycentre;
         wxToggleButton* Button_Normales_Sommets;
         wxToggleButton* Button_Outils;
         wxToggleButton* Button_Plein;
         wxToggleButton* Button_Points;
         wxToggleButton* Button_Sliders;
         wxToggleButton* Button_Source;
#else
         wxCustomButton* Button_Axes;
         wxCustomButton* Button_Boite;
         wxCustomButton* Button_Filaire;
         wxCustomButton* Button_Gouraud;
         wxCustomButton* Button_Groupes;
         wxCustomButton* Button_Materiaux;
         wxCustomButton* Button_Modifs;
         wxCustomButton* Button_Normale_Barycentre;
         wxCustomButton* Button_Normales_Sommets;
         wxCustomButton* Button_Outils;
         wxCustomButton* Button_Plein;
         wxCustomButton* Button_Points;
         wxCustomButton* Button_Sliders;
         wxCustomButton* Button_Source;
#endif // wxCHECK_VERSION

        BddInter *Element=nullptr;
        OvniFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~OvniFrame();
        bool OnInit();
        wxPoint wxPrefs_pos;
        wxPoint wxModificationPanel_pos;
        wxPoint wxPropertiesPanel_pos;

        CentreRotation*             CentreRotation_Panel;
        ChangerEchelleBdd*          ChangerEchelleBdd_Panel;
        ChoixAffichageObjets*       ChoixAffichageObjets_Panel;
        Cone*                       Cone_Panel;
        CouleursGroupes*            CouleursGroupes_Panel;
        Cube*                       Cube_Panel;
        Cylindre*                   Cylindre_Panel;
        DeplacerBdd*                DeplacerBdd_Panel;
        Ellipsoide*                 Ellipsoide_Panel;
        Facette*                    Facette_Panel;
        Icosaedre*                  Icosaedre_Panel;
        ManipulationsPanel*         Manipulations_Panel;
        ModificationPanel*          Modifications_Panel;
        PositionObs_AzimutSite*     PositionObsAzimutSite_Panel;
        PositionSource*             PositionSource_Panel;
        Prefs_Dialog*               Preferences_Panel;
        PropertiesPanel*            Properties_Panel;
        ReperageFacette*            ReperageFacette_Panel;
        ReperageGroupe*             ReperageGroupe_Panel;
        ReperageMateriau*           ReperageMateriau_Panel;
        ReperageObjet*              ReperageObjet_Panel;
        ReperagePoint*              ReperagePoint_Panel;
        RotationPanel*              Rotation_Panel;
        ScalePanel*                 Scale_Panel;
        SelectionPanel*             Selections_Panel;
        SelectionManuelleFacettes*  Selections_Manuelles_Facettes;
        SelectionManuelleObjets*    Selections_Manuelles_Objets;
        Sphere*                     Sphere_Panel;
        Tore*                       Tore_Panel;
        TranslationPanel*           Translation_Panel;
        ZoomSpecifique*             ZoomSpecifique_Panel;
        Aide_html*                  AideHtml_Panel;

        bool toggle_modif = false;
        bool toggle_outils= false;
        bool New_file     = true;
        bool verbose      = false;   // il y en a aussi un dans BddInter qui utilise une copie de celui-ci !

//        int FilterIndex=0;
//        static wxString s_extFileDef;
//        void changeangle();
        void Ouvrir_Fichier();
        int  Ouvrir_Ini();
        void ResizeOpenGL(int , int );
        void SetAngles();
        int  SetNewIcons(int);
        void Redisplay_Proprietes(wxCommandEvent& event);
        void Toggle_Sliders(wxCommandEvent& event); // Fonction publique pour appel externe à OvniFrame de OnButton_SlidersToggle
        void Toggle_Gouraud(wxCommandEvent& event); // Idem .  .  .  .  .  .  .  .  .  .  .  .  .  .  . de OnButton_GouraudToggle
        void Toggle_Groupes(wxCommandEvent& event);
        void Toggle_Materiaux(wxCommandEvent& event);

// ID_POPUP_* créés par wxSmith en private initialement, puis déplacés ici en public
        static const long ID_POPUP_RAZ;
        static const long ID_POPUP_COMPLEMT;
        static const long ID_POPUP_ETENDRE;
        static const long ID_POPUP_CENTRER;
        static const long ID_POPUP_MASQUER;
        static const long ID_POPUP_DEMASQUER;
        static const long ID_POPUP_DELETE;
        static const long ID_POPUP_UNDELETE;
        static const long ID_POPUP_NEWOBJECT;
        static const long ID_POPUP_INVERSER_N;
        static const long ID_POPUP_PARCOURS_I;
        static const long ID_POPUP_RAZ_SELECT;
        static const long ID_POPUP_NORM_F;
        static const long ID_POPUP_NORM_S;
        static const long ID_POPUP_FLAT;
        static const long ID_POPUP_NOT_FLAT;

        void ReperageObjet_activer();
        void ReperagePoint_activer();
        void ReperageFacette_activer();

        wxMenuItem* Menu_ReOpen3ds=nullptr; // Pour réouvrir spécifiquement un fichier 3ds en changeant le mode de décalage

    private:
        //(*Handlers(OvniFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnGLCanvasPaint(wxPaintEvent& event);
        void OnMenu_NotImplemented(wxCommandEvent& event);
        void OnButton_PleinToggle(wxCommandEvent& event);
        void OnButton_PointsToggle(wxCommandEvent& event);
        void OnButton_FilaireToggle(wxCommandEvent& event);
        void OnButton_SlidersToggle(wxCommandEvent& event);
        void OnButton_AxesToggle(wxCommandEvent& event);
        void OnButton_BoiteToggle(wxCommandEvent& event);
        void OnButton_SourceToggle(wxCommandEvent& event);
        void OnMenu_Affichage_PointsSelected(wxCommandEvent& event);
        void OnMenu_Affichage_FilaireSelected(wxCommandEvent& event);
        void OnMenu_Affichage_PleinSelected(wxCommandEvent& event);
        void OnMenu_Affichage_AxesSelected(wxCommandEvent& event);
        void OnMenu_Affichage_BoiteSelected(wxCommandEvent& event);
        void OnMenu_Affichage_SourceSelected(wxCommandEvent& event);
        void OnButton_GroupesToggle(wxCommandEvent& event);
        void OnButton_MateriauxToggle(wxCommandEvent& event);
        void OnMenu_Reperage_Couleurs_FacettesSelected(wxCommandEvent& event);
        void OnMenu_Reperage_Couleurs_GroupesSelected(wxCommandEvent& event);
        void OnMenu_Reperage_Couleurs_MateriauxSelected(wxCommandEvent& event);
        void OnMenu_OpenSelected(wxCommandEvent& event);
        void OnSlider_xCmdScroll(wxScrollEvent& event);
        void OnSlider_yCmdScroll(wxScrollEvent& event);
        void OnSlider_zCmdScroll(wxScrollEvent& event);
        void OnMenu5Selected(wxCommandEvent& event);
        void OnMenuItem3Selected(wxCommandEvent& event);
        void OnOutils_Selec_DeplSelected(wxCommandEvent& event);
        void OnOutils_ModificationsSelected(wxCommandEvent& event);
        void OnButton_GouraudToggle(wxCommandEvent& event);
        void OnButton_ModifsToggle(wxCommandEvent& event);
        void OnMenu_SensDesNormalesSelected(wxCommandEvent& event);
        void OnMenu_XminusX(wxCommandEvent& event);
        void OnMenu_YminusY(wxCommandEvent& event);
        void OnMenu_ZminusZ(wxCommandEvent& event);
        void OnMenu_XtoY(wxCommandEvent& event);
        void OnMenu_XtoZ(wxCommandEvent& event);
        void OnMenu_YtoZ(wxCommandEvent& event);
        void OnMenu_XtoYtoZ(wxCommandEvent& event);
        void OnButton_OutilsToggle(wxCommandEvent& event);
        void OnButton_GaucheClick(wxCommandEvent& event);
        void OnButtonDroiteClick(wxCommandEvent& event);
        void OnButton_ZoomPlusClick(wxCommandEvent& event);
        void OnButton_ZoomMoinsClick(wxCommandEvent& event);
        void OnButton_DroiteClick(wxCommandEvent& event);
        void OnButton_HautClick(wxCommandEvent& event);
        void OnButton_BasClick(wxCommandEvent& event);
        void OnMenuProprietesSelected(wxCommandEvent& event);
        void OnMenu_PreferencesSelected(wxCommandEvent& event);
        void OnMenu_ReperageGroupeSelected(wxCommandEvent& event);
        void OnMenu_ReperageMateriauSelected(wxCommandEvent& event);
        void OnMenu_ReperageObjetSelected(wxCommandEvent& event);
        void OnOutils_choix_afficherSelected(wxCommandEvent& event);
        void OnOutils_ReafficherSelected(wxCommandEvent& event);
        void OnOutils_Supprimer_MasquesSelected(wxCommandEvent& event);
        void OnOutils_UnDeleteSelected(wxCommandEvent& event);
        void OnMenu_ReperagePointSelected(wxCommandEvent& event);
        void OnMenu_ReperageFacetteSelected(wxCommandEvent& event);
        void OnButton_Normale_BarycentreClick(wxCommandEvent& event);
        void OnButton_Normales_SommetsClick(wxCommandEvent& event);
        void OnMenu_AjouteCubeSelected(wxCommandEvent& event);
        void OnMenu_AjouteConeSelected(wxCommandEvent& event);
        void OnMenu_AjouteCylindreSelected(wxCommandEvent& event);
        void OnMenu_AjouteEllipsoideSelected(wxCommandEvent& event);
        void OnMenu_AjouteFacetteSelected(wxCommandEvent& event);
        void OnMenu_AjouteSphereSelected(wxCommandEvent& event);
        void OnMenu_AjouteIcosaedreSelected(wxCommandEvent& event);
        void OnMenu_SupprimerDerniereSelected(wxCommandEvent& event);
        void OnMenu_AjouteSphereSelected1(wxCommandEvent& event);
        void OnMenu_Hardware3DSelected(wxCommandEvent& event);
        void OnMenu_RAZ_SelectionFacettesSelected(wxCommandEvent& event);
        void OnMenu_CentreRotationSelected(wxCommandEvent& event);
        void OnMenu_Enregistrer_Sous(wxCommandEvent& event);
        void OnMenu_OrigineSelected(wxCommandEvent& event);
        void OnMenu_PositionObservateurSelected(wxCommandEvent& event);
        void OnMenu_PositionSourceSelected(wxCommandEvent& event);
        void OnMenu_VueDeFaceSelected(wxCommandEvent& event);
        void OnMenu_VueDeProfilSelected(wxCommandEvent& event);
        void OnMenu_VueDeDessusSelected(wxCommandEvent& event);
        void OnMenu_ProprietesSelected(wxCommandEvent& event);
        void OnMenu_MasquerFacettesSelected(wxCommandEvent& event);
        void OnMenu_SupprimerFacettesSelected(wxCommandEvent& event);
        void OnGLCanvasRightUp(wxMouseEvent& event);
        void OnMenu_AfficherNormalesSelected(wxCommandEvent& event);
        void OnPopup_Afficher_NormalesSommetsSelected(wxCommandEvent& event);
        void OnPopup_Afficher_NormalesSelected(wxCommandEvent& event);
        void OnMenu_ReOpenSelected(wxCommandEvent& event);
        void OnMenu_Inverser_Toutes_les_NormalesSelected(wxCommandEvent& event);
        void OnMenu_Inverse_All_Selected_NormalesSelected(wxCommandEvent& event);
        void OnMenu_DeplacerBddSelected(wxCommandEvent& event);
        void OnMenu_FacteurEchelleBddSelected(wxCommandEvent& event);
        void OnMenu_CentrageAutoSelected(wxCommandEvent& event);
        void OnMenu_ZoomAutoSelected(wxCommandEvent& event);
        void OnMenu_EnregistrerPaletteSelected(wxCommandEvent& event);
        void OnMenu_RelirePaletteSelected(wxCommandEvent& event);
        void OnMenu_CouleurDesGroupesSelected(wxCommandEvent& event);
        void OnMenu_AddFileSelected(wxCommandEvent& event);
        void OnMenu_EnregistrerSelected(wxCommandEvent& event);
        void OnMenuItem_ImagePpmSelected(wxCommandEvent& event);
        void OnMenuItem_ImageJpegSelected(wxCommandEvent& event);
        void OnMenuItem_ImagePngSelected(wxCommandEvent& event);
        void OnTimer_SaveTrigger(wxTimerEvent& event);
        void OnMenu_ZoomSpecifiqueSelected(wxCommandEvent& event);
        void OnMenuItem_HelpSelected(wxCommandEvent& event);
        void OnMenuItem_AideSelected(wxCommandEvent& event);
        void OnMenu_ReOpen3dsSelected(wxCommandEvent& event);
        void OnMenu_AjouteToreSelected(wxCommandEvent& event);
        void OnMenu_Retracer3DSelected(wxCommandEvent& event);
        //*)

        void Lire_Image(int &, int &);
        void OnModifsXYZ();
        void OnPopup_Centrer_sur_SelectionSelected(wxCommandEvent& event);
        void OnPopup_Complementer_SelectionFacettesSelected(wxCommandEvent& event);
        void OnPopup_Etendre_la_SelectionSelected(wxCommandEvent& event);
        void OnPopup_Reverse_ParcoursSelected(wxCommandEvent& event);
        void OnPopup_Raz_Select_FSelected(wxCommandEvent& event);
        void OnPopup_ForcerFacettesPlanesSelected(wxCommandEvent& event);
        void OnPopup_ForcerFacettesNonPlanesSelected(wxCommandEvent& event);
        void OnPopup_CreerObjetFacettesSelected(wxCommandEvent& event);
        bool OnBdd_modifiee();
        void OnPal_modifiee();
        void GenereListeAretes();

        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_BUTTON10;
        static const long ID_BUTTON11;
        static const long ID_BUTTON12;
        static const long ID_BUTTON13;
        static const long ID_BUTTON14;
        static const long ID_BUTTON15;
        static const long ID_BUTTON16;
        static const long ID_BUTTON17;
        static const long ID_BUTTON18;
        static const long ID_BUTTON19;
        static const long ID_BUTTON20;

		//(*Identifiers(OvniFrame)
		static const long ID_STATICTEXT1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_PANEL1;
		static const long ID_GLCANVAS;
		static const long ID_SLIDER_X;
		static const long ID_SLIDER_Y;
		static const long ID_SLIDER_Z;
		static const long ID_PANEL2;
		static const long idOpenFile;
		static const long idReopenFile;
		static const long idAddFile;
		static const long idSaveFile;
		static const long idSaveFileAs;
		static const long idProperties;
		static const long idPrefs;
		static const long idHardware;
		static const long idMenuQuit;
		static const long menu_Affichage_Points;
		static const long menu_Affichage_Filaire;
		static const long menu_Affichage_Plein;
		static const long menu_Affichage_Axes;
		static const long menu_Affichage_Boite;
		static const long menu_Affichage_Source;
		static const long ID_MENUITEM1;
		static const long ID_MENUITEM2;
		static const long ID_MENUITEM3;
		static const long ID_MENUITEM4;
		static const long ID_MENUITEM5;
		static const long ID_MENUITEM6;
		static const long ID_MENUITEM7;
		static const long ID_MENUITEM8;
		static const long ID_MENUITEM32;
		static const long ID_MENUITEM33;
		static const long ID_MENUITEM34;
		static const long ID_MENUITEM35;
		static const long ID_MENUITEM36;
		static const long ID_MENUITEM37;
		static const long ID_MENUITEM38;
		static const long ID_MENUITEM51;
		static const long ID_MENUITEM39;
		static const long ID_MENUITEM9;
		static const long ID_MENUITEM10;
		static const long ID_MENUITEM11;
		static const long ID_MENUITEM12;
		static const long ID_MENUITEM13;
		static const long ID_MENUITEM14;
		static const long menu_reperage_couleurs_facettes;
		static const long menu_reperage_couleurs_groupes;
		static const long menu_reperage_couleurs_materiaux;
		static const long ID_MENUITEM50;
		static const long ID_MENUITEM49;
		static const long ID_MENUITEM48;
		static const long ID_MENUITEM40;
		static const long ID_MENUITEM41;
		static const long ID_MENUITEM42;
		static const long ID_MENUITEM15;
		static const long ID_MENUITEM16;
		static const long ID_MENUITEM28;
		static const long ID_MENUITEM29;
		static const long ID_MENUITEM30;
		static const long ID_MENUITEM31;
		static const long ID_MENUITEM52;
		static const long ID_MENUITEM17;
		static const long ID_MENUITEM18;
		static const long ID_MENUITEM19;
		static const long ID_MENUITEM20;
		static const long ID_MENUITEM21;
		static const long ID_MENUITEM22;
		static const long ID_MENUITEM23;
		static const long ID_MENUITEM24;
		static const long ID_MENUITEM27;
		static const long ID_MENUITEM25;
		static const long ID_MENUITEM26;
		static const long ID_MENUITEM43;
		static const long ID_MENUITEM44;
		static const long ID_MENUITEM45;
		static const long ID_MENUITEM46;
		static const long ID_MENUITEM47;
		static const long idMenuHelp;
		static const long idMenuAbout;
		static const long ID_STATUSBAR1;
		static const long ID_TIMER1;
		//*)

        static const long idReopenFile3ds;

        wxString str;

        //opengl methods
        wxGLContext* m_glcontext;
        void InitOpenGL(void);
//        void ResizeOpenGL(int iWidth, int iHeight);   // Déplacé en public pour tests
        void InitBoutons(void);
        unsigned char *pixelsRGB;

        DECLARE_EVENT_TABLE()

    protected:
        void OnSize(wxSizeEvent& event);
};

#endif // OVNIMAIN_H

