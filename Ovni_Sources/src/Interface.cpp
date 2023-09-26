//(*AppHeaders
#include "Interface.h"
//*)

// Fichier Interface.cpp général (base de la classe BddInter)
// **********************************************************

BEGIN_EVENT_TABLE(BddInter, wxGLCanvas)
    EVT_SIZE(BddInter::OnSize)
    EVT_PAINT(BddInter::OnPaint)
    EVT_ERASE_BACKGROUND(BddInter::OnEraseBackground)
    EVT_KEY_DOWN( BddInter::OnKeyDown )
    EVT_MOUSEWHEEL(BddInter::OnMouseWheelMoved) // Semble (?) réagir plus vite si placé avant EVT_MOUSE_EVENTS
    EVT_MOUSE_EVENTS(BddInter::OnMouse)
    EVT_TIMER(wxID_ANY,BddInter::OnTimer_Bdd)
END_EVENT_TABLE()

#if wxCHECK_VERSION(3,1,0)
BddInter::BddInter(wxWindow *parent, const wxGLAttributes& AttribList, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, bool main_verbose, const wxString& name):
// Explicitly create a new rendering context instance for this canvas.
    wxGLCanvas(parent, AttribList, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
    m_glRC = new wxGLContext(this);
#else
BddInter::BddInter(wxWindow *parent, const int* AttribList,            wxWindowID id, const wxPoint& pos, const wxSize& size, long style, bool main_verbose, const wxString& name):

#if wxCHECK_VERSION(3,0,0)
    wxGLCanvas(parent, id, AttribList, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
// Explicitly create a new rendering context instance for this canvas.
    m_glRC = new wxGLContext(this);

#else
    wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, AttribList)
{
#endif // wxCHECK_VERSION
#endif

/** \brief BddInter::BddInter Création/Initialisation de la classe BddInter
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 * \param AttribList    : liste des attributs de GLCanvas créés dans OvniMain
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 * \param style         : style créé dans OvniMain
 * \param main_verbose  : booléen indiquant si on affiche ou pas diverses entrées/sorties de fonctions
 * \param name          : nom transmis depuis OvniMain (peut ne pas exister)
 *
 */

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
    for (int j=0; j<4; j++) {
        for (int i=0; i<nb_couleurs; i++) {
            MatAmbient_avionG[i][j] = MatAmbient_avionG_def[i][j];
            MatDiffuse_avionG[i][j] = MatDiffuse_avionG_def[i][j];
        }
    }

    nb_max_threads = omp_get_max_threads(); // Récupérer en entrée le nombre max de threads/processeurs pour OpenMP
    ResetData();                            // Remettre aux valeurs par défaut
    Ouvrir_ini_file();                      // Lire dans le fichier .ini les valeurs éventuellement modifiées / défaut
    pal_file_modified = false;              // Fichier palette non lu, donc par défaut, non modifié !
    if ((nb_threads <= 0) || (nb_threads >= nb_max_threads)) {
        omp_set_dynamic(1);                 // Nombre de threads automatique, soit le maximum
//        if (nb_threads >= nb_max_threads) nb_threads = nb_max_threads;
//        omp_set_num_threads(nb_max_threads);
    } else {
        omp_set_dynamic(0);                 // Forcer le nombre de threads
        omp_set_num_threads(nb_threads);
    }

    Smemory = nullptr;
    selectBuffer        = (GLuint*)malloc(BUFSIZE*sizeof(GLuint));
    this->type_fichier  = -1;
    this->m_loaded      = false;
    this->MAIN_b        = dynamic_cast<OvniFrame*>(this->GetGrandParent());//((this->GetParent())->GetParent());    // Il faut remonter de 2 parents (GLCanvas puis OvniFrame).
//    wxSize ClientSize = this->GetSize();
//    printf("BddInter::BddInter ClientSize X/Y %d %d\n",ClientSize.x,ClientSize.y);
    //W=ClientSize.x;
    //H=ClientSize.y;
    if(verbose) printf("Sortie de BddInter::BddInter\n");
}

BddInter::~BddInter()
{
#if wxCHECK_VERSION(3,0,0)
    delete m_glRC;
#endif // wxCHECK_VERSION
}
int BddInter::Convert_Rotx_LSI() {
    int ival;
    ival = lround(fmod((m_gldata.rotx - 270), 360)) ;        // Comme dans la version TCL
    if (ival < -180) ival += 360 ;
    return ival;
}

int BddInter::Convert_Rotz_LAZ() {
    int ival;
    ival = lround(fmod((360 - m_gldata.rotz), 360)) ;
    if (ival < 0) ival += 360 ;
    return ival;
}

void BddInter::ResetData() {
    int ival;

    if (verbose) printf("Entree de BddInter::ResetData\n");
    m_gldata.BeginX         = 0;
    m_gldata.BeginY         = 0;
    len_axe                 = len_axe_def;
    len_normales            = len_normales_def;
    ray_sun                 = ray_sun_def;
    angle_Gouraud           = angle_Gouraud_def;
    fmult_Gouraud           = fmult_Gouraud_def;
    seuil_Gouraud           = cos(angle_Gouraud*to_Rad);
    if (angle_Gouraud  >= 179.9) seuil_Gouraud  = -1.0f;
    angle_Gouraud2          = angle_Gouraud*fmult_Gouraud;
    seuil_Gouraud2          = cos(angle_Gouraud2*to_Rad);
    if (angle_Gouraud2 >= 179.9) seuil_Gouraud2 = -1.0f;
    tolerance               = tolerance_def;
    svg_time                = svg_time_def;
    Raz_Selection_F         = Raz_Selection_F_def;
    msg_warning             = msg_warning_def;
    traiter_doublons_aretes = traiter_doublons_aretes_def;
    icon_index              = icon_index_def;
    icon_size               = icon_sizes[icon_index];

    nb_threads              = nb_threads_def;
    if ((nb_threads <= 0) || (nb_threads >= nb_max_threads)) {
        omp_set_dynamic(1);                     // Nombre de threads automatique, soit le maximum
//        if (nb_threads >= nb_max_threads) nb_threads = nb_max_threads;
        omp_set_num_threads(nb_max_threads);    // Par précaution
    } else {
        omp_set_dynamic(0);                     // Pour forcer/limiter le nombre de threads pour OpenMP
        omp_set_num_threads(nb_threads);
    }

    m_gldata.rotx           = 0.0f;
    m_gldata.roty           = 0.0f;
    m_gldata.rotz           = 0.0f;
    m_gldata.fmult_diag     = fmult_diag_def;
    reset_zoom              = true;
    SetPosObs(reset_zoom);
    mode_selection          = selection_facette;

    m_gldata.mode_Trackball = mode_Trackball_def;
    printf("ResetData : Mode Trackball : %d\n",m_gldata.mode_Trackball);
    if (Slider_x != nullptr) {
        Slider_x->SetValue(0);
        Slider_y->SetValue(0);
        Slider_z->SetValue(0);
    }
    trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);

//    antialiasing_soft   = antialiasing_soft_def;  // à vérifier, mais pas forcément utile
    test_decalage3ds    = test_decalage3ds_def;
    Forcer_1_Seul_Objet = Forcer_1_Seul_Objet_def;
    lect_obj_opt        = lect_obj_opt_def;
    forcer_simplification_doublons_aretes = forcer_simplification_doublons_aretes_def;
    if (Forcer_1_Seul_Objet) lect_obj_opt = false;              // Pas d'optimisation si 1 seul Objet 3D dans un fichier .obj
    CalculNormalesLectureBdd = CalculNormalesLectureBdd_def;
    Enr_Normales_Seuillees   = Enr_Normales_Seuillees_def;
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    wxWorkDir = wxOvniPath   = appPath + wxFILE_SEP_PATH;       // Ajouter le séparateur de Path à la fin
    if (verbose) {
        sprintf(Message,"ResetData : Répertoire de travail :\n%s\n",(const char*)wxWorkDir.utf8_str());
        printf("%s",utf8_To_ibm(Message));
    }
    if (MPrefs != nullptr) {
        MPrefs->CheckBox_AntialiasingSoft->SetValue(antialiasing_soft);
        MPrefs->CheckBox_TestDecalage3DS ->SetValue(test_decalage3ds);
        MPrefs->CheckBox_1SeulObjet3D    ->SetValue(Forcer_1_Seul_Objet);
        if (Forcer_1_Seul_Objet) MPrefs  ->CheckBox_LectureOptimisee->Disable();
        else                     MPrefs  ->CheckBox_LectureOptimisee->Enable();
        MPrefs->CheckBox_LectureOptimisee->SetValue(lect_obj_opt);
        MPrefs->RadioBox_Triangulation   ->SetSelection(methode_Triangulation);
        MPrefs->RadioBox_Trackball       ->SetSelection(m_gldata.mode_Trackball);
        MPrefs->CheckBox_DisplayFps      ->SetValue(viewFps_def);
        MPrefs->CheckBox_CalculNormales  ->SetValue(CalculNormalesLectureBdd);
        if (test_seuil_gouraud) MPrefs   ->CheckBox_RecNormales_Seuillees->Enable();
        else                    MPrefs   ->CheckBox_RecNormales_Seuillees->Disable();
        MPrefs->CheckBox_RecNormales_Seuillees->SetValue(Enr_Normales_Seuillees);
        MPrefs->CheckBox_TraiterDoublonsAretes->SetValue(traiter_doublons_aretes);
        MPrefs->RadioBox_IconSize->SetSelection(icon_index);

        MPrefs->SpinCtrl_PasSvg ->SetValue(svg_time);

        // Reset du répertoire de travail par défaut = chemin de l'exécutable
        MPrefs->TextCtrl_WorkDir->SetLabel(wxWorkDir);
        MPrefs->SpinCtrl_Threads->SetValue(nb_threads);
    }
    if (MPosObs != nullptr) {
        ival = Convert_Rotx_LSI();
        MPosObs->SpinCtrl_LSI->SetValue(ival);

        ival = Convert_Rotz_LAZ();
        MPosObs->SpinCtrl_LAZ->SetValue(ival);
    }
    if (MZoomSpec != nullptr) {
        ival = Convert_Rotx_LSI();
        MZoomSpec->SpinCtrl_LSI->SetValue(ival);

        ival = Convert_Rotz_LAZ();
        MZoomSpec->SpinCtrl_LAZ->SetValue(ival);
        if (MZoomSpec->IsShown()) {
           str.Printf(_T("%4.3f"),m_gldata.zoom);
           MZoomSpec->TextCtrl_FoV->SetValue(str);
           str.Printf(_T("%4.3f"),m_gldata.fmult_diag);
           MZoomSpec->TextCtrl_Distance->SetValue(str);
        }
    }
    for (int i=0; i<4; i++) Light0Position[i] = Light0Position_def[i];
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
        MPanel->TextCtrl_Tolerance   ->SetValue(str);
        MPanel->CheckBox_Transparence->SetValue(false);
        MPanel->activer_transparence = false;
        MPanel->CheckBox_NotFlat     ->SetValue(NotFlat);
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
        MSelect->RadioButton_Selection_Facettes->SetValue(true);
        MSelect->RadioButton_TypeSelection_Both->SetValue(true);
    }
    mode_selection = selection_facette;
    TypeSelection  = Both;      // <=> 0
    glDisable(GL_CULL_FACE);    // Mode compatible de TypeSelection = Both

    m_gldata.initialized = false ;

// Initialisation pour l'affichage des Fps
    viewFps  = viewFps_def ;
    timebase = glutGet(GLUT_ELAPSED_TIME);
	frames   = 0 ;
	sprintf(Message_fps," ");

// Initialisation des couleurs des groupes/matériaux via MatAmbient_avionG
//    for (int i=0; i<nb_couleurs; i++) {
//        for (int j=0; j<3; j++) color_groupe_material[i][j] = MatAmbient_avionG[i][j];
//    }

//    Refresh();  // Ce Refresh ne sert sans doute à rien ... à vérifier !
    if(verbose) printf("Sortie de BddInter::ResetData\n");
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

    for (int i=0; i<4; i++) m_gldata.quat[i]=quat[i];

// Construire la matrice de rotation
    build_rotmatrix( matquat, quat );
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
//    sin_beta = -m[2];
    beta = asin(sin_beta)*to_Deg;                       // suffisant car -90 <= beta <= 90°
    cos_beta = sqrt(1.0f-sin_beta*sin_beta);            // cos_beta est donc toujours >= 0

    if (fabs(cos_beta) > 0.0005f) {                     // En fait <> 0. mais ici avec une certaine tolérance
        // Signes changés / article ... + au lieu de -
        alpha=atan2(-m[9],m[10])*to_Deg;                    // en fait -m[9]/cos_beta et m[10]/cos_beta), mais <=> car cos_beta > 0
        gamma=atan2(-m[4],m[0] )*to_Deg;                    // Idem
//        alpha=atan2(+m[6],m[10])*to_Deg;                 // en fait -m[6]/cos_beta et m[10]/cos_beta), mais <=> car cos_beta > 0
//        gamma=atan2(+m[1],m[0] )*to_Deg;                 // Idem
    } else {                                            // ici sin(beta) = 1 ou -1 (à epsilon près)
        beta =( sin_beta < 0.0f ? -90.0f : +90.0f);         // Forcer la valeur de beta
        alpha=0.0f;                                         // Cas dégénéré (Gimball lock => alpha et gamma sur le même axe)=> cos(alpha)=1.
        gamma=atan2( m[1],m[5])*to_Deg;                     // Signe changé / article ... - au lieu de +
//        gamma=atan2(-m[4],m[5])*to_Deg;
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

void BddInter::OnTimer_Bdd(wxTimerEvent& WXUNUSED(event))
{
// Pour tester une wxGauge dans la StatusBar du bas, mais pas très convainquant (le timer n'a pas l'air de se déclencher !). wxProgressDialog semble mieux
    int old_val = m_gauge->GetValue();
    long pos = fichierBdd.tellg();
    int new_val = (pos*100./fichierBdd_length) +0.5;
    printf("%d %d\n",old_val,new_val);
    if (old_val < 100) {
        if (old_val != new_val) {
            m_gauge->SetValue(new_val);
            m_gauge->Update();
        }
//        m_gauge->Update();                              // Non indispensable mais peut donner un affichage plus fluide
//        wxLogStatus(_("i %d %d"),i,m_gauge->GetValue());
    } else {                        // On est arrivé au max
        m_timer->Stop();
        if (timer_bis) {            // La première fois, timer_bis est false
            wxDELETE(m_gauge);
            wxDELETE(m_timer);
        } else {
            timer_bis = true;       // Pour un second passage
            m_timer->Start(1500);   // Démarrer un second timer pour laisser un peu de temps à l'affichage des wxGauge arrivés au max
        }
    }
}

bool BddInter::Update_Dialog(long position, long max_value, bool cancel)
{
// Si cancel est true, affiche un bouton "Cancel" puis
// retourne true si le bouton Cancel a été activé. Sinon, retourne false.
// Le paramêtre "cancel" peut être omis. Dans ce cas, il est pris par défaut à false.

    if (!progress_dialog_en_cours) {
        if ((clock() - time_deb_dialog) > Dialog_Delay) {   // Pour temporiser le début d'affichage du ProgressDialog

// un wxGenericProgressDialog est OK en mode Darkmode alors qu'un wxProgressDialog reste avec les couleurs standard de Windows !

            int style = wxPD_AUTO_HIDE;
            if (cancel) style = wxPD_CAN_ABORT | wxPD_AUTO_HIDE;

//                                          wxPD_CAN_ABORT |
//                                          wxPD_APP_MODAL |
//                                          wxPD_ELAPSED_TIME |
//                                          wxPD_ESTIMATED_TIME |
//                                          wxPD_REMAINING_TIME |
//                                          wxPD_AUTO_HIDE

            progress_dialog = new wxGenericProgressDialog(Dialog_Titre,
                                          Dialog_Comment,   // Pour afficher le nom du fichier
                                          100,              // range
                                          this,             // parent
                                          style
                                         );
//            if (style | wxPD_CAN_ABORT) progress_dialog->m_btnAbort->SetLabel("Interrompre"); // Ne marche pas car m_btnAbort a été déclaré private
            progress_dialog->Update(0);   // Par précaution
            progress_dialog_en_cours = true;
            int new_val = round(position*100./max_value);
            progress_dialog->Update(new_val);
            if (progress_dialog->WasCancelled()) return true;       // Abandonner si détection de pression sur le bouton "Cancel"
        }
    } else {
        int old_val = progress_dialog->GetValue();
        int new_val = round(position*100./max_value);
        if (old_val != new_val) {
            if (new_val >= 100) {
                progress_dialog->Update(99); // Pour se donner une "petite" chance de voir "presque" la fin. Mais ne semble pas très efficace !
            } else {
                progress_dialog->Update(new_val);
                if (progress_dialog->WasCancelled()) return true;   // Abandonner si détection de pression sur le bouton "Cancel"
            }
        }
    }
    return false;
}

void BddInter::Fermer_progress_dialog()
{
    if (progress_dialog_en_cours) {
        progress_dialog->Update(100);
        wxDELETE(progress_dialog);
        progress_dialog_en_cours = false;    // Par précaution
    }
}

void BddInter::make_objet() {
    wxStringTokenizer tokenizer(str);
    wxStringlist.clear();
    while ( tokenizer.HasMoreTokens() ) {
        token = tokenizer.GetNextToken();
        this->wxStringlist.push_back(this->token);      // Pb : si le nom comporte des espaces, c'est plusieurs token
    }
    std::cout << '\n' << "Objet ";
    wxString Nom_Objet;
    unsigned int last = this->wxStringlist.size()-1;
    for (unsigned int i=1; i<=last; i++) {
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
    indiceObjet_courant = this->Objetlist.size() -1;    // plus judicieux à faire ici plutôt qu'après des make_objet ? A vérifier
}

void BddInter::make_face() {
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
    printf("%8d facettes\t\t(<FACE>/<POLYGON>)\n",Nb_facettes);
    this->Objetlist[indiceObjet_courant].Nb_facettes = Nb_facettes;  // Stocker le nombre de facettes dans l'objet
    this->Objetlist[indiceObjet_courant].Facelist.resize(Nb_facettes);
}

void BddInter::make_1_face() {
    int Numero;
    Face New1Face;
    Object *Objet_courant;

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
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    if (Numero > (int)Objet_courant->Facelist.size()) {
        Objet_courant->Facelist.push_back(New1Face) ; // Taille initiale trop petite (on est en création de facettes) => procéder par push_back !
        Objet_courant->Nb_facettes++;
    }
    else
        Objet_courant->Facelist[Numero-1] = New1Face;
}

//void BddInter::make_1_face(int Numero, const std::vector<int> &Numeros_Sommets) {
//// Cette version ne semble pas être vue par le compilateur !
//    Face New1Face;
//
//    New1Face = Face(Numero, Numeros_Sommets);
//
//    if (Numero > (int)this->Objetlist[indiceObjet_courant].Facelist.size()) {
//        this->Objetlist[indiceObjet_courant].Facelist.push_back(New1Face) ; // Taille initiale trop petite (on est en création de facettes) => procéder par push_back !
//        this->Objetlist[indiceObjet_courant].Nb_facettes++;
//    }
//    else
//        this->Objetlist[indiceObjet_courant].Facelist[Numero-1] = New1Face;
//}

void BddInter::make_sommet() {
// Dans cette version, on dimensionne la liste Sommetlist et on initialise Nb_sommets dans Objetlist[indiceObjet_courant].
    int Nb_sommets;
    Object *Objet_courant;

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
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);
    printf("%8d sommets\t\t(<SOMMET>/<VERTEX>)\n",Nb_sommets);
    Objet_courant->Nb_sommets = Nb_sommets;  // Stocker le nombre de sommets dans l'objet
    Objet_courant->Sommetlist.resize(Nb_sommets);
}

void BddInter::make_1_sommet() {
// <=> make_1_vecteur
// Si la liste est de taille suffisante, on utilise directement le Numero via les token ou N_Element
// sinon, on incrémente la taille de Sommetlist
    int Numero;
    Sommet New1Sommet;
    Object *Objet_courant;

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
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);
    if (Numero > (int)Objet_courant->Sommetlist.size()) {
        Objet_courant->Sommetlist.push_back(New1Sommet) ; // Taille initiale trop petite (on ajoute des points) => procéder par push_back !
        Objet_courant->Nb_sommets++;
    }
    else
        Objet_courant->Sommetlist[Numero-1] = New1Sommet;
}

void BddInter::make_normale() {
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
    printf("%8d normales aux facettes\t(<NORMALE>/<POLY_NORMAL>)\n",Nb_normales);

    this->Objetlist[indiceObjet_courant].Nb_normales = Nb_normales;
}

void BddInter::make_1_normale() {
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
//    for (int i=0; i<3; i++) printf("%f ",this->Objetlist[indiceObjet_courant].Facelist[Numero-1].normale_b[i]);
//    printf("\n");
}

void BddInter::make_aspect_face() {
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
    printf("%8d attributs aux facettes\t(<ASPECT_FACE>/<POLY_ATTR>)\n",Nb_aspect);
    this->Objetlist[indiceObjet_courant].Nb_aspects = Nb_aspect;
}

void BddInter::make_1_aspect_face() {
    int indice;
    int val;
    int grp_mat;
    Object *Objet_courant;

// Pour l'instant ne gère pas str.IsEmpty() comme les autres fonctions make_1_* !
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
        Objet_courant = &(this->Objetlist[indiceObjet_courant]);
        if (n_tokens == 3) {                                                    // Une ligne peut contenir à ce niveau 2 ou 3 champs (ou tokens)
            Aspect_face New1aspect_face(wxStringlist);                          // S'il y en a 3, il faut créer un nouveau aspect_face1
            indice = this->indiceAspect = wxAtoi(wxStringlist[0]) -1;           // Sauvegarder indice dans indiceAspect pour usage ultérieur
            Objet_courant->Facelist[indice].groupe = 0;
            Objet_courant->Facelist[indice].codmatface = codmatface_def;
            New_aspect_face = true;
        } else {
            indice = this->indiceAspect;
            New_aspect_face = false;
        }
        val = wxAtoi(token);                                                    // Récupère le numéro de groupe ou de matériau sous forme d'entier
        if (grp_mat == 1) {
            Objet_courant->Facelist[indice].groupe = val;
//            auto it = std::find(listeGroupes.begin(),listeGroupes.end(),val);   // Est-il déjà dans la liste ?
//            if (it == listeGroupes.end() || listeGroupes.empty()) {                                         // Non
//                listeGroupes.push_front(val);                                   // L'ajouter à la liste des groupes
//                listeGroupes.sort();                                            // Trier la liste (on pourrait le faire plus tard !)
//            }
        } else if (grp_mat == 2) {
            Objet_courant->Facelist[indice].codmatface = val;
//            auto it = std::find(listeMateriaux.begin(),listeMateriaux.end(),val);   // Est-il déjà dans la liste ?
//            if (it == listeMateriaux.end() || listeMateriaux.empty()) {                                       // Non
//                listeMateriaux.push_front(val);                                     // L'ajouter à la liste des matériaux
//                listeMateriaux.sort();
//            }
        }
//    }

    wxStringlist.clear();
}

void BddInter::make_luminance() {

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
    printf("%8d valeurs de Luminances\t(<LUMINANCE>)\n",Nb_luminances);

    this->Objetlist[indiceObjet_courant].Nb_luminances = Nb_luminances;
}

void BddInter::make_1_luminance() {
// ici reste avec des push_back, pas comme dans make_1_face ... peut-être à revoir !
    int Numero;
    Luminance New1luminance;
    Object *Objet_courant;

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

    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    Objet_courant->Facelist[Numero-1].Nb_Sommets_L = New1luminance.Nb_Sommets_L;
    Objet_courant->Facelist[Numero-1].L_sommets.clear();  // Forcer un clear ici car peut avoir été déjà initialisé ailleurs => push_back ne ferait pas ce qu'il faut !
    for (int i=0; i<New1luminance.Nb_Sommets_L; i++) {
        Objet_courant->Facelist[Numero-1].L_sommets.push_back(New1luminance.L_sommets[i]);
    }
}

void BddInter::make_1_luminance(int Numero, const std::vector<int> &Numeros_Sommets) {
// ici reste avec des push_back, pas comme dans make_1_face ... peut-être à revoir !
    Luminance New1luminance;
    Object *Objet_courant;

    New1luminance = Luminance(Numero, Numeros_Sommets) ;
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    Objet_courant->Facelist[Numero-1].Nb_Sommets_L = New1luminance.Nb_Sommets_L;
    Objet_courant->Facelist[Numero-1].L_sommets.clear();  // Forcer un clear ici car peut avoir été déjà initialisé ailleurs => push_back ne ferait pas ce qu'il faut !
    for (int i=0; i<New1luminance.Nb_Sommets_L; i++) {
        Objet_courant->Facelist[Numero-1].L_sommets.push_back(New1luminance.L_sommets[i]);
    }
}

void BddInter::make_vecteur() {
// Dans cette version, on dimensionne la liste Vecteurlist et on initialise Nb_vecteurs dans Objetlist[indiceObjet_courant].
    int Nb_vecteurs;
//    Vecteur Newvecteur;
    Object *Objet_courant;

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
    printf("%8d normales aux sommets\t(<VECTEUR>/<VERT_NORMAL>)\n",Nb_vecteurs);
    wxStringlist.clear();

    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    Objet_courant->Nb_vecteurs = Nb_vecteurs;
    Objet_courant->Vecteurlist.resize(Nb_vecteurs);
}

void BddInter::make_1_vecteur() {
// <=> make_1_sommet
// Si la liste est de taille suffisante, on utilise directement le Numero via les token ou N_Element
// sinon, on incrémente la taille de Vecteurlist
    int Numero;
    Vecteur New1Vecteur;
    Object *Objet_courant;

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
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    if (Numero > (int)Objet_courant->Vecteurlist.size()) {
        Objet_courant->Vecteurlist.push_back(New1Vecteur) ; // Taille initiale trop petite (on ajoute des points) => procéder par push_back !
        Objet_courant->Nb_vecteurs++;
    }
    else
        Objet_courant->Vecteurlist[Numero-1] = New1Vecteur;
}

void BddInter::make_position() {
    Object *Objet_courant;

    wxStringTokenizer tokenizer(str);
    while ( tokenizer.HasMoreTokens() ) {
        token = tokenizer.GetNextToken();
        wxStringlist.push_back(token);
    }
//    Position Newposition(token);
    printf("%8d matrice Position\t(<POSITION>/<PLACEMENT>)\n",wxAtoi(token));
    wxStringlist.clear();
//    this->positionlist.push_back(Newposition);
    Objet_courant = &(this->Objetlist[indiceObjet_courant]);

    Objet_courant->mat_position= true;            // il y a une matrice POSITION dans la BDD pour l'objet courant
    Objet_courant->Nb_matrices = wxAtoi(token);   // Nombre de transformations (normalement 1 seule si <MAT_POSITION>
}

void BddInter::Genere_Tableau_Points_Facettes(Object * objet)
{
    unsigned int nb_p, nb_fac;
    unsigned int i, j;
    unsigned int indice_sommet;
    Face    *Face_i;

    if (verbose) printf("Entree BddInter::Genere_Tableau_Points_Facettes\n");
    nb_p   = objet->Sommetlist.size();  //Nb_sommets;
    nb_fac = objet->Facelist.size();    //Nb_facettes;
    objet->Pointslist.clear();
    objet->Pointslist.resize(nb_p);
#pragma omp parallel for private(Face_i,j,indice_sommet)                    // Boucle for en parallèle
    for (i=0; i<nb_fac; i++) {
        Face_i = &(objet->Facelist[i]);
        if (Face_i->deleted) continue;
        for (j=0; j<Face_i->F_sommets.size(); j++) {
            indice_sommet = Face_i->F_sommets[j] -1;                        // <=> numero_sommet -1
#pragma omp critical                                                        // instruction suivante critique => une seule execution à la fois tous threads confondus
            objet->Pointslist[indice_sommet].IndicesFacettes.push_back(i);  // numero_sommet-1 = indice du sommet, i=indice / i+1 = numéro de la facette
        }
    }
    if (verbose) printf("Sortie BddInter::Genere_Tableau_Points_Facettes\n");
}

void BddInter::Genere_Tableau_Aretes(Object * objet)
{
    // NOTE : On pourrait chronométrer la partie test des doublons d'arêtes puis, si c'est suffisament court, activer Genere_Tableau_Aretes systématiquement quand c'est utile
    //        plutôt que de laisser ce soin à faire en manuel par l'utilisateur via le bouton "Recalculer les arêtes"

    unsigned int i,nb_fac,j,j_b,nb_p;
    unsigned int indice_sommet;
    unsigned int Numero_Objet;
    unsigned int Nb_avant, Nb_apres, Nb_deleted;
    unsigned int indice_a, indice_b;
    std::vector<int>  Numeros_Sommets;
    Aretes   Arete, Arete_test, *p_Arete;
    bool verbose_local = true;      // Local ici
    bool print_en_cours;    // un peu le même usage que progress_dialog_en_cours qui pourrait être suffisant
    bool cancelled;

    clock_t time_0, delta_time, time_c;
    #define NB_DELTA_TICKS 2*CLOCKS_PER_SEC         // Pour obtenir un affichage de points de progression toutes les 2 secondes

// Cette façon de faire (1ère partie) est simple, mais les arêtes communes à 2 facettes sont comptées 2 fois !
// La seconde partie permet de décoder les doublons et de les éliminer

    if (verbose) printf("Entree BddInter::Genere_Tableau_Aretes\n");
    progress_dialog_en_cours = print_en_cours = cancelled = false;

//    indice_point = 0;
    nb_fac = objet->Nb_facettes;
    objet->Areteslist.clear();
#pragma omp parallel for default(shared) private(Numeros_Sommets,nb_p,j,Arete,indice_a,indice_b,j_b)
    for (i=0; i< nb_fac; i++) {
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
#pragma omp critical
            objet->Areteslist.push_back(Arete);
        }
    }
    Nb_avant = objet->Nb_aretes = objet->Areteslist.size();
    bool traiter_doublons_aretes_svg = traiter_doublons_aretes;
    if (Nb_avant >= nb_aretes_test_d) traiter_doublons_aretes = false;  // Arete_size_test
//    else                              traiter_doublons_aretes = true;   // avec cette ligne, revalide la possibilité de traitement à chaque objet, sinon abandon dès qu'un objet déborde
    if (!traiter_doublons_aretes && !forcer_simplification_doublons_aretes) {
        if (verbose_local) {
            printf("Objet : %s\nNb_aretes : %d\nSimplification des doublons d'arêtes non effectuée\n",objet->GetName(),Nb_avant);
            if (verbose) printf("Sortie BddInter::Genere_Tableau_Aretes\n");
        }
        traiter_doublons_aretes = traiter_doublons_aretes_svg;
        return;
    }
    if (verbose_local) sprintf(Message,"Objet : %s\nNb_aretes avant : %d\n",objet->GetName(),Nb_avant);
    Nb_deleted = 0;

// Boucles imbriquées qui peuvent être longues à l'exécution sur de grosses bdd ( >10' sur ellipsoide_260000.bdd ou Millenium_Falcon !).

// Notes : on a stocké avec ind_a < ind_b (car l'ordre n'a pas d'importance) et du coup le test ci-dessous est + simple. On peut même tester
//         d'abord l'égalité sur le 1er indice, puis celui sur le second => moins de test => plus rapide !
//         On peut aussi arrêter de tester les indices et sortir de la boucle en j, dès que 2 correspondances ont été trouvées car dans la majorité des
//         cas une arête n'est utilisée que 2 fois au plus car commune à 2 facettes.

    time_0 = time_c = time_deb_dialog = clock();
// boucle for suivante probablement non parallélisable car les 2 boucles imbriquées travaillent sur la même Areteslist qui change quand un doublon est trouvé.
// de plus, le break a beaucoup moins d'intérêt car les threads qui n'y passent pas continuent jusqu'à la fin => perte de temps globale.
// Enfin, comme Areteslist est passé de <vector> à <list> plus de parallélisation possible !
    unsigned int nbaretes = objet->Areteslist.size();
    std::list<Aretes>::iterator itArete, itArete2;
    itArete = objet->Areteslist.begin();
    for (i=0; i<nbaretes; i++,itArete++) {
//        Arete_test = objet->Areteslist[i]; // Si Aretelist est un vector, sinon à supprimer
        Arete_test = *itArete;
        if (Arete_test.deleted) continue;   // Arête déjà supprimée. Passer à la suivante ... test sans doute inutile car ça n'arrive jamais tel que c'est construit !
        indice_a = Arete_test.ind_a;
        indice_b = Arete_test.ind_b;
        itArete2 = itArete;
        ++itArete2; // Se positionner sur l'arête suivante (<=> i+1)
////#pragma omp parallel for private(p_Arete) // break incompatible avec la boucle for dans openmp
        for (j=i+1; j<nbaretes; j++,itArete2++) {
//        for (j=i+1; j<objet->Areteslist.size(); j++) {    // Si Aretelist est un vector, sinon à supprimer
//            p_Arete = &(objet->Areteslist[j]);  // Ici, il vaut mieux travailler sur un pointeur, surtout avec la méthode 2 ci-dessous, sinon on ne modifie qu'une copie !
            p_Arete = &(*itArete2);             // Ici, il vaut mieux travailler sur un pointeur, surtout avec la méthode 2 ci-dessous, sinon on ne modifie qu'une copie !
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
//                    objet->Areteslist.erase(objet->Areteslist.begin()+j);   // Supprimer l'arête en doublon
                    objet->Areteslist.erase(itArete2);   // Supprimer l'arête en doublon
//                    j-- ; // poursuivre la boucle for avec le même indice j car ils ont été décalés d'une unité par le erase de la ligne ci-dessus
                    nbaretes--; // changer la borne max en j (fait automatiquement si on laisse objet->Areteslist.size() )
                    break; // doublon trouvé => abandonner le for j et passer au i suivant ! => ignore des arêtes en triple !
                }
            }
//            indice_point++;   // Pour ajouter éventuellement un indicateur de progression (un . affiché toutes les indice_test valeurs testées)
                                // L'affichage dépend du temps d'exécution => plutôt utiliser un timer et donc n'afficher un '.' que toutes les x secondes
            delta_time = clock() - time_c;
            if (delta_time >= NB_DELTA_TICKS && verbose_local) {
                if (!print_en_cours) {                                          // Seulement la 1ère fois
                    print_en_cours = true;
//                    printf("%s",Message);
//                    printf("Detection d'aretes en doublon ");
                    printf("%s",utf8_To_ibm(Message));                          // Afficher d'office Nb_avant (quelle que soit la future valeur de Nb_apres)
                    sprintf(Message,"Détection d'arêtes en doublon ");          // et le début de l'indicateur de progression
                    printf("%s",utf8_To_ibm(Message));
                    Dialog_Titre    = wxS("Détection d'arêtes en doublon");
                    Dialog_Comment  = wxS("Objet : ")+objet->GetwxName();
                    Dialog_Delay    = NB_DELTA_TICKS;                           // Ce sera forcément le cas car test sur delta_time la première fois
                    progress_dialog_en_cours = false;
                    cancelled = Update_Dialog((long)i, (long)nbaretes, true);   // true pour afficher le bouton "Cancel"
                }
                printf(".");            // On pourrait aussi afficher successivement | / - \ avec des backspaces.
                time_c = clock();
                if (cancelled) break;   // Pour sortir de la boucle interne en j. Plutôt bizzare ici, mais il y a des fois où ça repart !
            }
        }
        if(cancelled) break;            // Pour sortir aussi de la boucle en i
        if(progress_dialog_en_cours) cancelled = Update_Dialog((long)i, (long)nbaretes, true); // Placé ici, mais l'effet n'est pas immédiat car il faut d'abord terminer le for en j.
        if(cancelled) break;            // Pour sortir immédiatement du for
    }

    Fermer_progress_dialog();

    Nb_apres = objet->Nb_aretes = objet->Areteslist.size();
    if (Nb_deleted != 0) {
        Nb_apres -= Nb_deleted; // Ne sert qu'en méthode 1 ! Mais la taille de la liste d'arêtes ne change pas car les arêtes à supprimer sont marquées deleted = true !
    }
    // On n'affiche que si verbose est true et Nb_avant != Nb_après, sauf si "en_cours" est true car Nb_avant est déjà à l'écran avec l'indication de progression.
    if (verbose_local) {
//        if (!en_cours && (Nb_avant != Nb_apres)) printf("%s",Message);
        if (!print_en_cours && (Nb_avant != Nb_apres)) printf("%s",utf8_To_ibm(Message));
        if (print_en_cours) printf("\n");
        if (print_en_cours  || (Nb_avant != Nb_apres)) {
//            printf("Nb_aretes apres : %d\n",Nb_apres);
            sprintf(Message,"Nb_aretes après : %d\n",Nb_apres);
            printf("%s",utf8_To_ibm(Message));
        }
    }

    delta_time = clock() - time_0;
    objet->Temps_Calcul_Aretes = delta_time;
    if (delta_time >= CLOCKS_PER_SEC) {     // N'afficher ce temps que si >= 1 seconde
        printf("Temps de filtrage des doublons : %ld ticks (soit %3.1f secondes)\n\n", delta_time, float(delta_time)/CLOCKS_PER_SEC);
    }
    progress_dialog_en_cours = false;
    print_en_cours = false;
    traiter_doublons_aretes  = traiter_doublons_aretes_svg;;          // Remettre à l'état initial
    if (verbose) printf("Sortie BddInter::Genere_Tableau_Aretes\n");
}

void BddInter::Search_Min_Max() {
    std::vector<float>Point_XYZ;
    std::vector<int>numSommets;
    unsigned int i=0,j=0,k=0;
    Object * objet_courant;

    if (verbose) printf("Entree BddInter::Search_Min_Max\n");

    Reset_Min_Max();
    nb_objets = this->Objetlist.size();
    nb_objets_reels = nb_objets;
    nb_3points=nb_4points=nb_aretes=nb_facettes=nb_sommets=nb_sommets_max=numero_facette_max=numero_objet_max=0 ;
    for (i=0; i<nb_objets; i++) {
        objet_courant = &(this->Objetlist[i]);
        if (objet_courant->deleted) {
            nb_objets_reels--;
        } else {
            nb_facettes += objet_courant->Facelist.size();              // On pourrait utiliser objet_courant->Nb_facettes
            nb_sommets  += objet_courant->Sommetlist.size();            // idem Nb_sommets
            nb_aretes   += objet_courant->Areteslist.size();            // idem Nb_aretes
            unsigned int nb_fac = objet_courant->Facelist.size();
#pragma omp parallel for private(numSommets,Point_XYZ)
            for (j=0; j<nb_fac; j++) {
                if (objet_courant->Facelist[j].deleted) nb_facettes-- ; // Décompter les facettes supprimées
                if (objet_courant->Facelist[j].afficher && !objet_courant->Facelist[j].deleted) {
                    numSommets.clear();
                    numSommets = objet_courant->Facelist[j].getF_sommets();
                    unsigned int nombreSommets = numSommets.size();
//                    nb_sommets += nombreSommets ;   // Non car chaque point est compté plusieurs fois (1 par facette)
#pragma omp critical
{
                    if (nombreSommets == 3) nb_3points += 1 ;
                    else                    nb_4points += 1 ;

                    if (nombreSommets > nb_sommets_max) {
                        nb_sommets_max     = nombreSommets ;
                        numero_objet_max   = i ;
                        numero_facette_max = j ;
                    }
}
                   for (k=0; k<nombreSommets; k++) {
                        Point_XYZ = objet_courant->Sommetlist[numSommets[k]-1].getPoint();
#pragma omp critical
{
                        Set_Min_Max(Point_XYZ[0],Point_XYZ[1],Point_XYZ[2]);
}
                    }
                }
            }
        }
    }
    if (x_min == +FLT_MAX) {
        // min et max non initialisés correctement (valeurs de Reset_Min_Max() non retouchées !) : on ne teste que x_max, ça devrait être suffisant.
        x_min = x_max = y_min = y_max = z_min = z_max = 0.0;
    }
    diagonale_save = Norme3(x_max-x_min, y_max-y_min, z_max-z_min) ;
    centre_auto[0] = (x_max+x_min)*0.5 ;    // Centre de la boite englobante
    centre_auto[1] = (y_max+y_min)*0.5 ;
    centre_auto[2] = (z_max+z_min)*0.5 ;
    if (centrageRotAuto) centreRot = centre_auto ;

    if (verbose) printf("Sortie BddInter::Search_Min_Max\n");
}

float BddInter::Produit_Scalaire(const std::vector<float> &v1, const std::vector<float> &v2){
    float cross = v1[0]*v2[0] +v1[1]*v2[1] +v1[2]*v2[2];
//    cross = cross/(v1[0]*v1[0] +v1[1]*v1[1] +v1[2]*v1[2])/(v2[0]*v2[0] +v2[1]*v2[1] +v2[2]*v2[2]); // Serait à faire si v1 et/ou v2 non normalisés à 1
    return cross;
}

void BddInter::Flat_Selected_Facettes() {
// Force le mode "Facettes Planes" des facettes sélectionnées
    unsigned int i,j;
    Face *Face_ij;

    for (i=0; i<this->Objetlist.size(); i++) {
#pragma omp parallel for private(Face_ij)
        for (j=0; j<this->Objetlist[i].Facelist.size(); j++) {
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

    for (i=0; i<this->Objetlist.size(); i++) {
#pragma omp parallel for private(Face_ij)
        for (j=0; j<this->Objetlist[i].Facelist.size(); j++) {
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
        for (i=0; i<this->ToSelect.ListeSelect.size(); i++) {

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
                    for (k=1; k<(Face_ij->F_sommets.size()/2); k++) {
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

//void BddInter::DisplayMessage(wxString wxMessage, bool bip)   // Déplacé dans OvniMain.cpp en dehors de OvniFrame

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
        for (i=0; i<this->Objetlist.size(); i++) {
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
    unsigned int nb_fac = this->Objetlist[i].Facelist.size();
#pragma omp parallel for private(Face_ij,NumerosSommets,ReverseSommets,k)
    for (j=0; j<nb_fac; j++) {
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
    for (o=0; o<this->Objetlist.size(); o++) Inverser_les_Normales_Objet(o);
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
    unsigned int nb_fac = objet_courant->Nb_facettes;
#pragma omp parallel for private(Face_ij,k,m)
    for (j=0; j<nb_fac; j++) {
        Face_ij = &(objet_courant->Facelist[j]);
        if(Face_ij->afficher && !Face_ij->deleted) {
            Face_ij->normale_b[0] *= -1;                                    // Inverse le signe de la normale
            Face_ij->normale_b[1] *= -1;
            Face_ij->normale_b[2] *= -1;
            int base = Face_ij->F_sommets[0];
            Face_ij->F_sommets[0] = Face_ij->F_sommets[Face_ij->F_sommets.size()-1];    // Puis le sens de parcours des points
            for (k=1; k<(Face_ij->F_sommets.size()/2); k++) {
                m = Face_ij->F_sommets[k];
                Face_ij->F_sommets[k] = Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1];
                Face_ij->F_sommets[Face_ij->F_sommets.size()-k-1] = m;
            }
            Face_ij->F_sommets[Face_ij->F_sommets.size()-1]=base;
        }
    }

// Inversion du signe des normales aux sommets (vecteurs)
    unsigned int nb_vec = objet_courant->Nb_vecteurs;
#pragma omp parallel for private(Vecteur_ij)
    for (j=0; j<nb_vec; j++) {
        Vecteur_ij = &(objet_courant->Vecteurlist[j]);
        if (!Vecteur_ij->point.empty()) {
            Vecteur_ij->point[0] *= -1;
            Vecteur_ij->point[1] *= -1;
            Vecteur_ij->point[2] *= -1;
        }
    }

//    for (j=0; j<objet_courant->Nb_luminances; j++) {   // Souci si différent du nombre de facettes !
#pragma omp parallel for private(Luminance_ij,k,m)
    for (j=0; j<nb_fac; j++) {
        Luminance_ij = &(objet_courant->Facelist[j]);
        if (!Luminance_ij->L_sommets.empty()) {
            int base = Luminance_ij->L_sommets[0];
            Luminance_ij->L_sommets[0] = Luminance_ij->L_sommets[Luminance_ij->L_sommets.size()-1];    // Puis le sens de parcours des points
            for (k=1; k<(Luminance_ij->L_sommets.size()/2); k++) {
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

void BddInter::BuildAllFacettes_Selected() {

    // Repris en grande partie de DrawOpenGL

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
        printf("Entree BddInter::BuildAllFacettes_Selected\n");

    glDeleteLists(glliste_select,1);                                // Détruire une éventuelle liste existante de facettes sélectionnées
    if (this->ToSelect.ListeSelect.empty()) return;                 // Rien de plus à faire

    glNewList(glliste_select, GL_COMPILE_AND_EXECUTE) ;             // Création d'une liste de facettes sélectionnées : GL_COMPILE_AND_EXECUTE indispensable

    if (verbose)
        printf("Reconstruction de la liste des facettes sélectionnées\n");

//    int nb_normales_seuillees = 0; // A déclarer plutôt ailleurs, mais initialiser ici

    for (i=0; i<this->ToSelect.ListeSelect.size(); i++) {

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
                coloriserFacette(objet, numero, true, vert);

                NormaleFacette = Face_ij->getNormale_b();

                switch(Face_ij->Nb_Sommets_F) {
                case 2:                     // Lignes simples ou facette dégénérées réduites à 2 points !
                    glBegin(GL_LINES);
                    NumerosSommets = Face_ij->getF_sommets();
                    for (k=0; k<NumerosSommets.size(); k++) {
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
                    for (k=0; k<NumerosSommets.size(); k++) {
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

    }       // for (i=0; i<this->ToSelect.ListeSelect.size() ...

    glEndList();
}

void BddInter::Reset_Min_Max() {
    x_min = +FLT_MAX;
    x_max = -FLT_MAX;
    y_min = +FLT_MAX;
    y_max = -FLT_MAX;
    z_min = +FLT_MAX;
    z_max = -FLT_MAX;
}

void BddInter::Set_Min_Max(float x, float y, float z) {
    x_min = ( x<x_min ? x : x_min);
    y_min = ( y<y_min ? y : y_min);
    z_min = ( z<z_min ? z : z_min);
    x_max = ( x>x_max ? x : x_max);
    y_max = ( y>y_max ? y : y_max);
    z_max = ( z>z_max ? z : z_max);
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

void BddInter::Diviser_Arete(int objet, int face, int line) {
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

//    indice_a = objet_cible->Areteslist[line].ind_a;
//    indice_b = objet_cible->Areteslist[line].ind_b;
    std::list<Aretes>::iterator itArete = objet_cible->Areteslist.begin();
    std::advance(itArete,line);
    indice_a = (*itArete).ind_a;
    indice_b = (*itArete).ind_b;
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
    for (l=1; l<div_limit; l++) {
        NewSommet = Sommet_a;
        if (MPanel->division != -1) t = (float)l/div_limit;
        for (k=0; k<NewSommet.point.size(); k++) {
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
        for (j=0; j<limit; j++) {
            NumerosSommets = objet_cible->Facelist[j].getF_sommets();
            unsigned int k_fin = NumerosSommets.size();
            unsigned int k_suiv;
            for (k=0; k<k_fin; k++) {
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
            //// entre 1er et dernier point ?
        }
//        }
        numero_a = nouveau_sommet;
    }
    undo_memory++;
    MPanel->Button_Undo->Enable();

    buildAllPoints();
    Genere_Tableau_Points_Facettes(objet_cible);
    Genere_Tableau_Aretes(objet_cible);
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
////            Luminance NewLuminance;                                                            // Non utilisé ? pour l'instant
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

        for (j=0; j<nb_Fac_initial; j++) {
            if(objet_origine->Facelist[j].deleted) continue ;               // Passer à la facette suivante
            toUp1 = -1;
            toUp2 = -1;
            NumerosSommets = objet_origine->Facelist[j].getF_sommets();
            for (k=0; k<(int)NumerosSommets.size(); k++) {
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

        for (j=0; j<nb_Fac_initial; j++) {
        if(objet_origine->Facelist[j].deleted) continue ;                   // Passer à la facette suivante
//            toUp1 = -1;
            toUp2 = -1;
            NumerosSommets = objet_origine->Facelist[j].getF_sommets();
            for (k=0; k<(int)NumerosSommets.size(); k++) {
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
        Genere_Tableau_Points_Facettes(objet_origine);                  // Faut-il le faire à chaque soudure ou une seule fois en sortie de ModificationPanel
        Genere_Tableau_Aretes(objet_origine);                           // Car peut-être long si beaucoup de facettes à cause du test des doublons d'arêtes
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
    Face  * face_j;
    Sommet* sommet_j;

    if(undo_memory != 0) {
        for (i=0; i<this->Objetlist.size(); i++) {   // est-il nécessaire de balayer tous les objets, pas seulement ceux dans undo_memory ?
            objet_courant = &(this->Objetlist[i]);
            for (j=0; j<objet_courant->Facelist.size(); j++) {
                face_j = &(objet_courant->Facelist[j]);
                if (face_j->toshow == undo_memory) {
                    objet_courant->Facelist.erase(this->Objetlist[i].Facelist.begin()+j);
                    j--;    // 1 facette supprimée => passer une fois de moins dans la boucle en j
                } else if(face_j->toshow  == (-undo_memory)) {
                    face_j->deleted       = false;
                    face_j->toshow        = 0;
                } else if((face_j->toshow == undo_memory-1) && (undo_memory != 1)) {     // A vérifier : si undo_memory = 1,
                    face_j->deleted       = false;                                       // semble faire le undelete sur 1 facette de trop
                }
            }
            bool vecteurs_presents = true;
            if (objet_courant->Vecteurlist.size() == 0) vecteurs_presents = false;
            for (j=0; j<objet_courant->Sommetlist.size(); j++) {
                sommet_j = &(objet_courant->Sommetlist[j]);
                if (sommet_j->toshow == undo_memory) {
                    objet_courant->Sommetlist.erase(objet_courant->Sommetlist.begin()+j);
                    if (vecteurs_presents) objet_courant->Vecteurlist.erase(objet_courant->Vecteurlist.begin()+j);
                    j--;    // 1 sommet effacé => passer une fois de moins dans la boucle en j
                } else if(sommet_j->toshow == -undo_memory) {
                    sommet_j->show     = true;
                    sommet_j->toshow   = 0;
                    sommet_j->selected = false;
                    if (vecteurs_presents) {
//                        objet_courant->Vecteurlist[j].show   = true;
                        objet_courant->Vecteurlist[j].toshow = 0;
                    }
                } else if((sommet_j->toshow == undo_memory-1) && (undo_memory != 1)) {   // Idem facettes
                    sommet_j->show     = true;
                    sommet_j->selected = false;
//                    if (vecteurs_presents) objet_courant->Vecteurlist[j].show   = true;
                }
            }
            objet_courant->Nb_facettes = objet_courant->Facelist.size();    // Remettre à jour les Nb_*
            objet_courant->Nb_sommets  = objet_courant->Sommetlist.size();
            objet_courant->Nb_vecteurs = objet_courant->Vecteurlist.size();

            if (objet_courant->Temps_Calcul_Aretes <= tempo_s*CLOCKS_PER_SEC) {
                Genere_Tableau_Points_Facettes(objet_courant);                // Faut-il le faire à chaque soudure ou une seule fois en sortie de ModificationPanel
                Genere_Tableau_Aretes(objet_courant);                         // Car peut-être long si beaucoup de facettes à cause du test des doublons d'arêtes
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
        m_gllist = 0;           // Utile ? => reconstruire toutes les listes !
    }
    Refresh();
}

bool BddInter::Calcul_Normale_Seuillee(int indice_objet_cur, int ind_fac, int ind_P, std::vector<float> &NormaleFacette, std::vector<float> &NormaleSommet)
{
    bool test_np ;
    int npoint, i, k, k2, nbs, nfac ;
    double  coef ;
    std::vector<float> pa, pb, pc;
    Vector3D v1, v2, vp, np;
    Object *objet_cur;
    Points *p_Point;
    Face cur_fac ;
    int  NbFacettes;

    objet_cur = &(this->Objetlist[indice_objet_cur]);
    test_np   = false ;
    np = Vector3D();//    SetCoordonnees(np,0.,0.,0.) ;                             // Initialisation
    if (test_seuil_gouraud) {
        if (!((Produit_Scalaire(NormaleFacette,NormaleSommet) > seuil_Gouraud) || (seuil_Gouraud <= -0.999 ))) {
            npoint  = objet_cur->Facelist[ind_fac].F_sommets[ind_P] -1;              // Ici c'est le numéro d'indice du sommet qu'il faut récupérer
            p_Point = &(objet_cur->Pointslist[npoint]);
//            NbFacettes = objet_cur->Pointslist[npoint].IndicesFacettes.size();      // Nombre de facettes qui utilisent ce sommet
            NbFacettes = p_Point->IndicesFacettes.size();                           // Nombre de facettes qui utilisent ce sommet
            for (k=0; k< NbFacettes ; k++) {
//                nfac    = objet_cur->Pointslist[npoint].IndicesFacettes[k] ;
                nfac = p_Point->IndicesFacettes[k] ;
                /* Ne pas prendre en compte les facettes dont l'angle entre la normale avec celle de
                la facette en cours est trop fort */
                if ((Produit_Scalaire(objet_cur->Facelist[ind_fac].normale_b, objet_cur->Facelist[nfac].normale_b) > seuil_Gouraud2)  ||
                    (seuil_Gouraud2 <= -0.999) ) {
                    cur_fac = objet_cur->Facelist[nfac];
                    nbs     = cur_fac.Nb_Sommets_F;
                    coef    = 0.;
                    pa      = objet_cur->Sommetlist[cur_fac.F_sommets[0]-1].getPoint() ;
                    pb      = objet_cur->Sommetlist[cur_fac.F_sommets[1]-1].getPoint() ;
                    v1.X    = (double)(pb[0] - pa[0]); v1.Y = (double)(pb[1] - pa[1]); v1.Z = (double)(pb[2] - pa[2]);
                    for (k2=2; k2 < nbs; k2++) {
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

void BddInter::Calcul_All_Normales() {
    unsigned int o,i,nb_fac,nb_som;
    Object * objet_courant;
    Face   * facette_courante;
    double tdeb,tfin;

    printf("\nCalcul de toutes les normales\n");
    tdeb = omp_get_wtime();

    for (unsigned int o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        objet_courant->flat = false;
//        nb_fac = objet_courant->Facelist.size();
        nb_fac = objet_courant->Nb_facettes;
        nb_som = objet_courant->Nb_sommets;
#pragma omp parallel for
        for (i=0; i<nb_fac; i++) {
            Calcul_Normale_Barycentre(o,i);
            if (NotFlat) objet_courant->Facelist[i].flat = false;  // Garder plutôt ce qu'il y avait, sinon force le lissage de Gouraud partout : pas forcément souhaitable !
        }
        Genere_Tableau_Points_Facettes(objet_courant);
//        printf("\n%d sf %d %d %d\n",o,objet_courant->Nb_sommets, objet_courant->Nb_facettes,nbfac);
//        printf("%d vl %d %d\n",  o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);
//
        Genere_Normales_Aux_Sommets(o,nb_som);
//        printf("%d vl %d %d\n",  o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);

        // Recopie des numéros de sommets des facettes dans les numéros de vecteurs des luminances.
        objet_courant->Nb_vecteurs   = nb_som;
        objet_courant->Nb_luminances = nb_fac;
        indiceObjet_courant = o;
//        str.clear();
#pragma omp parallel for private(facette_courante)
        for (i=0; i<nb_fac; i++) {
            facette_courante = &(objet_courant->Facelist[i]);
//            NumerosSommets = objet_courant->Facelist[i].getF_sommets();
//            N_elements     = objet_courant->Facelist[i].getNumero();
//            make_1_luminance();
            make_1_luminance(facette_courante->getNumero(),facette_courante->getF_sommets()); // OK ici
        }
//        printf("%d vl %d %d\n",o,objet_courant->Nb_vecteurs,objet_courant->Nb_luminances);
    }
    bdd_modifiee = true;
    tfin = omp_get_wtime();
    printf("temps de calcul : %lf s via %d threads\n",tfin-tdeb,omp_get_max_threads());
    printf("Fin\n");
}

void BddInter::InverseX() {
    Object * objet_courant;
    unsigned int i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[0] *= -1;
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[0]  *= -1;
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[0] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::InverseY() {
    Object * objet_courant;
    unsigned i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[1] *= -1;
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[1]  *= -1;
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[1] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::InverseZ() {
    Object * objet_courant;
    unsigned i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            if (objet_courant->Vecteurlist[j].point.size() >= 3)
                objet_courant->Vecteurlist[j].point[2] *= -1;
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            if (objet_courant->Sommetlist[j].point.size() >= 3)
                objet_courant->Sommetlist[j].point[2]  *= -1;
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            objet_courant->Facelist[j].normale_b[2] *= -1;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoY() {
    Object * objet_courant;
    Face   * face_j;
    Sommet * sommet_j;
    Vecteur* vecteur_j;
    float memory;
    unsigned int i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            vecteur_j = &(objet_courant->Vecteurlist[j]);
            if (vecteur_j->point.size() >= 3) {
                memory = vecteur_j->point[0];
                vecteur_j->point[0] = vecteur_j->point[1];
                vecteur_j->point[1] = memory;
            }
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            sommet_j = &(objet_courant->Sommetlist[j]);
            if (sommet_j->point.size() >= 3) {
                memory = sommet_j->point[0];
                sommet_j->point[0] = sommet_j->point[1];
                sommet_j->point[1] = memory;
            }
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            face_j = &(objet_courant->Facelist[j]);
            memory = face_j->normale_b[0];
            face_j->normale_b[0] = face_j->normale_b[1];
            face_j->normale_b[1] = memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoZ() {
    Object * objet_courant;
    Face   * face_j;
    Sommet * sommet_j;
    Vecteur* vecteur_j;
    float memory;
    unsigned i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            vecteur_j = &(objet_courant->Vecteurlist[j]);
            if (vecteur_j->point.size() >= 3) {
                memory =vecteur_j->point[0];
                vecteur_j->point[0] = vecteur_j->point[2];
                vecteur_j->point[2] = memory;
            }
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            sommet_j = &(objet_courant->Sommetlist[j]);
            if (sommet_j->point.size() >= 3) {
                memory = sommet_j->point[0];
                sommet_j->point[0] = sommet_j->point[2];
                sommet_j->point[2] = memory;
            }
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            face_j = &(objet_courant->Facelist[j]);
            memory = face_j->normale_b[0];
            face_j->normale_b[0] = face_j->normale_b[2];
            face_j->normale_b[2] = memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::YtoZ() {
    Object * objet_courant;
    Face   * face_j;
    Sommet * sommet_j;
    Vecteur* vecteur_j;
    float memory;
    unsigned int i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            vecteur_j = &(objet_courant->Vecteurlist[j]);
            if (vecteur_j->point.size() >= 3) {
                memory = vecteur_j->point[1];
                vecteur_j->point[1] = vecteur_j->point[2];
                vecteur_j->point[2] = memory;
            }
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            sommet_j = &(objet_courant->Sommetlist[j]);
            if (sommet_j->point.size() >= 3) {
                memory = sommet_j->point[1];
                sommet_j->point[1] = sommet_j->point[2];
                sommet_j->point[2] = memory;
            }
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            face_j = &(objet_courant->Facelist[j]);
            memory = face_j->normale_b[1];
            face_j->normale_b[1] = face_j->normale_b[2];
            face_j->normale_b[2] = memory;
        }
    }
    bdd_modifiee = true;
}

void BddInter::XtoYtoZ() {
    Object * objet_courant;
    Face   * face_j;
    Sommet * sommet_j;
    Vecteur* vecteur_j;
    float memory;
    unsigned int i,j;
    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        for (j=0; j<objet_courant->Vecteurlist.size(); j++) {
            vecteur_j = &(objet_courant->Vecteurlist[j]);
            if (vecteur_j->point.size() >= 3) {
                memory = vecteur_j->point[2];
                vecteur_j->point[2] = vecteur_j->point[1];
                vecteur_j->point[1] = vecteur_j->point[0];
                vecteur_j->point[0] = memory;
            }
        }
        for (j=0; j<objet_courant->Sommetlist.size(); j++) {
            sommet_j = &(objet_courant->Sommetlist[j]);
            if (sommet_j->point.size() >= 3) {
                memory = sommet_j->point[2];
                sommet_j->point[2] = sommet_j->point[1];
                sommet_j->point[1] = sommet_j->point[0];
                sommet_j->point[0] = memory;
            }
        }
        for (j=0; j<objet_courant->Facelist.size(); j++) {
            face_j = &(objet_courant->Facelist[j]);
            memory = face_j->normale_b[2];
            face_j->normale_b[2] = face_j->normale_b[1];
            face_j->normale_b[1] = face_j->normale_b[0];
            face_j->normale_b[0] = memory;
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
    int tabPoints_j;
    long Nb_test, compteur;

    bool modification, indic;
    bool cancelled = false;
//    bool verbose=false;             // Si true affiche plus d'indications des changements. A généraliser et initialiser à plus haut niveau ?

    float epsilon=tolerance*diagonale_save/100.0;                       // La tolérance est donnée en % de la diagonale

    wxBeginBusyCursor();                                                // Affiche un curseur animé par défaut (pour faire patienter !)

    printf("\nSimplification de BDD : Debut du traitement\n");
    printf("Tolerance d'egalite des sommets : relative : %8.3f%%, absolue : %7.2e\n",tolerance,epsilon);
    nbp_changes = nbv_changes = 0;

    Dialog_Titre    = wxS("Simplification de la Base de données");
    Dialog_Comment  = wxS("Patience, ça peut être long... et même très long !");
    Dialog_Delay    = CLOCKS_PER_SEC/2;      // 1/2 seconde
    time_deb_dialog = clock();
    progress_dialog_en_cours = false;

    compteur = 0;   // Un compteur de boucles
    Nb_test  = 0;   // estimation du nombre de boucles (sur-évalué)
    for (o=0; o<this->Objetlist.size(); o++) {
        objet_courant = &(this->Objetlist[o]);
        Nb_test += 2*objet_courant->Nb_sommets;
        Nb_test += 2*objet_courant->Nb_vecteurs;
//      printf("0S %d\n",objet_courant->Nb_sommets);
//      printf("0V %d\n",objet_courant->Nb_vecteurs);
    }
//    printf("0  %d\n",Nb_test);


    for (o=0; o<this->Objetlist.size(); o++) {                          // On parcoure tous les objets en mémoire
        objet_courant = &(this->Objetlist[o]);
        printf("\nObjet : %s\n",objet_courant->GetName());
        if (objet_courant->deleted) continue ;                          // Ne pas traiter un objet supprimé (mais encore en mémoire), et donc passer au suivant
        nb_modifs    = 0;
        modification = false;
        nb_points    = objet_courant->Sommetlist.size() ;               // normalement égal à This->Objetlist[i].Nb_sommets;
        nb_points2   = objet_courant->Nb_sommets;
        if(nb_points != nb_points2) printf("Probleme sur objet %d: Sommetlist.size=%d, Nb_sommets=%d\n",o,nb_points,nb_points2);
        if (nb_points <= 0) continue ;                                  // Ne pas traiter un objet sans sommets
//        printf("Nombre de sommets avant le traitement   : %u\n",nb_points);
        tabPoints.resize(nb_points);                                    // Il ne devrait pas y en avoir plus !
        nbface = objet_courant->Nb_facettes;
        if (nbface <= 0) continue;

// Pour supprimer les points non utilisés .... on les marque comme des doublons du 1er sommet de la première facette
        int ind1 = objet_courant->Facelist[0].F_sommets[0] -1;          // -1 pour passer d'un numéro de sommet à un indice de sommet
        Point_1  = objet_courant->Sommetlist[ind1].getPoint();          // Coordonnées du 1er sommet de la 1ère facette dans Point_1
////#pragma omp parallel for private(cpt,k,l,Facette_courante,nbsom)
        for (i=1; i<=nb_points; ++i) {                                  // Ici, i est un numéro de point (décalé de 1 / indice)
        //pour chaque point d'un objet
            cpt = 0 ;
            for (k=0 ; k<nbface ; k++ ) {
                Facette_courante = &(objet_courant->Facelist[k]);
                nbsom = Facette_courante->Nb_Sommets_F;
                for (l=0; l<nbsom; l++) {
                    if (Facette_courante->F_sommets[l] == (int)i) {
                        cpt++ ;         // i est utilisé
                        break ;         // Ne pas continuer
                    }
                }
                if (cpt != 0) break ;   // Idem
            }
            if (cpt == 0) { // Ce point n'est pas utilisé dans l'objet. Forcer à en faire un doublon pour pouvoir ensuite l'éliminer
                objet_courant->Sommetlist[i-1].setPoint(Point_1);       // Numero de sommet i => indice i-1
                if (verbose) {
                    printf("\rPoint numero %6d non utilise dans l'objet %d",i,o) ;      // On pourrait mettre le nom de l'objet plutôt que son indice => transcription wx->Ascii
                    fflush(stdout); // Pour Sun/cc
                }
            }
//            #pragma omp critical
//            {
            compteur++;
//            }
// semble provoquer un blocage sur certaines bdd (comme klein_glass.STL) si à l'intérieur d'un pragma omp critical. Inutile de faire sur tous les threads => seulement le 0
// Mais ... le blocage subsiste de façon aléatoire. De plus, le gain de temps est peu sensible par rapport aux autres boucles ci-dessous.
//            if (omp_get_thread_num() == 0)
                cancelled = Update_Dialog(compteur,Nb_test,true);
                if (cancelled) goto Sortir;
        }
//// Fin de zone calcul //

//        compteur+=nb_points;
//        Update_Dialog(compteur,Nb_test,true);        // mieux (si supprimé ausi dans le pragma) mais l'Update_Dialog est tardif, bien après Dialog_Delay
//        printf("1 %d\n",compteur);
        if (verbose) printf("\n");
        for (i=0; i<nb_points; ++i) {           // Pas sûr que ce soit parallélisable !
            //pour chaque point d'un objet
//            objet_courant->Sommetlist[i].toshow = 0 ; // Raz
            Point_i = objet_courant->Sommetlist[i].getPoint();
            if (Point_i.empty()) continue;          // Point vide, non initialisé, non utilisé ?
            cpt = 0;
            for (j=(i+1); j<(nb_points-nb_modifs); ++j) {
            //on regarde si des points de la liste de sommets lui sont égaux (à epsilon près)
                Point_j = objet_courant->Sommetlist[j].getPoint();
                if (Point_j.empty()) continue;      // Point vide, non initialisé, non utilisé ?            A Vérifier ... si cela se produit ! Il faudrait l'éliminer plutôt !
                if (Points_Egaux(Point_i, Point_j, epsilon)) {
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

                for (j=0; j<cpt; ++j) {
#pragma omp parallel for private(l,Facette_courante,nbsom)
                    for (k=0; k<nbface; ++k) {
                        //on parcourt chaque face de l'objet
                        Facette_courante = &(objet_courant->Facelist[k]);
                        nbsom = Facette_courante->Nb_Sommets_F;
                        for (l=0; l<nbsom; ++l) {
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
// Fin de zone calcul //
                    //on traite aussi les sommets qui sont dans tabPoints
                    if (j < (cpt-1)) {
                        //si on n'est pas sur le dernier sommet de tabPoints[]
                        //Si les sommets suivants (après le rang j) de tabPoints sont supérieurs
                        //à tabPoints[j], alors on les décrémente aussi, pour que les sommets
                        //corrects soient remplacés
                        tabPoints_j = tabPoints[j];
#pragma omp parallel for
                        for (m=(j+1); m<cpt; ++m) {
                            if (tabPoints[m] > tabPoints_j) {
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
            compteur++;
            cancelled = Update_Dialog(compteur+nb_modifs,Nb_test,true);
            if (cancelled) goto Sortir;
        }
//        printf("2 %d\n",compteur);
        compteur +=nb_modifs;
        cancelled = Update_Dialog(compteur,Nb_test,true);
//        printf("2 %d\n",compteur);

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
        Simplification_Doublons_Points(o);  // pour éliminer les points en doublons dans les facettes si besoin

        if (cancelled) goto Sortir;

        if (objet_courant->Nb_sommets < 3) {
            printf("Il reste moins de 3 points dans l'objet => Suppression de l'objet.\n") ;
            objet_courant->deleted = true ;      // Passer à l'objet suivant ...
            bdd_modifiee = true ;
        } else {
            Simplification_Facettes(o);         // A faire même si pas de changement de sommets

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
            for (k=0; k<nb_points; k++) {
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
////#pragma omp parallel for private(cpt,k,l,Facette_courante,nbsom)
            for (i=1; i<=nb_points; ++i) {                           // Ici i est un numéro de vecteur, donc décalé de 1 / indice
            //pour chaque normale au sommet d'un objet (vecteur au sens SDM)
                cpt = 0 ;
                for (k=0; k<nbface; k++ ) {
                    Facette_courante = &(objet_courant->Facelist[k]);
                    Facette_courante->toshow = 0;   // Raz de précaution
                    nbsom = Facette_courante->Nb_Sommets_L;
                    for (l=0; l<nbsom; l++) {
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
                        printf("%s",utf8_To_ibm(Message));
                        fflush(stdout); // Pour Sun/cc
                    }
                }
////                #pragma omp critical
////                {
                compteur++;
////                }
//// Même problème que plus haut... blocage possible sur certaines bdd.
////                if (omp_get_thread_num() == 0)
                    cancelled = Update_Dialog(compteur,Nb_test,true);
            }
//// Fin de zone calcul //

            if (cancelled) goto Sortir;

//            printf("3 %d\n",compteur);
            if (verbose) printf("\n");
            for (i=0; i<nb_points; ++i) {
                //pour chaque vecteur d'un objet
                cpt     = 0;
                Point_i = objet_courant->Vecteurlist[i].getPoint();
                if (Point_i.empty()) continue;                      // Ne pas tenir compte des vecteurs vides
                for (j=(i+1); j<(nb_points-nb_modifs); ++j) {
                //on regarde si des points de la liste de vecteurs lui sont égaux (à epsilon près)
                    if (Points_Egaux(Point_i, objet_courant->Vecteurlist[j].getPoint(), epsilon)) {
                        tabPoints[cpt++] = j+1; // Numéro de vecteur et non indice ! décalé de 1 !!
                        if (verbose) {
                            sprintf(Message,"\rOBJET %u | doublon : vecteur %5u à remplacer par le numéro %5u",o,j,i);
                            printf("%s",utf8_To_ibm(Message));
                            fflush(stdout) ; // Pour Sun/cc
                        }
                    }
                }
                if (cpt > 0) {
                    if (verbose) printf("\n") ;
                    modification = true;
                    indic        = false;

                    for (j=0; j<cpt; ++j) {
#pragma omp parallel for private(l,Facette_courante,nbsom)
                        for (k=0; k<nbface; ++k) {
                            //on parcourt chaque face (luminance) de l'objet
                            Facette_courante = &(objet_courant->Facelist[k]);
                            nbsom = Facette_courante->Nb_Sommets_L;
                            for (l=0; l<nbsom; ++l) {                // Si la facette n'a pas de normales aux sommets, nbsom vaut 0 et on ne passe pas dans la boucle
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
// Fin de zone calcul //
                        //on traite aussi les sommets qui sont dans tabPoints
                        if (j<(cpt-1)) {
                            tabPoints_j = tabPoints[j];
#pragma omp parallel for
                            for (m=(j+1); m<cpt; ++m) {
                                if (tabPoints[m] > tabPoints_j) {
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
                compteur++;
                cancelled = Update_Dialog(compteur+nb_modifs,Nb_test,true);
            }

            if (cancelled) goto Sortir;
//            printf("4 %d\n",compteur);
            compteur +=nb_modifs;
            cancelled = Update_Dialog(compteur,Nb_test,true);
//            printf("4 %d\n",compteur);
            if (cancelled) goto Sortir;

            tabPoints.clear();
            if (modification) {
                printf("Nombre de normales aux sommets (vecteurs) avant le traitement : %u\n",nb_points);
                printf("Nouveau nombre de normales aux sommets                        : %u\n",objet_courant->Nb_vecteurs);
                nbv_changes  = nb_points - objet_courant->Nb_vecteurs;
                bdd_modifiee = true;
            } else {
                printf("Nombre de vecteurs : sans changement\n");
            }
        }
    }   // boucle for sur les objets (o)

Sortir:
    Fermer_progress_dialog();

    for (o=0; o<this->Objetlist.size(); o++) {                      // On parcoure tous les objets en mémoire
        objet_courant = &(this->Objetlist[o]);
        if (objet_courant->deleted) continue ;                      // Passer au suivant
        Genere_Tableau_Points_Facettes(objet_courant);
        Genere_Tableau_Aretes(objet_courant);
    }
    m_gllist = 0;
    Search_Min_Max();    // Reset des comptages de facettes, points, ...
    if (wxIsBusy()) wxEndBusyCursor();                                              // Supprime le curseur animé... C'est terminé !

    printf("\n");
    if((nbp_changes == 0) && (nbv_changes == 0)) {
        printf("Aucun changement dans la BDD\n");
    } else {
        if (nbp_changes != 0) {
            if (nbp_changes == 1)
                sprintf(Message,"%4d point a été supprimé dans la BDD\n",      nbp_changes);
            else
                sprintf(Message,"%4d points ont été supprimés dans la BDD\n",  nbp_changes);
            printf("%s",utf8_To_ibm(Message));
        }
        if (nbv_changes != 0) {
            if (nbv_changes == 1)
                sprintf(Message,"%4d vecteur a été supprimé dans la BDD\n",    nbv_changes);
            else
                sprintf(Message,"%4d vecteurs ont été supprimés dans la BDD\n",nbv_changes);
            printf("%s",utf8_To_ibm(Message));
        }
    }

    undo_memory = 0;                        // Un undo de souder n'est plus possible
    MPanel->Button_Undo->Disable();         // et donc désactiver le bouton

    printf("\nFin de simplification de la Bdd\n");
    if (cancelled) printf("La simplification a été interrompue. Risque d'instabilités !\n");

}

void BddInter::Genere_Attributs_Facettes(int indiceObjet, int Nb_facettes, int numeroGroupe, int numeroMateriau)
{
    // Groupes dans Aspect_face
    Object *Objet_courant;

    Objet_courant = &(this->Objetlist[indiceObjet]);
    Genere_Attributs_Facettes(Objet_courant, Nb_facettes, numeroGroupe, numeroMateriau);
}

void BddInter::Genere_Attributs_Facettes(Object *Objet_courant, int Nb_facettes, int numeroGroupe, int numeroMateriau)
{
    // Groupes dans Aspect_face
    this->str.clear();
    this->N_elements = Nb_facettes;
    Objet_courant->Nb_aspects = Nb_facettes;

    this->make_aspect_face();
#pragma omp parallel for
    for (int i=1; i <= this->N_elements ; i++) {
        Objet_courant->Facelist[i-1].groupe     = numeroGroupe;
        Objet_courant->Facelist[i-1].codmatface = numeroMateriau;
    }
}

void BddInter::Genere_Luminances(int indiceObjet, int Nb_facettes)
{
// Ici, ce sont les mêmes indices que pour les facettes

    Object * objet_courant;

    objet_courant = &(this->Objetlist[indiceObjet]);
    Genere_Luminances(objet_courant, Nb_facettes);
}

void BddInter::Genere_Luminances(Object *objet_courant, int Nb_facettes)
{
// Ici, ce sont les mêmes indices que pour les facettes

    this->str.clear();
    this->N_elements = Nb_facettes;
    objet_courant->Nb_luminances = Nb_facettes;
    this->make_luminance();
    int numero = 0;

#pragma omp parallel for
    for (int i=0; i<Nb_facettes; i++) {
//        std::vector<int> NumerosSommets = objet_courant->Facelist[i].getF_sommets();
//        numero++;
//        this->N_elements = numero;
//        this->Set_numeros(NumerosSommets);
//        this->make_1_luminance();
        this->make_1_luminance(i+1,objet_courant->Facelist[i].getF_sommets());  // OK ici
    }
}

void BddInter::Calcul_Normale_Barycentre(int indice_objet, int indice_facette) {

// Calcul d'une normale d'une facette d'indice j dans l'objet d'indice i
// La normale est une "moyenne" si la facette possède plus de 3 points => tolérance aux facettes non planes.

    Face   *Face_ij;
    Object *Objet_i;
    float vector1[3];
    float vector2[3];
    float mag;
    std::vector<float> normalf(3,0.0);
    std::vector< std::vector<float> > normal;
    std::vector<float> normal1;
    int n,z;
    Objet_i = &(this->Objetlist[indice_objet]);
    Face_ij = &(Objet_i->Facelist[indice_facette]);

    normal1.clear();
    normal.clear();

    for (n=2; n<(int)Face_ij->F_sommets.size(); n++) {    // n= Face_ij.point.size() nombre de sommets de la facette
        for (z=0; z<3; z++) {
            vector1[z]=
                 Objet_i->Sommetlist[Face_ij->F_sommets[n-1]-1].point[z]
                -Objet_i->Sommetlist[Face_ij->F_sommets[0]  -1].point[z];       // Vecteur P1-P0 si n=2, puis P2-P0 si n=3, ...
            vector2[z]=
                 Objet_i->Sommetlist[Face_ij->F_sommets[n]-1].point[z]          // Vecteur P2-P0 si n=2, puis P3-P0 si n=3, ..., Pn-P0 avec n >= 2
                -Objet_i->Sommetlist[Face_ij->F_sommets[0]-1].point[z];
        }
        normal1.push_back(vector1[1]*vector2[2]-vector1[2]*vector2[1]);         // Calcul de n-2 sous-normales
        normal1.push_back(vector1[2]*vector2[0]-vector1[0]*vector2[2]);
        normal1.push_back(vector1[0]*vector2[1]-vector1[1]*vector2[0]);
        normal.push_back(normal1);
        normal1.clear();
    }

    // Calcul d'une normale moyenne : Pb: le vecteur Point1-Point0 a un rôle prépondérant.
    // Voir si besoin la fonction correspondante dans la version Tcl.

    for (n=0; n<(int)normal.size(); n++) {
        for (z=0; z<3; z++) normalf[z] += normal[n][z];      // La version originale faisait une moyenne, mais une somme suffit car la normalisation à 1 est faite ci-dessous
    }
    mag = sqrt(normalf[0]*normalf[0] + normalf[1]*normalf[1] + normalf[2]*normalf[2]);
    if (mag != 0.0) for (z=0; z<3; z++) normalf[z] /= mag;   // Normalisation à 1 (si mag différent de 0 !)
    Face_ij->normale_b = normalf;
    bdd_modifiee = true ;
}

void BddInter::Genere_Normales_Facettes(int indiceObjet, int Nb_facettes)
{
// Génère les normales à partir des 3 premiers points de chaque facette
// N'est plus utilisé sous cette forme. Cf version avec pointeur sur objet_courant
    Object * objet_courant;

    objet_courant = &(this->Objetlist[indiceObjet]);
    Genere_Normales_Facettes(objet_courant, Nb_facettes);
}

void BddInter::Genere_Normales_Facettes(Object *objet_courant, int Nb_facettes)
{
// Génère les normales à partir des 3 premiers points de chaque facette (peut ne pas être suffisant si points doublés, facette très allongée, ...)
// Peut poser des soucis sur des facettes très allongées : voir dans la version Tcl comment est contourné ce problème !

// ***> Voir éventuellement comment fusionner avec Calcul_Normale_Barycentre. Toutefois, uniquement appelé dans les primitives => donc pas de soucis normalement

    std::vector<float> xyz_point;
    std::vector<int>   NumerosSommets;

    // Normales aux facettes
    this->str.clear();
    this->N_elements = Nb_facettes;
    objet_courant->Nb_facettes = Nb_facettes;
    this->make_normale();

#pragma omp parallel for private(NumerosSommets,xyz_point)
    for (int i=0; i<Nb_facettes; i++) {
        objet_courant->Facelist[i].flat = false;   // Sphère ou ellipsoïde => non plat
        NumerosSommets = objet_courant->Facelist[i].getF_sommets();
        xyz_point = objet_courant->Sommetlist[NumerosSommets[0]-1].getPoint();
        Vector3D P1(xyz_point[0],xyz_point[1],xyz_point[2]);
        xyz_point = objet_courant->Sommetlist[NumerosSommets[1]-1].getPoint();
        Vector3D P2(xyz_point[0],xyz_point[1],xyz_point[2]);
        xyz_point = objet_courant->Sommetlist[NumerosSommets[2]-1].getPoint();
        Vector3D P3(xyz_point[0],xyz_point[1],xyz_point[2]);
        P1 -= P3;
        P2 -= P3;
        Vector3D Vn = P1.crossProduct(P2);
        Vn.normalize();
#pragma omp critical
{
        this->N_elements=i+1; this->Setxyz(Vn.X,Vn.Y,Vn.Z); this->make_1_normale();
}
    }
}

void BddInter::Genere_Facettes_Sphere(int Nb_Meridiens, int Nb_Paralleles, bool New_typeSphere)
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
    this->make_face();
    (this->Objetlist.end()-1)->Nb_facettes = this->N_elements;

    numero = 0;
    for (i=1; i<m; i++) {
        // Nord
        numero++;
        i1 = 1;
        i3 = 1 + i;
        i2 = i3+ 1;
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
        this->make_1_face();

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
            this->make_1_face();

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
            this->make_1_face();

            if (New_typeSphere) sens *= -1;
        }

        // Sud
        numero++;
        i1 = Nmp +2;                                            // Pole Sud
        this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i3,i2);  // récup des 2 derniers i2,i3 mais inverser le sens
        this->make_1_face();
    }

// Code spécifique pour la dernière série entre dernier et 1er méridien

    if (New_typeSphere) sens = -sens1;  // Ne fonctionne bien que si le nombre de méridiens est pair.
    // derniere face pole nord
    numero++;
    i1 = 1;
    i2 = 2 ;
    i3 = 1 + m ;
    this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
    this->make_1_face();

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
        this->make_1_face();

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
        this->make_1_face();
        if (New_typeSphere) sens *= -1;
    }

    // derniere face pole sud
    numero++;
    i2 = 1  + (m*p) ;
    i1 = i2 + 1;
    i3 = i2 -m +1;
    this->str.Printf(_T("%d 3 %d %d %d"),numero,i1,i2,i3);
    this->make_1_face();
}

void BddInter::Genere_Sommets_Sphere(int Nb_Meridiens, int Nb_Paralleles, float centre[3], float rayon, float coefx, float coefy, float coefz)
{
    int i, j, numero;
    float theta,phi, sin_phi, cos_phi;
    float Xc = centre[0] , Yc = centre[1] , Zc = centre[2];
    int NbPoints = (Nb_Meridiens*Nb_Paralleles)+2;

    this->str.clear();
    this->N_elements = NbPoints;
    this->make_sommet();
    (this->Objetlist.end()-1)->Nb_sommets = NbPoints;
    // Pôle Nord
    numero = 1;
    this->N_elements = numero;
    this->Setxyz(Xc, Yc+coefy*rayon, Zc);
    this->make_1_sommet();

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
            this->make_1_sommet();
        }
    }
    // Pôle Sud
    numero = NbPoints;
    this->N_elements = numero;
    this->Setxyz(Xc, Yc-coefy*rayon, Zc);
    this->make_1_sommet();
}

void BddInter::Genere_Normales_Sommets_Sphere(int Nb_Meridiens, int Nb_Paralleles, float coefx, float coefy, float coefz)
{
    int i, j, numero;
    float theta, phi, sin_phi, cos_phi;
    int NbPoints = (Nb_Meridiens*Nb_Paralleles)+2;

    this->str.clear();
    this->N_elements = NbPoints;
    this->make_vecteur();
    (this->Objetlist.end()-1)->Nb_vecteurs = NbPoints;
    // Pôle Nord
    numero = 1;
    this->N_elements = numero;
    this->Setxyz(0., 1., 0.);
    this->make_1_vecteur();

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
            this->make_1_vecteur();
        }
    }
    // Pôle Sud
    numero = NbPoints;
    this->N_elements = numero;
    this->Setxyz(0., -1., 0.);
    this->make_1_vecteur();
}

void BddInter::Genere_Normale_1_Sommet(Object *current_objet, unsigned int indice_point, unsigned int indice_vecteur)
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
    Points *p_Point;

    test_np = false ;
    np = Vector3D();//    SetCoordonnees(np,0.,0.,0.) ;                               // Initialisation

    p_Point    = &(current_objet->Pointslist[indice_point]);
    NbFacettes = p_Point->IndicesFacettes.size();
////#pragma omp parallel for private(nfac,cur_fac,nbs,coef,pa,pb,pc,v1,v2,vp,k2) // Pas très intéressant ici car NbFacettes petit en général et appel depuis Genere_Normales_Aux_Sommets
    for (k=0; k < NbFacettes; k++) {
        nfac    = p_Point->IndicesFacettes[k] ;
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
        coef = acos(Produit_Scalaire(&v1, &v2)) ;
*/
// Méthode par surface de facette (si elles ne sont pas planes, on pourrait calculer à partir du sommet k,
// et incrémenter k+1, k+2,... avec le même test que ci-dessus si dépassement sur les numéros).
// Le résultat sera cependant assez peu différent.
        coef = 0.;
        pa   = current_objet->Sommetlist[cur_fac.F_sommets[0]-1].getPoint() ;
        pb   = current_objet->Sommetlist[cur_fac.F_sommets[1]-1].getPoint() ;
        v1.X    = (double)(pb[0] - pa[0]); v1.Y = (double)(pb[1] - pa[1]); v1.Z = (double)(pb[2] - pa[2]);
        for (k2=2; k2 < nbs; k2++) {
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
void BddInter::Genere_Normales_Aux_Sommets(unsigned int o, int nb_p)
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
#pragma omp parallel for
    for (indice_point=0; indice_point < nb_p; indice_point++) {
        if ((indice_point%500000) == 0 && verbose) { // 500 à l'origine, histoire de faire patienter, mais n'est plus très utile maintenant.(encore + avec omp !!)
            sprintf(Message,"\rCalcul des normales aux sommets de l'objet %2d : %8d",
                    objet_courant->GetValue(), indice_point);
            printf("%s",utf8_To_ibm(Message));
//            printf("\rCalcul des normales aux sommets de l'objet %2d : %8d", objet_courant->GetValue(), indice_point);
            fflush(stdout); // Nécessaire sur cc SUN, sinon la ligne n'apparaît pas
        }
        Genere_Normale_1_Sommet(objet_courant,indice_point,indice_point) ;   // NOTE : ici indice_vecteur = indice_point
                                                                            // => synchroniser les numéros de sommets des facettes et des luminances
    }
    // Recopie des numéros de sommets de facettes dans numéros de sommets des luminances (doivent être les mêmes du fait de l'appel à Genere_Normale_1_Sommet ci-dessus !)
    unsigned nb_fac = objet_courant->Facelist.size();
#pragma omp parallel for private(facette_courante)
    for (unsigned int nfac=0; nfac<nb_fac; nfac++) {    // <=> Nb_facettes
        facette_courante = &(objet_courant->Facelist[nfac]);
        facette_courante->L_sommets.clear();                                // Pas sûr que ce soit utile (libération de la mémoire)
        facette_courante->L_sommets    = facette_courante->F_sommets;
        facette_courante->Nb_Sommets_L = facette_courante->Nb_Sommets_F;    // Par précaution, mais c'est sans doute déjà le cas.
    }
    if (verbose) {
        sprintf(Message,"\rCalcul des normales aux sommets de l'objet %2d : %8d\n",
                objet_courant->GetValue(), nb_p);
//                objet_courant->GetValue(), indice_point);
        printf("%s",utf8_To_ibm(Message));
//        printf("\rCalcul des normales aux sommets de l'objet %2d : %8d\n", objet_courant->GetValue(), indice_point);
    }
}

bool BddInter::Compare_Normales(int objet, int face_1, int face_2) {
/* renvoie true si les composantes des normales aux barycentres des facettes face_1 et face_2 ont les mêmes signes (version originale) */

// Et si on vérifiait plutôt que l'angle entre n1 et n2 soit inférieur à une valeur test (+-45° par exemple !)
// On pourrait paramétrer cette valeur dans Préférences et/ou directement dans Sélection et déplacements

    std::vector<float> n1, n2;
    Object *Objet_courant;

    const float seuil=0.5;//707f; // 0.707 = cos(45°) , 0.5 = cos(60°)
    float prod;

    Objet_courant = &(this->Objetlist[objet]);
    n1   = Objet_courant->Facelist[face_1].getNormale_b();
    n2   = Objet_courant->Facelist[face_2].getNormale_b();
    prod = Produit_Scalaire(n1,n2);

    if (prod > seuil) return true; // L'angle entre les 2 normales est < acos(seuil)

    return false;
}

bool BddInter::Points_Egaux(const std::vector<float> &point_1, const std::vector<float> &point_2, float epsilon)
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
void BddInter::Simplification_Doublons_Points(unsigned int objet)
{
	unsigned int i,j,k,nbsom;
    Object      * objet_courant;
    Face        * facette;
    bool test;

    objet_courant = &(this->Objetlist[objet]) ;

#pragma omp parallel for private(facette,nbsom,j,k,test)
	for (i=0; i<objet_courant->Nb_facettes; ++i) {
        facette = &(objet_courant->Facelist[i]);
		nbsom   = facette->Nb_Sommets_F;
		for (j=0; j<nbsom; ++j) {
			for (k=0; k<nbsom; ++k) { // Faut-il reprendre à partir de 0 ou seulement de j+1 ?
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
#pragma omp critical
{
                            printf("objet : %2d, doublons sur facette %u \n",objet,i);
}
                            facette->deleteone(k);
                            nbsom   = facette->Nb_Sommets_F; // Mettre à jour la borne sup !
                            facette->Nb_Sommets_L-- ;        // Le deleteone a aussi supprimé la normale aux sommets correspondante
                            //// Ne faudrait-il pas faire la même chose sur luminance ?
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
void BddInter::Simplification_Facettes(unsigned int objet)
{
	unsigned int i,nbface,decalage;
    Object      * objet_courant;

    objet_courant = &(this->Objetlist[objet]) ;

	nbface  = objet_courant->Nb_facettes;
	decalage=0 ;
	for (i=0; i<nbface; ++i)	{               // Non parallélisable -à cause des --nbface et --i
        objet_courant->Facelist[i].toshow = 0;              // Raz de cette valeur pour éviter qu'un éventuel futur Undo de souder trouve quelque chose à faire !
		if(objet_courant->Facelist[i].Nb_Sommets_F < 3) {
		/* On supprime la facette i */
            if(decalage == 0) printf("Objet indice %d\n",objet) ;
            sprintf(Message,"facette %u a supprimer (moins de 3 points)\n",i+decalage);
            printf("%s",utf8_To_ibm(Message));

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

void BddInter::Genere_Liste_Groupes_Materiaux(unsigned int indice_objet)
{
    Object *objet_courant;

    objet_courant = &(this->Objetlist[indice_objet]) ;
    Genere_Liste_Groupes_Materiaux(objet_courant);
}

void BddInter::Genere_Liste_Groupes_Materiaux(Object *objet_courant)
{
    unsigned int indice;
    int groupe, materiau;

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
