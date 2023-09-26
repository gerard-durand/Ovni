#ifndef GLCANVAS_H_INCLUDED
#define GLCANVAS_H_INCLUDED

#include <wx/app.h>
//#include "dxfrenderer.h"
#include "wx/glcanvas.h"

// OpenGL view data
struct GLData
{
    bool  initialized;          // have OpenGL been initialized?
    int   BeginX, BeginY;       // position of mouse
    float quat[4];              // orientation of object
    float zoom;                 // field of view in degrees
    float zoom_step;            // Incrément de zoom
    float fmult_diag;           // Facteur multiplicatif / diagonale de boîte englobante
    float zNear;
    float zFar;
    float posx, posy, posz;     // Position du centre
    float depl_xy ;
    int   mode_Trackball;       // 1 si Mode Trackball, 0 si rotation directe
    float rotx, roty, rotz;     // Valeurs des angles sur les 3 axes en degrés
};

#endif // GLCANVAS_H_INCLUDED
