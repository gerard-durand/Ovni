//(*AppHeaders
#include "Interface.h"
//*)
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>      /* printf */
#include <cmath>       /* round, floor, ceil, trunc, M_PI, ... */
#include <cstdlib>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "vector3d.h"
#include "utf8.h"
#include "ply.h"

#include "OvniMain.h"

const float to_Deg = 180.0f/M_PI;
const float to_Rad = 1.0f/to_Deg;
const float pas_deg= 5.0f;
const float pas_rad= pas_deg/to_Deg;

static char Message[1024];

const char *fichier_init="Ovni.ini";        // a priori dans le même répertoire que l'exécutable Ovni.exe. à vérifier ! Sinon récupérer le appPath (via OvniApp ?)

/* Clés du fichier .ini */
const char *init1="Longueur_axe=";
const char *init2="Longueur_normales=";
const char *init3="Diam_source=";
const char *init4="Antialiasing_Soft=";
const char *init5="Forcer_1_Obj=";
const char *init6="Lecture_obj_opt=";
const char *init7="Decalage_3ds=";
const char *init8="Visu_seuils=";
const char *init9="Seuil_gouraud=";
const char *initA="Coef_2eme_angle=";
const char *initB="Enr_Normales_Seuil=";
const char *initC="Calcul_normales_S=";
const char *initD="Temps_sauveg=";
const char *initE="Methode_triangles=";
const char *initF="Mode_Trackball=";
const char *initG="View_OpenGL_FPS=";
const char *initH="Rep_travail=";       // Répertoire où est le fichier Bdd (pas Ovni.exe !)
const char *initI="Creer_Backup=";
const char *initJ="Suppr_Backup=";
const char *initK="Msg_Warning=";
const char *initL="Raz_Selection_F=";
const char *initM="FacettesNotFlat=";

FILE* f;                                // Doit être ici pour pouvoir être utilisé aussi dans la lecture des fichiers G3D (hors BddInter)

#define XML_FMT_INT_MOD "l"
#define BUFFSIZE        8192

char XML_Buff[BUFFSIZE];
int  XML_Depth;
int  i_objetXML_courant=0;              // Utilisé seulement en lecture de fichier G3D. Donner une valeur initiale

char s1[666]; //,buffer[1000] ;			// chaines de caractères. Déclarée ici car utilisée aussi dans la lecture des fichiers G3D (hors BddInter)

unsigned int codegroupe,codemateriau;
int message_multi_sample_actif = 0;

BEGIN_EVENT_TABLE(BddInter, wxGLCanvas)
    EVT_SIZE(BddInter::OnSize)
    EVT_PAINT(BddInter::OnPaint)
    EVT_ERASE_BACKGROUND(BddInter::OnEraseBackground)
    EVT_KEY_DOWN( BddInter::OnKeyDown )
    EVT_MOUSE_EVENTS(BddInter::OnMouse)
    EVT_MOUSEWHEEL(BddInter::OnMouseWheelMoved)
END_EVENT_TABLE()

BddInter::BddInter(wxWindow *parent, wxWindowID id, const int* AttribList, const wxPoint& pos, const wxSize& size, long style, bool main_verbose, const wxString& name):
#if wxCHECK_VERSION(3,0,0)
    wxGLCanvas(parent, id, AttribList, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
// Explicitly create a new rendering context instance for this canvas.
    m_glRC = new wxGLContext(this);

#else
    wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, AttribList)
{
#endif // wxCHECK_VERSION

    verbose = main_verbose; // On recopie dans verbose la valeur transmise depuis OvniMain (ou on utilise la valeur par défaut soit false !)

    if(verbose) printf("Entree de BddInter::BddInter\n");

    m_gldata.initialized = false;
    alpha  = 0.0;            // Ne servent plus ??? sauf peut-être via et dans CalculAngles
    beta   = 0.0;
    gamma  = 0.0;
    modeGL = standard;
    objet_under_mouse = -1;
    face_under_mouse  = -1;
    point_under_mouse = -1;
    line_under_mouse  = -1;
    undo_memory       =  0;
    OK_ToSave         = false;

    centreRot.resize(3);    // Init vecteur à 3 composantes : x, y et z
    centre_auto.resize(3);  // Idem

    // Initialiser les tables de couleurs avec les valeurs par défaut
    for (int j=0;  j<4 ; j++) {
        for (int i=0 ; i<nb_couleurs ; i++) {
            MatAmbient_avionG[i][j] = MatAmbient_avionG_def[i][j];
            MatDiffuse_avionG[i][j] = MatDiffuse_avionG_def[i][j];
        }
    }

    ResetData();                // Remettre aux valeurs par défaut
    Ouvrir_ini_file();          // Lire dans le fichier .ini les valeurs éventuellement modifiées / défaut
    pal_file_modified = false;  // Fichier palette non lu, donc par défaut, non modifié !

    Smemory = nullptr;
    selectBuffer   = (GLuint*)malloc(BUFSIZE*sizeof(GLuint));
    this->type     = -1;
    this->m_loaded = false;
    this->MAIN_b   = dynamic_cast<OvniFrame*>((this->GetParent())->GetParent());    // Il faut remonter de 2 parents (GLCanvas puis OvniFrame).
//    wxSize ClientSize = this->GetSize();
//    printf("BddInter::BddInter ClientSize X/Y %d %d\n",ClientSize.x,ClientSize.y);
    //W=ClientSize.x;
    //H=ClientSize.y;
    if(verbose) printf("Sortie de BddInter::BddInter\n");
}

void BddInter::output_Glut_Msg(GLfloat x, GLfloat y, char *text)
{
// Affiche un message text dans la fenetre OpenGL
    char *p;
    int sx=800, sy=600; // Ne sert qu'en local, et les valeurs n'ont pas d'importance

    glPushAttrib(GL_ENABLE_BIT);
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, sx, 0, sy);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos3f(x*sx,y*sy,1.0) ;
    for (p = text; *p; p++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);   // GLUT_BITMAP_TIMES_ROMAN_24 ou 10 GLUT_BITMAP_9_BY_15 8_BY_13
                                                        // GLUT_BITMAP_HELVETICA_10 12 ou 18
                                                        // S'arrête sur *p = NULL
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glPopAttrib();
}

/*void idleFunc()
{
	delta_time = glutGet(GLUT_ELAPSED_TIME) - timebase;
	frames++;
	if (delta_time > 1000) {
	    fps = frames*1000.0f/delta_time;
	    frames = 0;
	    timebase += delta_time;
	    printf("fps = %5.1f\n",fps) ;
	}
} */

int BddInter::convert_rotx_LSI() {
    int ival;
    ival = lround(fmod((m_gldata.rotx - 270), 360)) ;        //! Comme dans la version TCL
    if (ival < -180) ival += 360 ;
    return ival;
}

int BddInter::convert_rotz_LAZ() {
    int ival;
    ival = lround(fmod((360 - m_gldata.rotz), 360)) ;
    if (ival < 0) ival += 360 ;
    return ival;
}

void BddInter::ResetData() {
    int ival;

    if (verbose) printf("Entree de BddInter::ResetData\n");
    m_gldata.BeginX     = 0;
    m_gldata.BeginY     = 0;
    len_axe             = len_axe_def;
    len_normales        = len_normales_def;
    ray_sun             = ray_sun_def;
    angle_Gouraud       = angle_Gouraud_def;
    fmult_Gouraud       = fmult_Gouraud_def;
    seuil_Gouraud       = cos(angle_Gouraud*to_Rad);
    if (angle_Gouraud  >= 179.9) seuil_Gouraud  = -1.0f;
    angle_Gouraud2      = angle_Gouraud*fmult_Gouraud;
    seuil_Gouraud2      = cos(angle_Gouraud2*to_Rad);
    if (angle_Gouraud2 >= 179.9) seuil_Gouraud2 = -1.0f;
    tolerance           = tolerance_def;
    svg_time            = svg_time_def;
    Raz_Selection_F     = Raz_Selection_F_def;
    msg_warning         = msg_warning_def;

    m_gldata.rotx       = 0.0f;
    m_gldata.roty       = 0.0f;
    m_gldata.rotz       = 0.0f;
    m_gldata.fmult_diag = fmult_diag_def;
    reset_zoom          = true;
    SetPosObs(reset_zoom);
    mode_selection      = selection_facette;

    m_gldata.mode_Trackball = mode_Trackball_def;
    printf("ResetData : Mode Trackball : %d\n",m_gldata.mode_Trackball);
    if (Slider_x != nullptr) {
        Slider_x->SetValue(0);
        Slider_y->SetValue(0);
        Slider_z->SetValue(0);
    }
    trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);

    antialiasing_soft   = antialiasing_soft_def;
    test_decalage3ds    = test_decalage3ds_def;
    Forcer_1_Seul_Objet = Forcer_1_Seul_Objet_def;
    lect_obj_opt        = lect_obj_opt_def;
    if (Forcer_1_Seul_Objet) lect_obj_opt=false;                // Pas d'optimisation si 1 seul Objet 3D dans un fichier .obj
    CalculNormalesLectureBdd = CalculNormalesLectureBdd_def;
    Enr_Normales_Seuillees   = Enr_Normales_Seuillees_def;
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    wxWorkDir = wxOvniPath   = appPath + wxFILE_SEP_PATH;       // Ajouter le séparateur de Path à la fin
    if (verbose) {
        sprintf(Message,"ResetData : Répertoire de travail :\n%s\n",(const char*)wxWorkDir.utf8_str());
        printf(utf8_To_ibm(Message));
    }
    if (MPrefs != nullptr) {
        MPrefs->CheckBox_AntialiasingSoft->SetValue(antialiasing_soft);
        MPrefs->CheckBox_TestDecalage3DS->SetValue(test_decalage3ds);
        MPrefs->CheckBox_1SeulObjet3D->SetValue(Forcer_1_Seul_Objet);
        if (Forcer_1_Seul_Objet) MPrefs->CheckBox_LectureOptimisee->Disable();
        else                     MPrefs->CheckBox_LectureOptimisee->Enable();
        MPrefs->CheckBox_LectureOptimisee->SetValue(lect_obj_opt);
        MPrefs->RadioBox_Triangulation->SetSelection(methode_Triangulation);
        MPrefs->RadioBox_Trackball->SetSelection(m_gldata.mode_Trackball);
        MPrefs->CheckBox_DisplayFps->SetValue(viewFps_def);
        MPrefs->CheckBox_CalculNormales->SetValue(CalculNormalesLectureBdd);
        MPrefs->CheckBox_RecNormales_Seuillees->SetValue(Enr_Normales_Seuillees);
        if (test_seuil_gouraud) MPrefs->CheckBox_RecNormales_Seuillees->Enable();
        else                    MPrefs->CheckBox_RecNormales_Seuillees->Disable();

        MPrefs->SpinCtrl_PasSvg->SetValue(svg_time);
        // Reset du répertoire de travail par défaut = chemin de l'exécutable
        MPrefs->TextCtrl_WorkDir->SetLabel(wxWorkDir);
    }
    if (MPosObs != nullptr) {
        ival = convert_rotx_LSI();
        MPosObs->SpinCtrl_LSI->SetValue(ival);

        ival = convert_rotz_LAZ();
        MPosObs->SpinCtrl_LAZ->SetValue(ival);
    }
    if (MZoomSpec != nullptr) {
        ival = convert_rotx_LSI();
        MZoomSpec->SpinCtrl_LSI->SetValue(ival);

        ival = convert_rotz_LAZ();
        MZoomSpec->SpinCtrl_LAZ->SetValue(ival);
        if (MZoomSpec->IsShown()) {
           str.Printf(_T("%4.3f"),m_gldata.zoom);
           MZoomSpec->TextCtrl_FoV->SetValue(str);
           str.Printf(_T("%4.3f"),m_gldata.fmult_diag);
           MZoomSpec->TextCtrl_Distance->SetValue(str);
        }
    }
    for (int i=0 ; i<4 ; i++) Light0Position[i] = Light0Position_def[i];
    if (MPosLight != nullptr) {
        MPosLight->SpinCtrlDouble_PosX->SetValue(Light0Position[0]);
        MPosLight->SpinCtrlDouble_PosY->SetValue(Light0Position[1]);
        MPosLight->SpinCtrlDouble_PosZ->SetValue(Light0Position[2]);
        MPosLight->SpinCtrlDouble_PosW->SetValue(Light0Position[3]);
    }

    centreRot = centre_auto;
    centrageRotAuto = true;
    if (MPosCRot != nullptr) {
        MPosCRot->SpinCtrlDouble_X->SetValue(centreRot[0]);
        MPosCRot->SpinCtrlDouble_Y->SetValue(centreRot[1]);
        MPosCRot->SpinCtrlDouble_Z->SetValue(centreRot[2]);
    }

    if (MPanel != nullptr) {
        str.Printf(_T("%7.1e"),tolerance);
        MPanel->TextCtrl_Tolerance->SetValue(str);
        MPanel->CheckBox_Transparence->SetValue(false);
        MPanel->activer_transparence = false;
        MPanel->CheckBox_NotFlat->SetValue(NotFlat);
    }

    if (MRotation != nullptr) {
        MRotation->RotX = MRotation->RotY = MRotation->RotZ = 0.0;
        MRotation->RadioBox_Centre->SetSelection(0);
    }
    if (MScale != nullptr) {
        MScale->ScaleX = MScale->ScaleY = MScale->ScaleZ = 1.0;
        str.Printf(_T("%4.1f"),MScale->ScaleX);
        MScale->TextCtrl_ScaleX->SetValue(str);
        MScale->TextCtrl_ScaleY->SetValue(str);
        MScale->TextCtrl_ScaleZ->SetValue(str);
        MScale->CheckBox_ScaleUnique->SetValue(true);
    }
    if (MSelect != nullptr) {
        MSelect->RadioBox_TypeSelection->SetSelection(0);
        TypeSelection = 0;
    }
    glDisable(GL_CULL_FACE);    // Mode compatible de TypeSelection = 0

    m_gldata.initialized = false ;

// Initialisation pour l'affichage des Fps
    viewFps  = viewFps_def ;
    timebase = glutGet(GLUT_ELAPSED_TIME);
	frames   = 0 ;
	sprintf(Message_fps," ");

// Initialisation des couleurs des groupes/matériaux via MatAmbient_avionG
//    for (int i=0; i<nb_couleurs ; i++) {
//        for (int j=0; j<3; j++) color_groupe_material[i][j] = MatAmbient_avionG[i][j];
//    }

//    Refresh();  // Ce Refresh ne sert sans doute à rien ... à vérifier !
    if(verbose) printf("Sortie de BddInter::ResetData\n");
}

void BddInter::Ouvrir_ini_file()
{
    FILE *f_init;
    char *Lu, *p_txt_wrk;
    int icmp, ibool, len ;

    f_init = fopen(fichier_init,"r") ;  // Si le fichier n'existe pas, l'ignorer => Utiliser les valeurs par défaut
    if (f_init != NULL) {
        while ((Lu = fgets(Message,300,f_init)) != NULL) {
            len = strlen( init1);
            icmp= strncmp(init1,Message,len) ;                  // Test sur 1er mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%f",&len_axe) ;               // Récupère la valeur de len_axe
                continue;   // Passer au while suivant
            }
            len = strlen( init2);
            icmp= strncmp(init2,Message,len) ;                  // Test sur 2ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%f",&len_normales) ;          // Récupère la valeur des longueurs de normales
                continue;   // Passer au while suivant
            }
            len = strlen( init3);
            icmp= strncmp(init3,Message,len) ;                  // Test sur 3ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%f",&ray_sun) ;               // Récupère la valeur de dia_sun
                continue;   // Passer au while suivant
            }
            len = strlen( init4);
            icmp= strncmp(init4,Message,len) ;                  // Test sur 4ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de antialiasing_soft (Gestion par OpenGL ou Hard)
                if (ibool == 0)
                     antialiasing_soft = false;
                else antialiasing_soft = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( init5);
            icmp= strncmp(init5,Message,len) ;                  // Test sur 5ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de Forcer_1_Seul_Objet
                if (ibool == 0)
                     Forcer_1_Seul_Objet = false;
                else Forcer_1_Seul_Objet = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( init6);
            icmp= strncmp(init6,Message,len) ;                  // Test sur 6ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de lect_obj_opt
                if (ibool == 0)
                     lect_obj_opt = false;
                else lect_obj_opt = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( init7);
            icmp= strncmp(init7,Message,len) ;                  // Test sur 7ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de test_decalage3ds
                if (ibool == 0)
                     test_decalage3ds = false;
                else test_decalage3ds = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( init8);
            icmp= strncmp(init8,Message,len) ;                  // Test sur 8ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de test_seuil_gouraud
                if (ibool == 0)
                     test_seuil_gouraud = false;
                else test_seuil_gouraud = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( init9);
            icmp= strncmp(init9,Message,len) ;                  // Test sur 9ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%f",&angle_Gouraud) ;         // Récupère la valeur de angle_Gouraud
                seuil_Gouraud = cos(angle_Gouraud*to_Rad);
                if (angle_Gouraud >= 179.9) seuil_Gouraud=-1.0;
                // Adapter aussi le 2ème angle
                angle_Gouraud2 = angle_Gouraud*fmult_Gouraud;
                seuil_Gouraud2 = cos(angle_Gouraud2*to_Rad);
                if (angle_Gouraud2 >= 179.9) seuil_Gouraud2 = -1.0f;
                continue;   // Passer au while suivant
            }
            len = strlen( initA);
            icmp= strncmp(initA,Message,len) ;                  // Test sur 10ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%f",&fmult_Gouraud) ;         // Récupère la valeur de fmult_Gouraud
                angle_Gouraud2 = angle_Gouraud*fmult_Gouraud;
                seuil_Gouraud2 = cos(angle_Gouraud2*to_Rad);
                if (angle_Gouraud2 >= 179.9) seuil_Gouraud2 = -1.0f;
                continue;   // Passer au while suivant
            }
            len = strlen( initB);
            icmp= strncmp(initB,Message,len) ;                  // Test sur 11ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de Enr_Normales_Seuillees
                if (ibool == 0)
                     Enr_Normales_Seuillees = false;
                else Enr_Normales_Seuillees = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initC);
            icmp= strncmp(initC,Message,len) ;                  // Test sur 12ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de CalculNormalesLectureBdd (forcer le calcul des normales aux sommets dès la lecture)
                if (ibool == 0)
                     CalculNormalesLectureBdd = false;
                else CalculNormalesLectureBdd = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initD);
            icmp= strncmp(initD,Message,len) ;                  // Test sur 13ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&svg_time) ;              // Récupère la valeur de svg_time
                continue;   // Passer au while suivant
            }
            len = strlen( initE);
            icmp= strncmp(initE,Message,len) ;                  // Test sur 14ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&methode_Triangulation) ; // Récupère la valeur de mode_Triangulation
                continue;   // Passer au while suivant
            }
            len = strlen( initF);
            icmp= strncmp(initF,Message,len) ;                  // Test sur 15ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&m_gldata.mode_Trackball) ;// Récupère la valeur de mode_Trackball
                continue;   // Passer au while suivant
            }
            len = strlen(initG);
            icmp= strncmp(initG,Message,len) ;                  // Test sur 16ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de viewFps
                if (ibool == 0)
                     viewFps = false;
                else viewFps = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initH);
            icmp= strncmp(initH,Message,len) ;                  // Test sur 16ème mot clé
            if (!icmp) {
                p_txt_wrk = Lire_chaine(&Message[len]) ;
                sprintf(Message,"%s",p_txt_wrk) ;               // Récupère le répertoire de travail
                wxWorkDir = wxString::FromUTF8(Message);        // FromAscii(Message); // Lire en utf8 le nom (sinon souci avec les caractères accentués !)
                wxString Last_Char = wxFILE_SEP_PATH ;          // <=> wxFileName::GetPathSeparator()
                if (!wxWorkDir.EndsWith(Last_Char)) wxWorkDir += wxFILE_SEP_PATH ;          // Ajouter le séparateur de Path à la fin s'il n'y est pas déjà !
                sprintf(Message,"Travail dans : %s\n",(const char *)wxWorkDir.utf8_str()) ; // pb avec caractères accentués dans la console windows !
                printf(utf8_To_ibm(Message));
                continue;   // Passer au while suivant
            }
            len = strlen( initI);
            icmp= strncmp(initI,Message,len) ;                  // Test sur 16ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de viewFps
                if (ibool == 0)
                     CreerBackup = false;
                else CreerBackup = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initJ);
            icmp= strncmp(initJ,Message,len) ;                  // Test sur 16ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de viewFps
                if (ibool == 0)
                     SupprBackup = false;
                else SupprBackup = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initK);
            icmp= strncmp(initK,Message,len) ;                  // Test sur 17ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de msg_warning
                if (ibool == 0)
                     msg_warning = false;
                else msg_warning = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initL);
            icmp= strncmp(initL,Message,len) ;                  // Test sur 18ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de Raz_Selection_F
                if (ibool == 0)
                     Raz_Selection_F = false;
                else Raz_Selection_F = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initM);
            icmp= strncmp(initM,Message,len) ;                  // Test sur 19ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de NotFlat
                if (ibool == 0)
                     NotFlat = false;
                else NotFlat = true ;
                continue;   // Passer au while suivant
            }
        }
        ini_file_modified = false;      // Contenu du fichier ini_file non modifié (pas encore !!)
    } else {
        ini_file_modified = true;       // Contenu du fichier ini_file à créer car n'existait pas (sauf modifs ultérieures, ce sera avec les valeurs par défaut) !
    }
}

void BddInter::Stocker_ini_file()
{
    FILE *f_init;

    if (!ini_file_modified) return ;        // Ne rien faire si le fichier n'a pas été modifié

    f_init=fopen(fichier_init,"w") ;
    if (f_init != NULL) {
        fprintf(f_init,"%s%f\n",init1,len_axe) ;
        fprintf(f_init,"%s%f\n",init2,len_normales);
        fprintf(f_init,"%s%f\n",init3,ray_sun) ;
        fprintf(f_init,"%s%d\n",init4,antialiasing_soft);
        fprintf(f_init,"%s%d\n",init5,Forcer_1_Seul_Objet);
        fprintf(f_init,"%s%d\n",init6,lect_obj_opt);
        fprintf(f_init,"%s%d\n",init7,test_decalage3ds);            // Ne pas stocker tant que ce n'est pas suffisament fiable
        fprintf(f_init,"%s%d\n",init8,test_seuil_gouraud);
        fprintf(f_init,"%s%f\n",init9,angle_Gouraud);
        fprintf(f_init,"%s%f\n",initA,fmult_Gouraud);
        fprintf(f_init,"%s%d\n",initB,Enr_Normales_Seuillees);
        fprintf(f_init,"%s%d\n",initC,CalculNormalesLectureBdd);
        fprintf(f_init,"%s%d\n",initD,svg_time);                        // Pas encore implémenté
        fprintf(f_init,"%s%d\n",initE,methode_Triangulation);
        fprintf(f_init,"%s%d\n",initF,m_gldata.mode_Trackball);
        fprintf(f_init,"%s%d\n",initG,viewFps);
        fprintf(f_init,"%s%s\n",initH,(const char *)wxWorkDir.utf8_str());//mb_str());  // Convertir en utf8 pour enregistrer dans le fichier (accents conservés)
                // Note : si mb_str() il faut FromAscii() dans Ouvrir_ini_file mais alors pb avec wxWidgets 3.11 et + alors que c'est OK en 2.8.12 !

        fprintf(f_init,"%s%d\n",initI,CreerBackup);
        fprintf(f_init,"%s%d\n",initJ,SupprBackup);
        fprintf(f_init,"%s%d\n",initK,msg_warning);
        fprintf(f_init,"%s%d\n",initL,Raz_Selection_F);
        fprintf(f_init,"%s%d\n",initM,NotFlat);

//        fprintf(f_init,"TEST\n") ;
        fclose(f_init) ;
    }
    else {
        wxString wxMessage=_T("Ecriture impossible dans le fichier .ini");
        DisplayMessage(wxMessage,true);
//        return ;
    }
    return ;
}

void BddInter::OnSize(wxSizeEvent& event)
{
    // Ne semble pas (ou plus) servir (sauf peut-être via EVT_SIZE
    if(verbose) printf("Entree BddInter::OnSize\n");

//    // this is also necessary to update the context on some platforms
//    wxGLCanvas::OnSize(event);
//    wxSize s;
//    s = event.GetSize();
//    OvniFrame* MAIN=dynamic_cast<OvniFrame*>(this->GetParent());
//    MAIN->ResizeOpenGL(s.GetWidth(), s.GetHeight());
//    MAIN->Element->Refresh();
//    // Reset the OpenGL view aspect
//    ResetProjectionMode();
//    Resize();
//    event.Skip();
    if(verbose) printf("Sortie BddInter::OnSize\n");
}

void BddInter::Resize() {
    if (verbose) printf("Entree BddInter::Resize\n");
    ResetProjectionMode();
    Refresh();                                              // Est-ce utile ?
    if (verbose) printf("Sortie BddInter::Resize\n");
}

//void BddInter::Forcer_OnPaint( wxPaintEvent& event ) {    // Ne sert plus : il suffit de faire Update()
//    OnPaint(event);
//}

//void BddInter::OnPaint( wxPaintEvent& WXUNUSED(event) ) {
void BddInter::OnPaint( wxPaintEvent& event )
{
    float   quat[4];
    GLfloat m[4][4];
    int multi_s = 0;

    // must always be here
    wxPaintDC dc(this);
//    wxSize ClientSize = this->GetParent()->GetSize();

    if (verbose) printf("Entree BddInter::OnPaint\n");
#if wxCHECK_VERSION(3,0,0)
    SetCurrent(*m_glRC);
#else
#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();
#endif // wxCHECK_VERSION

    // Initialize OpenGL
    if (!m_gldata.initialized) {
        InitGL();
        ResetProjectionMode();
        m_gldata.initialized = true;
    }

    if (!m_loaded) {
        return;
    }
    // Clear
    glClearColor( 0.39f, 0.58f, 0.74f, 1.0f );  // Comme dans la version Tcl, sinon ( 0.3f, 0.4f, 0.6f, 1.0f ) dans la version originale de J.Dias
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(m_gldata.posx,m_gldata.posy,m_gldata.posz);
    glTranslatef(centreRot[0],centreRot[1],centreRot[2]);

//    printf("pos : %f %f %f, centre_auto : %f %f %f\n",m_gldata.posx, m_gldata.posy, m_gldata.posz,centre_auto[0],centre_auto[1],centre_auto[2]);
    //    printf("type=%d\n",type);
    if(type!=-1) {

        if (m_gldata.mode_Trackball) {
            build_rotmatrix( m, m_gldata.quat );
///            glMultMatrixf( &m[0][0] ) ;  // Ne doit pas servir à grand chose vu que la matrice précédente est Identity !
            // Calcul des 3 rotations de la Bdd d'après la matrice de rotation.
            CalculAngles(*m, m_gldata.rotx, m_gldata.roty, m_gldata.rotz) ;
        }
//        printf("%8.4f %8.4f %8.4f\n",m_gldata.rotx,m_gldata.roty,m_gldata.rotz);
        glRotatef(m_gldata.rotx,1.,0.,0.) ; // Affiche la Bdd avec les angles rotx, roty et rotz
        glRotatef(m_gldata.roty,0.,1.,0.) ;
        glRotatef(m_gldata.rotz,0.,0.,1.) ;
        glTranslatef(-centreRot[0],-centreRot[1],-centreRot[2]);

    }

    glGetIntegerv(GL_SAMPLE_BUFFERS, &multi_s); // On pourrait aussi tester glGetIntegerv(GL_SAMPLES, &multi_s) en plus, mais ne semble pas nécessaire !
    if (multi_s != 0) {                         // Ne pas activer/désactiver GL_MULTISAMPLE si ce n'est pas disponible (dans ce cas multi_s est nul !
        if (antialiasing_soft) {
            glEnable(GL_MULTISAMPLE);
//            glEnable(GL_DEPTH);
            if (verbose) printf("antialiasing On,  ");
        } else {
            glDisable(GL_MULTISAMPLE);
//            glDisable(GL_DEPTH);
            if (verbose) printf("antialiasing Off, ");
        }
    } else {
        if (antialiasing_soft) {
            message_multi_sample_actif += 1;
            if ((message_multi_sample_actif == 2) && msg_warning) {  // à ne faire qu'une seule fois par session, mais pas trop tôt !
                wxString wxMessage = _T("Le driver de la carte graphique n'active pas l'option GL_MULTISAMPLE et/ou\n");
                wxMessage         += _T("OVNI n'est pas dans la liste des softs connus de la carte graphique.\n");
                wxMessage         += _T("OpenGL ne peut donc pas traiter l'antialiasing des polygones.\n\n");
                wxMessage         += _T("Pour ne plus afficher ce message mettre Msg_Warning=0 dans Ovni.ini");
                DisplayMessage(wxMessage,false);
            }
        }
    }
    if (verbose) printf("glIsEnabled : %d\n", glIsEnabled(GL_MULTISAMPLE));
    if (verbose) {
        glGetIntegerv(GL_SAMPLE_BUFFERS,  &multi_s);
        printf("GL_SAMPLE_BUFFERS = %d\n", multi_s);
        glGetIntegerv(GL_SAMPLES, &multi_s);
        printf("GL_SAMPLES        = %d\n", multi_s);
    }

    if (type != -1) {
        switch(type_new) {      // A vérifier, mais type_new fait souvent double emploi avec type => en particulier dans les primitives, il faut entrer les mêmes valeurs !
        case 0:
//            printf("\ndraw dxf!!!\n");
            m_renderer.Render();
            break;
        case 1:
            this->drawOpenGL();
            break;
        default:
            printf("\nType non gere pour l'instant !!!\n");
            break;
        }
    }

    // Flush
//    glFlush();    // Pas forcément utile car le SwapBuffers ci-dessous le ferait implicitement

    // Swap
    SwapBuffers();

//    event.Skip(); // ne pas faire si on a mis WXUNUSED(event) dans la déclaration de OnPaint
                    // si on met event, le Skip ne trace pas tout (ou seulement après avoir bougé la souris par ex !)

    if (verbose) printf("Sortie BddInter::OnPaint\n");
}

/* Calcul d'une matrice de rotation OpenGL à partir des angles xAngle, yAngle et zAngle */
void BddInter::CalculMatriceRot() {
// Version pour des rotations X, Y puis Z
    float q[4],a[3],quat[4];

// Convertir les angles actuels en quaternion
    vset(a,1.,0.,0.);
    axis_to_quat(a,-m_gldata.rotx*to_Rad,quat);         // Pourquoi -1 ?

    vset(a,0.,1.,0.);
    axis_to_quat(a,-m_gldata.roty*to_Rad,q);            // Pourquoi -1 ?
    add_quats(q,quat,quat);

    vset(a,0.,0.,1.);
    axis_to_quat(a, m_gldata.rotz*to_Rad,q);
    add_quats(q,quat,quat);
    quat[2] *=-1.;                                      // Pourquoi changer de signe seulement cette composante ?

    for (int i=0;i<4;i++) m_gldata.quat[i]=quat[i];

// Construire la matrice de rotation
    build_rotmatrix( matquat, quat );
}

void BddInter::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    if(verbose) printf("Entree de OnEraseBackground\n");
    // Do nothing, to avoid flashing on MSW
    if(verbose) printf("Sortie de OnEraseBackground\n");
}

void BddInter::OnMouseZoom(wxMouseEvent& event, int signe, bool Touche_Maj) {

    int  mult = (Touche_Maj ? 10 : 1);  // Si la touche Maj du clavier est enfoncée, l'effet du zoom à la molette est multiplié par 10
    if (signe != 0) {
        if (signe > 0 ) {
            m_gldata.zoom += mult*m_gldata.zoom_step ;
        } else {
            if (m_gldata.zoom >= 5*m_gldata.zoom_step) {
                m_gldata.zoom -= mult*m_gldata.zoom_step ;//1.0;
            } else {
                if ( m_gldata.zoom >= m_gldata.zoom_step)
                     m_gldata.zoom -= m_gldata.zoom_step/10;    // Dans ces cas, réduire l'effet et ne plus utiliser le facteur multiplicatif
                else m_gldata.zoom -= m_gldata.zoom_step/100;
            }
        }
        m_gldata.zoom = (m_gldata.zoom < 0. ? m_gldata.zoom_step/100 : m_gldata.zoom) ; // Pour éviter les valeurs négatives
        if (MZoomSpec != nullptr) {
            if (MZoomSpec->IsShown()) {
                str.Printf(_T("%4.3f"),m_gldata.zoom);
                MZoomSpec->TextCtrl_FoV->SetValue(str);
            }
        }
        reset_zoom = false; // Inhiber un reset de Zoom
        ResetProjectionMode();
        Refresh(false);
    }
}

void BddInter::OnMouse(wxMouseEvent& event) {

    GLfloat m[4][4];  // On pourrait utiliser matquat de BddInter ?
    int val;
    static bool previous_right_drag = false;    // En statique pour retrouver la valeur précédente à chaque appel
                                                // (pourrait être dans BddInter mais n'est utile que dans OnMouse !)
    mouse_position = event.GetPosition();
    if (finishdraw == true) {

        if (event.RightUp() && !previous_right_drag) {  // Ne pas faire l'affichage des menus contextuels si juste après un drag souris bouton droit
//            printf("Right clic\n");
            wxMenu My_popupmenu;
            long ID_POPUP_RAZ        = MAIN_b->ID_POPUP_RAZ;
            long ID_POPUP_ETENDRE    = MAIN_b->ID_POPUP_ETENDRE;
            long ID_POPUP_CENTRER    = MAIN_b->ID_POPUP_CENTRER;
            long ID_POPUP_MASQUER    = MAIN_b->ID_POPUP_MASQUER;
            long ID_POPUP_DEMASQUER  = MAIN_b->ID_POPUP_DEMASQUER;
            long ID_POPUP_DELETE     = MAIN_b->ID_POPUP_DELETE;
            long ID_POPUP_UNDELETE   = MAIN_b->ID_POPUP_UNDELETE;
            long ID_POPUP_INVERSER_N = MAIN_b->ID_POPUP_INVERSER_N;
            long ID_POPUP_PARCOURS_I = MAIN_b->ID_POPUP_PARCOURS_I;
            long ID_POPUP_RAZ_SELECT = MAIN_b->ID_POPUP_RAZ_SELECT;
            long ID_POPUP_NORM_F     = MAIN_b->ID_POPUP_NORM_F;
            long ID_POPUP_NORM_S     = MAIN_b->ID_POPUP_NORM_S;
            long ID_POPUP_FLAT       = MAIN_b->ID_POPUP_FLAT;
            long ID_POPUP_NOT_FLAT   = MAIN_b->ID_POPUP_NOT_FLAT;
            // Recopie de ce qui est fait par wxSmith ... Faute de mieux ! Bourrin mais ça marche à condition de forcer en public les ID_POPUP*
            // Recopier en dehors de wxSmith, ce qui a été créé (ID_POPUP, CONNECT, ...) puis supprimer le popup menu de wxSmith !
            wxMenuItem * Popup_RAZ;
            if (mode_selection == selection_point)
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, _T("RAZ de sélection des points\t(s)"),   wxEmptyString, wxITEM_NORMAL);
            else if (mode_selection == selection_objet)
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, _T("RAZ de sélection des objets\t(s)"),   wxEmptyString, wxITEM_NORMAL);
            else
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, _T("RAZ de sélection des facettes\t(s)"), wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_RAZ);

            wxMenuItem * Popup_Centrer;
            if (this->ToSelect.ListeSelect.size() > 0)
                Popup_Centrer = new wxMenuItem((&My_popupmenu), ID_POPUP_CENTRER, _T("Centrer la rotation sur la sélection\t(c)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Centrer = new wxMenuItem((&My_popupmenu), ID_POPUP_CENTRER, _T("Recentrer sur la rotation par défaut\t(c)"), wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Centrer);

            wxMenuItem * Popup_Etendre = new wxMenuItem((&My_popupmenu), ID_POPUP_ETENDRE, _T("Étendre la sélection\t(x)"), wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Etendre);
            if (this->ToSelect.ListeSelect.size() == 0) Popup_Etendre->Enable(false);

            My_popupmenu.AppendSeparator();

            wxMenuItem * Popup_Masquer = new wxMenuItem((&My_popupmenu), ID_POPUP_MASQUER, _T("Masquer les facettes sélectionnées\t(Numpad Suppr)"), wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Masquer);

            wxMenuItem * Popup_Delete  = new wxMenuItem((&My_popupmenu), ID_POPUP_DELETE,  _T("Supprimer les facettes sélectionnées\t(Suppr)"),      wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Delete);

            My_popupmenu.AppendSeparator();

            wxMenuItem * Popup_Inverser ;
            if (this->ToSelect.ListeSelect.size() > 0)
                Popup_Inverser = new wxMenuItem((&My_popupmenu), ID_POPUP_INVERSER_N, _T("Inverser les normales sélectionnées\t(i)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Inverser = new wxMenuItem((&My_popupmenu), ID_POPUP_INVERSER_N, _T("Inverser toutes les normales\t(i)"),        wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Inverser);

            wxMenuItem * Popup_Reverse  = new wxMenuItem((&My_popupmenu), ID_POPUP_PARCOURS_I, _T("Inverser le sens de parcours des facettes sélectionnées\t(p)"), wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Reverse);

            wxMenuItem * Popup_Raz_Select;
            if (this->Raz_Selection_F)
                Popup_Raz_Select = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ_SELECT, _T("Désactiver la désélection automatique\t(z)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Raz_Select = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ_SELECT, _T("Activer la désélection automatique\t(z)"),    wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Raz_Select);

            My_popupmenu.AppendSeparator();

            wxMenuItem * Popup_Afficher_Normales;
            if (AfficherNormaleFacette)
                Popup_Afficher_Normales = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_F, _T("Ne pas afficher les normales aux barycentres"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Afficher_Normales = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_F, _T("Afficher les normales aux barycentres"),        wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Afficher_Normales);

            wxMenuItem * Popup_Afficher_NormalesSommets;
            if (AfficherNormalesSommets)
                Popup_Afficher_NormalesSommets = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_S, _T("Ne pas afficher les normales aux sommets"),  wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Afficher_NormalesSommets = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_S, _T("Afficher les normales aux sommets"),         wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Afficher_NormalesSommets);

            wxMenuItem * Popup_Forcer_Facettes_Planes;
            Popup_Forcer_Facettes_Planes = new wxMenuItem((&My_popupmenu), ID_POPUP_FLAT,         _T("Forcer les facettes à être planes"),  wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Forcer_Facettes_Planes);

            wxMenuItem * Popup_Forcer_Facettes_NonPlanes;
            Popup_Forcer_Facettes_NonPlanes = new wxMenuItem((&My_popupmenu), ID_POPUP_NOT_FLAT,  _T("Forcer les facettes à être non planes"),  wxEmptyString, wxITEM_NORMAL);
            My_popupmenu.Append(Popup_Forcer_Facettes_NonPlanes);

            if (Elements_Masques || Elements_Supprimes)
                My_popupmenu.AppendSeparator(); // Ajout d'un séparateur si des élements de menu suivent ...

            if (Elements_Masques) {             // S'il y a au moins une facette masquée, proposer de les réafficher
                wxMenuItem * Popup_Demasquer = new wxMenuItem((&My_popupmenu), ID_POPUP_DEMASQUER, _T("Réafficher les facettes masquées"), wxEmptyString, wxITEM_NORMAL);
                My_popupmenu.Append(Popup_Demasquer);
            }
            if (Elements_Supprimes) {           // S'il y a au moins une facette supprimée, proposer de les restituer
                wxMenuItem * Popup_Undelete = new wxMenuItem((&My_popupmenu), ID_POPUP_UNDELETE, _T("Restituer les facettes supprimées"),  wxEmptyString, wxITEM_NORMAL);
                My_popupmenu.Append(Popup_Undelete);
            }
            PopupMenu(&My_popupmenu, mouse_position.x, mouse_position.y);
//            printf("This : %d\n",this);
//            PopupMenu(&MAIN_b->My_popupmenu, mouse_position.x, mouse_position.y);   // Plante à l'exécution quand créé par wxSmith
        }

        previous_right_drag = false;    // Reset de l'affichage des menus contextuels (ne le sauter que la première fois après un drag bouton droit)

        if (event.LeftDown()) {
            m_gldata.BeginX = mouse_position.x ; //event.GetX(); // idem à mouse_position.x
            m_gldata.BeginY = mouse_position.y ; //event.GetY(); // idem à mouse_position.y
//            printf("begin X Y : %d %d\n",m_gldata.BeginX,m_gldata.BeginY);
        }

//        modeGL = standard;
        if (mode_selection == selection_point) modeGL = points;

//        if(event.RightDown()) {
        if (event.MiddleDown()) {   // Comme en version Tcl, bouton Milieu. mêmes actions qu'initialement event.RightDown() ||
//            printf("event.MiddleDown\n");
            this->ToSelect.verrouiller_ListeSelect(false);  // Déverrouiller la liste (par précaution)
//            if (test_rectangle_selection) {                 // ici pour l'instant (phase de test)
                xd_sel = xa_sel = mouse_position.x;             // Initialisations pour un éventuel tracé de rectangle de sélection
                yd_sel = ya_sel = mouse_position.y;
//                select_largeur  = select_hauteur = 1;           // Donner au moins 1 pixel de largeur/hauteur
//            }
//            if (mode_selection == selection_point)   m_gllist = glliste_points;
//            if (mode_selection == selection_facette) m_gllist = glliste_select;
            testPicking(mouse_position.x, mouse_position.y, modeGL, true) ; // OnOff sur true pour basculer en sélectionné / non sélectionné
//            stopPicking();    // Ne sert plus car action équivallente déjà faite en fin de testPicking
            if (modeGL == aretes) click_sur_segment = true;
        } else if(event.MiddleIsDown()) { // event.RightIsDown() || dans ce cas <=> Dragging avec le bouton du Milieu
//            printf("event.MiddleIsDown x,y,mode %d %d %d\n",mouse_position.x,mouse_position.y,modeGL);
            if (!this->ToSelect.getVerrou_ListeSelect()) {  // Ne rien faire si la liste est verrouillée
//                if (mode_selection == selection_point)   m_gllist = glliste_points;
//                if (mode_selection == selection_facette) m_gllist = glliste_select;
                if ((mouse_position.x != xd_sel) || (mouse_position.y != yd_sel)) {
                    testPicking(mouse_position.x, mouse_position.y, modeGL, false) ;    // OnOff sur false => Sélectionner si glisser à la souris
//                  stopPicking();
                }
            }
            if (test_rectangle_selection) {                 /// ici pour l'instant (phase de test)
                Selection_rectangle((GLint)mouse_position.x,(GLint)mouse_position.y);   // Paramétrage de l'arrivée du rectangle de sélection
                Refresh();
            }
        } else if(event.MiddleUp()) {   // event.RightUp() ||
//            printf("event.MiddleUp\n");
            this->ToSelect.verrouiller_ListeSelect(false);  // Déverrouiller la liste : évite de recommencer une sélection tant qu'on n'a pas relaché la souris
            if (test_rectangle_selection) {                 ///ici pour l'instant (phase de test)
                xd_sel = xa_sel = (GLint)mouse_position.x;  // Donner la même valeur aux 2 extrémités => ne tracera plus le rectangle
                yd_sel = ya_sel = (GLint)mouse_position.y;
                if (mode_selection == selection_point)   m_gllist = glliste_points;
                if (mode_selection == selection_facette) m_gllist = glliste_select;
                Refresh();
            }
            if ((modeGL == aretes) && click_sur_segment) {
                buildAllLines(); /// OK si on a cliqué sur une arête, sinon inutile !
                click_sur_segment = false;
            }
        } else if (event.Dragging()) {
            if(event.LeftIsDown()) {        // Ici Dragging avec bouton gauche de la souris
                wxSize sz(GetClientSize());
                if (m_gldata.mode_Trackball) {
                    /* drag in progress, simulate trackball */
                    float spin_quat[4];
                    trackball(spin_quat,
                             (2.0*m_gldata.BeginX - sz.x) / sz.x,
                             (sz.y - 2.0*m_gldata.BeginY) / sz.y,
                             (2.0*event.GetX() - sz.x)    / sz.x,
                             (sz.y - 2.0*event.GetY())    / sz.y); // Pour augmenter la sensibilité, multiplier par 2 ou 3 chacune des 4 valeurs
                    add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
                    glGetFloatv(GL_MODELVIEW_MATRIX, *m);
                    CalculAngles(*m, m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
                } else {

                    /* Mode rotation directe : seulement 2 axes possibles, ici X et Y */
//                    printf("rot avant X Y : %f %f\n",m_gldata.rotx,m_gldata.roty);
                    // Inversion rotations en XY avec axes XY écran
                    m_gldata.rotz += (float)(event.GetX()-m_gldata.BeginX)/(float)sz.x*120.; // Amplitude totale fenêtre = 120° z <-> y. X écran -> z de la bdd
                    m_gldata.rotx += (float)(event.GetY()-m_gldata.BeginY)/(float)sz.y*120.; // a peu près compatible de Trackball.      Y écran -> x de la bdd
                    // Recadrer dans [0.,360.]
                    m_gldata.rotx = fmod(m_gldata.rotx,360.);
                    m_gldata.rotz = fmod(m_gldata.rotz,360.);
                    if (m_gldata.rotx < 0) m_gldata.rotx += 360. ;
                    if (m_gldata.rotz < 0) m_gldata.rotz += 360. ;
/*                    printf("begin X Y : %d %d\n",m_gldata.BeginX,m_gldata.BeginY);
                    printf("pos   X Y : %d %d\n",event.GetX(),event.GetY());
                    printf("rot   X Y : %f %f\n",m_gldata.rotx,m_gldata.roty);*/
                }
                Slider_x->SetValue(lround(m_gldata.rotx));
                Slider_y->SetValue(lround(m_gldata.roty));
                Slider_z->SetValue(lround(m_gldata.rotz));

                if (MPosObs->IsShown()) {
                // Même code que dans SetAngles (mais accès + direct)
                // ATTENTION : si roty != 0, les valeurs Site Azimut sont erronnées
                    val = convert_rotx_LSI();
                    MPosObs->SpinCtrl_LSI->SetValue(val);

                    val = convert_rotz_LAZ();
                    MPosObs->SpinCtrl_LAZ->SetValue(val);

                    if (lround(m_gldata.roty) == 0) MPosObs->StaticText_Warning->Hide();
                    else                            MPosObs->StaticText_Warning->Show();
                }
                if (MZoomSpec->IsShown()) {
                    val = convert_rotx_LSI();
                    MZoomSpec->SpinCtrl_LSI->SetValue(val);

                    val = convert_rotz_LAZ();
                    MZoomSpec->SpinCtrl_LAZ->SetValue(val);

                    if (lround(m_gldata.roty) == 0) MZoomSpec->StaticText_Warning->Hide();
                    else                            MZoomSpec->StaticText_Warning->Show();
                }

                /* l'orientation a changé, retracer les objets */
                Refresh();
            } else if(event.RightIsDown()) {            // Ici Dragging avec bouton droit de la souris (<=> Zoom sans molette). Utile si pas de molette !
                int signe = +1;
                if (mouse_position.y > m_gldata.BeginY) signe = -1;     // Pour repérer le sens de déplacement, seulement en Y
                bool Touche_Maj = event.ShiftDown();                    // Si Touche Maj du clavier enfoncée, Zoom plus rapide
                OnMouseZoom(event, signe, Touche_Maj);
                previous_right_drag = true;             // Marquer qu'on a fait un drag bouton droit. évite d'afficher le menu contextuel
                                                        // quand on relache ce bouton droit
            }
            m_gldata.BeginX = event.GetX();
            m_gldata.BeginY = event.GetY();

        } else {    // Tout autre évênement de souris non déjà traité ci-dessus

            if(MPanel->Bool_souder || show_points) {
                GLint hits=0;
                GLint viewport[4];
                if(MPanel->Bool_souder) modeGL = points;    // On ne passe pas en mode sélection de points si show_points seulement !
                wxSize ClientSize = this->GetSize();
                glGetIntegerv(GL_VIEWPORT, viewport);
                glSelectBuffer(BUFSIZE, selectBuffer);
                glRenderMode(GL_SELECT);
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluPickMatrix((GLdouble)mouse_position.x, (GLdouble)(viewport[3] -mouse_position.y -offset_pointeur), width_point, width_point, viewport);
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
                glInitNames();
//                showAllPoints();
                glCallList(glliste_points);
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glFlush();
                hits = glRenderMode(GL_RENDER);
                if(hits != 0) {
                    if (letscheckthemouse(0,hits)) {
//                        m_gllist = 0;                         // reconstruit toutes les listes alors que seule celle des points suffirait
                        searchMin_Max();                        // Pourquoi ? pour être sûr d'avoir les bons min-max en mémoire ?
                        if (show_points && !MPanel->Bool_souder) { // En mode show_points seulement, pas en mode soudure de points
                            if ((point_under_mouse != point_under_mouse_old) || (objet_under_mouse != objet_under_mouse_old)) {
                                if (verbose) printf("Survol du point %d de l'objet %d\n", point_under_mouse, objet_under_mouse);
                                objet_under_mouse_old = objet_under_mouse;  // Mémoriser les valeurs pour ne pas afficher plusieurs fois de suite la même chose !
                                point_under_mouse_old = point_under_mouse;
                            }
                        }
                        m_gllist = glliste_points;// Reconstruire toute la liste de points
                        if (MPanel->Bool_souder) {
                            if (ifexist_sommet(objet_under_mouse_old,point_under_mouse_old)) Objetlist[objet_under_mouse_old].Sommetlist[point_under_mouse_old].selected = false;
                            if (ifexist_sommet(objet_under_mouse    ,point_under_mouse))     Objetlist[objet_under_mouse].Sommetlist[point_under_mouse].selected = false;
                            // Plutôt que de reconstruire toute la liste de points, seuls 2 points suffisent en superposition, 1 point jaune survolé et un point rouge stocké
                            m_gllist = glliste_objets; //points;
                        }
                        Refresh();
                    }
                } else { // Pas de clic ni de point survolé
                    // Mettre l'attribut selected à false sur les 2 valeurs de points (par précaution)
//                    if (ifexist_sommet(objet_under_mouse_old,point_under_mouse_old)) Objetlist[objet_under_mouse_old].Sommetlist[point_under_mouse_old].selected = false;
//                    if (ifexist_sommet(objet_under_mouse    ,point_under_mouse))     Objetlist[objet_under_mouse].Sommetlist[point_under_mouse].selected = false;
                    objet_under_mouse_old = objet_under_mouse = -1;
                    face_under_mouse_old  = face_under_mouse  = -1;
                    point_under_mouse_old = point_under_mouse = -1;
                    line_under_mouse_old  = line_under_mouse  = -1;
                    m_gllist = glliste_objets; //points;
                    Refresh();
                }
                glMatrixMode(GL_MODELVIEW);
                stopPicking();

            } else if(MPanel->Bool_diviser) {

//                printf("OnMouse + Bool_diviser\n");
                GLint hits=0;
                GLint viewport[4];
                modeGL = aretes;
                wxSize ClientSize = this->GetSize();
                glGetIntegerv(GL_VIEWPORT, viewport);
                glSelectBuffer(BUFSIZE, selectBuffer);
                glRenderMode(GL_SELECT);
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                gluPickMatrix((GLdouble)mouse_position.x, (GLdouble)(viewport[3] -mouse_position.y -offset_pointeur), width_ligne, width_ligne, viewport);
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
                glInitNames();
//                showAllLines();           // Long a exécuter ... car recolorise 1 seul segment à chaque fois
                glCallList(glliste_lines);  // N'est-ce pas suffisant ?? plutôt que showAllLines ci-dessus ; OK tant qu'on est dans un seul objet, mais ...
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glFlush();
                hits = glRenderMode(GL_RENDER);
                if(hits != 0) {
                    if (letscheckthemouse(1,hits)) {
//                        m_gllist = glliste_lines; //0;     // On ne reconstruira que la liste des arêtes dans le Refresh() ci-dessous
                        // Il suffirait d'afficher la liste complète et d'y ajouter seulement le segment vert, pas tout recalculer !
                        searchMin_Max();
                        segment_surligne = true;
//                        Refresh();
                    } else {
                        segment_surligne = false;
                    }
                } else {
                    objet_under_mouse = -1;
                    face_under_mouse  = -1;
                    point_under_mouse = -1;
                    line_under_mouse  = -1;
                    segment_surligne  = false;
//                    Refresh();
                }
                m_gllist = glliste_objets;  // Pas besoin de regénérer la liste des lignes ici. Sera fait plus tard après clic sur la ligne.
                Refresh();
                glMatrixMode(GL_MODELVIEW);
                stopPicking();
            }
        }
    }
    event.Skip();                   /// Indispensable
}

void BddInter::OnMouseWheelMoved(wxMouseEvent& event) {
// Zoom avec la molette
    int signe = event.GetWheelRotation();
    bool Touche_Maj  = event.ShiftDown();
    OnMouseZoom(event, signe, Touche_Maj);
    m_gldata.BeginX  = event.GetX();
    m_gldata.BeginY  = event.GetY();
//    printf("Mouseweelmoved\n");
    event.Skip();                                       /// N'a pas l'air utile, mais ne semble pas gêner !
}

void BddInter::OnKeyLeftRight(wxKeyEvent& event, int signe) {
    float spin_quat[4];
    float degres;
    bool  VerrNum;
    VerrNum = false;
    if (event.GetKeyCode() == 0x0148) VerrNum = true;   // Touche 4 pavé numérique + Verr Num enfoncée
    if (event.GetKeyCode() == 0x014a) VerrNum = true;   // Touche 6   "      "     "   "   "    "

    if (event.ShiftDown() || VerrNum) {                 // Si touche Shift enfoncée (ou VerrNum true) => Rotation
        if (m_gldata.mode_Trackball) {
            trackball(spin_quat, 0., 0., signe*pas_rad, 0.);
            add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
            Reset_Sliders();
        } else {
            m_gldata.rotz += signe*pas_deg;
            degres = fmod(m_gldata.rotz, 360) ;
            if (degres < 0) degres +=360;
            m_gldata.rotz = degres;                     // Utile ou fait dans les sliders ?
            Slider_z->SetValue(lround(degres));
        }
    } else {                                            // Sinon Déplacement
        m_gldata.posx += signe*m_gldata.depl_xy;//1.0f;
    }
}

void BddInter::OnKeyUpDown(wxKeyEvent& event, int signe) {
    float spin_quat[4];
    float degres;
    bool  VerrNum;
    VerrNum = false;
    if (event.GetKeyCode() == 0x0146) VerrNum = true;   // Touche 2 pavé numérique + Verr Num enfoncée
    if (event.GetKeyCode() == 0x014c) VerrNum = true;   // Touche 8   "      "     "   "   "    "

    if (event.ShiftDown() || VerrNum) {                 // Si touche Shift enfoncée ou Verr Num true => Rotation
        if (m_gldata.mode_Trackball) {
            trackball(spin_quat, 0., 0., 0., signe*pas_rad);
            add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
            Reset_Sliders();
        } else {
            m_gldata.rotx -= signe*pas_deg;
            degres = fmod(m_gldata.rotx, 360) ;
            if (degres < 0) degres +=360;
            m_gldata.rotx = degres;                     // Utile ou fait dans les sliders ?
            Slider_x->SetValue(lround(degres));
        }
    } else {                                            // Sinon Déplacement
        m_gldata.posy += signe*m_gldata.depl_xy;//1.0f;
    }
}

void BddInter::OnKeyDown(wxKeyEvent& event) {
    float spin_quat[4];
    float q[4],a[3];
    float degres;
    int test=0;
    std::vector<int>   Numeros;
    std::vector<int>   NumerosF, NumerosJ;
    std::vector<float> xyz_point;
    float X_m, Y_m, Z_m ;
    int   Nb_Valeurs;
    int   GrpMat;
    bool  Touche_Maj;
    Face    *Facette;
    Sommet  *Sommet;
    wxString wxMessage;

    wxMouseEvent   event_mouse;
    wxCommandEvent cmd_event;
    wxKeyEvent     key_event;

    long evkey = event.GetKeyCode();    // pas de différence entre une minuscule et une majuscule à ce niveau
    if (evkey == 0) {
        return;
    }
    if(verbose) printf("Entree de BddInter::OnKeyDown\n");

// Décodage de la Touche
    switch (evkey) {

    case 0x016c:                    // Touche Verr Num
    break;

// Zoom -
    case WXK_NUMPAD_SUBTRACT:       // Touche - du Numpad ou 0x0186
    case WXK_NUMPAD_PAGEUP:         // Via la touche 9 du Numpad
    case WXK_PAGEUP:                // Touche PageUp
        Touche_Maj = true;                                      // Zoom accéléré
        if (evkey == WXK_NUMPAD_SUBTRACT) Touche_Maj = false;   // Zoom normal
        OnMouseZoom(event_mouse, 1, Touche_Maj);                // Simule un Zoom à la souris
        break;

    case 0x0036:                    // Touche 6 (ou -)
//        case '-':     // ne marche pas en fait !
        if (!event.ShiftDown()) {   // Ne retenir cette touche que si Shift NON enfoncée (<=> ignorer le 6)
            OnMouseZoom(event_mouse, +1, false);   // Signe inverse du Zoom à la souris, false => Zoom normal
        }
        break;

// Zoom +
    case WXK_NUMPAD_ADD:            // Touche + du Numpad ou 0x0184
    case WXK_NUMPAD_PAGEDOWN:       // Via la touche 3 du Numpad
    case WXK_PAGEDOWN:              // Touche PageDown
        Touche_Maj = true;                                  // Zoom accéléré
        if (evkey == WXK_NUMPAD_ADD) Touche_Maj = false;    // Zoom normal
        OnMouseZoom(event_mouse, -1, Touche_Maj);
        break;

    case 0x003d:                    // Touche + (ou = )
//        case '=':
        if (event.ShiftDown()) {    // Ne retenir cette touche que si Shift enfoncée (<=> ignorer le =)
            OnMouseZoom(event_mouse, -1, false);  // Effet OK ici, mais avec signe inverse / souris, false => Zoom normal
        }
        break;

// Déplacement ou Rotation
// Vers la droite
    case WXK_RIGHT:                 // Flèche droite ou 0x013c (défini dans defs.h, wxKeyCode : non unicode)
    case WXK_NUMPAD_RIGHT:          // Flèche droite du Numpad (si Num Lock non enfoncé) ou 0x017a
    case 0x014a:
        OnKeyLeftRight(event, +1);
        test=2;
        break;

// Vers la gauche
    case WXK_LEFT:                  // Flèche gauche ou 0x013a
    case WXK_NUMPAD_LEFT:           // Flèche gauche du Numpad (si Num Lock non enfoncé) ou 0x0178
    case 0x0148:
        OnKeyLeftRight(event, -1);
        test=2;
        break;

// Vers le haut
    case WXK_UP:                    // Flèche haut ou 0x013b
    case WXK_NUMPAD_UP:             // Flèche haut du Numpad (si Num Lock non enfoncé) ou 0x0179
    case 0x014c:
        OnKeyUpDown(event, +1);
        test=2;
        break;

// Vers le bas
    case WXK_DOWN:                  // Flèche bas ou 0x013d
    case WXK_NUMPAD_DOWN:           // Flèche bas du Numpad (si Num Lock non enfoncé) ou 0x017b
    case 0x0146:
        OnKeyUpDown(event, -1);
        test=2;
        break;

    case 'C':
// ou    case 'c':

        if (event.ShiftDown()) {    // C'est un 'C' majuscule
            MAIN_b->Toggle_Sliders(cmd_event);
            if (MAIN_b->Panel_Sliders->IsShown()) {         // Tester si le Panneau des sliders est visible
                MAIN_b->Button_Sliders->SetValue(true) ;    // Oui <=> Bouton enfoncé
            } else {
                MAIN_b->Button_Sliders->SetValue(false) ;   // Non <=> Bouton relevé
            }
            break;
        }

// Centrer la rotation sur la sélection

        X_m = Y_m = Z_m = 0.0;
        Nb_Valeurs= 0;

        if (mode_selection == selection_point) {    // En mode selection_point, ListeSelect contient un couple "objet,sommet" et non "objet,face"
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                for (unsigned int j=0; j < Objetlist[objet].Sommetlist.size(); j++) {
                    if (ToSelect.check_if_in_ListeSelect(objet,j)) {
                        xyz_point = this->Objetlist[objet].Sommetlist[j].getPoint();
                        X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                        Nb_Valeurs++;
                    }
                }
            }
        } else if (mode_selection == selection_facette) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                    if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                        Numeros = this->Objetlist[objet].Facelist[face].getF_sommets();
                        for (unsigned int j=0; j < Numeros.size(); j++) {
                            xyz_point = this->Objetlist[objet].Sommetlist[Numeros[j] -1].getPoint();
                            X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                            Nb_Valeurs++;
                        }
                    }
                }
            }
        } else if (mode_selection == selection_objet) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                if (!Objetlist[objet].selected) continue;
                for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                    Numeros = this->Objetlist[objet].Facelist[face].getF_sommets();
                    for (unsigned int j=0; j < Numeros.size(); j++) {
                        xyz_point = this->Objetlist[objet].Sommetlist[Numeros[j] -1].getPoint();
                        X_m += xyz_point[0]; Y_m += xyz_point[1]; Z_m += xyz_point[2];
                        Nb_Valeurs++;
                    }
                }
            }
        }

        if (Nb_Valeurs > 0) {    // Ne rien faire si nul !
            X_m /= Nb_Valeurs; Y_m /= Nb_Valeurs; Z_m /= Nb_Valeurs;
            centreRot = {X_m, Y_m, Z_m};    // <=> centreRot[0] = X_m ; centreRot[1] = Y_m ; centreRot[2] = Z_m;
            centrageRotAuto = false;
        } else {
            centreRot = centre_auto;
            centrageRotAuto = true;
        }
        SetPosObs(false);
        Refresh();
        break;

// Afficher les valeurs de position
    case 'D':
// ou    case 'd':
        printf("\nPosition :\n");
        printf("X   : %f\n",m_gldata.posx) ;
        printf("Y   : %f\n",m_gldata.posy) ;
        printf("Z   : %f\n",m_gldata.posz) ;
        printf("Centre de rotation :\n")   ;
        printf("X   : %f\n",centreRot[0])  ;
        printf("Y   : %f\n",centreRot[1])  ;
        printf("Z   : %f\n",centreRot[2])  ;
        printf("FoV : %f (zoom)\n",m_gldata.zoom) ;
        break;

// Permutation circulaire entre Coloriser les groupes, Coloriser les matériaux ou Pas de colorisation
    case 'G':
// ou    case 'g':
        GrpMat = 0;
        if (MAIN_b->Button_Groupes  ->GetValue()) GrpMat = 1;
        if (MAIN_b->Button_Materiaux->GetValue()) GrpMat = 2;
        if (GrpMat == 0) {
            MAIN_b->Button_Groupes->SetValue(true);         // Active la colorisation des groupes
            MAIN_b->Toggle_Groupes(cmd_event);
        } else {
            if (GrpMat == 1) {
                MAIN_b->Button_Materiaux->SetValue(true);   // Active la colorisation des Matériaux
            } else {
                MAIN_b->Button_Materiaux->SetValue(false);  // Désactive toute colorisation
            }
            MAIN_b->Toggle_Materiaux(cmd_event);
        }
        break;

// Inverser les normales (aux barycentres) + sens de parcours des sommets de facettes sélectionnées
    case 'I':
// ou    case 'i':
        Inverse_Selected_Normales();
        if (Raz_Selection_F) {
            SELECTION old  = mode_selection;
            mode_selection = selection_facette;
            key_event.m_keyCode = 'S';
            OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection des facettes
            mode_selection = old;
            m_gllist = 0;           // Regénérer toutes les listes
            Refresh();
            break;
        }
//        m_gllist = glliste_select; // Initialisé dans Inverse_Selected_Normales (2 valeurs possibles)
        Refresh();
        break;

// Afficher la Matrice et les angles
    case 'M':
// ou    case 'm':

        if (event.ShiftDown()) {    // C'est un 'M' majuscule
            MAIN_b->Toggle_Gouraud(cmd_event);
            MAIN_b->Button_Gouraud->SetValue(smooth);
            break;
        }

        GLfloat m[4][4];            // C'est un m minuscule => affiche la matrice

        glGetFloatv(GL_MODELVIEW_MATRIX, *m);
        printf("Matrice GL 2D:\n");
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][0],m[1][0],m[2][0],m[3][0]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][1],m[1][1],m[2][1],m[3][1]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][2],m[1][2],m[2][2],m[3][2]) ;
        printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][3],m[1][3],m[2][3],m[3][3]) ;
        glGetFloatv(GL_MODELVIEW_MATRIX, *m);
        CalculAngles(&m[0][0],alpha,beta,gamma);
        printf("alpha %8.3f deg.\n",alpha);
        printf("beta  %8.3f deg.\n",beta);
        printf("gamma %8.3f deg.\n",gamma);
        break;

// Raccourci Clavier pour "Sens des normales"
    case 'N':
// ou    case 'n';
        show_CW_CCW = !show_CW_CCW;
        MAIN_b->Menu_SensDesNormales->Check(show_CW_CCW);   // Cocher/décocher le menu correspondant
        m_gllist = 0;
        Refresh();
        break;

// Inverser seulement le sens de parcours des sommets de facettes sélectionnées
    case 'P':
// ou    case 'p':
        Inverser_Parcours_Selected();
        if (Raz_Selection_F) {
            SELECTION old  = mode_selection;
            mode_selection = selection_facette;
            key_event.m_keyCode = 'S';
            OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection des facettes
            mode_selection = old;
            m_gllist = 0;           // Regénérer toutes les listes
            Refresh();
            break;  /// m_gllist = 0; et Refresh() déjà fait via 'S' ???
        }
        if (show_CW_CCW) {
            m_gllist = 0;           // Utile si Sens des normales est activé. Sans effet visuel sinon
            Refresh();
        }
        break;

// Reset (valeurs d'initialisation)
    case 'R':
// ou    case 'r':
        ResetData();
        test=1;
        break;

// Reset de la sélection d'objets, de facettes ou de points
    case 'S':
// ou    case 's':
        if (mode_selection == selection_objet) {
            for (unsigned int objet=0; objet < Objetlist.size(); objet++) Objetlist[objet].selected = false;
            listeObjets.clear();
        }
        // Les élements de ToSelect.Liste et ToSelect.ListeSelect vont être remis aux valeurs par défaut indépendamment du fait que ces listes soient des facettes ou des sommets
        while (this->ToSelect.ListeSelect.size() > 0) { // Faire tant que la liste ToSelect.ListeSelect n'est pas vide ...
            // On récupère le premier élément de la liste et on remet les valeurs de selected à false (par défaut)
            int objet = ToSelect.ListeSelect[0].objet;
            int numero= ToSelect.ListeSelect[0].face_sommet ;
            if (mode_selection == selection_point) {
                if (ifexist_sommet(objet,numero)) {
                    Sommet = &(this->Objetlist[objet].Sommetlist[numero]);
                    Sommet->selected = false;                               // si numero peut correspondre à un sommet, mettre selected à false
                }
            }
            if (mode_selection == selection_facette) {
                if (ifexist_facette(objet,numero)) {
                    Facette = &(this->Objetlist[objet].Facelist[numero]);
                    Facette->selected = false;                              // si numero peut correspondre à une facette, mettre selected à false, et reset de la couleur de la facette
                    Facette->color[0] = Facette->color[1] = Facette->color[2] = gris_def;   // Ne sert plus ?
                }
            }
            ToSelect.ListeSelect.erase(ToSelect.ListeSelect.begin());   // On supprime le premier élément de la liste => la liste se décale d'un cran.
        }

        if (MSelect->IsShown()) {
            wxString str_reset;
            str_reset.clear();
            MSelect->TextCtrl_NomObjet   ->ChangeValue(str_reset);
            MSelect->TextCtrl_NumObjet   ->SetValue(str_reset);
            MSelect->TextCtrl_IndObjet   ->SetValue(str_reset);
            MSelect->TextCtrl_Selection  ->SetValue(str_reset);
            MSelect->TextCtrl_NumFacette ->SetValue(str_reset);
            MSelect->TextCtrl_IndFacette ->SetValue(str_reset);
            MSelect->TextCtrl_NumGroupe  ->SetValue(str_reset);
            MSelect->TextCtrl_NumMateriau->SetValue(str_reset);
            MSelect->Button_Fusionner    ->Disable();
            MSelect->RadioBox_TypeSelection->SetSelection(0);
        }

        if (mode_selection != selection_objet) {
            buildAllFacettesSelected(); // Va supprimer la liste des facettes sélectionnées si elle existe
            buildAllPoints();           // Idem pour les points
            m_gllist = glliste_objets;  // suffisant ici ? pas de regénération de points, facettes ou lignes.
        } else
            m_gllist = 0;               // on est en mode selection_objet
        Refresh();
        break;

// Changer de mode Trackball <-> Direct
    case 'T':
// ou    case 't':
        m_gldata.mode_Trackball = 1 - m_gldata.mode_Trackball ;
        MPrefs->RadioBox_Trackball->SetSelection(m_gldata.mode_Trackball) ; // Pour changer le mode affiché dans le panneau Préférences (si présent à l'écran)
        if (m_gldata.mode_Trackball) {
            printf("Mode Trackball\n");

            CalculMatriceRot();

        } else {
            printf("Mode Rotation directe\n");
            GLfloat m[4][4];

            // Calcul des rotations à partir de la matrice MODELVIEW
            glGetFloatv(GL_MODELVIEW_MATRIX, *m);
            CalculAngles(*m, m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
 //           printf("Angles xyz : %f %f %f\n",m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
        }
        test=1; // Forcer un Refresh (ne devrait rien changer sur l'écran ....)
        break;

// Switch du mode verbeux
    case 'V':
// ou    case 'v':
        verbose = !verbose ;
        printf("Mode bavard : %s\n",verbose ? "true" : "false") ;
        break;

// Étendre la sélection de facettes aux facettes voisines et de normales compatibles
    case 'X':
// ou    case 'x':

        if (mode_selection == selection_point) break;   // X ne s'applique pas au mode de sélection des sommets

//        printf("On y va\n");
        wxBeginBusyCursor();                                               // Affiche un curseur animé par défaut (pour faire patienter !)
        bool extend_auto;
        int sz_numF, sz_numJ;

        extend_auto = false;                                               // Initialiser le marqueur d'ajouts de facettes
        // Du fait des boucles imbriquées ce test peut-être long (et même très long sur de grosses Bdds)
        for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
            for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                if (ToSelect.check_if_in_ListeSelect(objet,face)) {
//                    printf("objet : %d, facette : %d\n",objet,face);
                    NumerosF = this->Objetlist[objet].Facelist[face].getF_sommets();            // Récupérer les numéros de sommets de la facette "face" de l'objet "objet"
                    sz_numF  = NumerosF.size();
                    for (unsigned int j=0; j<this->Objetlist[objet].Facelist.size(); j++) {     // Commencer directement à face+1 plutôt que 0 ?
                        if (j == face) continue;                                                // Passer directement au suivant (inclus dans le test ci-dessous, mais + rapide ?)
                        if (ToSelect.check_if_in_ListeSelect(objet,j)) continue ;               // Si la facette j déjà dans la liste, passer à la suivante
                        NumerosJ = this->Objetlist[objet].Facelist[j].getF_sommets();           // Récupérer les numéros de sommets de la facette "j" de l'objet "objet"
                        sz_numJ  = NumerosJ.size();
                        for (int nn=0; nn<sz_numF; nn++) {
                            int Numeros_nn = NumerosF[nn];
                            for (int nj=0 ; nj<sz_numJ ; nj++) {
                                if (Numeros_nn != NumerosJ[nj]) continue ;      // Ne faire la suite (comparaison des normales) que si 1 point commun entre les facettes j et face
                                if (compare_normales(objet,face,j)) {           // Test sur les normales
//                                   printf("OK  %d\n",j);
                                    if (!ToSelect.check_if_in_ListeSelect(objet,j)) {   // N'ajouter que si non déjà présent dans la Liste
                                        ToSelect.add_one(objet,j);
                                        extend_auto = true;                             // Marquer qu'on a ajouté une facette dans la liste
                                        this->Objetlist[objet].Facelist[j].selected = true;
//                                            colorface(objet,j);   // Maintenant <=> ligne ci-dessus
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        str.Printf(_T("%d"),(int)this->ToSelect.ListeSelect.size());
        MSelect->TextCtrl_Selection->SetValue(str); // Peut se faire dans tous les cas, même si SelectionPanel non affiché (MSelect->IsShown() est false)
        m_gllist = glliste_select; // 0;
        Refresh();
//        printf("C'est fini\n");
        wxEndBusyCursor();                                                  // Supprime le curseur animé... C'est terminé !

        if (!extend_auto) {                                                 // La sélection n'a pas été modifiée : afficher un message
//            printf("Extension automatique : limite atteinte\n");
            if (ToSelect.ListeSelect.size() == 0)
                wxMessage = _T("La sélection de facettes est vide. Pas d'extension possible");
            else {
                wxMessage = _T("L'extension automatique de la sélection a atteind sa limite :\n");
                wxMessage+= _T("pas ou plus de points communs avec les facettes voisines");
            }
            DisplayMessage(wxMessage,false);
        }
        break;

// Supprimer les facettes sélectionnées
    case WXK_DELETE :

        if (mode_selection == selection_point) break;   // Delete ne s'applique pas au mode de sélection des sommets

        if (mode_selection == selection_objet) {
            auto it = listeObjets.begin();
            for (unsigned int i=0; i<listeObjets.size(); i++, it++) {
                Objetlist[*it].deleted = true;
            }
            if (MSelObj->IsShown()) {                   // Mettre à jour la fenêtre de Sélection Manuelle d'objets si elle est affichée
                unsigned int i,n,indice_ListBox;
                wxString str_loc;
                n = MSelObj->CheckListBox1->GetCount(); // Récupère le nombre d'items actuels
                for (i=0 ; i<n ; i++)
                    MSelObj->CheckListBox1->Delete(0);  // Supprime ces "anciens" items

                indice_ListBox = 0; // Pour découpler l'indice d'un objet de celui de la CheckListBox (en cas d'objets supprimés)
                for (i=0; i<Objetlist.size(); i++) {
                    if (!Objetlist[i].deleted) {
                        str_loc = wxString(Objetlist[i].GetName(), wxConvUTF8);
                        MSelObj->CheckListBox1->Append(str_loc);                                // Recrée tous les items (des objets non supprimés)
                        MSelObj->CheckListBox1->Check(indice_ListBox,Objetlist[i].selected);    // Coche ceux qui sont déjà sélectionnés
                        indice_ListBox++;
                    }
                }
            }
            m_gllist = 0;
            Refresh();
            break;
        }

        if (mode_selection == selection_facette) {
            do {
    //            printf("\ntouche S : \nsize avant: %d\n",this->ToSelect.ListeSelect.size());
                for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                    for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                        if (ToSelect.check_if_in_ListeSelect(objet,face)) {
        //                    printf("%d %d %d\n",ToSelect.check_if_in_ListeSelect(objet,face),objet,face);
                            Facette = &(this->Objetlist[objet].Facelist[face]);
                            Facette->selected = false;
//                            Facette->color[0] = Facette->color[1] = Facette->color[2] = gris_def;     // Ne sert plus ?
                            Facette->deleted  = true;
                            Elements_Supprimes= true;              // Au moins une facette supprimée
                            ToSelect.erase_one_ListeSelect(objet,face);
                        }
                    }
                }
    //            printf("size apres: %d\n",this->ToSelect.ListeSelect.size());     // Effet escompté mais la Liste n'est pas forcément vide !
            } while (this->ToSelect.ListeSelect.size() > 0);                        // Donc, on boucle ... jusqu'à ce que ...
            m_gllist = 0;
            Refresh();
        }
        break;

        // Masquer les facettes sélectionnées
    case WXK_NUMPAD_DELETE :

        if (mode_selection == selection_point) break;   // Masquer ne s'applique pas au mode de sélection des sommets

        if (mode_selection == selection_objet) {
            auto it = listeObjets.begin();
            for (unsigned int i=0; i<listeObjets.size(); i++, it++) {
                Objetlist[*it].afficher = false;
            }
            m_gllist = 0;
            Refresh();
            break;
        }

        if (mode_selection == selection_facette) {
            do {
    //            printf("\ntouche S : \nsize avant: %d\n",this->ToSelect.ListeSelect.size());
                for (unsigned int objet=0; objet < Objetlist.size(); objet++) {
                    for (unsigned int face=0; face < Objetlist[objet].Facelist.size(); face++) {
                        if (ToSelect.check_if_in_ListeSelect(objet,face)) {
                            Facette = &(this->Objetlist[objet].Facelist[face]);
        //                    printf("%d %d %d\n",ToSelect.check_if_in_ListeSelect(objet,face),objet,face);
                            Facette->selected = false;
//                            Facette->color[0] = Facette->color[1] = Facette->color[2] = gris_def; // Ne sert plus ?
                            Facette->afficher = false;
                            Elements_Masques  = true;               // Au moins une facette masquée
                            ToSelect.erase_one_ListeSelect(objet,face);
                        }
                    }
                }
    //            printf("size apres: %d\n",this->ToSelect.ListeSelect.size());     // Effet escompté mais la Liste n'est pas forcément vide !
            } while (this->ToSelect.ListeSelect.size() > 0);                        // Donc, on boucle ... jusqu'à ce que ...
            m_gllist = 0;
            Refresh();
        }
        break;

// Quitter par les touches Q ou Escape (en plus de Alt-X déjà programmé et les icônes X de fermeture)
    case 'Q':
    case WXK_ESCAPE:
        wxTheApp->ExitMainLoop();
        break;

    case 'A':                                   // Bascule du mode Antialiasing OpenGL
        antialiasing_soft = !antialiasing_soft;
        if (MPrefs->IsShown()) {
            MPrefs->CheckBox_AntialiasingSoft->SetValue(antialiasing_soft); // Modifier la case à cocher si le dialogue Préférences est affiché
        }
        Refresh();
        ini_file_modified = true;               // Enregistrer ce changement dans le fichier init
        break;

    case 'Z':
        Raz_Selection_F   = !Raz_Selection_F; // Inverse le mode de RAZ de sélection de facettes utilisé après une inversion de normales pour garder ou remettre à 0 la sélection
        ini_file_modified = true;             // Enregistrer ce changement dans le fichier init
        break ;

    case 'J':
        // Touche J pour divers tests provisoires ... A supprimer donc !
        test_rectangle_selection = !test_rectangle_selection;
        printf("Mode Test ");
        if (test_rectangle_selection)   printf("on\n");
        else                            printf("off\n");
//        Refresh();
        break;

// Touche non reconnue : rien de spécial à faire (sauf l'afficher)
    default:
        if (evkey != 0x0132 && evkey != 0x0133 && evkey != 0x0134 ) { // on passe si Shift, Alt ou Ctrl
            printf("code : %3d, %c, %#06x\n", (int)evkey,char(evkey),(int)evkey);
        }
        break;
    }

    if (test != 0) { // Quelque chose à changé (rotation, déplacement, Zoom). Il faut redessiner la fenêtre OpenGL
        if (test == 1) {
            ResetProjectionMode();
        }
        if (test == 2) {
            if (MPosObs->IsShown()) {
//                    printf("Pos Obs est affiche\n");
            // Même code que dans SetAngles (mais accès + direct)
            // ATTENTION : si roty != 0, les valeurs Site Azimut sont erronnées
                int val = convert_rotx_LSI() ;
                MPosObs->SpinCtrl_LSI->SetValue(val);

                val = convert_rotz_LAZ();
                MPosObs->SpinCtrl_LAZ->SetValue(val);
            }
            if (MZoomSpec->IsShown()) {
                int val = convert_rotx_LSI();
                MZoomSpec->SpinCtrl_LSI->SetValue(val);

                val = convert_rotz_LAZ();
                MZoomSpec->SpinCtrl_LAZ->SetValue(val);
            }

        }
        Refresh(false);
    }

///    event.Skip();   // Avec ou sans ne semble rien changer !!!
    if(verbose) printf("Sortie de BddInter::OnKeyDown\n");
}

void BddInter::Reset_Sliders() {
// Calcul et affichage de la position des sliders pour un quaternion donné dans m_gldata.quat
    build_rotmatrix(matquat, m_gldata.quat);
    CalculAngles(*matquat, m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
    Slider_x->SetValue(lround(m_gldata.rotx));
    Slider_y->SetValue(lround(m_gldata.roty));
    Slider_z->SetValue(lround(m_gldata.rotz));
}

void BddInter::ResetProjectionMode() {
    if (verbose) printf("Entree ResetProjectionMode\n");

    wxSize ClientSize = this->GetParent()->GetSize();
//    wxSize ClientSize_bis = this->GetSize();

    if (verbose) printf("ClientSize X/Y %d %d\n", ClientSize.GetX(), ClientSize.GetY());
//    printf("ClientSize bis X/Y %d %d\n", ClientSize_bis.x, ClientSize_bis.y);

//    if ((ClientSize.x == 800) && (ClientSize_bis.x != 1)) ClientSize = ClientSize_bis ; // si ClientSize_bis.x = 1 garder ClientSize

//    wxSize ClientSize = this->GetSize();
    this->SetSize(ClientSize.GetX(),ClientSize.GetY());
    glViewport(0, 0, ClientSize.GetX(), ClientSize.GetY());
//    printf("ClientSize ter X/Y %d %d\n", ClientSize.GetX(), ClientSize.GetY());

//    OvniFrame* MAIN=dynamic_cast<OvniFrame*>(this->GetParent());
//    MAIN->ResizeOpenGL(ClientSize.GetX(), ClientSize.GetY());

///    int w, h;
///    GetClientSize(&w, &h);
//    printf("ClientSize X/Y %d %d\n", w, h);

#if wxCHECK_VERSION(3,0,0)
    {
        SetCurrent(*m_glRC);
#else
#ifndef __WXMOTIF__
    if ( GetContext() )
#endif
    {
        SetCurrent();
#endif // wxCHECK_VERSION

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);   /// ici .x et .y, pas .GetX et .GetY ? à vérifier peut-être
        glMatrixMode(GL_MODELVIEW);

   }
    if (verbose) printf("Sortie ResetProjectionMode\n");
}

void BddInter::CalculAngles(float *m, float &alpha, float &beta, float &gamma) {

// D'après Matrix and Quaternions (http://www.flipcode.com/documents/matrfaq.html, Question 37)

    //GLfloat m[16];
//    float torad= M_PI/180.0 ;
    /*    printf("Matrice GL 1D:\n");
                printf("%8.3f %8.3f %8.3f %8.3f\n", m[0],m[4],m[8], m[12]) ;
                printf("%8.3f %8.3f %8.3f %8.3f\n", m[1],m[5],m[9], m[13]) ;
                printf("%8.3f %8.3f %8.3f %8.3f\n", m[2],m[6],m[10],m[14]) ;
                printf("%8.3f %8.3f %8.3f %8.3f\n", m[3],m[7],m[11],m[15]) ;*/

    double sin_beta, cos_beta ;

    sin_beta = m[8];
///    sin_beta = -m[2];
    beta = asin(sin_beta)*to_Deg;                       // suffisant car -90 <= beta <= 90°
    cos_beta = sqrt(1.0f-sin_beta*sin_beta);            // cos_beta est donc toujours >= 0

    if (fabs(cos_beta) > 0.0005f) {                     // En fait <> 0. mais ici avec une certaine tolérance
        // Signes changés / article ... + au lieu de -
        alpha=atan2(-m[9],m[10])*to_Deg;                    // en fait -m[9]/cos_beta et m[10]/cos_beta), mais <=> car cos_beta > 0
        gamma=atan2(-m[4],m[0] )*to_Deg;                    // Idem
///        alpha=atan2(+m[6],m[10])*to_Deg;                 // en fait -m[6]/cos_beta et m[10]/cos_beta), mais <=> car cos_beta > 0
///        gamma=atan2(+m[1],m[0] )*to_Deg;                 // Idem
    } else {                                            // ici sin(beta) = 1 ou -1 (à epsilon près)
        beta =( sin_beta < 0.0f ? -90.0f : +90.0f);         // Forcer la valeur de beta
        alpha=0.0f;                                         // Cas dégénéré (Gimball lock => alpha et gamma sur le même axe)=> cos(alpha)=1.
        gamma=atan2( m[1],m[5])*to_Deg;                     // Signe changé / article ... - au lieu de +
///        gamma=atan2(-m[4],m[5])*to_Deg;
    }
    Clamp(alpha,0.0f,360.0f);                           // Recadrer les angles dans l'intervalle [0,360°[
    Clamp(beta, 0.0f,360.0f);                           // Vu l'usage fait ici, un fmod avec décalage de 360 si valeurs négatives doit être suffisant.
    Clamp(gamma,0.0f,360.0f);
}

void BddInter::Clamp(float &angle, const float vmin, const float vmax) {
// Recadre les valeurs de angle dans l'intervalle [vmin, vmax[
// ATTENTION : si vmin vmax mal entré, peut boucler à l'infini....
    float ampl = vmax-vmin;
    while(angle <  vmin) {
        angle += ampl;
    }
    while(angle >= vmax) {
        angle -= ampl;
    }
}

wxString BddInter::get_file() {
    return this->file;
}

wxString BddInter::get_firstFile() {
    return this->first_file;
}

void BddInter::set_file(wxString file) {
    this->file=file;
}

void BddInter::set_firstFile(wxString file) {
    this->first_file=file;
}

void BddInter::clearall() {

    if(verbose) printf("Entree de BddInter::clearall\n");
    Objetlist.clear();
    wxStringlist.clear();

    type=1;                         // <=> .bdd
    reset_zoom = true;
    m_loaded   = false;
    diagonale_save=0.0;
    glDeleteLists( 1, 100);   // ?? pourquoi 100 et pas plus ? que se passe t-il si plus de 100 listes ? 6 doit être suffisant en fait (glliste_* de 1 à 6)
    glClear(GL_COLOR_BUFFER_BIT);
    m_gldata.initialized = false;
}

void BddInter::create_bdd() {

    unsigned int indice_premierObjet;   // indice du premier objet de la nouvelle Bdd à charger

    if(verbose) printf("Entree de BddInter::create_bdd\n");

    if (Numero_base == 0) {                                             // Ouvrir ou réouvrir un fichier
        clearall();                                                         // RAZ global
        wxNomsFichiers = wxFileNameFromPath(get_file());                    // N'extraire que le nom du fichier (pas le path !)
    } else {                                                            // Fusion avec la bdd déjà en mémoire
        Numero_base = this->Objetlist.rbegin()->GetValue();                 // Retourne la "value" du dernier objet de la liste
        Numero_base = ((Numero_base + 10)/10)*10;                           // Pour calculer un offset de numérotation et commencer à la dizaine supérieure
        wxNomsFichiers += _T(" + ") + wxFileNameFromPath(get_file());       // Concaténer avec le nom du/des fichier(s) précédent(s)
    }
    indice_premierObjet = this->Objetlist.size();                       // Utile pour les fusions
    wxString str_nom = this->file;
    str_nom.MakeLower();    // Force le nom à être en minuscules (simplifie les tests de détection d'extension ci-dessous)

    wxCharBuffer buffer=str_nom.mb_str();
    printf("Chargement de : %s\n",buffer.data());
    if(str_nom.EndsWith(_T(".bdd"))) {
        type=1;
    } else if(str_nom.EndsWith(_T(".3ds"))) {
        type=2;
    } else if(str_nom.EndsWith(_T(".g3d"))) {
        type=3;
    } else if(str_nom.EndsWith(_T(".obj"))) {
        type=4;
    } else if(str_nom.EndsWith(_T(".ply"))) {
        type=5;
    } else if(str_nom.EndsWith(_T(".off"))) {
        type=6;
    } else if(str_nom.EndsWith(_T(".m3d"))) {
        type=7;
    } else if(str_nom.EndsWith(_T(".stl"))) {
        type=8;
    } else if(str_nom.EndsWith(_T(".dxf"))) {
        type=0;
    }
    printf("create_bdd : type=%d\n",type);

    wxFileInputStream stream(str_nom);
    mtllib_OK = false;                      // Ne sert que si une ligne mtllib existe dans un fichier .obj
    if(type != -1) {
        switch(type) {
        case 0:
            printf("\nChargement d'un fichier .dxf !!!\n");
            printf("ATTENTION : pour le moment, le support des fichiers dxf n'est que partiel !\n");
            printf("            Visuel uniquement, notamment, pas de sauvegardes en format SDM .bdd\n");
            m_renderer.Load(stream);            // lecture incompatible de BddInter !!
            this->m_loaded = m_renderer.IsOk(); //true; // Récupère le m_loaded interne à m_renderer
            this->m_gldata.zoom = 4;            // ces valeurs sont compatibles de penguin.dxf ! Pour généraliser, il faut récupérer des valeurs comme min et max en x, y et z
            this->m_gldata.zoom_step = 0.5;
            this->m_gldata.posz = -150.;
            this->m_gldata.zNear= abs(this->m_gldata.posz) -10.;    // Objet recadré sur -5,+5 dans DXFRenderer::NormalizeEntities
            this->m_gldata.zFar = abs(this->m_gldata.posz) +10.;
            this->m_gldata.posx = 0.;
            this->m_gldata.posy = 0.;
            centre_auto = {0.0f, 0.0f, 0.0f};   // <=> centre_auto[0] = centre_auto[1] = centre_auto[2] = 0.0;
            centreRot = centre_auto;
            diagonale_save = 10.*sqrt(3.);
            x_min = y_min = z_min = -5.;
            x_max = y_max = z_max = +5.;
            this->Refresh(true);
            this->SetFocus();
            this->finishdraw=true;
            type_new = type;
            break;
        case 1:
            printf("\nChargement d'un fichier .bdd !!!\n");
            this->LoadBDD();
//            printf("Nb groupes   : %d\n",listeGroupes.size());
//            for (int n : listeGroupes) std::cout << n << ' '; std::cout << '\n' ;
//            printf("Nb materiaux : %d\n",listeMateriaux.size());
//            for (int n : listeMateriaux) std::cout << n << ' '; std::cout << '\n' ;
            break;
        case 2:
            printf("\nChargement d'un fichier Autodesk .3ds !!! \n");
            this->Load3DS();
            break;
        case 3:
            printf("\nChargement d'un fichier Xml .g3d !!! \n");
            this->LoadG3D();
            break;
        case 4:
            printf("\nChargement d'un fichier Wavefront .obj !!! \n");
            this->LoadOBJ();
            break;
        case 5:
            printf("\nChargement d'un fichier .ply Niratam ou Stanford Polygon File !!! \n");
            this->LoadPLY();
            break;
        case 6:
            printf("\nChargement d'un fichier .off !!! \n");
            this->LoadOFF();
            break;
        case 7:
            printf("\nChargement d'un fichier .m3d !!! \n");
            this->LoadM3D();
            break;
        case 8:
            printf("\nChargement d'un fichier .stl !!! \n");
            this->LoadSTL();
            break;
        }
    }
    if (type > 0) {
        type_new = 1;
        bdd_modifiee = false;   // Pas sûr que ce soit ici le meilleur endroit, surtout si on fusionne des bdd !

        bool Normales_sommets_presentes=false;
        unsigned int i;
        for(i=indice_premierObjet; i<this->Objetlist.size(); ++i) {
            if ((this->Objetlist[i].Nb_vecteurs != 0) && (this->Objetlist[i].Nb_luminances != 0)) Normales_sommets_presentes = true;
            GenereTableauPointsFacettes(&this->Objetlist[i]);
            GenereTableauAretes(&this->Objetlist[i]);
        }

        this->searchMin_Max();

// Etablir une liste de numéros de groupes et de matériaux
        listeGroupes.clear();       // à faire ailleurs ? (surtout si fusion de plusieurs Bdds)
        listeMateriaux.clear();
        listeObjets.clear();
        listePoints.clear();
        for(i=0; i<this->Objetlist.size(); ++i) {
            GenereListeGroupesMateriaux(i);
        }
        listeGroupes.sort();                                            // Trier les listes
        listeMateriaux.sort();
        printf("Liste des groupes   : ");
//        std::list<int>::iterator it = listeGroupes.begin();
        auto it = listeGroupes.begin();
        for (i=0; i<listeGroupes.size();   i++) {
            printf("%3d ", *it++); // ou it = listeGroupes.begin(); std::advance(it, i); ou encore std::list<int>::iterator it = std::next(listeGroupes.begin(), i);
        }
        printf("\n");
        it = listeMateriaux.begin();
        sprintf(Message,"Liste des matériaux : ");
        printf(utf8_To_ibm(Message));
        for (i=0; i<listeMateriaux.size(); i++,it++) {
            printf("%3d ", *it);
        }
        printf("\n");

// Ici on force le calcul des normales aux sommets même si elles sont données dans le fichier, mais après confirmation

        if (CalculNormalesLectureBdd) {
            bool Forcer_calcul=false;
            for(i=indice_premierObjet; i<this->Objetlist.size(); ++i) {
                if (Normales_sommets_presentes) {
                    if (i == 0) {
                        wxString wxMessage = _T("Les Vecteurs et Luminances sont déjà présents dans la BDD.\n");
                        wxMessage         += _T("Le calcul refait à la lecture du fichier va les remplacer...\n");
                        wxMessage         += _T("Est-ce bien ce que vous voulez ?");
                        wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Question"),
                                                 wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
                        if (query->ShowModal() == wxID_YES) Forcer_calcul = true;
                        query->Destroy();
                    }
                }
                else Forcer_calcul = true;
                if (Forcer_calcul) {
                    indiceObjet_courant = i;
                    unsigned int nb_p = this->Objetlist[i].Nb_sommets;
                    this->N_elements = nb_p;
                    this->str.clear();
                    makevecteur();

                    unsigned int nb_fac = this->Objetlist[i].Nb_facettes;
                    this->N_elements = nb_fac;
                    makeluminance();

                    // Recopie des numéros de sommets des facettes dans luminances
                    for (unsigned int nfac=0; nfac < nb_fac ; nfac++) {
                        NumerosSommets   = this->Objetlist[i].Facelist[nfac].getF_sommets();
                        this->N_elements = this->Objetlist[i].Facelist[nfac].getNumero();
                        make1luminance();
                        this->Objetlist[i].Facelist[nfac].flat = false ;
                    }

                    GenereNormalesAuxSommets(i, nb_p);
                    this->Objetlist[i].flat=false;
                }
            }
        }
    }
//    this->Refresh(true);  // Sert à quoi ?
    if(verbose) printf("Sortie de BddInter::create_bdd\n");
}

//****************************************************************************
static void XMLCALL start_XML_Element(void *data, const char *el, const char **attr)
{
    int i,n,i1;
    unsigned int tmp, tmp2 ;
    int o;
    static unsigned int ind_xyz, ind_fac ;
    BddInter *Element=(BddInter *)data;
    float vx,vy,vz;
    char *cptr;
//    int valp[500];  // Attention, stockage provisoire limité à 500 valeurs (nb sommets ou normales / facette)
    int  val_id=0 ;
    static int nb_val_id=0;
    char sc[100];
    int  nc ;
    static float vxyz[3];
    wxString wxNumeros;

    Face   *Face_ij;
    Object *Objet_courant;
    std::vector<int> Numeros_L;

// Profondeur et nombre d'attributs détectés
/*  printf("Depth=%2d",XML_Depth) ;
    for (i = 0; attr[i]; i += 2) {
    }
    printf(", nb_atts=%2d ",i/2);
// *****************************************

    for (i = 0; i < XML_Depth; i++)
        printf("  ");

    printf("%s", el); */

    o = i_objetXML_courant;

    for (i = 0; attr[i]; i += 2) {
//    printf(" %s='%s'", attr[i], attr[i + 1]);

        if (XML_Depth == 1) {
            if (!strcmp(el,"objets")) {
                if (!strcmp(attr[i],"nbr")) {
                    o = atoi(attr[i+1]);
                    printf("Nb objets : attribut nbr= %d\n",o);
//                    o = i_objetXML_courant = -1;
                }
            } else if(!strcmp(el,"types_valeurs")) {
                if (!strcmp(attr[i],"nbr")) {
                    nb_val_id = atoi(attr[i+1]);
                }
            }
        } else if (XML_Depth == 2) {
            if (!strcmp(el,"objet")) {
                if (i == 0) {
                    Element->str = _T("<OBJET> 0 Test_Init"); // Donner un numéro et un nom provisoire
                    Element->makeobjet();
                    // Initialisations pour un objet
//                    o=++i_objetXML_courant ;
                    o = i_objetXML_courant = Element->indiceObjet_courant; //Objetlist.size() -1;
                    Objet_courant = &(Element->Objetlist[o]);
                    Objet_courant->afficher = true;
                    Objet_courant->deleted  = false;
                    Objet_courant->flat     = true;
//                    Element->indiceObjet_courant   = o;
                }
                if (!strcmp(attr[i],"id")) {
                    Element->Objetlist[o].SetValue(atoi(attr[i+1])+Element->Numero_base);   // En cas de fusion, on ajoute Numero_base en offset
                    printf("Indice objet : %d balise id+offset : %d\n",i_objetXML_courant,Element->Objetlist[o].GetValue()); // atoi(attr[i+1])+Element->Numero_base
                }
                if (!strcmp(attr[i],"nom")) {

#if wxCHECK_VERSION(3,0,0)
                    wxString wxNom = wxString(attr[i+1]);
#else
                    wxString wxNom = wxString::FromAscii(attr[i+1]);
#endif // wxCHECK_VERSION

                    Element->Objetlist[o].SetName(wxNom);
                    printf("Indice objet : %d Nom lu dans la balise nom : %s\n",i_objetXML_courant,attr[i+1]);
                }
            } else if(!strcmp(el,"type_materiau")) {
                if (!strcmp(attr[i],"id")) {
                    val_id = atoi(attr[i+1]);
                } else if (!strcmp(attr[i],"nom")) { // pour OVNI, seul le matériau "optique" est reconnu. Le matériau thermique est ignoré
                    if (!strcmp(attr[i+1],"optique")  ) codemateriau= val_id; // n° d'Identification du mot clé matériau optique
//                    if (!strcmp(attr[i+1],"thermique")) codemateriau= val_id; // idem pour le numéro de matériau thermique
                }
            } else if(!strcmp(el,"type_valeur")) {
                if (!strcmp(attr[i],"id")) {
                    val_id = atoi(attr[i+1]);
                } else if (!strcmp(attr[i],"nom")) { // pour OVNI, actuellement, tous les mots clés sont équivallents à "groupe". codmatface devrait être <=> codemateriau mais conflit ?
                    if (!strcmp(attr[i+1],"groupe")    ) codegroupe = val_id; // n° d'Identification du mot clé groupe
                    if (!strcmp(attr[i+1],"codmatface")) codegroupe = val_id; // idem pour le numéro de matériau
                    if (!strcmp(attr[i+1],"code Crira")) codegroupe = val_id; // idem pour le numéro de matériau
                    printf("type_valeur id:%d/%d nom:%s\n",val_id,nb_val_id,attr[i+1]);
                }
            }
        } else if (XML_Depth == 5) {
            Objet_courant = &(Element->Objetlist[o]);
            if (!strcmp(el,"sommets")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_sommets = tmp;
//                    printf("Indice objet : %d, sommets   , balise nbr :%d\n",o,tmp);
                    // Création du tableau des points
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->makesommet();
                }
            } else if (!strcmp(el,"normales_s")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_vecteurs = tmp;
//                    printf("Indice objet : %d, normales_s, balise nbr :%d\n",o,tmp);
                    // Création du tableau des normales aux sommets
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->makevecteur();
                }
            } else if (!strcmp(el,"facettes")) {
                if (!strcmp(attr[i],"nbr")) {
                    tmp = atoi(attr[i+1]);
                    Objet_courant->Nb_facettes = tmp;
//                    printf("Indice objet : %d, facettes,   balise nbr :%d\n",o,tmp);
                    // Création du tableau des facettes
                    Element->str.clear();
                    Element->N_elements = tmp;
                    Element->makeface();
                    Element->makenormale();
                    Element->makeaspect_face();
//                    if (Objet_courant->Nb_vecteurs != 0)
//                        Element->makeluminance();
                }
            }
        } else if (XML_Depth == 6) {
            if (!strcmp(el,"sommet")) {
                if (!strcmp(attr[i],"id")) {
                    // Ne marchera bien que si id est en 0
                    ind_xyz = atoi(attr[i+1]);              // Indice courant du sommet
                } else if (!strcmp(attr[i],"xyz")) {        // lecture des x,y,z des sommets
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz);
                    Element->str.clear();
                    Element->N_elements = ind_xyz;
                    Element->Setxyz(vx,vy,vz);
                    Element->make1sommet();
                }
            } else if (!strcmp(el,"normale_s")) {
                if (!strcmp(attr[i],"id")) {
                    ind_xyz = atoi(attr[i+1]);              // Indice courant de la normale au sommet
                } else if (!strcmp(attr[i],"xyz")) {        // lecture des x,y,z des normales au sommet
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz);
                    Element->str.clear();
                    Element->N_elements = ind_xyz;
                    Element->Setxyz(vx,vy,vz);
                    Element->make1vecteur();
                }
            } else if (!strcmp(el,"facette")) {
                if (!strcmp(attr[i],"id")) {
                    ind_fac = atoi(attr[i+1]);              // Indice courant de la facette
                    // Initialiser quelques valeurs de la facette
                    Element->Objetlist[o].Facelist[ind_fac-1].flat = true;
                }
            }
        } else if (XML_Depth == 7) {
            Face_ij = &(Element->Objetlist[o].Facelist[ind_fac-1]);
            if (!strcmp(el,"normale_b")) {
                if (!strcmp(attr[i],"xyz")) {
                    sscanf(attr[i+1],"%f%f%f",&vx,&vy,&vz); // Lire x,y,z de la normale au barycentre
                    Element->str.clear();
                    Element->N_elements = ind_fac;
                    Element->Setxyz(vx,vy,vz);
                    // Garder les 3 valeurs dans un tableau statique. On en aura besoin si le mot clé sommets est lu après normale_b
                    vxyz[0] = vx ; vxyz[1] = vy ; vxyz[2] = vz ;
                    Element->make1normale();
                }
            } else if (!strcmp(el,"sommets")) {
                if (!strcmp(attr[i],"ref")) {
//                    i1=sscanf(attr[i+1],"%d",&tmp);
//                    printf("%d %d\n",i1,tmp);
                    cptr = (char *)attr[i+1] ;
                    i1=0;                                               // Pour compter le nombre de sommets
                    while (*cptr == ' ') cptr++ ;                       // Eliminer les blancs de tête
                    do {
                        i1++;
//                        sscanf(cptr,"%d",&valp[i1]) ;                   // Stockage provisoire, mais valp ne sert plus ici
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
                        while (*cptr == ' ') cptr++ ;                   // Passer les blancs
                        nc=sscanf(cptr,"%s",sc) ;
                    } while (nc != EOF) ;
//                    printf("%d sommets dans cette facette\n",i1);
                    n=i1;
                    Element->str.Printf(_T("%d %d "),ind_fac,n);
                    wxNumeros = wxString::FromAscii(attr[i+1]) ;        // plutôt que les valp on met l'intégralité de la chaîne attr
                    Element->str += wxNumeros;
                    Element->make1face();
                    // make1face remet à 0 la normale au barycentre
                    // => la restituer via vxyz (intérêt si sommets lu après normale_b)
//                    Element->Objetlist[Element->indiceObjet_courant].Facelist[ind_fac-1].setNormale_b(vxyz);
//                    Element->Objetlist[Element->indiceObjet_courant].Facelist[ind_fac-1].flat = true ; // Facette plane par défaut => normales_s lu après !
                    Face_ij->setNormale_b(vxyz);
//                    Face_ij->flat = true ; // Facette plane par défaut => normales_s doit être lu après !
                }
            } else if (!strcmp(el,"normales_s")) {
                Objet_courant = &(Element->Objetlist[o]);
                if (!strcmp(attr[i],"ref")) {
//                    i1=sscanf(attr[i+1],"%d",&tmp);
//                    printf("%d %d\n",i1,tmp);
                    cptr = (char *)attr[i+1] ;
                    i1=0;                                               // Pour compter le nombre de normales aux sommets
                    while (*cptr == ' ') cptr++ ;                       // Eliminer les blancs de tête
                    do {
                        i1++;
 //                       sscanf(cptr,"%d",&valp[i1]) ;                   // Stockage provisoire
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
                        while (*cptr == ' ') cptr++ ;                   // Passer les blancs
                        nc=sscanf(cptr,"%s",sc) ;
                    } while (nc != EOF) ;
//                    printf("%d sommets dans cette facette\n",i1);
                    n=i1;

                    if (Objet_courant->Nb_luminances == 0) {
                        Element->N_elements = Objet_courant->Nb_facettes;
                        Element->str.clear();
                        Element->makeluminance();
                    }
                    Element->str.Printf(_T("%d %d "),ind_fac,n);
                    wxNumeros = wxString::FromAscii(attr[i+1]) ;
                    Element->str += wxNumeros;
                    Element->make1luminance();
                    Face_ij->flat = false ;    // La facette n'est a priori pas plane
                    Objet_courant->flat = false;                         // Du coup, idem pour l'objet.
                    // Vérifier que la facette n'est pas plane => même numéro de vecteur sur tous les sommets
                    Numeros_L = Face_ij->getL_sommets();
                    int Num_0 = Numeros_L[0];
                    bool Facette_plane = true;
                    for (unsigned int k=1; k<Numeros_L.size(); k++) {
                        if (Numeros_L[k] != Num_0) Facette_plane = false;
                    }
                    if (Facette_plane) Face_ij->flat = true ;    // La facette est donc plane
                }
            } else if (!strcmp(el,"materiaux")) {
                if (!strcmp(attr[i],"ref")) {
                    sscanf(attr[i+1],"%d%d",&tmp,&tmp2);
                    // Affecter groupe ou materiau suivant ce qui a été donné dans types_valeurs
                    // ATTENTION : ci dessous ne marche que si codmatface est après codegroupe dans le fichier g3d
                    //             de même, le mot clé materiaux (ou valeurs) doit être situé après le mot clé sommets sinon make1face réinitialise groupe de codmatface !
//                    if (tmp==codegroupe  )
//                        Element->str.Printf(_T("%d <GROUPE>  %d"),ind_fac,tmp2)    ; Element->make1aspect_face();
//                    if (tmp==codemateriau)
//                        Element->str.Printf(_T("%d <CODMATFACE> %d"),ind_fac,tmp2); Element->make1aspect_face(); // ind_fac,
                        Face_ij->codmatface = tmp2;
                }
            } else if (!strcmp(el,"valeurs")) {
                if (!strcmp(attr[i],"val")) {
                    sscanf(attr[i+1],"%d%f",&tmp,&vx);
                    // Affecter groupe ou materiau suivant ce qui a été donné dans types_valeurs
//                    if (tmp==codegroupe  )
//                        Element->str.Printf(_T("%d <GROUPE>  %d"),ind_fac,(int)vx)     ; Element->make1aspect_face();
                        Face_ij->groupe = (int)vx;
//                    if (tmp==codemateriau)
//                        Element->str.Printf(_T("%d <CODMATFACE> %d"),ind_fac,(int)vx) ; Element->make1aspect_face(); //ind_fac,
                }
            }
        }
    }

//  printf("\n");
    XML_Depth++;
}

static void XMLCALL end_XML_Element(void *data, const char *el)
{
    XML_Depth--;
}

//******************************************************************************

void BddInter::LectureXML_G3d (FILE *f)
{
   /*
    *  Lecture d'un Fichier de polygones au format XML G3d Version 2.
    */
    int done;
    int len;
    int o;
    int nb_T_points, nb_T_norml, nb_T_facettes ;
    XML_Parser p;
    unsigned int indice_premierObjet;

    rewind(f);
    indice_premierObjet = this->Objetlist.size();   // Pour utilisation en cas de fusion

    sprintf(Message,"\nNon complètement testé : pas assez de fichiers de test ...\n");
    printf(utf8_To_ibm(Message));
//    printf("\nNon completement teste : pas assez de fichiers de test ...\n");

    codemateriau = -1;
    codegroupe   = -1;

    p = XML_ParserCreate(NULL);
    if (!p) {
        fprintf(stderr, "Allocation memoire impossible pour le parseur\n");
        exit(-1);
    }

    XML_SetElementHandler(p, start_XML_Element, end_XML_Element);
    XML_SetUserData(p, this);

    for (;;) {

        len = (int)fread(XML_Buff, 1, BUFFSIZE, f);
        if (ferror(f)) {
            fprintf(stderr, "Erreur de lecture du fichier XML\n");
            exit(-1);
        }
        done = feof(f);

        if (XML_Parse(p, XML_Buff, len, done) == XML_STATUS_ERROR) {
            fprintf(stderr, "Erreur de Parseur ligne : %" XML_FMT_INT_MOD "u:\n%s\n",
                    XML_GetCurrentLineNumber(p),
                    XML_ErrorString(XML_GetErrorCode(p)));
#ifdef WIN32
            system("pause") ;
#endif
            exit(-1);
        }

        if (done)
            break;
    }
    XML_ParserFree(p);

//    printf("Nombre d'objets                : %d\n",xnb_obj);
    int nb_objets = this->Objetlist.size();
    printf("\nNombre d'objets : %d\n",nb_objets -indice_premierObjet);
    /*    printf("Nombre total de points         : %d\n",xnb_pts);
        printf("Nombre de normales aux sommets : %d\n",xnb_norm);
        printf("Nombre total de facettes       : %d\n",xnb_fac); */

    nb_T_points = nb_T_norml = nb_T_facettes = 0 ;

//    if (indice_premierObjet != 0) printf("Recapitulatif de tous les objets :\n");

    for (o=indice_premierObjet ; o< nb_objets ; o++) {
		indiceObjet_courant = o;
        sprintf(Message,"\nNuméro de l'objet %2d                 : %d\n",o,this->Objetlist[indiceObjet_courant].GetValue());
        printf(utf8_To_ibm(Message));
        wxCharBuffer buf_nom = this->Objetlist[o].GetName();    // Normalement déjà en utf8 dans le format g3d
        printf("Nom de l'objet %2d                    : %s\n",o,buf_nom.data());
        printf("Nombre de points de l'objet %2d       : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_sommets);
        nb_T_points += Objetlist[o].Nb_sommets;
        printf("Nombre de normales_s de l'objet %2d   : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_vecteurs);
        nb_T_norml += this->Objetlist[o].Nb_vecteurs ;
        printf("Nombre de facettes de l'objet %2d     : %d\n",o,this->Objetlist[indiceObjet_courant].Nb_facettes);
        nb_T_facettes += this->Objetlist[o].Nb_facettes ;
    }
    printf("\n");
    printf("Nombre total de points               : %d\n",nb_T_points  );
    printf("Nombre total de normales aux sommets : %d\n",nb_T_norml   );
    printf("Nombre total de facettes             : %d\n",nb_T_facettes);

    sprintf(Message,"Non encore totalement opérationnel/testé\n\n");
    printf(utf8_To_ibm(Message));
/*#ifdef WIN32
    system("pause") ;
#endif
    exit(9); */
}

//******************************************************************************

void BddInter::LoadG3D()
{
    if(verbose) printf("Entree de BddInter::LoadG3D\n");
    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f=fopen(buffer.data(),"r");	//ouverture du fichier
    fgets(s1,160,f) ;
    if (!strncmp(s1,"<?xml ",6)) {
    // Lire la ligne suivante pour identifier g3d version 2
        fgets(s1,160,f);
        if (!strncmp(s1,"<g3d version=\"2.",16)) {           // Vérifie que la seconde ligne commence par <g3d version="2.
            sprintf(Message,"Fichier de type : XML Format g3d Version 2\n") ;
            printf(utf8_To_ibm(Message)) ;
//            printf("Fichier de type : XML Format g3d Version 2\n") ;
//           printf("%x %x\n",cel,&cel);
            LectureXML_G3d(f); //cel,f,entete,svg_entete) ;
            fclose(f) ;
//            if (svg_entete && entete != NULL) fclose(entete) ;
//            init_liste_Materiaux(cel) ;
            m_loaded = true;
            m_gllist = 0;

            return; // (0) ;
        } else {
            sprintf(Message,"Fichier de type XML mais n'est pas du g3d Version 2\n") ;
            printf(utf8_To_ibm(Message)) ;
            type = -1; // Erreur
        }
    }

    fclose(f);
    if(verbose) printf("Sortie de BddInter\n");
}

void BddInter::Optimiser_Obj_Sommets(Object * objet_courant, int o, bool &msg_optim)
{
    int i, j;
    int indice_min, indice_max, numero_sommet;
    int NbFacettes;
    Face *facette_courante;

    NbFacettes = objet_courant->Nb_facettes;

    if (NbFacettes <= 0) return;    // Pas de facettes, sortir immédiatement car rien à faire sur cet objet

    if (msg_optim) {
        printf("Optimisation du fichier .obj : Traitement des sommets de facettes et des vecteurs\n");  // à n'écrire que la première fois
        msg_optim = false;
    }
    indice_max = -1;
    indice_min = objet_courant->Nb_sommets;                     // On traite d'abord les sommets de facettes
    for (i=0; i<NbFacettes; i++) {                              // Recherche des numéros de sommets min et max utilisés dans l'objet
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_F; j++) {
            numero_sommet = facette_courante->F_sommets[j];
            indice_min = std::min(indice_min, numero_sommet);
            indice_max = std::max(indice_max, numero_sommet);
        }
    }
    printf("Objet %3d, Sommets  : indice_min=%5d, indice_max=%5d, Nouvelles valeurs [1,%d]\n",o,indice_min,indice_max,indice_max-indice_min+1);
    // Diminuer la taille du tableau des sommets en enlevant le début -> indice_min-1 et la fin à partir de indice_max+1
//                    if (indice_max < (int)objet_courant->Nb_sommets)              // Effacer d'abord la fin
//                        objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin()+indice_max,objet_courant->Sommetlist.end());
    objet_courant->Sommetlist.resize(indice_max);                   // Effacer d'abord la fin : un resize suffit <=> ligne précédente !
    if (indice_min > 1)                                             // Puis le début
        objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin(),objet_courant->Sommetlist.begin()+indice_min-1);
    objet_courant->Nb_sommets = indice_max - indice_min +1;         // Mettre à jour Nb_sommets
    if (objet_courant->Nb_sommets != objet_courant->Sommetlist.size()) printf("Oups tailles != Nb_sommets\n");  // Au cas où ...
    // Changer les numéros de sommets des facettes
    indice_min--;
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (int j=0; j < facette_courante->Nb_Sommets_F; j++) {
            facette_courante->F_sommets[j] -= indice_min;           // Renumérotation des sommets utilisés dans chaque facette
        }
    }
    // En toute rigueur, il faudrait aussi changer les objet_courant->Sommetlist[*].Numero en les décalant de indice_min (idem sur les vecteurs)
}

void BddInter::Optimiser_Obj_Vecteurs(Object * objet_courant, int o)
{
    int i, j;
    int indice_min, indice_max, numero_sommet;
    int NbFacettes;
    Face *facette_courante;

    NbFacettes = objet_courant->Nb_facettes;

    if (NbFacettes <= 0) return;    // Pas de facettes, sortir immédiatement car rien à faire sur cet objet

    indice_max = -1;
    indice_min = objet_courant->Nb_vecteurs;                        // On traite maintenant les normales aux sommets de facettes
    objet_courant->Nb_vecteurs = 0;                                 // Réinitialiser Nb_vecteurs
    if (objet_courant->Facelist[0].Nb_Sommets_L == 0) {
        objet_courant->Nb_luminances = 0;                           // et Nb_luminances par précaution ici !
        return;                                                     // Passer à l'objet suivant car pas de normales aux sommets des facettes
    }
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_L; j++) {
            numero_sommet = facette_courante->L_sommets[j];
            indice_min = std::min(indice_min, numero_sommet);
            indice_max = std::max(indice_max, numero_sommet);
        }
    }
    printf("Objet %3d, Vecteurs : indice_min=%5d, indice_max=%5d, Nouvelles valeurs [1,%d]\n",o,indice_min,indice_max,indice_max-indice_min+1);

    objet_courant->Vecteurlist.resize(indice_max);                  // Effacer d'abord la fin : un resize suffit
    if (indice_min > 1)                                             // Puis le début
        objet_courant->Vecteurlist.erase(objet_courant->Vecteurlist.begin(),objet_courant->Vecteurlist.begin()+indice_min-1);
    objet_courant->Nb_vecteurs = indice_max - indice_min +1;        // Mettre à jour Nb_vecteurs
    if (objet_courant->Nb_vecteurs != objet_courant->Vecteurlist.size()) printf("Oups tailles != Nb_vecteurs\n");
    // Changer les numéros de sommets des facettes
    indice_min--;
    for (i=0; i<NbFacettes; i++) {
        facette_courante = &(objet_courant->Facelist[i]);
        for (j=0; j < facette_courante->Nb_Sommets_L; j++) {
            facette_courante->L_sommets[j] -= indice_min;
        }
    }
    // En toute rigueur, il faudrait aussi changer les objet_courant->Vecteurlist[*].Numero en les décalant de indice_min (idem sur les sommets)
}

void BddInter::LoadOBJ()
{
/*
 *  Lecture d'un Fichier de polygones au format Wavefront OBJ
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  NOTE : la lecture se fait en 3 passes car les numéros de sommets peuvent être utilisés dans des objets/facettes avant d'être définis.
 *         Tous les sommets de tous les objets vont être définis dans le premier objet. Le tri et l'optimisation seront faits plus tard.
 */
//    FILE* f;                                      // Déjà déclaré au niveau général, donc autant l'utiliser
    char sc[100];//,s1[666],buffer[1000] ;			//chaines de caractères
    char nom_obj[512];
    char newline='\\' ;
    unsigned int i, n, nfac, nfac_t, npoint, nb_fac, nb_p, found, first, npoint_courant ;
    unsigned int nb_norm, nnorm ;
    unsigned int npoint_t, nnorm_t ;
    unsigned int o=0, oo=0;		            // pour compter le nombre d'objets
    int nc;
    int valp[100],valn[100] ;

    #define nb_mat_max 4000
    char *tab_nom[nb_mat_max]; // Provisoire en attendant de mettre directement dans la structure d'objet

    int num_mat=-123 ;
//    bool Forcer_1_Seul_Objet=false ; // mis un niveau au dessus ! Si false, on crée le nombre d'objets trouvés dans la bdd, si true, on met tout dans 1 seul objet

    unsigned int num_min, num_max, num_tot, num_cur ;
    unsigned int *full_ind_sommets=NULL, *new_ind_sommets=NULL, *inv_ind_sommets=NULL, *p_uint ;
    int indice, num_min1, num_min2;
    int indice_min,indice_max;
    unsigned norm_point_fac_existe, nb_normp_fac;
    int compt=0, speed=200 ;

    unsigned int Nb_objets, indice_premierObjet;
    float vx,vy,vz;

    bool nom_utf8;
    wxString str_loc;

    std::vector<int> Numeros;

    Face *Face_ij;

    // A mettre ailleurs ??
    const char delimiters[] = "/\\" ;
    char *cptr,*token,*nom_fichier;             //Pointeur de chaîne de caractères

    if(verbose) printf("Entree de BddInter::LoadOBJ\n");

    wxCharBuffer buffer=this->file.mb_str();

    // A mettre ailleurs ??
    cptr  = strdup(buffer.data());
    token = strtok(cptr,delimiters) ;
    do {
        cptr=token;
        token = strtok(NULL,delimiters) ;
    } while (token != NULL) ;
    printf("Nom du fichier : %s\n", cptr);
    nom_fichier = cptr;                         // a simplifier ?

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f=fopen(buffer.data(),"r");	//ouverture du fichier
    fgets(s1,160,f) ;
    if (!strncmp(s1,"#",1) || !strncmp(s1,"mtllib",6) || !strncmp(s1,"g ",2) || !strncmp(s1,"v ",2)) {
        if (s1[0] == '#') {
            for (i=2; i<100; i++) {
                if (fgets(s1,160,f) != NULL) {
                    if(!strncmp(s1,"mtllib",6) || !strncmp(s1,"g ",2) || !strncmp(s1,"v ",2)) {
                        if(!strncmp(s1,"mtllib",6)) {
                            sprintf(Message,"Fichier Wavefront OBJ avec fichier matériau : %s",s1+7) ;
                            mtllib_OK  = true;
                            mtllib_nom = strdup(s1);    // Sauvegarde de la ligne mtllib
//                            printf("Fichier Wavefront OBJ avec fichier materiau : %s",s1+7) ;
                        } else {
                            sprintf(Message,"Fichier Wavefront OBJ : %s\n",cptr) ;
                            mtllib_OK  = false;
//                            printf("Fichier Wavefront OBJ : %s\n",cptr) ;
                        }
                        printf(utf8_To_ibm(Message)) ;
                        found = 1 ;
                        break ;
                    }
                } else {
                    break;
                }
            }
        } else found = 1;
        if (found) {
//            printf("%d ligne\n",i) ;
//            LectureOBJ(cel,f,entete,svg_entete,cptr) ;
            nb_p   = 0;
            nb_fac = 0;
            nb_norm= 0;
            nb_mat = 0;
            o      = 0;
            tab_nom[0] = strdup(buffer.data()) ;
            rewind(f) ;

            while (fgets(s1,660,f) != NULL) {
//                if (!strncmp(s1,"mtllib",6)) mtllib_OK = true; // Déjà fait plus tôt
                if (!strncmp(s1,"v ", 2))  nb_p++;
                if (!strncmp(s1,"f ", 2))  nb_fac++;
                if (!strncmp(s1,"fo ", 3)) nb_fac++; // f et fo synonymes
                if (!strncmp(s1,"g ", 2)) {
                    tab_nom[o]=strdup(s1+2) ;
                    o++;
                    if (o == nb_mat_max) {
                      sprintf(Message,"ATTENTION : Trop d'objets dans ce fichier .obj augmenter nb_mat_max > %u\n",nb_mat_max);
                      printf(utf8_To_ibm(Message));
//                      --nb_mat ; // On utilisera le dernier indice pour tous les suivants...
#ifdef WIN32
                        system("pause");
#endif
                    }
                }
                if (!strncmp(s1,"vn ",3)) nb_norm++;
                if (!strncmp(s1,"usemtl ",7)) {
                    if (nb_mat == 0) {
                        tab_mat[nb_mat++] = strdup(s1+7);
                    } else {
                        found=0;
                        for (i=0 ; i< nb_mat ; i++) {
                            if (!strncmp(s1+7,tab_mat[i],strlen(tab_mat[i]))) {
                                found = 1;  // Ce matériau est déjà dans la liste
                                break;
                            }
                        }
                        if(!found) {        // Ajouter le nouveau matériau
                            tab_mat[nb_mat++] = strdup(s1+7);
                            if (nb_mat >= nb_mat_max) {
                                sprintf(Message,"ATTENTION : Trop de matériaux dans ce fichier .obj nb_mat > %u\n",nb_mat_max);
                                printf(utf8_To_ibm(Message));
//                                printf("ATTENTION : Trop de materiaux dans ce fichier .obj nb_mat > %u\n",nb_mat_max);
                                --nb_mat ; // On utilisera le dernier indice pour tous les suivants...
#ifdef WIN32
                                system("pause");
#endif
                            }
                        }
                    }
                }
            }
            if (o == 0) o=1; // pas de "g ", mais tout de même un objet
            printf("Nombre total d'objets    : %d\n",o) ;
            if (o) {
                sprintf(Message,"Liste des nom lus:\n");
                printf(utf8_To_ibm(Message));
//                printf("Liste des nom lus:\n");
                for (i=0 ; i< o ; i++) {
                    str_loc  = wxString(tab_nom[i]);
                    nom_utf8 = is_utf8(tab_nom[i]) ;
                    if (!nom_utf8) str_loc = str_loc.utf8_str();    // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
                    sprintf(Message,"Numéro: %3d, %s",i+1,(const char *)str_loc.data());//tab_nom[i]); // Pas besoin de \n, il est déjà dans tab_nom
                    printf(utf8_To_ibm(Message));
//                    printf("Numero: %3d, %s",i+1,tab_nom[i]); // Pas besoin de \n, il est déjà dans tab_nom
                }
                printf("\n") ;
            }
            printf("Nombre total de points   : %d\n",nb_p) ;
            printf("Nombre total de normales : %d\n",nb_norm) ;
            printf("Nombre total de facettes : %d\n",nb_fac)  ;
            sprintf(Message,"Nombre total de matériaux: %d\n",nb_mat);
            printf(utf8_To_ibm(Message));
//            printf("Nombre total de materiaux: %d\n",nb_mat);
            if (nb_mat) {
                sprintf(Message,"Liste des matériaux :\n");
                printf(utf8_To_ibm(Message));
//                printf("Liste des materiaux:\n");
                for (i=0 ; i< nb_mat ; i++) {
                    str_loc  = wxString(tab_mat[i]);
                    nom_utf8 = is_utf8(tab_mat[i]);
                    if (!nom_utf8) str_loc = str_loc.utf8_str();    // Le nom du matériau n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
                    sprintf(Message,"Numéro: %3d, %s",i+1,(const char *)str_loc.data());//tab_mat[i]); // Pas besoin de \n, il est déjà dans tab_mat
                    printf(utf8_To_ibm(Message));
//                    printf("Numero: %3d, %s",i+1,tab_mat[i]); // Pas besoin de \n, il est déjà dans tab_mat
                }
                printf("\n") ;
            }

            if(Forcer_1_Seul_Objet) o=1 ;
            Nb_objets = o;

// Création des tableaux de points et de normales (tous identiques, avec tous les points de tous les objets)
// Ici, ne créer que les tableaux pour l'objet 1 et y faire pointer ceux des autres objets.

            indice_premierObjet = this->Objetlist.size();   // indice du premier objet de l'objet à charger
            for (o=0; o<Nb_objets; o++) {
                this->str.Printf(_T("<OBJET> %d Test_%d"),o,o);    // Nom provisoire
                this->makeobjet();
//                indiceObjet_courant = this->Objetlist.size() -1;

// ?? ne faire que si o != 0 ?
                this->N_elements = nb_p;
                this->str.clear();
                this->makesommet();
                this->Objetlist[indiceObjet_courant].Nb_sommets = nb_p; // Ici tous les sommets

                this->N_elements = nb_norm;
//                if (nb_norm == 0) this->N_elements = nb_p ;  // Pas de normales dans le fichier => il faudra les calculer
//                if (nb_norm != 0) this->makevecteur();
                this->makevecteur();
                this->Objetlist[indiceObjet_courant].Nb_vecteurs = this->N_elements; // Ici toutes les normales aux sommets
            }

//
//            if (svg_entete && temp != NULL) {
//                fprintf(temp,"#### Fichier original de type Wavefront OBJ ####\n\n") ;
//            }

// Seconde analyse pour remplir les tableaux de points et de normales aux sommets incluses dans le fichier et créer la structure
// pour les facettes
            rewind(f) ;
            o=indice_premierObjet ;
            indiceObjet_courant = o;
            npoint = 0 ;
            nfac   = 0 ;
            nnorm  = 0 ;
            first  = 1 ;
            while (fgets(s1,660,f) != NULL) {   // Jusqu'à la fin de fichier
                if (!strncmp(s1,"v ", 2)) {     // Lecture des points (Les mettre tous dans l'objet 1)
                    npoint++;
                    sscanf(s1+2,"%f%f%f", &vx,&vy,&vz);
                    this->str.clear();
                    this->N_elements = npoint;
                    this->Setxyz(vx,vy,vz);
                    this->make1sommet();
                    continue;
                }
                if (!strncmp(s1,"vn ", 3)) {            // Lecture des normales aux sommets
                    nnorm++;
                    sscanf(s1+3,"%f%f%f", &vx,&vy,&vz);
                    this->str.clear();
                    this->N_elements = nnorm;
                    this->Setxyz(vx,vy,vz);
                    this->make1vecteur();
                    continue;
                }
                if (!strncmp(s1,"f ", 2) || !strncmp(s1,"fo ", 3)) {
                    nfac++;
                    continue;
                }
                if (!strncmp(s1,"g ", 2) && !Forcer_1_Seul_Objet) {
                    if (first) {
                        first = 0 ;
                    } else {

// Création du tableau des facettes de l'objet précédent
                        this->Objetlist[o].Nb_facettes = nfac ;                  // Nombre de facettes de l'objet
                        this->N_elements = nfac;
                        str.clear();
                        indiceObjet_courant = o;    // Pour le makeface et makeluminance, il faut l'indice réel dans indiceObjet_courant
                        makeface();
                        if (nb_norm != 0) makeluminance();
                        o++;
                        indiceObjet_courant = indice_premierObjet;    // Pour forcer à écrire tous les sommets et vecteurs dans le 1er objet
                        nfac = 0;
                    }
                    continue;
                }
            }

// Création du tableau des facettes du dernier objet
            this->Objetlist[o].Nb_facettes = nfac ;                  // Nombre de facettes de l'objet
            this->N_elements = nfac;
            str.clear();
            indiceObjet_courant = o;    // Pour le makeface et makeluminance, donner la bonne valeur à indiceObjet_courant
            makeface();
//            makenormale();
            if (nb_norm != 0) makeluminance();
//            makeaspect_face();

// Troisième analyse : remplissage des tableaux de facettes

            rewind(f) ;
//    printf("OK2...\n");
            npoint_courant = 1 ;
            i=0 ;
            if (mtllib_OK) {
                do {
                    i++;
                    fgets(s1,660,f) ;
                } while (strncmp(s1,"mtllib",6)) ;
                strcpy(nom_obj, Lire_chaine(s1+7)) ;      // Récupérer ce qui suit mtllib
        //        if ((cptr=strstr(nom_obj,".mtl")) != NULL) *cptr='\0' ; // Supprime l'extension .mtl : ici capte aussi une chaine .mtl qui ne serait pas une extension (en fin de nom_obj)
                if ((cptr=strstr(nom_obj,".mtl")) == ((char *)&nom_obj+strlen(nom_obj)-4)) *cptr='\0' ; // Supprime l'extension .mtl
            } else {
                strcpy(nom_obj,nom_fichier) ;
                if ((cptr=strstr(nom_obj,".obj")) == ((char *)&nom_obj+strlen(nom_obj)-4)) *cptr='\0' ; // Supprime l'extension .obj
            }
            printf("\nNom de l'objet : %s\n",nom_obj) ;   // c'est le Nom par défaut (si pas de "g ")

#if wxCHECK_VERSION(3,0,0)
            wxString wxNom=wxString(nom_obj);
#else
            wxString wxNom=wxString::FromAscii(nom_obj);
#endif
            nom_utf8 = is_utf8(wxNom.data());
            if (!nom_utf8) wxNom = wxNom.utf8_str();        // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8
            this->Objetlist[indice_premierObjet].SetName(wxNom);
        //    printf("OK3...\n");
            nfac    = 0 ;
            nfac_t  = 0 ;
            nnorm   = 0 ;
            o = indice_premierObjet ;
            indiceObjet_courant = o;
            first = 1 ;
            while (fgets(s1,660,f) != NULL) { // Jusqu'à la fin de fichier
        //        printf("%s",s1) ;
                if (!strncmp(s1,"g ",2) && !Forcer_1_Seul_Objet) {
                    if (first) {
                        first = 0 ;
                    } else {
                        o++ ;
                        nfac_t   += nfac;
                        nfac  =0 ;
                    }

#if wxCHECK_VERSION(3,0,0)
                    wxNom = wxString(Lire_chaine(s1+2));                    // Nom de l'objet
#else
                    wxNom = wxString::FromAscii(Lire_chaine(s1+2));
#endif // wxCHECK_VERSION
                    nom_utf8 = is_utf8(wxNom.data());
                    if (!nom_utf8) wxNom = wxNom.utf8_str();                // Le nom n'est pas en utf8 (donc probablement en Ansi) : le convertir en utf8

                    this->Objetlist[o].SetName(wxNom);                      // ! A vérifier car actuellement 1 seul objet !
                    indiceObjet_courant = o;
                    continue;
                }
                if (!strncmp(s1,"usemtl ",7)) {         // Lecture du nom de matériau
                    for (i=0 ; i< nb_mat ; i++) {
                        if (!strncmp(s1+7,tab_mat[i],strlen(tab_mat[i]))) {
                            num_mat = i+1;  // Le matériau est déjà dans la liste => num_mat = rang dans le tableau pour commencer à 1
                            // Cas particulier où le nom est group_* (conversion en .obj par Ovni)
                            if (!strncmp(s1+7,"group_",6)) {
                                num_mat = atoi(s1+13);  // Dans ce cas, ce qu'il y a derrière "group_" est le numéro de groupe original
                            }
                            break;
                        }
                    }
                    continue;
                }
                if (!strncmp(s1,"vn ", 3)) {            // comptage des normales aux sommets ... au cas où
                    nnorm++;
                    continue;
                }
                if (!strncmp(s1,"v ", 2)) {             // comptage des sommets ...
                    npoint_courant++;
                    continue;
                }
                if (!strncmp(s1,"f ",2) || !strncmp(s1,"fo ",3)) {
                    nfac++;
                    i=0 ;
                    cptr = s1+2 ;
                    nb_normp_fac = 0;
        //        printf("%s",s1);
                    do {
                        i++;
                        while (*cptr == ' ' || *cptr == '/') cptr++ ;   // Eliminer les blancs et les slashs
                        sscanf(cptr,"%d",&valp[i]) ;
                        if (valp[i] < 0) {                              // Ici ce sont des valeurs relatives / Point courant
                            valp[i] += npoint_courant ;
                            cptr++ ; // Sauter le signe -
                        }

        //        printf("lue %d\n",valp[i]) ;
                        while (*cptr >= '0' && *cptr <= '9') cptr++ ;   // Passer les chiffres
        //        printf("%s",cptr) ;
                        norm_point_fac_existe = 0;
                        if (*cptr == '/') {                             // Atteindre le / suivant (ou l'espace ou la fin de ligne)
        //        printf("%s",cptr) ;
                            cptr++;
        //        printf("%s",cptr) ;
                            while (*cptr != '/' && *cptr != ' ' && *cptr != '\n') {
                                cptr++;
        //        printf("%s",cptr) ;
        //        system("pause");
                            }
                            if (*cptr == '/') norm_point_fac_existe = 1;
                            if (*cptr != '\n') cptr++;
                        }
                        if (nb_norm != 0 && norm_point_fac_existe) {
                            while (*cptr == ' ' || *cptr == '/') cptr++ ;
                            if (*cptr != '\n') {                            // Pourrait arriver si // en fin de ligne et rien derrière
                                sscanf(cptr,"%d",&valn[i]);
                                ++nb_normp_fac;
                                if (valn[i] < 0 ) {                         // Ici ce sont des valeurs relatives / normale courante
                                    valn[i] += (nnorm+1) ;
                                    cptr++ ; // Sauter le signe -
                                }
                                while (*cptr >= '0' && *cptr <= '9') cptr++ ; // Passer les chiffres
                            }
                        }
                        nc=sscanf(cptr,"%s",sc) ;
                        if (nc == 1 && sc[0] == newline) {
                            fgets(s1,660,f);
                            cptr = s1;
                        }
                    } while (nc != EOF) ;
        //            system("pause") ;
                    n=i ;

                    Numeros.resize(n);

                    for (i=0; i<n ; i++) {
                        Numeros[i] = valp[i+1];
                    }
                    str.clear();
                    this->N_elements = nfac;
                    this->Set_numeros(Numeros);
                    make1face();

                    this->Setxyz(1.,0.,0.);
                    make1normale();             // Normale bidon

// Ci-dessous : OK si dans tous les cas autant de normales que de sommets. Si nnorm==0 pas de pb, sinon, si à la lecture
// certaines facettes n'ont pas de normales aux sommets => soucis ! N'arrive dans aucun fichier testé mais ....
// après introduction de nb_normp_fac, c'est un peu mieux, mais à vérifier ...

                    Numeros.resize(nb_normp_fac);
                    if (nb_normp_fac != 0 && nb_normp_fac != n) {
                        fprintf(stderr,"Attention : la facette %d a un nombre de points (%d) et de normales aux sommets (%d) different !\n",nfac,n,nb_normp_fac);
                        // Non this->Objetlist[o].Nb_normales = 0 ;    // Forcer à 0 car il y a certainement une erreur !!
                    }
                    for (i=0 ; i<n ; i++) {
                        if(nb_norm != 0 && nb_normp_fac == n) Numeros[i]=valn[i+1];// - nnorm_t;
                        else                                  Numeros[i]=valp[i+1];// - npoint_t;
                    }
//                    printf("%d %d\n",nb_normp_fac,n);
                    Face_ij = &(this->Objetlist[o].Facelist[nfac-1]);
                    if (nb_normp_fac == n) {            // Pas sûr que ce soit le bon test !!!
                        this->str.clear();
                        this->N_elements = nfac;
                        this->Set_numeros(Numeros);
                        this->make1luminance();
                        Face_ij->flat = false;              // Facette non plane
                        this->Objetlist[o].flat = false;    // donc l'objet aussi
                    } else {
                        Face_ij->flat = true;               // Facette plane
                    }
                    Face_ij->groupe     = num_mat;
                    Face_ij->codmatface = num_mat;
                    continue;
                }
            }
            nfac_t += nfac;     // Dernière mise à jour ici, car en dehors de la boucle des objets !

//! Et maintenant utiliser les tableaux de sommets et vecteurs de l'objet numéro 0
//! ATTENTION : ainsi, on fait des copies. Il faudrait plutôt, à ce niveau, pointer sur les tableaux/vectors de l'objet 0
//!             pour éviter de dupliquer ces (gros) tableaux.
//!             Si lecture optimisée des fichiers .obj, on éliminera ces sommets/vecteurs par la suite.
            Object * PremierObjet = &(this->Objetlist[indice_premierObjet]);
            Object * objet_courant;
            bool msg_optim = true;

            for (o=1+indice_premierObjet ; o<Nb_objets+indice_premierObjet ; o++) { // On ne commence que sur le 2ème objet les copies

                objet_courant= &(this->Objetlist[o]);
                objet_courant->Sommetlist  = PremierObjet->Sommetlist ;
                objet_courant->Vecteurlist = PremierObjet->Vecteurlist;

                if (lect_obj_opt) { // Suite inutile c'est forcément le cas ... && !Forcer_1_Seul_Objet && (Nb_objets > 1)) {
                    // Le but est d'éliminer dans chaque objet tous les points et vecteurs inutiles et renuméroter en conséquence
                    // les numéros de sommets utilisés dans les facettes et les luminances
                    // Lancer après coup une simplification de Bdd peut aussi faire le job
                    Optimiser_Obj_Sommets (objet_courant, o, msg_optim);
                    Optimiser_Obj_Vecteurs(objet_courant, o);
                    if (objet_courant->Nb_facettes == 0) {
                        objet_courant->deleted = true;
                        objet_courant->Sommetlist.clear();        // Réinitialiser les vecteurs pour libérer de la mémoire
                        objet_courant->Vecteurlist.clear();
                        objet_courant->Nb_sommets = objet_courant->Nb_vecteurs = 0;
                    }
                }
            }

            for (o=indice_premierObjet; o<Nb_objets+indice_premierObjet; o++) {
                for (i=0; i<this->Objetlist[o].Nb_facettes; i++) Calcul_Normale_Barycentre(o,i);
            }

            // Optimisation du fichier .obj à la lecture : élimination des sommets et vecteurs non utilisés
            // Cas particulier du 1er objet, celui contenant la liste des sommets et vecteurs de référence

            if (lect_obj_opt && !Forcer_1_Seul_Objet && (Nb_objets > 1)) {

                for (o=indice_premierObjet; o<=indice_premierObjet; o++) { // En fait, on ne passe qu'une seule fois dans cette boucle
                    objet_courant = &(this->Objetlist[o]);
                    Optimiser_Obj_Sommets (objet_courant, o, msg_optim);
                    Optimiser_Obj_Vecteurs(objet_courant, o);
                }

                if (PremierObjet->Nb_facettes == 0) {   // Cet objet est en fait vide, pas de facettes, seulement des sommets et des vecteurs
                    // éliminer les sommets et vecteurs du 1er objet (objet 0 si 1 seule Bdd) car ne servira plus.
                    PremierObjet->Sommetlist.resize(0);
                    PremierObjet->Vecteurlist.resize(0);
                    PremierObjet->Nb_sommets = PremierObjet->Nb_vecteurs = 0;
                    PremierObjet->deleted    = true;    // Ceinture et bretelles !!!! :-)
                }
            }

//****************************************
//            if (svg_entete && entete != NULL) fclose(entete) ;
//            init_liste_Materiaux(cel) ;
            m_loaded = true;
            m_gllist = 0;
            return ;
        }
        printf("Fichier .obj non conforme Wavefront\n") ;
        type = -1;
    }
    fclose(f);
    if(verbose) printf("Sortie de BddInter::LoadOBJ\n");
}

//******************************************************************************

void BddInter::LoadM3D()
{
/*
 *  Lecture d'un Fichier de polygones au format MilkShape 3D Ascii
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  Une création de groupes faite ?????????????????????????????????
 */

    char nom_obj[80];
    int sommet[3] ;
    int normal[3] ;
    char *cptr ;
    unsigned int n,norm ;
    unsigned int i, nfac, npoint, nb_fac, nb_p ;
    unsigned int NbVecteurs;
    unsigned int o=0;		            // pour compter le nombre d'objets
    unsigned int Nb_objets;
    float vx,vy,vz;
    std::vector<int> Numeros;
    bool test_flat;

    if(verbose) printf("Entree de BddInter::LoadM3D\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f = fopen(buffer.data(),"r");   //ouverture du fichier
    fgets(s1,160,f);
    if (!strncmp(s1+3,"MilkShape 3D ASCII",18)) {
        printf("Fichier de type : MilkShape 3D en Ascii\n") ;
    } else {
        printf("Fichier .m3d mais pas reconnu comme de type MilkShape 3D en Ascii\n") ;
        type = -1;
        fclose(f);
        return;
    }

    Numeros.resize(3); // Toujours 3 points dans ce format

//    fgets(s1,160,f);

    for (i=2; i<=6 ; i++) {
        fgets(s1,100,f) ;
    }
    if (strncmp(s1,"Meshes:",7)) {
        sprintf(Message,"Erreur dans le fichier : chaîne \"Meshes\" non trouvée\n") ;
        printf(utf8_To_ibm(Message)) ;
//        printf("Erreur dans le fichier : chaine \"Meshes\" non trouvee\n") ;
#ifdef WIN32
        system("pause") ;
#endif
        type = -1;
        exit(2) ;
    }
    sscanf(s1+7,"%d",&Nb_objets);
    printf("%d objets\n",Nb_objets);

//#ifdef WIN32
//        system("pause") ;
//#endif
//        exit(9);
//    }
//    if (svg_entete && temp != NULL) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Fichier MilkShape 3D Ascii ####\n\n") ;
//    }

    for (o=0 ; o< Nb_objets ; o++) {
        fgets(s1,100,f) ;
        strcpy(nom_obj, Lire_chaine(s1)+1) ;        //+1 pour enlever le " de début
        cptr = strchr(nom_obj,'\0') ;               // Pointe la fin de chaîne
        while(*--cptr != '"') {*cptr = '\0' ;}      // élimine la fin de chaîne.
        *cptr = '\0' ;                              // Puis éliminer ce " de fin
        printf("Objet :%4d, Nom : %s\n",o,nom_obj);

#if wxCHECK_VERSION(3,0,0)
        wxString wxNom=wxString(nom_obj);
#else
        wxString wxNom=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        this->str.Printf(_T("<OBJET> %d "),o);
        this->str += wxNom;
        this->makeobjet();
//        indiceObjet_courant = this->Objetlist.size()-1;//o;

        // Lecture du nombre de points
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&nb_p) ;

        this->str.clear();
        this->N_elements = nb_p;

        this->makesommet();
        // Lectures des points
        for (npoint = 1 ; npoint <= nb_p ; npoint++) {
            fgets(s1,200,f) ;
            // Lecture des x, y et z
            sscanf(s1,"%d%f%f%f", &i, &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make1sommet();
        }

        // Lecture du nombre de Normales aux sommets
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&NbVecteurs) ;

        this->N_elements = NbVecteurs;
        this->makevecteur();

        // Lectures des normales
        for (npoint = 1 ; npoint <= NbVecteurs ; npoint++) {
            fgets(s1,200,f) ;
            // Lecture des x, y et z
            sscanf(s1,"%f%f%f", &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make1vecteur();
        }

        // Lecture du nombre de facettes
        fgets(s1,100,f) ;
        sscanf(s1,"%d",&nb_fac) ;
        this->N_elements = nb_fac;
        this->Objetlist[indiceObjet_courant].Nb_facettes = nb_fac;

        this->makeface();
        this->makenormale();
        this->makeluminance();
        this->makeaspect_face();

        // Lecture des facettes
        for (nfac=1 ; nfac <= nb_fac ; nfac++) {
            this->str.clear();
            fgets(s1,200,f) ;
            for (i=0 ; i<3 ; i++) normal[i] = 0;
            sscanf(s1,"%d%d%d%d%d%d%d", &norm, &sommet[0], &sommet[1],&sommet[2], &normal[0], &normal[1] , &normal[2]) ;

            for (i=0; i<3; i++) Numeros[i] = sommet[i]+1;// Ajouter 1 au n°, car on commence à 1 et non 0
            this->N_elements = nfac;
            this->Set_numeros(Numeros);
            this->make1face();

            test_flat = false;
            for (i=0; i<3; i++) {
                n          = normal[i]+1;
                Numeros[i] = n;
                if (n > NbVecteurs) test_flat = true ; // incompatibilité de numéro => forcer cette facette à être plane
            }
            this->Set_numeros(Numeros);
            this->make1luminance();
            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].flat = test_flat; // Facette non plane si le numéro de vecteurs est compatible

            Calcul_Normale_Barycentre(indiceObjet_courant,nfac-1);

            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].groupe     = groupe_def;
            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].codmatface = codmatface_def;
        }
        this->Objetlist[indiceObjet_courant].Nb_facettes  = nb_fac;
        this->Objetlist[indiceObjet_courant].Nb_normales  = nb_fac;
        this->Objetlist[indiceObjet_courant].Nb_luminances= nb_fac;
        this->Objetlist[indiceObjet_courant].flat         = false;    // Objet a priori avec normales aux sommets, mais peut être invalidé facette/facette
    }
    m_loaded = true;
    m_gllist = 0;
    fclose(f);
    sprintf(Message,"\nFin de la lecture des données.\n");
    printf(utf8_To_ibm(Message));
    if(verbose) printf("Sortie de BddInter::LoadM3D\n");
}

//******************************************************************************

void BddInter::LoadPLY()
{
/*
*  Lecture d'un Polygon file, format Niratam - GEO
*  Identifie éventuellement plusieurs objets, crée des normales.
*  Une création de groupes faite (d'après les infos de materi)
*/

    char ident[80] ;			//chaînes de caractères  s1[666],
    char nom_obj[80], nom_prec[80] ;
    char *cptr ;
    int i0, n_OK, ittem ;
    unsigned int i, nfac, npoint, nb_fac, nb_p, n, indice_premierObjet ;
    int materi ;
    int index_ply= 0;
    bool i_warn  = false ;
    int id1, id2, id3 ;
    int o=0;		            // pour compter le nombre d'objets

    int o_dim=51, o_inc=10 ;
    int *o_nfac, *o_npoint ;
    int code_facette=0;     //flag pour tester s'il y a des facettes dégénérées (< 3 points)

    int Groupe_ply[maxGroupes] ;
    float vx, vy, vz;

    std::vector<int> Numeros_Sommets;

    if(verbose) printf("Entree de BddInter::LoadPLY\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f=fopen(buffer.data(),"r");	//ouverture du fichier

    fgets(s1,160,f);
    if (strncmp(s1,"GEO",3)) {
        if (strncmp(s1,"ply",3)) {
            printf("Fichier .ply mais pas de type GEO Niratam, ni Stanford Polygon file !\n");
            fclose(f);
            type = -1;
            return ;
        }
        fclose(f);
        this->LoadPLY_Stanford();
        return;
    }

    printf("Fichier GEO - Niratam : %s",s1) ;
    unsigned int nb_grp_ply = 0 ;

    for (i=0 ; i <= 4 ; i++) {
        fgets(s1,100,f) ;    // Le nom de l'avion est à la seconde ligne, donc pour i=0
        printf("%s",s1) ;
        if (i == 0) strcpy(ident,s1) ;
        if ((cptr=strstr(s1,"ITTEM =")) != NULL) break ;
        if (i == 4) {
            sprintf(Message,"Erreur dans le Polygon File : chaîne \"ITTEM =\" non trouvée\n") ;
            printf(utf8_To_ibm(Message)) ;
//            printf("Erreur dans le Polygon File : chaine \"ITTEM =\" non trouvee\n") ;
#ifdef WIN32
            system("pause") ;
#endif
            exit(2) ;
        }
    }
    sscanf(cptr+7,"%d",&ittem) ;
    for (i=0 ; i <= 10 ; i++) {
        fgets(s1,100,f)   ;
        printf("%s",s1) ;
        if ((cptr=strstr(s1,"Start")) != NULL) break ;
        if (i == 10) {
            sprintf(Message,"Erreur dans le Polygon File : chaîne \"Start\" non trouvée\n") ;
            printf(utf8_To_ibm(Message)) ;
//            printf("Erreur dans le Polygon File : chaine \"Start\" non trouvee\n") ;
#ifdef WIN32
            system("pause") ;
#endif
            exit(2) ;
        }
    }
//  Comptage des facettes, des objets et des points...
    fscanf(f,"%d",&n) ;
    nfac     = 0 ;
    npoint   = 0 ;
    nb_fac   = 0 ;
    nb_p     = 0 ;
    o_nfac   = (int *)malloc(o_dim*sizeof(int)) ;     // Pour conserver les valeurs nfac et npoint lues
    o_npoint = (int *)malloc(o_dim*sizeof(int)) ;
    strcpy(nom_prec,"testxxx") ;
    while (n != 0) {  // La fin du fichier est atteinte quand on lit n à 0
        fgets(s1,100,f) ;
        fgets(s1,100,f) ;
        fscanf(f,"%d", &i);
        fgets( s1,100,f)  ;

        strcpy(nom_obj, Lire_chaine(s1)) ;
        n      = (n-2)/3 ;
        if (strstr(nom_obj,nom_prec) == NULL) {
            if (nb_fac != 0) {
                printf(" avec %5d facettes et %5d points\n", nfac, npoint) ;
                if (o == o_dim) {
                    o_dim   += o_inc ;
                    o_nfac   = (int *)realloc(o_nfac,  o_dim*sizeof(int)) ; // Tableaux initiaux trops petits
                    o_npoint = (int *)realloc(o_npoint,o_dim*sizeof(int)) ; // Les agrandir....
                }
                o_nfac[o]   = nfac ;
                o_npoint[o] = npoint ;
            }
            o++ ;
            printf("%-30s",nom_obj) ;
            strcpy(nom_prec, nom_obj) ;
            npoint = 0 ;
            nfac   = 0 ;
        }
        for (i=1 ; i <= n ; i++) fgets(s1,100,f) ;
        npoint = npoint + n ;
        nb_p   = nb_p + n ;
        nfac ++ ;
        nb_fac++;

        fscanf(f,"%d",&n) ;
    }
    printf(" avec %5d facettes et %5d points\n", nfac, npoint) ;
    if (o == o_dim) {
        o_dim += o_inc ;
        o_nfac   = (int *)realloc(o_nfac,  o_dim*sizeof(int)) ;
        o_npoint = (int *)realloc(o_npoint,o_dim*sizeof(int)) ;
    }
    o_nfac[o]   = nfac ;
    o_npoint[o] = npoint ;
    printf("Total :\n%8d Objets\n%8d Facettes\n%8d Points\n",o,nb_fac,nb_p) ;

// Ici on constate que pour certains fichiers, un même nom d'objet apparaît
// plusieurs fois a différents endroits (faut-il les regrouper ?)

    /* retour au debut du fichier pour commencer la lecture des données */
    rewind(f);

    /* Lire jusqu'à Start */
    do {
        fgets(s1,100,f) ;
    } while (strstr(s1,"Start") == NULL) ;
// printf("%s",s1) ;
//    if (svg_entete && temp != NULL ) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Polygon File Niratam - GEO ####\n\n") ;
//        fprintf(temp,"#### %s\n\n", ident) ;
//    }

// On est au début des polygones/facettes

    int Nb_objets = o;
    o=-1 ;
    indice_premierObjet = this->Objetlist.size();   // peut-être != 0 en cas de fusions d'objets

    strcpy(nom_prec,"test_abcd_xxyyzz") ; // Nom bidon, initialisation...

    fscanf(f,"%d",&n) ;
    while (n != 0) {  // La fin du fichier est atteinte quand on lit n à 0
        if (ittem == 5) {
            fgets(s1,100,f) ;   // Passer le cr
            fgets(s1,100,f) ;   // Lire la ligne
//          printf("%s",s1) ;
            materi = 0 ;
//       s1 = toupper(s1) ;
            if (!strncasecmp(s1,"  SKN",5)) materi = 1 ; // On pourrait différentier en analysant
            if (!strncasecmp(s1,"  ITO",5)) materi = 2 ; // la chaîne suivante sur la ligne
            if (!strncasecmp(s1,"  ITI",5)) materi = 3 ; // Genre Paint-A, Paint-B,...
            if (!strncasecmp(s1,"  JCV",5)) materi = 4 ;
        } else {
            fscanf(f,"%d",&materi) ; // Décripter materi pour obtenir des groupes ...
            fgets(s1,100,f) ;   // Passer le cr
        }

// Différentiation des groupes
        if (nb_grp_ply == 0) {
            nb_grp_ply = 1 ;
            index_ply  = 1 ;
            Groupe_ply[1] = materi ;
        } else {
            i0 = 0 ;
            for (i=1 ; i <= nb_grp_ply ; i++) {
                if (materi == Groupe_ply[i]) {
                    i0 = 1 ;
                    index_ply = i ;
                }
            }
            if (!i0) {
                if (nb_grp_ply == maxGroupes) {
                    if (!i_warn) {
//                        printf("Warning : Nombre de valeurs ""materi"" differentes > %d\n",maxGroupes) ;
                        sprintf(Message,"Warning : Nombre de valeurs ""materi"" différentes > %d\n",maxGroupes) ;
                        printf(utf8_To_ibm(Message)) ;
                        i_warn = true ;
                    }
                } else nb_grp_ply++;
                index_ply = nb_grp_ply ;
                Groupe_ply[nb_grp_ply] = materi ;
            }
        }

        if(materi == 0) {
            // A faire
            printf("Warning : Fichier .ply : cas materi = 0 a coder ?\n");
        }

//      fgets(s1,100,f)     ; // Lire la ligne (contient un numéro et un identifiant d'objet)
//      sscanf(s1,"%d", &i) ; //

        fscanf(f,"%d", &i);     // Lire un numéro (serait à comparer à nfac)
        fgets( s1,100,f)  ;     // et un identifiant d'objet)
        strcpy(nom_obj, Lire_chaine(s1)) ;

        if (strstr(nom_obj,nom_prec) == NULL) {
            o++ ;                                   // On change d'objet dès que l'identifiant change
            strcpy(nom_prec, nom_obj) ;
            str.Printf(_T("<OBJET> %d "),o);

#if wxCHECK_VERSION(3,0,0)
            wxString wxNom=wxString(nom_obj);
#else
            wxString wxNom=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

            str += wxNom;
            makeobjet();
//            indiceObjet_courant = this->Objetlist.size()-1; //o;
            this->Objetlist[indiceObjet_courant].Nb_facettes = o_nfac[o+1];
            this->Objetlist[indiceObjet_courant].Nb_sommets  = o_npoint[o+1];

            this->N_elements = this->Objetlist[indiceObjet_courant].Nb_facettes;
            str.clear();
            makeface();
            makenormale();
//            makeaspect_face();

            this->N_elements = this->Objetlist[indiceObjet_courant].Nb_sommets;
            makesommet();

            nfac   = 0;
            npoint = 0;
        }
        ++nfac ;
        n = (n-2)/3 ; // Nombre de sommets à lire
        if (n < 3) code_facette = 1 ;

        Numeros_Sommets.resize(n);
        for (i=0 ; i < n ; i++) {
            ++npoint ;                // Il serait intéressant de vérifier ici si le point n'existe pas déjà (limiter les doublons)
            fscanf(f,"%f",&vx);		// Lecture du x du point
            fscanf(f,"%f",&vy);		// Lecture du y du point
            fscanf(f,"%f",&vz);		// Lecture du z du point
            this->str.clear();
            this->Setxyz(vx,vy,vz);
            this->N_elements = npoint;
            this->make1sommet();
            Numeros_Sommets[i] = npoint;
        }
        N_elements = nfac;
        this->Set_numeros(Numeros_Sommets);
        make1face();
        this->Objetlist[indiceObjet_courant].Facelist[nfac-1].flat = true;  // provisoire
        this->Setxyz(1.,0.,0.); // Provisoire
        make1normale();

        this->Objetlist[indiceObjet_courant].Facelist[nfac-1].groupe     = index_ply;
        this->Objetlist[indiceObjet_courant].Facelist[nfac-1].codmatface = index_ply;

        fscanf(f,"%d",&n) ;

// Vérifications de précaution : des fois que ...
        if (npoint > nb_p) {
            n = 0 ;
            printf("Warning : Trop de points dans le fichier ( > %d)\n",npoint) ;
        }
        if (nfac > nb_fac) {
            n = 0 ;
            printf("Warning : Trop de facettes dans le fichier ( > %d)\n",nfac) ;
        }
    }
    for (o=0; o<Nb_objets; o++) {
        indiceObjet_courant = indice_premierObjet + o;
        for (i=0 ; i < Objetlist[indiceObjet_courant].Facelist.size();i++) Calcul_Normale_Barycentre(indiceObjet_courant,i);
    }
    free(o_nfac)   ; // Libérer la mémoire obtenue par malloc/realloc ...
    sprintf(Message,"Nombre de groupes identifiés : %d\n",nb_grp_ply) ;
    printf(utf8_To_ibm(Message)) ;

    for (i=1 ; i<=nb_grp_ply ; i++) printf("%2d %8d\n",i,Groupe_ply[i]) ;
    free(o_npoint) ;

    m_loaded = true;
    m_gllist = 0;

    if (code_facette == 1) {
        printf("La bdd contient des facettes de 2 points ou moins !!\n") ;
        printf("Utiliser la Simplification de Bdd sinon risques de plantage\n");
        printf("avec certaines cartes graphiques!\n");
    }
    sprintf(Message,"\nFin de la lecture des données.\n");
    printf(utf8_To_ibm(Message));

//******************************************************************************

    fclose(f);

    if(verbose) printf("Sortie de BddInter::LoadPLY\n");
}

void BddInter::LoadPLY_Stanford()
{
/*
 *  Lecture d'un Fichier de polygones au format PLY de Stanford
 *  Un seul objet, crée les normales aux barycentres, lecture éventuelle des normales aux sommets (si présentes)
 *
 *  Largement inspiré/copié de headply et ply2iv, des programmes écrit par Greg Turk (auteur original du format ply).
 *  headply.c : Read the header from a PLY file and print it out
 *  ply2iv.c  : Convert a PLY file to an Inventor file.
 *
 *  Greg Turk

 * -----------------------------------------------------------------------

 *  Copyright (c) 2020, Georgia Institute of Technology
 *  All rights reserved.

 */

    int compteur = 0;
    char * buf;
    int i,j;
    int elem_count;

    char *elem_name;
    PlyFile *in_ply;

    std::vector<int> Numeros;
    Face *facette_courante;

//    bool verbose = true;  // Version locale de verbose

/* vertex and face definitions for a polygonal object */

    typedef struct PlyVertex {
        float x,y,z;
        float r,g,b;
        float nx,ny,nz;
        void *other_props;       /* other properties */
    } PlyVertex;

    typedef struct PlyFace {
        unsigned char nverts;    /* number of vertex indices in list */
        int *verts;              /* vertex index list */
        void *other_props;       /* other properties */
    } PlyFace;

    const char *elem_names[] = { /* list of the elements in the object */
        "vertex", "face"
    };

    PlyProperty vert_props[] = { /* list of property information for a vertex */
        {(char*)"x",  Float32, Float32, offsetof(PlyVertex,x),  0, 0, 0, 0},
        {(char*)"y",  Float32, Float32, offsetof(PlyVertex,y),  0, 0, 0, 0},
        {(char*)"z",  Float32, Float32, offsetof(PlyVertex,z),  0, 0, 0, 0},
        {(char*)"r",  Float32, Float32, offsetof(PlyVertex,r),  0, 0, 0, 0},
        {(char*)"g",  Float32, Float32, offsetof(PlyVertex,g),  0, 0, 0, 0},
        {(char*)"b",  Float32, Float32, offsetof(PlyVertex,b),  0, 0, 0, 0},
        {(char*)"nx", Float32, Float32, offsetof(PlyVertex,nx), 0, 0, 0, 0},
        {(char*)"ny", Float32, Float32, offsetof(PlyVertex,ny), 0, 0, 0, 0},
        {(char*)"nz", Float32, Float32, offsetof(PlyVertex,nz), 0, 0, 0, 0},
    };

    PlyProperty face_props[] = { /* list of property information for a face */
        {(char*)"vertex_indices", Int32, Int32, offsetof(PlyFace,verts), 1, Uint8, Uint8, offsetof(PlyFace,nverts)},
        {(char*)"vertex_index"  , Int32, Int32, offsetof(PlyFace,verts), 1, Uint8, Uint8, offsetof(PlyFace,nverts)},
    };
// En fait, "vertex_indices" et "vertex_index" sont synonymes. La norme actuelle dit "vertex_indices", mais l'autre forme semble exister dans d'anciens fichiers.

    /*** the PLY object ***/

    static int nverts,nfaces;
    static PlyVertex **vlist;
    static PlyFace   **flist;

    static PlyOtherProp *vert_other,*face_other;

    static int per_vertex_color = 0;
    static int has_normals = 0;

    if(verbose)
        printf("Entrée de BddInter::LoadPLY_Stanford\n");
    printf("... En cours d'implémentation ...\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f = fopen(buffer.data(),"r");	    //ouverture du fichier

// Lecture et affichage du header

// Ci-dessous, garder les fgets ... pour détection de problèmes de header, mais on peut mettre en commentaire les 2 printf suivants (ou via if(verbose)...)

    fgets(s1,160,f);
    printf ("%s", s1);                  // le \n est déjà dans s1

    while (1) {
        buf = fgets(s1,160,f);
        if (buf == NULL) {
            fprintf (stderr, "Fin de fichier atteinte ... Abandon de la lecture !\n");
            type = -1;
            break;
        }
        printf ("%s", s1);              // le \n est déjà dans s1

        if (strncmp(s1, "comment", 7)) compteur++;  // Ne pas compter les lignes "comment"

        if (compteur > 50) {
            fprintf (stderr, "Le Header semble trop long ... Abandon de la lecture !\n");
            type = -1;
            break;
        }

        if (strncmp(s1, "end_header", 10) == 0)
            break;
    }

    if (type < 0) {
        fclose(f);
        printf("Sortie en Erreur de BddInter::LoadPLY_Stanford\n");
        return;
    }

    rewind(f);  // Repositionner le fichier à son début

    /*** Read in the original PLY object ***/
    in_ply  = read_ply (f);

    if (in_ply == NULL) {
        type = -1;
        printf("Sortie en Erreur de BddInter::LoadPLY_Stanford\n");
        return;
    }

    printf("num_elem_types %d\n",in_ply->num_elem_types);
    for (i = 0; i < in_ply->num_elem_types; i++) {

        /* prepare to read the i'th list of elements */
        elem_name = setup_element_read_ply (in_ply, i, &elem_count);

        if (equal_strings ((char*)"vertex", elem_name)) {

            /* create a vertex list to hold all the vertices */
            vlist = (PlyVertex **) malloc (sizeof (PlyVertex *) * elem_count);
            nverts = elem_count;

            printf("nprops         %d\n",in_ply->elems[i]->nprops);
            printf("nverts         %d\n",nverts);

            /* set up for getting vertex elements */

            setup_property_ply (in_ply, &vert_props[0]);
            setup_property_ply (in_ply, &vert_props[1]);
            setup_property_ply (in_ply, &vert_props[2]);

            for (j = 0; j < in_ply->elems[i]->nprops; j++) {
                PlyProperty *prop;
                prop = in_ply->elems[i]->props[j];
                if (equal_strings ((char*)"r", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[3]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"g", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[4]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"b", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[5]);
                    per_vertex_color = 1;
                }
                if (equal_strings ((char*)"nx", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[6]);
                    has_normals = 1;
                }
                if (equal_strings ((char*)"ny", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[7]);
                    has_normals = 1;
                }
                if (equal_strings ((char*)"nz", prop->name)) {
                    setup_property_ply (in_ply, &vert_props[8]);
                    has_normals = 1;
                }
            }

            vert_other = get_other_properties_ply (in_ply, offsetof(PlyVertex, other_props));

            /* grab all the vertex elements */
            for (j = 0; j < elem_count; j++) {
                vlist[j] = (PlyVertex *) malloc (sizeof (PlyVertex));
                vlist[j]->r = 1;
                vlist[j]->g = 1;
                vlist[j]->b = 1;
                get_element_ply (in_ply, (void *) vlist[j]);
            }

        } else if (equal_strings ((char*)"face", elem_name)) {

            /* create a list to hold all the face elements */
            flist = (PlyFace **) malloc (sizeof (PlyFace *) * elem_count);
            nfaces = elem_count;

            printf("nfaces         %d\n",nfaces);

            /* set up for getting face elements */

            PlyProperty *propE;
            propE = in_ply->elems[i]->props[0];
            if (equal_strings ((char*)"vertex_indices", propE->name)) {
                setup_property_ply (in_ply, &face_props[0]);
            } else {
//            if (equal_strings ((char*)"vertex_index", propE->name)) { // vertex_indices et vertext_index (version ply antérieure ?) sont en fait synonymes
                setup_property_ply (in_ply, &face_props[1]);
            }
            face_other = get_other_properties_ply (in_ply, offsetof(PlyFace, other_props));

            /* grab all the face elements */
            for (j = 0; j < elem_count; j++) {
                flist[j] = (PlyFace *) malloc (sizeof (PlyFace));
                get_element_ply (in_ply, (void *) flist[j]);
            }
        } else
            get_other_element_ply (in_ply);
    }

    close_ply(in_ply);
    free_ply (in_ply) ;

    if (vert_other == NULL || face_other == NULL) {
        // Ignorer ? faire quelque-chose ? Permet d'éviter un warning de compilation sur "variable set but not used" !
    }

    str.Printf(_T("<OBJET> %d "),0+Numero_base);
    str += wxFileName(buffer.data()).GetName();     // Récupérer le nom du fichier sans l'extension .ply ni le path comme nom d'objet
    makeobjet();

    // Création des sommets et des facettes

    this->Objetlist[indiceObjet_courant].Nb_sommets = nverts;
    this->N_elements = nverts;
    str.clear();
    makesommet();
    if (has_normals) makevecteur();

    if (per_vertex_color) {
        // colorisier un groupe peut-être, mais comme c'est par sommet et non par facettes ??? permet d'éviter un warning de compilation
    }

    for (i = 0; i < nverts; i++) {
        this->N_elements = i+1;
//        printf ("    %g %g %g \n", vlist[i]->x, vlist[i]->y, vlist[i]->z);
        this->Setxyz(vlist[i]->x, vlist[i]->y, vlist[i]->z);
        make1sommet();
        if (has_normals) {
            this->Setxyz(vlist[i]->nx, vlist[i]->ny, vlist[i]->nz);
            make1vecteur();
        }
        free(vlist[i]); // Libérer la mémoire de chaque vertex (malloc de vlist[i])
    }
    free(vlist);        // Libérer la mémoire des vertex (malloc de vlist global)

    this->N_elements = nfaces;
    makeface();
    makenormale();
    makeaspect_face();
    if (has_normals) makeluminance();

    if (has_normals) {
        this->Objetlist[indiceObjet_courant].flat=false;
        this->Objetlist[indiceObjet_courant].Nb_luminances= nfaces; // déjà fait via makeluminance
        this->Objetlist[indiceObjet_courant].Nb_vecteurs  = nverts; //  ""       via makevecteur
    } else {
        this->Objetlist[indiceObjet_courant].flat=true;
        this->Objetlist[indiceObjet_courant].Nb_luminances= 0;
        this->Objetlist[indiceObjet_courant].Nb_vecteurs  = 0;
    }

    str.clear();

    for (i=0; i<nfaces; i++) {
        this->N_elements = i+1;
        int nb_som = flist[i]->nverts;
        Numeros.resize(nb_som);
        for (j=0; j<nb_som; j++) Numeros[j]=(flist[i]->verts[j]) +1;
        this->Set_numeros(Numeros) ;
        make1face();
        facette_courante = &(this->Objetlist[indiceObjet_courant].Facelist[i]);
        facette_courante->groupe     = groupe_def;
        facette_courante->codmatface = codmatface_def;
        if (has_normals) {
            make1luminance();
            facette_courante->flat = false;
        } else {
            facette_courante->flat = true;
        }
        Calcul_Normale_Barycentre(indiceObjet_courant,i);
        free(flist[i]);     // Libérer la mémoire de chaque Ply facettes (malloc de flist[i])
    }
    free(flist);            // Libérer la mémoire de toutes les Ply facettes (malloc de flist global)

//    type = -1;  // Provisoire

    m_loaded = true;
    m_gllist = 0;

    if(verbose)
        printf("Sortie de BddInter::LoadPLY_Stanford\n");

}

void BddInter::LoadOFF()
{
/*
 *  Lecture d'un Fichier de polygones au format OFF
 *  Un seul objet, crée les normales aux barycentres.
 */

    char nom_obj[80];
    int n;
    unsigned int i, nfac, npoint, nb_fac, nb_p, nsommets ;
    unsigned int o=0;		            // pour compter le nombre d'objets
    float vx,vy,vz;
    unsigned int Nb_objets;
    std::vector<int>   Numeros;
    std::vector<float> xyz_point;

    if(verbose) printf("Entree de BddInter::LoadOFF\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f=fopen(buffer.data(),"r");	//ouverture du fichier
    fgets(s1,160,f) ;
    if (strncmp(s1,"OFF",3)) {
        printf("Fichier .off, mais pas de type Object File Format !\n");    // Doit obligatoirement commencer par OFF en 1ère ligne
        fclose(f);
        type = -1;
        return ;
    }

     printf("Fichier de type Object File Format : %s",s1) ;
     Nb_objets = 1 ;     // A priori, 1 seul objet par fichier
//    if (svg_entete && temp != NULL) {
//        fprintf(temp,"#### Fichier Oktal : Transformation d'un Fichier OFF ####\n\n") ;
//    }

    for (o=0 ; o< Nb_objets ; o++) {
        sprintf(nom_obj, "Objet OFF - %d",o) ;

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),o);//+Numero_base);
        str += NomObj;
        makeobjet();
//        indiceObjet_courant = this->Objetlist.size() -1; //o;

        // Lecture du nombre de sommets et de facettes
        fgets(s1,100,f) ;
        while (s1[0] == '#') fgets(s1,100,f);   // Sauter les lignes commençant par # ... Il faudrait aussi sauter les lignes blanches (vides ou que des espaces)
        sscanf(s1,"%d%d",&nb_p,&nb_fac) ;

        this->Objetlist[indiceObjet_courant].Nb_sommets = nb_p;
        this->N_elements = nb_p;
        str.clear();
        makesommet();

        // Lecture des points
        str.clear();
        for (npoint = 1 ; npoint <= nb_p ; npoint++) {
            // Lecture des x, y et z
            fgets(s1,100,f) ;
            while (s1[0] == '#') fgets(s1,100,f);
            sscanf(s1,"%f%f%f", &vx, &vy, &vz);
            this->N_elements = npoint;
            this->Setxyz(vx,vy,vz);
            this->make1sommet();
        }

        this->N_elements = nb_fac;
        makeface();
        makenormale();
        makeaspect_face();

        // Lecture des facettes
        for (nfac=1 ; nfac <= nb_fac ; nfac++) {
            fscanf(f,"%d",&nsommets) ;
            Numeros.resize(nsommets);
            for (i=0 ; i< nsommets ; i++) {
                fscanf(f,"%d", &n) ;
                Numeros[i] = n+1; // Ajouter 1 au n°, car on commence à 1 et non 0
            }
            str.clear();
            this->N_elements = nfac;
            this->Set_numeros(Numeros) ;
            make1face();
            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].flat = true; // provisoire

            fgets(s1,200,f) ; //Terminer la lecture de cette ligne. Il reste parfois des données (normales ? autres ?)

            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].groupe     = groupe_def;
            this->Objetlist[indiceObjet_courant].Facelist[nfac-1].codmatface = codmatface_def;
        }
        this->Objetlist[indiceObjet_courant].Nb_facettes  = nb_fac;
        this->Objetlist[indiceObjet_courant].Nb_normales  = nb_fac;
        this->Objetlist[indiceObjet_courant].Nb_luminances= 0;    // Dans le format OFF, il n'y a pas de normales aux sommets
        this->Objetlist[indiceObjet_courant].Nb_vecteurs  = 0;    // enregistrées dans le fichier de Bdd
        this->Objetlist[indiceObjet_courant].flat=true;           // A la lecture, on considère que l'objet n'est composé que de facettes planes
        for (i=0; i<nb_fac; i++) Calcul_Normale_Barycentre(indiceObjet_courant,i);

        // Il n'y a pas de normales aux sommets dans ce format.
        // Le calcul sera fait éventuellement dans create_bdd si la case adéquate ("Calcul des normales dès la lecture") est cochée dans le dialogue "Préférences".

    }

    m_loaded = true;
    m_gllist = 0;

    fclose(f);
    if(verbose) printf("Sortie de BddInter::LoadOFF\n");
}

void BddInter::LoadSTL() {
/*
 * Lecture d'un fichier STL (stéréolithographique, ou encore Standard Triangles Language ou encore Standard Tessellation Language)
 * 2 formats reconnus : Ascii et Binaire
 * 1 seul objet, que des facettes triangulaires, coordonnées des sommets dans chaque facette => plusieurs fois le même point => simplification possible
 */

    bool binary;            // Pour identifier/différentier un fichier .stl de type Ascii ou Binaire
    char *cptr, *nom_obj;
    unsigned int i,j,ilen;
    unsigned int numero_facette;
    unsigned int numero_sommet,numero_sommetB;
    std::vector<int> Numeros;
    float vx,vy,vz;
    Face *facette_courante;
    unsigned int nb_facettes_loc=0;
    unsigned int nb_sommets_loc=0;

    if(verbose) printf("Entree de BddInter::LoadSTL\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }
    f=fopen(buffer.data(),"r");     //ouverture du fichier
    cptr=fgets(s1,81,f) ;           // 80 caractères +1 pour permettre un \n
    s1[81] = '\0';                  // Simple précaution
    printf("longueur de l'entête : %d\n",(int)strlen(s1));
    if (cptr == NULL) {
        printf("Fichier vide !\n");
        fclose(f);
        type = -1;
        return;
    }

    binary = true;
    cptr   = s1;
    for (i=0;i<80;i++,cptr++) {
        if (*cptr == 0x0A) {
            binary = false;     // Si le caractère LF est trouvé dans les 80 premiers du fichier, c'est que ce n'est pas un fichier binaire, mais plutôt Ascii !
            break;              // => Inutile de poursuivre la recherche
        }
    }

    if (!binary && strncmp(s1,"solid ",6)) {    // Fichier Ascii, mais ne contenant pas la chaîne "solid " sur les 6 premiers caractères !
        printf("Fichier .stl, mais pas de type Ascii stéréolithographique !\n");
        fclose(f);
        type = -1;
        return ;
    }

    if (binary) {
        printf("Fichier .stl binaire !\n");
        if (!strncmp(s1,"solid ",6)) {      // Débute par "solid ", mais c'est un fichier binaire
            nom_obj = strdup(s1+6);         // Le nom commence donc au 7ème caractère
        } else {
            nom_obj = strdup(s1);           // Retenir toute la chaîne comme nom
        }
    } else {
        printf("Fichier .stl Ascii !\n");
        nom_obj = strdup(s1+6);             // Le nom commence donc au 7ème caractère
    }
    ilen = strlen(nom_obj);
    cptr = nom_obj + ilen-1;
    while (*cptr == '\n') {                 // éliminer un \n de fin (ou evt. plusieurs)
        *cptr = '\0';
        cptr--;
    }
    while (*cptr == ' ') {                  // éliminer les espaces de fin
        *cptr = '\0';
        cptr--;
    }

    printf("Nom de l'objet : %s\n",nom_obj);

    if (!binary) {

// Lecture d'un fichier STL Ascii

        // Compter le nombre de facettes et sommets (1ère lecture)
        rewind(f);
        fgets(s1,160,f) ;                   // Les lignes contiennent moins de 160 caractères (plutôt 60). On prévoie large donc.
        while (strncmp(s1,"endsolid ",9)) { // Lire tant que la chaîne endsolid n'a pas été trouvée. ERREUR : Que se passe t-il si elle n'existe pas ???
            cptr=fgets(s1,160,f) ;          // Peut-il y avoir des lignes vides ?
            if (cptr == NULL) {
                printf("Erreur\n");         // Ligne "endsolid " non trouvée ?
                return;
            }
            cptr = s1;
            while (*cptr == ' ') cptr++;    // Pour éliminer les espaces de début
            if (!strncmp(cptr,"facet normal ",13)) nb_facettes_loc++;
            if (!strncmp(cptr,"vertex ",7))        nb_sommets_loc++ ;
        }
        rewind(f);

        // Remplir l'objet (2ème lecture)

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),0+Numero_base);    // Voir si Numero_base est utile (en cas de lecture de plusieurs fichiers ?)
        str += NomObj;
        makeobjet();

        // Création des sommets et des facettes

        this->Objetlist[indiceObjet_courant].Nb_sommets = nb_sommets_loc;
        this->N_elements = nb_sommets_loc;
        str.clear();
        makesommet();

        this->N_elements = nb_facettes_loc;
        makeface();
        makenormale();
        makeaspect_face();

        fgets(s1,160,f) ;
        numero_sommet = 1;
        numero_sommetB= 1;
        numero_facette= 1;
        Numeros.resize(3);

        while (strncmp(s1,"endsolid ",9)) {
            fgets(s1,160,f) ;
            cptr = s1;
            while (*cptr == ' ') cptr++;
            if (!strncmp(cptr,"facet normal ",13)) {
                cptr+=13;
                for (i=0 ; i< 3 ; i++) {
                    Numeros[i] = numero_sommet++;
                }
                str.clear();
                this->N_elements = numero_facette;
                this->Set_numeros(Numeros) ;
                make1face();
                facette_courante = &(this->Objetlist[indiceObjet_courant].Facelist[numero_facette-1]);
                facette_courante->flat       = true;
                facette_courante->groupe     = groupe_def;
                facette_courante->codmatface = codmatface_def;

                sscanf(cptr,"%f%f%f", &vx, &vy, &vz);   // 3 composantes de la normale à la facette
                this->Setxyz(vx,vy,vz);
                make1normale();

                numero_facette++;
            }
            if (!strncmp(cptr,"vertex ",7)) {           // Normalement 3 lignes vertex consécutives => 3 sommets d'un triangle
                cptr+=7;
                str.clear();
                sscanf(cptr,"%f%f%f", &vx, &vy, &vz);
                this->N_elements = numero_sommetB;
                this->Setxyz(vx,vy,vz);
                make1sommet();

                numero_sommetB++;
            }
        }

    } else {

// Lecture d'un fichier STL Binaire
        bool erreur_couleurs = false;
        int index=0;
        UINT32 nb_triangles;            // C'est un Unsigned Integer sur 32 bits (en fait <=> int sur gcc 32 ou 64 bits)
        UINT16 Attribute ;              // Unsigned Integer sur 16 bits (<=> unsigned short en gcc 32 ou 64 bits)

        fclose(f);                      // Fermer le fichier car ouvert précédemment en mode Ascii
        f=fopen(buffer.data(),"rb");    // Le réouvrir en mode lecture+binaire
        fseek(f,80,0);                  // passer les 80 premiers octets d'entête (déjà lus et décodés)

        fread(&nb_triangles,sizeof(UINT32),1,f);
        printf("Nombre de triangles : %d\n",nb_triangles);

        // Remplir l'objet

#if wxCHECK_VERSION(3,0,0)
        wxString NomObj=wxString(nom_obj);
#else
        wxString NomObj=wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        str.Printf(_T("<OBJET> %d "),0+Numero_base);    // Numero_base ?? voir plus haut.
        str += NomObj;
        makeobjet();

        // Création des sommets et des facettes

        nb_sommets_loc = nb_triangles*3;                // Facettes triangulaires, donc 3 sommets
        nb_facettes_loc= nb_triangles;

        this->Objetlist[indiceObjet_courant].Nb_sommets = nb_sommets_loc;
        this->N_elements = nb_sommets_loc;
        str.clear();
        makesommet();

        this->N_elements = nb_facettes_loc;
        makeface();
        makenormale();
        makeaspect_face();

        numero_sommet = 1;
        numero_sommetB= 1;
        numero_facette= 1;
        Numeros.resize(3);

        listeMateriaux.clear();

        for (i=1; i<=nb_facettes_loc; i++) {
            for (j=0 ; j< 3 ; j++) {
                Numeros[j] = numero_sommet++;
            }
            str.clear();
            this->N_elements = numero_facette;
            this->Set_numeros(Numeros) ;
            make1face();
            facette_courante = &(this->Objetlist[indiceObjet_courant].Facelist[numero_facette-1]);
            facette_courante->flat       = true;        // provisoire
            facette_courante->groupe     = groupe_def;
            facette_courante->codmatface = codmatface_def;

            fread(xyz,sizeof(float),3,f);               // Trois composantes de la normale. Ce doit être des float32 (OK sous gcc GNU 32 et 64)
            make1normale();

            numero_facette++;

            for (j=0 ; j<3 ; j++) {                     // 3 sommets d'un triangle
                fread(xyz,sizeof(float),3,f);           // 3*3 coordonnées des sommets
                this->N_elements = numero_sommetB;
                make1sommet();
                numero_sommetB++;
            }

            fread(&Attribute,sizeof(UINT16),1,f);       // Entier 16 bits de réserve (très peu utilisé apparemment)

            if (Attribute > 0 || !listeMateriaux.empty()) {
                int materiau = Attribute;
                auto it = std::find(listeMateriaux.begin(),listeMateriaux.end(),materiau);  // Est-il déjà dans la liste ?
                if (it == listeMateriaux.end() || listeMateriaux.empty()) {                 // Non
                    listeMateriaux.push_back(materiau);                                     // L'ajouter à la liste des matériaux
                    // Les 6 lignes suivantes permettent de coder materiau avec la couleur trouvée dans Attribute
                    index = listeMateriaux.size();
                    if (index < nb_couleurs) {
                        MatDiffuse_avionG[index][0] = MatAmbient_avionG[index][0] = (materiau % 32) / 31.;          // Bits  0 à 4  -> rouge entre 0. et 1.
                        MatDiffuse_avionG[index][1] = MatAmbient_avionG[index][1] = ((materiau >>  5) % 32) / 31.;  // Bits  5 à 9  -> vert     "
                        MatDiffuse_avionG[index][2] = MatAmbient_avionG[index][2] = ((materiau >> 10) % 32) / 31.;  // Bits 10 à 14 -> bleu     "
                    } else erreur_couleurs = true;
                }
                int rang = 1;
                for (it = listeMateriaux.begin(); it != listeMateriaux.end(); ++it,++rang) {
                    if (*it == materiau) break;         // materiau identifié => sortir du for pour obtenir son rang
                }
                facette_courante->codmatface = rang;    // Coder le matériau pas son rang dans la liste
                facette_courante->groupe     = rang-1;  // Pour donner une valeur différente du matériau
            }
        }
        if (erreur_couleurs) {
            // les tableaux MatAmbient_avionG et MatDiffuse_avionG sont trops petits
            wxString wxMessage;
            wxMessage.clear();
            wxMessage.Printf(_T("Trop de couleurs/valeurs Attribute différentes dans ce ficher .stl binaire.\n%d valeurs trouvées, maximum : %d"),index,nb_couleurs-1);
            wxMessage  +=    _T("\nAgrandir MatAmbient_avionG et MatDiffuse_avionG");
            DisplayMessage(wxMessage,true);
        }
    }

    this->Objetlist[indiceObjet_courant].Nb_luminances= 0;    // Dans le format STL, il n'y a pas de normales aux sommets
    this->Objetlist[indiceObjet_courant].Nb_vecteurs  = 0;    // enregistrées dans le fichier de Bdd
    this->Objetlist[indiceObjet_courant].flat=true;           // A la lecture, on considère que l'objet n'est composé que de facettes planes

    m_loaded = true ; //true;
    m_gllist = 0;

    fclose(f);
//    type = -1;  // Pour le moment
//    Update();
    if(verbose) printf("Sortie de BddInter::LoadSTL\n");
}

void BddInter::Load3DS()
{
/*
 *  Lecture d'un Fichier de polygones au format Autodesk 3DS
 *  Identifie éventuellement plusieurs objets, crée des normales.
 *  Une création de groupes faite ?????????????????????????????????
 *  Utilise la librairie lib3ds (d'après l'exemple 3ds2m et 3dsplay)
 */
    int im ;
    Lib3dsNode *p, *node ;
    int meshes=0;
    int nnodes=0;
    int i;
//    int indiceObjet;

    if(verbose) printf("Entree de BddInter::Load3DS\n");

    wxCharBuffer buffer=this->file.mb_str();

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    f3ds=lib3ds_file_open(buffer.data());
    if (!f3ds) {
        printf("Le fichier %s n'est pas un fichier 3ds\n",buffer.data());
        type = -1;
        return;
    }

    /* Pas de nodes?  En fabriquer pour favoriser l'affichage (cf 3dsplay.c). */
    if( !f3ds->nodes ) {
//        Lib3dsNode *node;

        sprintf(Message,"Création de Nodes, car ce fichier .3ds n'en a pas !\n") ;
        printf(utf8_To_ibm(Message)) ;

        for (i = 0 ; i < f3ds->nmeshes; ++i) {
            Lib3dsMesh *mesh = f3ds->meshes[i];
            node = lib3ds_node_new(LIB3DS_NODE_MESH_INSTANCE);
            strcpy(node->name, mesh->name);
            lib3ds_file_insert_node(f3ds, node, NULL);
        }
        sprintf(Message,"%d meshes lus et autant de nodes créés\n",f3ds->nmeshes);
        printf(utf8_To_ibm(Message)) ;
    }

    lib3ds_file_eval(f3ds, 0.0f);   // Indispensable pour configurer proprement les matrices des nodes

    // Code pas très utile ! Sert juste à vérifier 2 méthodes de comptage qui ne donnent pas toujours la même chose !
//    for (node=f3ds->nodes ; node!=NULL ; node=node->next ) {
//        nnodes++;                               // Compter le nombre de nodes (noeuds)
//        printf("Node : %s\n",node->name);
//        for (p=node->childs ; p!=NULL ; p=p->next) {
//            nnodes++;
//            printf("Node : %s\n",p->name);
//        }
//        /*       if (nnode) {
//                    if (strcmp(nnode, p->name)!=0) {
//                        continue;
//                    }
//                }*/
//    }
//    printf("Nodes : %d\n",nnodes);

// Appel de compter_nodes plutôt que faire nb_objets = nnodes
    o_3ds=0;
    for (p = f3ds->nodes; p != 0; p = p->next) {   // Pas forcément utile car objets construits au fur et à mesure
        compter_nodes(p);
    }
    printf("sortie de compter_nodes : o_3ds = %d\n",o_3ds);
//    if (o_3ds != nnodes) {
//        sprintf(Message,"Nombre de nodes retenus : %d, différent du nombre initial : %d\n",o_3ds,nnodes);
//        printf(utf8_To_ibm(Message)) ;
////        printf("Nombre de nodes retenus : %d, different du nombre initial : %d\n",o_3ds,nnodes);
//    }
//    cel->nb_objet = o_3ds ; //meshes;
    meshes=o_3ds;
    printf("%d objets\n",meshes);

    if (meshes == 0) {
        sprintf(Message,"Erreur dans le fichier 3DS : ne contient pas d'objets (meshes)\n") ;
        printf(utf8_To_ibm(Message)) ;
#ifdef WIN32
        system("pause") ;
#endif
//        exit(2) ;
        return;
    }

// Seconde analyse des objets avec appel récursif des nodes
    o_3ds      = 0 ; //indiceObjet_courant = o_3ds;
    nb_mat_3ds = 0 ;
    for (p = f3ds->nodes; p != 0; p = p->next) {
        decoder_node(p);
    }

//    numObjet_suiv += ((o_3ds+10)/10)*10 ; // Pour arrondir à la dizaine supérieure
    printf("\nnb_mat = %d\n",nb_mat_3ds) ;
    if (nb_mat_3ds >= 1) {
        sprintf(Message,"Liste des matériaux du fichier 3ds\n") ;
        printf(utf8_To_ibm(Message));
//        printf("Liste des materiaux du fichier 3ds\n") ;
        for (im=0; im < nb_mat_3ds ; im++) {
            sprintf(Message,"%4d %s\n",im+1, tab_mat[im]);
            printf(utf8_To_ibm(Message));
        }
    }
    sprintf(Message,"\nFin de la lecture des données.\n");
    printf(utf8_To_ibm(Message));

    lib3ds_file_free(f3ds);

    m_loaded = true;
    m_gllist = 0;

    if(verbose) printf("Sortie de BddInter::Load3DS\n");
}

int BddInter::mesh_count_smoothing(Lib3dsMesh *mesh)
{
    int i;
    int compteur_smoothing = 0;

    if (!mesh->nfaces) {
        return (0);
    }

    for (i = 0; i < mesh->nfaces; ++i) {
        Lib3dsFace *f = &mesh->faces[i];
        if (f->smoothing_group) compteur_smoothing++;
    }
    return (compteur_smoothing);
}

int BddInter::compter_nodes (Lib3dsNode *node)
{
    Lib3dsNode *p ;

    for (p = node->childs; p != 0 ; p = p->next) {
        compter_nodes(p);
    }

    if (node->type == LIB3DS_NODE_MESH_INSTANCE) {
        if (strcmp(node->name,"$$$DUMMY") == 0 ) {
            return (0);
        }
        o_3ds++;
    }
    return (1);
}

char * BddInter::Lire_chaine( char st [])
{
// élimine les blancs de début et de fin de chaîne

    char *cptr;          //Pointeur de chaîne de caractères
    char c ;

    cptr = strrchr(st,'\n') ; // élimination du terminateur de ligne.
    if (cptr != NULL) {
        *cptr = '\0' ;
    }
    cptr = strrchr(st,'\r') ; // élimination d'un éventuel cr.
    if (cptr != NULL) {
        *cptr = '\0' ;
    }
    c     = st[0] ;
    st[0] = 'x'   ;                         // Pour limiter le -- dans le while suivant
    cptr = strchr(st,'\0') ;                // Pointe la fin de chaîne
	while(*--cptr == ' ') {*cptr = '\0' ;}  // élimine les blancs de fin de chaîne.
    cptr = st ;                             // Pointe le début de chaîne
    st[0] = c ;                             // Restituer le caractère
    while(*cptr != '\0') {
        if (*cptr == '\t') *cptr = ' ' ;    // Remplace les tab par des espaces dans la chaîne
        cptr++ ;
    }
    cptr = st ;
	while(*cptr == ' ')  {cptr++ ;}         // élimine les blancs de début de chaîne.
    if (*cptr == '\0')
        cptr = strcpy(st, " ") ;            // Laisser au moins un blanc
    return (cptr) ;
}

void BddInter::Affiche_Matrice(float M[4][4]) {
    int i,j;
    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) printf("%12.5f ",M[j][i]);
        printf("\n");
    }
    printf("\n");
}

int BddInter::decoder_node (Lib3dsNode *node)
{
/*  Lecture d'un Fichier de polygones au format Autodesk 3DS
 *  Identifie éventuellement plusieurs objets, crée des normales aux barycentres des facettes.
 *  Une création de groupes faite ?????????????????????????????????
 *  Utilise la librairie lib3ds (d'après l'exemple 3ds2m ou 3Dsplay, mais quelques soucis, voir ci-dessus ! )
 */
    char nom_obj[80];
    int im;
    unsigned int i,k, nfac, npoint, nb_fac, nb_p, found ;
    unsigned int o=0;		            // pour compter le nombre d'objets
    int num_mat ;
    int index;
    float matrice[4][4], inv_matrice[4][4] ;
    float matrice_w[4][4];
    Object *objet_courant;
    Face   *facette_courante;

    Lib3dsMesh *mesh;
    float pos[3],pos2[3];
    Lib3dsMaterial *mat3ds = 0;
    Lib3dsMeshInstanceNode *n;
    char nom_mat_tmp[80];
    std::vector<int>   Numeros;
    std::vector<float> xyz_point;
    int N0,N1,N2;

    Numeros.resize(3);

    {
        Lib3dsNode *p ;
        for (p = node->childs; p != 0 ; p = p->next) {
            decoder_node(p);
        }
    }

    if (node->type == LIB3DS_NODE_MESH_INSTANCE) {

        n = (Lib3dsMeshInstanceNode*)node;

        if (strcmp(node->name,"$$$DUMMY") == 0) {
            return (0);
        }

        index = lib3ds_file_mesh_by_name(f3ds, n->instance_name);
        if( index < 0 )
            index = lib3ds_file_mesh_by_name(f3ds, node->name);     // 2ème essai, si pas trouvé par instance_name
        if (index < 0) {
            return (0);                                             // Si toujours pas trouvé, sortir
        }

        mesh = f3ds->meshes[index];

//        printf("user_id : %d\n",mesh->user_id);

        strcpy(nom_obj, Lire_chaine(mesh->name)) ;
//        printf("Objet :%4d, Nom : %s\n",o,nom_obj);

        wxString wx_num_obj;
        wx_num_obj.Printf(_T("%d "),(int)Objetlist.size());
        str.Printf(_T("<OBJET> "));
        str += wx_num_obj;

#if wxCHECK_VERSION(3,0,0)
        wxString wxNom = wxString(nom_obj);
#else
        wxString wxNom = wxString::FromAscii(nom_obj);
#endif // wxCHECK_VERSION

        // Récupération du nombre de points
        nb_p = mesh->nvertices ;
        if (nb_p == 0) {
            printf("\nObjet :%4d, Nom : %s ==> VIDE. Non pris en compte !\n",o,nom_obj);
            return (0);   // Pas de points => objet vide donc l'ignorer
        }

        str += wxNom;
        makeobjet();

//        o_3ds = Objetlist.size() -1;
        o = Objetlist.size() -1;
//        o = o_3ds ;   // Plus simple que o_3ds à écrire dans les indices de tableaux

        printf("Objet :%4d, Nom : %s\n",o,nom_obj);
//        indiceObjet_courant = o;

        str.clear();
        N_elements = nb_p;
        makesommet();       // Construire les sommets
        objet_courant = &(this->Objetlist[o]) ;
        objet_courant->Nb_sommets = nb_p ;

        N_elements = 3*mesh->nfaces;
        makevecteur();  // Ici 3 vecteurs par facette. On ne tient pas compte des vecteurs communs à plusieurs facettes
        objet_courant->Nb_vecteurs = N_elements;

// Allocation de mémoire pour le calcul des normales aux sommets
        float (*normal_S)[3] = (float(*)[3])malloc(3 * 3 * sizeof(float) * mesh->nfaces);
// Calcul des normales aux sommets par la fonction de la lib3ds
        lib3ds_mesh_calculate_vertex_normals(mesh, normal_S);
        int compteur_smoothing = mesh_count_smoothing(mesh);
// Allocation de mémoire pour le calcul des normales au barycentre des facettes
        float (*normal_B)[3] = (float(*)[3])malloc(3 * sizeof(float) * mesh->nfaces);
// Calcul des normales aux barycentres par la fonction de la lib3ds
        lib3ds_mesh_calculate_face_normals(mesh, normal_B);

// Dans lib3ds, il y a aussi lib3ds_mesh_calculate_face_normals (à voir ...)

        if (test_decalage3ds) {
            printf("\nObjet : Nom : %s\n",nom_obj);
            lib3ds_matrix_copy(inv_matrice,mesh->matrix);
//            Affiche_Matrice(inv_matrice);
            lib3ds_matrix_inv(inv_matrice);
//            Affiche_Matrice(inv_matrice);
            lib3ds_matrix_copy(matrice_w,node->matrix);
//            Affiche_Matrice(matrice_w);
//            lib3ds_matrix_copy(matrice_w,  mesh->matrix);
//            lib3ds_matrix_mult(matrice,matrice_w,inv_matrice);    // Est ça ou (matrice,inv_matrice,matrice_w)
//            Affiche_Matrice(matrice_w);
//            printf("pivot : %f %f %f \n", n->pivot[0], n->pivot[1], n->pivot[2]);
//            printf("diff  : %f %f %f \n", matrice_w[3][0] +n->pivot[0], matrice_w[3][1] +n->pivot[1], matrice_w[3][2] +n->pivot[2]);
            lib3ds_matrix_translate(matrice_w, -n->pivot[0], -n->pivot[1], -n->pivot[2]);
//            Affiche_Matrice(matrice_w);
            lib3ds_matrix_mult(matrice,matrice_w,inv_matrice);
//            lib3ds_matrix_mult(matrice,inv_matrice,matrice_w);
//            Affiche_Matrice(matrice);
//            lib3ds_matrix_translate(matrice, -n->pivot[0], -n->pivot[1], -n->pivot[2]);
        }

        // Lecture et transformation des points
        for (i = 0 ; i < nb_p ; i++) {
            if (test_decalage3ds) {
                lib3ds_vector_copy(pos2, mesh->vertices[i]);
                lib3ds_vector_transform(pos,matrice,pos2);
            } else {
                lib3ds_vector_copy(pos, mesh->vertices[i]);
            }

            // Lecture des x, y et z
            N_elements = i+1;
            Setxyz(pos[0],pos[1],pos[2]);
            make1sommet();
        }

// Modification éventuelle de matrice de transformation
        if (test_decalage3ds) { // Pour la rotation des normales : matrice de rotation pure ! => ne garder que [0], [1] et [2] dans les 2 dimensions
            matrice[0][3] = matrice[3][0] =  matrice[1][3] = matrice[3][1] =  matrice[2][3] = matrice[3][2] = 0.;
            matrice[3][3] = 1;
        }

        // Récupération du nombre de facettes
        nb_fac = mesh->nfaces ;
        objet_courant->Nb_facettes = nb_fac ;
        objet_courant->Nb_normales = nb_fac;

        str.clear();
        N_elements = nb_fac;
        makeface();
        makeluminance();
        makenormale();
        makeaspect_face();

        // Lecture des facettes
        int n_vect=1;   // Numéro du 1er vecteur calculé
        for (i=0; i < nb_fac; ++i) {
            nfac = i+1 ;

            Numeros[0] = N0 = mesh->faces[i].index[0];  // Garder ces indices dans N0,... car ils vont reservir
            Numeros[1] = N1 = mesh->faces[i].index[1];
            Numeros[2] = N2 = mesh->faces[i].index[2];
            for (k=0;k<3;k++) Numeros[k]++;             // Ajouter 1 au n°, car on commence à 1 et non 0
            this->N_elements = nfac;
            Set_numeros(Numeros);
//            printf("Numeros Sommets : %d %d %d\n",Numeros[0],Numeros[1],Numeros[2]);
            str.clear();
            make1face();

// Calcul de la normale au barycentre de la facette via lib3ds_mesh_calculate_face_normals
            lib3ds_vector_copy(this->xyz, normal_B[i]);
            if (test_decalage3ds) {
                lib3ds_vector_transform(this->xyz, matrice, normal_B[i]);
            }
            this->N_elements=nfac; this->make1normale();

// Normales aux sommets
            for (int nn = 0; nn<3 ; nn++) {
                int nnp = 3*i + nn;
                lib3ds_vector_copy(this->xyz, normal_S[nnp]);
                if (test_decalage3ds) {
                    lib3ds_vector_transform(this->xyz, matrice, normal_S[nnp]); // On prend les normales qui ont été calculées, même si = normal_B correspondante
                }
                this->N_elements = n_vect++;
                str.clear();
                this->make1vecteur();
            }
            this->N_elements = nfac;
            Numeros[0]=Numeros[1]=Numeros[2] = 3*i+1;  Numeros[1] += 1 ; Numeros[2] += 2;
            Set_numeros(Numeros);
            str.clear();
            make1luminance();

// Identification des matériaux (s'il y en a)
            num_mat = -123 ;
//            mat3ds->name[0] = 0;
//            printf("i= %d, material= %d\n",i,mesh->faces[i].material);

            if (mesh->faces[i].material >= 0) {
                mat3ds = f3ds->materials[mesh->faces[i].material];
                strcpy(nom_mat_tmp,mat3ds->name);
            } else {
                sprintf(nom_mat_tmp,"Mat_Null"); // Forcer un nom de matériau
            }

            if (nb_mat_3ds == 0) {
                if (strlen(nom_mat_tmp) > 0) {
                    tab_mat[nb_mat_3ds++] = strdup(nom_mat_tmp) ;
                    num_mat = 1 ;
                }
            } else {
                found=0;
                for (im=0 ; im< nb_mat_3ds ; im++) {
                    if (!strncmp(nom_mat_tmp,tab_mat[im],strlen(tab_mat[im]))) {
                        found = 1;      // Ce matériau est déjà dans la liste
                        break;
                    }
                }
                if(found) num_mat = im+1 ;
                else {                  // Ajouter le nouveau matériau
                    if (strlen(nom_mat_tmp) > 0) {
                        tab_mat[nb_mat_3ds++] = strdup(nom_mat_tmp);
                        num_mat = nb_mat_3ds ;
                    }
                    if (nb_mat_3ds >= nb_mat_max) {
                        sprintf(Message,"ATTENTION : Trop de matériaux dans ce fichier .3ds nb_mat > %u\n",nb_mat_max);
                        printf(utf8_To_ibm(Message));
//                        printf("ATTENTION : Trop de materiaux dans ce fichier .3ds nb_mat > %u\n",nb_mat_max);
                    }
                }
            }
            facette_courante = &(objet_courant->Facelist[i]);
            facette_courante->groupe     = num_mat;
            facette_courante->codmatface = num_mat;
            facette_courante->afficher   = true;       // Pour afficher la facette
            facette_courante->deleted    = false;
            facette_courante->flat       = false;      // En réalité, la facette peut être plane : les 3 normales calculées aux sommets = normale au barycentre/
        }

// Désallocation de la mémoire de calcul des normales
        free(normal_S);
        free(normal_B);

        if (compteur_smoothing)
            objet_courant->flat = false;    // Si compteur_smoothing != 0, les normales aux sommets sont utilisables car != normales aux barycentres
        else
            objet_courant->flat = true;     // Objet sans normales aux sommets => forcer les facettes planes de cet objet

        objet_courant->afficher = true;
        objet_courant->deleted  = false;
    }
    return (0);
}

void BddInter::LoadBDD() {

/// ATTENTION : la lecture originale via tfile pose problème s'il y a des caractères accentués dans un fichier ANSI => Plante car agit comme si on avait trouvé un Eof !
///             Pas de soucis, si le fichier comportant des caractères accentués est encodé en UTF8.
/// Autres soucis : tfile semble lire tout le fichier en mémoire dès le début => s'il y a un caractère erronné dedans (comme dans f16_celar.bdd, ligne 11052)
///                 c'est comme ci-dessus => le prend pour un Eof et le fichier n'est pas décodé.
///             OK avec wxFileInputStream + wxTextInputStream c'est mieux car la ligne est certes mal lue, mais ça n'empêche pas de lire la suite !

//!             Revoir le calcul des Luminances => plante sur certains fichiers celar à cause d'erreur d'indicage (variable compteur >= 96 sur 1er objet
//!             de EuroFigh_8.bdd par exemple !

//! On pourrait décoder d'autres mots comme OMBRAGE / SHADING (ce qui permettrait d'imposer des facettes planes ou avec lissage de Gouraud/Phong ou ...)

    int x1=0;
    unsigned int i;
    int mode_lecture= 0;
    int non_retenus = 0;
    unsigned int NbFacettes   = 0;
    unsigned int NbSommets    = 0;
    unsigned int NbNormales   = 0;
    unsigned int NbAspectFaces= 0;
    unsigned int NbLuminances = 0;
    unsigned int NbVecteurs   = 0;
    unsigned int indiceObjet  = 0;
    unsigned int NbMatrices   = 0;

    int N_elements_prec;

#define notFound std::string::npos      // Définition sous forme de macro compatible 32 / 64 bits (notFound ne peut pas être une variable car le type diffère en 32 et 64 bits)

    char  c_ligne[300],mot_cle[300];
    int j, k, Nb;
    double matrice[16];

    bool ligne_utf8;

    if(verbose) printf("Entree de BddInter::LoadBdd\n");

    wxCharBuffer buffer=this->file.mb_str();
    std::ifstream fichierBdd (buffer.data());

    if(m_gllist != 0) {
        glDeleteLists(glliste_objets,1);
        m_gllist = 0;
    }

    while(getline(fichierBdd, ligne)) { // Lit ligne par ligne jusqu'à la fin du fichier (si EoF getline retourne eof())
        str.clear();
        while (ligne.back() == ' ') ligne.pop_back();   // Eliminer les éventuels espaces de fin de ligne
        if (ligne.empty())   continue;                  // Si la ligne est vide, passer à la suivante
        if (ligne[0] == '#') continue;                  // Si c'est une ligne de commentaires, passer à la suivante

#if wxCHECK_VERSION(3,0,0)
        str = wxString(ligne);
#else
        str = wxString::FromAscii(ligne.c_str());
#endif // wxCHECK_VERSION

        if((ligne.find("<OBJET>") != notFound) || (ligne.find("<OBJECT>") != notFound)) { // La ligne contient <OBJET> (ou <OBJECT> en version SDM anglaise)
//            printf("<OBJET> %d %d %d\n",ligne.find("<OBJET>"),ligne.find("<OBJECT>"),std::string::npos);
//            printf("taille %d\n",Objetlist.size());
            if (Objetlist.size() != 0) {
//                printf("Nb_facettes : %d, Nb_sommets : %d\n",Objetlist[Objetlist.size()-1].Nb_facettes,(Objetlist.end()-1)->Nb_sommets);
//                printf("Nb_facettes : %d\n",(Objetlist.end()-1)->Nb_facettes);
                if ((Objetlist.end()-1)->Nb_facettes == 0) {
                    printf("L'objet %s est vide => non pris en compte\n",(Objetlist.end()-1)->GetName());
                    Objetlist.erase(Objetlist.end()-1); // On supprime cet objet s'il est vide (car sans facettes)
                    non_retenus++;
                }
            }

            ligne_utf8 = is_utf8(ligne.data());     // La ligne est-elle en utf8 ?
            if(!ligne_utf8) str = str.utf8_str();   // Ligne non utf8 (donc probablement Ansi) : la convertir en utf8

            makeobjet();
            mode_lecture = 0;
            indiceObjet  = Objetlist.size() -1;      // Numéro d'indice de l'objet à lire
//            indiceObjet_courant = indiceObjet;      // Enregistrer ce numéro d'indice dans BddInter
        } else if((ligne.find("<FACE>") != notFound) || (ligne.find("<POLYGON>") != notFound)) {
            makeface();
            mode_lecture = 1;
            NbFacettes   = this->Objetlist[indiceObjet].Nb_facettes;
            for (i=0; i<NbFacettes; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();    // Eliminer les éventuels espaces de fin de ligne
                while (ligne.empty()) {
                    getline(fichierBdd,ligne);
                    while (ligne.back() == ' ') ligne.pop_back();
                }
                str.clear();
                std::istringstream ss(ligne);
                ss >> N_elements;   // en fait ici, numéro de la facette
                ss >> Nb;           // ici, nombre de sommets de la facette
                NumerosSommets.resize(Nb);
                for (j=0; j<Nb ; j++) ss >> NumerosSommets[j];
                make1face();
            }
            mode_lecture = -1;
        } else if((ligne.find("<SOMMET>") != notFound) || (ligne.find("<VERTEX>") != notFound)) {
            makesommet();
            mode_lecture = 2;
            NbSommets = this->Objetlist[indiceObjet].Nb_sommets;
            str.clear();
            N_elements_prec = 0;
            for (i=0; i<NbSommets; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne);
                    while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
                make1sommet();
                if (N_elements - N_elements_prec != 1)
                    printf("==> Erreur possible :\n    Sommet %d (indice %d, Objet d'indice %d) rupture de sequence\n",N_elements,i,indiceObjet);

                N_elements_prec = N_elements;
            }
            mode_lecture = -1;
        } else if((ligne.find("<NORMALE>") != notFound) || (ligne.find("<POLY_NORMAL>") != notFound)) {
            makenormale();
            mode_lecture = 3;
            NbNormales   = this->Objetlist[indiceObjet].Nb_normales;
            str.clear();
            for (i=0; i<NbNormales; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                   getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
//                make1normale();
                int indiceFacette = N_elements-1;
                this->Objetlist[indiceObjet].Facelist[indiceFacette].setNormale_b(xyz);
            }
            mode_lecture = -1;
        } else if((ligne.find("<ASPECT_FACE>") != notFound) || (ligne.find("<POLY_ATTR>") != notFound)) {
            makeaspect_face();
            mode_lecture = 4;
            NbAspectFaces = this->Objetlist[indiceObjet].Nb_aspects;
            for (i=0; i<NbAspectFaces;) { //; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back(); // Passer la(les) ligne(s) vide(s)
                }
                while (!ligne.empty()) {    // Lire et décoder jusqu'à ce qu'on trouve une ligne vide (ATTENTION s'il n'y en a pas !!!)
                    str.clear();
                    str=wxString::FromAscii(ligne.c_str());
                    make1aspect_face();     // A supprimer par la suite ??? Pour lire les lignes  <GROUPE> et <CODMATFACE> avec le même indice i
                                            // Mais laisser pour l'instant car décodage et rangement faits dans make1aspect_face à revoir
                                            /// C'est le SEUL endroit ou make1aspect_face est encore actif (mais utile si pas de ligne blanche intermédiaire !)
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                    if (New_aspect_face) i++;   // Incrémenter le compteur i seulement si 3 valeurs dans la ligne
                }
            }
            mode_lecture = -1;
        } else if((ligne.find("<VECTEUR>") != notFound) || (ligne.find("<VERT_NORMAL>") != notFound)) {
            makevecteur();
            mode_lecture = 5;
            NbVecteurs   = this->Objetlist[indiceObjet].Nb_vecteurs;
            str.clear();
            N_elements_prec = 0;
            for (i=0; i<NbVecteurs; i++) {
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                strcpy(c_ligne,ligne.c_str());
                sscanf(c_ligne,"%d%f%f%f",&N_elements,&xyz[0],&xyz[1],&xyz[2]);
                make1vecteur();
                if (N_elements - N_elements_prec != 1)
                    printf("==> Erreur possible :\n    Vecteur %d (indice %d, Objet d'indice %d) rupture de sequence\n",N_elements,i,indiceObjet);

                N_elements_prec = N_elements;
            }
            mode_lecture = -1;
        } else if(ligne.find("<LUMINANCE>") != notFound) {
            makeluminance();
            mode_lecture = 6;
            this->Objetlist[indiceObjet].flat = false;                  // L'objet peut être tracé en mode smooth car <LUMINANCE> est présent
            unsigned int NbLuminances_a_lire = this->Objetlist[indiceObjet].Nb_luminances;
            if (NbLuminances_a_lire < NbFacettes) {                    // C'est le cas de F16.bdd original
                printf("-> Nombre de luminances a lire : %d, Nouvelle dimension : %d\n",NbLuminances_a_lire, NbFacettes);
                this->Objetlist[indiceObjet].Nb_luminances = NbFacettes;
                for (unsigned int nfac=0 ; nfac < NbFacettes ; nfac++) this->Objetlist[indiceObjet].Facelist[nfac].flat = true; // Tout initialiser à plat
            }
            for (i=0; i<NbLuminances_a_lire; i++) {                     // Mais ne lire que les NbLuminances données
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();
                }
                str.clear();
                std::istringstream ss(ligne);
                ss >> N_elements;   // en fait ici, numéro de la facette
                ss >> Nb;           // ici, nombre de sommets de la facette
                NumerosSommets.resize(Nb);
                for (j=0; j<Nb ; j++) ss >> NumerosSommets[j];
                make1luminance();
                this->Objetlist[indiceObjet].Facelist[N_elements-1].flat = false; // Smooth possible uniquement pour les facettes avec Luminances données
                // Test de vérification de facette plane (tous numéros égaux)
                bool Facette_plane = true;
                int Num_0 = NumerosSommets[0];
                for (j=1; j<Nb ; j++) {
                    if (NumerosSommets[j] != Num_0) Facette_plane = false;
                }
                if (Facette_plane) this->Objetlist[indiceObjet].Facelist[N_elements-1].flat = true;
            }
            mode_lecture = -1;
        } else if((ligne.find("<POSITION>") != notFound) || (ligne.find("<PLACEMENT>") != notFound)) {
            makeposition(); // Ne sert pas à grand chose !
            mode_lecture = 7;
            NbMatrices = this->Objetlist[indiceObjet].Nb_matrices;
            printf("Nombre de matrices a lire : %d\n",NbMatrices);  // Il ne devrait y en avoir qu'une seule par objet ! ATTENTION ici seulement le type MAT_POSITION !
            for (i=0 ; i<NbMatrices ; i++) {                        // Toutefois, on va toutes les lire (au cas où ...) mais ne garder que la dernière
                getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();        // Lire 1 ligne mais supprimer tous les espaces de fin
                while (ligne.empty()) {
                    getline(fichierBdd,ligne); while (ligne.back() == ' ') ligne.pop_back();    // Passer la(les) ligne(s) vide(s)
                }
                Nb = 0;
//                while (!ligne.empty()) {    // Lire et décoder jusqu'à ce qu'on trouve une ligne vide (ATTENTION s'il n'y en a pas !!!)
                for (j = 0 ; j<4 ; j++) { // Lire et décoder 4 lignes.
                    str.clear();
                    str=wxString::FromAscii(ligne.c_str());
                    strcpy(c_ligne,ligne.c_str());
                    if (j == 0) {
                        sscanf(c_ligne,"%d%s%lf%lf%lf%lf",&k,&mot_cle[0],&matrice[0],&matrice[1],&matrice[2],&matrice[3]);  // La 1ère contient un n° de matrice, <MAT_POSITION> et 4 doubles
                        // En toute rigueur, il faudrait vérifier que le mot_cle est bien <MAT_POSITION> car d'autres transformations sont possibles : Translation, Rotation, Scale.
                        // et du coup adapter la lecture ... car elles n'ont pas les mêmes valeurs à lire !
//                        printf("%d %12.8f %12.8f %12.8f %12.8f\n",Nb,matrice[0],matrice[1],matrice[2],matrice[3]);
                    } else {
                        sscanf(c_ligne,"%lf%lf%lf%lf",&matrice[Nb],&matrice[Nb+1],&matrice[Nb+2],&matrice[Nb+3]);           // Les 3 suivantes contiennent 4 doubles
//                        printf("%d %12.8f %12.8f %12.8f %12.8f\n",Nb,matrice[Nb],matrice[Nb+1],matrice[Nb+2],matrice[Nb+3]);
                    }
                    Nb +=4;
                    getline(fichierBdd,ligne); // while (ligne.back() == ' ') ligne.pop_back();    // Pour supprimer les espaces de fin
                }
            }
            for (i=0 ; i<16 ; i++) this->Objetlist[indiceObjet].matrice[i] = matrice[i];        // Enregistrer la matrice dans l'objet
            // Vérification
//            for (i=0 ; i<16 ; i++) {
//                printf("%12.8f ",this->Objetlist[indiceObjet].matrice[i]);
//                if (i%4 == 3) printf("\n");
//            }
            mode_lecture = -1;
        } else {
                if (mode_lecture > 0) printf("mode_lecture = %d : POURQUOI passer par ici ????\n",mode_lecture);
        }
        x1++;
//        printf("ligne : %d\n",x1);
    }

    if (Objetlist.size() == 0) {
        printf("Le fichier ne comporte aucun objet !\n");
        m_gllist = 0;
        fichierBdd.close();
        wxString wxMessage = _T("Le fichier ne comporte aucun objet !");
        DisplayMessage(wxMessage,true);
        return;
    }
    if ((Objetlist.end()-1)->Nb_facettes == 0) {
        printf("L'objet %s est vide => non pris en compte\n",(Objetlist.end()-1)->GetName());
        Objetlist.erase(Objetlist.end()-1); // On supprime cet objet s'il est vide (car sans facettes)
        non_retenus++;
    }
    if (Objetlist.size() == 1)
        printf("\n%d objet",(int)Objetlist.size());
    else
        printf("\n%d objets\n",(int)Objetlist.size());
    if (non_retenus != 0)
        printf("%d objets(s) non pris en compte\n",non_retenus);

    printf("\nNb objets      %d\n",(int)this->Objetlist.size());

    for(i=0; i<this->Objetlist.size(); ++i) {
        printf("Objet %2d, numero %2d, Nb_facettes : %4d\n",i,this->Objetlist[i].GetValue(), this->Objetlist[i].Nb_facettes);

        if (this->Objetlist[i].mat_position) TraiterMatricePosition(i) ;
    }

//    if (this->Objetlist[indiceObjet].Vecteurlist.size()==0) {
//        printf("\nCalcul Vecteur !!!\n");
//        Calcul_All_Vecteurs();
//    }

    m_loaded = true;
    m_gllist = 0;
    fichierBdd.close();

    if(verbose) printf("Sortie de BddInter::LoadBdd\n");
}

void BddInter::TraiterMatricePosition(unsigned int i)
{
    // Il y a une matrice position pour cet objet => transformer d'abord les points puis les normales

    Vector3D V1, V2, New_Norm;  // Vecteurs 3D initialisés à 0.
    std::vector<float> Normale;
    std::vector<float> xyz_point;
    double matrice[16];
    unsigned int j, NbSommets, NbFacettes, NbVecteurs;

    for (j=0; j<16 ; j++) matrice[j] = this->Objetlist[i].matrice[j] ; // Copie locale de la matrice

    // Traitement des sommets/points par la matrice de position

    NbSommets = this->Objetlist[i].Nb_sommets;
    for (j=0 ; j<NbSommets ; j++) {
        xyz_point = this->Objetlist[i].Sommetlist[j].getPoint();
        Vector3D P1(xyz_point[0],xyz_point[1],xyz_point[2]);
        Vector3D P2 = P1.MultPoint4dMatrice4x4(matrice);
//        xyz_point[0] = P2.X ; xyz_point[1] = P2.Y ; xyz_point[2] = P2.Z ;
        xyz_point = {(float)P2.X, (float)P2.Y, (float)P2.Z} ;          // cast (float) sinon Warnings car P2 en double
        this->Objetlist[i].Sommetlist[j].setPoint(xyz_point);
    }

    // Modification de la matrice locale pour y supprimer les translations (sans objet sur des normales) ;
    matrice[12] = matrice[13] = matrice[14] = 0.0;

    // Traitement des normales au barycentres des facettes

/*          prendre deux vecteurs (non colinéaires) V1 et V2 passant par le sommet (ou barycentre) étudié (ramené en 0,0,0),
    les modifier par la matrice M puis faire le produit vectoriel des 2 vecteurs modifiés => Nouvelle normale
    équation générale ax + by + cz + d = 0, normale au plan : (a, b, c)
    passe par 0,0,0 => d=0
    cas c != 0 : on fixe x et y, on calcule z
            si x = 1, y = 0 => z = -a/c => V1 = (1, 0, -a/c)
            si x = 0, y = c => z = -b   => V2 = (0, c, -b)
            ainsi le produit vectoriel V1^V2 = (a, b, c) est bien égal à la normale
    cas c = 0 (=> division par 0 ou V1 et V2 ci-dessus vecteurs colinéaires)
    cas b != 0 : on fixe x et z, on calcule y
            si x = -b, z = 0 => y = a => V1 = (-b, a, 0)
            si x = 0,  z = 1 => y = 0 => V2 = ( 0, 0, 1)
            on obtient le produit vectoriel V1^V2 = (a, b, 0)
    cas b = 0
            C'est le plan x = 0 (a est forcément != 0 )
            V1 = (0,a,0)
            V2 = (0,0,1) en fait n'est pas différent du cas b != 0
*/

    NbFacettes = this->Objetlist[i].Nb_facettes;
    for (j=0 ; j<NbFacettes ; j++){
        Normale = this->Objetlist[i].Facelist[j].getNormale_b();
        if (Normale[2] != 0.0) {
            V1 = Vector3D(1.0,        0.0, -Normale[0]/Normale[2]);  // <=> V1.X = 1.0 ; V1.Y = 0.0 ; V1.Z = -Normale[0]/Normale[2];
            V2 = Vector3D(0.0, Normale[2], -Normale[1]);
        } else {
            V1 = Vector3D(-Normale[1], Normale[0], 0.0);
            V2 = Vector3D(        0.0,        0.0, 1.0);
        }
        V1 = V1.MultPoint4dMatrice4x4(matrice);     // Transformation de V1
        V2 = V2.MultPoint4dMatrice4x4(matrice);     // Transformation de V2
        New_Norm = V1.crossProduct(V2);             // Normale à V1 et V2 : V1^V2
        New_Norm.normalize();                       // Normalisation à 1.

        Normale = {(float)New_Norm.X, (float)New_Norm.Y, (float)New_Norm.Z} ;
        this->Objetlist[i].Facelist[j].setNormale_b(Normale);
    }

    // Traitement des normales aux sommets
    NbVecteurs = this->Objetlist[i].Nb_vecteurs;
    for (j=0 ; j<NbVecteurs ; j++){
        Normale = this->Objetlist[i].Vecteurlist[j].getPoint();
        if (Normale[2] != 0.0) {
            V1 = Vector3D(1.0,        0.0, -Normale[0]/Normale[2]);
            V2 = Vector3D(0.0, Normale[2], -Normale[1]);
        } else {
            V1 = Vector3D(-Normale[1], Normale[0], 0.0);
            V2 = Vector3D(        0.0,        0.0, 1.0);
        }
        V1 = V1.MultPoint4dMatrice4x4(matrice);     // Transformation de V1
        V2 = V2.MultPoint4dMatrice4x4(matrice);     // Transformation de V2
        New_Norm = V1.crossProduct(V2);             // Normale à V1 et V2 : V1^V2
        New_Norm.normalize();                       // Normalisation à 1.

        Normale = {(float)New_Norm.X, (float)New_Norm.Y, (float)New_Norm.Z} ;
        this->Objetlist[i].Vecteurlist[j].setPoint(Normale);
    }
}

void BddInter::makeobjet() {
    wxStringTokenizer tokenizer(str);
    wxStringlist.clear();
    while ( tokenizer.HasMoreTokens() ) {
        token = tokenizer.GetNextToken();
        this->wxStringlist.push_back(this->token);      // Pb : si le nom comporte des espaces, c'est plusieurs token
    }
    std::cout << '\n' << "Objet ";
    wxString Nom_Objet;
    unsigned int last = this->wxStringlist.size()-1;
    for(unsigned int i=1; i<=last; i++) {
        wxString *mystring= new wxString(wxStringlist[i]);
        std::string stl_string = std::string(mystring->mb_str());
        std::cout <<" "+stl_string;
        if (i > 1) {
            Nom_Objet += wxStringlist[i];               // Concaténer les wxStringList à partir de [2]
            if (i < last) Nom_Objet += _T(" ");         // Séparer par des espaces sauf si c'est le dernier
        }
    }
    std::cout << '\n' ;
//    this->buffer = Nom_Objet.mb_str();
//    printf("Nom complet : %s\n",buffer.data());
    Object NewObject(Nom_Objet,wxStringlist[1]);
    NewObject.afficher = true;                          // Par défaut, on affiche l'objet
    NewObject.deleted  = false;                         // Et on le marque comme "non supprimé" (pas tout de suite !)
    if (Numero_base != 0) NewObject.SetValue(NewObject.GetValue() + Numero_base) ; // Changer le numéro d'objet (value) en y ajoutant un offset (Numero_base) !
    wxStringlist.clear();
    this->Objetlist.push_back(NewObject);
    indiceObjet_courant = this->Objetlist.size() -1;    // plus judicieux à faire ici plutôt qu'apès des makeobjet ? A vérifier
}

void BddInter::makeface() {
    int Nb_facettes;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Nb_facettes = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_facettes = N_elements;
    }
    printf("%6d facettes\t\t\t(<FACE>/<POLYGON>)\n",Nb_facettes);
    this->Objetlist[indiceObjet_courant].Nb_facettes = Nb_facettes;  // Stocker le nombre de facettes dans l'objet
    this->Objetlist[indiceObjet_courant].Facelist.resize(Nb_facettes);
}

void BddInter::make1face() {
    int Numero;
    Face New1Face;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Numero   = wxAtoi(wxStringlist[0]);
        New1Face = Face(wxStringlist); // Ici on passe un numéro de facette, un nombre de point n, puis n valeurs
        wxStringlist.clear();
    } else {
        Numero   = N_elements;
        New1Face = Face(Numero, NumerosSommets);
    }

    if (Numero > (int)this->Objetlist[indiceObjet_courant].Facelist.size()) {
        this->Objetlist[indiceObjet_courant].Facelist.push_back(New1Face) ; // Taille initiale trop petite (on est en création de facettes) => procéder par push_back !
        this->Objetlist[indiceObjet_courant].Nb_facettes++;
    }
    else
        this->Objetlist[indiceObjet_courant].Facelist[Numero-1] = New1Face;
}

void BddInter::makesommet() {
// Dans cette version, on dimensionne la liste Sommetlist et on initialise Nb_sommets dans Objetlist[indiceObjet_courant].
    int Nb_sommets;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Nb_sommets = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_sommets = N_elements;
    }
    printf("%6d sommets\t\t\t(<SOMMET>/<VERTEX>)\n",Nb_sommets);
    this->Objetlist[indiceObjet_courant].Nb_sommets = Nb_sommets;  // Stocker le nombre de sommets dans l'objet
    this->Objetlist[indiceObjet_courant].Sommetlist.resize(Nb_sommets);
}

void BddInter::make1sommet() {
// <=> make1vecteur
// Si la liste est de taille suffisante, on utilise directement le Numero via les token ou N_Element
// sinon, on incrémente la taille de Sommetlist
    int Numero;
    Sommet New1Sommet;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Numero = wxAtoi(wxStringlist[0]);
        New1Sommet = Sommet(wxStringlist);      // Ici, on passe 4 valeurs : un numéro et 3 valeurs en x, y et z
        wxStringlist.clear();
    } else {
        Numero = N_elements;
        New1Sommet = Sommet(Numero,xyz);
    }
    if (Numero > (int)this->Objetlist[indiceObjet_courant].Sommetlist.size()) {
        this->Objetlist[indiceObjet_courant].Sommetlist.push_back(New1Sommet) ; // Taille initiale trop petite (on ajoute des points) => procéder par push_back !
        this->Objetlist[indiceObjet_courant].Nb_sommets++;
    }
    else
        this->Objetlist[indiceObjet_courant].Sommetlist[Numero-1] = New1Sommet;
}

void BddInter::makenormale() {
    // Ne sert qu'à initialiser Nb_normales dans Objetlist[indiceObjet_courant]
    int Nb_normales;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Nb_normales = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_normales = N_elements;
    }
    printf("%6d normales aux facettes\t(<NORMALE>/<POLY_NORMAL>)\n",Nb_normales);

    this->Objetlist[indiceObjet_courant].Nb_normales = Nb_normales;
}

void BddInter::make1normale() {
// Enregistre la normale au barycentre dans la Facelist courante
    int Numero;
    Normale New1Normale;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Numero      = wxAtoi(wxStringlist[0]);
        New1Normale = Normale(wxStringlist);
        wxStringlist.clear();
    } else {
        Numero      = N_elements;
        New1Normale = Normale(Numero,xyz);
    }
    this->Objetlist[indiceObjet_courant].Facelist[Numero-1].normale_b = New1Normale.getPoint();
//    printf("%d ",Numero-1);
//    for (int i=0;i<3;i++) printf("%f ",this->Objetlist[indiceObjet_courant].Facelist[Numero-1].normale_b[i]);
//    printf("\n");
}

void BddInter::makeaspect_face() {
    // Ne sert qu'à initialiser Nb_aspects dans Objetlist[indiceObjet_courant]
    int Nb_aspect;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Nb_aspect = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_aspect = N_elements;
    }
    printf("%6d attributs aux facettes\t(<ASPECT_FACE>/<POLY_ATTR>)\n",Nb_aspect);
    this->Objetlist[indiceObjet_courant].Nb_aspects = Nb_aspect;
}

void BddInter::make1aspect_face() {
    int indice;
    int val;
    int grp_mat;

// Pour l'instant ne gère pas str.IsEmpty() comme les autres fonctions make1* !
// car Aspect_face ne gère que wxStringlist actuellement.
// Possibilités à voir : Aspect_face(int& numero, int& grp_mat, int& valeur) grp_mat valant 1 si valeur est un groupe, 2 si valeur est un matériau
//                    ou Aspect_face(int& numero, int& numeroGroupe, int& numeroMateriau), mais la faudrait donner les 2 à chaque fois
//                    ou Aspect_face(int& numero, std::vector<int> Numeros) Numeros[0] serait le groupe et [1] le matériau

    grp_mat = 0;
    if((str.Contains(_T("<GROUPE>")))     || (str.Contains(_T("<GROUP>")) )) {
        grp_mat = 1;
    }
    if((str.Contains(_T("<CODMATFACE>"))) || (str.Contains(_T("<POLY_REF_ATTR>")) )) {
        grp_mat = 2;
    }
    if((str.Contains(_T("<TEXTURE>"))) || (str.Contains(_T("<OMBRAGE>"))) || (str.Contains(_T("<SHADING>"))) ) {
        grp_mat = 3;
    }
//    if (grp_mat != 0) {
//        str.Replace (_T("<GROUP>"),_T("<GROUPE>"));
//        str.Replace (_T("<POLY_REF_ATTR>"), _T("<CODMATFACE>"));
//        buffer=str.mb_str();
//        printf("\n1N     %s\n",buffer.data());
        wxStringTokenizer tokenizer(str);
        int n_tokens = tokenizer.CountTokens();
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        if (n_tokens == 3) {                                                    // Une ligne peut contenir à ce niveau 2 ou 3 champs (ou tokens)
            Aspect_face New1aspect_face(wxStringlist);                          // S'il y en a 3, il faut créer un nouveau aspect_face1
            indice = this->indiceAspect = wxAtoi(wxStringlist[0]) -1;           // Sauvegarder indice dans indiceAspect pour usage ultérieur
            this->Objetlist[indiceObjet_courant].Facelist[indice].groupe = 0;
            this->Objetlist[indiceObjet_courant].Facelist[indice].codmatface = codmatface_def;
            New_aspect_face = true;
        } else {
            indice = this->indiceAspect;
            New_aspect_face = false;
        }
        val = wxAtoi(token);                                                    // Récupère le numéro de groupe ou de matériau sous forme d'entier
        if (grp_mat == 1) {
            this->Objetlist[indiceObjet_courant].Facelist[indice].groupe = val;
//            auto it = std::find(listeGroupes.begin(),listeGroupes.end(),val);   // Est-il déjà dans la liste ?
//            if (it == listeGroupes.end() || listeGroupes.empty()) {                                         // Non
//                listeGroupes.push_front(val);                                   // L'ajouter à la liste des groupes
//                listeGroupes.sort();                                            // Trier la liste (on pourrait le faire plus tard !)
//            }
        } else if (grp_mat == 2) {
            this->Objetlist[indiceObjet_courant].Facelist[indice].codmatface = val;
//            auto it = std::find(listeMateriaux.begin(),listeMateriaux.end(),val);   // Est-il déjà dans la liste ?
//            if (it == listeMateriaux.end() || listeMateriaux.empty()) {                                       // Non
//                listeMateriaux.push_front(val);                                     // L'ajouter à la liste des matériaux
//                listeMateriaux.sort();
//            }
        }
//    }

    wxStringlist.clear();
}

void BddInter::makeluminance() {

// Ne sert plus maintenant qu'à initialiser Nb_luminances dans un objet. => On pourrait s'en passer (à condition d'initialiser ailleurs et/ou autrement !)

    int Nb_luminances;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
//              this->wxStringlist.push_back(this->token);
            wxStringlist.push_back(token);
        }
        Nb_luminances = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_luminances = N_elements;
    }
    printf("%6d valeurs de Luminances\t(<LUMINANCE>)\n",Nb_luminances);

    this->Objetlist[indiceObjet_courant].Nb_luminances = Nb_luminances;
}

void BddInter::make1luminance() {
// ici reste avec des push_back, pas comme dans make1face ... peut-être à revoir !
    int Numero;
    Luminance New1luminance;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Numero = wxAtoi(wxStringlist[0]);
        New1luminance = Luminance(wxStringlist);
        wxStringlist.clear();
    } else {
        Numero = N_elements;
        New1luminance = Luminance(Numero, NumerosSommets) ;
    }

    this->Objetlist[indiceObjet_courant].Facelist[Numero-1].Nb_Sommets_L = New1luminance.Nb_Sommets_L;
    this->Objetlist[indiceObjet_courant].Facelist[Numero-1].L_sommets.clear();  // Forcer un clear ici car peut avoir été déjà initialisé ailleurs => push_back ne ferait pas ce qu'il faut !
    for(int i=0; i<New1luminance.Nb_Sommets_L; i++) {
        this->Objetlist[indiceObjet_courant].Facelist[Numero-1].L_sommets.push_back(New1luminance.L_sommets[i]);
    }
}

void BddInter::makevecteur() {
// Dans cette version, on dimensionne la liste Vecteurlist et on initialise Nb_vecteurs dans Objetlist[indiceObjet_courant].
    int Nb_vecteurs;
//    Vecteur Newvecteur;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Nb_vecteurs = wxAtoi(token);
        wxStringlist.clear();
    } else {
        Nb_vecteurs = N_elements;
    }
//    this->Vecteurlist.push_back(Newvecteur);
    printf("%6d normales aux sommets\t(<VECTEUR>/<VERT_NORMAL>)\n",Nb_vecteurs);
    wxStringlist.clear();

    this->Objetlist[indiceObjet_courant].Nb_vecteurs = Nb_vecteurs;
    this->Objetlist[indiceObjet_courant].Vecteurlist.resize(Nb_vecteurs);
}

void BddInter::make1vecteur() {
// <=> make1sommet
// Si la liste est de taille suffisante, on utilise directement le Numero via les token ou N_Element
// sinon, on incrémente la taille de Vecteurlist
    int Numero;
    Vecteur New1Vecteur;

    if (!str.IsEmpty()) {
        wxStringTokenizer tokenizer(str);
        while ( tokenizer.HasMoreTokens() ) {
            token = tokenizer.GetNextToken();
            wxStringlist.push_back(token);
        }
        Numero = wxAtoi(wxStringlist[0]);
        New1Vecteur = Vecteur(wxStringlist);    // Ici, on passe 4 valeurs : un numéro et 3 valeurs en x, y et z
        wxStringlist.clear();
    } else {
        Numero = N_elements;
        New1Vecteur = Vecteur(Numero,xyz);
    }
    if (Numero > (int)this->Objetlist[indiceObjet_courant].Vecteurlist.size()) {
        this->Objetlist[indiceObjet_courant].Vecteurlist.push_back(New1Vecteur) ; // Taille initiale trop petite (on ajoute des points) => procéder par push_back !
        this->Objetlist[indiceObjet_courant].Nb_vecteurs++;
    }
    else
        this->Objetlist[indiceObjet_courant].Vecteurlist[Numero-1] = New1Vecteur;
}

void BddInter::makeposition() {
    wxStringTokenizer tokenizer(str);
    while ( tokenizer.HasMoreTokens() ) {
        token = tokenizer.GetNextToken();
        wxStringlist.push_back(token);
    }
//    Position Newposition(token);
    printf("%6d matrice Position\t\t(<POSITION>/<PLACEMENT>)\n",wxAtoi(token));
    wxStringlist.clear();
//    this->positionlist.push_back(Newposition);
    this->Objetlist[indiceObjet_courant].mat_position= true;            // il y a une matrice POSITION dans la BDD pour l'objet courant
    this->Objetlist[indiceObjet_courant].Nb_matrices = wxAtoi(token);   // Nombre de transformations (normalement 1 seule si <MAT_POSITION>
}

void BddInter::GenereTableauPointsFacettes(Object * objet)
{
    unsigned int nb_p, nb_fac;
    unsigned int i, j;
    unsigned int indice_sommet;
    Face    *Face_i;

    if (verbose) printf("Entree BddInter::GenereTableauPointsFacettes\n");
    nb_p   = objet->Sommetlist.size();  //Nb_sommets;
    nb_fac = objet->Facelist.size();    //Nb_facettes;
    objet->Pointslist.clear();
    objet->Pointslist.resize(nb_p);
    for (i=0; i<nb_fac; i++) {
        Face_i = &(objet->Facelist[i]);
        if (Face_i->deleted) continue;
        for (j=0; j<Face_i->F_sommets.size();j++) {
            indice_sommet = Face_i->F_sommets[j] -1;                            // <=> numero_sommet -1
            objet->Pointslist[indice_sommet].IndicesFacettes.push_back(i);      // numero_sommet-1 = indice du sommet, i=indice / i+1 = numéro de la facette
        }
    }
    if (verbose) printf("Sortie BddInter::GenereTableauPointsFacettes\n");
}

void BddInter::GenereTableauAretes(Object * objet)
{
    // NOTE : On pourrait chronométrer la partie test des doublons d'arêtes puis, si c'est suffisament court, activer GenereTableauAretes systématiquement quand c'est utile
    //        plutôt que de laisser ce soin à faire en manuel par l'utilisateur via le bouton "Recalculer les arêtes"

    unsigned int i,nb_fac,j,j_b,nb_p;
    unsigned int indice_sommet;
    unsigned int Numero_Objet;
    unsigned int Nb_avant, Nb_apres, Nb_deleted;
    unsigned int indice_a, indice_b;
    std::vector<int>  Numeros_Sommets;
    Aretes   Arete, Arete_test, *p_Arete;
    bool verbose_local = true;      // Local ici
    static bool traiter_ici = true; // Si true, traite les doublons d'arêtes à la génération. Mais ça peut être long si ce n'est pas nécessaire !
                                    // mis en static pour conserver la valeur si elle a été modifiée (dès qu'un objet dépasse un nombre d'arêtes spécifié)
    bool en_cours;
    unsigned int Arete_size_test=200000; // 1000000 Si le nombre d'arêtes à traiter dépasse cette valeur, on court-circuite le traitement des doublons car trop long... faute de mieux !

    clock_t time_0, delta_time, time_c;
    #define NB_DELTA_TICKS 2*CLOCKS_PER_SEC         // Pour obtenir un affichage de points de progression toutes les 2 secondes

// Cette façon de faire (1ère partie) est simple, mais les arêtes communes à 2 facettes sont comptées 2 fois !
// La seconde partie permet de décoder les doublons et de les éliminer

    if (verbose) printf("Entree BddInter::GenereTableauAretes\n");
    en_cours     = false;
//    indice_point = 0;
    nb_fac = objet->Nb_facettes;
    objet->Areteslist.clear();
    for (i=0; i< nb_fac ; i++) {
        if (objet->Facelist[i].deleted) continue;   // Facette supprimée => Passer à la suivante
        Numeros_Sommets = objet->Facelist[i].getF_sommets();
        nb_p = Numeros_Sommets.size();
        for (j=0; j<nb_p; j++) {
            Arete.afficher = true;
            Arete.deleted  = false;
            indice_a       = Numeros_Sommets[j]   -1;
            j_b = j+1; if (j_b >= nb_p) j_b = 0;                        // Pour reboucler sur le premier point de la facette
            indice_b       = Numeros_Sommets[j_b] -1;
            // tester pour que ind_a < ind_b
            if (indice_a < indice_b) {
               Arete.ind_a = indice_a;
               Arete.ind_b = indice_b;
            } else {
               Arete.ind_a = indice_b;
               Arete.ind_b = indice_a;
            }
//            Arete.point_a  = &(objet->Sommetlist[Arete.ind_a].point);   // A supprimer si non utilisé ??? point_a est un pointeur (enfin c'est ce qu'on voudrait !)
//            Arete.point_b  = &(objet->Sommetlist[Arete.ind_b].point);   // sur les coordonnées des sommets d'indices ind_a et ind_b, extrémités de l'arête.
            objet->Areteslist.push_back(Arete);
        }
    }
    Nb_avant = objet->Nb_aretes = objet->Areteslist.size();
    if (Nb_avant >= Arete_size_test) traiter_ici = false;
    if (!traiter_ici) {
        if (verbose_local) {
            printf("Objet : %s\nNb_aretes : %d\n",objet->GetName(),Nb_avant);
            if (verbose) printf("Sortie BddInter::GenereTableauAretes\n");
        }
        return;
    }
    if (verbose_local) sprintf(Message,"Objet : %s\nNb_aretes avant : %d\n",objet->GetName(),Nb_avant);
    Nb_deleted = 0;


// Boucles imbriquées qui peuvent être longues à l'exécution sur de grosses bdd ( >10' sur ellipsoide_260000.bdd ou Millenium_Falcon !).

// Notes : on pourrait stocker avec ind_a < ind_b (car l'ordre n'a pas d'importance) et du coup le test ci-dessous serait + simple. On pourrait même tester
//          d'abord l'égalité sur le 1er indice, puis celui sur le second => moins de test => plus rapide !
//         On pourrait aussi arrêter de tester les indices et sortir de la boucle en j, dès que 2 correspondances ont été trouvées car dans la majorité des
//         cas une arête n'est utilisée que 2 fois au plus car commune à 2 facettes.

    time_0 = time_c = clock();
    for (i=0; i<objet->Areteslist.size(); i++) {
        Arete_test = objet->Areteslist[i];
        if (Arete_test.deleted) continue;   // Arête déjà supprimée. Passer à la suivante ... test sans doute inutile car ça n'arrive jamais tel que c'est construit !
        indice_a = Arete_test.ind_a;
        indice_b = Arete_test.ind_b;
        for (j=i+1; j<objet->Areteslist.size(); j++) {
            p_Arete = &(objet->Areteslist[j]);  // Ici, il vaut mieux travailler sur un pointeur, surtout avec la méthode 2 ci-dessous, sinon on ne modifie qu'une copie !
            if (Arete.deleted) continue ;       // Arête déjà supprimée. Passer à la suivante
            // Doublon si les 2 indices a et b sont identiques (ou inversés)
//            if (((p_Arete->ind_a == Arete_test.ind_a) && (p_Arete->ind_b == Arete_test.ind_b)) ||
//                ((p_Arete->ind_a == Arete_test.ind_b) && (p_Arete->ind_b == Arete_test.ind_a))) { // Test ne fonctionne pas ?
            if (p_Arete->ind_a == indice_a) {
                if (p_Arete->ind_b == indice_b) {
        // 2 méthodes testées : la 1 est à peine plus rapide car on garde toute la liste tout le temps. La seconde réajuste les tailles
        // au fur et à mesure mais + de traitements à chaque étape...
        // Méthode 1 : on marque l'arête comme deleted car déjà tracée
//                p_Arete->deleted = true;
//                Nb_deleted++;
//                printf("aretes %d et %d en doublons\n",i,j);
//        // Méthode 2 : on supprime physiquement l'arête de la liste et on réajuste la taille
                    objet->Areteslist.erase(objet->Areteslist.begin()+j);   // Supprimer l'arête en doublon
//                    j-- ; // poursuivre la boucle for avec le même indice j car ils ont été décalés d'une unité par le erase de la ligne ci-dessus
                    break; // doublon trouvé => abandonner le for j et passer au i suivant ! => ignorer des arêtes en triple !
                }
            }
//            indice_point++;   // Pour ajouter éventuellement un indicateur de progression (un . affiché toutes les indice_test valeurs testées)
                                // L'affichage dépend du temps d'exécution => plutôt utiliser un timer et donc n'afficher un '.' que toutes les x secondes
            delta_time = clock() - time_c;
            if (delta_time >= NB_DELTA_TICKS && verbose_local) {
                if (!en_cours) {                                    // Seulement la 1ère fois
                    en_cours = true;
//                    printf("%s",Message);
//                    printf("Detection d'aretes en doublon ");
                    printf(utf8_To_ibm(Message));                       // Afficher d'office Nb_avant (quelle que soit la future valeur de Nb_apres)
                    sprintf(Message,"Détection d'arêtes en doublon ");  // et le début de l'indicateur de progression
                    printf(utf8_To_ibm(Message));
                }
                printf(".");            // On pourrait aussi afficher successivement | / - \ avec des backspaces.
                time_c = clock();
            }
        }
    }
    Nb_apres = objet->Nb_aretes = objet->Areteslist.size();
    if (Nb_deleted != 0) {
        Nb_apres -= Nb_deleted; // Ne sert qu'en méthode 1 ! Mais la taille de la liste d'arêtes ne change pas car les arêtes à supprimer sont marquées deleted = true !
    }
    // On n'affiche que si verbose est true et Nb_avant != Nb_après, sauf si "en_cours" est true car Nb_avant est déjà à l'écran avec l'indication de progression.
    if (verbose_local) {
//        if (!en_cours && (Nb_avant != Nb_apres)) printf("%s",Message);
        if (!en_cours && (Nb_avant != Nb_apres)) printf(utf8_To_ibm(Message));
        if (en_cours) printf("\n");
        if (en_cours  || (Nb_avant != Nb_apres)) {
//            printf("Nb_aretes apres : %d\n",Nb_apres);
            sprintf(Message,"Nb_aretes après : %d\n",Nb_apres);
            printf(utf8_To_ibm(Message));
        }
    }

    delta_time = clock() - time_0;
    objet->Temps_Calcul_Aretes = delta_time;
    if (delta_time >= CLOCKS_PER_SEC) { // N'afficher ce temps que si >= 1 seconde
        printf("Temps de filtrage des doublons : %ld ticks (soit %3.1f secondes)\n\n", delta_time, float(delta_time)/CLOCKS_PER_SEC);
    }

    if (verbose) printf("Sortie BddInter::GenereTableauAretes\n");
}

void BddInter::InitGL() {
    {       // sert à quoi ?
        if (verbose) printf("Entree BddInter::InitGL\n");
        glLoadIdentity();

        /* speedups */
//        glEnable(GL_DITHER);
        if(smooth) {
            glShadeModel(GL_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
        } else {
            glShadeModel(GL_FLAT);
            glDisable(GL_LINE_SMOOTH);
        }

        if (antialiasing_soft)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);

        glEnable(GL_NORMALIZE);

//Peut être utile, car l'affichage est plus rapide lorsque cette option est activée.
//Cependant des trous sont visibles, à la place des faces arrières, pour les BDD erronnées.
//      glCullFace(GL_FRONT_AND_BACK);
//      glEnable(GL_CULL_FACE);				                /*! Ne traite pas les faces cachées*/

        glEnable(GL_DEPTH_TEST);							/*! Active le Z-Buffer*/
        glDepthFunc(GL_LEQUAL);								/*! Mode de fonctionnement du Z-Buffer*/
// L'option par défaut est GL_LESS mais ne semble pas changer grand chose
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	/*! Active la correction de perspective (pour ombrage, texture, ...)*/

        /* light */
        glLightfv(GL_LIGHT0, GL_AMBIENT, Light0Ambient);	/*! Couleur ambiante de la source de lumière 0 (directionnelle)*/
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Light0Diffuse);	/*! Couleur diffuse de la source de lumière 0*/
        glLightfv(GL_LIGHT0, GL_SPECULAR,Light0Specular);	/*! Couleur spéculaire de la source de lumière 0*/
        glLightfv(GL_LIGHT0, GL_POSITION,Light0Position);   /*! Direction de la source de lumière 0*/
//        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,light0_dir);
//        glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, &angle);

        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);

//        glColorMaterial(GL_FRONT, GL_AMBIENT);
//        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);     /*! nécessaire pour la colorisation des sens des normales qui ne font que du glColor3fv ! */
        glEnable(GL_COLOR_MATERIAL);                        /*! active la colorisation des matériaux et groupes ??? MAIS non différentiée pour l'instant ? */

//        printf("Sortie BddInter::InitGL\n");
    }
        if (verbose) printf("Sortie BddInter::InitGL\n");
}

void BddInter::searchMin_Max() {
    std::vector<float>Point_XYZ;
    std::vector<int>numSommets;
    unsigned int i=0,j=0,k=0;

    if (verbose) printf("Entree BddInter::searchMin_Max\n");

    resetMin_Max();
    nb_objets = this->Objetlist.size();
    nb_objets_reels = nb_objets;
    nb_3points=nb_4points=nb_aretes=nb_facettes=nb_sommets=nb_sommets_max=numero_facette_max=numero_objet_max=0 ;
    for(i=0; i<nb_objets; i++) {
        if (this->Objetlist[i].deleted) {
            nb_objets_reels--;
        } else {
            nb_facettes += this->Objetlist[i].Facelist.size();      // On pourrait utiliser this->Objetlist[i].Nb_facettes
            nb_sommets  += this->Objetlist[i].Sommetlist.size();    // idem Nb_sommets
            nb_aretes   += this->Objetlist[i].Areteslist.size();    // idem Nb_aretes
            for(j=0; j<this->Objetlist[i].Facelist.size(); j++) {
                if (this->Objetlist[i].Facelist[j].deleted) nb_facettes-- ; // Décompter les facettes supprimées
                if (this->Objetlist[i].Facelist[j].afficher && !this->Objetlist[i].Facelist[j].deleted) {
                    numSommets.clear();
                    numSommets=this->Objetlist[i].Facelist[j].getF_sommets();
                    unsigned int nombreSommets = numSommets.size();
//                    nb_sommets += nombreSommets ;   // Non car chaque point est compté plusieurs fois (1 par facette)
                    if (nombreSommets == 3) nb_3points += 1 ;
                    else                    nb_4points += 1 ;

                    if (nombreSommets > nb_sommets_max) {
                        nb_sommets_max     = nombreSommets ;
                        numero_objet_max   = i ;
                        numero_facette_max = j ;
                    }
                    for(k=0; k<nombreSommets; k++) {
                        Point_XYZ = this->Objetlist[i].Sommetlist[numSommets[k]-1].getPoint();
                        setMin_Max(Point_XYZ[0],Point_XYZ[1],Point_XYZ[2]);
                    }
                }
            }
        }
    }
    if (x_min == +FLT_MAX) {
        // min et max non initialisés correctement (valeurs de resetMin_Max() non retouchées !) : on ne teste que x_max, ça devrait être suffisant.
        x_min = x_max = y_min = y_max = z_min = z_max = 0.0;
    }
    diagonale_save = Norme3(x_max-x_min, y_max-y_min, z_max-z_min) ;
    centre_auto[0] = (x_max+x_min)*0.5 ;    // Centre de la boite englobante
    centre_auto[1] = (y_max+y_min)*0.5 ;
    centre_auto[2] = (z_max+z_min)*0.5 ;
    if (centrageRotAuto) centreRot = centre_auto ;

    if (verbose) printf("Sortie BddInter::searchMin_Max\n");
}

float BddInter::produit_scalaire(const std::vector<float> &v1, const std::vector<float> &v2){
    float cross = v1[0]*v2[0] +v1[1]*v2[1] +v1[2]*v2[2];
//    cross = cross/(v1[0]*v1[0] +v1[1]*v1[1] +v1[2]*v1[2])/(v2[0]*v2[0] +v2[1]*v2[1] +v2[2]*v2[2]); // Serait à faire si v1 et/ou v2 non normalisés à 1
    return cross;
}

void BddInter::Tracer_normale(const std::vector <float> &o, const std::vector <float> &n, int typeCouleur)
{
// Tracé d'un vecteur n (une normale unitaire) au point o (origine)
// typeCouleur détermine la couleur tracée et vaut 1 ou autre chose :
//     En pratique 0 pour une normale à la facette et 1 pour une normale au sommet

    std::vector <float> v;   // extrémité du vecteur tracé

    v = o;
    v[0] += longueur_normales*n[0];
    v[1] += longueur_normales*n[1];
    v[2] += longueur_normales*n[2];

	glPushMatrix();
	glLineWidth(3);
    glPointSize(3);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    if (typeCouleur == 1) {
        glColor3f(0.0,0.8,0.8) ; // Cyan foncé = Vert + Bleu
    } else {
        glColor3f(0.0,0.8,0.0) ; // Vert foncé
    }

	glBegin(GL_LINES);
		glVertex3d(o[0], o[1], o[2]);
		glVertex3d(v[0], v[1], v[2]);
	glEnd();

    if (typeCouleur == 1) {
        glColor3fv(cyan) ; // Cyan = Vert + Bleu
    } else {
        glColor3fv(vert) ; // Vert
    }

	glBegin(GL_POINTS);
		glVertex3d(v[0], v[1], v[2]);
	glEnd();

    glPointSize(1);
	glLineWidth(1);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);

	glPopMatrix();
}

void BddInter::coloriserFacette(unsigned int indiceObjet, unsigned int indiceFacette, bool tracerFacette, GLfloat couleur[3])
{
// colorise les facettes sélectionnées ou la facette surlignée et y ajoute éventuellement la/les normale(s)

    std::vector <int>   NumerosSommets;
    std::vector <float> xyz_sommet;
    std::vector <float> barycentre;
    std::vector <float> NormaleFacette;
    std::vector <float> NormaleSommet;
    unsigned int k=0,j=0;
    GLfloat couleurs[4];    // R, G, B, alpha
    Object *Objet_courant;
    Face   *Facette_courante;

    if (this->Objetlist[indiceObjet].deleted) return;   // Ne rien faire sur un objet supprimé

    Objet_courant    = &(this->Objetlist[indiceObjet]);
    Facette_courante = &(this->Objetlist[indiceObjet].Facelist[indiceFacette]);
    glGetFloatv(GL_CURRENT_COLOR, couleurs);            // Sauvegarde de la couleur actuelle (R G B alpha)
    glDisable(GL_LIGHTING);
    if (tracerFacette) {
        glColor3fv(couleur);  // Les facettes surlignées sont en jaune (mais est sans effet dans le mode sélection de facettes)
//      glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionJ)  ;
//      glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionJ)  ;
//      glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionJ) ;
//      glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionJ);
        glBegin(GL_POLYGON);
    }
    // Pas de traitement de smooth ici ni des normales aux facettes ! Colorisation directe.
    NumerosSommets  = Facette_courante->getF_sommets();
//    selectMode(mode);
    unsigned int ns = NumerosSommets.size();
    for(k=0; k<ns; k++) {
        xyz_sommet = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
        if (tracerFacette) glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
        if (k == 0)
            barycentre = xyz_sommet;
        else {
            for (j=0; j<3; j++) barycentre[j] += xyz_sommet[j];
        }
    }
    if (tracerFacette) glEnd();

    if (!Facette_courante->deleted) {   // Ne pas afficher les normales et points des facettes supprimées
        if (AfficherNormaleFacette) {
            for (j=0; j<3; j++) barycentre[j] /= ns;
            NormaleFacette = Facette_courante->getNormale_b();
            Tracer_normale(barycentre,NormaleFacette,0);
        }
        if (AfficherNormalesSommets) {
            NormaleFacette = Facette_courante->getNormale_b();
            if (Facette_courante->flat || Objet_courant->flat) {
            // La facette est plane => tracer la normale à la facette sur les ns sommets
                for(k=0; k<ns; k++) {
                    xyz_sommet = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
                    Tracer_normale(xyz_sommet,NormaleFacette,1);
                }
            } else {
            // La facette n'est pas plane ...
                for(k=0; k<ns; k++) {
                    xyz_sommet    = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
                    NormaleSommet = Objet_courant->Vecteurlist[Facette_courante->L_sommets[k]-1].point;
                    Calcul_Normale_Seuillee(indiceObjet,indiceFacette,k,NormaleFacette,NormaleSommet) ;
                    Tracer_normale(xyz_sommet,NormaleSommet,1);
                }
            }
        }
        if (Sommets_Surlignes && tracerFacette) {
            glColor3fv(vert);
            glPointSize(8);
            glBegin(GL_POINTS);
            for(k=0; k<ns; k++) {
                xyz_sommet = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
                glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                if (NumerosSommets[k] == Numero_Sommet_a_Surligner) point_star = xyz_sommet;
            }
            glEnd();
            glPointSize(1);
            GenereEtoile();
        }
    }
    glEnable(GL_LIGHTING);
    glColor4fv(couleurs); // Restaurer la couleur actuelle (R, G, B et alpha sauvegardés en début)
}

void BddInter::drawOpenGL() {
    char test;
    std::vector<int>   NumerosSommets;
    std::vector<float> xyz_sommet;
//    Vector3D      np;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;
    Face    *Face_ij;
    Object  *Objet_i;
    unsigned int i=0,j=0,k=0;
    bool test2 = false; // Utilisé pour faire un double passage en mode visualisation du "Sens des normales", le premier en GL_CCW et le second en GL_CW
    bool test_np, lissage_Gouraud;

    if (verbose)
        printf("Entrée BddInter::drawOpenGL\n");

    glPointSize(5.0);
    longueur_normales = diagonale_save*len_normales*m_gldata.zoom/FoV_auto; // Attention : diagonale_save peut être modifié dans searchMin_Max => effet indésirable potentiel ?

    if(show_CW_CCW == true) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
    }

//    printf("smooth : %d\n",smooth);

    if( m_gllist == 0 ) {

        if (verbose) printf("Reconstruction des listes\n");

        searchMin_Max();    // Pour mettre à jour les différents compteurs d'objets, facettes, points ... Met aussi à jour diagonale_save et centrage_auto
        int nb_normales_seuillees = 0;      // A déclarer plutôt ailleurs, mais initialiser ici
        glDeleteLists(glliste_objets,1);    // Supprime la liste des objets
        m_gllist = glliste_objets;
        glNewList( m_gllist, GL_COMPILE_AND_EXECUTE );  /// Si seulement GL_COMPILE, la sélection de facettes via clic milieu souris est en bleu au lieu de vert ! Pourquoi ?
        if (MPanel->activer_transparence) {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        }
        glInitNames();

// C'est ici qu'on revient pour le second passage en mode de visualisation du "Sens des normales"
Boucle:
        for(i=0; i<this->Objetlist.size(); i++) {

            // Objet courant
            Objet_i = &this->Objetlist[i];

            if (Objet_i->afficher && !Objet_i->deleted) {
                glPushName(i);

                // Note : on pourrait peut-être fusionner les 2 if (Rotation_Objets) et if (Changer_Echelle) : voir si souci de performances ?
                if (Rotation_Objets) { // Rotation visuelle. Ici seulement les objets complets sélectionnés.
                    glPushMatrix();
                    // Est-ce que l'objet i est dans listeObjets ?
                    int n_val = listeObjets.size();
                    auto it   = listeObjets.begin();
                    for (int ii=0; ii<n_val; ii++, it++) {
                        if (*it == (int)i) {
                            // Rotation visuelle uniquement. Pas de changement dans la Bdd (Sommets, normales, ... inchangés)
                            glTranslated(Centre_X,   Centre_Y,  Centre_Z);  // Recentrer en 0,0,0
                            glRotated(Rot_Z,0.0,0.0,1.0);                   // Effectuer les 3 rotations
                            glRotated(Rot_Y,0.0,1.0,0.0);
                            glRotated(Rot_X,1.0,0.0,0.0);
                            glTranslated(-Centre_X, -Centre_Y, -Centre_Z);  // Replacer l'objet à sa position originale
                        }
                    }
                }

                if (Changer_Echelle) { // Mis à l'échelle visuelle. Ici seulement les objets complets sélectionnés.
                    glPushMatrix();
                    // Est-ce que l'objet i est dans listeObjets ?
                    int n_val = listeObjets.size();
                    auto it   = listeObjets.begin();
                    for (int ii=0; ii<n_val; ii++, it++) {
                        if (*it == (int)i) {
                            // Mise à l'échelle visuelle uniquement. Pas de changement dans la Bdd (Sommets, normales, ... inchangés)
                            glTranslated(Centre_X,   Centre_Y,  Centre_Z);  // Recentrer en 0,0,0
                            glScaled(Scale_X,Scale_Y,Scale_Z);              // Mise à l'échelle des 3 axes
                            glTranslated(-Centre_X, -Centre_Y, -Centre_Z);  // Replacer l'objet à sa position originale
                        }
                    }
                }

                for(j=0; j<Objet_i->Facelist.size(); j++) {
                    glPushName(j);

                    // Identifier les objets courants (facette, aspect, normale à la facette, luminance si Objet/facette non plat)
                    // => écriture + simple et execution + rapide

                    Face_ij = &(Objet_i->Facelist[j]);

                    if (Face_ij->afficher && !Face_ij->deleted) {
                        NormaleFacette.clear();
                        NumerosSommets.clear();

                        if (show_CW_CCW==false) {
                            if((groupes==false)&&(materials==false)) {
                            glColor3fv(Face_ij->color); // Etait initialisé ou modifié dans colorface
                                glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avion)  ;
                                glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avion)  ;
                                glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
                                glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);

                                if (SelectionObjet != 0) { // On est en mode Repérage d'objets => Coloriser l'objet sélectionné en rouge
                                    if ((SelectionObjet == i+1) || Objet_i->selected ) {            // 1er test utile ??
                                        glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionR)  ;
                                        glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionR)  ;
                                        glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionR) ;
                                        glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionR);
                                    }
                                }

                                // Colorisation d'un groupe ou d'un matériau particulier en bleu
                                if (GroupeMateriau[0] != 0) {
                                    int grpmat;

                                    if (GroupeMateriau[0] == 1) grpmat = Face_ij->getGroupe();        // On est dans Repérage Groupes
                                    else                        grpmat = Face_ij->getCodmatface();    // On est dans Repérage Matériaux
                                    if (grpmat == GroupeMateriau[1]) {
                                        glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionB)  ;
                                        glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionB)  ;
                                        glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionB) ;
                                        glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionB);
                                    }
                                }

                            } else {
                                int groupe   = Face_ij->getGroupe();
                                int material = Face_ij->getCodmatface();
                                int color_max= nb_couleurs-1;
//                                  printf("objet %d facette %d groupe %d material %d\n",i,j,groupe,material);
                                if (groupe<0) {
                                    groupe=0;
                                } else if(groupe>color_max) {
                                    groupe=color_max;
                                }
                                if (material<0) {
                                    material=10;    // Si material non initialisé (à -1) tracer en rouge comme un jet
                                } else if(material>color_max) {
                                    material=color_max;
                                }
                                if(groupes == true) {
                                    // Le glColor3f ci-dessous n'existe pas dans la version Tcl. Pourquoi le faut-il ici ? cf InitGL peut-être
                                    // pour activer ambiant et diffuse dans glColorMaterial !!!
//                                        glColor3fv(color_groupe_material[groupe]) ;//[0], // écriture + concise qu'avec glColor3f
                                    glColor3fv(MatAmbient_avionG[groupe]) ;//[0], // écriture + concise qu'avec glColor3f même avec 4 valeurs par groupe dans MatAmbient_avionG
//                                                  color_groupe_material[groupe][1],
//                                                  color_groupe_material[groupe][2]);
                                    glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionG[groupe])  ;
                                    glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionG[groupe])  ;
                                    glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
                                    glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);
                                } else {
//                                        glColor3fv(color_groupe_material[material]) ;
                                    glColor3fv(MatAmbient_avionG[material]) ;   // Idem ci-dessus pour les groupes
                                    glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionG[material])  ;
                                    glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionG[material])  ;
                                    glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
                                    glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);
                                }
                            }

                        } else {
                            // Ces 2 glColor3fv nécessitent que glColorMaterial et glEnable(GL_COLOR_MATERIAL) soient actifs dans InitGL;
                            // De plus il faut les glColor3fv(color_groupe ... ci-dessus) soient présents ! Pour éviter ce double coloriage, il faudrait traiter le sens des normales
                            // comme le reste, soit avec des glMaterialfv (cf version TCL) ou à la rigueur disable puis enable de GL_LIGHTING. à voir à l'occasion ...
                            if(test2 == true) {
                                glColor3fv(rouge);  // Vert initialement, mais souci en cas de sélection sur Sens de rotation inversé (même couleur => ne se voit pas).
//                              glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionRG) ;    // Pas convainquant, de plus inhibe la sélection des facettes ! => faire autrement
//                              glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionRG) ;
//                              glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
//                              glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);
                            } else {                // Peut-être essayer un vert + clair (vert_c) ou + foncé (vert_f) ? Bof !
                                glColor3fv(bleu);
//                              glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionBG) ;
//                              glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionBG) ;
//                              glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
//                              glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);
                            }
                        }

                        NormaleFacette = Face_ij->getNormale_b();

                        switch(Face_ij->Nb_Sommets_F) {
                        case 2:                     // Lignes simples ou facette dégénérées réduites à 2 points !
                            glBegin(GL_LINES);
                            NumerosSommets = Face_ij->getF_sommets();
                            for(k=0; k<NumerosSommets.size(); k++) {
                                glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);  // ??? BUG ??? d'où vient xyz_sommet dans ce cas ????
                            }
                            glEnd();
                            break;

//! NOTE : la distinction GL_TRIANGLES, GL_QUADS n'apporte pas grand chose / GL_POLYGON : c'est le même code C++ dans chaque "case"
//!        => ne pas le dupliquer sauf si raisons de performances !

    //                     case 3:
    //                        glBegin(GL_TRIANGLES);
    // ...
    //                        glEnd();
    //                        break;
    //

                        default:
                            glBegin(GL_POLYGON);
                            NumerosSommets = Face_ij->getF_sommets();
                            style = GL_POLYGON; // Remplace la ligne ci-dessous ? Oui si mode=11 mais sinon ????

                            lissage_Gouraud = smooth && !Objet_i->flat && !Face_ij->flat && (Face_ij->Nb_Sommets_L != 0) ; // On pourrait utiliser aussi Face_ij->L_sommets.size() != 0
                            // Il faudrait peut-être aussi tester si Face_ij->Nb_Sommets_F == Face_ij->Nb_Sommets_L (ou Face_ij->F_sommets.size() == Face_ij->L_sommets.size() )
                            for(k=0; k<NumerosSommets.size(); k++) {
                                if (lissage_Gouraud) {
                                    // Facette non plane => utiliser le lissage de Gouraud (et donc les normales aux sommets)
                                    NormaleSommet = Objet_i->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                                    test_np = Calcul_Normale_Seuillee(i,j,k,NormaleFacette,NormaleSommet) ;
                                    if (test_np) nb_normales_seuillees++;
                                    glNormal3f(NormaleSommet[0], NormaleSommet[1], NormaleSommet[2]);
                                } else {
                                    // Facette plane
                                    glNormal3f(NormaleFacette[0],NormaleFacette[1],NormaleFacette[2]);
                                }
                                xyz_sommet = Objet_i->Sommetlist[NumerosSommets[k]-1].getPoint();
                                if (Changer_Echelle) {
                                    if (Objet_i->Sommetlist[NumerosSommets[k]-1].selected  || Face_ij->selected) {  // en mode selection_point ou selection_facette
                                        xyz_sommet[0] -= Centre_X ; xyz_sommet[1] -= Centre_Y ; xyz_sommet[2] -= Centre_Z;
                                        xyz_sommet[0] *= Scale_X  ; xyz_sommet[1] *= Scale_Y  ; xyz_sommet[2] *= Scale_Z ;
                                        xyz_sommet[0] += Centre_X ; xyz_sommet[1] += Centre_Y ; xyz_sommet[2] += Centre_Z;
                                    }
                                }
                                glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                            }
                            glEnd();
                            break;
                        }
                    }
                    glPopName();
                }

                if (Rotation_Objets || Changer_Echelle) {
                    glPopMatrix();
                }

                glPopName();
            }   // Objet_i->afficher && !Objet_i->deleted

        }       // for(i=0; i<this->Objetlist.size(); ...

        // Si le mode "Sens des normales" est activé : Pour forcer une seconde exploration complète des boucles sur i et j en changeant le mode glFrontFace
        if((test2 == false) && (show_CW_CCW == true) ) {
            glFrontFace(GL_CW);
            test2 = true;
            goto Boucle;    // Pas joli, pas pour les puristes, mais c'est simple, évite un test sur l'indice i+1 et de mettre i à -1 pour forcer l'itération suivante avec i=0 ...
        }

        if (show_CW_CCW == true) {
            glDisable(GL_CULL_FACE);
        }
        if (MPanel->Bool_souder) {
            modeGL = points;
        } else if(MPanel->Bool_diviser) {
            modeGL = aretes;
        } else {
            modeGL = standard;
        }
//        diagonale_save = Norme3(maxx-minx, maxy-miny, maxz-minz) ;
        if (MPanel->activer_transparence) {
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        glEndList();

        SetPosObs(reset_zoom);
        ResetProjectionMode();

        buildAllLines();
        buildAllPoints();
        buildBoite();
        buildRepereOXYZ();
        buildAllFacettesSelected();

        finishdraw = true;
        m_gllist   = glliste_objets; // Déjà comme ça en principe
        Refresh();                  /// S'il n'est pas là, pas d'affichage des objets la première fois !
    }   // if (m_gllist == 0)

    // En dehors des créations de liste => fait systématiquement

    if (m_gllist == glliste_lines) {
        if (verbose) printf("Reconstruction de la liste de aretes seulement\n");
        buildAllLines();
        m_gllist = glliste_objets;
    }
    if (m_gllist == glliste_points) {                                           // le signe - utilisé auparavant n'est plus justifié
        if (verbose) printf("Reconstruction de la liste de points seulement\n");
        buildAllPoints();
        m_gllist = glliste_objets;
    }
    if (m_gllist == glliste_select) {
        buildAllFacettesSelected();
        m_gllist = glliste_objets;
    }

    if (verbose) printf("Affichage des listes\n");

    if (show_plein)  {
        glCallList(glliste_objets);    // La valeur m_gllist semble écrasée ou différente de 1 dans certains cas (grosses BDD notamment) : pourquoi ?
        if ((mode_selection == selection_facette) && (!MPanel->Bool_souder))
            glCallList(glliste_select);
    }
    if (show_lines)  glCallList(glliste_lines) ;    // <=> showAllLines() mais appel direct de la liste donc bien plus rapide;
    if (show_points) {
            glCallList(glliste_points);     // <=> showAllPoints()  ""  ;
            showPoint();                    // Pour n'afficher que les points surlignés en jaune au survol ou un premier point sélectionné en rouge lors d'une soudure
    }
    if (show_box)   glCallList(glliste_boite) ;     // <=> AffichageBoite() ""  ;
    if (show_axes)  glCallList(glliste_repere);     // <=> repereOXYZ()     ""  ;
    if (show_light) AfficherSource();               // Une liste pourrait être utile ? pas sûr !
    if (show_star)  GenereEtoile();

    if (segment_surligne) showSegment();            // Pas de liste ici ; génération directe du segment surligné

    if (Symetrie_Objets) {              // Tracer une boîte englobante rouge autour l'objet d'origine et une cyan pour matérialiser l'objet à créer
        glColor3fv(rouge);
        TracerBoite(x1_b1,x2_b1,y1_b1,y2_b1,z1_b1,z2_b1);   // Boîte de l'objet d'origine
        glColor3fv(cyan);
        TracerBoite(x1_b2,x2_b2,y1_b2,y2_b2,z1_b2,z2_b2);   // Boîte de l'objet à créer par symétrie
    }

    if (Facette_Surlignee) {    // Faire en dehors de la génération de liste car ici, on a une seule facette à traiter
//        printf("Facette surlignee : %d %d\n",Numero_Objet_a_Surligner, Numero_Facette_Surlignee);
        coloriserFacette(Numero_Objet_a_Surligner, Numero_Facette_Surlignee, true, jaune); // Ici tracé de la facette en jaune et des normales (si demandées)
    }

    if (viewFps) {
        delta_time = glutGet(GLUT_ELAPSED_TIME) - timebase;
        frames++;
        if (delta_time >= 200) {            // Attendre au moins 200 ms
            fps = frames*1000.0f/delta_time;
            frames = 0;
            timebase = glutGet(GLUT_ELAPSED_TIME);
            sprintf(Message_fps,"fps=%4.1f",fps) ;
    //	    printf("%s\n",Message_fps);
        }
        glColor3fv(bleu);       // Ecrire en bleu le message de Frames par secondes dans l'image, en bas, à gauche.
        output_Glut_Msg(0.005,0.01,Message_fps);
    }

    if (test_rectangle_selection) draw_rectangle_selection();   // Trace un rectangle pour sélectionner les facettes qui sont internes

    SetFocus(); // Donne le focus à la fenètre OpenGL => utilisation des touches de raccourcis clavier

    if (verbose)
        printf("Sortie BddInter::drawOpenGL\n");
}

void BddInter::Flat_Selected_Facettes() {
// Force le mode "Facettes Planes" des facettes sélectionnées
    unsigned int i,j;
    Face *Face_ij;

    for(i=0; i<this->Objetlist.size(); i++) {
        for(j=0; j<this->Objetlist[i].Facelist.size(); j++) {
            Face_ij = &(this->Objetlist[i].Facelist[j]);
            if(Face_ij->afficher && !Face_ij->deleted) {    // Utile ?
                if (Face_ij->selected) Face_ij->flat = true;
            }
        }
    }
    bdd_modifiee = true;    // Marquer BDD modifiée pour rappeler à Enregistrer en sortie
}

void BddInter::NotFlat_Selected_Facettes() {
// Force le mode "Facettes Non Planes" des facettes sélectionnées
    unsigned int i,j;
    Face *Face_ij;

    for(i=0; i<this->Objetlist.size(); i++) {
        for(j=0; j<this->Objetlist[i].Facelist.size(); j++) {
            Face_ij = &(this->Objetlist[i].Facelist[j]);
            if(Face_ij->afficher && !Face_ij->deleted) {    // Utile ?
                if (Face_ij->selected) Face_ij->flat = false;
            }
        }
    }
    bdd_modifiee = true;    // Marquer BDD modifiée pour rappeler à Enregistrer en sortie
}

void BddInter::Inverse_Selected_Normales() {
// Inverse les normales au barycentre et le sens de parcours des sommets des facettes sélectionnées.

    unsigned int i,k;
    int m;
    Face   *Face_ij;
    Object *Objet_i;
    bool no_selected = true;

    if (mode_selection == selection_objet) {
        auto it = listeObjets.begin();
        for (i=0; i<listeObjets.size(); i++, it++) {
            Inverser_les_Normales_Objet(*it);
        }
        m_gllist = 0;
        return;
    }

    if (mode_selection != selection_facette) return;    // On n'est pas dans le bon mode

    if (!(this->ToSelect.ListeSelect.empty())) {        // La liste est vide => inverser toutes les normales
        for(i=0; i<this->ToSelect.ListeSelect.size(); i++) {

            int objet = this->ToSelect.ListeSelect[i].objet;
            int numero= this->ToSelect.ListeSelect[i].face_sommet ;

//        printf("%d %d\n",objet,numero);

            Objet_i = &this->Objetlist[objet];

            if (Objet_i->afficher && !Objet_i->deleted) {   // Utile ?

                Face_ij = &(Objet_i->Facelist[numero]);
                if(Face_ij->afficher && !Face_ij->deleted) {
                    Face_ij->normale_b[0] *= -1;                                // Inverser la normale
                    Face_ij->normale_b[1] *= -1;
                    Face_ij->normale_b[2] *= -1;
                    bdd_modifiee = true;
                    no_selected  = false;
                    int base = Face_ij->F_sommets[0];
                    Face_ij->F_sommets[0] = Face_ij->F_sommets[Face_ij->F_sommets.size()-1];    // Puis le sens de parcours des points
                    for(k=1; k<(Face_ij->F_sommets.size()/2); k++) {
                        m = Face_ij->F_sommets[k];
                        Face_ij->F_sommets[k] = Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1];
                        Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1] = m;
                    }
                    Face_ij->F_sommets[Face_ij->F_sommets.size()-1]=base;
                }
            }
        }
    }

    if (no_selected) {
        Inverser_Toutes_les_Normales(); // Si aucune facette n'est sélectionnée, tout inverser
        m_gllist = 0;                   // Et regénérer toutes les listes
    } else
        m_gllist = glliste_select;      // Ne regénérer que les facettes sélectionnées

//    m_gllist = 0;   // Fait dans le OnMenu ...
//    Refresh();
}

void BddInter::DisplayMessage(wxString wxMessage, bool bip)
{
    long style     = wxOK | wxICON_QUESTION;        // Avec l'icône wxICON_QUESTION, l'affichage reste silencieux (wxICON_INFORMATION + logique, mais bruyant !!)
    if (bip) style = wxOK | wxICON_INFORMATION;

    wxMessageDialog *query = new wxMessageDialog(NULL, wxMessage, _T("Avertissement"), style);
    query->ShowModal();
    query->Destroy();
}

void BddInter::Inverser_Parcours_Selected() {
// <=> Inverse_Selected_Normales mais on ne garde que l'inversion du sens de parcours des sommets de facettes

// PROBLEME : avec la touche clavier "p", les facettes inversées deviennent parfois invisibles en mode affichage "Sens des Normales" (sur M2000_V9 mais pas sur 00_convertedf1nvCdKe_0 !!)

    unsigned int i;
    wxString wxMessage;

    if (mode_selection == selection_facette) {
        if (ToSelect.ListeSelect.empty()) {
            wxMessage = _T("Aucune Facette sélectionnée !");
            DisplayMessage(wxMessage,true);
            return;
        }
//        printf("Inverser parcours des facettes sélectionnées\n");
        for(i=0; i<this->Objetlist.size(); i++) {
            Inverser_Parcours_Facettes_Objet(i, false);         // Seulement les facettes sélectionnées
        }
        bdd_modifiee = true;
    }

    if (mode_selection == selection_objet) {
        if (listeObjets.empty()) {
            wxMessage = _T("Aucun Objet sélectionné !");
            DisplayMessage(wxMessage,true);
            return;
        }
//        printf("Inverser parcours des facettes des objets sélectionnés\n");
        auto it = listeObjets.begin();
        for (unsigned int i=0; i<listeObjets.size(); i++, it++) {
            Inverser_Parcours_Facettes_Objet(*it, true);        // Toutes les facettes des objets sélectionnés
        }
        bdd_modifiee = true;
    }

//    m_gllist = 0;   // Fait dans le OnMenu ...
//    Refresh();
}

void BddInter::Inverser_Parcours_Facettes_Objet(unsigned int i, bool all) {
// <=> Inverse_Selected_Normales mais on ne garde que l'inversion du sens de parcours des sommets de facettes
// Si all est : false, ne touche que les facettes sélectionnées de l'objet i
//            : true, toutes les facettes de l'objet i sont inversées

    unsigned int j,k;
    int m;
    Face *Face_ij;
    std::vector<int> NumerosSommets;
    std::vector<int> ReverseSommets;
    bool test_print = false;

//    printf("Inverser parcours\n");

    for(j=0; j<this->Objetlist[i].Facelist.size(); j++) {
        Face_ij = &(this->Objetlist[i].Facelist[j]);
        if(Face_ij->afficher && !Face_ij->deleted) {
            if (Face_ij->selected || all) {
                NumerosSommets = Face_ij->getF_sommets();
                ReverseSommets.clear();
                if (test_print) {
                    for (unsigned int k=0; k<NumerosSommets.size(); k++) printf("%d ",NumerosSommets[k]);
                    printf("\n");
                }
                for (auto it=NumerosSommets.crbegin(); it != NumerosSommets.crend(); ++it) ReverseSommets.push_back(*it);
                Face_ij->setFsommet(ReverseSommets);

                if (test_print) {
                    NumerosSommets = Face_ij->getF_sommets();
                    for (k=0; k<NumerosSommets.size(); k++) printf("%d ",NumerosSommets[k]);
                    printf("\n");
                }
                // Idem sur les normales aux sommets, si elles existent
                NumerosSommets = Face_ij->getL_sommets();
                if (NumerosSommets.size() > 0) {
                    ReverseSommets.clear();
                    for (auto it=NumerosSommets.crbegin(); it != NumerosSommets.crend(); ++it) ReverseSommets.push_back(*it);
                    Face_ij->setLsommet(ReverseSommets);
                }
            }
        }
    }
}

void BddInter::Inverser_Toutes_les_Normales() {
    unsigned int o;
    for(o=0; o<this->Objetlist.size(); o++) Inverser_les_Normales_Objet(o);
}

void BddInter::Inverser_les_Normales_Objet(unsigned int o) {
    unsigned int j,k;
    int m;
    Face *Face_ij;
    Face *Luminance_ij;
    Vecteur *Vecteur_ij;
    Object  *objet_courant;

    objet_courant = &(this->Objetlist[o]);

// Inversion des normales au barycentre et du sens de parcours
    for(j=0; j<objet_courant->Nb_facettes; j++) {
        Face_ij = &(objet_courant->Facelist[j]);
        if(Face_ij->afficher && !Face_ij->deleted) {
            Face_ij->normale_b[0] *= -1;                                    // Inverse le signe de la normale
            Face_ij->normale_b[1] *= -1;
            Face_ij->normale_b[2] *= -1;
            int base = Face_ij->F_sommets[0];
            Face_ij->F_sommets[0] = Face_ij->F_sommets[Face_ij->F_sommets.size()-1];    // Puis le sens de parcours des points
            for(k=1; k<(Face_ij->F_sommets.size()/2); k++) {
                m = Face_ij->F_sommets[k];
                Face_ij->F_sommets[k] = Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1];
                Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1] = m;
            }
            Face_ij->F_sommets[Face_ij->F_sommets.size()-1]=base;
        }
    }

// Inversion du signe des normales aux sommets (vecteurs)
    for(j=0; j<objet_courant->Nb_vecteurs; j++) {
        Vecteur_ij = &(objet_courant->Vecteurlist[j]);
        if (!Vecteur_ij->point.empty()) {
            Vecteur_ij->point[0] *= -1;
            Vecteur_ij->point[1] *= -1;
            Vecteur_ij->point[2] *= -1;
        }
    }

//    for(j=0; j<objet_courant->Nb_luminances; j++) {   // Souci si différent du nombre de facettes !
    for(j=0; j<objet_courant->Nb_facettes; j++) {
        Luminance_ij = &(objet_courant->Facelist[j]);
        if (!Luminance_ij->L_sommets.empty()) {
            int base = Luminance_ij->L_sommets[0];
            Luminance_ij->L_sommets[0] = Luminance_ij->L_sommets[Luminance_ij->L_sommets.size()-1];    // Puis le sens de parcours des points
            for(k=1; k<(Luminance_ij->L_sommets.size()/2); k++) {
                m = Luminance_ij->L_sommets[k];
                Luminance_ij->L_sommets[k] = Luminance_ij->L_sommets[Luminance_ij->L_sommets.size()-k-1];
                Luminance_ij->L_sommets[Luminance_ij->L_sommets.size()-k-1] = m;
            }
            Luminance_ij->L_sommets[Luminance_ij->L_sommets.size()-1]=base;
        }
    }

    bdd_modifiee = true;

//    m_gllist = 0;   // Fait dans le OnMenu ...
//    Refresh();
}

void BddInter::buildAllFacettesSelected() {

    // Repris en grande partie de drawOpenGL

//    char test;
    std::vector<int>   NumerosSommets;
    std::vector<float> xyz_sommet;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;
    Face   *Face_ij;
    Object *Objet_i;
    unsigned int i=0,j=0,k=0;
//    bool test_np;
    bool lissage_Gouraud;

    if (verbose)
        printf("Entree BddInter::buildAllFacettesSelected\n");

    glDeleteLists(glliste_select,1);                                // Détruire une éventuelle liste existante de facettes sélectionnées
    if (this->ToSelect.ListeSelect.empty()) return;                 // Rien de plus à faire

    glNewList(glliste_select, GL_COMPILE_AND_EXECUTE) ;             // Création d'une liste de facettes sélectionnées

    if (verbose)
        printf("Reconstruction de la liste des facettes sélectionnées\n");

//    int nb_normales_seuillees = 0; // A déclarer plutôt ailleurs, mais initialiser ici

    for(i=0; i<this->ToSelect.ListeSelect.size(); i++) {

        int objet = this->ToSelect.ListeSelect[i].objet;
        int numero= this->ToSelect.ListeSelect[i].face_sommet ;

//        printf("%d %d\n",objet,numero);

        // Objet courant
        Objet_i = &this->Objetlist[objet];

        if (Objet_i->afficher && !Objet_i->deleted) {

            Face_ij = &(Objet_i->Facelist[numero]);

            if (Changer_Echelle) { // Mis à l'échelle visuelle
                glPushMatrix();
                // Mise à l'échelle visuelle uniquement. Pas de changement dans la Bdd (Sommets, normales, ... inchangés)
                // l'effet n'est pas top car 2 surfaces se superposent : donne des facettes composites grises et vertes
                glTranslated(Centre_X,   Centre_Y,  Centre_Z);  // Recentrer en 0,0,0
                glScaled(Scale_X,Scale_Y,Scale_Z);              // Mise à l'échelle des 3 axes
                glTranslated(-Centre_X, -Centre_Y, -Centre_Z);  // Replacer l'objet à sa position originale
            }

            if (Face_ij->afficher && !Face_ij->deleted) {  // Test sur show et afficher ???
                NormaleFacette.clear();
                NumerosSommets.clear();
                // Note : les facettes sélectionnées et leurs normales sont incluses dans la liste
                glColor3fv(vert);
                coloriserFacette(objet,numero,true, vert);

                NormaleFacette = Face_ij->getNormale_b();

                switch(Face_ij->Nb_Sommets_F) {
                case 2:                     // Lignes simples ou facette dégénérées réduites à 2 points !
                    glBegin(GL_LINES);
                    NumerosSommets = Face_ij->getF_sommets();
                    for(k=0; k<NumerosSommets.size(); k++) {
                        glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);  // ??? BUG ??? d'où vient xyz_sommet dans ce cas ????
                    }
                    glEnd();
                    break;

                default:
                    glBegin(GL_POLYGON);
                    NumerosSommets = Face_ij->getF_sommets();
                    style = GL_POLYGON; // Remplace la ligne ci-dessous ? Oui si mode=11 mais sinon ????

                    lissage_Gouraud = smooth && !Objet_i->flat && !Face_ij->flat && (Face_ij->Nb_Sommets_L != 0) ; // On pourrait utiliser aussi Face_ij->L_sommets.size() != 0
                    // Il faudrait peut-être aussi tester si Face_ij->Nb_Sommets_F == Face_ij->Nb_Sommets_L (ou Face_ij->F_sommets.size() == Face_ij->L_sommets.size() )
                    for(k=0; k<NumerosSommets.size(); k++) {
                        if (lissage_Gouraud) {
                            // Facette non plane => utiliser le lissage de Gouraud (et donc les normales aux sommets)
                            NormaleSommet = Objet_i->Vecteurlist[Face_ij->L_sommets[k]-1].point;
//                            test_np =
                            Calcul_Normale_Seuillee(i,j,k,NormaleFacette,NormaleSommet) ;
 //                           if (test_np) nb_normales_seuillees++;
                            glNormal3f(NormaleSommet[0], NormaleSommet[1], NormaleSommet[2]);
                        } else {
                            // Facette plane
                            glNormal3f(NormaleFacette[0],NormaleFacette[1],NormaleFacette[2]);
                        }
                        xyz_sommet = Objet_i->Sommetlist[NumerosSommets[k]-1].getPoint();
//                        if (Changer_Echelle) {
////                            if (Face_ij->selected) { // C'est forcément le cas ici !
//                                xyz_sommet[0] -= Centre_X ; xyz_sommet[1] -= Centre_Y ; xyz_sommet[2] -= Centre_Z;
//                                xyz_sommet[0] *= Scale_X  ; xyz_sommet[1] *= Scale_Y  ; xyz_sommet[2] *= Scale_Z ;
//                                xyz_sommet[0] += Centre_X ; xyz_sommet[1] += Centre_Y ; xyz_sommet[2] += Centre_Z;
////                            }
//                        }
                        glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                    }
                    glEnd();

                    break;
                }
            }

            if (Changer_Echelle) {
                glPopMatrix();
            }

        }   // Objet_i->afficher && !Objet_i->deleted

    }       // for(i=0; i<this->ToSelect.ListeSelect.size() ...

    glEndList();
}

void BddInter::showPoint() {

    std::vector<float> xyz_sommet;
    Object*     objet_courant;

    if(Smemory != nullptr) {                                        // Tracé en rouge du point déjà sélectionné
        objet_courant = &(this->Objetlist[Smemory->objet]);
        if (objet_courant->afficher && !objet_courant->deleted){
            glDisable(GL_LIGHTING);
            glBegin(GL_POINTS);
            xyz_sommet=objet_courant->Sommetlist[Smemory->sommet].getPoint();
            glColor3fv(rouge);
            if (!xyz_sommet.empty()) glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            glEnd();
            glEnable(GL_LIGHTING);
        }
    }
    if (objet_under_mouse != -1 && point_under_mouse != -1) {       // Tracé en jaune du point survolé
        objet_courant = &(this->Objetlist[objet_under_mouse]);
        if (objet_courant->afficher && !objet_courant->deleted){
            glDisable(GL_LIGHTING);
            glBegin(GL_POINTS);
            xyz_sommet=objet_courant->Sommetlist[point_under_mouse].getPoint();
            glColor3fv(jaune);
            if (!xyz_sommet.empty()) glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            glEnd();
            glEnable(GL_LIGHTING);
        }
    }
}

void BddInter::buildAllPoints() {
// Utilise showAllPoints pour construire une liste de points

    if (verbose)
        printf("Entrée BddInter::buildAllPoints\n");

    glDeleteLists(glliste_points,1);         // Détruire une éventuelle liste de points existante
    glNewList(glliste_points, GL_COMPILE_AND_EXECUTE) ;
    glInitNames();
    showAllPoints();
    glEndList();
}

void BddInter::showAllPoints() {
    std::vector<float> xyz_sommet;
    Object*     objet_courant;

    glDisable(GL_LIGHTING);

    for(unsigned int i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        if (objet_courant->afficher && !objet_courant->deleted){
//            glPushName(i+1);                    // +1 ??
            glPushName(i);
            for(unsigned int j=0; j<objet_courant->Sommetlist.size(); j++) {
                if (objet_courant->Sommetlist[j].show == true) {
//                    glPushName(j+1);            // +1 permet d'identifier par le numéro du point et non l'indice dans le tableau
                    glPushName(j);
                    xyz_sommet=objet_courant->Sommetlist[j].getPoint();
                    glPushName(-1);
                    glBegin(GL_POINTS);
                    glColor3fv(bleu);                                                   // Point bleu standard
                    if (objet_courant->Sommetlist[j].selected) glColor3fv(vert);        // Point vert si on le sélectionne
                    if (Changer_Echelle) {
                        if (objet_courant->Sommetlist[j].selected) {
                            xyz_sommet[0] -= Centre_X ; xyz_sommet[1] -= Centre_Y ; xyz_sommet[2] -= Centre_Z;
                            xyz_sommet[0] *= Scale_X  ; xyz_sommet[1] *= Scale_Y  ; xyz_sommet[2] *= Scale_Z ;
                            xyz_sommet[0] += Centre_X ; xyz_sommet[1] += Centre_Y ; xyz_sommet[2] += Centre_Z;
                        }
                    }
                    if (!xyz_sommet.empty()) glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]); // Ne pas tenir compte de points vides (non utilisés a priori !)
                    glEnd();
                    glPopName();
                    glPopName();
                }
            }
            glPopName();
        }
    }
    glEnable(GL_LIGHTING);
}

void BddInter::buildAllLines() {
// Utilise showAllLines pour construire une liste de segments

    if (verbose)
        printf("Entrée BddInter::buildAllLines\n");

    glDeleteLists(glliste_lines,1);         // Détruire une éventuelle liste d'arêtes existante
    glNewList(glliste_lines, GL_COMPILE) ;  // ou GL_COMPILE_AND_EXECUTE ?
    glInitNames();
    showAllLines();
    glEndList();
}

void BddInter::showAllLines() {

// Version GD utilisant la génération préalable des arêtes de GenereTableauAretes.
// Liste de toutes les arêtes. La ligne survolée (en vert) sera générée à part, hors liste.

    std::vector<float> xyz_sommet;
    unsigned int i,j,k;
    Aretes*  Arete;
    Object*     objet_courant;

    if (verbose)
        printf("Entrée BddInter::showAllLines\n");

    GLfloat l_width_n = 1.;                 // Largeur de ligne normale (1 devrait aller mais un peu juste pour sélectionner !)
    GLfloat l_width_e = l_width_n*1.05;     // Augmenter un peu la largeur de ligne pour l'antialiasing soft. Initialement 1.75

    glDisable(GL_LIGHTING);

    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        glLineWidth(l_width_n);
        if (objet_courant->afficher && !objet_courant->deleted){
            int Nb_lignes=0;
            glPushName(i);
            for(j=0; j<objet_courant->Areteslist.size(); j++) {
                Arete = &(objet_courant->Areteslist[j]);
                if (Arete->deleted) continue;       // L'arête a été supprimée (doublon ou autre raison), passer à la suivante
                Nb_lignes++;
                glPushName(j);
                glPushName(j);  // Un de plus (pour compatibilité avec la version originale et l'offset dans letscheckthemouse) !
                if (Arete->afficher) {
                    glColor3fv(blanc);  // cyan +clair
                    if (antialiasing_soft) {
                        glLineWidth(l_width_e);
                        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
                        glEnable(GL_LINE_SMOOTH);
                        glEnable(GL_BLEND);
                        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
                    }
                    glBegin(GL_LINES);
                    xyz_sommet = objet_courant->Sommetlist[Arete->ind_a].getPoint();
                    glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
//                    xyz_sommet = objet_courant->Areteslist[j].point_b;
                    xyz_sommet = objet_courant->Sommetlist[Arete->ind_b].getPoint();
                    glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                    glEnd();
                    if (antialiasing_soft) {
                        glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
                        glDisable(GL_LINE_SMOOTH);
                        glDisable(GL_BLEND);
                        glLineWidth(l_width_n);
                    }
                }
                glPopName();
                glPopName();
            }
            if (verbose) printf("Nombre de lignes à tracer : %d\n",Nb_lignes);
        }
        glPopName();
        glLineWidth(l_width_n); // Par précaution
    }
    glEnable(GL_LIGHTING);
}

void BddInter::showSegment() {

// Version GD utilisant la génération préalable des arêtes de GenereTableauAretes.
// Reprise de l'ancien shwAllLines, générant toutes les lignes systématiquement.

    std::vector<float> xyz_sommet;
    unsigned int i,j,k;
    Aretes* Arete;
    Object* objet_courant;

    GLfloat l_width_n = 1.;     // Largeur de ligne normale
    GLfloat l_width_e = 3.;     // Largeur de ligne plus épaisse

    if((objet_under_mouse == -1) && (line_under_mouse == -1)) return;

    objet_courant = &(this->Objetlist[objet_under_mouse]);
    if (objet_courant->afficher && !objet_courant->deleted){
        Arete = &(objet_courant->Areteslist[line_under_mouse]);
        if (Arete->deleted) return;       // L'arête a été supprimée : ignorer

        if (Arete->afficher) {
            glDisable(GL_LIGHTING);
            glLineWidth(l_width_e);
            glColor3fv(vert);
            if (antialiasing_soft) {
                glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
                glEnable(GL_LINE_SMOOTH);
                glEnable(GL_BLEND);
                glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
            }
            glBegin(GL_LINES);
            xyz_sommet = objet_courant->Sommetlist[Arete->ind_a].getPoint();
            glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            xyz_sommet = objet_courant->Sommetlist[Arete->ind_b].getPoint();
            glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            glEnd();
            if (antialiasing_soft) {
                glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
                glDisable(GL_LINE_SMOOTH);
                glDisable(GL_BLEND);
            }
            glLineWidth(l_width_n);
            glEnable(GL_LIGHTING);
        }
    }
}

// Copie locale de fghCircleTable dans freeglut_geometry.c 2.8.1 : lignes 124 ...
void BddInter::fghCircleTable_local(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = abs(n);

    /* Determine the angle between samples */

    const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        free(*sint);
        free(*cost);
///        fgError("Failed to allocate memory in fghCircleTable");
        printf("Echec d'allocation memoire dans fghCircleTable\n");
        exit (1);
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}

/*
 * Draws a solid sphere (Copie locale de glutSolidSphere dans freeglut_geometry.c 2.8.1) : lignes 167 ...
 * Raison : dans Freeglut 3.x la fonction intégrée correspondante plante dans Ovni. Pourquoi ???
 */
void BddInter::glutSolidSphere_local(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks are drawn. */

    double z0,z1;
    double r0,r1;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;

///    FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidSphere" );

    fghCircleTable_local(&sint1,&cost1,-slices);
    fghCircleTable_local(&sint2,&cost2,stacks*2);

    /* The top stack is covered with a triangle fan */

    z0 = 1.0;
    z1 = cost2[(stacks>0)?1:0];
    r0 = 0.0;
    r1 = sint2[(stacks>0)?1:0];

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,1);
        glVertex3d(0,0,radius);

        for (j=slices; j>=0; j--)
        {
            glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
            glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
        }

    glEnd();

    /* Cover each stack with a quad strip, except the top and bottom stacks */

    for( i=1; i<stacks-1; i++ )
    {
        z0 = z1; z1 = cost2[i+1];
        r0 = r1; r1 = sint2[i+1];

        glBegin(GL_QUAD_STRIP);

            for(j=0; j<=slices; j++)
            {
                glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
                glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
                glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
                glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
            }

        glEnd();
    }

    /* The bottom stack is covered with a triangle fan */

    z0 = z1;
    r0 = r1;

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,-1);
        glVertex3d(0,0,-radius);

        for (j=0; j<=slices; j++)
        {
            glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
            glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
        }

    glEnd();

    /* Release sin and cos tables */

    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);
}

void BddInter::AfficherSource() {

    GLdouble rayon;
//    printf("ShowTheLight\n");
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glColor3fv(jaune) ;     // Jaune ici pour figurer plus ou moins un soleil, mais en fait devrait être blanc!!!
    glTranslatef(centreRot[0],centreRot[1],centreRot[2]) ;
    glRotatef(-m_gldata.rotz, 0.0, 0.0, 1.0);
    glRotatef(-m_gldata.roty, 0.0, 1.0, 0.0);
    glRotatef(-m_gldata.rotx, 1.0, 0.0, 0.0);
    float lpx=+Light0Position[0]*diagonale_save/17.7;   // Affiche au même endroit sur l'écran que sur le M2000 (diagonale de 17.7 m)
    float lpy=+Light0Position[1]*diagonale_save/17.7;   // du coup, les Light0Position sont en coordonnées relatives et non absolues !
    float lpz=+Light0Position[2]*diagonale_save/17.7;
//    printf("diagonale_save : %f\n",diagonale_save);
//    glTranslatef(+Light0Position[0],+Light0Position[1],+Light0Position[2]);
    glTranslatef(lpx,lpy,lpz);
    rayon = ray_sun*diagonale_save ;
    glutSolidSphere_local(rayon,20,20);       // Avec freeglut 3.0 de base, plante ici dans un appel interne à glutGameModeGet
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

double BddInter::Norme3(float x, float y, float z) {
    double norme;
    norme=sqrt(x*x + y*y + z*z) ;
    return norme;
}

void BddInter::buildBoite() {
    float dx,dy,dz ;

//    printf("Afficher la boite\n");

// Peut être fait une fois, puis stocké dans une liste.
// Réinitialiser seulement si changements dans les min et max...

    glDeleteLists(glliste_boite,1); // Supprimer une éventuelle liste 4 existante
    glNewList(glliste_boite, GL_COMPILE);

// Modif GD : ajustement du marquage si trop fin en x, y ou z.
    dx=dy=dz = diagonale_save*0.1 ;
    if (x_max-x_min <= 2*dx) dx = (x_max-x_min)/3 ;
    if (y_max-y_min <= 2*dy) dy = (y_max-y_min)/3 ;
    if (z_max-z_min <= 2*dz) dz = (z_max-z_min)/3 ;
//    glPushName(-1);
    glDisable(GL_LIGHTING);
    glColor3fv(rouge);
    glLineWidth(1.2);

    if (antialiasing_soft) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }

    /*! maxx, maxy, maxz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max,    y_max, z_max);
    glVertex3f(x_max-dx, y_max, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_max,    z_max);
    glVertex3f(x_max, y_max-dy, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_max, z_max)   ;
    glVertex3f(x_max, y_max, z_max-dz);
    glEnd();

    /*! minx, maxy, maxz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min,    y_max, z_max);
    glVertex3f(x_min+dx, y_max, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_max,    z_max);
    glVertex3f(x_min, y_max-dy, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_max, z_max)   ;
    glVertex3f(x_min, y_max, z_max-dz);
    glEnd();

    /*! minx, miny, maxz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min,    y_min, z_max);
    glVertex3f(x_min+dx, y_min, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_min,    z_max);
    glVertex3f(x_min, y_min+dy, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_min, z_max)   ;
    glVertex3f(x_min, y_min, z_max-dz);
    glEnd();

    /*! maxx, miny, maxz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max,    y_min, z_max);
    glVertex3f(x_max-dx, y_min, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_min,    z_max);
    glVertex3f(x_max, y_min+dy, z_max);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_min, z_max)   ;
    glVertex3f(x_max, y_min, z_max-dz);
    glEnd();

    /*! minx, maxy, minz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min,    y_max, z_min);
    glVertex3f(x_min+dx, y_max, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_max,    z_min);
    glVertex3f(x_min, y_max-dy, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_max, z_min)   ;
    glVertex3f(x_min, y_max, z_min+dz);
    glEnd();

    /*! maxx, maxy, minz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max,    y_max, z_min);
    glVertex3f(x_max-dx, y_max, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_max,    z_min);
    glVertex3f(x_max, y_max-dy, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_max, z_min)   ;
    glVertex3f(x_max, y_max, z_min+dz);
    glEnd();

    /*! maxx, miny, minz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max,    y_min, z_min);
    glVertex3f(x_max-dx, y_min, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_min,    z_min);
    glVertex3f(x_max, y_min+dy, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_max, y_min, z_min)   ;
    glVertex3f(x_max, y_min, z_min+dz);
    glEnd();

    /*! minx, miny, minz */
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min,    y_min, z_min);
    glVertex3f(x_min+dx, y_min, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_min,    z_min);
    glVertex3f(x_min, y_min+dy, z_min);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_min, y_min, z_min)   ;
    glVertex3f(x_min, y_min, z_min+dz);
    glEnd();

    glLineWidth(1.0);
	if (antialiasing_soft) {
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	    glDisable(GL_LINE_SMOOTH);
	    glDisable(GL_BLEND);
	}
//    glPopName();
    glEnable(GL_LIGHTING);
    glEndList();
}

void BddInter::TracerBoite(double x1,double x2,double y1,double y2,double z1,double z2) {
// Trace une boîte complète (au contraire de buildBoite et AffichageBoite)
    glDisable(GL_LIGHTING);
    glLineWidth(1.2);

    if (antialiasing_soft) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }

    glBegin(GL_LINE_LOOP);
        glVertex3d(x1,y1,z1);
        glVertex3d(x2,y1,z1);
        glVertex3d(x2,y2,z1);
        glVertex3d(x1,y2,z1);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3d(x1,y1,z2);
        glVertex3d(x2,y1,z2);
        glVertex3d(x2,y2,z2);
        glVertex3d(x1,y2,z2);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex3d(x1,y1,z1);
        glVertex3d(x1,y1,z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
        glVertex3d(x2,y1,z1);
        glVertex3d(x2,y1,z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
        glVertex3d(x2,y2,z1);
        glVertex3d(x2,y2,z2);
	glEnd();

	glBegin(GL_LINE_STRIP);
        glVertex3d(x1,y2,z1);
        glVertex3d(x1,y2,z2);
	glEnd();

    glLineWidth(1.0);
	if (antialiasing_soft) {
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	    glDisable(GL_LINE_SMOOTH);
	    glDisable(GL_BLEND);
	}
    glEnable(GL_LIGHTING);
}

void BddInter::GenereEtoile() {
    float lon_1, lon_2 ;
    float xp,yp,zp;

    lon_1 = diagonale_save*0.05 ;
    lon_2 = lon_1*0.8 ;

    xp = point_star[0];
    yp = point_star[1];
    zp = point_star[2];

	glPushMatrix();
	glLineWidth(2);

	if (antialiasing_soft) {
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
        glEnable(GL_LINE_SMOOTH);   //Permet d'activer l'antialiasing (lissage) lorsqu'une droite est tracée
        glEnable(GL_BLEND);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	}

    glDisable(GL_LIGHTING);
	glColor3fv(violet) ;

	glBegin(GL_LINES);
		glVertex3d( xp+lon_1, yp, zp);
		glVertex3d( xp-lon_1, yp, zp);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp+lon_2, yp, zp-lon_2);
		glVertex3d( xp-lon_2, yp, zp+lon_2);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp+lon_2, yp, zp+lon_2);
		glVertex3d( xp-lon_2, yp, zp-lon_2);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp+lon_1, zp);
		glVertex3d( xp, yp-lon_1, zp);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp+lon_2, zp-lon_2);
		glVertex3d( xp, yp-lon_2, zp+lon_2);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp+lon_2, zp+lon_2);
		glVertex3d( xp, yp-lon_2, zp-lon_2);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp, zp+lon_1);
		glVertex3d( xp, yp, zp-lon_1);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp+lon_2, zp-lon_2);
		glVertex3d( xp, yp-lon_2, zp+lon_2);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( xp, yp+lon_2, zp+lon_2);
		glVertex3d( xp, yp-lon_2, zp-lon_2);
	glEnd();
	glLineWidth(1);
	if (antialiasing_soft) {
        glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
	}
    glEnable(GL_LIGHTING);
	glPopMatrix();
}

void BddInter::resetMin_Max() {
    x_min = +FLT_MAX;
    x_max = -FLT_MAX;
    y_min = +FLT_MAX;
    y_max = -FLT_MAX;
    z_min = +FLT_MAX;
    z_max = -FLT_MAX;
}

void BddInter::setMin_Max(float x, float y, float z) {
    x_min = ( x<x_min ? x : x_min);
    y_min = ( y<y_min ? y : y_min);
    z_min = ( z<z_min ? z : z_min);
    x_max = ( x>x_max ? x : x_max);
    y_max = ( y>y_max ? y : y_max);
    z_max = ( z>z_max ? z : z_max);
}

void BddInter::buildRepereOXYZ() {

    glDeleteLists(glliste_repere,1); // Supprimer une éventuelle liste 5 existante
    glNewList(glliste_repere, GL_COMPILE);
    float longueur = diagonale_save*len_axe; //10.0;

//printf("Tracer les axes\n");

    glDisable(GL_LIGHTING);
    glLineWidth(3);
//    glPushName(-1);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	if (antialiasing_soft) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	    glEnable(GL_LINE_SMOOTH);
	    glEnable(GL_BLEND);
	}

/* On trace les 3 axes dans des couleurs différentes */

    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 0, 0);      // Axe X en rouge
    glVertex3f(0,0,0);
    glVertex3f(longueur,0,0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 255, 0);      // Axe Y en vert
    glVertex3f(0,0,0);
    glVertex3f(0,longueur,0) ;
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3ub(0, 0, 255);      // Axe Z en bleu
    glVertex3f(0,0,0);
    glVertex3f(0,0,longueur);
    glEnd();

//    glPopName();
	glLineWidth(1);	            //taille en pixel d'une LINE

	if (antialiasing_soft) {
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	    glDisable(GL_LINE_SMOOTH);
	    glDisable(GL_BLEND);
	}
    glEnable(GL_LIGHTING);
    glEndList();
}

void BddInter::SetPosObs(bool resetFoV) {
    float FoV;

    if (verbose) printf("Entree BddInter::SetPosObs\n");

    m_gldata.posx = -centreRot[0]; //0.;
    m_gldata.posy = -centreRot[1]; //0.;
//
//    if (diagonale_save > 1000)
//        m_gldata.posz = -4.0*diagonale_save -centreRot[2] ;
        m_gldata.posz = -m_gldata.fmult_diag*diagonale_save -centreRot[2] ;     //Mieux que 1000 surtout sur des bases dont les min-max sont dans ces valeurs. Peut-être mettre plus 10* par exemple au lieu de 4 ?

    FoV = std::max(fabs(x_max-x_min), fabs(y_max-y_min));
    m_gldata.depl_xy = FoV/100.;
    FoV = fabs(FoV/m_gldata.posz)*to_Deg ;
    if (resetFoV) {
        m_gldata.zoom      = FoV_auto = FoV;
        m_gldata.zoom_step = FoV/100;
    }
    m_gldata.zNear = abs(m_gldata.posz) - 4*diagonale_save ; //  800. m. /// Plutôt que 200 m on pourrait utiliser la diagonale
    m_gldata.zFar  = abs(m_gldata.posz) + 4*diagonale_save ; // 1200. m. ///(en multipliant par 10 par exemple, pour avoir de la marge)

    if (verbose) printf("Sortie BddInter::SetPosObs\n");
}

void BddInter::testPicking(int cursorX, int cursorY, int mode, bool OnOff) {
    GLfloat width;
    //build_rotmatrix( m,v);
    //glTranslatef( +m_gldata.posx, +m_gldata.posy, +m_gldata.posz );

    if (verbose) printf("mode = ");
    if (mode == (int)standard) {
        if (verbose) printf("standard ");
        width = width_pixel;
        style= GL_POLYGON;          //!style plein
    } else if (mode == (int)points) {
        if (verbose) printf("points ");
        width = width_point;
        style= GL_POINTS;           //!style points
    } else if (mode == (int)aretes) {
        if (verbose) printf("aretes ");
        width = width_ligne;
        style= GL_LINE_LOOP;        //!style fils de fer
    } else {
        if (verbose) printf("inconnu ! ");
        width = width_pixel;
        style= GL_POLYGON;          //!style plein
    }
    if (verbose) printf("(%d)\n",mode);

//    selectMode(mode);   // Si on est en GL_POINTS on devrait avoir mode != standard !!!!
    wxSize ClientSize = this->GetSize();
//    printf("\nheuuu ?? %d %d \n",ClientSize.x, ClientSize.y);
    this->SetSize(ClientSize.x, ClientSize.y);
//    GLuint *selectBuffer;
    GLint hits=0;
    GLint viewport[4];

//    selectBuffer = (GLuint*)malloc(BUFSIZE*sizeof(GLuint)); // En statique et beaucoup + gros en version Tcl (BUFSIZE=102400 !!)

    glGetIntegerv(GL_VIEWPORT, viewport);

    glSelectBuffer(BUFSIZE, selectBuffer);
    glRenderMode(GL_SELECT);

    glInitNames();    // Sert à quoi ? Fait aussi dans drawOpenGL ...
    glPushName(0);    // idem

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    if (type!=-1) {
        switch(type_new) {
        case 0:                 // Spécifique fichier .dxf. Serait à supprimer une fois le décodage réalisé ... mais reste à faire !
            //printf("\ndraw dxf!!!\n");
            m_renderer.Render();
            break;
        case 1:
            //printf("\ndraw a .bdd file !!!\n");
            if (mode == standard) { // || mode == points) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width, width, viewport);
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
//                    glInitNames();    // C'est ici en version Tcl ! mais ne change rien !!
//                    glPushName(0);
                this->drawOpenGL();
            }
            if (MPanel->Bool_souder || mode == points) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width_point, width_point, viewport); // cf version tcl ligne 6554
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
//                buildAllPoints();             // Ne suffisait pas ici ! Il y manquait le glInitNames ?
                glCallList(glliste_points);
//                m_gllist = glliste_points;//0;
//                glDeleteLists(m_gllist,1);
//                glNewList(m_gllist, GL_COMPILE_AND_EXECUTE);
//                glInitNames();
//                showAllPoints();
//                glEndList();
                face_under_mouse = -1;
                if ((MPanel->Bool_souder) && (point_under_mouse != -1)) {
                    ToSelect.ListeSelect.clear();
                    m_gllist = 0;   // Refaire toutes les listes car les facettes, les arêtes ont changé (les points sont à jour : on pourrait éviter de les reconstruire !)
                } else {
                    m_gllist = glliste_objets;
                }
                Refresh();
            }
            if (MPanel->Bool_diviser) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width_ligne, width_ligne, viewport); // cf version tcl ligne 6552
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
                glCallList(glliste_lines);
//                m_gllist = glliste_lines;
////                printf("liste : %d\n",m_gllist);
//                glDeleteLists(m_gllist,1);                    <=> buildAllLines, mais superflu. glCallList est suffisant et bien plus rapide
//                glNewList(m_gllist, GL_COMPILE_AND_EXECUTE);
//                glInitNames();
//                showAllLines();
//                glEndList();
                line_under_mouse = -1;  // Raz du numéro d'arête survolé par la souris
                objet_under_mouse= -1;
                Refresh();
            }
            //glTranslatef( m_gldata.posx, m_gldata.posy,m_gldata.posz );
            break;
        default:
            //printf("\ndraw something else ? !!!\n");
            break;
        }
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
    hits = glRenderMode(GL_RENDER); //if(test_rectangle_selection) printf("hits = %d\n",hits);
    if (hits != 0) {
//        printf("hits2= %d\n",hits);
        processHits(hits, OnOff);//, selectBuffer);
    }
    ResetProjectionMode();
}

void BddInter::stopPicking() {
    ResetProjectionMode();
}

void BddInter::processHits(GLint hits, bool OnOff) {

// Appel dans testPicking
// Fonction très proche de letscheckthemouse. A t-on besoin des 2 ?

    int i, ii, offset;
    int objet, face, line, point;
    unsigned int indice_pt, indice_vec;
    GLuint z;
//    float zz;
    GLuint *ptr=nullptr;

    Object *objet_courant;
    Face   *new_facette;

    bool test_print = false ;       // Mettre true pour activer des impressions de test et commenter la ligne en dessous (ou activer verbose via lettre v)!
    test_print = verbose;
//    test_print = true;

    if (verbose) printf("Entree de processHits\n");

    if (hits >= BUFSIZE || hits < 0) {
        if (hits > 0) { // Si valeur négative, ne rien afficher car ce n'est pas toujours une erreur ! Peut-être pb de timing
            std::cout<<"trop de points cliques ou valeur negative :  ";
            std::cout<< hits;
            std::cout<<" \n";
        }
    } else {
        /*!on initialise le pointeur*/
        ptr = (GLuint *) selectBuffer;

        if (style == GL_POLYGON || style == GL_POINTS) offset= 5; else offset= 6 ;

        if (test_print)
            printf("\nhits en entree de processHits : %d\n",hits);
// Test
        if (test_print)
            printf("style : %d (POINTS:%d LINE_LOOP:%d POLYGON:%d)\n",style,GL_POINTS,GL_LINE_LOOP,GL_POLYGON);
        GLuint val0;
        val0 = *ptr;

        if (val0 < 2) return;   /// pourquoi ? si 0, c'est le fond, si 1 il y a un pb => offset varie => décalages dans les ptr de 1 ou 2 => mieux vaut ignorer

        int incrHits = 0;
        if (val0 != 0) incrHits = 1;    // Si val0 == 0, c'est qu'on est sur le fond <=> hits = 0 !
        int newHits  = 0;

        for (i = 0; i < hits; i++) {
            if (*ptr == val0)
                newHits += incrHits;    // On incrémente tant que la première valeur est égale = val0, sinon on s'arrête (<=>modif de hits)
            else
                incrHits=0;
            if (test_print) {
                printf("0 : %u;\n", ptr[0]);
                printf("1 : %u; z\n", ptr[1]);
                printf("2 : %u; z2\n",  ptr[2]);
                printf("3 : %u; objet\n", ptr[3]);
                if (style == GL_POINTS)
                    printf("4 : %u; point\n",   ptr[4]);
                else
                    printf("4 : %u; facette\n",  ptr[4]);
//                if (style == GL_POINTS)     printf("5 : %d; ID\n",   ptr[5]);
                if (style == GL_LINE_LOOP)  printf("5 : %d; line\n", ptr[5]);
            }
            ptr += offset;
        }

        ptr = (GLuint *) selectBuffer;
// Fin du test
        z = ptr[1];
        if (test_print) printf("z : %u; ", z);
        ii = 0;
        if(z > ptr[2]) {
            z=ptr[2];
            if (test_print) printf("z2: %u; ", z);
        }

//        hits--; // Pourquoi ? Pour ne pas tenir compte d'un hit sur le fond ?
        if (test_print) printf("newHits %d\n",newHits);

        for (i = 1; i < newHits; i++) {                 // Commencer à 1, car 0 déjà fait !
            /*!on teste quel point picke est le plus proche*/
//            ptr++;
            ptr += offset;                              // Pour i=0, déjà fait, commencer à i =1 après avoir mis à jour ptr
            if (z >= ptr[1]) {                          // Modif GD > remplacé par >= : à tester sur 2 objets ayant un point en commun
                z=ptr[1];
                ii=i;
                if (test_print) printf("\nz : %u; ", z);
            }
            if (z >= ptr[2]) {                          // Idem
                z=ptr[2];
                ii=i;
                if (test_print) printf("\nz2: %u; ", z);
            }
//            ptr += offset;

        }

        ptr = (GLuint *) selectBuffer;/*!on reinitialise le pointeur*/

        int indiceptr_objet = (ii*offset)+3;
        objet = ptr[indiceptr_objet];

        if (style == GL_POINTS) {
            point = ptr[indiceptr_objet+1];
            int ID= ptr[indiceptr_objet+2];
            if (test_print) {
                printf("Point appartenant a l'objet ");
                printf("%d ", objet);

                printf("et d'indice ");
                printf("%d ", point);

                printf("et ayant pour id ");
                printf("%d\n", ID);
            }
            MSelect->TextCtrl_NomObjet->ChangeValue(wxString(this->Objetlist[objet].GetName(), wxConvUTF8));    // si fichier bdd en utf8
//            MSelect->TextCtrl_NomObjet->ChangeValue(this->Objetlist[objet].GetwxName());
            str.Printf(_T("%d"),this->Objetlist[objet].GetValue());
            MSelect->TextCtrl_NumObjet->SetValue(str);
            str.Printf(_T("%d"),objet);
            MSelect->TextCtrl_IndObjet->SetValue(str);
            str.Printf(_T("%d"),point);
            MSelect->TextCtrl_IndFacette->SetValue(str); // Case renommée en numéro de point dans ce cas
            str.Printf(_T("%d"),point+1);
            MSelect->TextCtrl_NumFacette->SetValue(str);
            if (ToSelect.check_if_in_ListeSelect(objet,point)) {
//                printf("point deja present => l'enlever en mode OnOff actif\n");
                if (creation_facette && (ToSelect.ListeSelect.size() > 1)) { // Ne pas passer ici si 1 seul point dans ListeSelect (c'est alors 2 x le 1er)
                    if ((ToSelect.ListeSelect[0].objet == objet) && (ToSelect.ListeSelect[0].face_sommet == point)) {
                        printf("Bouclage sur le premier point\n");
                        unsigned int new_size, new_indice;
                        bool UnSeulObjet=true;
                        for(i=1; i<(int)ToSelect.ListeSelect.size(); i++) {
                            if (ToSelect.ListeSelect[0].objet != ToSelect.ListeSelect[i].objet) { // Si objets différents, créer de nouveaux sommets dans le 1er objet listé
//                                UnSeulObjet = false;                                                              // N'a plus lieu d'être
                                indiceObjet_courant = ToSelect.ListeSelect[i].objet;
                                objet_courant = &(this->Objetlist[indiceObjet_courant]);                            // Objet [i] différent de l'objet [0]
                                Sommet NewSommet = objet_courant->Sommetlist[ToSelect.ListeSelect[i].face_sommet];  // Récupère le sommet du deuxième objet
                                objet_courant->Sommetlist[ToSelect.ListeSelect[i].face_sommet].selected = false;    // Marque l'ancien sommet comme non sélectionné
                                indiceObjet_courant = ToSelect.ListeSelect[0].objet;                                // Retour à l'objet [0]
                                objet_courant = &(this->Objetlist[indiceObjet_courant]);
                                NewSommet.Numero = objet_courant->Sommetlist.size() +1;                             // Metrre à jour le nouveau numéro du sommet à créer
                                objet_courant->Sommetlist.push_back(NewSommet);                                     // Ajout de ce nouveau sommet à l'objet [0]
                                objet_courant->Nb_sommets++;                                                        // Mise à jour du nombre de sommets dans l'objet [0]
                                ToSelect.ListeSelect[i].objet       = ToSelect.ListeSelect[0].objet;                // Mise à jour dans ListeSelect de objet et face_sommet
                                ToSelect.ListeSelect[i].face_sommet = NewSommet.Numero -1;
                            }
                        }
                        indiceObjet_courant = objet;
                        objet_courant = &(this->Objetlist[objet]);
                        new_size = new_indice = objet_courant->Facelist.size();
                        if (objet_courant->Nb_facettes_original == 0) objet_courant->Nb_facettes_original = new_size; // Sauvegarde initiale du nombre de facettes
                        new_size++;
                        N_elements = new_size;
                        str.clear();
                        NumerosSommets.resize(ToSelect.ListeSelect.size());
                        for(i=0; i<(int)ToSelect.ListeSelect.size();i++) NumerosSommets[i] = ToSelect.ListeSelect[i].face_sommet +1; // Numéros décalés d'1 cran / indices
                        make1face();
                        objet_courant->Nb_facettes = new_size;
                        Calcul_Normale_Barycentre(objet,new_indice);                // Ici la facette créée est plane
                        objet_courant->Facelist[new_indice].selected = true;
                        GenereTableauPointsFacettes(objet_courant);
                        GenereTableauAretes(objet_courant);
                        new_facette    = &(objet_courant->Facelist[new_indice]);
                        new_facette->groupe     = MPanel->NumeroGroupe;             // OK mais l'effet n'est pas immédiat si affichage coloré (via bouton groupes ou matériaux)
                        new_facette->codmatface = MPanel->NumeroMateriau;
                        NumerosSommets = new_facette->F_sommets;
                        new_facette->L_sommets    = NumerosSommets;                 // Recopie provisoire des numéros de sommets de facettes dans les numéros de vecteurs pour les lumminances
                        new_facette->Nb_Sommets_F = new_facette->F_sommets.size();  // Ajustement des tailles des vecteurs = sommets
                        new_facette->Nb_Sommets_L = new_facette->L_sommets.size();  //                                       et luminances
                        // Augmenter le nombre de vecteurs car les nouvelles normales aux sommets ne remplacent pas forcément les anciennes, notamment
                        // quand une normale spécifique est utilisée par 2 sommets ou plus. Peut arriver en cas de simplification de Bdd !
                        indice_vec = this->Objetlist[objet].Vecteurlist.size();     // Initialise indice_vec avec la taille actuelle des vecteurs
                        new_size   = indice_vec + new_facette->Nb_Sommets_L;        // Nouvelle taille
                        this->Objetlist[objet].Vecteurlist.resize(new_size);        // Resize dzs vecteurs
                        this->Objetlist[objet].Nb_vecteurs = new_size;              // et maj de Nb_vecteurs
                        for(i=0; i<(int)NumerosSommets.size();i++) {
//                               printf("indice %d, Numero de sommet : %d\n",i,NumerosSommets[i]);
                            indice_pt = NumerosSommets[i]-1;
                            GenereNormale_1_Sommet(objet_courant,indice_pt,indice_vec);
                            indice_vec++;
                        }
                        objet_courant->Facelist[new_indice].flat = MPanel->FacetteCreeePlane;   // dépend de la case à cocher "Plane" du dialogue "ModificationPanel
                        ToSelect.verrouiller_ListeSelect(false);
                        MPanel->Button_SupprimerFacette->Enable();
                        MPanel->Button_InverserNormale ->Enable();
                        SELECTION old  = mode_selection;
                        mode_selection = selection_point;
                        wxKeyEvent key_event;
                        key_event.m_keyCode = 'S';
                        OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection de points
                        mode_selection = old;

                        m_gllist = 0;
                        Refresh();

                    } else if (OnOff) {
                        ToSelect.erase_one_ListeSelect(objet,point);    // Si le point est déjà dans la liste et que ce n'est pas le 1er, le supprimer
                        Objetlist[objet].Sommetlist[point].selected = false;
                    }
                } else if (OnOff) {
                    ToSelect.erase_one_ListeSelect(objet,point);        // Si OnOff est true et si le point est déjà dans la liste, le supprimer
                    Objetlist[objet].Sommetlist[point].selected = false;
                }
            } else {
//                printf("point absent       => l'ajouter\n");
                ToSelect.add_one(objet,point);                      // N'ajouter que si non déjà présent dans la Liste
                Objetlist[objet].Sommetlist[point].selected = true;
            }
            str.Printf(_T("%d"),(int)ToSelect.ListeSelect.size()); // + (int)ToSelect.Liste.size());
            MSelect->TextCtrl_Selection->SetValue(str);

            if (MPanel->Bool_souder) {
                if(Smemory == nullptr) {
                    // Enregistrer le premier point sélectionné dans Smemory
                    if (test_print) printf("Memoire 1 : objet %d, point %d\n", objet, point);
                    Smemory = new Sommetmemory(objet, point);
                } else {
                    // Réaliser la soudure du second point sélectionné avec celui enregistré précédemment
                    if (test_print) printf("avec n. 2 : objet %d, point %d\n",objet,point);
                    if(objet != this->Smemory->objet || point != this->Smemory->sommet) {
                        souderPoints(objet, point);
                        Objetlist[objet].Sommetlist[point].selected = false;    // Sinon le second point sélectionné reste coloré en vert
                    }
                    delete this->Smemory;       // Utile ??
                    this->Smemory  = nullptr;   // Reset
                    ToSelect.ListeSelect.clear();
                }
            }
        }

        if (style == GL_LINE_LOOP) {
            face = ptr[indiceptr_objet+1];
            line = ptr[indiceptr_objet+2];
            if (test_print) {
                printf("Fil appartenant l'objet ");
                printf("%d ", objet);

                printf(" et a la Facette d'indice ");
                printf("%d ", face);                    // Ne semble pas gérer le fait qu'une arête (un fil) puisse appartenir à 2 facettes

                printf(" et a la Ligne ");
                printf("%d ", line);
            }

            diviserArete(objet, face, line);
        }

        if (style == GL_POLYGON) {
            face = ptr[indiceptr_objet+1];
            if (hits == 1) {
                objet = face = -1;  // C'est un clic sur le Fond
            }
            if (test_print) {
                printf("Facette appartenant a l'objet ");
                printf("%d ", objet);

                printf("et d'indice ");
                printf("%d ", face);
            }

            if (mode_selection == selection_facette) {
                if (objet != -1) {  // Ne rien faire si le clic sur l'objet est en fait sur le fond
                if (OnOff) {
                    if (!ToSelect.check_if_in_ListeSelect(objet,face)) {
                            ToSelect.add_one(objet,face);                       // N'ajouter que si non déjà présent dans la Liste
                            colorface(objet, face, true);
                    } else {
                        ToSelect.erase_one_ListeSelect(objet,face);         // Sinon, le supprimer
                            colorface(objet, face, false);
                    }
                } else {
                    if (!ToSelect.check_if_in_ListeSelect(objet,face)) {    // N'ajouter que si non déjà présent dans la Liste
                            ToSelect.add_one(objet,face);
                            colorface(objet, face, true);
                        }
                    }
                }
            }

            if (mode_selection == selection_objet) {
//                if (objet == -1) return;      /// Pour l'instant c'est mieux que de passer par l'affichage de "Fond" et le comptabiliser comme un objet
                SelectionObjet = -1 ; //objet+1; //this->Objetlist[objet].GetValue();   // Valeur < 0 pour drawOpenGL, la sélection se faisant via l'attribut selected de Objetlist[objet]
                if (objet >= 0) {                   // Pour ne comptabiliser que des clics sur un objet, pas sur le Fond
    //                printf("Objet : %d %d %d\n",objet,SelectionObjet,mode_selection);
                    auto it = std::find(listeObjets.begin(),listeObjets.end(),objet);       // Est-il déjà dans la liste ?
                    if (it == listeObjets.end() || listeObjets.empty()) {                   // Non
                        listeObjets.push_front(objet);                                          // L'ajouter à la liste des objets
                        if (objet >= 0) this->Objetlist[objet].selected = true;                     // Le marquer comme sélectionné
                    } else {                                                                // Oui
                        listeObjets.erase(it);                                                  // Le supprimer de la liste des objets
                        if (objet >= 0) this->Objetlist[objet].selected = false;                    // Le marquer comme non sélectionné
                    }
                }
            }

            if (MSelect->IsShown()) { // On peut le faire même si MSelect n'est pas affiché => est OK dès l'affichage ultérieur
                str.clear();
                if (objet == -1) {
                    MSelect->TextCtrl_NomObjet->ChangeValue(_T("Fond"));
                } else {
                    MSelect->TextCtrl_NomObjet->ChangeValue(wxString(this->Objetlist[objet].GetName(), wxConvUTF8));    // si fichier bdd en utf8
//                    MSelect->TextCtrl_NomObjet->ChangeValue(this->Objetlist[objet].GetwxName());
                }
                if (objet != -1) {
                    str.Printf(_T("%d"),this->Objetlist[objet].GetValue());     // Numéro de l'objet (!= son indice)
                    MSelect->TextCtrl_NumObjet->SetValue(str);
                } else {
                    MSelect->TextCtrl_NumObjet->SetValue(_T(""));
                }
                str.Printf(_T("%d"),objet);                                 // Indice de l'objet
                MSelect->TextCtrl_IndObjet->SetValue(str);
                str.Printf(_T("%d"),face);
                MSelect->TextCtrl_IndFacette->SetValue(str);
                str.Printf(_T("%d"),face+1);
                MSelect->TextCtrl_NumFacette->SetValue(str);
                if (mode_selection == selection_objet) {
                    int nb_Objets_selected = (int)listeObjets.size();
                    str.Printf(_T("%d"),nb_Objets_selected) ;
                    if (nb_Objets_selected >= 2) {
                        MSelect->Button_Fusionner->Enable();    // Activer le bouton de fusion d'objets si le nombre d'objets sélectionnés est >= 2
                        MSelect->TextCtrl_NomObjet->ChangeValue(_T("Sélection multiple"));  // Ici ChangeValue plutôt que SetValue sinon active SelectionPanel::OnTextCtrl_NomObjetText
                    } else {
                        MSelect->Button_Fusionner->Disable();   // Le désactiver sinon (rien à fusionner).
                        if ((nb_Objets_selected == 0) && (objet >= 0)) MSelect->TextCtrl_NomObjet->SetValue(""); // Le clic sur l'objet restant l'a déselectionné !
                    }
                    if (nb_Objets_selected == 1) {
                        if (objet == -1) {
                            MSelect->CheckBox_ForcerFlat->SetValue(false);
                        } else {
                            MSelect->CheckBox_ForcerFlat->SetValue(this->Objetlist[objet].flat);
                        }
                    } else {
                        MSelect->CheckBox_ForcerFlat->SetValue(false);  // Ici, si plusieurs objets sélectionnés, on pourrait evt. mettre wxCHK_UNDETERMINED via Set3StateValue
//                        MSelect->CheckBox_ForcerFlat->Set3StateValue(wxCHK_UNDETERMINED);
                    }
                } else {
                    str.Printf(_T("%d"),(int)ToSelect.ListeSelect.size()); // + (int)ToSelect.Liste.size());
                }
                MSelect->TextCtrl_Selection->SetValue(str);
                if (objet == -1) {
                    MSelect->TextCtrl_NumGroupe  ->SetValue(_T(""));
                    MSelect->TextCtrl_NumMateriau->SetValue(_T(""));
                } else {
                    str.Printf(_T("%d"),this->Objetlist[objet].Facelist[face].groupe);
                    MSelect->TextCtrl_NumGroupe->SetValue(str);
                    str.Printf(_T("%d"),this->Objetlist[objet].Facelist[face].codmatface);
                    MSelect->TextCtrl_NumMateriau->SetValue(str);
                }
                int n_val;
                wxString str_grpmat;
                str_grpmat.clear();
                auto it = listeGroupes.begin();;
                if (MSelect->RadioBox_GrpMat->GetSelection() == 0) {
                    // Groupe est sélectionné
                    n_val = listeGroupes.size();
//                    it = listeGroupes.begin();
                    str.Printf(_T("%d"), (int)*it++);
                    str_grpmat += str;
                    for (i=1; i<n_val; i++) {
                        str.Printf(_T(", %d"), *it++);
                        str_grpmat += str;
                    }
                } else {
                    //Matériau est sélectionné
                    n_val = listeMateriaux.size();
                    it = listeMateriaux.begin();
                    str.Printf(_T("%d"), (int)*it++);
                    str_grpmat += str;
                    for (i=1; i<n_val; i++) {
                        str.Printf(_T(", %d"), *it++);
                        str_grpmat += str;
                    }
                }
                MSelect->TextCtrl_NumerosUtilises->SetLabel(str_grpmat);
                if (mode_selection == selection_objet) {
                    m_gllist = 0;
                    Refresh();
                }
            }

            if (MSelObj != nullptr) {       // Devrait toujours être le cas ... mais test par précaution
                if (MSelObj->IsShown()) {   // Normalement a lieu si MSelObj->IsShown()), mais pourrait avoir été clos manuellement
                    unsigned int iobj,indice_ListBox;

// Lignes recopiées et adaptées de SelectionPanel::Reset_ListeObjets

//                    wxString str_loc;                                       // Recréer la liste : ne sert à rien à ce niveau semble t-il !
//                    unsigned int n = MSelObj->CheckListBox1->GetCount();    // Récupère le nombre d'items actuels
//                    for (iobj=0 ; iobj<n ; iobj++)
//                        MSelObj->CheckListBox1->Delete(0);    // Supprime ces "anciens" items

                    indice_ListBox = 0; // Pour découpler l'indice d'un objet de celui de la CheckListBox (en cas d'objets supprimés)
                    for (iobj=0; iobj<Objetlist.size(); iobj++) {
                        if (!Objetlist[iobj].deleted) {
//                            str_loc = wxString(Objetlist[iobj].GetName(), wxConvUTF8);
//                            MSelObj->CheckListBox1->Append(str_loc);                                  // Recrée tous les items (des objets non supprimés)
                            MSelObj->CheckListBox1->Check(indice_ListBox,Objetlist[iobj].selected);     // Coche ceux qui sont déjà sélectionnés
                            indice_ListBox++;
                        }
                    }
                }
            }
        }
        if (test_print) printf("\n");
    }
}

void BddInter::colorface(int objet,int face, bool OnOff) {

/// Ne sert plus à grand chose ... notamment ne colorise plus la facette sélectionnée, car fonction reportée dans la création de liste

    Face *facette_courante;
//    int i;

    if(ifexist_facette(objet,face)) {   // On vérifie que la facette existe bien dans l'objet
        facette_courante = &(Objetlist[objet].Facelist[face]);
//        if(ToSelect.check_if_in_ListeSelect(objet,face)) {
        if(OnOff) {
//            for (i=0; i<2 ; i++) facette_courante->color[i] = vert[i];
            facette_courante->selected = true;
        } else {
//            for (i=0; i<2 ; i++) facette_courante->color[i] = gris[i];
            facette_courante->selected = false;
        }
        m_gllist = glliste_select;  // Pour ne reconstruire que la liste des facettes sélectionnées
        Refresh();
    }
    return;
}

bool BddInter::ifexist_facette(int objet,int face) {
    if((objet < 0) || (face < 0)) return false;
    if (objet < (int)Objetlist.size()) {
        if(face < (int)Objetlist[objet].Facelist.size()) {
            return true;
        }
    }
    return false;
}

bool BddInter::ifexist_sommet(int objet,int sommet) {
    if((objet < 0) || (sommet < 0)) return false;
    if (objet < (int)Objetlist.size()) {
        if(sommet < (int)Objetlist[objet].Sommetlist.size()) {
            return true;
        }
    }
    return false;
}

void BddInter::SaveTo(wxString str, int index) {

// Pour enregistrer sous différents formats, le format .bdd est celui par défaut (tous les formats lus ne sont pas accessibles en écriture)

    if(OK_ToSave) {
        int types=-1;
        wxString local_str = str ;
        local_str.MakeLower();                          // Forcer le nom de fichier à être en minuscules

        if (local_str.EndsWith(_T(".bdd"))) {           // Teste si l'extension est .bdd
            types = 1;
        } else if (local_str.EndsWith(_T(".obj"))) {    // Teste si l'extension est .obj
            types = 2;
        } else if (local_str.EndsWith(_T(".g3d"))) {    // Teste si l'extension est .g3d
            types = 3;
        } else if (local_str.EndsWith(_T(".off"))) {    // Teste si l'extension est .off
            types = 4;
        } else if (local_str.EndsWith(_T(".stl"))) {    // Teste si l'extension est .stl
            types = index+1;                            // Via index on sait si c'est Ascii ou Binaire
        } else if (local_str.EndsWith(_T(".dxf"))) {    // Ne devrait jamais arriver !!!
            types = 0;
        }
        if (types == -1) {
            // Extension absente ou non reconnue => forcer l'ajout de .bdd (pas forcément utile car fait via le dialogue saveFileDialog dans OvniFrame::OnMenu_Enregistrer_Sous
            str += _T(".bdd");
            types = 1;
        }

//        printf("saving type                        %d\n",types);

        if(types > 0) {
            buffer = str.utf8_str();
            sprintf(Message,"Enregistrement de :\n%s\n",buffer.data());
            printf(utf8_To_ibm(Message));
            switch(types) {
            case 0:                         // En réserve, mais ne peut pas se produire ici !
                //m_renderer.Save(stream);
                break;
            case 1:
                SaveBDD(str);
                break;
            case 2:
                SaveOBJ(str);
                break;
            case 3:
                SaveG3D(str);
                break;
            case 4:
                SaveOFF(str);
                break;
            case 5:
                SaveSTL(str, true);     // Appel en mode Acsii
                break;
            case 6:
                SaveSTL(str, false);    // Appel en mode binaire
                break;
            default:
                break;
            }
        } /* else {
            wxMessageBox(_T("Type de fichier non valide!"),_T("Error"));
        }*/

    }
}

void BddInter::SaveBDD(wxString str) {
    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur = 0;
    unsigned int compteur_sommets = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        wxString Msg = _T("Écriture dans le fichier ") + wxNomsFichiers + _T(" impossible !");
        wxMessageBox(Msg,_T("Avertissement"));
        return;
    }

    printf("\nNombre d'objets : %d\n",(int)this->Objetlist.size());

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) continue;                            // Objet sans facettes => l'ignorer

        compteur_sommets = 0;
        NewVecteurs.clear();
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {   // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
///                compteur++;                                 // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
///                objet_courant->Sommetlist[j].Numero = compteur; // Pourquoi ??? car modifie la base, mais important si soudures ?
///            }
        }
        myfile << "<OBJET> ";
        myfile << objet_courant->GetValue();
        myfile << " ";
        myfile << objet_courant->GetName();                           // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//        wxString mystring = this->Objetlist[i].GetwxName();                 // Récupère le nom wxString
//        std::string stlstring = std::string(mystring.mb_str(wxConvUTF8));   // et convertion en UTF8
//        myfile << stlstring;
//        wxCharBuffer buffer = mystring.ToUTF8();                            // Revient au même que les 2 lignes précédentes
//        myfile << buffer.data();                                            // mais écriture plus claire !
        myfile << "\n";
        myfile << "\n";

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        myfile << "\t<FACE>\t";
        myfile << compteur ;
        myfile << "\n";
        myfile << "\n";

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;   // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // => on ne travaille que sur les facettes non supprimées !
            numeros_Sommets = Face_ij->F_sommets; // <=> Face_ij->getF_sommets();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t";
            myfile << std::setw(2) << numeros_Sommets.size();
            for(k=0; k<numeros_Sommets.size(); k++) {
                myfile << "\t";
//                myfile << std::setw(5) << objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero;     // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << std::setw(5) << numeros_Sommets[k];
            }
            myfile << "\n";
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        unsigned int compteur_facettes = compteur;   // A conserver car resert pour les normales, aspect_faces, luminances
//        printf("\n Face end");
        compteur = 0;
        myfile << "\n\t<SOMMET>\t";
        myfile << compteur_sommets;
        myfile << "\n";
        myfile << "\n";
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {      // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                compteur++;
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                myfile << "\t";
                myfile << std::setw(5) << compteur;
//                myfile << std::setw(5) << objet_courant->Sommetlist[j].Numero;
                for(k=0; k<xyz_sommet.size(); k++) {
                    myfile << "\t";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
///            }
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        myfile << "\n\t<NORMALE>\t";
        myfile << std::setw(5) << compteur_facettes;
        myfile << "\n";
        myfile << "\n";
//        for(j=0; j<compteur_facettes; j++) {                  // NON car en cas de soudure, on saute des facettes
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;
            compteur++;
            xyz_sommet = Face_ij->getNormale_b();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            for(k=0; k<xyz_sommet.size(); k++) {
                myfile << "\t";
                myfile << std::fixed << std::setprecision(8)<< std::setw(11) << xyz_sommet[k];
            }
            myfile << "\n";
        }
        compteur = 0;
        myfile << "\n\t<ASPECT_FACE>\t";
        myfile << compteur_facettes ;
        myfile << "\n";
        myfile << "\n";
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue ;
            compteur++;
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t<GROUPE>\t\t";
            myfile << Face_ij->getGroupe();
            myfile << "\n";
            if(Face_ij->getCodmatface() != codmatface_def) {
                myfile << "\t\t\t";
                myfile << "<CODMATFACE>\t";
                myfile << Face_ij->getCodmatface();
                myfile << "\n";
            }
            myfile << "\n"; // Forcer une ligne blanche après GROUPE, CODMATFACE ou les 2 groupés
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        numeros_Sommets.clear();
//        if (test_seuil_gouraud && Enr_Normales_Seuillees) {
            nouvel_indice = objet_courant->Nb_vecteurs +1;
//        }
        unsigned compteur_luminances = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
           if (!objet_courant->Facelist[j].deleted) {
                numeros_Sommets = objet_courant->Facelist[j].getL_sommets();
                compteur_luminances += numeros_Sommets.size();
            }
            // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
            if (compteur_luminances != 0) break;
        }

        if (compteur_luminances == 0) continue ;        // Pas de luminances => objet à facettes planes => pas de vecteurs => passer à l'objet suivant

        myfile << "\n\t<LUMINANCE>\t";
        myfile << compteur_facettes;                    // Le nombre de luminances est égal au nombre de facettes
        myfile << "\n";
        myfile << "\n";
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) {
//                printf("j=%d ",j);  // Bug : si on le met, OK quand souder a été fait, sinon plante (du moins avec g++ 8.1, car semble OK en 9.3 de Msys2 !)
                continue ;
            }
            compteur++;
//            if (compteur == 1) printf("\n");    // Bug : OK si souder et exe normal, mais enlever en mode Debug !!!!!
            numeros_Sommets = Face_ij->getL_sommets();
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t";
            myfile << std::setw(2) << numeros_Sommets.size();
            bool test_composite = (test_seuil_gouraud && Enr_Normales_Seuillees) || (Face_ij->flat) ;
            if (test_composite) {
                NormaleFacette = Face_ij->getNormale_b();
            }
            for(k=0; k<numeros_Sommets.size(); k++) {
                myfile << "\t";
                if (test_composite) {
                    if (Face_ij->flat) {
                        NormaleSommet = NormaleFacette; // Dans ce cas, on remplace la normale au sommet par la normale au barycentre de la facette
                        test_np = true;
                    } else {
                        NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                        test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                    }
                    if (test_np) {
                    // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                        int test   = 0;
                        bool found = false;
                        for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                            test = 0;
                            if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                            if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                            if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                            if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                myfile << std::fixed << std::setw(5) << objet_courant->Nb_vecteurs+ 1 +jj;
                                found = true;
                                break;  // Abandon de l'exploration du for jj
                            }
                        }
                        if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                            myfile << std::fixed << std::setw(5) << nouvel_indice++;
                            New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                            NewVecteurs.push_back(New_vecteur);
                        }
                    } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                        myfile << std::fixed << std::setw(5) << numeros_Sommets[k];
                    }
                } else { // Pas d'enregistrement de normales seuillées.
                    myfile << std::fixed << std::setw(5) << numeros_Sommets[k];
                }
            }
            myfile << "\n";
        }
        myfile.flush(); // Forcer l'écriture sur le fichier

        compteur = 0;
        myfile << "\n\t<VECTEUR>\t";
//        int NbVecteurs = objet_courant->Nb_vecteurs;
        int NbVecteurs = objet_courant->Vecteurlist.size();     // mieux mais soucis en cas de soudures !
//        printf("%d\n",NbVecteurs);
        NbVecteurs += NewVecteurs.size();
//        printf("%d\n",NbVecteurs);
//        fflush(stdout);
        myfile << std::setw(5) << NbVecteurs;
        myfile << "\n";
        myfile << "\n";
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
//            if(objet_courant->Vecteurlist[j].show == true) {   // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                compteur++;
                NormaleSommet = objet_courant->Vecteurlist[j].point;
                myfile << "\t";
                myfile << std::setw(5) << compteur;
                for(k=0; k<3; k++) {
                    myfile << "\t";
                    myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                }
                myfile << "\n";
                myfile.flush(); // Forcer l'écriture sur le fichier
//            }
        }
        for (j=0; j<NewVecteurs.size(); j++) {
            compteur++;
            myfile << "\t";
            myfile << std::setw(5) << compteur;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
            myfile << "\t"; myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
            myfile << "\n";
        }
        myfile << "\n";
        myfile.flush(); // Forcer l'écriture sur le fichier
    }
    myfile << "\n";
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm * timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveOBJ(wxString str) {

// A vérifier ...

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<int>   numeros_Sommets_L;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur   = 0;
    int compteur_o = 0;
    int offset_vertices = 0;
    int offset_normales = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;
    int last_groupe = codmatface_def;
    int current_groupe;

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        wxString Msg = _T("Écriture dans le fichier ") + wxNomsFichiers + _T(" impossible !");
        wxMessageBox(Msg,_T("Avertissement"));
        return;
    }

    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    myfile << "# Fichier Wavefront obj créé par Ovni\n";

    if (mtllib_OK) myfile << "\n" << mtllib_nom ;               // Restitution de la ligne mtllib sauvegardée (entre 2 lignes blanches)

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }
        compteur_o++;

        NewVecteurs.clear();
        unsigned int compteur_sommets = 0;
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
///                compteur++;                                         // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
///                objet_courant->Sommetlist[j].Numero = compteur;     // Pourquoi ??? car modifie la base
///            }
        }
        myfile << "\ng ";
        myfile << objet_courant->GetName();                         // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//        wxString mystring = objet_courant->GetwxName();
//        wxCharBuffer buffer = mystring.ToUTF8();
//        myfile << buffer.data();
        myfile << "\n";

        myfile << "# " << compteur_sommets << " Sommets\n";

        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                myfile << "v";
                for(k=0; k<xyz_sommet.size(); k++) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
///            }
        }

//        compteur = 0;
        numeros_Sommets.clear();
//        if (test_seuil_gouraud && Enr_Normales_Seuillees) {
            nouvel_indice = objet_courant->Nb_vecteurs +1 + offset_normales ;   // Indice de départ pour stocker les nouvelles normales seuillées
//        }
        unsigned compteur_luminances = 0;
        if (objet_courant->Nb_luminances != 0) {
            for(j=0; j<objet_courant->Facelist.size(); j++) {
                Face_ij = &(objet_courant->Facelist[j]);
                if(!(Face_ij->deleted || Face_ij->flat)) {
                    numeros_Sommets = Face_ij->getL_sommets();
                    compteur_luminances += numeros_Sommets.size();
                }
                // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
                if (compteur_luminances != 0) break;
            }
        }
        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(!objet_courant->Facelist[j].deleted) {   // Test original sur .show, mais une facette masquée ne l'est qu'à l'affichage !
                compteur++;                             // Donc compter toutes les facettes non supprimées (.show original != .afficher !)
            }
        }

        myfile << "\n# " << compteur << " Elements\n";

        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            current_groupe = Face_ij->getGroupe();
            if (current_groupe != last_groupe) {
                if ((current_groupe <= 0) || (nb_mat == 0)) {
                    myfile << "usemtl group_";
                    if (current_groupe < 10) myfile << "0";             // Pour forcer un nom comme group_01, ... group_09, group_10,...
                    if (current_groupe <= 0) current_groupe = 0;        // pour éviter un codage comme group_0-123 par exemple et y mettre group_00
                    myfile << current_groupe << "\n";
                    last_groupe = current_groupe;
                } else {
                    myfile << "usemtl ";//group_";
                    myfile << tab_mat[current_groupe -1];               // ne donne pas tout à fait le résultat escompté mais semble OK (?)
                    last_groupe = current_groupe;
                }
            }

            numeros_Sommets = Face_ij->F_sommets;
            myfile << "f";
            if (compteur_luminances != 0) {                         // Il y a des des normales aux sommets
                numeros_Sommets_L = Face_ij->getL_sommets();
                if (test_seuil_gouraud && Enr_Normales_Seuillees) {
                    NormaleFacette = Face_ij->getNormale_b();
                }
            }
            for(k=0; k<numeros_Sommets.size(); k++) {
                myfile << " ";
//                myfile << (objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero + offset_vertices); // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << (numeros_Sommets[k] + offset_vertices);
                if(Face_ij->flat) continue;                         // Facette plane, ne pas enregistrer les normales aux sommets
                if (compteur_luminances != 0) {                     // Il y des des normales aux sommets
                    if (numeros_Sommets_L.size() == 0) continue;    // Si pas de normales aux sommets sur cette facette en particulier, passer à la suivante
                    myfile << "//";                                 // le champ entre les 2 / est réservé aux textures. Non utilisé dans Ovni
                    if (test_seuil_gouraud && Enr_Normales_Seuillees) {
                        NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                        test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                        if (test_np) {
                        // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                            int  test  = 0;
                            bool found = false;
                            for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                                test   = 0;
                                if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                                if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                                if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                                if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                    myfile << objet_courant->Nb_vecteurs + 1 +jj + offset_normales;
                                    found = true;
                                    break;  // Abandon de l'exploration du for jj
                                }
                            }
                            if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                                myfile << nouvel_indice++;
                                New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                                NewVecteurs.push_back(New_vecteur);
                            }
                        } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                            myfile << numeros_Sommets_L[k] + offset_normales;
                        }
                    } else { // Pas d'enregistrement de normales seuillées.
                        myfile << numeros_Sommets_L[k] + offset_normales;
                    }
                }
            }
            myfile << "\n";
        }

        int NbVecteurs = objet_courant->Vecteurlist.size();         // mieux mais soucis possibles en cas de soudures !
        NbVecteurs += NewVecteurs.size();

        if (NbVecteurs != 0) {                                      // Enregistrer les normales aux sommets
            myfile << "\n# " << NbVecteurs << " Normales aux sommets\n";

            for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
                    NormaleSommet = objet_courant->Vecteurlist[j].point;
                    myfile << "vn";
                    for(k=0; k<3; k++) {
                        myfile << " ";
                        myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                    }
                    myfile << "\n";
    //            }
            }
            for (j=0; j<NewVecteurs.size(); j++) {
                myfile << "vn";
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
                myfile << " " << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
                myfile << "\n";
            }
            myfile << "\n";
        }
        offset_vertices += compteur_sommets;
        offset_normales += NbVecteurs;

    }
    if ((int)this->Objetlist.size() != compteur_o)
        printf("Nombre d'objets retenus  : %d\n",compteur_o);

    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm * timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveOFF(wxString str) {

// A vérifier ...

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    int compteur = 0;
    int total_vertices = 0;
    int total_facettes = 0;
    int offset_vertices= -1;    // Dans ce format, on commence la numérotation des sommets à 0 et non 1 !
    unsigned int compteur_sommets;
    unsigned o,j,k ;
    bool commentaires  = false; // Pour ajouter des commentaires (lignes commençant par #). Mais pose problèmes avec certain logiciels !

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        wxString Msg = _T("Écriture dans le fichier ") + wxNomsFichiers + _T(" impossible !");
        wxMessageBox(Msg,_T("Avertissement"));
        return;
    }
    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    myfile << "OFF\n";
// ATTENTION : certain logiciels (par ex Deep Exploration) n'acceptent pas les lignes de commentaires
    if (commentaires) myfile << "# Fichier Object File Format off créé par Ovni\n";

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }

        compteur_sommets = 0;
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
                compteur_sommets++;
///            }
        }
        total_vertices += compteur_sommets;

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Test original sur .show, mais une facette masquée ne l'est qu'à l'affichage !
            compteur++;                                         // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        total_facettes += compteur;
    }
    myfile << total_vertices << " " << total_facettes << " 0\n";

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        if (commentaires) {
// ATTENTION : certains logiciels (par ex Deep Exploration) n'acceptent pas les lignes de commentaires
            myfile << "# Objet initial : ";
            myfile << objet_courant->GetName();                         // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
//            wxString mystring = objet_courant->GetwxName();
//            wxCharBuffer buffer = mystring.ToUTF8();
//            myfile << buffer.data();
            myfile << "\n";
        }

        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ?
                xyz_sommet = objet_courant->Sommetlist[j].getPoint();
                for(k=0; k<xyz_sommet.size(); k++) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << "\n";
///            }
        }
    }
    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            numeros_Sommets = Face_ij->F_sommets;
            myfile << numeros_Sommets.size();
            for(k=0; k<numeros_Sommets.size(); k++) {
                myfile << " ";
//                myfile << (objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero + offset_vertices); // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << (numeros_Sommets[k] + offset_vertices);
            }
            myfile << "\n";
        }

        compteur_sommets = 0;
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {           // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
                compteur_sommets++;
///            }
        }

        offset_vertices += compteur_sommets;
    }
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm * timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveSTL(wxString str, bool ascii) {

// Construit à partir de SaveOFF
// si ascii est true => mode Ascii, sinon mode binaire (fusion de SaveSTL_Ascii et SaveSTL_Binary)

    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<float> xyz_sommet;

    Face   *Face_ij=nullptr;
    Object *objet_courant;

    UINT32 compteur = 0;
    UINT16 Attribute= 0;
    unsigned int o,i,j,k,len ;

    buffer=str.mb_str();
    std::ofstream myfile;
    if (ascii) {
        myfile.open (buffer.data());
    } else {
        myfile.open(buffer.data(), std::ofstream::binary);
    }
    if (!myfile.is_open()) {
        wxString Msg = _T("Écriture dans le fichier ") + wxNomsFichiers + _T(" impossible !");
        wxMessageBox(Msg,_T("Avertissement"));
        return;
    }

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;
            compteur++;                                         // Compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur == 0) {
            objet_courant->deleted = true;                      // Objet sans facettes => le marquer comme supprimé
            continue;                                           // puis l'ignorer en passant au suivant
        }
    }

    printf("\nNombre d'objets initiaux : %d\n",(int)this->Objetlist.size());

    if (ascii) {
        myfile << "solid " << this->Objetlist[0].GetName() << "\n";
    } else {
        len = strlen(this->Objetlist[0].GetName())    ;
        myfile.write(this->Objetlist[0].GetName(),len);
        for(i=len+1;i<=80;i++) myfile.write(" ",1)    ;
        myfile.write((char *)&compteur,sizeof(UINT32));
    }

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        for(i=0; i<objet_courant->Facelist.size(); i++) {
            Face_ij = &(objet_courant->Facelist[i]);
            if(Face_ij->deleted) continue;
            if (ascii) myfile << "  facet normal";
            xyz_sommet = Face_ij->getNormale_b();
            for (j=0;j<3;j++) {
                if (ascii) {
                    myfile << " ";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[j] ;
                } else {
                    myfile.write((char *)&xyz_sommet[j], sizeof(float)) ;
                }
            }
            if (ascii) myfile << "\n" << "    outer loop\n";
            numeros_Sommets = Face_ij->F_sommets;
            for(j=0; j<3; j++) {    // numeros_Sommets.size() doit être = 3
                xyz_sommet = objet_courant->Sommetlist[numeros_Sommets[j]-1].getPoint();
                if (ascii) myfile << "      vertex";
                for(k=0;k<3;k++) {
                    if (ascii) {
                        myfile << " ";
                        myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                    } else {
                        myfile.write((char *)&xyz_sommet[k],sizeof(float));
                    }
                }
                if (ascii) myfile << "\n";
            }
            if (ascii) {
                myfile << "    endloop\n";
                myfile << "  endfacet\n";
            } else {
                myfile.write((char *)&Attribute,sizeof(UINT16));
            }
        }
    }

    if (ascii) myfile << "endsolid " << this->Objetlist[0].GetName() << "\n";

    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm * timeinfo;
    char buffer_time [10];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data());  // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::SaveG3D(wxString str) {
    // En cours d'écriture ...
    wxCharBuffer  buffer;
    std::vector<int>   numeros_Sommets;
    std::vector<int>   numeros_Sommets_L;
    std::vector<float> xyz_sommet;
    Vector3D           New_vecteur;
    std::vector<Vector3D> NewVecteurs;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;

    Face   *Face_ij=nullptr;
    Sommet *Sommet_ij;
    Object *objet_courant;

    int compteur = 0;
    unsigned o,j,k ;
    int nouvel_indice;
    bool test_np;

// Déclarations pour récupérer l'heure actuelle
    time_t rawtime;
    struct tm * timeinfo;
    char buffer_time [16];

    buffer=str.mb_str();
    std::ofstream myfile;
    myfile.open (buffer.data());
    if (!myfile.is_open()) {
        wxString Msg = _T("Écriture dans le fichier ") + wxNomsFichiers + _T(" impossible !");
        wxMessageBox(Msg,_T("Avertissement"));
        return;
    }

    printf("\nNombre d'objets originaux : %d\n",(int)this->Objetlist.size());

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer_time,16,"%d/%m/%Y",timeinfo);              // date du jour courant (format à la française)

    myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";   // Marquer l'encodage comme UTF-8 plutôt que ISO-8859-1, sinon soucis possibles avec caractères accentués !
    myfile << "<g3d version=\"2.0\">\n";
	myfile << "\t<document version=\"1.0\" date=\"";
	myfile << buffer_time ;
	myfile << "\" auteur=\"ONERA\" commentaire=\"Fichier graphique\">\n";
	myfile << "\t\t<source format=\"CRIRA\">\n";
	myfile << "\t\t\t<entete>\n";
	myfile << "\t\t\t\tFichier g3d créé par Ovni\n";
	myfile << "\t\t\t</entete>\n";
	myfile << "\t\t</source>\n";
	myfile << "\t</document>\n";
	unsigned int nb_mat = listeMateriaux.size();
	if (nb_mat != 0) {
        auto it = listeMateriaux.begin();
        myfile << "\t<types_materiaux nbr=\"1\">\n";
        myfile << "\t\t<type_materiau id=\"1\" nom=\"optique\" nbr=\"";
        myfile << nb_mat << "\">\n";
        for (j=0; j<nb_mat; j++,it++) {
            myfile << "\t\t\t<materiau id=\"" << j+1 << "\" nom=\"materiau_";
            if (*it == codmatface_def) {
                myfile << "def";
            } else {
                if (*it < 10) myfile << "0";
                myfile << *it ;
            }
            myfile << "\"/>\n";
        }
        myfile << "\t\t</type_materiau>\n";
        myfile << "\t</types_materiaux>\n";
	}
	myfile << "\t<types_valeurs nbr=\"2\">\n";
    myfile << "\t\t<type_valeur id=\"1\" nom=\"groupe\"/>\n";
    myfile << "\t\t<type_valeur id=\"2\" nom=\"codmatface\"/>\n";
	myfile << "\t</types_valeurs>\n";

	compteur = 0;
	for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        int compteur_F = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;    // Original de jdias sur .show, mais au final synonyme de !deleted
            compteur_F++;                                       // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }
        if (compteur_F == 0) objet_courant->deleted = true;     // Objet sans facettes => le marquer comme supprimé

	    if (!objet_courant->deleted) compteur++ ;
	}

    if ((int)this->Objetlist.size() != compteur)
        printf("Nombre d'objets retenus   : %d\n",compteur);

    myfile << "\t\t<objets nbr=\"" << compteur << "\">\n";

    for(o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                  // Ne pas enregistrer un objet supprimé, donc passer directement au o suivant

        NewVecteurs.clear();
        unsigned int compteur_sommets = 0;
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
///            if(objet_courant->Sommetlist[j].show == true) {       // A vérifier. Ne serait-ce pas plutôt un test sur ! deleted ? ou même inutile !
///                compteur++;                                     // Non car si soudures c'est .show qui est utilisé (deleted n'existe pas sur sommets !)
                compteur_sommets++;
///                objet_courant->Sommetlist[j].Numero = compteur; // Pourquoi ??? car modifie la base
///            }
        }

        numeros_Sommets_L.clear();
        nouvel_indice = objet_courant->Nb_vecteurs +1;          // Premier indice libre au-dessus de NB_vecteurs pour l'objet courant

        unsigned compteur_luminances = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(!Face_ij->deleted) {
                numeros_Sommets_L = Face_ij->getL_sommets();
                compteur_luminances += numeros_Sommets_L.size();
            }
            // NOTE : on peut arrêter le comptage dès que ce compteur n'est plus nul...
            if (compteur_luminances != 0) break;
        }

        myfile << "\t\t<objet id=\"";
        myfile << objet_courant->GetValue();
        myfile << "\" nom=\"";
//        wxString mystring = objet_courant->GetwxName();                         // Récupère le nom wxString
//        std::string stlstring = std::string(mystring.mb_str(wxConvUTF8));   // et convertion en UTF8
//        myfile << stlstring;
//        wxCharBuffer buffer = mystring.ToUTF8();                                // Revient au même que les 2 lignes précédentes
//        myfile << buffer.data();                                                // mais écriture plus claire !
        myfile << objet_courant->GetName();                                   // Problèmes possibles avec les caractères accentués non Ascii ou UTF8
        myfile << "\" unite=\"\">\n";
        myfile << "\t\t\t<maillages nbr=\"1\">\n";
        myfile << "\t\t\t\t<maillage lod=\"1\">\n";

///        compteur = 0;
        myfile << "\t\t\t\t\t<sommets nbr=\"";
        myfile << compteur_sommets;
        myfile << "\">\n";
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            Sommet_ij = &(objet_courant->Sommetlist[j]);
///            if(objet_courant->Sommetlist[j].show == true) {         // A vérifier.
//                compteur++;
                xyz_sommet = Sommet_ij->getPoint();
                myfile << "\t\t\t\t\t\t<sommet id=\"";
                myfile << (j+1) ; //Sommet_ij->Numero; // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << "\" xyz=\"";
                for(k=0; k<xyz_sommet.size(); k++) {
                    myfile << "\t";
                    myfile << std::scientific << std::setprecision(6) << std::setw(14) << xyz_sommet[k];
                }
                myfile << " \"/>\n";
///            }
        }
        myfile << "\t\t\t\t\t</sommets>\n";
        myfile.flush();

        compteur=0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            if(objet_courant->Facelist[j].deleted) continue;        // Test original sur .show
            compteur++;                                             // Donc compter toutes les facettes non supprimées (.show != .afficher !)
        }

        myfile << "\t\t\t\t\t<facettes nbr=\"";
        myfile << compteur ;
        myfile << "\">\n";

        compteur = 0;
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            Face_ij = &(objet_courant->Facelist[j]);
            if(Face_ij->deleted) continue;
            compteur++;
            myfile << "\t\t\t\t\t\t<facette id=\"";
            myfile << compteur;
            myfile << "\">\n";

            xyz_sommet = Face_ij->getNormale_b();
            myfile << "\t\t\t\t\t\t\t<normale_b xyz=\"";
            for(k=0; k<xyz_sommet.size(); k++) {
                myfile << std::fixed << std::setprecision(8)<< std::setw(11) << xyz_sommet[k];
                myfile << " ";
            }
            myfile << "\"/>\n";
            myfile.flush();

            myfile << "\t\t\t\t\t\t\t<sommets ref=\"";
            numeros_Sommets = Face_ij->F_sommets;
            for(k=0; k<numeros_Sommets.size(); k++) {
//                myfile << objet_courant->Sommetlist[numeros_Sommets[k]-1].Numero;     // Ne marche pas sur fichier .obj optimisé à la lecture
                myfile << numeros_Sommets[k];
                myfile << " ";
            }
            myfile << "\"/>\n";
            myfile.flush();

            numeros_Sommets_L=Face_ij->getL_sommets();
            if ((numeros_Sommets_L.size() != 0) || !Face_ij->flat) {
                bool test_composite = (test_seuil_gouraud && Enr_Normales_Seuillees) || (Face_ij->flat) ;
                if (test_composite) {
                    NormaleFacette = Face_ij->getNormale_b();
                }
                myfile << "\t\t\t\t\t\t\t<normales_s ref=\"";
                for(k=0; k<numeros_Sommets_L.size(); k++) {
                    if (test_composite) {
                        if (Face_ij->flat) {
                            NormaleSommet = NormaleFacette; // Dans ce cas, on remplace la normale au sommet par la normale au barycentre de la facette
                            test_np = true;
                        } else {
                            NormaleSommet = objet_courant->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                            test_np = Calcul_Normale_Seuillee(o,j,k,NormaleFacette,NormaleSommet) ;
                        }
                        if (test_np) {
                        // Vérifier si la nouvelle normale aux sommets n'est pas déjà présente dans NewVecteurs
                            int test = 0;
                            bool found = false;
                            for (unsigned int jj=0; jj < NewVecteurs.size(); jj++) {
                                test = 0;
                                if (NormaleSommet[0] == NewVecteurs[jj].X) test ++;
                                if (NormaleSommet[1] == NewVecteurs[jj].Y) test ++;
                                if (NormaleSommet[2] == NewVecteurs[jj].Z) test ++;
                                if (test == 3) {    // 3 coordonnées identiques => existe déjà => utiliser le numéro de l'existant
                                    myfile << objet_courant->Nb_vecteurs+ 1 +jj;
                                    found = true;
                                    break;  // Abandon de l'exploration du for jj
                                }
                            }
                            if (!found) {   // Utiliser un nouvel indice, initialiser New_vecteur et le ranger dans NewVecteurs pour le moment;
                                myfile << nouvel_indice++;
                                New_vecteur.X = NormaleSommet[0] ; New_vecteur.Y = NormaleSommet[1] ; New_vecteur.Z = NormaleSommet[2] ;
                                NewVecteurs.push_back(New_vecteur);
                            }
                        } else { // Test_np est faux => enregistrer les numéros de sommets existants tels que !
                              myfile << numeros_Sommets_L[k];
                        }
                    } else { // Pas d'enregistrement de normales seuillées.
                          myfile << numeros_Sommets_L[k];
                    }
                    myfile << " ";
                }
                myfile << "\"/>\n";
            }
            myfile.flush();

            if(Face_ij->getCodmatface() != codmatface_def) {
                myfile << "\t\t\t\t\t\t\t<materiaux ref=\"1 ";
                myfile << Face_ij->getCodmatface();
                myfile << "\"/>\n";
            }
            myfile.flush();

            myfile << "\t\t\t\t\t\t\t<valeurs val=\"2 ";
            myfile << Face_ij->getGroupe();
            myfile << ".0\"/>\n";   // Ici, le format attend un flottant

            myfile << "\t\t\t\t\t\t</facette>\n";
            myfile.flush();
        }
        myfile << "\t\t\t\t\t</facettes>\n";
        myfile.flush();

//        unsigned int compteur_facettes = compteur;   // A conserver car resert pour les normales, aspect_faces, luminances

        if (compteur_luminances != 0) {        // Pas de luminances => objet à facettes planes => pas de vecteurs => passer à l'objet suivant

            compteur=0;
            myfile << "\t\t\t\t\t<normales_s nbr=\"";
    //        int NbVecteurs = objet_courant->Nb_vecteurs;
            int NbVecteurs = objet_courant->Vecteurlist.size();     // mieux mais soucis en cas de soudures !
            NbVecteurs += NewVecteurs.size();
            myfile << NbVecteurs;
            myfile << "\">\n";
            for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
    //            if(objet_courant->Vecteurlist[j].show == true) {   // A vérifier.
                    compteur++;
                    NormaleSommet = objet_courant->Vecteurlist[j].point;
                    myfile << "\t\t\t\t\t\t<normale_s id=\"";
                    myfile << compteur;
                    myfile << "\" xyz=\"";
                    for(k=0; k<3; k++) {
                        myfile << std::fixed << std::setprecision(8) << std::setw(11) << NormaleSommet[k];
                        myfile << " ";
                    }
                    myfile << "\"/>\n";
    //            }
            }
            for (j=0; j<NewVecteurs.size(); j++) {
                compteur++;
                myfile << "\t\t\t\t\t\t<normale_s id=\"";
                myfile << compteur;
                myfile << "\" xyz=\"";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].X;
                myfile << " ";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Y;
                myfile << " ";
                myfile << std::fixed << std::setprecision(8) << std::setw(11) << NewVecteurs[j].Z;
                myfile << " \"/>\n";
            }
            myfile << "\t\t\t\t\t</normales_s>\n";
        }
        myfile << "\t\t\t\t</maillage>\n";
		myfile << "\t\t\t</maillages>\n";
		myfile << "\t\t</objet>\n";

    }
    myfile << "\t</objets>\n";
    myfile << "</g3d>\n";
    myfile.close();

    wxString Nom = wxFileNameFromPath(str);
    buffer = Nom.mb_str();

    strftime (buffer_time,10,"%H:%M:%S",timeinfo);                          // Reprendre cette info déjà récupérée en début de SaveG3D
    printf("\n%s : Enregistrement de %s OK !\n",buffer_time,buffer.data()); // on aurait pu faire directement (const char*)Nom.mb_str();
    bdd_modifiee = false ;
}

void BddInter::diviserArete(int objet, int face, int line) {
// Divise un segment en N points équidistants
// Dans "Ajouter sous le pointeur" (MPanel->division vaut alors -1) => calcul différent

    std::vector<int> NumerosSommets;
    unsigned int i,j,k;
    int l;
    GLdouble modelview[16], proj[16];
    GLdouble xawin,yawin,zawin,xbwin,ybwin,zbwin;
    GLdouble vx,vy,abx,aby,t;
    GLint global_viewport[4];  // Serait à globaliser pour éviter d'aller le chercher à chaque fois ...
    int div_limit, Nb_vecteurs_0 ;
    bool vecteurs_presents;

    Face    NewFace;
//    Normale NewNormale;
    std::vector<float> NewNormale;
    Sommet  NewSommet;
    Sommet  Sommet_a;
    Sommet  Sommet_b;
    Object  *objet_cible;

    int indice_objet_cible = objet;
    int indice_face_cible  = face;
    int indice_a, indice_b, numero_a, numero_b;

    printf("objet %d, face %d, line %d\n",objet,face,line);

    Luminance NewLuminance;
    Vecteur   NewVecteur;

    std::vector<int>::iterator it;

    objet_cible = &(this->Objetlist[indice_objet_cible]);

    indice_a = objet_cible->Areteslist[line].ind_a;
    indice_b = objet_cible->Areteslist[line].ind_b;
    numero_a = indice_a +1;
    numero_b = indice_b +1;
    Sommet_a = objet_cible->Sommetlist[indice_a];
    Sommet_b = objet_cible->Sommetlist[indice_b];
    if (MPanel->division == -1) {   // On est dans "Ajouter sous le pointeur"
        // Calcul des positions sur l'écran des 2 sommets
        glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,proj);
        glGetIntegerv(GL_VIEWPORT, global_viewport);

		gluProject(Sommet_a.point[0],Sommet_a.point[1],Sommet_a.point[2],modelview,proj,global_viewport,&xawin,&yawin,&zawin);
		gluProject(Sommet_b.point[0],Sommet_b.point[1],Sommet_b.point[2],modelview,proj,global_viewport,&xbwin,&ybwin,&zbwin);

		vx  = (mouse_position.x-xawin); vy = ((global_viewport[3]-mouse_position.y)-yawin);
		abx = (xbwin-xawin); aby = (ybwin-yawin);

		t = sqrt( (vx*vx)+(vy*vy) ) / sqrt( (abx*abx)+(aby*aby) );
//		printf("valeur de t : %lf\n",t);
		div_limit = 2;
    } else {
        div_limit = this->MPanel->SpinCtrl_NbSegments->GetValue();
        t = 1;  // juste pour éviter un warning !
    }
    objet_cible->Nb_sommets  += div_limit-1;   // div_limit ??? plutôt div_limit -1 (par ex si on divise en 2, on n'ajoute qu'un sommet !)
    Nb_vecteurs_0 = objet_cible->Nb_vecteurs;
    if (Nb_vecteurs_0 > 0) {
        vecteurs_presents = true;
        objet_cible->Nb_vecteurs += div_limit-1;
    } else vecteurs_presents = false;
    for(l=1; l<div_limit; l++) {
        NewSommet = Sommet_a;
        if (MPanel->division != -1) t = (float)l/div_limit;
        for(k=0; k<NewSommet.point.size(); k++) {
            NewSommet.point[k] = Sommet_a.point[k] + (Sommet_b.point[k]-Sommet_a.point[k])*t;
        }
        if (vecteurs_presents) {
            NewVecteur = objet_cible->Vecteurlist[indice_a];
            NewVecteur.Numero = objet_cible->Vecteurlist.size() +1;
            NewVecteur.toshow = (undo_memory+1);
            objet_cible->Vecteurlist.push_back(NewVecteur);
        }
        NewSommet.Numero = objet_cible->Sommetlist.size() +1;
        NewSommet.toshow = (undo_memory+1);
        NewSommet.show   = true;
        objet_cible->Sommetlist.push_back(NewSommet);
        int nouveau_sommet  = objet_cible->Sommetlist.size();
//        int indice_sommet   = nouveau_sommet-1;
//        objet_cible->Sommetlist [indice_sommet].toshow = (undo_memory+1);
//        objet_cible->Sommetlist [indice_sommet].show   = true;
//        if (vecteurs_presents) {
//            objet_cible->Vecteurlist[indice_sommet].toshow = (undo_memory+1);
////            objet_cible->Vecteurlist[indice_sommet].show   = true;
//        }

        unsigned int limit = objet_cible->Facelist.size();
        for(j=0; j<limit; j++) {
            NumerosSommets = objet_cible->Facelist[j].getF_sommets();
            unsigned int k_fin = NumerosSommets.size();
            unsigned int k_suiv;
            for(k=0; k<k_fin; k++) {
                k_suiv = k+1;
                if (k_suiv >= k_fin) k_suiv = 0; // Pour boucler entre le dernier point et le point 0
                if(((NumerosSommets[k] == numero_a && NumerosSommets[k_suiv] == numero_b) ||
                    (NumerosSommets[k] == numero_b && NumerosSommets[k_suiv] == numero_a)) ) {
                    NewFace = objet_cible->Facelist[j];
                    if (objet_cible->Facelist[j].toshow == 0) {
                        objet_cible->Facelist[j].toshow  = ((undo_memory*(-1))-1);
                        objet_cible->Facelist[j].deleted = true;
                    } else {
                        objet_cible->Facelist[j].deleted = true;
                    }
                    it = NewFace.F_sommets.begin()+k+1;
                    NewFace.F_sommets.insert (it, nouveau_sommet);
                    NewFace.Nb_Sommets_F++;
                    if (vecteurs_presents) {
                        it = NewFace.L_sommets.begin()+k+1;
                        NewFace.L_sommets.insert (it, nouveau_sommet);
                        NewFace.Nb_Sommets_L++;
                    }
                    NewFace.toshow = ((undo_memory)+1);
                    NewFace.Numero = objet_cible->Facelist.size() +1;
                    objet_cible->Facelist.push_back(NewFace);
                    objet_cible->Nb_facettes = NewFace.Numero;
                }
            }
            /// entre 1er et dernier point ?
        }
//        }
        numero_a = nouveau_sommet;
    }
    undo_memory++;
    MPanel->Button_Undo->Enable();

    buildAllPoints();
    GenereTableauPointsFacettes(objet_cible);
    GenereTableauAretes(objet_cible);
//    buildAllLines();
    bdd_modifiee = true ;
    Refresh();
}

void BddInter::souderPoints(int objet, int point) {

// A revoir : si dans un même objet, ça marche (mais crée un point de plus inutilement) mais si soudure avec un point d'un autre objet, il n'y a pas de raisons
//            de modifier le second objet.
//            On y récupère seulement les coordonnées du point cible et on créé un nouveau point de même coordonnées dans l'objet original (old)
//            Or les premiers push_back sont faits sur indice_objet_cible ... Pas très logique.
//        ... Peut-être conserver tel quel car gestion du Undo est plus simple ! Pas de distingo interne/externe

//        05/2020 mieux. Faut-il traiter les vecteurs/luminances car très peu de chances qu'ils restent OK après une soudure ! => Recalcul de toutes les normales

    std::vector<int>NumerosSommets;
    unsigned int i;
    int j,k;
    int toUp1=-1;
    int toUp2=-1;
    int nb_Fac_initial;

    Face    NewFace;
    Object *objet_courant, *objet_origine, *objet_cible;

    int indice_objet_cible   = objet;
    int indice_point_cible   = point;
    int numero_point_cible   = point +1;

    // Récupération dans Smemory des numéros d'objet et de point du 1er clic : objet et point d'origine
    int indice_objet_origine = Smemory->objet ;
    int indice_point_origine = Smemory->sommet;
    int numero_point_origine = Smemory->sommet +1;

    objet_cible   = &(this->Objetlist[indice_objet_cible]);
    objet_origine = &(this->Objetlist[indice_objet_origine]);

    Sommet   NewSommet = objet_cible->Sommetlist [indice_point_cible];      // Récupère le point cible dans l'objet cible
    NewSommet.selected = false;
    NewSommet.Numero   = objet_origine->Sommetlist.size() +1;               // Sommetlist pas encore agrandi, donc +1
    objet_origine->Sommetlist.push_back(NewSommet);                         // Ajoute NewSommet en fin de liste origine (même si cible = origine !)

    if (objet_cible->Nb_vecteurs > 0) {
        Vecteur NewVecteur=objet_cible->Vecteurlist[indice_point_cible];    // Suppose que indice_points = indice_vecteurs ? Faux si plusieurs normales sur 1 sommet !
        if (objet_origine->Nb_vecteurs > 0) {
            NewVecteur.Numero = objet_origine->Vecteurlist.size() +1;
            objet_origine->Vecteurlist.push_back(NewVecteur);               // Idem pour NewVecteur, mais il faudra le modifier car non adapté
///            Luminance NewLuminance;                                                            // Non utilisé ? pour l'instant
        }                                                                   // que fait-on si Nb_vecteurs dans cible = 0 mais pas dans origine : il faudrait en créer un ?
    }
//    printf("1\n"); fflush(stdout);

    int nouveau_sommet = objet_origine->Sommetlist.size();

    if (objet_origine->Sommetlist[indice_point_origine].toshow==0) {
        objet_origine->Sommetlist[indice_point_origine].toshow=(-undo_memory-1);
    }

    objet_origine->Sommetlist[indice_point_origine].show = false;

    int new_indice = nouveau_sommet-1;
    objet_origine->Sommetlist[new_indice].toshow = (undo_memory+1);
    objet_origine->Sommetlist[new_indice].show   = true;

    nb_Fac_initial = objet_origine->Facelist.size();

    if (indice_objet_cible == indice_objet_origine) {
        printf("Soudure interne dans un objet\n");
//        fflush(stdout);

        for(j=0; j<nb_Fac_initial; j++) {
            if(objet_origine->Facelist[j].deleted) continue ;               // Passer à la facette suivante
            toUp1 = -1;
            toUp2 = -1;
            NumerosSommets = objet_origine->Facelist[j].getF_sommets();
            for(k=0; k<(int)NumerosSommets.size(); k++) {
                if(NumerosSommets[k] == numero_point_cible) {
                    toUp1 = k;
                    if (verbose) printf("toUp1 = %d\n",toUp1);
                } else if(NumerosSommets[k] == numero_point_origine) {
                    toUp2 = k;
                    if (verbose) printf("toUp2 = %d\n",toUp2);
                }
            }
            if ((toUp1 == -1) && (toUp2 == -1)) continue;                   // Aucun sommet modifié dans cette facette, passer à la suivante
            NewFace = objet_origine->Facelist[j];                           // Recopie de la facette à modifier dans NewFace
            if (objet_origine->Nb_vecteurs == 0) NewFace.Nb_Sommets_L = 0;  // Par précaution
            if (objet_origine->Facelist[j].toshow == 0) {
                objet_origine->Facelist[j].toshow = ((undo_memory*(-1))-1);
            }
            objet_origine->Facelist[j].deleted = true ;                     // GD. Marquer cette ancienne facette comme "deleted"
            NewFace.toshow = (undo_memory+1);
            if((toUp1 != -1) && (toUp2 != -1)) {                            // Soudure interne dans une même facette
                NewFace.setNewSommet_F(toUp1, nouveau_sommet);
                NewFace.setNewSommet_F(toUp2, nouveau_sommet);
                NewFace.F_sommets.erase(NewFace.F_sommets.begin()+toUp2);   // dans ce cas, la facette comportera 1 sommet de moins
                NewFace.Nb_Sommets_F--;                                     // donc on actualise Nb_Sommets_F
                if (NewFace.Nb_Sommets_L > 0) {
                    NewFace.setNewSommet_L(toUp1, nouveau_sommet);
                    NewFace.setNewSommet_L(toUp2, nouveau_sommet);
                    NewFace.L_sommets.erase(NewFace.L_sommets.begin()+toUp2);                   // Idem pour les luminances
                    NewFace.Nb_Sommets_L--;
                }
            } else if(toUp1 != -1) {
                NewFace.setNewSommet_F(toUp1, nouveau_sommet);                                  // Remplacer l'ancien sommet par le nouveau
                if (NewFace.Nb_Sommets_L > 0) NewFace.setNewSommet_L(toUp1, nouveau_sommet);
            } else if(toUp2 != -1) {
                NewFace.setNewSommet_F(toUp2, nouveau_sommet);                                  // Idem
                if (NewFace.Nb_Sommets_L > 0) NewFace.setNewSommet_L(toUp2, nouveau_sommet);
            }
            NewFace.Numero = objet_origine->Facelist.size() +1;                                 // Donner le bon numéro à cette nouvelle facette
            objet_origine->Facelist.push_back(NewFace);                                         // Enfin ajouter cette nouvelle facette

            Calcul_Normale_Barycentre(indice_objet_origine, objet_origine->Facelist.size()-1);  // Calcul de la nouvelle normale au barycentre
        }
    } else {
// Note : peu différent de soudure interne, mais quelques cas n'ont pas lieu d'être ici => On préfère réécrire le code, même si c'est "presque" le même !
        printf("Soudure entre 2 objets\n");
//        fflush(stdout);

        for(j=0; j<nb_Fac_initial; j++) {
        if(objet_origine->Facelist[j].deleted) continue ;                   // Passer à la facette suivante
//            toUp1 = -1;
            toUp2 = -1;
            NumerosSommets = objet_origine->Facelist[j].getF_sommets();
            for(k=0; k<(int)NumerosSommets.size(); k++) {
                if(NumerosSommets[k] == numero_point_origine) {
                    toUp2 = k;
                    if (verbose) printf("toUp2 = %d\n",toUp2);
                }
            }
//            if ((toUp1 == -1) && (toUp2 == -1)) continue;                 // Sommet non modifié, passer à la facette suivante
            if (toUp2 == -1) continue;                                      // Aucun sommet modifié dans cette facette, passer à la suivante
            NewFace = objet_origine->Facelist[j];                           // Recopie de la facette à modifier dans NewFace
            if (objet_origine->Nb_vecteurs == 0) NewFace.Nb_Sommets_L = 0;  // Par précaution
            if (objet_origine->Facelist[j].toshow == 0) {
                objet_origine->Facelist[j].toshow = ((undo_memory*(-1))-1);
            }
            objet_origine->Facelist[j].deleted = true;                      // GD
            NewFace.toshow = (undo_memory+1);

            NewFace.setNewSommet_F(toUp2, nouveau_sommet);
            if (NewFace.Nb_Sommets_L > 0) NewFace.setNewSommet_L(toUp2, nouveau_sommet);

            NewFace.Numero = objet_origine->Facelist.size() +1;                                 // Donner le bon numéro à cette nouvelle facette
            objet_origine->Facelist.push_back(NewFace);                                         // Enfin ajouter cette nouvelle facette

            Calcul_Normale_Barycentre(indice_objet_origine, objet_origine->Facelist.size()-1);  // Calcul de la nouvelle normale au barycentre
        }
    }
    objet_origine->Nb_sommets = objet_origine->Sommetlist.size();       // Mise à jour des valeurs
    objet_origine->Nb_facettes= objet_origine->Facelist.size();
    objet_origine->Nb_vecteurs= objet_origine->Vecteurlist.size();

    if (objet_origine->Temps_Calcul_Aretes < tempo_s*CLOCKS_PER_SEC) {
        GenereTableauPointsFacettes(objet_origine);                     // Faut-il le faire à chaque soudure ou une seule fois en sortie de ModificationPanel
        GenereTableauAretes(objet_origine);                             // Car peut-être long si beaucoup de facettes à cause du test des doublons d'arêtes
        buildAllLines();                                                // Indispensable sinon blocage apparent après 2 soudures
        Refresh();
    } else MPanel->aretes_calculees = false;

    undo_memory++;
    MPanel->Button_Undo->Enable();
    bdd_modifiee = true ;
}

void BddInter::UNDO_ONE() {
// On va réactiver les facettes marquées .deleted, remettre en état les tableaux (vector en réalité) de facettes, sommets, vecteurs
    unsigned int i,j ;
    Object* objet_courant;

    if(undo_memory != 0) {
        for(i=0; i<this->Objetlist.size(); i++) {   /// est-il nécessaire de balayer tous les objets, pas seulement ceux dans undo_memory ?
            objet_courant = &(this->Objetlist[i]);
            for(j=0; j<objet_courant->Facelist.size(); j++) {
                if (objet_courant->Facelist[j].toshow == undo_memory) {
                    objet_courant->Facelist.erase(this->Objetlist[i].Facelist.begin()+j);
                    j--;    // 1 facette supprimée => passer une fois de moins dans la boucle en j
                } else if(objet_courant->Facelist[j].toshow  == (-undo_memory)) {
                    objet_courant->Facelist[j].deleted       = false;
                    objet_courant->Facelist[j].toshow        = 0;
                } else if((objet_courant->Facelist[j].toshow == undo_memory-1) && (undo_memory != 1)) {     // A vérifier : si undo_memory = 1,
                    objet_courant->Facelist[j].deleted       = false;                                       // semble faire le undelete sur 1 facette de trop
                }
            }
            bool vecteurs_presents = true;
            if (objet_courant->Vecteurlist.size() == 0) vecteurs_presents = false;
            for(j=0; j<objet_courant->Sommetlist.size(); j++) {
                if (objet_courant->Sommetlist[j].toshow == undo_memory) {
                    objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin()+j);
                    if (vecteurs_presents) objet_courant->Vecteurlist.erase(objet_courant->Vecteurlist.begin()+j);
                    j--;    // 1 sommet effacé => passer une fois de moins dans la boucle en j
                } else if(objet_courant->Sommetlist[j].toshow == -undo_memory) {
                    objet_courant->Sommetlist[j].show     = true;
                    objet_courant->Sommetlist[j].toshow   = 0;
                    objet_courant->Sommetlist[j].selected = false;
                    if (vecteurs_presents) {
//                        objet_courant->Vecteurlist[j].show   = true;
                        objet_courant->Vecteurlist[j].toshow = 0;
                    }
                } else if((objet_courant->Sommetlist[j].toshow == undo_memory-1) && (undo_memory != 1)) {   // Idem facettes
                    objet_courant->Sommetlist[j].show     = true;
                    objet_courant->Sommetlist[j].selected = false;
//                    if (vecteurs_presents) objet_courant->Vecteurlist[j].show   = true;
                }
            }
            objet_courant->Nb_facettes = objet_courant->Facelist.size();    // Remettre à jour les Nb_*
            objet_courant->Nb_sommets  = objet_courant->Sommetlist.size();
            objet_courant->Nb_vecteurs = objet_courant->Vecteurlist.size();

            if (objet_courant->Temps_Calcul_Aretes <= tempo_s*CLOCKS_PER_SEC) {
                GenereTableauPointsFacettes(objet_courant);                 // Faut-il le faire à chaque soudure ou une seule fois en sortie de ModificationPanel
                GenereTableauAretes(objet_courant);                         // Car peut-être long si beaucoup de facettes à cause du test des doublons d'arêtes
//                buildAllLines();                                            // Indispensable ??? sinon blocage apparent après 2 soudures
//                Refresh();                                                  //  " "
            } else MPanel->aretes_calculees = false;
        }
        undo_memory--;
        if (undo_memory == 0) {
            MPanel->Button_Undo->Disable();
        }

        wxKeyEvent key_event;
        key_event.m_keyCode = 'S';
        OnKeyDown(key_event);   // Simule une pression sur la touche S au clavier => Reset de la sélection de points
        m_gllist = 0;           /// Utile ? => reconstruire toutes les listes !
    }
    Refresh();
}

void BddInter::Calcul_Normale_Barycentre(int i, int j) {

// Calcul d'une normale d'une facette d'indice j dans l'objet d'indice i
// La normale est une "moyenne" si la facette possède plus de 3 points => tolérance aux facettes non planes.

    Face *Face_ij;
    float vector1[3];
    float vector2[3];
    float mag;
    std::vector<float> normalf(3,0.0);
    std::vector< std::vector<float> > normal;
    std::vector<float> normal1;
    int n,z;

    Face_ij = &(this->Objetlist[i].Facelist[j]);

    normal1.clear();
    normal.clear();

    for(n=2; n<(int)Face_ij->F_sommets.size(); n++) {    // n= Face_ij.point.size() nombre de sommets de la facette
        for(z=0; z<3; z++) {
            vector1[z]=
                 this->Objetlist[i].Sommetlist[Face_ij->F_sommets[n-1]-1].point[z]
                -this->Objetlist[i].Sommetlist[Face_ij->F_sommets[0]  -1].point[z];     //! Vecteur P1-P0 si n=2, puis P2-P0 si n=3, ...
            vector2[z]=
                 this->Objetlist[i].Sommetlist[Face_ij->F_sommets[n]-1].point[z]        //! Vecteur P2-P0 si n=2, puis P3-P0 si n=3, ..., Pn-P0 avec n >= 2
                -this->Objetlist[i].Sommetlist[Face_ij->F_sommets[0]-1].point[z];
        }
        normal1.push_back(vector1[1]*vector2[2]-vector1[2]*vector2[1]);             //! Calcul de n-2 sous-normales
        normal1.push_back(vector1[2]*vector2[0]-vector1[0]*vector2[2]);
        normal1.push_back(vector1[0]*vector2[1]-vector1[1]*vector2[0]);
        normal.push_back(normal1);
        normal1.clear();
    }

    // Calcul d'une normale moyenne : Pb: le vecteur Point1-Point0 a un rôle prépondérant.
    // Voir si besoin la fonction correspondante dans la version Tcl.

    for(n=0; n<(int)normal.size(); n++) {
        for(z=0; z<3; z++) normalf[z] += normal[n][z];      // La version originale faisait une moyenne, mais une somme suffit car la normalisation à 1 est faite ci-dessous
    }
    mag = sqrt(normalf[0]*normalf[0] + normalf[1]*normalf[1] + normalf[2]*normalf[2]);
    if (mag != 0.0) for(z=0; z<3; z++) normalf[z] /= mag;   // Normalisation à 1 (si mag différent de 0 !)
    Face_ij->normale_b = normalf;
    bdd_modifiee = true ;
}

bool BddInter::Calcul_Normale_Seuillee(int indice_objet_cur, int ind_fac, int ind_P, std::vector<float> &NormaleFacette, std::vector<float> &NormaleSommet)
{
    bool test_np ;
    int npoint, i, k, k2, nbs, nfac ;
    double  coef ;
    std::vector<float> pa, pb, pc;
    Vector3D v1, v2, vp, np;
    Object *objet_cur;
    Face cur_fac ;
    int  NbFacettes;

    objet_cur = &(this->Objetlist[indice_objet_cur]);
    test_np   = false ;
    np = Vector3D();//    SetCoordonnees(np,0.,0.,0.) ;                             // Initialisation
    if (test_seuil_gouraud) {
        if (!((produit_scalaire(NormaleFacette,NormaleSommet) > seuil_Gouraud) || (seuil_Gouraud <= -0.999 ))) {
            npoint = objet_cur->Facelist[ind_fac].F_sommets[ind_P] -1;              // Ici c'est le numéro d'indice du sommet qu'il faut récupérer
            NbFacettes = objet_cur->Pointslist[npoint].IndicesFacettes.size();      // Nombre de facettes qui utilisent ce sommet
            for (k=0; k< NbFacettes ; k++) {
                nfac    = objet_cur->Pointslist[npoint].IndicesFacettes[k] ;
                /* Ne pas prendre en compte les facettes dont l'angle entre la normale avec celle de
                la facette en cours est trop fort */
                if ((produit_scalaire(objet_cur->Facelist[ind_fac].normale_b, objet_cur->Facelist[nfac].normale_b) > seuil_Gouraud2)  ||
                    (seuil_Gouraud2 <= -0.999) ) {
                    cur_fac = objet_cur->Facelist[nfac];
                    nbs     = cur_fac.Nb_Sommets_F;
                    coef    = 0.;
                    pa      = objet_cur->Sommetlist[cur_fac.F_sommets[0]-1].getPoint() ;
                    pb      = objet_cur->Sommetlist[cur_fac.F_sommets[1]-1].getPoint() ;
                    v1.X    = (double)(pb[0] - pa[0]); v1.Y = (double)(pb[1] - pa[1]); v1.Z = (double)(pb[2] - pa[2]);
                    for (k2=2 ; k2 < nbs ; k2++) {
                        pc = objet_cur->Sommetlist[cur_fac.F_sommets[k2]-1].getPoint() ;
                        v2.X = (double)(pc[0] - pa[0]); v2.Y = (double)(pc[1] - pa[1]); v2.Z = (double)(pc[2] - pa[2]);
//                      coef += Norme(produit_vect(&v1,&v2)) ; // En toute rigueur diviser par 2 !
                        vp = v1.crossProduct(v2);
                        coef += vp.length();  // En toute rigueur diviser par 2 !
                        v1 = v2 ;
                    }

                    np.X += coef*cur_fac.normale_b[0] ;
                    np.Y += coef*cur_fac.normale_b[1] ;
                    np.Z += coef*cur_fac.normale_b[2] ;
                    test_np = true ;
                }
                if (np.length() == 0) test_np = false;  // Si c'est le cas, c'est plutôt étrange ! Par précaution ...
            }
            if (test_np) {
                np.normalize();
                NormaleSommet = {(float)np.X, (float)np.Y, (float)np.Z} ;
            } else { /* Si test_np est faux, normalement ne devrait pas arriver, mais .... on ne garde alors que la normale à la facette <=> ancienne méthode */
                NormaleSommet = NormaleFacette;
                test_np = true ; // NormaleSommet est remplacé par NormaleFacette, et donc modifié : retourner true également dans ce cas !
            }
        }
//            }
    }
    return test_np; // En sortie, NormaleSommet est soit inchangé, soit recalculé, éventuellement remplacé par NormaleFacette
}

bool BddInter::letscheckthemouse(int mode_appel, int hits) {

// Appel dans OnMouse, tests sur MPanel->Bool_souder et MPanel->Bool_diviser
// Fonction très proche de processHits. A t-on besoin des 2 ?
// mode_appel=0 si appel dans le test souderPoints
// mode_appel=1 si appel dans le test diviserArete

    int i, ii, offset;
    int objet, face, point;
    GLuint z;
    GLuint *ptr=nullptr;

    bool test_print = false;    // Mettre true pour activer des impressions de test
    test_print = verbose;
//    test_print = true;

    if (verbose) printf("Entree de letscheckthemouse\n");

    if (hits >= BUFSIZE || hits < 0) {
        if (hits > 0) {// Si valeur négative, ne rien afficher car ce n'est pas toujours une erreur ! Peut-être pb de timing
            std::cout<<"trop de points cliques ou valeur negative :  ";
            std::cout<< hits;
            std::cout<<" \n";
        }
        objet_under_mouse = -1;
        face_under_mouse  = -1;
        point_under_mouse = -1;
        line_under_mouse  = -1;
        return false;
    } else {
        /*!on initialise le pointeur*/
        ptr = (GLuint *) selectBuffer;

        GLuint val0 = *ptr;
        if (val0 < 2) {
            objet_under_mouse = -1;
            face_under_mouse  = -1;
            point_under_mouse = -1;
            line_under_mouse  = -1;
            return false;     /// pourquoi ? si 0, c'est le fond, si 1 il y a un pb => offset varie => décalages dans les ptr de 1 ou 2 => mieux vaut ignorer
        }

        offset = 6; // ici on est soit en GL_POINTS, soit en GL_LINE*

        if (test_print) {
            printf("\nhits dans letscheckthemouse : %d\n",hits);
//// Test
            for (i = 0; i < hits; i++) {
                printf("0 : %u;\n", ptr[0]);
                printf("1 : %u; z\n", ptr[1]);
                printf("2 : %u; z2\n",  ptr[2]);
                printf("3 : %u; objet\n", ptr[3]);
                if (mode_appel == 0)
                    printf("4 : %u; point\n", ptr[4]);
                else
                    printf("4 : %u; facette\n", ptr[4]);
                printf("5 : %d; ID ou line\n", ptr[5]);
                ptr += offset;
            }
            ptr = (GLuint *) selectBuffer;
//// Fin du test
        }

        z=ptr[1];
        if (test_print) printf("z : %u;\n",  z);
        ii=0;
        if(z > ptr[2]) {
            z=ptr[2];
            if (test_print) printf("z2: %u; ", z);
        }

        for (i = 1; i < hits; i++) {                    // Commencer à 1, car 0 déjà fait !
            /*!on teste quel point picke est le plus proche*/
//            printf("i=%d %u\n",i, *ptr);
//            ptr++;
            ptr += offset;                              // Pour i=0, déjà fait, commencer à i =1 après avoir mis à jour ptr
            if(z >= ptr[1]) {                           // Modif GD > remplacé par >= : à tester sur 2 objets ayant un point en commun
                z=ptr[1];
                ii=i;
                if (test_print) printf("\nz : %u; ", z);
            }
            if(z >= ptr[2]) {                           // Idem
                z=ptr[2];
                ii=i;
                if (test_print) printf("\nz2: %u; ", z);
            }
//            ptr += offset;
        }
//        printf("ii=%d\n",ii);

        ptr = (GLuint *) selectBuffer;  /*!on reinitialise le pointeur*/
        int indiceptr_objet = (ii*offset)+3; // mode points ou line => 6 valeurs par Hit
//        printf("indiceptr_objet : %d\n",indiceptr_objet);
//        printf("valeur objet    : %d (%d)\n",ptr[indiceptr_objet],ptr[indiceptr_objet]-1);
//        if (mode_appel == 0)
//            printf("valeur facette  : %d (%d)\n",ptr[indiceptr_objet+1],ptr[indiceptr_objet+1]-1);
//        if (mode_appel == 1)
//            printf("valeur point    : %d (%d)\n",ptr[indiceptr_objet+1],ptr[indiceptr_objet+1]-1);
//        printf("valeur ID       : %d\n",ptr[indiceptr_objet+2]);

        objet_under_mouse = ptr[indiceptr_objet];
        if (mode_appel == 0) {
            point_under_mouse = ptr[indiceptr_objet+1];
            face_under_mouse  = -1; // par précaution : ici c'est un point qui est sélectionné, pas une facette
        }

        if (mode_appel == 1) {
            face_under_mouse  = ptr[indiceptr_objet+1];
            line_under_mouse  = ptr[indiceptr_objet+2];
        }
        // NOTE : mode_appel doit être à 0 ou à 1. Toute autre valeur est a priori une erreur d'appel !
    }
    return true;
}

void BddInter::Calcul_All_Normales() {
    unsigned int o,i,nb_fac,nb_som;
    Object * objet_courant;

    printf("\nCalcul de toutes les normales\n");

    for(unsigned int o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        objet_courant->flat = false;
//        nb_fac = objet_courant->Facelist.size();
        nb_fac = objet_courant->Nb_facettes;
        nb_som = objet_courant->Nb_sommets;
        for(i=0; i<nb_fac; i++) {
            Calcul_Normale_Barycentre(o,i);
            if (NotFlat) objet_courant->Facelist[i].flat = false;  // Garder plutôt ce qu'il y avait, sinon force le lissage de Gouraud partout : pas forcément souhaitable !
        }
        GenereTableauPointsFacettes(objet_courant);
//        printf("\n%d sf %d %d %d\n",o,objet_courant->Nb_sommets, objet_courant->Nb_facettes,nbfac);
//        printf("%d vl %d %d\n",  o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);
//
        GenereNormalesAuxSommets(o,nb_som);
//        printf("%d vl %d %d\n",  o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);

        // Recopie des numéros de sommets des facettes dans les numéros de vecteurs des luminances.
        objet_courant->Nb_vecteurs   = nb_som;
        objet_courant->Nb_luminances = nb_fac;
        indiceObjet_courant = o;
        str.clear();
        for (i=0; i<nb_fac; i++) {
            NumerosSommets = objet_courant->Facelist[i].getF_sommets();
            N_elements     = objet_courant->Facelist[i].getNumero();
            make1luminance();
        }
//        printf("%d vl %d %d\n",o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);
    }
    bdd_modifiee = true;
    printf("Fin\n");
}

void BddInter::InverseX() {
    Object * objet_courant;
    unsigned int i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[0] *= -1;
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[0]  *= -1;
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[0] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::InverseY() {
    Object * objet_courant;
    unsigned i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[1] *= -1;
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[1]  *= -1;
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[1] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::InverseZ() {
    Object * objet_courant;
    unsigned i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[2] *= -1;
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[2]  *= -1;
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[2] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoY() {
    Object * objet_courant;
    float memory;
    unsigned int i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3) {
                memory=objet_courant->Vecteurlist[j].point[0];
                objet_courant->Vecteurlist[j].point[0]=objet_courant->Vecteurlist[j].point[1];
                objet_courant->Vecteurlist[j].point[1]=memory;
            }
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3) {
                memory=objet_courant->Sommetlist[j].point[0];
                objet_courant->Sommetlist[j].point[0]=objet_courant->Sommetlist[j].point[1];
                objet_courant->Sommetlist[j].point[1]=memory;
            }
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            memory=objet_courant->Facelist[j].normale_b[0];
            objet_courant->Facelist[j].normale_b[0]=objet_courant->Facelist[j].normale_b[1];
            objet_courant->Facelist[j].normale_b[1]=memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoZ() {
    Object * objet_courant;
    float memory;
    unsigned i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3) {
                memory=objet_courant->Vecteurlist[j].point[0];
                objet_courant->Vecteurlist[j].point[0]=objet_courant->Vecteurlist[j].point[2];
                objet_courant->Vecteurlist[j].point[2]=memory;
            }
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3) {
                memory=objet_courant->Sommetlist[j].point[0];
                objet_courant->Sommetlist[j].point[0]=objet_courant->Sommetlist[j].point[2];
                objet_courant->Sommetlist[j].point[2]=memory;
            }
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            memory=objet_courant->Facelist[j].normale_b[0];
            objet_courant->Facelist[j].normale_b[0]=objet_courant->Facelist[j].normale_b[2];
            objet_courant->Facelist[j].normale_b[2]=memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::YtoZ() {
    Object * objet_courant;
    float memory;
    unsigned int i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3) {
                memory=objet_courant->Vecteurlist[j].point[1];
                objet_courant->Vecteurlist[j].point[1]=objet_courant->Vecteurlist[j].point[2];
                objet_courant->Vecteurlist[j].point[2]=memory;
            }
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3) {
                memory=objet_courant->Sommetlist[j].point[1];
                objet_courant->Sommetlist[j].point[1]=objet_courant->Sommetlist[j].point[2];
                objet_courant->Sommetlist[j].point[2]=memory;
            }
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            memory=objet_courant->Facelist[j].normale_b[1];
            objet_courant->Facelist[j].normale_b[1]=objet_courant->Facelist[j].normale_b[2];
            objet_courant->Facelist[j].normale_b[2]=memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoYtoZ() {
    Object * objet_courant;
    float memory;
    unsigned int i,j;
    for(i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for(j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3) {
                memory=objet_courant->Vecteurlist[j].point[2];
                objet_courant->Vecteurlist[j].point[2]=objet_courant->Vecteurlist[j].point[1];
                objet_courant->Vecteurlist[j].point[1]=objet_courant->Vecteurlist[j].point[0];
                objet_courant->Vecteurlist[j].point[0]=memory;
            }
        }
        for(j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3) {
                memory=objet_courant->Sommetlist[j].point[2];
                objet_courant->Sommetlist[j].point[2]=objet_courant->Sommetlist[j].point[1];
                objet_courant->Sommetlist[j].point[1]=objet_courant->Sommetlist[j].point[0];
                objet_courant->Sommetlist[j].point[0]=memory;
            }
        }
        for(j=0; j<objet_courant->Facelist.size(); j++) {
            memory=objet_courant->Facelist[j].normale_b[2];
            objet_courant->Facelist[j].normale_b[2]=objet_courant->Facelist[j].normale_b[1];
            objet_courant->Facelist[j].normale_b[1]=objet_courant->Facelist[j].normale_b[0];
            objet_courant->Facelist[j].normale_b[0]=memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::Simplification_BDD()
{
/* Fonction permettant de réduire le nombre de sommets de la BDD et d'éliminer les facettes
   dégénérées qui ont moins de 3 points. La distance minimale entre deux sommets est
   définie dans la fonction Points_Egaux() par la variable epsilon

   Réécrit à partir de la version Tcl */

    unsigned int o,i,j,k,l,m;
    unsigned int nb_points, nbface, cpt, nbsom, nb_modifs;
    unsigned int nbp_changes, nbv_changes;
    unsigned int nb_points2;
    std::vector<int> tabPoints;
    std::vector<float> Point_1, Point_i, Point_j;
    Object * objet_courant;
    Face   * Facette_courante;

    bool modification, indic;
//    bool verbose=false;             // Si true affiche plus d'indications des changements. A généraliser et initialiser à plus haut niveau ?

    float epsilon=tolerance*diagonale_save/100.0;                       // La tolérance est donnée en % de la diagonale

    wxBeginBusyCursor();                                                // Affiche un curseur animé par défaut (pour faire patienter !)

    printf("\nSimplification de BDD : Debut du traitement\n");
    printf("Tolerance d'egalite des sommets : relative : %8.3f%%, absolue : %7.2e\n",tolerance,epsilon);
    nbp_changes = nbv_changes = 0;

    for(o=0; o<this->Objetlist.size(); o++) {                       // On parcoure tous les objets en mémoire
        objet_courant = &(this->Objetlist[o]);
        printf("\nObjet : %s\n",objet_courant->GetName());
        if (objet_courant->deleted) continue ;                      // Ne pas traiter un objet supprimé (mais encore en mémoire), et donc passer au suivant
        nb_modifs    = 0;
        modification = false;
        nb_points    = objet_courant->Sommetlist.size() ;           // normalement égal à This->Objetlist[i].Nb_sommets;
        nb_points2   = objet_courant->Nb_sommets;
        if(nb_points != nb_points2) printf("Probleme sur objet %d: Sommetlist.size=%d, Nb_sommets=%d\n",o,nb_points,nb_points2);
        if (nb_points <= 0) continue ;                              // Ne pas traiter un objet sans sommets
//        printf("Nombre de sommets avant le traitement   : %u\n",nb_points);
        tabPoints.resize(nb_points);                                // Il ne devrait pas y en avoir plus !
        nbface = objet_courant->Nb_facettes;
        if (nbface <= 0) continue;

// Pour supprimer les points non utilisés .... on les marque comme des doublons du 1er sommet de la première facette
        int ind1 = objet_courant->Facelist[0].F_sommets[0] -1;      // -1 pour passer d'un numéro de sommet à un indice de sommet
        Point_1  = objet_courant->Sommetlist[ind1].getPoint();      // Coordonnées du 1er sommet de la 1ère facette dans Point_1
        for(i=1; i<=nb_points; ++i) {                               // Ici, i est un numéro de point (décalé de 1 / indice)
        //pour chaque point d'un objet
            cpt = 0 ;
            for (k=0 ; k<nbface ; k++ ) {
                Facette_courante = &(objet_courant->Facelist[k]);
                nbsom = Facette_courante->Nb_Sommets_F;
                for (l=0 ; l<nbsom; l++) {
                    if (Facette_courante->F_sommets[l] == (int)i) {
                        cpt++ ;         // i est utilisé
                        break ;         // Ne pas continuer
                    }
                }
                if (cpt != 0) break ;   // Idem
            }
            if (cpt == 0) { // Ce point n'est pas utilisé dans l'objet. Forcer à en faire un doublon pour pouvoir ensuite l'éliminer
                objet_courant->Sommetlist[i-1].setPoint(Point_1);   // Numero de sommet i => indice i-1
                if (verbose) {
                    printf("\rPoint numero %6d non utilise dans l'objet %d",i,o) ;      // On pourrait mettre le nom de l'objet plutôt que son indice => transcription wx->Ascii
                    fflush(stdout); // Pour Sun/cc
                }
            }
        }
        if (verbose) printf("\n");
        for(i=0; i<nb_points; ++i) {
            //pour chaque point d'un objet
//            objet_courant->Sommetlist[i].toshow = 0 ; // Raz
            Point_i = objet_courant->Sommetlist[i].getPoint();
            if (Point_i.empty()) continue;          // Point vide, non initialisé, non utilisé ?
            cpt = 0;
            for(j=(i+1); j<(nb_points-nb_modifs); ++j) {
            //on regarde si des points de la liste de sommets lui sont égaux (à epsilon près)
                Point_j = objet_courant->Sommetlist[j].getPoint();
                if (Point_j.empty()) continue;      // Point vide, non initialisé, non utilisé ?            A Vérifier ... si cela se produit ! Il faudrait l'éliminer plutôt !
                if (points_egaux(Point_i, Point_j, epsilon)) {
                    tabPoints[cpt++] = j+1; // Numéro de sommet et non indice ! décalé de 1 !!
                    if (verbose) {
                        printf("\rOBJET %u | doublon : sommet indice %5u a remplacer par le numero indice %5u",o,j,i);
                        fflush(stdout) ; // Pour Sun/cc
                    }
                }
            }
            if (cpt > 0) {
                if (verbose) printf("\n") ;
                modification = true;
                indic        = false;

                for(j=0; j<cpt; ++j) {
                    for(k=0; k<nbface; ++k) {
                        //on parcourt chaque face de l'objet
                        Facette_courante = &(objet_courant->Facelist[k]);
                        nbsom = Facette_courante->Nb_Sommets_F;
                        for(l=0; l<nbsom; ++l) {
                            if (Facette_courante->F_sommets[l] == tabPoints[j]) {
                                Facette_courante->F_sommets[l] = i+1; // Numéro de sommet et non indice ! décalé de 1 !!
                                if (verbose) {
                                    printf("\rModification de la facette %u pour l'objet %u",k,o);
                                    fflush(stdout) ;
                                    indic=true ;
                                }
                            }
                            else if (Facette_courante->F_sommets[l] > tabPoints[j]) {
                                //on décrémente tous les numéros de sommets supérieurs à tabPoints[l] dans le tableau
                                //de facettes, car lorsqu'on remplace n sommets, le nombre de sommets diminue
                                //de n sommets
                                //	printf("Numéro sommet supérieur %u ",elem->ob.tab_facette[k].ind_sommets[l]);
                                --(Facette_courante->F_sommets[l]);
                            }
                        }
                    }
                    //on traite aussi les sommets qui sont dans tabPoints
                    if (j < (cpt-1)) {
                        //si on n'est pas sur le dernier sommet de tabPoints[]
                        //Si les sommets suivants (après le rang j) de tabPoints sont supérieurs
                        //à tabPoints[j], alors on les décrémente aussi, pour que les sommets
                        //corrects soient remplacés
                        for(m=(j+1); m<cpt; ++m) {
                            if (tabPoints[m] > tabPoints[j]) {
                            //	printf("Dans le tableau de sommet, le sommet %u ",tabPoints[m]);
                                tabPoints[m] -= 1;
                            //	printf("devient %u\n",tabPoints[m]);
                            }
                        }
                    }
                    objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin()+tabPoints[j]-1);  // Supprime le sommet de rang tabPoints[j] dans la liste des sommets
                    objet_courant->Nb_sommets = objet_courant->Sommetlist.size();                       // Mettre à jour le nombre de sommets de l'objet
                    nb_modifs++;
                }
                if(indic && verbose) printf("\n") ;
            }
        }
        tabPoints.clear();

        if (modification) {
//            printf("Modification du tableau de points...\n");
            printf("Nombre de sommets avant le traitement : %u\n",nb_points);
            printf("Nouveau nombre de sommets             : %u\n",objet_courant->Nb_sommets);
//                for (i=0; i<objet_courant->Nb_sommets; i++) {
//                    Point_i = objet_courant->Sommetlist[i].getPoint();
//                    printf("%d %f %f %f\n",i,Point_i[0],Point_i[1],Point_i[2]);
//                }
//                for (i=0; i<objet_courant->Nb_facettes; i++) {
//                    std::vector<int> Nums = objet_courant->Facelist[i].F_sommets;
//                    printf("%3d : ",i);
//                    for (j=0; j<objet_courant->Facelist[i].F_sommets.size(); j++) {
//                        printf("%2d ",objet_courant->Facelist[i].F_sommets[j]);
//                    }
//                    printf("\n");
//                }
            bdd_modifiee = true;
            nbp_changes += nb_points - objet_courant->Nb_sommets;
        } else {
            printf("Nombre de sommets  : sans changement\n");
        }
        simplification_doublons_points(o);  // pour éliminer les points en doublons dans les facettes si besoin

        if (objet_courant->Nb_sommets < 3) {
            printf("Il reste moins de 3 points dans l'objet => Suppression de l'objet.\n") ;
            objet_courant->deleted = true ;      // Passer à l'objet suivant ...
            bdd_modifiee = true ;
        }
        else {
//            simplification_doublons_points(o);  // pour éliminer les points en doublons dans les facettes si besoin
            simplification_facettes(o);         // A faire même si pas de changement de sommets

            // Faire la même chose avec les normales aux sommets (ou vecteur selon la terminologie du format SDM)
            nb_modifs    = 0;
            modification = false;
            nb_points    = objet_courant->Nb_vecteurs;
            nb_points2   = objet_courant->Vecteurlist.size();       // normalement égal à This->Objetlist[i].Nb_vecteurs;
            if(nb_points != nb_points2) printf("Probleme sur objet %d: Nb_vecteurs=%d, Vecteurlist.size=%d\n",o,nb_points,nb_points2);
//            printf("Nombre de normales aux sommets (vecteurs) avant le traitement   : %u\n",nb_points);
            if (nb_points <= 0) {
                printf("Nombre de vecteurs : nul\n");
                continue ;                          // Ne pas traiter un objet sans vecteurs
            }
//            if (nb_points >= 0) continue ;                          /// pour test <=> désactive la simplification des normales aux sommets qui fait bugguer dans certains cas
//            printf("Nombre de normales aux sommets (vecteurs) avant le traitement   : %u\n",nb_points);
            for (k=0; k<nb_points;k++) {
                Point_1 = objet_courant->Vecteurlist[k].getPoint();     // Coordonnées du 1er vecteur dans Point_1
                if (!Point_1.empty()) break;                            // En fait, on prend la 1ère normale aux sommets qui a été définie
            }
            if (Point_1.empty()) {
                printf("Nombre de vecteurs : %d, mais pourtant Point_1 est vide !",nb_points);  // Oups ! Il y a un problème
                continue;                          // S'il est vide, passer à l'objet suivant
            }
            tabPoints.resize(nb_points);
            nbface = objet_courant->Nb_facettes;

            // Pour supprimer les normales non utilisées .... on les marque comme des doublons du 1er vecteur
            for(i=1; i<=nb_points; ++i) {                           // Ici i est un numéro de vecteur, donc décalé de 1 / indice
            //pour chaque normale au sommet d'un objet (vecteur au sens SDM)
                cpt = 0 ;
                for (k=0 ; k<nbface ; k++ ) {
                    Facette_courante = &(objet_courant->Facelist[k]);
                    Facette_courante->toshow = 0;   // Raz de précaution
                    nbsom = Facette_courante->Nb_Sommets_L;
                    for (l=0 ; l<nbsom; l++) {
                        if (Facette_courante->L_sommets[l] == (int)i) {
                            cpt++ ;         // i est utilisé
                            break ;         // Ne pas continuer
                        }
                    }
                    if (cpt != 0) break ;   // Idem
                }
                if (cpt == 0) { // Cette normale n'est pas utilisée dans l'objet. Forcer à en faire un doublon pour pouvoir ensuite l'éliminer
                    objet_courant->Vecteurlist[i-1].setPoint(Point_1);              // Numero de sommet i => indice i-1
                    if (verbose) {
                        sprintf(Message,"\rVecteur numéro %6d non utilisé dans l'objet %d",i,o) ;
                        printf(utf8_To_ibm(Message));
                        fflush(stdout); // Pour Sun/cc
                    }
                }
            }
            if (verbose) printf("\n");
            for(i=0; i<nb_points; ++i) {
                //pour chaque vecteur d'un objet
                cpt     = 0;
                Point_i = objet_courant->Vecteurlist[i].getPoint();
                if (Point_i.empty()) continue;                      // Ne pas tenir compte des vecteurs vides
                for(j=(i+1); j<(nb_points-nb_modifs); ++j) {
                //on regarde si des points de la liste de vecteurs lui sont égaux (à epsilon près)
                    if (points_egaux(Point_i, objet_courant->Vecteurlist[j].getPoint(), epsilon)) {
                        tabPoints[cpt++] = j+1; // Numéro de vecteur et non indice ! décalé de 1 !!
                        if (verbose) {
                            sprintf(Message,"\rOBJET %u | doublon : vecteur %5u à remplacer par le numéro %5u",o,j,i);
                            printf(utf8_To_ibm(Message));
                            fflush(stdout) ; // Pour Sun/cc
                        }
                    }
                }
                if (cpt > 0) {
                    if (verbose) printf("\n") ;
                    modification = true;
                    indic        = false;

                    for(j=0; j<cpt; ++j) {
                        for(k=0; k<nbface; ++k) {
                            //on parcourt chaque face (luminance) de l'objet
                            Facette_courante = &(objet_courant->Facelist[k]);
                            nbsom = Facette_courante->Nb_Sommets_L;
                            for(l=0; l<nbsom; ++l) {                // Si la facette n'a pas de normales aux sommets, nbsom vaut 0 et on ne passe pas dans la boucle
                                if (Facette_courante->L_sommets[l] == tabPoints[j]) {
                                    Facette_courante->L_sommets[l] = i+1; // Numéro de vecteur et non indice ! décalé de 1 !!
                                    if (verbose) {
                                        printf("\rModification de la luminance %u pour l'objet %u",k,o);
                                        fflush(stdout) ;
                                        indic = true ;
                                    }
                                }
                                else if (Facette_courante->L_sommets[l] > tabPoints[j]) {
                                    //on décrémente tous les numéros de vecteurs supérieurs à tabPoints[l] dans le tableau de luminances
                                    --(Facette_courante->L_sommets[l]);
                                }
                            }
                        }
                        //on traite aussi les sommets qui sont dans tabPoints
                        if (j<(cpt-1)) {
                            for(m=(j+1); m<cpt; ++m) {
                                if (tabPoints[m] > tabPoints[j]) {
                                //	printf("Dans le tableau de vecteurs, le vecteur %u ",tabPoints[m]);
                                    tabPoints[m] -= 1;
                                //	printf("devient %u\n",tabPoints[m]);
                                }
                            }
                        }
                        objet_courant->Vecteurlist.erase(objet_courant->Vecteurlist.begin()+tabPoints[j]-1);    // Supprime le vecteur de rang tabPoints[j] dans la liste des vecteurs
                        objet_courant->Nb_vecteurs = objet_courant->Vecteurlist.size();                         // Mettre à jour le nombre de vecteurs de l'objet
                        nb_modifs++;
                    }
                    if(indic && verbose) printf("\n") ;
                }
            }
            tabPoints.clear();
            if (modification) {
                printf("Nombre de normales aux sommets (vecteurs) avant le traitement : %u\n",nb_points);
                printf("Nouveau nombre de normales aux sommets                        : %u\n",objet_courant->Nb_vecteurs);
                nbv_changes = nb_points - objet_courant->Nb_vecteurs;
                bdd_modifiee = true;
            } else {
                printf("Nombre de vecteurs : sans changement\n");
            }
        }
    }   // boucle for sur les objets (o)

    for(o=0; o<this->Objetlist.size(); o++) {                       // On parcourt tous les objets en mémoire
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                      // Passer au suivant
        GenereTableauPointsFacettes(objet_courant);
        GenereTableauAretes(objet_courant);
    }
    m_gllist = 0;
    searchMin_Max();    // Reset des comptages de facettes, points, ...
    wxEndBusyCursor();                                              // Supprime le curseur animé... C'est terminé !

    printf("\n");
    if((nbp_changes == 0) && (nbv_changes == 0)) {
        printf("Aucun changement dans la BDD\n");
    } else {
        if (nbp_changes != 0) {
            if (nbp_changes == 1)
                sprintf(Message,"%4d point a été supprimé dans la BDD\n",      nbp_changes);
            else
                sprintf(Message,"%4d points ont été supprimés dans la BDD\n",  nbp_changes);
            printf(utf8_To_ibm(Message));
        }
        if (nbv_changes != 0) {
            if (nbv_changes == 1)
                sprintf(Message,"%4d vecteur a été supprimé dans la BDD\n",    nbv_changes);
            else
                sprintf(Message,"%4d vecteurs ont été supprimés dans la BDD\n",nbv_changes);
            printf(utf8_To_ibm(Message));
        }
    }

    undo_memory = 0;                        // Un undo de souder n'est plus possible
    MPanel->Button_Undo->Disable();         // et donc désactiver le bouton

    printf("\nFin de simplification de la Bdd\n");
}

void BddInter::genereAttributsFacettes(int indiceObjet, int Nb_facettes, int numeroGroupe, int numeroMateriau)
{
    // Groupes dans Aspect_face
    this->str.clear();
    this->N_elements = Nb_facettes;
    this->Objetlist[indiceObjet].Nb_aspects = Nb_facettes;

    this->makeaspect_face();
    for (int i=1; i <= this->N_elements ; i++) {
        this->Objetlist[indiceObjet].Facelist[i-1].groupe     = numeroGroupe;
        this->Objetlist[indiceObjet].Facelist[i-1].codmatface = numeroMateriau;
    }
}

void BddInter::genereLuminances(int indiceObjet, int Nb_facettes)
{
// Ici, ce sont les mêmes indices que pour les facettes

    this->str.clear();
    this->N_elements = Nb_facettes;
    this->Objetlist[indiceObjet].Nb_luminances = Nb_facettes;
    this->makeluminance();
    int numero = 0;

    for (int i=0; i<Nb_facettes; i++) {
        std::vector<int> NumerosSommets = this->Objetlist[indiceObjet].Facelist[i].getF_sommets();
        numero++;
        this->N_elements = numero;
        this->Set_numeros(NumerosSommets);
        this->make1luminance();
    }
}

void BddInter::genereNormalesFacettes(int indiceObjet, int Nb_facettes)
{
// Génère une normale à partir des 3 premiers points de la facette (peut ne pas être suffisant si points doublés, facette très allongée, ...)
// Peut poser des soucis sur des facettes très allongées : voir dans la version Tcl comment est contourné ce problème !

    int numero;
    std::vector<float> xyz_point;
    std::vector<int>   NumerosSommets;

    // Normales aux facettes
    this->str.clear();
    this->N_elements = Nb_facettes;
    this->Objetlist[indiceObjet].Nb_facettes = Nb_facettes;
    this->makenormale();

    numero = 0;
    for (int i=0; i<Nb_facettes; i++) {
        numero++;
        this->Objetlist[indiceObjet].Facelist[i].flat = false;   // Sphère ou ellipsoïde => non plat
        NumerosSommets = this->Objetlist[indiceObjet].Facelist[i].getF_sommets();
        xyz_point = this->Objetlist[indiceObjet].Sommetlist[NumerosSommets[0]-1].getPoint();
        Vector3D P1(xyz_point[0],xyz_point[1],xyz_point[2]);
        xyz_point = this->Objetlist[indiceObjet].Sommetlist[NumerosSommets[1]-1].getPoint();
        Vector3D P2(xyz_point[0],xyz_point[1],xyz_point[2]);
        xyz_point = this->Objetlist[indiceObjet].Sommetlist[NumerosSommets[2]-1].getPoint();
        Vector3D P3(xyz_point[0],xyz_point[1],xyz_point[2]);
        P1 -= P3;
        P2 -= P3;
        Vector3D Vn = P1.crossProduct(P2);
        Vn.normalize();
        this->N_elements=numero; this->Setxyz(Vn.X,Vn.Y,Vn.Z); this->make1normale();
    }
}

void BddInter::genereFacettesSphere(int Nb_Meridiens, int Nb_Paralleles, bool New_typeSphere)
{
    // D'après Facette_Sphere de sphere.c de la version Tcl
    // Version modifiée pour générer, en option, un nouveau tracé de facettes plus homogène, sans direction privilégiée.
    // Ce nouveau tracé fonctionne avec un nombre de // impair. Toutefois, la symétrie ne sera pas respectée si le nombre de méridiens n'est pas pair !

    int i, j, numero;
    int i1, i2, i3;
//    wxString str_loc;

    int m = Nb_Meridiens;
    int p = Nb_Paralleles;
    int Nb_facettes = 2*m*p;
    int Nmp  = m*p;
    int sens = 1;
    int sens1= 1;

    this->str.clear();
    this->N_elements = Nb_facettes;
    this->makeface();
    (this->Objetlist.end()-1)->Nb_facettes = this->N_elements;

    numero = 0;
    for (i=1; i<m; i++) {
        // Nord
        numero++;
        i1 = 1;
        i3 = 1 + i;
        i2 = i3+ 1;
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
        this->make1face();

        if (New_typeSphere) {
            sens = sens1;
            if ((i%2) == 0) sens = -sens1;  // En réinitialisant systématiquement à chaque fois, on peut avoir un nombre impair de //
        }

        // i+(m*j) = 1er de chaque parallele ; i+(m*j)+m = 1er du parallele suivant
        // m+(m*j) = dernier de chaque parallele ; m+(m*j)+m = dernier du parallele suivant
        for (j=0; j<p-1 ; j++) {
            // face haut droit
            numero++;
            i1 = 1  + i + (m*j);
            i2 = i1 + 1;
            if (sens > 0) {
/*     2---1
         \ |
           3
*/
                i3 = i1 + m;
            } else {
/*     2---1
       | /
       3
*/
                i3 = i2 + m;
            }
            this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
            this->make1face();

			// face bas gauche
            numero++;
/*     1
       | \
       2---3
*/
            if (sens > 0) {
                i1 = i2;
                i2 = i1 + m ;   // i3 inchangé
            } else {
/*         1
         / |
       2---3
*/
                i2 = i3;
                i3 = i2 -1;
            }
            this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
            this->make1face();

            if (New_typeSphere) sens *= -1;
        }

        // Sud
        numero++;
        i1 = Nmp +2;                                            // Pole Sud
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i3,i2);  // récup des 2 derniers i2,i3 mais inverser le sens
        this->make1face();
    }

// Code spécifique pour la dernière série entre dernier et 1er méridien

    if (New_typeSphere) sens = -sens1;  // Ne fonctionne bien que si le nombre de méridiens est pair.
    // derniere face pole nord
    numero++;
    i1 = 1;
    i2 = 2 ;
    i3 = 1 + m ;
    this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
    this->make1face();

    for (j=0; j<p-1 ; j++) {
        // derniere face haut droit
        numero++;
        i1 = 1 + m + (m*j);
        i2 = 2 + (m*j);
        if (sens > 0) {
            i3 = i1 + m;
        } else {
            i3 = i2 + m;
        }
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
        this->make1face();

        // derniere face bas gauche
        numero++;
        if (sens > 0) {
            i1 = i2; //2 + (m*j);
            i2 = i1+ m ;
            i3 = 1 + m*(j+2);
        } else {
            i2 = i3;
            i3 = i1 + m;
        }
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
        this->make1face();
        if (New_typeSphere) sens *= -1;
    }

    // derniere face pole sud
    numero++;
    i2 = 1  + (m*p) ;
    i1 = i2 + 1;
    i3 = i2 -m +1;
    this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
    this->make1face();
}

void BddInter::genereSommetsSphere(int Nb_Meridiens, int Nb_Paralleles, float centre[3], float rayon, float coefx, float coefy, float coefz)
{
    int i, j, numero;
    float theta,phi, sin_phi, cos_phi;
    float Xc = centre[0] , Yc = centre[1] , Zc = centre[2];
    int NbPoints = (Nb_Meridiens*Nb_Paralleles)+2;

    this->str.clear();
    this->N_elements = NbPoints;
    this->makesommet();
    (this->Objetlist.end()-1)->Nb_sommets = NbPoints;
    // Pôle Nord
    numero = 1;
    this->N_elements = numero;
    this->Setxyz(Xc, Yc+coefy*rayon, Zc);
    this->make1sommet();

    // Parcours des parallèles et des méridiens
    for (i=1; i<= Nb_Paralleles; i++) {
        phi = M_PI*i / (Nb_Paralleles+1);
        sin_phi = sin(phi);
        cos_phi = cos(phi);
        for (j=1; j<=Nb_Meridiens; j++) {
            theta = 2*M_PI*j / Nb_Meridiens;
            numero++;
            this->N_elements = numero;
            this->Setxyz(Xc +coefx*rayon*cos(theta)*sin_phi,
                         Yc +coefy*rayon*cos_phi,
                         Zc +coefz*rayon*sin(theta)*sin_phi);
            this->make1sommet();
        }
    }
    // Pôle Sud
    numero = NbPoints;
    this->N_elements = numero;
    this->Setxyz(Xc, Yc-coefy*rayon, Zc);
    this->make1sommet();
}

void BddInter::genereNormalesSommetsSphere(int Nb_Meridiens, int Nb_Paralleles, float coefx, float coefy, float coefz)
{
    int i, j, numero;
    float theta, phi, sin_phi, cos_phi;
    int NbPoints = (Nb_Meridiens*Nb_Paralleles)+2;

    this->str.clear();
    this->N_elements = NbPoints;
    this->makevecteur();
    (this->Objetlist.end()-1)->Nb_vecteurs = NbPoints;
    // Pôle Nord
    numero = 1;
    this->N_elements = numero;
    this->Setxyz(0., 1., 0.);
    this->make1vecteur();

    // Parcours des parallèles et des méridiens
    for (i=1; i<= Nb_Paralleles; i++) {
        phi = M_PI*i / (Nb_Paralleles+1);
        sin_phi = sin(phi);
        cos_phi = cos(phi);
        for (j=1; j<=Nb_Meridiens; j++) {
            theta = 2*M_PI*j / Nb_Meridiens;
            numero++;
            this->N_elements = numero;
            Vector3D vn(cos(theta)*sin_phi/coefx, cos_phi/coefy, sin(theta)*sin_phi/coefz) ;
            vn.normalize();
            this->Setxyz(vn.X, vn.Y, vn.Z);
            this->make1vecteur();
        }
    }
    // Pôle Sud
    numero = NbPoints;
    this->N_elements = numero;
    this->Setxyz(0., -1., 0.);
    this->make1vecteur();
}

void BddInter::GenereNormale_1_Sommet(Object *current_objet, unsigned int indice_point, unsigned int indice_vecteur)
{
// Calcul de la normale sur un sommet (additions des normales des facettes adjacentes et normalisation)

    bool test_np;
    int nfac, k, k1, k2, nbs;
    float  coef ;
    std::vector<float> pa, pb, pc;
    std::vector<float> NormaleSommet;
    Vector3D v1, v2, vp, np;
    Face cur_fac;
    int  NbFacettes;

    test_np   = false ;
    np = Vector3D();//    SetCoordonnees(np,0.,0.,0.) ;                               // Initialisation

    NbFacettes = current_objet->Pointslist[indice_point].IndicesFacettes.size();
    for (k=0; k < NbFacettes ; k++) {
        nfac    = current_objet->Pointslist[indice_point].IndicesFacettes[k] ;
        cur_fac = current_objet->Facelist[nfac];
        nbs     = cur_fac.Nb_Sommets_F;
// ATTENTION : si vecteur + son opposé => Norme nulle !! cas de 2 facettes confondues (dessus et dessous) ...

//                    np = AdditionPoint(np, cur_fac.normale);
// Calcul de l'angle en ce sommet pour pondérer le calcul des moyennes de normales
//                angle = 1 ;

// Méthode par somme d'angles :
/*      k1 = k-1 ;
        if (k1 <= 0)  k1 = k1+nbs ;
        k2 = k+1 ;
        if (k2 > nbs) k2 = k2-nbs ;
        pa = current_objet->tab_point[npoint] ;
        pb = current_objet->tab_point[cur_fac.ind_sommets[k1]] ;
        pc = current_objet->tab_point[cur_fac.ind_sommets[k2]] ;
        v1 = SoustractionPoint(pb,pa) ;
        v2 = SoustractionPoint(pc,pa) ;
        Normalise(&v1) ;
        Normalise(&v2) ;
        coef = acos(produit_scalaire(&v1, &v2)) ;
*/
// Méthode par surface de facette (si elles ne sont pas planes, on pourrait calculer à partir du sommet k,
// et incrémenter k+1, k+2,... avec le même test que ci-dessus si dépassement sur les numéros).
// Le résultat sera cependant assez peu différent.
        coef = 0.;
        pa   = current_objet->Sommetlist[cur_fac.F_sommets[0]-1].getPoint() ;
        pb   = current_objet->Sommetlist[cur_fac.F_sommets[1]-1].getPoint() ;
        v1.X    = (double)(pb[0] - pa[0]); v1.Y = (double)(pb[1] - pa[1]); v1.Z = (double)(pb[2] - pa[2]);
        for (k2=2 ; k2 < nbs ; k2++) {
            pc = current_objet->Sommetlist[cur_fac.F_sommets[k2]-1].getPoint() ;
//            v2 = SoustractionPoint(pc,pa) ;
            v2.X = (double)(pc[0] - pa[0]); v2.Y = (double)(pc[1] - pa[1]); v2.Z = (double)(pc[2] - pa[2]);
//            coef += Norme(produit_vect(&v1,&v2)) ; // En toute rigueur diviser par 2 !
            vp = v1.crossProduct(v2);
            coef += vp.length();  // En toute rigueur diviser par 2 !
            v1 = v2 ;
        }

        np.X += coef*cur_fac.normale_b[0] ;
        np.Y += coef*cur_fac.normale_b[1] ;
        np.Z += coef*cur_fac.normale_b[2] ;
        test_np = true ;
    }
    if (np.length() == 0) test_np = false;  // Si c'est le cas, c'est plutôt étrange ou cas de 2 facettes confondues (dessus et dessous) ! Par précaution ...
    if (test_np) {
        np.normalize() ;
//        NormaleSommet.resize(3);
//        NormaleSommet[0] = np.X ; NormaleSommet[1] = np.Y ; NormaleSommet[2] = np.Z ;
        NormaleSommet = {(float)np.X, (float)np.Y, (float)np.Z};
        current_objet->Vecteurlist[indice_vecteur].setPoint(NormaleSommet) ;
    } else {
        NormaleSommet = {0., 0., 1.};                                       // initialiser tout de même ??? évite un plantage dans les InverseX, Y ou Z si pas de point.size()
        current_objet->Vecteurlist[indice_vecteur].setPoint(NormaleSommet) ;
    }
}

//******************************************************************************
void BddInter::GenereNormalesAuxSommets(unsigned int o, int nb_p)
{
// Calcul des normales aux sommets (additions des normales des facettes adjacentes et normalisation)
// + Reconstruction des luminances
    int indice_point ;
    Object * objet_courant;
    Face   * facette_courante;
//    bool verbose=false;       // Pour l'instant ici !

//    printf("\n");
    objet_courant = &(this->Objetlist[o]) ;
    objet_courant->Vecteurlist.clear();         // On remet à 0 la liste de vecteurs (normales aux sommets)
    objet_courant->Vecteurlist.resize(nb_p);    // La liste des vecteurs a donc la même dimension que la liste de sommets
    for (indice_point=0 ; indice_point < nb_p ; indice_point++) {
        if ((indice_point%500000) == 0 && verbose) { // 500 à l'origine, histoire de faire patienter, mais n'est plus très utile maintenant.
            sprintf(Message,"\rCalcul des normales aux sommets de l'objet %2d : %8d",
                    objet_courant->GetValue(), indice_point);
            printf(utf8_To_ibm(Message));
//            printf("\rCalcul des normales aux sommets de l'objet %2d : %8d", objet_courant->GetValue(), indice_point);
            fflush(stdout); // Nécessaire sur cc SUN, sinon la ligne n'apparaît pas
        }
        GenereNormale_1_Sommet(objet_courant,indice_point,indice_point) ;   // NOTE : ici indice_vecteur = indice_point
                                                                            // => synchroniser les numéros de sommets des facettes et des luminances
    }
    // Recopie des numéros de sommets de facettes dans numéros de sommets des luminances (doivent être les mêmes du fait de l'appel à GenereNormale_1_Sommet ci-dessus !)
    for (unsigned int nfac=0; nfac<objet_courant->Facelist.size(); nfac++) {    // <=> Nb_facettes
        facette_courante = &(objet_courant->Facelist[nfac]);
        facette_courante->L_sommets.clear();                                // Pas sûr que ce soit utile (libération de la mémoire)
        facette_courante->L_sommets    = facette_courante->F_sommets;
        facette_courante->Nb_Sommets_L = facette_courante->Nb_Sommets_F;    // Par précaution, mais c'est sans doute déjà le cas.
    }
    if (verbose) {
        sprintf(Message,"\rCalcul des normales aux sommets de l'objet %2d : %8d\n",
                objet_courant->GetValue(), indice_point);
        printf(utf8_To_ibm(Message));
//        printf("\rCalcul des normales aux sommets de l'objet %2d : %8d\n", objet_courant->GetValue(), indice_point);
    }
}

bool BddInter::compare_normales(int objet, int face_1, int face_2) {
/* renvoie true si les composantes des normales aux barycentres des facettes face_1 et face_2 ont les mêmes signes (version originale) */

// Et si on vérifiait plutôt que l'angle entre n1 et n2 soit inférieur à une valeur test (+-45° par exemple !)
// On pourrait paramétrer cette valeur dans Préférences et/ou directement dans Sélection et déplacements

    std::vector<float> n1, n2;

    const float seuil=0.5;//707f; // 0.707 = cos(45°) , 0.5 = cos(60°)
    float prod;

    n1   = this->Objetlist[objet].Facelist[face_1].getNormale_b();
    n2   = this->Objetlist[objet].Facelist[face_2].getNormale_b();
    prod = produit_scalaire(n1,n2);

    if (prod > seuil) return true; // L'angle entre les 2 normales est < acos(seuil)

    return false;
}

bool BddInter::points_egaux(const std::vector<float> &point_1, const std::vector<float> &point_2, float epsilon)
{
// Retourne true si la distance entre les 2 points est <= epsilon. Les points sont alors considérés comme égaux !

// Note : dans la version TCL on comparait séparément les 3 différences sur x, y et z (en fabs) et return true si les 3 différences < epsilon. Plus rapide ?
    if (point_1.empty()) return false;
    if (point_2.empty()) return false;
    float distance = Norme3(point_2[0]-point_1[0], point_2[1]-point_1[1], point_2[2]-point_1[2]);
    if (distance > epsilon) return false;
//    printf("x : %f %f\n",point_2[0],point_1[0]);
//    printf("y : %f %f\n",point_2[1],point_1[1]);
//    printf("z : %f %f\n",point_2[2],point_1[2]);
//    printf("distance, epsilon : %f %f\n",distance,epsilon);
    return true;
}

/* Cette fonction permet d'enlever les doublons qui se trouvent dans le tableau de sommets
   d'une facette (l'équivalent pour les normales n'a pas de raison d'être : 2 points d'une facette peuvent avoir la même normale) */
void BddInter::simplification_doublons_points(unsigned int objet)
{
	unsigned int i,j,k,nbsom;
    Object      * objet_courant;
    Face        * facette;
    bool test;

    objet_courant = &(this->Objetlist[objet]) ;

	for(i=0; i<objet_courant->Nb_facettes; ++i) {
        facette   = &(objet_courant->Facelist[i]);
		nbsom   = facette->Nb_Sommets_F;
		for(j=0; j<nbsom; ++j) {
			for(k=0; k<nbsom; ++k) { // Faut-il reprendre à partir de 0 ou seulement de j+1 ?
				if(k!=j) {
					if(facette->F_sommets[j] == facette->F_sommets[k] ) {
                        test = false;                               // true si pas de normales aux sommets ???
                        if(objet_courant->Nb_luminances != 0) {
                            if(facette->L_sommets[j] == facette->L_sommets[k] )
                                test = true;
                            else
                                test = false;
                        }
                        if (test){
                            // Si les normales existent, 2 points sont égaux si elles sont aussi égales => Supprimer 1 des points
                            printf("objet : %2d, doublons sur facette %u \n",objet,i);
                            facette->deleteone(k);
                            nbsom   = facette->Nb_Sommets_F; // Mettre à jour la borne sup !
                            facette->Nb_Sommets_L-- ;        // Le deleteone a aussi supprimé la normale aux sommets correspondante
                            /// Ne faudrait-il pas faire la même chose sur luminance ?
                            bdd_modifiee = true;
                        }
					}
				}
			}
		}
	}
}

/* Cette fonction permet de supprimer de la Bdd en mémoire les facettes dégénérées à moins de 3 points
(ne se contente pas de les marquer deleted et ainsi évite qu'un undelete ne les restitue !)            */
void BddInter::simplification_facettes(unsigned int objet)
{
	unsigned int i,nbface,decalage;
    Object      * objet_courant;

    objet_courant = &(this->Objetlist[objet]) ;

	nbface  = objet_courant->Nb_facettes;
	decalage=0 ;
	for(i=0; i<nbface; ++i)	{
        objet_courant->Facelist[i].toshow = 0;              // Raz de cette valeur pour éviter qu'un éventuel futur Undo de souder trouve quelque chose à faire !
		if(objet_courant->Facelist[i].Nb_Sommets_F < 3) {
		/* On supprime la facette i */
            if(decalage == 0) printf("Objet indice %d\n",objet) ;
            sprintf(Message,"facette %u a supprimer (moins de 3 points)\n",i+decalage);
            printf(utf8_To_ibm(Message));

            objet_courant->Facelist.erase(objet_courant->Facelist.begin()+i);
            --objet_courant->Nb_facettes;                                       // Faut-il aussi changer Nb_luminances (si Nb_sommets_L = 0 ?)
            if (objet_courant->Facelist[i].Nb_Sommets_L > 0) {
                --objet_courant->Nb_luminances;
                objet_courant->Facelist[i].Nb_Sommets_L--;
            }
            --nbface;

            --i ; /* Décalé de 1, donc recommencer avec le même i à la boucle suivante */
            ++decalage;
            bdd_modifiee = true; ;
		}
	}
}

void BddInter::GenereListeGroupesMateriaux(unsigned int objet)
{
    Object      * objet_courant;
    unsigned int indice;
    int groupe, materiau;

    objet_courant = &(this->Objetlist[objet]) ;
    for (indice=0; indice < objet_courant->Nb_facettes; indice++) {
        groupe = objet_courant->Facelist[indice].groupe;
        auto it = std::find(listeGroupes.begin(),listeGroupes.end(),groupe);    // Est-il déjà dans la liste ?
        if (it == listeGroupes.end() || listeGroupes.empty()) {                 // Non
            listeGroupes.push_front(groupe);                                    // L'ajouter à la liste des groupes
        }
        materiau = objet_courant->Facelist[indice].codmatface;
        it = std::find(listeMateriaux.begin(),listeMateriaux.end(),materiau);   // Est-il déjà dans la liste ?
        if (it == listeMateriaux.end() || listeMateriaux.empty()) {             // Non
            listeMateriaux.push_front(materiau);                                // L'ajouter à la liste des matériaux
        }
    }
}

/* Fixe les coordonnées du 2ème point (xa_sel,ya_sel) permettant de sélectionner des pixels dans le rectangle de sélection avec le premier point xd_sel,yd_sel*/
void BddInter::Selection_rectangle (GLint xa, GLint ya)
{
	int i,j,nb_points;
    GLdouble Cursor_X0, Cursor_Y0, Cursor_X, Cursor_Y, pas_X, pas_Y;
    GLdouble select_largeur, select_hauteur;

    nb_points = 4;  // Soit nb_points -1 intervalles (doit être > 2)

	xa_sel = xa;
	ya_sel = ya;

//	printf("%4d %4d %4d %4d\n",xd_sel,yd_sel,xa_sel,ya_sel);

	//calcul de la largeur et de la hauteur de la zone de sélection
	select_largeur = (GLdouble)abs(xa_sel-xd_sel);
	select_hauteur = (GLdouble)abs(ya_sel-yd_sel);

	//calcul du point central de la zone de sélection
//	xmil = (GLdouble)(xd_sel+xa_sel)*0.5;
//	ymil = (GLdouble)(yd_sel+ya_sel)*0.5;

    Cursor_X0 = ((xd_sel < xa_sel) ? xd_sel : xa_sel);  // Le point debut (d) peut-être > point final (a) => Cursor_X0 est la valeur minimale sur l'écran
    Cursor_Y0 = ((yd_sel < ya_sel) ? yd_sel : ya_sel);  // Idem en Y
    pas_X     = select_largeur/(nb_points-1);           // Pour limiter le nombre de boucles en i et j mais du coup, remplissage avec des oublis
    pas_Y     = select_hauteur/(nb_points-1);           // car on ne parcoure pas toute la sélection rectangulaire ! Temps de parcours trop long

    // NOTE : on peut se contenter du début et de l'arrivée sur les bords du rectangle et ne subdiviser qu'à l'intérieur
    //        => moins de points à tester => + rapide mais un peu moins efficace
    //        On pourrait tester un tirage aléatoire de points en nombre pas trop élevé ...
    /****/
    // 4 coins dans un premier temps
//    for (i=0,Cursor_X=Cursor_X0 ; i<2 ; i++,Cursor_X+=select_largeur) {
//        for (j=0,Cursor_Y=Cursor_Y0 ; j<2 ; j++,Cursor_Y+=select_hauteur) {
//            testPicking(round(Cursor_X), round(Cursor_Y), modeGL, false) ;
//        }
//    }
//    // Intérieur maintenant
//    Cursor_X0 += pas_X;
//    Cursor_Y0 += pas_Y;
//    nb_points -= 2;
    /****/
    // Ici suffit, sans les 4 coins à part pour être plus exhaustif (supprimer/commenter les lignes entre les 2 /****/

    for (i=0,Cursor_X=Cursor_X0 ; i<nb_points ; i++,Cursor_X+=pas_X) {
        for (j=0,Cursor_Y=Cursor_Y0 ; j<nb_points ; j++,Cursor_Y+=pas_Y) {
            testPicking(int(Cursor_X +0.5), int(Cursor_Y +0.5), modeGL, false) ;
//            testPicking(round(Cursor_X), round(Cursor_Y), modeGL, false) ;   // round souci en debug !
        }
    }
// Via tirage aléatoire, mais pas beaucoup mieux !
//	srand (time(NULL));
//	nb_points*=2;
//    for (i=0;i<nb_points;i++) {
//        Cursor_X = Cursor_X0 + float(rand())/RAND_MAX*select_largeur;
//        Cursor_Y = Cursor_Y0 + float(rand())/RAND_MAX*select_hauteur;
//        testPicking(round(Cursor_X), round(Cursor_Y), modeGL, false) ;
//    }
	return;
}

/*Dessine le rectangle de sélection pendant la multi-sélection*/
void BddInter::draw_rectangle_selection()
{
//    printf("OK2 : ");
    if ((xd_sel == xa_sel) && (yd_sel == ya_sel)) return; // Rien à faire dans ce cas

    GLint yd_offset,ya_offset;

    wxSize ClientSize = this->GetSize();
//printf ("%d %d\n",ClientSize.x,ClientSize.y);
	glDisable(GL_LIGHTING);

	glPushMatrix();                      // sauvegarde de la matrice de modélisation
	glLoadIdentity();                    // réinitialisation de la matrice

	glMatrixMode(GL_PROJECTION);         // on sélectionne la matrice de projection
	glPushMatrix();                      // On sauvegarde la matrice de projection
	glLoadIdentity();                    // on charge l'identité

	gluOrtho2D(0,ClientSize.x,ClientSize.y,0);//On passe en 2D

	glMatrixMode(GL_MODELVIEW);          // on sélectionne la matrice de modélisation

	glLineWidth(1.3);
	glColor3f(0.8f,0.7f,0.6f);

	glDepthFunc(GL_ALWAYS);             // le rectangle de sélection s'affiche par dessus tout les objets de la scène

	//glEnable(GL_LINE_STIPPLE);        // active les pointillés
	//glLineStipple(1,0x0F0F);          // spécifie le type de pointillé

	yd_offset = yd_sel+offset_pointeur; // Décaler en Y à cause de l'offset du pointeur
	ya_offset = ya_sel+offset_pointeur; // ""

	glBegin(GL_LINE_LOOP);              // On trace le rectangle de sélection
		glVertex2i(xd_sel,yd_offset);
		glVertex2i(xa_sel,yd_offset);
		glVertex2i(xa_sel,ya_offset);
		glVertex2i(xd_sel,ya_offset);
	glEnd();
	//glDisable(GL_LINE_STIPPLE);


	glDepthFunc(GL_LEQUAL);             // on repasse au test normal (objet le plus près affiché)

	glLineWidth(1);
//	glDisable(GL_LINE_STIPPLE);//on désactive les pointillés

	glPopMatrix();                      // on restaure la matrice de modélisation

	glMatrixMode(GL_PROJECTION);        // on sélectionne la matrice de projection
	glPopMatrix();                      // et on la restaure

	glMatrixMode(GL_MODELVIEW);         // on re-sélectionne la matrice de modélisation avant de quitter la fonction

	glEnable(GL_LIGHTING);
}
