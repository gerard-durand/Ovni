#include "GLCanvas.h"
#include "trackball.h"
#include <wx/dcclient.h>
#include <wx/string.h>


const float to_Deg=180.0f/M_PI;
//const float pas_deg=to_Deg/10.   // 1/10eme de radian
const float pas_deg=5.0f;
const float pas_rad=pas_deg/to_Deg;


/*BEGIN_EVENT_TABLE(TestGLCanvas, wxGLCanvas)
    EVT_SIZE(TestGLCanvas::OnSize)
    EVT_PAINT(TestGLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(TestGLCanvas::OnEraseBackground)
    EVT_KEY_DOWN( TestGLCanvas::OnKeyDown )
    EVT_MOUSE_EVENTS(TestGLCanvas::OnMouse)
//    EVT_MOTION(TestGLCanvas::OnMouse)
    EVT_MOUSEWHEEL(TestGLCanvas::OnMouseWheelMoved)
END_EVENT_TABLE()

TestGLCanvas::TestGLCanvas(wxWindow *parent, wxWindowID id,const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
    m_gldata.initialized = false;

    // initialize view matrix
    m_gldata.beginx = 0.0f;
    m_gldata.beginy = 0.0f;
    m_gldata.zoom   = 45.0f;
    m_gldata.posx   = 0.0f;
    m_gldata.posy   = 0.0f;
    m_gldata.mode_Trackball = 1;
    printf("Mode Trackballabc\n");
    m_gldata.rotx   = 0.0f;
    m_gldata.roty   = 0.0f;
    m_gldata.rotz   = 0.0f;
    trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);
}

TestGLCanvas::~TestGLCanvas()
{
}

/*void TestGLCanvas::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
    SetFocus();
}*/

/*void TestGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
//    float q[4],a[3];

    // must always be here
    wxPaintDC dc(this);

#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();

    // Initialize OpenGL
    if (!m_gldata.initialized) {
        InitGL();
        ResetProjectionMode();
        m_gldata.initialized = true;
    }

    // Clear
    glClearColor( 0.3f, 0.4f, 0.6f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Transformations
    glLoadIdentity();
    glTranslatef( m_gldata.posx, m_gldata.posy, -20.0f );

    if (m_gldata.mode_Trackball) {
        GLfloat m[4][4];
        build_rotmatrix( m, m_gldata.quat );
        glMultMatrixf( &m[0][0] ) ;

    } else {
//        printf("%8.4f %8.4f %8.4f\n",m_gldata.rotx,m_gldata.roty,m_gldata.rotz);
        glRotatef(m_gldata.rotz,0.,0.,1.) ; // Ordre important Z, puis Y et enfin X
        glRotatef(m_gldata.roty,0.,1.,0.) ;
        glRotatef(m_gldata.rotx,1.,0.,0.) ;
    }

    m_renderer.Render();

    // Flush
    glFlush();

    // Swap
    SwapBuffers();
}

/*void TestGLCanvas::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);
    // Reset the OpenGL view aspect
    ResetProjectionMode();
}

void TestGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

// Load the DXF file.  If the zlib support is compiled in wxWidgets,
// supports also the ".dxf.gz" gzip compressed files.
/*void TestGLCanvas::LoadDXF(const wxString& filename)
{
    wxFileInputStream stream(filename);
    if (stream.Ok())
    {
        m_renderer.Load(stream);
    }
}

void TestGLCanvas::OnMouse(wxMouseEvent& event)
{
// Déplacement de la souris
    if (event.Dragging()) {
        wxSize sz(GetClientSize());

        if (m_gldata.mode_Trackball) {

            /* drag in progress, simulate trackball */
/*     float spin_quat[4];
     trackball(spin_quat,
         (2.0*m_gldata.beginx - sz.x) / sz.x,
         (sz.y - 2.0*m_gldata.beginy) / sz.y,
         (2.0*event.GetX() - sz.x)    / sz.x,
         (sz.y - 2.0*event.GetY())    / sz.y); // Pour augmenter la sensibilité, multiplier par 2 ou 3 chacune des 4 valeurs

     add_quats(spin_quat, m_gldata.quat, m_gldata.quat);

 } else {

     /* Mode rotation directe : seulement 2 axes possibles, ici X et Y */
/*   m_gldata.roty += (float)(event.GetX()-m_gldata.beginx)/(float)sz.x*120.; // Amplitude totale fenêtre = 120°
   m_gldata.rotx += (float)(event.GetY()-m_gldata.beginy)/(float)sz.y*120.; // a peu près compatible de Trackball
}

/* orientation has changed, redraw mesh */
/* Refresh(false);
}

m_gldata.beginx = event.GetX();
m_gldata.beginy = event.GetY();
event.Skip();

}

/*void TestGLCanvas::OnMouseWheelMoved(wxMouseEvent& event)
{
// Zoom avec la molette
int signe=event.GetWheelRotation();
if (signe != 0) {
 if (signe > 0 )
     m_gldata.zoom += 1.0;
 else
     m_gldata.zoom -= 1.0;
 ResetProjectionMode();
 Refresh(false);
}

m_gldata.beginx = event.GetX();
m_gldata.beginy = event.GetY();
event.Skip();
}

/*void TestGLCanvas::OnKeyUp(wxKeyEvent& event)
{
event.Skip();
}*/

/*void TestGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    float alpha, beta, gamma;
    float spin_quat[4];
    float q[4],a[3];

    int test=0;
    long evkey = event.GetKeyCode();
    if (evkey == 0) return;

// Décodage de la Touche
    switch (evkey) {
// Zoom -
        case WXK_NUMPAD_SUBTRACT:       // Touche - du Numpad ou 0x0186
            m_gldata.zoom += 1.0;
            test=1;
            break;
        case 0x0036:                    // Touche 6 (ou -)
//        case '-':     // ne marche pas en fait !
            if (!event.ShiftDown()) {   // Ne retenir cette touche que si Shift NON enfoncée
                m_gldata.zoom += 1.0;
                test=1;
            }
            break;
// Zoom +
        case WXK_NUMPAD_ADD:            // Touche + du Numpad ou 0x0184
            m_gldata.zoom -= 1.0;
            test=1;
            break;
        case 0x002b:                    // Touche + (ou = )
//        case '+':
            if (event.ShiftDown()) {    // Ne retenir cette touche que si Shift enfoncée
                m_gldata.zoom -= 1.0;
                test=1;
            }
            break;
// Déplacement ou Rotation
// Vers la droite
        case WXK_RIGHT:         // Flèche droite ou 0x013c (défini dans defs.h, wxKeyCode : non unicode)
        case WXK_NUMPAD_RIGHT:  // Flèche droite du Numpad (si Num Lock non enfoncé) ou 0x017a
            if (event.ShiftDown()) {    // Si touche Shift enfoncée => Déplacement
                m_gldata.posx += 1.0f;
            } else {                    // Sinon Rotation
                if (m_gldata.mode_Trackball) {
                    trackball(spin_quat, 0.,0.,+pas_rad,0.); // de 0.1/TRACKBALLSIZE radians
                    add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
                } else {
                    m_gldata.roty += pas_deg;
                }
            }
            test=2;
            break;
// Vers la gauche
        case WXK_LEFT:          // Flèche gauche ou 0x013a
        case WXK_NUMPAD_LEFT:   // Flèche gauche du Numpad (si Num Lock non enfoncé) ou 0x0178
            if (event.ShiftDown()) {
                m_gldata.posx -= 1.0f;
            } else {
                if (m_gldata.mode_Trackball) {
                    trackball(spin_quat, 0.,0.,-pas_rad,0.);
                    add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
                } else {
                    m_gldata.roty -= pas_deg;
                }
            }
            test=2;
            break;
// Vers le haut
        case WXK_UP:            // Flèche haut ou 0x013b
        case WXK_NUMPAD_UP:     // Flèche haut du Numpad (si Num Lock non enfoncé) ou 0x0179
            if (event.ShiftDown()) {
                m_gldata.posy += 1.0f;
            } else {
                if (m_gldata.mode_Trackball) {
                    trackball(spin_quat, 0.,0.,0.,+pas_rad);
                    add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
                } else {
                    m_gldata.rotx -= pas_deg;
                }
            }
            test=2;
            break;
 // Vers le bas
        case WXK_DOWN:          // Flèche bas ou 0x013d
        case WXK_NUMPAD_DOWN:   // Flèche bas du Numpad (si Num Lock non enfoncé) ou 0x017b
            if (event.ShiftDown()) {
                m_gldata.posy -= 1.0f;
            } else {
                if (m_gldata.mode_Trackball) {
                    trackball(spin_quat, 0.,0.,0.,-pas_rad);
                    add_quats(spin_quat, m_gldata.quat, m_gldata.quat);
                } else {
                    m_gldata.rotx += pas_deg;
                }
            }
            test=2;
            break;
// Reset (valeurs d'initialisation)
        case 'r':
        case 'R':
            m_gldata.beginx = 0.0f;
            m_gldata.beginy = 0.0f;
            m_gldata.zoom   = 45.0f;
            m_gldata.posx   = 0.0f;
            m_gldata.posy   = 0.0f;
            m_gldata.mode_Trackball = 1;
            printf("Mode Trackball\n");
            m_gldata.rotx   = 0.0f;
            m_gldata.roty   = 0.0f;
            m_gldata.rotz   = 0.0f;
            trackball(m_gldata.quat, 0.0f, 0.0f, 0.0f, 0.0f);
            test=1;
            break;
// Changer de mode Trackball <-> Direct
        case 't':
        case 'T':
            m_gldata.mode_Trackball = 1 - m_gldata.mode_Trackball ;
            if (m_gldata.mode_Trackball) {
                printf("Mode Trackball\n");

            // Calcul du quaternion d'après les rotations dans l'ordre Z, Y et X
                vset(a,0.,0.,1.);
                axis_to_quat(a,+m_gldata.rotz/to_Deg,m_gldata.quat);

                vset(a,0.,1.,0.);
                axis_to_quat(a,-m_gldata.roty/to_Deg,q);            // Pourquoi -1 ?
                add_quats(q,m_gldata.quat,m_gldata.quat);

                vset(a,1.,0.,0.);
                axis_to_quat(a,-m_gldata.rotx/to_Deg,q);            // Pourquoi -1 ?
                add_quats(q,m_gldata.quat,m_gldata.quat);
                m_gldata.quat[2] *=-1.;                             // Pourquoi changer de signe seulement cette composante ?            }
/*              float alpha,beta,gamma;
                CalculAngles(alpha,beta,gamma);
                printf("%8.4f %8.4f %8.4f\n",alpha,beta,gamma);*/
/*      } else {
          printf("Mode Rotation directe\n");

      // Calcul des rotations à partir de la matrice MODELVIEW
          CalculAngles(m_gldata.rotx,m_gldata.roty,m_gldata.rotz);
  /*        float q_r[4];

          vset(a,1.,0.,0.);
          axis_to_quat(a, m_gldata.rotx/to_Deg,q_r);

          vset(a,0.,1.,0.);
          axis_to_quat(a, m_gldata.roty/to_Deg,q);
          add_quats(q,q_r,q_r);

          vset(a,0.,0.,1.);
          axis_to_quat(a, m_gldata.rotz/to_Deg,q);
          add_quats(q,q_r,q_r);

          printf("%8.4f %8.4f %8.4f %8.4f\n",m_gldata.quat[0],m_gldata.quat[1],m_gldata.quat[2],m_gldata.quat[3]);
          printf("%8.4f %8.4f %8.4f %8.4f\n",q_r[0],q_r[1],q_r[2],q_r[3]);*/
// }
/*       test=1; // Forcer un Refresh (ne devrait rien changer sur l'écran ....)
       break;
// Afficher la Matrice et les angles
   case 'm':
   case 'M':
       GLfloat m[4][4];
//            GLfloat m2[16];

       glGetFloatv(GL_MODELVIEW_MATRIX, *m);
       printf("Matrice GL 2D:\n");
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][0],m[1][0],m[2][0],m[3][0]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][1],m[1][1],m[2][1],m[3][1]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][2],m[1][2],m[2][2],m[3][2]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[0][3],m[1][3],m[2][3],m[3][3]) ;
/*            glGetFloatv(GL_MODELVIEW_MATRIX, m2);
       printf("Matrice GL 1D:\n"); // Ordre Idem 2D
       printf("%8.3f %8.3f %8.3f %8.3f\n", m2[0],m2[4],m2[8], m2[12]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m2[1],m2[5],m2[9], m2[13]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m2[2],m2[6],m2[10],m2[14]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m2[3],m2[7],m2[11],m2[15]) ;*/
/*         CalculAngles(alpha,beta,gamma);
         printf("alpha %8.3f deg.\n",alpha);
         printf("beta  %8.3f deg.\n",beta);
         printf("gamma %8.3f deg.\n",gamma);
         break;
// Quitter par les touches Q ou Escape (en plus de Alt-X déjà programmé et les icônes X de fermeture)
     case 'Q':
     case WXK_ESCAPE:
         wxTheApp->ExitMainLoop();
         break;
// Touche non reconnue : rien de spécial à faire (sauf l'afficher)
     default:
         if (evkey != 0x0132 && evkey != 0x0133 && evkey != 0x0134 ) { // on passe si Shift, Alt ou Ctrl
/*            wxString p;
             p=wxString::Format(wxT("%#06x"), evkey);
             std::cout << "code : " << evkey << ", " << (char)evkey << ", " ;
             for (int i=0; p[i] != 0 ; ++i) std::cout << (char)p[i] ; // On doit pouvoir faire mieux !!!
             std::cout<< std::endl; */
// La même chose en + facile !
/*           printf("code : %3d, %c, %#06x\n", (int)evkey,char(evkey),(int)evkey);
       }
       break;
}

if (test) { // Quelque chose à changé (rotation, déplacement, Zoom). Il faut redessiner la fenêtre OpenGL
   if (test == 1) ResetProjectionMode();
   Refresh(false);
}
//    std::cout << "code : " << evkey << ", " << (char)evkey << std::endl;

event.Skip();
}

/*void TestGLCanvas::CalculAngles(float &alpha, float &beta, float &gamma)
{

// D'après Matrix and Quaternions (http://www.flipcode.com/documents/matrfaq.html, Question 37)

double sin_beta, cos_beta ;
GLfloat m[16];

glGetFloatv(GL_MODELVIEW_MATRIX, m);
/*    printf("Matrice GL 1D:\n");
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[0],m[4],m[8], m[12]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[1],m[5],m[9], m[13]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[2],m[6],m[10],m[14]) ;
       printf("%8.3f %8.3f %8.3f %8.3f\n", m[3],m[7],m[11],m[15]) ;*/

/*  sin_beta = -m[2];
  beta = asin(sin_beta)*to_Deg;                   // suffisant car -90 <= beta <= 90°
  cos_beta = sqrt(1.0f-sin_beta*sin_beta);        // cos_beta est donc toujours >= 0

  if (fabs(cos_beta) > 0.0005f) {                 // En fait <> 0. mais ici avec une certaine tolérance
                                                  // Signes changés / article ... + au lieu de -
      alpha=atan2(+m[6],m[10])*to_Deg;            // en fait -m[6]/cos_beta et m[10]/cos_beta), mais <=> car cos_beta > 0
      gamma=atan2(+m[1],m[0] )*to_Deg;            // Idem
  } else {                                        // ici sin(beta) = 1 ou -1 (à epsilon près)
      beta =( sin_beta < 0.0f ? -90.0f : +90.0f); // Forcer la valeur de beta
      alpha=0.0f;                                 // Cas dégénéré (Gimball lock => alpha et gamma sur le même axe)
      gamma=atan2(-m[4],m[5]);
      gamma=gamma*to_Deg;
  }
  Clamp(alpha,0.0f,360.0f);   // Recadrer les angles dans l'intervalle [0,360°[
  Clamp(beta ,0.0f,360.0f);
  Clamp(gamma,0.0f,360.0f);
}

/*void TestGLCanvas::Clamp(float &angle, const float vmin, const float vmax)
{
// Recadre les valeurs de angle dans l'intervalle [vmin, vmax[
// ATTENTION : si vmin vmax mal entré, peut boucler à l'infini....
  float ampl=vmax-vmin;
  while(angle <  vmin) angle+=ampl;
  while(angle >= vmax) angle-=ampl;
}

void TestGLCanvas::InitGL()
{
  static const GLfloat light0_pos[4]   = { -50.0f, 50.0f, 0.0f, 0.0f };

  // white light
  static const GLfloat light0_color[4] = { 0.6f, 0.6f, 0.6f, 1.0f };

  static const GLfloat light1_pos[4]   = {  50.0f, 50.0f, 0.0f, 0.0f };

  // cold blue light
  static const GLfloat light1_color[4] = { 0.4f, 0.4f, 1.0f, 1.0f };

  /* remove back faces */
/*   glDisable(GL_CULL_FACE);
   glEnable(GL_DEPTH_TEST);

   /* speedups */
/*  glEnable(GL_DITHER);
  glShadeModel(GL_SMOOTH);                // En fait le modèle penguin est GL_FLAT (pas de normales aux sommets ?)
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

  /* light */
/*  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_color);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_color);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

void TestGLCanvas::ResetProjectionMode()
{
  int w, h;
  GetClientSize(&w, &h);
#ifndef __WXMOTIF__
  if ( GetContext() )
#endif
  {
      SetCurrent();
      glViewport(0, 0, (GLint) w, (GLint) h);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(m_gldata.zoom, (GLfloat)w/h, 1.0, 100.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
  }
}*/
