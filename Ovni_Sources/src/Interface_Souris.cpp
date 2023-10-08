#include "Interface.h"

// Gestion de l'interface de BddInter à la souris
// **********************************************

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
    GLint hits = 0;
    GLint viewport[4];
    int val;
    static bool previous_right_drag = false;    // En statique pour retrouver la valeur précédente à chaque appel
                                                // (pourrait être dans BddInter mais n'est utile que dans OnMouse !)
    mouse_position = event.GetPosition();
    if (finishdraw == true) {
//        finishdraw = false;

        if (event.RightUp() && !previous_right_drag) {  // Ne pas faire l'affichage des menus contextuels si juste après un drag souris bouton droit
//            printf("Right clic\n");
            wxMenu My_popupmenu;                        // Recopie locale des ID* sinon ça ne marche pas (et c'est plus court !)
            long ID_POPUP_RAZ        = MAIN_b->ID_POPUP_RAZ;
            long ID_POPUP_COMPLEMT   = MAIN_b->ID_POPUP_COMPLEMT;
            long ID_POPUP_ETENDRE    = MAIN_b->ID_POPUP_ETENDRE;
            long ID_POPUP_CENTRER    = MAIN_b->ID_POPUP_CENTRER;
            long ID_POPUP_MASQUER    = MAIN_b->ID_POPUP_MASQUER;
            long ID_POPUP_DEMASQUER  = MAIN_b->ID_POPUP_DEMASQUER;
            long ID_POPUP_DELETE     = MAIN_b->ID_POPUP_DELETE;
            long ID_POPUP_UNDELETE   = MAIN_b->ID_POPUP_UNDELETE;
            long ID_POPUP_NEWOBJECT  = MAIN_b->ID_POPUP_NEWOBJECT;
            long ID_POPUP_INVERSER_N = MAIN_b->ID_POPUP_INVERSER_N;
            long ID_POPUP_PARCOURS_I = MAIN_b->ID_POPUP_PARCOURS_I;
            long ID_POPUP_RAZ_SELECT = MAIN_b->ID_POPUP_RAZ_SELECT;
            long ID_POPUP_NORM_F     = MAIN_b->ID_POPUP_NORM_F;
            long ID_POPUP_NORM_S     = MAIN_b->ID_POPUP_NORM_S;
            long ID_POPUP_FLAT       = MAIN_b->ID_POPUP_FLAT;
            long ID_POPUP_NOT_FLAT   = MAIN_b->ID_POPUP_NOT_FLAT;
#if !wxCHECK_VERSION(3,3,0)
            wxColour Forg;
            wxColour Back;
            if (theme_b) {
                Forg    = New_Forg;
                Back    = New_Back;
            } else {
                Forg    = wxNullColour;
                Back    = wxNullColour;
            }
#endif

#if wxCHECK_VERSION(3,3,0)
//            Back = ;
//            Forg = ;
#endif // wxCHECK_VERSION

            bool desactiver_menu = true;
            if ((mode_selection == selection_facette) && (this->ToSelect.ListeSelect.size() > 0)) desactiver_menu = false;
            // Note : plutôt que désactiver, ou pourrait ne pas afficher ces menus qui ont besoin que des facettes soient sélectionnées

            // Recopie de ce qui est fait par wxSmith ... Faute de mieux ! Bourrin mais ça marche à condition de forcer en public les ID_POPUP*
            // Recopier en dehors de wxSmith, ce qui a été créé (ID_POPUP, CONNECT, ...) puis supprimer le popup menu de wxSmith !
            wxMenuItem* Popup_RAZ;
            if (mode_selection == selection_point)
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, wxS("RAZ de sélection des points\t(s)"),   wxEmptyString, wxITEM_NORMAL);
            else if (mode_selection == selection_objet)
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, wxS("RAZ de sélection des objets\t(s)"),   wxEmptyString, wxITEM_NORMAL);
            else
                Popup_RAZ = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ, wxS("RAZ de sélection des facettes\t(s)"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_RAZ->SetBackgroundColour(Back);
            Popup_RAZ->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_RAZ);

            wxMenuItem* Popup_Complemt = nullptr;
            if (mode_selection == selection_facette) {
                Popup_Complemt = new wxMenuItem((&My_popupmenu), ID_POPUP_COMPLEMT, wxS("Sélectionner les facettes complémentaires\t(k)"),   wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
                Popup_Complemt->SetBackgroundColour(Back);
                Popup_Complemt->SetTextColour(Forg);
#endif
                My_popupmenu.Append (Popup_Complemt);
                if (desactiver_menu) Popup_Complemt->Enable(false);
            }

            wxMenuItem* Popup_Centrer;
            if (this->ToSelect.ListeSelect.size() > 0)
                Popup_Centrer = new wxMenuItem((&My_popupmenu), ID_POPUP_CENTRER, wxS("Centrer la rotation sur la sélection\t(c)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Centrer = new wxMenuItem((&My_popupmenu), ID_POPUP_CENTRER, wxS("Recentrer sur la rotation par défaut\t(c)"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Centrer->SetBackgroundColour(Back);
            Popup_Centrer->SetTextColour(Forg);
#endif // wxCHECK_VERSION
            My_popupmenu.Append(Popup_Centrer);

            wxMenuItem* Popup_Etendre = new wxMenuItem((&My_popupmenu), ID_POPUP_ETENDRE, wxS("Étendre la sélection\t(x)"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Etendre->SetBackgroundColour(Back);
            Popup_Etendre->SetTextColour(Forg);
#endif // wxCHECK_VERSION

            My_popupmenu.Append (Popup_Etendre);
            if (desactiver_menu) Popup_Etendre->Enable(false);

            My_popupmenu.AppendSeparator();

            wxMenuItem* Popup_Masquer = new wxMenuItem((&My_popupmenu), ID_POPUP_MASQUER, wxS("Masquer les facettes sélectionnées\t(Numpad Suppr)"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Masquer->SetBackgroundColour(Back);
            Popup_Masquer->SetTextColour(Forg);
#endif
            My_popupmenu.Append (Popup_Masquer);
            if (desactiver_menu) Popup_Masquer->Enable(false);

            wxMenuItem* Popup_Delete  = new wxMenuItem((&My_popupmenu), ID_POPUP_DELETE,  wxS("Supprimer les facettes sélectionnées\t(Suppr)"),      wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Delete->SetBackgroundColour(Back);
            Popup_Delete->SetTextColour(Forg);
#endif
            My_popupmenu.Append (Popup_Delete);
            if (desactiver_menu) Popup_Delete->Enable(false);

            wxMenuItem* Popup_NewObject = new wxMenuItem((&My_popupmenu), ID_POPUP_NEWOBJECT, wxS("Couper/coller les facettes sélectionnées vers un nouvel objet"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_NewObject->SetBackgroundColour(Back);
            Popup_NewObject->SetTextColour(Forg);
#endif
            My_popupmenu.Append (Popup_NewObject);
            if (desactiver_menu) Popup_NewObject->Enable(false);

            My_popupmenu.AppendSeparator();

            wxMenuItem* Popup_Inverser ;
            if (this->ToSelect.ListeSelect.size() > 0)
                Popup_Inverser = new wxMenuItem((&My_popupmenu), ID_POPUP_INVERSER_N, wxS("Inverser les normales sélectionnées\t(i)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Inverser = new wxMenuItem((&My_popupmenu), ID_POPUP_INVERSER_N, wxS("Inverser toutes les normales\t(i)"),        wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Inverser->SetBackgroundColour(Back);
            Popup_Inverser->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Inverser);

            wxMenuItem* Popup_Reverse  = new wxMenuItem((&My_popupmenu), ID_POPUP_PARCOURS_I, wxS("Inverser le sens de parcours des facettes sélectionnées\t(p)"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Reverse->SetBackgroundColour(Back);
            Popup_Reverse->SetTextColour(Forg);
#endif
            My_popupmenu.Append (Popup_Reverse);
            if (desactiver_menu) Popup_Reverse->Enable(false);

            wxMenuItem* Popup_Raz_Select;
            if (this->Raz_Selection_F)
                Popup_Raz_Select = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ_SELECT, wxS("Désactiver la désélection automatique\t(z)"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Raz_Select = new wxMenuItem((&My_popupmenu), ID_POPUP_RAZ_SELECT, wxS("Activer la désélection automatique\t(z)"),    wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Raz_Select->SetBackgroundColour(Back);
            Popup_Raz_Select->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Raz_Select);  // On peut toujours l'afficher comme activé (par précaution)

            My_popupmenu.AppendSeparator();

            wxMenuItem* Popup_Afficher_Normales;
            if (AfficherNormaleFacette)
                Popup_Afficher_Normales = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_F, wxS("Ne pas afficher les normales aux barycentres"), wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Afficher_Normales = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_F, wxS("Afficher les normales aux barycentres"),        wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Afficher_Normales->SetBackgroundColour(Back);
            Popup_Afficher_Normales->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Afficher_Normales);

            wxMenuItem* Popup_Afficher_NormalesSommets;
            if (AfficherNormalesSommets)
                Popup_Afficher_NormalesSommets = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_S, wxS("Ne pas afficher les normales aux sommets"),  wxEmptyString, wxITEM_NORMAL);
            else
                Popup_Afficher_NormalesSommets = new wxMenuItem((&My_popupmenu), ID_POPUP_NORM_S, wxS("Afficher les normales aux sommets"),         wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Afficher_NormalesSommets->SetBackgroundColour(Back);
            Popup_Afficher_NormalesSommets->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Afficher_NormalesSommets);

            wxMenuItem* Popup_Forcer_Facettes_Planes;
            Popup_Forcer_Facettes_Planes = new wxMenuItem((&My_popupmenu), ID_POPUP_FLAT,         wxS("Forcer les facettes à être planes"),  wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Forcer_Facettes_Planes->SetBackgroundColour(Back);
            Popup_Forcer_Facettes_Planes->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Forcer_Facettes_Planes);

            wxMenuItem* Popup_Forcer_Facettes_NonPlanes;
            Popup_Forcer_Facettes_NonPlanes = new wxMenuItem((&My_popupmenu), ID_POPUP_NOT_FLAT,  wxS("Forcer les facettes à être non planes"),  wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
            Popup_Forcer_Facettes_NonPlanes->SetBackgroundColour(Back);
            Popup_Forcer_Facettes_NonPlanes->SetTextColour(Forg);
#endif
            My_popupmenu.Append(Popup_Forcer_Facettes_NonPlanes);

            if (Elements_Masques || Elements_Supprimes)
                My_popupmenu.AppendSeparator(); // Ajout d'un séparateur si des élements de menu suivent ...

            wxMenuItem* Popup_Demasquer = nullptr;
            if (Elements_Masques) {             // S'il y a au moins une facette masquée, proposer de les réafficher
                Popup_Demasquer = new wxMenuItem((&My_popupmenu), ID_POPUP_DEMASQUER, wxS("Réafficher les facettes masquées"), wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
                Popup_Demasquer->SetBackgroundColour(Back);
                Popup_Demasquer->SetTextColour(Forg);
#endif
                My_popupmenu.Append(Popup_Demasquer);
            }
            wxMenuItem* Popup_Undelete = nullptr;
            if (Elements_Supprimes) {           // S'il y a au moins une facette supprimée, proposer de les restituer
                Popup_Undelete = new wxMenuItem((&My_popupmenu), ID_POPUP_UNDELETE,   wxS("Restituer les facettes supprimées"),wxEmptyString, wxITEM_NORMAL);
#if !wxCHECK_VERSION(3,3,0)
                Popup_Undelete->SetBackgroundColour(Back);
                Popup_Undelete->SetTextColour(Forg);
#endif
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
            if (test_rectangle_selection) {                 // ici pour l'instant (phase de test)
                Selection_rectangle((GLint)mouse_position.x,(GLint)mouse_position.y);   // Paramétrage de l'arrivée du rectangle de sélection
                Refresh();
            }
        } else if(event.MiddleUp()) {   // event.RightUp() ||
//            printf("event.MiddleUp\n");
            this->ToSelect.verrouiller_ListeSelect(false);  // Déverrouiller la liste : évite de recommencer une sélection tant qu'on n'a pas relaché la souris
            if (test_rectangle_selection) {                 // ici pour l'instant (phase de test)
                xd_sel = xa_sel = (GLint)mouse_position.x;  // Donner la même valeur aux 2 extrémités => ne tracera plus le rectangle
                yd_sel = ya_sel = (GLint)mouse_position.y;
                if (mode_selection == selection_point)   m_gllist = glliste_points;
                if (mode_selection == selection_facette) m_gllist = glliste_select;
                Refresh();
            }
            if ((modeGL == aretes) && click_sur_segment) {
                buildAllLines(); // OK si on a cliqué sur une arête, sinon inutile !
                click_sur_segment = false;
            }
            if (MManip->IsShown()) {
                if (this->ToSelect.ListeSelect.empty()) // Si besoin, activer/désactiver le bouton Button_NewObjet
                    MManip->Button_NewObjet->Disable();
                else
                    MManip->Button_NewObjet->Enable();
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
                    val = Convert_Rotx_LSI();
                    MPosObs->SpinCtrl_LSI->SetValue(val);

                    val = Convert_Rotz_LAZ();
                    MPosObs->SpinCtrl_LAZ->SetValue(val);

                    if (lround(m_gldata.roty) == 0) MPosObs->StaticText_Warning->Hide();
                    else                            MPosObs->StaticText_Warning->Show();
                }
                if (MZoomSpec->IsShown()) {
                    val = Convert_Rotx_LSI();
                    MZoomSpec->SpinCtrl_LSI->SetValue(val);

                    val = Convert_Rotz_LAZ();
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

//            if(MPanel->Bool_souder || show_points) {    // Ici, survol des points affichés, soit simplement affichés, soit en mode de soudure de points
            if (detection_survol_point) {    // Ici, survol des points affichés, soit simplement affichés, soit en mode de soudure de points
                                                        // Si show_points seulement, l'affichage est rallenti à cause de la détection de points survolés

                hits = 0;   // Initialisation
                if(MPanel->Bool_souder) modeGL = points;      // On ne passe pas en mode sélection de points si show_points seulement !
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
                glCallList(glliste_points);             // Afficher la liste de tous les points
                glCallList(glliste_points_selected);    // Afficher aussi la liste des points sélectionnés
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glFlush();
                hits = glRenderMode(GL_RENDER);
                if(hits != 0) {
                    if (WhoIsUnderTheMouse(hits, 0)) {
                        Search_Min_Max();                           // Pourquoi ? pour être sûr d'avoir les bons min-max en mémoire ?
                        if (show_points && !MPanel->Bool_souder) {  // En mode show_points seulement, pas en mode soudure de points
                            if ((point_under_mouse != point_under_mouse_old) || (objet_under_mouse != objet_under_mouse_old)) {
                                if (verbose) printf("Survol du point %d de l'objet %d\n", point_under_mouse, objet_under_mouse);
                                objet_under_mouse_old = objet_under_mouse;  // Mémoriser les valeurs pour ne pas afficher plusieurs fois de suite la même chose !
                                point_under_mouse_old = point_under_mouse;
                            }
                        }
                        m_gllist = glliste_points_selected;         // Pour ne reconstruire que la liste de points sélectionnés
                        if (MPanel->Bool_souder) {
                            if (ifexist_sommet(objet_under_mouse_old,point_under_mouse_old)) Objetlist[objet_under_mouse_old].Sommetlist[point_under_mouse_old].selected = false;
                            if (ifexist_sommet(objet_under_mouse    ,point_under_mouse))     Objetlist[objet_under_mouse].Sommetlist[point_under_mouse].selected = false;
                            // Plutôt que de reconstruire toute la liste de points, seuls 2 points suffisent en superposition, 1 point jaune survolé et un point rouge stocké
                            m_gllist = -1;  //glliste_objets; //points;
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
                    m_gllist = -1;              // glliste_objets;  //points;
                    Refresh();                  // Ne rafraîchir que le tracé des listes déjà établies
                }
                glMatrixMode(GL_MODELVIEW);
                ResetProjectionMode();

//            } else if(MPanel->Bool_diviser) {               // Ici en mode survol d'arêtes en mode division d'arêtes
            } else if(detection_survol_arete) {               // Ici en mode survol d'arêtes en mode division d'arêtes

//                printf("OnMouse + Bool_diviser\n");
                hits   = 0;   // Initialisation
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
                    if (WhoIsUnderTheMouse(hits, 1)) {
//                        m_gllist = glliste_lines; //0;     // On ne reconstruira que la liste des arêtes dans le Refresh() ci-dessous
                        // Il suffirait d'afficher la liste complète et d'y ajouter seulement le segment vert, pas tout recalculer !
                        Search_Min_Max();
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
                m_gllist = -1;          //glliste_objets;  // Pas besoin de regénérer la liste des lignes ici. Sera fait plus tard après clic sur la ligne.
                Refresh();
                glMatrixMode(GL_MODELVIEW);
                ResetProjectionMode();
            }
        }
//        finishdraw = true;
    }
    event.Skip();                   // Indispensable
}

void BddInter::OnMouseWheelMoved(wxMouseEvent& event) {
// Zoom avec la molette

/* Note : Lorsqu'on bouge la molette de plusieurs crans, on passe plusieurs fois par ici avec un Refresh() à chaque fois (fait dans OnMouseZoom).
 *        Dans l'idéal, il ne faudrait le faire que la dernière fois pour avoir directement l'effet cumulé, car sur de grosses bdd l'affichage est lent,
 *        en particulier si les points sont affichés. Dans ce cas, le ralentissement est aussi peut-être dû au fait qu'on cherche en plus à détecter des points
 *        survolés pour les identifier en jaune (car pas ce souci de lenteur avec les arêtes affichées).
 */
    int signe = event.GetWheelRotation();
    bool Touche_Maj  = event.ShiftDown();
    OnMouseZoom(event, signe, Touche_Maj);
    m_gldata.BeginX  = event.GetX();
    m_gldata.BeginY  = event.GetY();
    if (verbose) printf("event MouseWeelMoved\n");
//    event.Skip();                                       // N'a pas l'air utile, mais ne semble pas trop gêner !
}

/* Fixe les coordonnées du 2ème point (xa_sel,ya_sel) permettant de sélectionner des pixels dans le rectangle de sélection avec le premier point xd_sel,yd_sel*/
void BddInter::Selection_rectangle (GLint xa, GLint ya)
{
// A revoir ...
// ************
// Dans la version Tcl, on rentrait tout le rectangle dans la fonction de picking. Ici, ça ne marche pas ainsi et via testPicking, on teste point par point, ce qui est bien trop long.
// Du coup, on ne prend que quelques points dans le rectangle, mais ce n'est pas bon car on rate des facettes ou des points.

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
//    for (i=0; i<nb_points; i++) {
//        Cursor_X = Cursor_X0 + float(rand())/RAND_MAX*select_largeur;
//        Cursor_Y = Cursor_Y0 + float(rand())/RAND_MAX*select_hauteur;
//        testPicking(round(Cursor_X), round(Cursor_Y), modeGL, false) ;
//    }
	return;
}

/*Dessine le rectangle de sélection pendant la multi-sélection*/
void BddInter::Draw_Rectangle_Selection()
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
