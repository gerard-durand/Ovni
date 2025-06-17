#include "Interface.h"

// Gestion de l'interface BddInter avec Glut (Freeglut) et OpenGL
// **************************************************************

void BddInter::Output_Glut_Msg(GLfloat x, GLfloat y, char *text)
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

void BddInter::OnSize(wxSizeEvent& event)
{
    // Ne semble pas (ou plus) servir (sauf peut-être via EVT_SIZE
    if(verbose) printf("Entree BddInter::OnSize\n");

//    // this is also necessary to update the context on some platforms
//    wxGLCanvas::OnSize(event);
//    wxSize s;
//    s = event.GetSize();
//    OvniFrame* MAIN=dynamic_cast<OvniFrame*>(this->GetParent());
//    MAIN->Resize_OpenGL(s.GetWidth(), s.GetHeight());
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

void BddInter::OnPaint( wxPaintEvent& WXUNUSED(event) )
//void BddInter::OnPaint( wxPaintEvent& event )
{
    float   quat[4];
    GLfloat m[4][4];
    int multi_s = 0;
    static bool SetCurrentOK = false;

    static int old_size_x = frame_size_x;   // Initialisation (exécuté seulement une fois)
    static int old_size_y = frame_size_y;   // " "

    // must always be here
    wxPaintDC dc(this);
//    wxSize ClientSize = this->GetParent()->GetSize();

    if (verbose) printf("Entree BddInter::OnPaint\n");
#if wxCHECK_VERSION(3,0,0)
    if (!SetCurrentOK) {
        SetCurrent(*m_glRC);
        SetCurrentOK = true;    // Il suffit de faire le SetCurrent une seule fois
    }

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
    if(type_fichier != -1) {

        if (m_gldata.mode_Trackball) {
            build_rotmatrix( m, m_gldata.quat );
//            glMultMatrixf( &m[0][0] ) ;  // Ne doit pas servir à grand chose vu que la matrice précédente est Identity !
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

    if (type_fichier != -1) {
        if (type_dxf) {
//            printf("\ndraw dxf!!!\n");
//            m_renderer.Render();
            this->RenderDXF();
        } else {
            this->DrawOpenGL();
        }
    }

    // Flush
//    glFlush();    // Pas forcément utile car le SwapBuffers ci-dessous le ferait implicitement

    // Swap

//    event.Skip(); // ne pas faire si on a mis WXUNUSED(event) dans la déclaration de OnPaint
                    // si on met event, le Skip ne trace pas tout (ou seulement après avoir bougé la souris par ex !)

    wxRect frame_xy = this->GetGrandParent()->GetRect();    // Récupère la position et la taille de la fenêtre actuelle d'OvniFrame
    bool test = false;
    if ((old_size_x != frame_xy.width) || (old_size_y != frame_xy.height)) test = true;
    if (test) {                                             // Si la taille est différente, c'est qu'on a déplacé les bords et donc mettre à jour
        old_size_x = frame_size_x = frame_xy.width;
        old_size_y = frame_size_y = frame_xy.height;
        ini_file_modified = true;                           // Pour enregistrer ces nouvelles tailles en sortie d'Ovni
        if (verbose) {
            printf("tailles changées\n");
            printf("tailles : %d %d %d %d\n",frame_xy.width,frame_xy.height,old_size_x,old_size_y);
        }
    }

    SwapBuffers();

    if (verbose) printf("Sortie BddInter::OnPaint\n");
}

void BddInter::OnEraseBackground(wxEraseEvent& WXUNUSED(event)) {
    if(verbose) printf("Entree de OnEraseBackground\n");
    // Do nothing, to avoid flashing on MSW
    if(verbose) printf("Sortie de OnEraseBackground\n");
}

void BddInter::Reset_Sliders() {
// Calcul et affichage de la position des sliders/curseurs pour un quaternion donné dans m_gldata.quat
    build_rotmatrix(matquat, m_gldata.quat);
    CalculAngles(*matquat, m_gldata.rotx, m_gldata.roty, m_gldata.rotz);
    Slider_x->SetValue(lround(m_gldata.rotx));
    Slider_y->SetValue(lround(m_gldata.roty));
    Slider_z->SetValue(lround(m_gldata.rotz));
}

void BddInter::ResetProjectionMode() {
//    int static compteur=0;
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
//    MAIN->Resize_OpenGL(ClientSize.GetX(), ClientSize.GetY());
    if (verbose) {
        int w, h;
        GetClientSize(&w, &h);
        printf("ClientSize X/Y %d %d\n", w, h);
    }

#if wxCHECK_VERSION(3,0,0)
//    compteur++;

//    SetCurrent(*m_glRC);    // utile ?????
#else
#ifndef __WXMOTIF__
    if ( GetContext() )
#endif
        SetCurrent();
#endif // wxCHECK_VERSION
    if (verbose) {
        printf(" 6\n");fflush(stdout);
    }
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);   // ici .x et .y, pas .GetX et .GetY ? à vérifier peut-être
        glMatrixMode(GL_MODELVIEW);

    if (verbose) printf("Sortie ResetProjectionMode\n");
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

void BddInter::coloriserFacette(unsigned int indiceObjet, unsigned int indiceFacette, bool tracerFacette, const GLfloat couleur[3])
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

    Objet_courant    = &(this->Objetlist[indiceObjet]);

    if (Objet_courant->deleted) return;   // Ne rien faire sur un objet supprimé

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
    for (k=0; k<ns; k++) {
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
                for (k=0; k<ns; k++) {
                    xyz_sommet = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
                    Tracer_normale(xyz_sommet,NormaleFacette,1);
                }
            } else {
        // La facette n'est pas plane ...
                for (k=0; k<ns; k++) {
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
            for (k=0; k<ns; k++) {
                xyz_sommet = Objet_courant->Sommetlist[NumerosSommets[k]-1].getPoint();
                glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                if (NumerosSommets[k] == Numero_Sommet_a_Surligner) point_star = xyz_sommet;
            }
            glEnd();
            glPointSize(1);
            Genere_Etoile();
        }
    }
    glEnable(GL_LIGHTING);
    glColor4fv(couleurs); // Restaurer la couleur actuelle (R, G, B et alpha sauvegardés en début)
}

void BddInter::DrawOpenGL() {
// Boucles for non parallélisable à cause des listes OpenGL apparemment !
    char test;
    std::vector<int>   NumerosSommets;
    std::vector<float> xyz_sommet;
    std::vector<float> NormaleFacette;
    std::vector<float> NormaleSommet;
    Face    *Face_ij;
    Object  *Objet_i;
    unsigned int i=0,j=0,k=0;
    int grpmat, groupe, material, color_max;
    bool test2 = false; // Utilisé pour faire un double passage en mode visualisation du "Sens des normales", le premier en GL_CCW et le second en GL_CW
    bool test_np, lissage_Gouraud;

    unsigned int compteur=0;
    float couleurs_svg[4];

    volatile DWORD dwStart;
    bool test_timing = false;

    if (verbose)
        printf("Entrée BddInter::DrawOpenGL\n");

    glPointSize(5.0);
    longueur_normales = diagonale_save*len_normales*m_gldata.zoom/FoV_auto; // Attention : diagonale_save peut être modifié dans Search_Min_Max => effet indésirable potentiel ?
    color_max = nb_couleurs-1;  // Indice de la dernière couleur (plutôt que nombre de couleurs)

    if(show_CW_CCW == true) {
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
    }

//    printf("smooth : %d\n",smooth);

    if (test_timing) dwStart = GetTickCount();

    if( m_gllist == 0 ) {       // Reconstruire les listes, en particulier celle des objets

        if (verbose) printf("Reconstruction des listes\n");

        wxBeginBusyCursor();    // Active le curseur Busy pendant la génération des listes

        Search_Min_Max();       // Pour mettre à jour les différents compteurs d'objets, facettes, points ... Met aussi à jour diagonale_save et centrage_auto
        int nb_normales_seuillees = 0;      // A déclarer plutôt ailleurs, mais initialiser ici
        style = GL_POLYGON;                 // Déplacé ici, en dehors des boucles car, en l'état, invariant

        glDeleteLists(glliste_objets,1);    // Supprime la liste des objets
        m_gllist = glliste_objets;
        glNewList( glliste_objets, GL_COMPILE);
        if (MPanel->GetActiverTransparence()) {
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        }

        glInitNames();


// C'est ici qu'on revient pour le second passage en mode de visualisation du "Sens des normales"
Boucle:
        for (i=0; i<this->Objetlist.size(); i++) {

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

                unsigned int nb_faces = Objet_i->Facelist.size();

// Essai de parallélisation mais ne fonctionne pas. à cause des listes qui du coup ne sont pas générées dans le bon ordre ?
// Des tests placés différemment pour générer autant de listes (via glNewList) que de threads n'a pas non plus fonctionné !
// Est-ce parce qu'OpenGL ne peut gérer qu'une seule liste à la fois ? même si chaque thread gère une liste différente ?

////#pragma omp parallel for ordered private(Face_ij,NormaleFacette,NumerosSommets,NormaleSommet,\
                grpmat,groupe,material,lissage_Gouraud,k,test_np,xyz_sommet)
                for (j=0; j<nb_faces; j++) {

                    compteur++;
                    if (verbose) if ((compteur % 100000) == 0) {printf("\r%d",compteur); fflush(stdout);}

                    glPushName(j);

                    // Identifier les objets courants (facette, aspect, normale à la facette, luminance si Objet/facette non plat)
                    // => écriture + simple et execution + rapide

                    Face_ij = &(Objet_i->Facelist[j]);

                    if (Face_ij->afficher && !Face_ij->deleted) {
                        NormaleFacette.clear();
                        NumerosSommets.clear();

                        if (show_CW_CCW == false) {
                            if((groupes == false) && (materials == false)) {
                                glColor3fv(Face_ij->color); // Etait initialisé ou modifié dans colorface
                                glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avion)   ;
                                glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avion)   ;
                                glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
                                glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);

                                if (SelectionObjet != 0) { // On est en mode Repérage d'objets => Coloriser l'objet sélectionné en rouge
                                    if ((SelectionObjet == (int)i+1) || Objet_i->selected ) {            // 1er test utile ??
                                        glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionR)  ;
                                        glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionR)  ;
                                        glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionR) ;
                                        glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionR);
                                    }
                                }

                                // Colorisation d'un groupe ou d'un matériau particulier en bleu
                                if (GroupeMateriau[0] != 0) {

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
                                groupe   = Face_ij->getGroupe();
                                material = Face_ij->getCodmatface();
//                                  printf("objet %d facette %d groupe %d material %d\n",i,j,groupe,material);
                                if (groupe<0) {
                                    groupe=0;
                                } else if(groupe>color_max) {
                                    groupe=color_max;
                                }
                                if (material <0 ) {
                                    material = 10;      // Si material non initialisé (à -1) tracer en rouge comme un jet
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
                                    glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG)         ;
                                    glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG)        ;
                                } else {    // donc ici material est true
//                                        glColor3fv(color_groupe_material[material]) ;
                                    glColor3fv(MatAmbient_avionG[material]) ;   // Idem ci-dessus pour les groupes
                                    glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionG[material]);
                                    glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avionG[material]);
                                    glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG)         ;
                                    glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG)        ;
                                }
                            }
                        } else {
                            glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avion)   ;  // A faire, surtout suite à une colorisation des groupes ou des matériaux
                            glMaterialfv(GL_FRONT, GL_DIFFUSE,   MatDiffuse_avion)   ;
                            glMaterialfv(GL_FRONT, GL_SPECULAR,  MatSpecular_avionG) ;
                            glMaterialfv(GL_FRONT, GL_SHININESS, MatShininess_avionG);
                            // Ces 2 glColor3fv nécessitent que glColorMaterial et glEnable(GL_COLOR_MATERIAL) soient actifs dans InitGL;
                            // De plus il faut les glColor3fv(color_groupe ... ci-dessus) soient présents ! Pour éviter ce double coloriage, il faudrait traiter le sens des normales
                            // comme le reste, soit avec des glMaterialfv (cf version TCL) ou à la rigueur disable puis enable de GL_LIGHTING. à voir à l'occasion ...
                            if(test2 == true) {
                                glColor3fv(rouge);  // Vert initialement, mais souci en cas de sélection sur Sens de rotation inversé (même couleur => ne se voit pas).
//                              glMaterialfv(GL_FRONT, GL_AMBIENT,   MatAmbient_avionRG) ;    // Pas convainquant
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
//// A priori, le case 2 ne sert à rien et donc le switch est inutile => Lignes mises en commentaires par ////
////                        switch(Face_ij->Nb_Sommets_F) {
////                        case 2:                     // Lignes simples ou facette dégénérées réduites à 2 points !
////                            glBegin(GL_LINES);
////                            NumerosSommets = Face_ij->getF_sommets();
////                            for (k=0; k<NumerosSommets.size(); k++) {
////                                glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);  /// ??? BUG ??? d'où vient xyz_sommet dans ce cas ???? Mais a priori, on ne passe pas ici !!!
////                            }
////                            glEnd();
////                            break;

// NOTE : la distinction GL_TRIANGLES, GL_QUADS n'apporte pas grand chose / GL_POLYGON : c'est le même code C++ dans chaque "case"
//        => ne pas le dupliquer sauf si raisons de performances !

    //                     case 3:
    //                        glBegin(GL_TRIANGLES);
    // ...
    //                        glEnd();
    //                        break;
    //

////                        default:

                            glBegin(GL_POLYGON);
                            NumerosSommets = Face_ij->getF_sommets();
//                            style = GL_POLYGON; // Remplace la ligne ci-dessous ? Oui si mode=11 mais sinon ???? en l'état, à sortir des boucles de toutes façons

                            lissage_Gouraud = smooth && !Objet_i->flat && !Face_ij->flat && (Face_ij->Nb_Sommets_L != 0) ; // On pourrait utiliser aussi Face_ij->L_sommets.size() != 0
                            // Il faudrait peut-être aussi tester si Face_ij->Nb_Sommets_F == Face_ij->Nb_Sommets_L (ou Face_ij->F_sommets.size() == Face_ij->L_sommets.size() )
                            for (k=0; k<NumerosSommets.size(); k++) {
                                if (lissage_Gouraud) {
                                    // Facette non plane => utiliser le lissage de Gouraud (et donc les normales aux sommets)
                                    NormaleSommet = Objet_i->Vecteurlist[Face_ij->L_sommets[k]-1].point;
                                    test_np = Calcul_Normale_Seuillee(i,j,k,NormaleFacette,NormaleSommet) ;
                                    if (test_np) nb_normales_seuillees++;
                                    glNormal3f(NormaleSommet[0], NormaleSommet[1], NormaleSommet[2]); // NormaleSommet est un vector => glNormal3fv(NormaleSommet) ne marche pas !
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
////                            break;
////                        }
                    }
                    glPopName();
                }

                if (Rotation_Objets || Changer_Echelle) {
                    glPopMatrix();
                }

                glPopName();
            }   // Objet_i->afficher && !Objet_i->deleted

        }       // for (i=0; i<this->Objetlist.size(); ...

        if (test_timing) {printf("0-1 %6lu millisecondes\n", GetTickCount() - dwStart); dwStart = GetTickCount();}

        if (verbose) {printf("\r%d\n",compteur); fflush(stdout);}

        // Si le mode "Sens des normales" est activé : Pour forcer une seconde exploration complète des boucles sur i et j en changeant le mode glFrontFace
        if ((test2 == false) && (show_CW_CCW == true)) {
            glFrontFace(GL_CW);
            test2 = true;
            goto Boucle;    // Pas joli, pas pour les puristes, mais c'est simple, évite un test sur l'indice i+1 et de mettre i à -1 pour forcer l'itération suivante avec i=0 ...
        }

        if (show_CW_CCW == true) {
            glFrontFace(GL_CCW);    // Remettre la valeur par défaut
            glDisable(GL_CULL_FACE);
        }

        if (MPanel->GetActiverTransparence()) {
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        }

        glEndList();

        if (MPanel->GetBoolSouder()) {
            modeGL = points;
        } else if(MPanel->GetBoolDiviser()) {
            modeGL = aretes;
        } else {
            modeGL = standard;
        }

        SetPosObs(reset_zoom);
        ResetProjectionMode();

        if (Genere_Tableau_Aretes_OK && (nb_facettes < nb_facettes_test)) {       // Si arêtes non générées, faire plutôt un test sur nb_facettes et non sur nb_aretes_test
            buildAllLines();
        } else {                                // Ne pas générer la liste maintenant si beaucoup d'arêtes au total
            printf("Génération de la liste OpenGL des arêtes ... différée à plus tard !\n");
            liste_aretes_OK  = false;
        }
        if (nb_sommets  < nb_sommets_test)  {
            buildAllPoints();
        } else {                                // Idem pour le nombre total de sommets
            printf("Génération de la liste OpenGL des sommets... différée à plus tard !\n");
            liste_sommets_OK = false;
        }
        buildBoite();
        buildRepereOXYZ();
        BuildAllFacettes_Selected();

        finishdraw = true;
        m_gllist   = -1;        // glliste_objets; // Déjà comme ça en principe
        Refresh();              // S'il n'est pas là, pas d'affichage des objets la première fois ! mais du coup, double passage dans DrawOpenGL

        if (wxIsBusy()) wxEndBusyCursor();          // Listes générées : arrêter le curseur Busy

    }   // if (m_gllist == 0)

    if (test_timing) {printf("1-2 %6lu millisecondes\n", GetTickCount() - dwStart); dwStart = GetTickCount();}

    // En dehors des créations de liste => fait systématiquement

    if (!wxIsBusy()) wxBeginBusyCursor();   // N'activer le curseur Busy que s'il ne l'est pas déjà

    if (m_gllist == glliste_lines) {
        if (verbose) printf("Reconstruction de la liste de aretes seulement\n");
        buildAllLines();
        m_gllist = -1;      //glliste_objets;
    }
    if (m_gllist == glliste_points) {                                           // le signe - utilisé auparavant n'est plus justifié
        if (verbose) printf("Reconstruction de la liste de points seulement\n");
        buildAllPoints();
        m_gllist = -1;      //glliste_objets;
    }
    if (m_gllist == glliste_points_selected) {                                           // le signe - utilisé auparavant n'est plus justifié
        if (verbose) printf("Reconstruction de la liste de points sélectionnés seulement\n");
        buildAllPoints_Selected();
        m_gllist = -1;      //glliste_objets;
    }
    if (m_gllist == glliste_select) {
        if (verbose) printf("Reconstruction de la liste des facettes sélectionnées\n");
        BuildAllFacettes_Selected();
        m_gllist = -1;      //glliste_objets;
    }

    if (verbose) printf("Affichage des listes\n");

    if (show_plein)  {
        glCallList(glliste_objets);     // La valeur m_gllist semble écrasée ou différente de 1 dans certains cas (grosses BDD notamment) : pourquoi ?
        if (verbose) printf("DrawOpenGL : liste objets\n");
        if ((mode_selection == selection_facette) && !MPanel->GetBoolSouder() && !ToSelect.ListeSelect.empty()) {
            if (verbose) printf("liste select\n");
            glCallList(glliste_select);
        }
    }
    if (show_lines)  {
        if (verbose) printf("DrawOpenGL : liste lines\n");
        glCallList(glliste_lines) ;     // <=> showAllLines() mais appel direct de la liste donc bien plus rapide;
    }
    if (show_points) {
        if (verbose) printf("DrawOpenGL : liste points\n");
        glCallList(glliste_points);     // <=> showAllPoints()  ""  ;
        if (pointsSelected) glCallList(glliste_points_selected);
        showPoint_Selection();          // Pour n'afficher que les points surlignés en jaune au survol ou un premier point sélectionné en rouge lors d'une soudure
    }
//    if (m_gllist == -1) showPoint_Selection();
    if (show_box)   glCallList(glliste_boite) ;     // <=> AffichageBoite() ""  ;
    if (show_axes)  glCallList(glliste_repere);     // <=> repereOXYZ()     ""  ;
    if (show_light) AfficherSource();               // Une liste pourrait être utile ? pas sûr !
    if (show_star)  Genere_Etoile();

    if (segment_surligne) showSegment_Selection();          // Pas de liste ici ; génération directe du segment surligné

    if (Symetrie_Objets) {                  // Tracer une boîte englobante rouge autour l'objet d'origine et une cyan pour matérialiser l'objet à créer
        glColor3fv(rouge);
        TracerBoite(x1_b1,x2_b1,y1_b1,y2_b1,z1_b1,z2_b1);   // Boîte de l'objet d'origine
        glColor3fv(cyan);
        TracerBoite(x1_b2,x2_b2,y1_b2,y2_b2,z1_b2,z2_b2);   // Boîte de l'objet à créer par symétrie
    }

    if (Facette_Surlignee) {                // Faire en dehors de la génération de liste car ici, on a une seule facette à traiter
//        printf("Facette surlignee : %d %d\n",Numero_Objet_a_Surligner, Numero_Facette_Surlignee);
        coloriserFacette(Numero_Objet_a_Surligner, Numero_Facette_Surlignee, true, jaune); // Ici tracé de la facette en jaune et des normales (si demandées)
    }

    if (wxIsBusy()) wxEndBusyCursor();      // Désactiver le curseur Busy uniquement s'il est actif

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
        glGetFloatv(GL_CURRENT_COLOR, couleurs_svg);    // Conserver l'état actuel des couleurs
        glColor3fv(bleu);                               // Ecrire en bleu le message de Frames par secondes dans l'image, en bas, à gauche.
        Output_Glut_Msg(0.005,0.01,Message_fps);
        glColor4fv(couleurs_svg);                       // Restituer les couleurs
    }

    if (test_rectangle_selection) Draw_Rectangle_Selection();   // Trace un rectangle pour sélectionner les facettes qui sont internes

    SetFocus(); // Donne le focus à la fenètre OpenGL => utilisation des touches de raccourcis clavier

    if (test_timing) {printf("2-3 %6lu millisecondes\n", GetTickCount() - dwStart); dwStart = GetTickCount();}

    if (verbose)
        printf("Sortie BddInter::DrawOpenGL\n");
}

void BddInter::showPoint_Selection() {

    std::vector<float> xyz_sommet;
    Object *objet_courant;

    if(Smemory != nullptr) {                                        // Tracé en rouge du point déjà sélectionné
        objet_courant = &(this->Objetlist[Smemory->objet]);
        if (objet_courant->afficher && !objet_courant->deleted){
            glDisable(GL_LIGHTING);
            glBegin(GL_POINTS);
            xyz_sommet = objet_courant->Sommetlist[Smemory->sommet].getPoint();
            glColor3fv(rouge);
            if (!xyz_sommet.empty()) glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            glEnd();
            glEnable(GL_LIGHTING);
            pointsSelected = true;
        }
    }
    if (objet_under_mouse != -1 && point_under_mouse != -1) {       // Tracé en jaune du point survolé
        objet_courant = &(this->Objetlist[objet_under_mouse]);
        if (objet_courant->afficher && !objet_courant->deleted){
            glDisable(GL_LIGHTING);
            glBegin(GL_POINTS);
            xyz_sommet = objet_courant->Sommetlist[point_under_mouse].getPoint();
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
// Liste de tous les points
    glDeleteLists(glliste_points,1);                // Détruire une éventuelle liste de points existante
    glNewList(glliste_points, GL_COMPILE) ;         // GL_COMPILE seulement ici
    glInitNames();
    showAllPoints();
    glEndList();

    buildAllPoints_Selected();

    liste_sommets_OK = true;
}

void BddInter::buildAllPoints_Selected() {
// Utilise showAllPoints_selected pour construire une liste de points

    if (verbose)
        printf("Entrée BddInter::buildAllPoints_Selected\n");

// Liste des points sélectionnés seulement
    glDeleteLists(glliste_points_selected,1);       // Détruire une éventuelle liste de points existante
    glNewList(glliste_points_selected, GL_COMPILE); // GL_COMPILE seulement ici
    glInitNames();
    showAllPoints_Selected();
    glEndList();

    liste_sommets_OK = true;
}

void BddInter::showAllPoints() {
    std::vector<float> xyz_sommet;
    Object *objet_courant;
    Sommet *sommet_courant;

//    pointsSelected  = false;

    glDisable(GL_LIGHTING);

    for (unsigned int i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        if (objet_courant->afficher && !objet_courant->deleted){
//            glPushName(i+1);                    // +1 ??
            glPushName(i);
            for (unsigned int j=0; j<objet_courant->Sommetlist.size(); j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                if (sommet_courant->show == true) {
//                    glPushName(j+1);            // +1 permet d'identifier par le numéro du point et non l'indice dans le tableau
                    glPushName(j);
                    xyz_sommet = sommet_courant->getPoint();
                    glPushName(-1);
                    glBegin(GL_POINTS);
                    glColor3fv(bleu);                                       // Point bleu standard
//                    if (sommet_courant->selected) {
//                        glColor3fv(vert);                                   // Point vert si on le sélectionne
//                        pointsSelected = true;
//                    }
                    if (Changer_Echelle) {
                        if (sommet_courant->selected) {
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

void BddInter::showAllPoints_Selected() {
    std::vector<float> xyz_sommet;
    Object *objet_courant;
    Sommet *sommet_courant;

    pointsSelected  = false;

    glDisable(GL_LIGHTING);

    for (unsigned int i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        if (objet_courant->afficher && !objet_courant->deleted){
//            glPushName(i+1);                    // +1 ??
            glPushName(i);
            for (unsigned int j=0; j<objet_courant->Sommetlist.size(); j++) {
                sommet_courant = &(objet_courant->Sommetlist[j]);
                if ((sommet_courant->show == true) && (sommet_courant->selected) ) {
//                    glPushName(j+1);            // +1 permet d'identifier par le numéro du point et non l'indice dans le tableau
                    glPushName(j);
                    xyz_sommet = sommet_courant->getPoint();
                    glPushName(-1);
                    glBegin(GL_POINTS);
                    glColor3fv(vert);                                       // Point vert si on le sélectionne
                    pointsSelected = true;
                    if (Changer_Echelle) {
                        if (sommet_courant->selected) {
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
    glNewList(glliste_lines, GL_COMPILE) ;  // GL_COMPILE seulement ici
    glInitNames();
    showAllLines();
    glEndList();
    liste_aretes_OK = true;
}

void BddInter::showAllLines() {

// Version GD utilisant la génération préalable des arêtes de Genere_Tableau_Aretes.
// Liste de toutes les arêtes. La ligne survolée (en vert) sera générée à part, hors liste.

    std::vector<float> xyz_sommet;
    unsigned int i,j,k;
    Aretes  *p_Arete;
    Object  *objet_courant;

    if (verbose)
        printf("Entrée BddInter::showAllLines\n");

    GLfloat l_width_n = 1.;                 // Largeur de ligne normale (1 devrait aller mais un peu juste pour sélectionner !)
    GLfloat l_width_e = l_width_n*1.25;     // Augmenter un peu la largeur de ligne pour l'antialiasing soft. Initialement 1.75

    glDisable(GL_LIGHTING);
    glLineWidth(l_width_n);
    glColor3fv(blanc);      // initialement : cyan +clair

    if (antialiasing_soft) {
        glLineWidth(l_width_e);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    }

    for (i=0; i<this->Objetlist.size(); i++) {
        objet_courant = &(this->Objetlist[i]);
        if (objet_courant->afficher && !objet_courant->deleted){
            int Nb_lignes=0;
            glPushName(i);
            unsigned int nbAretes = objet_courant->Areteslist.size();
            std::list<Aretes>::iterator itArete = objet_courant->Areteslist.begin();
//            int tid;
////#pragma omp parallel for private(p_Arete,xyz_sommet,tid)    // Ne marche pas ici car liste OpenGL
            for (j=0; j<nbAretes; j++,itArete++) {
//                p_Arete = &(objet_courant->Areteslist[j]);
                p_Arete = &(*itArete);
//                tid = omp_get_thread_num();
                if (p_Arete->deleted) continue;       // L'arête a été supprimée (doublon ou autre raison), passer à la suivante
//                printf("j:%d n:%d\n",j,tid);
                Nb_lignes++;
                glPushName(j);
                glPushName(j);  // Un de plus (pour compatibilité avec la version originale et l'offset dans WhoIsUnderTheMouse) !
                if (p_Arete->afficher) {
//                    if (antialiasing_soft) {
//                        glLineWidth(l_width_e);
//                        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); // A mettre ailleurs ?
//                        glEnable(GL_LINE_SMOOTH);
//                        glEnable(GL_BLEND);
//                        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
//                    }
                    glBegin(GL_LINES);
                        xyz_sommet = objet_courant->Sommetlist[p_Arete->ind_a].getPoint();
                        glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                        xyz_sommet = objet_courant->Sommetlist[p_Arete->ind_b].getPoint();
                        glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
                    glEnd();
//                    if (antialiasing_soft) {
//                        glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
//                        glDisable(GL_LINE_SMOOTH);
//                        glDisable(GL_BLEND);
//                        glLineWidth(l_width_n);
//                    }
                }
                glPopName();
                glPopName();
            }
            if (verbose) printf("Nombre de lignes à tracer : %d\n",Nb_lignes);
        }
        glPopName();
    }
    if (antialiasing_soft) {
        glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
        glLineWidth(l_width_n);
    }
    glLineWidth(l_width_n); // Par précaution
    glEnable(GL_LIGHTING);
}

void BddInter::showSegment_Selection() {

// Version GD utilisant la génération préalable des arêtes de Genere_Tableau_Aretes.
// Reprise de l'ancien shwAllLines, générant toutes les lignes systématiquement.

    std::vector<float> xyz_sommet;
    unsigned int i,j,k;
    Aretes *p_Arete;
    Object *objet_courant;

    GLfloat l_width_n = 1.;     // Largeur de ligne normale
    GLfloat l_width_e = 3.;     // Largeur de ligne plus épaisse

    if((objet_under_mouse == -1) && (line_under_mouse == -1)) return;

    objet_courant = &(this->Objetlist[objet_under_mouse]);
    if (objet_courant->afficher && !objet_courant->deleted){
//        p_Arete = &(objet_courant->Areteslist[line_under_mouse]);
        std::list<Aretes>::iterator itArete = objet_courant->Areteslist.begin();
        std::advance(itArete,line_under_mouse);
        p_Arete = &(*itArete);
        if (p_Arete->deleted) return;           // L'arête a été supprimée : ignorer

        if (p_Arete->afficher) {
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
            xyz_sommet = objet_courant->Sommetlist[p_Arete->ind_a].getPoint();
            glVertex3f(xyz_sommet[0],xyz_sommet[1],xyz_sommet[2]);
            xyz_sommet = objet_courant->Sommetlist[p_Arete->ind_b].getPoint();
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
//        fgError("Failed to allocate memory in fghCircleTable");
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

//    FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidSphere" );

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

    for (i=1; i<stacks-1; i++ )
    {
        z0 = z1; z1 = cost2[i+1];
        r0 = r1; r1 = sint2[i+1];

        glBegin(GL_QUAD_STRIP);

            for (j=0; j<=slices; j++)
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
    norme = sqrt(x*x + y*y + z*z) ;
    return norme;
}

void BddInter::buildBoite() {
    float dx,dy,dz ;

//    printf("Afficher la boite\n");

// Peut être fait une fois, puis stocké dans une liste.
// Réinitialiser seulement si changements dans les min et max...

    glDeleteLists(glliste_boite,1);         // Supprimer une éventuelle liste 4 existante
    glNewList(glliste_boite, GL_COMPILE);   // GL_COMPILE seulement ici

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

void BddInter::Genere_Etoile() {
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

void BddInter::buildRepereOXYZ() {

    glDeleteLists(glliste_repere,1);        // Supprimer une éventuelle liste 5 existante
    glNewList(glliste_repere, GL_COMPILE);  // GL_COMPILE seulement ici
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
    m_gldata.posz = -m_gldata.fmult_diagonale*diagonale_save -centreRot[2] ;     //Mieux que 1000 surtout sur des bases dont les min-max sont dans ces valeurs. Peut-être mettre plus 10* par exemple au lieu de 4 ?

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
        style = GL_POLYGON;             // style plein
    } else if (mode == (int)points) {
        if (verbose) printf("points ");
        width = width_point;
        style = GL_POINTS;              // style points
    } else if (mode == (int)aretes) {
        if (verbose) printf("aretes ");
        width = width_ligne;
        style = GL_LINE_LOOP;           // style fils de fer
    } else {
        if (verbose) printf("inconnu ! ");
        width = width_pixel;
        style = GL_POLYGON;             // style plein
    }
    if (verbose) printf("(%d)\n",mode);

//    selectMode(mode);   // Si on est en GL_POINTS on devrait avoir mode != standard !!!!
    wxSize ClientSize = this->GetSize();
    this->SetSize(ClientSize.x, ClientSize.y);
//    GLuint *selectBuffer;
    GLint hits=0;
    GLint viewport[4];

//    selectBuffer = (GLuint*)malloc(BUFSIZE*sizeof(GLuint)); // En statique et beaucoup + gros en version Tcl (BUFSIZE=102400 !!)

    glGetIntegerv(GL_VIEWPORT, viewport);

    glSelectBuffer(BUFSIZE, selectBuffer);
    glRenderMode(GL_SELECT);

    glInitNames();    // Sert à quoi ? Fait aussi dans DrawOpenGL ...
    glPushName(0);    // idem

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    if (type_fichier != -1) {
        if (type_dxf) {
            //printf("\ndraw dxf!!!\n");
//            m_renderer.Render();
            this->RenderDXF();  // Spécifique fichier .dxf. C'est RenderDXF qui assure la compatibilité avec les bases Internes d'Ovni (BddInter).
        } else {
            //printf("\ndraw a .bdd file !!!\n");
            if (mode == standard) { // || mode == points) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width, width, viewport);
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
//                    glInitNames();    // C'est ici en version Tcl ! mais ne change rien !!
//                    glPushName(0);
                this->DrawOpenGL();
            }
            if (MPanel->GetBoolSouder() || mode == points) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width_point, width_point, viewport); // cf version tcl ligne 6554
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
                glCallList(glliste_points);
                face_under_mouse = -1;
                if ((MPanel->GetBoolSouder()) && (point_under_mouse != -1)) {
                    ToSelect.ListeSelect.clear();
                    m_gllist = 0;   // Refaire toutes les listes car les facettes, les arêtes ont changé (les points sont à jour : on pourrait éviter de les reconstruire !)
                } else {
                    m_gllist = -1;  //glliste_objets;
                }
                Refresh();
            }
            if (MPanel->GetBoolDiviser()) {
                gluPickMatrix((GLdouble)cursorX, (GLdouble)(viewport[3] -cursorY -offset_pointeur), width_ligne, width_ligne, viewport); // cf version tcl ligne 6552
                gluPerspective(m_gldata.zoom, (GLfloat)ClientSize.x/ClientSize.y, m_gldata.zNear, m_gldata.zFar);
                glMatrixMode(GL_MODELVIEW);
                glCallList(glliste_lines);
                line_under_mouse = -1;  // Raz du numéro d'arête survolé par la souris
                objet_under_mouse= -1;
                Refresh();
            }
            //glTranslatef( m_gldata.posx, m_gldata.posy,m_gldata.posz );
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

//void BddInter::stopPicking() {
//    ResetProjectionMode();
//}

void BddInter::processHits(GLint hits, bool OnOff) {

// Appel dans testPicking
// Fonction très proche de WhoIsUnderTheMouse, du moins au début. A t-on besoin des 2 ?

    int i, ii, offset;
    int objet, face, point;
    GLuint z;
    GLuint *ptr=nullptr;

    bool test_print = false ;       // Mettre true pour activer des impressions de test et commenter la ligne en dessous (ou activer verbose via lettre v)!
    test_print = verbose;
//    test_print = true;

    unsigned int indice_pt, indice_vec;
    int line;

    Object *objet_courant;
    Face   *new_facette;

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

        if (test_print) {
            printf("\nhits en entree de processHits : %d\n",hits);
            printf("style : %d (POINTS:%d LINE_LOOP:%d POLYGON:%d)\n",style,GL_POINTS,GL_LINE_LOOP,GL_POLYGON);
        }

        GLuint val0 = *ptr;

        if (val0 < 2) return;   // pourquoi ? si 0, c'est le fond, si 1 il y a un pb => offset varie => décalages dans les ptr de 1 ou 2 => mieux vaut ignorer

        int incrHits = 0;
        if (val0 != 0) incrHits = 1;    // Si val0 == 0, c'est qu'on est sur le fond <=> hits = 0 !
        int newHits  = 0;

        for (i = 0; i < hits; i++) {
            if (*ptr == val0)
                newHits += incrHits;    // On incrémente tant que la première valeur est égale = val0, sinon on s'arrête (<=>modif de hits)
            else
                incrHits=0;
            if (test_print) {
                printf("0 : %u;\n",       ptr[0]);
                printf("1 : %u; z\n",     ptr[1]);
                printf("2 : %u; z2\n",    ptr[2]);
                printf("3 : %u; objet\n", ptr[3]);
                if (style == GL_POINTS)
                    printf("4 : %u; point\n",   ptr[4]);
                else
                    printf("4 : %u; facette\n", ptr[4]);
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
            if (MSelect->IsShown()) {   // On peut le faire même si MSelect n'est pas affiché
                MSelect->TextCtrl_NomObjet->ChangeValue(wxString(this->Objetlist[objet].GetName(), wxConvUTF8));    // si fichier bdd en utf8
    //            MSelect->TextCtrl_NomObjet->ChangeValue(this->Objetlist[objet].GetwxName());
                str.Printf(_T("%d"),this->Objetlist[objet].GetNumero());
                MSelect->TextCtrl_NumObjet->SetValue(str);
                str.Printf(_T("%d"),objet);
                MSelect->TextCtrl_IndObjet->SetValue(str);
                str.Printf(_T("%d"),point);
                MSelect->TextCtrl_IndFacette->SetValue(str); // Case renommée en numéro de point dans ce cas
                str.Printf(_T("%d"),point+1);
                MSelect->TextCtrl_NumFacette->SetValue(str);
            }
            if (ToSelect.check_if_in_ListeSelect(objet,point)) {
//                printf("point deja present => l'enlever en mode OnOff actif\n");
                if (creation_facette && (ToSelect.ListeSelect.size() > 1)) { // Ne pas passer ici si 1 seul point dans ListeSelect (c'est alors 2 x le 1er)
                    if ((ToSelect.ListeSelect[0].objet == objet) && (ToSelect.ListeSelect[0].face_sommet == point)) {
                        printf("Bouclage sur le premier point\n");
                        unsigned int new_size, new_indice;
                        bool UnSeulObjet=true;
                        for (i=1; i<(int)ToSelect.ListeSelect.size(); i++) {
                            if (ToSelect.ListeSelect[0].objet != ToSelect.ListeSelect[i].objet) { // Si objets différents, créer de nouveaux sommets dans le 1er objet listé
//                                UnSeulObjet = false;                                                                  // N'a plus lieu d'être
                                indiceObjet_courant = ToSelect.ListeSelect[i].objet;
                                objet_courant       = &(this->Objetlist[indiceObjet_courant]);                          // Objet [i] différent de l'objet [0]
                                Sommet NewSommet    = objet_courant->Sommetlist[ToSelect.ListeSelect[i].face_sommet];   // Récupère le sommet du deuxième objet
                                objet_courant->Sommetlist[ToSelect.ListeSelect[i].face_sommet].selected = false;        // Marque l'ancien sommet comme non sélectionné
                                indiceObjet_courant = ToSelect.ListeSelect[0].objet;                                    // Retour à l'objet [0]
                                objet_courant       = &(this->Objetlist[indiceObjet_courant]);
                                NewSommet.Numero    = objet_courant->Sommetlist.size() +1;                              // Metrre à jour le nouveau numéro du sommet à créer
                                objet_courant->Sommetlist.push_back(NewSommet);                                         // Ajout de ce nouveau sommet à l'objet [0]
                                objet_courant->Nb_sommets++;                                                            // Mise à jour du nombre de sommets dans l'objet [0]
                                ToSelect.ListeSelect[i].objet       = ToSelect.ListeSelect[0].objet;                    // Mise à jour dans ListeSelect de objet et face_sommet
                                ToSelect.ListeSelect[i].face_sommet = NewSommet.Numero -1;
                            }
                        }
                        indiceObjet_courant = objet;
                        objet_courant = &(this->Objetlist[objet]);
                        new_size = new_indice = objet_courant->Facelist.size();
                        if (objet_courant->Nb_facettes_original == 0) objet_courant->Nb_facettes_original = new_size;   // Sauvegarde initiale du nombre de facettes
                        new_size++;
                        N_elements = new_size;
                        str.clear();
                        NumerosSommets.resize(ToSelect.ListeSelect.size());
                        for (i=0; i<(int)ToSelect.ListeSelect.size(); i++) NumerosSommets[i] = ToSelect.ListeSelect[i].face_sommet +1; // Numéros décalés d'1 cran / indices
                        make_1_face();
                        objet_courant->Nb_facettes = new_size;
                        Calcul_Normale_Barycentre(indiceObjet_courant,new_indice);  // Ici la facette créée est plane
                        objet_courant->Facelist[new_indice].selected = true;
                        Genere_Tableau_Points_Facettes(objet_courant);
                        Genere_Tableau_Aretes(objet_courant);
                        new_facette    = &(objet_courant->Facelist[new_indice]);
                        new_facette->groupe     = MPanel->GetNumeroGroupe();        // OK mais l'effet n'est pas immédiat si affichage coloré (via bouton groupes ou matériaux)
                        new_facette->codmatface = MPanel->GetNumeroMateriau();
                        NumerosSommets = new_facette->F_sommets;
                        new_facette->L_sommets    = NumerosSommets;                 // Recopie provisoire des numéros de sommets de facettes dans les numéros de vecteurs pour les lumminances
                        new_facette->Nb_Sommets_F = new_facette->F_sommets.size();  // Ajustement des tailles des vecteurs = sommets
                        new_facette->Nb_Sommets_L = new_facette->L_sommets.size();  //                                       et luminances
                        // Augmenter le nombre de vecteurs car les nouvelles normales aux sommets ne remplacent pas forcément les anciennes, notamment
                        // quand une normale spécifique est utilisée par 2 sommets ou plus. Peut arriver en cas de simplification de Bdd !
                        indice_vec = objet_courant->Vecteurlist.size();             // Initialise indice_vec avec la taille actuelle des vecteurs
                        new_size   = indice_vec + new_facette->Nb_Sommets_L;        // Nouvelle taille
                        objet_courant->Vecteurlist.resize(new_size);                // Resize dzs vecteurs
                        objet_courant->Nb_vecteurs = new_size;                      // et maj de Nb_vecteurs
                        for (i=0; i<(int)NumerosSommets.size(); i++) {
//                               printf("indice %d, Numero de sommet : %d\n",i,NumerosSommets[i]);
                            indice_pt = NumerosSommets[i]-1;
                            Genere_Normale_1_Sommet(objet_courant,indice_pt,indice_vec);
                            indice_vec++;
                        }
                        objet_courant->Facelist[new_indice].flat = MPanel->GetFacetteCreeePlane();   // dépend de la case à cocher "Plane" du dialogue "ModificationPanel
                        ToSelect.verrouiller_ListeSelect(false);
                        MPanel->Button_SupprimerFacette->Enable();
                        MPanel->Button_InverserNormale ->Enable();
                        SELECTION old  = mode_selection;
                        mode_selection = selection_point;
                        wxKeyEvent key_event;
                        key_event.m_keyCode = 'S';
                        OnKeyDown(key_event);                       // Simule une pression sur la touche S au clavier => Reset de la sélection de points
                        mode_selection = old;

                        m_gllist = 0;
                        Refresh();

                    } else if (OnOff) {
                        ToSelect.erase_one(objet,point);            // Si le point est déjà dans la liste et que ce n'est pas le 1er, le supprimer
                        Objetlist[objet].Sommetlist[point].selected = false;
                    }
                } else if (OnOff) {
                    ToSelect.erase_one(objet,point);                // Si OnOff est true et si le point est déjà dans la liste, le supprimer
                    Objetlist[objet].Sommetlist[point].selected = false;
                }
            } else {
//                printf("point absent       => l'ajouter\n");
                ToSelect.add_one(objet,point);                      // N'ajouter que si non déjà présent dans la Liste
                Objetlist[objet].Sommetlist[point].selected = true;
            }

            printf ("Nombre de points dans Liste : %d\n",(int)ToSelect.ListeSelect.size()); //vérification que le clic sur un point a été pris en compte
            if (MSelect->IsShown()) {                               // On peut le faire même si MSelect n'est pas affiché
                str.Printf(_T("%d"),(int)ToSelect.ListeSelect.size()); // + (int)ToSelect.Liste.size());
                MSelect->TextCtrl_Selection->SetValue(str);
            }

            if (MPanel->GetBoolSouder()) {
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
                        pointsSelected = false;                                 // Ceinture et bretelles
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

            Diviser_Arete(objet, face, line);
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
                        ToSelect.erase_one(objet,face);                         // Sinon, le supprimer
                            colorface(objet, face, false);
                    }
                } else {
                    if (!ToSelect.check_if_in_ListeSelect(objet,face)) {        // N'ajouter que si non déjà présent dans la Liste
                            ToSelect.add_one(objet,face);
                            colorface(objet, face, true);
                        }
                    }
                }
            }

            if (mode_selection == selection_objet) {
//                if (objet == -1) return;      /// Pour l'instant c'est mieux que de passer par l'affichage de "Fond" et le comptabiliser comme un objet
                SelectionObjet = -1 ; //objet+1; //this->Objetlist[objet].GetValue();   // Valeur < 0 pour DrawOpenGL, la sélection se faisant via l'attribut selected de Objetlist[objet]
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
                    str.Printf(_T("%d"),this->Objetlist[objet].GetNumero());     // Numéro de l'objet (!= son indice)
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
                if (MSelect->RadioButton_Grp->GetValue()) {
                    // Groupe est sélectionné
                    n_val = listeGroupes.size();
//                    it = listeGroupes.begin();
                    str.Printf(_T("%d"), (int)*it++);
                    str_grpmat += str;
                    for (i=1; i<n_val; i++) {
                        str.Printf(_T(", %d"), *it++);
                        str_grpmat += str;
                    }
                } else {    // <=> MSelect->RadioButton_Mat->GetValue()
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
                MSelect->TextCtrl_NumerosUtilises->SetValue(str_grpmat);    // Plutôt que SetLabel pour wxWidgets 3.3
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
//                    for (iobj=0; iobj<n; iobj++)
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

// Ne sert plus à grand chose ... notamment ne colorise plus la facette sélectionnée, car fonction reportée dans la création de liste

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

bool BddInter::WhoIsUnderTheMouse(GLint hits, int mode_appel) {

// Appel dans OnMouse, tests sur MPanel->Bool_souder (ou juste points affichés) et MPanel->Bool_diviser, respectivement lors du survol de points ou d'arêtes
// Si OK, retourne true et initialise objet_under_mouse, face_under_mouse, point_under_mouse et/ou line_under_mouse

// Fonction très proche de processHits, du moins au début. A t-on besoin des 2 ? Les mutualiser ?

// mode_appel=0 si appel dans le test souderPoints
// mode_appel=1 si appel dans le test Diviser_Arete

    int i, ii, offset;
    int objet, face, point;
    GLuint z;
    GLuint *ptr=nullptr;

    bool test_print = false ;       // Mettre true pour activer des impressions de test et commenter la ligne en dessous (ou activer verbose via lettre v)!
    test_print = verbose;
//    test_print = true;

    if (verbose) printf("Entree de WhoIsUnderTheMouse\n");

    if (hits >= BUFSIZE || hits < 0) {
        if (hits > 0) {// Si valeur négative, ne rien afficher car ce n'est pas toujours une erreur ! Peut-être pb de timing
            std::cout<<"trop de points cliques ou valeur negative :  ";
            std::cout<< hits;
            std::cout<<" \n";
        }
        objet_under_mouse = face_under_mouse = point_under_mouse = line_under_mouse = -1;
        return false;
    } else {
        /*!on initialise le pointeur*/
        ptr = (GLuint *) selectBuffer;

        offset = 6; // ici on est soit en GL_POINTS, soit en GL_LINE*.              ????? Pourquoi dans processHits offset vaut 5 en GL_POINTS ?

        if (test_print) {
            printf("\nhits dans WhoIsUnderTheMouse : %d\n",hits);
        }

        GLuint val0 = *ptr;

        if (val0 < 2) {
            objet_under_mouse = face_under_mouse = point_under_mouse = line_under_mouse  = -1;
            return false;     // pourquoi ? si 0, c'est le fond, si 1 il y a un pb => offset varie => décalages dans les ptr de 1 ou 2 => mieux vaut ignorer
        }

//// Test
        if (test_print) {
            for (i = 0; i < hits; i++) {
                printf("0 : %u;\n",       ptr[0]);
                printf("1 : %u; z\n",     ptr[1]);
                printf("2 : %u; z2\n",    ptr[2]);
                printf("3 : %u; objet\n", ptr[3]);
                if (mode_appel == 0)
                    printf("4 : %u; point\n",   ptr[4]);
                else
                    printf("4 : %u; facette\n", ptr[4]);
                printf("5 : %d; ID ou line\n",  ptr[5]);
                ptr += offset;
            }
            ptr = (GLuint *) selectBuffer;
        }
//// Fin du test

        z=ptr[1];
        if (test_print) printf("z : %u; ",  z);
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

