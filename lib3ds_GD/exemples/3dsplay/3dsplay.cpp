/*
    Copyright (C) 1996-2008 by Jan Eric Kyprianidis <www.kyprianidis.com>
    All rights reserved.

    This program is free  software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program  is  distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should  have received a copy of the GNU Lesser General Public License
    along with  this program; If not, see <http://www.gnu.org/licenses/>.
*/
#include <lib3ds.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "tga.h"

#define FRAMES_PER_SECOND 10

// OS X has a different path than everyone else
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "LoadJPEG.h"
#include "LoadPNG.h"
#include "LoadBMP.h"

#define MOUSE_SCALE .1 /* degrees/pixel movement */

/*
Previews a <i>3DS</i> file using OpenGL.
\warning To compile this program you must have OpenGL and glut (or freeglut) installed.
*/

typedef enum {ROTATING, WALKING} RunMode;

static RunMode runMode = ROTATING;

static const char* filepath = NULL;
static char  datapath[256];
static char  filename[256];
static int   dbuf       = 1;
static int   halt       = 0;
static int   flush      = 0;
static int   anti_alias = 0;

static const char* camera  = 0;
static Lib3dsFile *f3ds    = 0;
static float current_frame = 0.0;
static int   gl_width;
static int   gl_height;
static int   menu_id       = 0;
static int   show_object   = 1;
static int   show_bounds   = 0;
static int   rotating      = 0;
static int   show_cameras  = 0;
static int   show_lights   = 0;
static int   gouraud       = 1;
static int   gouraud_prev  = 1;

static int tracer_normales = 0;

static int cameraList, lightList;   /* Icon display lists */

static float bmin[3], bmax[3];
static float sx, sy, sz, size_m;    /* bounding box dimensions */
static float cx, cy, cz;            /* bounding box center */
static float diagonale, longueur;   /* diagonale de la boîte englobante et longueur des normales */

static float view_rotx = 0., view_roty = 0., view_rotz = 0.;
static float anim_rotz = 0.;

static int mx, my;

static const GLfloat white[4] = {1.,  1.,  1.,  1.};
static const GLfloat dgrey[4] = {.25, .25, .25, 1.};
static const GLfloat grey[4]  = {.5,  .5,  .5,  1.};
static const GLfloat lgrey[4] = {.75, .75, .75, 1.};
static const GLfloat black[4] = {0.,  0.,  0.,  1.};
static const GLfloat red[4]   = {1.,  0.,  0.,  1.};
static const GLfloat green[4] = {0.,  1.,  0.,  1.};
static const GLfloat blue[4]  = {0.,  0.,  1.,  1.};


static void  solidBox(double bx, double by, double bz);
static void  solidCylinder(double r, double h, int slices);
static int   callback(void (*cb)(int m, int d, void *), void *client);
static void  call_callback(int idx, int data);

static void  solidBox(double bx, double by, double bz);
static void  solidCylinder(double r, double h, int slices);
static const char *Basename(const char *filename);

static float fov_mult = 1.0f;
static float fov_calc = 0.f;
int premier=1;
//static bool light = true;

typedef struct {
    GLuint tex_id; //OpenGL texture ID
    int w;
    int h;
    unsigned char *pixels;
} PlayerTexture;


#define NA(a) (sizeof(a)/sizeof(a[0]))

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

static int compteur_smoothing_global = 0;

int mesh_compter_smoothing(Lib3dsMesh *mesh)
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


static void menu_cb(int value)
{
    call_callback(value, 0);
}


/*!
* Switch cameras based on user's menu choice.
*/
static void camera_menu(int menu, int value, void *client)
{
    Lib3dsCamera *c = (Lib3dsCamera*)client;
    view_rotx = view_roty = view_rotz = anim_rotz = 0.;
    camera = c->name;
}


/*!
* Toggle an arbitrary int (bool) variable
*/
static void toggle_bool(int menu, int value, void *client)
{
    int *var = (int*)client;
    *var = !*var;
    glutPostRedisplay();
}


/*!
* Build the menu
*/
static void build_menu()
{
    int i;
    menu_id = glutCreateMenu(menu_cb);

    for (i = 0; i < f3ds->ncameras; ++i)
        glutAddMenuEntry(f3ds->cameras[i]->name, callback(camera_menu, f3ds->cameras[i]));

    glutAddMenuEntry("Show cameras", callback(toggle_bool, &show_cameras));
    glutAddMenuEntry("Show lights",  callback(toggle_bool, &show_lights));
    glutAddMenuEntry("Show bounds",  callback(toggle_bool, &show_bounds));
}

#define LIB3DS_PI 3.14159265358979323846


/*!
* Time function, called every frame
*/
static void timer_cb(int value)
{
    if (!halt) {
        view_rotz += anim_rotz;
        current_frame+= 1;
        if (current_frame > f3ds->frames)
            current_frame = 0;
        lib3ds_file_eval(f3ds, current_frame);

        glutTimerFunc(1000 / FRAMES_PER_SECOND, timer_cb, 0);
    }
    glutPostRedisplay();
}


static void set_halt(int h)
{
    if (h != halt) {
        halt = h;
        if (!halt)
            glutTimerFunc(10, timer_cb, 0);
    }
}


/*!
* Initialize OpenGL
*/
static void init(void)
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    //glDisable(GL_NORMALIZE);
    //glPolygonOffset(1.0, 2);
}


/*!
* Load the model from .3ds file.
*/
static void load_model(void)
{
    f3ds = lib3ds_file_open(filepath);
    if (!f3ds) {
        puts("3dsplayer: Error: Loading 3DS file failed.\n");
        exit(1);
    }

    /* No nodes?  Fabricate nodes to display all the meshes. */
    printf("premier node : f3ds->nodes : %s\n",f3ds->nodes->name);
    if (!f3ds->nodes) {
        Lib3dsNode *node;
        int i;

        if (premier) printf("Creation de nodes car il n'y en a pas dans ce fichier 3ds\n");

        for (i = 0; i < f3ds->nmeshes; ++i) {
            Lib3dsMesh *mesh = f3ds->meshes[i];
            node = lib3ds_node_new(LIB3DS_NODE_MESH_INSTANCE);
            strcpy(node->name, mesh->name);
            lib3ds_file_insert_node(f3ds, node, NULL);
        }
    }

    lib3ds_file_eval(f3ds, 0.0f);
    lib3ds_file_bounding_box_of_nodes(f3ds, 1, 0, 0, bmin, bmax, NULL);
    sx     = bmax[0] - bmin[0];
    sy     = bmax[1] - bmin[1];
    sz     = bmax[2] - bmin[2];
    size_m = MAX(sx, sy);
    size_m = MAX(size_m, sz);
    cx     = (bmin[0] + bmax[0]) / 2;
    cy     = (bmin[1] + bmax[1]) / 2;
    cz     = (bmin[2] + bmax[2]) / 2;

    diagonale = sqrt(sx*sx + sy*sy + sz*sz);
    longueur  = diagonale/30.;

    /* No cameras in the file?  Add four */

    if (!f3ds->ncameras) {

        /* Add some cameras that encompass the bounding box */

        Lib3dsCamera *camera = lib3ds_camera_new("Camera_X");
        camera->target[0]   = cx;
        camera->target[1]   = cy;
        camera->target[2]   = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[0] = bmax[0] + 1.5 * MAX(sy, sz);
        camera->near_range  = (camera->position[0] - bmax[0]) * .5;
        camera->far_range   = (camera->position[0] - bmin[0]) * 2;
        lib3ds_file_insert_camera(f3ds, camera, -1);

        /* Since lib3ds considers +Y to be into the screen, we'll put
        * this camera on the -Y axis, looking in the +Y direction.
        */
        camera = lib3ds_camera_new("Camera_Y");
        camera->target[0]   = cx;
        camera->target[1]   = cy;
        camera->target[2]   = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[1] =  bmin[1] - 1.5 * MAX(sx, sz);
        camera->near_range  = (bmin[1] - camera->position[1]) * .5;
        camera->far_range   = (bmax[1] - camera->position[1]) * 2;
        lib3ds_file_insert_camera(f3ds, camera, -1);

        camera = lib3ds_camera_new("Camera_Z");
        camera->target[0]   = cx;
        camera->target[1]   = cy;
        camera->target[2]   = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[2] = bmax[2] + 1.5 * MAX(sx, sy);
        camera->near_range  = (camera->position[2] - bmax[2]) * .5;
        camera->far_range   = (camera->position[2] - bmin[2]) * 2;
        lib3ds_file_insert_camera(f3ds, camera, -1);

        camera = lib3ds_camera_new("Camera_ISO");
        camera->target[0]   = cx;
        camera->target[1]   = cy;
        camera->target[2]   = cz;
        memcpy(camera->position, camera->target, sizeof(camera->position));
        camera->position[0] = bmax[0] + .75 * size_m;
        camera->position[1] = bmin[1] - .75 * size_m;
        camera->position[2] = bmax[2] + .75 * size_m;
        camera->near_range  = (camera->position[0] - bmax[0]) * .5;
        camera->far_range   = (camera->position[0] - bmin[0]) * 3;
        lib3ds_file_insert_camera(f3ds, camera, -1);
    }

    /* No lights in the file?  Add some. */

    if (!f3ds->nlights) {
        Lib3dsLight *light;

        light = lib3ds_light_new("light0");
        light->spot_light  = 0;
        light->see_cone    = 0;
        light->color[0]    = light->color[1] = light->color[2] = .6;
        light->position[0] = cx + size_m * .75;
        light->position[1] = cy - size_m * 1.;
        light->position[2] = cz + size_m * 1.5;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier  = 1;
        lib3ds_file_insert_light(f3ds, light, -1);

        light = lib3ds_light_new("light1");
        light->spot_light  = 0;
        light->see_cone    = 0;
        light->color[0]    = light->color[1] = light->color[2] = .3;
        light->position[0] = cx - size_m;
        light->position[1] = cy - size_m;
        light->position[2] = cz + size_m * .75;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier  = 1;
        lib3ds_file_insert_light(f3ds, light, -1);

        light = lib3ds_light_new("light2");
        light->spot_light  = 0;
        light->see_cone    = 0;
        light->color[0]    = light->color[1] = light->color[2] = .3;
        light->position[0] = cx;
        light->position[1] = cy + size_m;
        light->position[2] = cz + size_m;
        light->position[3] = 0.;
        light->outer_range = 100;
        light->inner_range = 10;
        light->multiplier  = 1;
        lib3ds_file_insert_light(f3ds, light, -1);
    }

    camera = f3ds->cameras[0]->name;

    for (int i = 0; i < f3ds->nmaterials; ++i) {
        Lib3dsMaterial *mat = f3ds->materials[i];
        if (mat->texture1_map.name[0]) {  /* texture map? */
            Lib3dsTextureMap *tex = &mat->texture1_map;

            char texname[1024];
            char *ptr_texname;
            PlayerTexture *pt = (PlayerTexture*)calloc(sizeof(*pt),1);
            tex->user_ptr = pt;
            strcpy(texname, datapath);
            strcat(texname, "/");
            strcat(texname, tex->name);

            printf("Loading '%s' ... ", texname);
            bool Ok_texture = false;
            // Essai sur fichier de type .tga
            if (pt->tex_id = Load_TGA(texname, &pt->pixels, &pt->w, &pt->h)) {      // &pt->pixels, &pt->w, &pt->h ne servent plus par la suite !!!
//                glGenTextures(1, &pt->tex_id);                                    // Pour faire comme les autres Load_*, transféré en fin de Load_TGA
//                glBindTexture(GL_TEXTURE_2D, pt->tex_id);
//                glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, pt->w, pt->h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pt->pixels); // GL_BGRA_EXT plutôt que GL_RGBA
                Ok_texture = true;
            } else {
            // Si ce n'est pas un .tga, c'est peut-être un jpeg
                if (strcasecmp(".jpg",strrchr(texname,'.')) == 0) {                 // Ne rentrer dans Load_JPEG que si l'extention est .jpg (ou .JPG)
                    if (pt->tex_id = Load_JPEG(texname, &pt->w, &pt->h)) {          // Pas sûr qu'on ait besoin de width(pt->w) et height pas la suite !
                        Ok_texture = true;
                    }
            // Tester si c'est un .png
                } else if (strcasecmp(".png",strrchr(texname,'.')) == 0) {          // Idem ...
                    if (pt->tex_id = Load_PNG(texname,  &pt->w, &pt->h)) {
                        Ok_texture = true;
                    }
            // un bmp ?
                } else if (strcasecmp(".bmp",strrchr(texname,'.')) == 0) {          // Idem ... Décalages sii witdh n'est pas un multiple de 4
//                    fprintf(stderr,"pas encore reconnu\n");
                    if (pt->tex_id = Load_BMP(texname,  &pt->w, &pt->h)) {
                        Ok_texture = true;
                    }
            // un tif ?
                } else if (strcasecmp(".tif",strrchr(texname,'.')) == 0) {          // Idem ...
                    fprintf(stderr,"pas encore reconnu\n");
                } else
            // Fichier de texture non reconnu (autre que .tga, .jpg, .png)
                    fprintf(stderr, "failed!\n");
            }
            if (Ok_texture) {
                printf("image %d*%d\n",pt->w,pt->h);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            }
        }
    }

    lib3ds_file_eval(f3ds, 0);
}


static void Affiche_Matrice(float M[4][4]) {
    int i,j;
    for (i=0;i<4;i++) {
        for (j=0;j<4;j++) printf("%12.5f ",M[j][i]);
        printf("\n");
    }
    printf("\n");
}


/*!
* Render node recursively, first children, then parent.
* Each node receives its own OpenGL display list.
*/
static void render_node(Lib3dsNode *node)
{
    assert(file);

    {
        Lib3dsNode *p;
        if (premier) printf("entree de render_node :   %s\n",node->name);
        for (p = node->childs; p != 0; p = p->next) {
            if (premier) printf("\nboucle sur childs :       %s\n",p->name);
            render_node(p);
        }
    }
    if (premier) printf("render_node : %s : Type : %d\n",node->name,node->type);

    if (node->type == LIB3DS_NODE_MESH_INSTANCE) {
        int index;
        Lib3dsMesh *mesh;
        Lib3dsMeshInstanceNode *n = (Lib3dsMeshInstanceNode*)node;

        if (strcmp(node->name, "$$$DUMMY") == 0) {
            return;
        }

        index = lib3ds_file_mesh_by_name(f3ds, n->instance_name);

        if (index < 0 && premier )  printf("index %d n'existe pas\n", index) ;

        if (index < 0)
            index = lib3ds_file_mesh_by_name(f3ds, node->name);

        if (index >= 0 && premier )  printf("index %d existe\n", index) ;

		if (index < 0) {	   // N'existe toujours pas ?
            return;
        }
        mesh = f3ds->meshes[index];
//if(premier) mesh->user_id = 0;
        if (!mesh->user_id || (gouraud_prev != gouraud)) {
            assert(mesh);

            if (premier) printf("Creation de la liste pour %s\n",node->name);

            mesh->user_id = glGenLists(1);
            glNewList(mesh->user_id, GL_COMPILE);

            {
                int p;
                float (*normalL)[3] = (float(*)[3])malloc(3 * 3 * sizeof(float) * mesh->nfaces);    // Normales aux sommets, 3 sommets par face
                float (*normalB)[3] = (float(*)[3])malloc(3 * sizeof(float) * mesh->nfaces);        // Normales aux barycentres, 1 normale par face
                Lib3dsMaterial *oldmat = (Lib3dsMaterial *) - 1;
                {
                    float M[4][4];
                    lib3ds_matrix_copy(M, mesh->matrix);
                    lib3ds_matrix_inv(M);
                    glMultMatrixf(&M[0][0]);
                }

                if (gouraud) {
                    lib3ds_mesh_calculate_vertex_normals(mesh, normalL);
                }

                if (premier) {  // Compter et sommer au premier passage seulement, indépendamment de la valeur de gouraud
                    int count_smoothing = mesh_compter_smoothing(mesh);
                    compteur_smoothing_global += count_smoothing;
                    printf("compteur smoothing : %d\n",count_smoothing);
                }

                lib3ds_mesh_calculate_face_normals  (mesh, normalB);

                for (p = 0; p < mesh->nfaces; ++p) {
                    Lib3dsMaterial *mat = 0;
                    if (mesh->faces[p].material >= 0) {
                        mat = f3ds->materials[mesh->faces[p].material];
                    }

                    if (mat != oldmat) {
                        if (mat != nullptr) {
                            //if (mat->two_sided)
                            //    glDisable(GL_CULL_FACE);
                            //else
                            //    glEnable(GL_CULL_FACE);
                            //
                            //glDisable(GL_CULL_FACE);

                            if (mat->texture1_map.user_ptr) {
                                PlayerTexture* pt = (PlayerTexture*)mat->texture1_map.user_ptr;
                                glEnable(GL_TEXTURE_2D);
                                glBindTexture(GL_TEXTURE_2D, pt->tex_id);
                            } else {
                                glDisable(GL_TEXTURE_2D);
                            }

                            {
                                float a[4], d[4], s[4];
                                int i;
                                for (i=0; i<3; ++i) {
                                    a[i] = mat->ambient[i];
                                    d[i] = mat->diffuse[i];
                                    s[i] = mat->specular[i];
                                }
                                a[3] = d[3] = s[3] = 1.0f;

                                glMaterialfv(GL_FRONT, GL_AMBIENT,  a);
                                glMaterialfv(GL_FRONT, GL_DIFFUSE,  d);
                                glMaterialfv(GL_FRONT, GL_SPECULAR, s);
                            }
                            float shininess = pow(2, 10.0*mat->shininess);
                            glMaterialf(GL_FRONT, GL_SHININESS, shininess <= 128? shininess : 128);
                        } else {
                            static const float a[4] = {0.7, 0.7, 0.7, 1.0};
                            static const float d[4] = {0.7, 0.7, 0.7, 1.0};
                            static const float s[4] = {1.0, 1.0, 1.0, 1.0};
                            glMaterialfv(GL_FRONT, GL_AMBIENT,  a);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE,  d);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, s);
                            glMaterialf (GL_FRONT, GL_SHININESS, pow(2, 10.0*0.5));
                        }
                        oldmat = mat;
                    }

                    if (tracer_normales) {
                        float v1[3], n[3], v2[3];
                        int i;
                        GLfloat couleurs[4];
//                        glGetIntegerv(GL_TEXTURE_2D,&mode_tex);
                        glDisable(GL_TEXTURE_2D);
                        glDisable(GL_LIGHTING);
                        glGetFloatv(GL_CURRENT_COLOR, couleurs);
                        glLineWidth(2);

                        if (gouraud)
                            glColor3f(0.5,0.0,0.0); // Rouge foncé
                        else
                            glColor3f(0.0,0.5,0.0); // Vert foncé

                        glBegin(GL_LINES);
                        if (gouraud) {
                            for (i = 0; i < 3; ++i) {                                               // Sert à tracer les normales aux sommets
                                lib3ds_vector_copy(v1, mesh->vertices[mesh->faces[p].index[i]]);
                                glVertex3fv(v1);
                                lib3ds_vector_scalar_mul(n, normalL[3*p+i], longueur);
                                lib3ds_vector_add(v2, v1, n);
                                glVertex3fv(v2);
                            }
                        } else {
                            lib3ds_vector_copy(v1, mesh->vertices[mesh->faces[p].index[0]]);        // Sert à tracer les normales aux barycentres
                            for (i = 1; i < 3; ++i) {
                                lib3ds_vector_add(v1, v1, mesh->vertices[mesh->faces[p].index[i]]); // Sommer les 3 sommets
                            }
                            lib3ds_vector_scalar_mul(v1, v1, 1./3.);        // Moyenne des 3 sommets = Barycentre
                            glVertex3fv(v1);
                            lib3ds_vector_scalar_mul(n, normalB[p],longueur);
                            lib3ds_vector_add(v2, v1, n);
                            glVertex3fv(v2);
                        }
                        glEnd();
                        glColor4fv(couleurs);
                        glLineWidth(1);
                        glEnable(GL_LIGHTING);
                        if (mat != nullptr) {
                            if (mat->texture1_map.user_ptr) glEnable(GL_TEXTURE_2D);
                        }
                    }

//                    if (light)
//                        glEnable(GL_LIGHTING) ;
//                    else
//                        glDisable(GL_LIGHTING);
                    glBegin(GL_TRIANGLES);
                    glNormal3fv(normalB[p]);
                    for (int i = 0; i < 3; ++i) {
                        if (gouraud) glNormal3fv(normalL[3*p+i]);
                        if (mat != nullptr) {                                   // Ajout GD sinon plantage sur certaines Bdds
                            if (mat->texture1_map.user_ptr) {
                                if(mesh->texcos != nullptr)                     // évite un plantage dans certains cas ! mais pourquoi y a t-il u pointeur null dans ces cas là ?
                                    glTexCoord2f(
                                        mesh->texcos[mesh->faces[p].index[i]][0],
                                      1-mesh->texcos[mesh->faces[p].index[i]][1] );
                            }
                        }

                        glVertex3fv(mesh->vertices[mesh->faces[p].index[i]]);
                    }
                    glEnd();
                }

                free(normalL);
                free(normalB);
            }

            glDisable(GL_TEXTURE_2D);
            glEndList();
        }

        if (mesh->user_id) {
            glPushMatrix();
            glMultMatrixf(&node->matrix[0][0]);
            glTranslatef(-n->pivot[0], -n->pivot[1], -n->pivot[2]);
            glCallList(mesh->user_id);
            /* glutSolidSphere(50.0, 20,20); */
            glPopMatrix();
            if (flush)
                glFlush();
        }
    }
}


/*!
* Update information about a light.  Try to find corresponding nodes
* if possible, and copy values from nodes into light struct.
*/
static void light_update(Lib3dsLight *l)
{
    Lib3dsNode *ln, *sn;

    ln = lib3ds_file_node_by_name(f3ds, l->name, LIB3DS_NODE_SPOTLIGHT);
    sn = lib3ds_file_node_by_name(f3ds, l->name, LIB3DS_NODE_SPOTLIGHT_TARGET);

    if (ln != NULL) {
        Lib3dsSpotlightNode *n = (Lib3dsSpotlightNode*)ln;
        memcpy(l->color, n->color,  3 * sizeof(float));
        memcpy(l->position, n->pos, 3 * sizeof(float));
    }

    if (sn != NULL) {
        Lib3dsTargetNode *n = (Lib3dsTargetNode*)sn;
        memcpy(l->target, n->pos, 3* sizeof(float));
    }
}


static void draw_bounds(float tgt[3])
{
    double cx, cy, cz;
    double lx, ly, lz;

    lx = sx / 10.;
    ly = sy / 10.;
    lz = sz / 10.;
    cx = tgt[0];
    cy = tgt[1];
    cz = tgt[2];

// Boite englobante

    glDisable(GL_LIGHTING);
    glColor4fv(white);
    glBegin(GL_LINES);
        glVertex3f(bmin[0], bmin[1], bmin[2]);
        glVertex3f(bmax[0], bmin[1], bmin[2]);
        glVertex3f(bmin[0], bmax[1], bmin[2]);
        glVertex3f(bmax[0], bmax[1], bmin[2]);
        glVertex3f(bmin[0], bmin[1], bmax[2]);
        glVertex3f(bmax[0], bmin[1], bmax[2]);
        glVertex3f(bmin[0], bmax[1], bmax[2]);
        glVertex3f(bmax[0], bmax[1], bmax[2]);

        glVertex3f(bmin[0], bmin[1], bmin[2]);
        glVertex3f(bmin[0], bmax[1], bmin[2]);
        glVertex3f(bmax[0], bmin[1], bmin[2]);
        glVertex3f(bmax[0], bmax[1], bmin[2]);
        glVertex3f(bmin[0], bmin[1], bmax[2]);
        glVertex3f(bmin[0], bmax[1], bmax[2]);
        glVertex3f(bmax[0], bmin[1], bmax[2]);
        glVertex3f(bmax[0], bmax[1], bmax[2]);

        glVertex3f(bmin[0], bmin[1], bmin[2]);
        glVertex3f(bmin[0], bmin[1], bmax[2]);
        glVertex3f(bmax[0], bmin[1], bmin[2]);
        glVertex3f(bmax[0], bmin[1], bmax[2]);
        glVertex3f(bmin[0], bmax[1], bmin[2]);
        glVertex3f(bmin[0], bmax[1], bmax[2]);
        glVertex3f(bmax[0], bmax[1], bmin[2]);
        glVertex3f(bmax[0], bmax[1], bmax[2]);

        glVertex3f(cx - size_m / 32, cy, cz);
        glVertex3f(cx + size_m / 32, cy, cz);
        glVertex3f(cx, cy - size_m / 32, cz);
        glVertex3f(cx, cy + size_m / 32, cz);
        glVertex3f(cx, cy, cz - size_m / 32);
        glVertex3f(cx, cy, cz + size_m / 32);
    glEnd();

// 3 Axes

    glColor4fv(red);
    glBegin(GL_LINES);
        glVertex3f(0., 0., 0.);
        glVertex3f(lx, 0., 0.);
    glEnd();

    glColor4fv(green);
    glBegin(GL_LINES);
        glVertex3f(0., 0., 0.);
        glVertex3f(0., ly, 0.);
    glEnd();

    glColor4fv(blue);
    glBegin(GL_LINES);
        glVertex3f(0., 0., 0.);
        glVertex3f(0., 0., lz);
    glEnd();

    glEnable(GL_LIGHTING);
}


static void draw_light(const GLfloat *pos, const GLfloat *color)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glScalef(size_m / 20, size_m / 20, size_m / 20);
    glCallList(lightList);
    glPopMatrix();
}


/*!
* Main display function; called whenever the scene needs to be redrawn.
*/
static void display(void)
{
    Lib3dsTargetNode *t;
    Lib3dsCameraNode *c;
    float fov=0, roll=0;
    float znear, zfar, dist;    // znear et zfar plutôt que near et far, sinon conflit
    float *campos=0;
    float *tgt=0;
    float M[4][4];
    int camidx;
    Lib3dsCamera *cam;
    float v[3];
    Lib3dsNode *p;

    if (f3ds != NULL && f3ds->background.use_solid)
        glClearColor(f3ds->background.solid_color[0],
                     f3ds->background.solid_color[1],
                     f3ds->background.solid_color[2], 1.);

    /* TODO: fog */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (anti_alias)
        glEnable (GL_POLYGON_SMOOTH);
    else
        glDisable(GL_POLYGON_SMOOTH);


    if (!f3ds) {
        return;
    }

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, f3ds->ambient);

    c = (Lib3dsCameraNode*)lib3ds_file_node_by_name(f3ds, camera, LIB3DS_NODE_CAMERA);
    t = (Lib3dsTargetNode*)lib3ds_file_node_by_name(f3ds, camera, LIB3DS_NODE_CAMERA_TARGET);

    if (t != NULL) {
        tgt = t->pos;
    }
    if (c != NULL) {
        fov    = c->fov;
        roll   = c->roll;
        campos = c->pos;
    }

    if ((camidx = lib3ds_file_camera_by_name(f3ds, camera)) == -1)
        return;
    cam = f3ds->cameras[camidx];

    znear = cam->near_range;
    zfar  = cam->far_range;

    znear /= 3; // élargir la plage des valeurs sinon 0028 est trop tronquée
    zfar  *= 3;

    if (c == NULL || t == NULL) {
        if (c == NULL) {
            fov    = cam->fov;
            roll   = cam->roll;
            campos = cam->position;
        }
        if (t == NULL)
            tgt = cam->target;
    }

    fov_calc = fov; // Sauvegarde de fov dans fov_calc (pour pouvoir ensuite modifier le champ de vue)

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* KLUDGE alert:  OpenGL can't handle a near clip plane of zero,
    * so if the camera's near plane is zero, we give it a small number.
    * In addition, many .3ds files I've seen have a far plane that's
    * much too close and the model gets clipped away.  I haven't found
    * a way to tell OpenGL not to clip the far plane, so we move it
    * further away.  A factor of 10 seems to make all the models I've
    * seen visible.
    */
    if (znear <= 0.) znear = zfar * .001;

    if (zfar == 0.0) {
        gluPerspective(fov_calc*fov_mult, 1.0*gl_width / gl_height, 1, 10000) ;
    } else {
        // Modif GD refaire comme dans lib3ds 1.3.0 sinon troncatures
        // printf("znear,zfar : %f %f\n",znear,zfar);
        gluPerspective(fov_calc*fov_mult, 1.0*gl_width / gl_height, znear, zfar);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-90, 1.0, 0, 0);

    /* User rotates the view about the target point */

    lib3ds_vector_sub(v, tgt, campos);
    dist = lib3ds_vector_length(v);

    glTranslatef(0., dist, 0.);
    glRotatef(view_rotx, 1., 0., 0.);
    glRotatef(view_roty, 0., 1., 0.);
    glRotatef(view_rotz, 0., 0., 1.);
    glTranslatef(0., -dist, 0.);

    lib3ds_matrix_camera(M, campos, tgt, roll);
    glMultMatrixf(&M[0][0]);

    /* Lights.  Set them from light nodes if possible.  If not, use the
    * light objects directly.
    */
    {
        static const GLfloat a[] = {0.0f, 0.0f, 0.0f, 1.0f};
        static GLfloat c[]       = {1.0f, 1.0f, 1.0f, 1.0f};
        static GLfloat p[]       = {0.0f, 0.0f, 0.0f, 1.0f};
        Lib3dsLight *l;
        int i;

        int li = GL_LIGHT0;
        for (i = 0; i < f3ds->nlights; ++i) {
            l = f3ds->lights[i];
            glEnable(li);
            light_update(l);

            c[0] = l->color[0];
            c[1] = l->color[1];
            c[2] = l->color[2];
            glLightfv(li, GL_AMBIENT, a);
            glLightfv(li, GL_DIFFUSE, c);
            glLightfv(li, GL_SPECULAR, c);

            p[0] = l->position[0];
            p[1] = l->position[1];
            p[2] = l->position[2];
            glLightfv(li, GL_POSITION, p);

            if (l->spot_light) {
                p[0] = l->target[0] - l->position[0];
                p[1] = l->target[1] - l->position[1];
                p[2] = l->target[2] - l->position[2];
                glLightfv(li, GL_SPOT_DIRECTION, p);
            }
            ++li;
        }
    }

    if (show_object) {
        if (premier) printf("debut boucle dans display/show_objet\n");
        for (p = f3ds->nodes; p != 0; p = p->next) {
            render_node(p);
        }
        if (premier) {
            printf("Fin boucle dans display/show_objet\n");
            printf("Nombre total de facettes avec smoothing : %d\n",compteur_smoothing_global);
        }

        gouraud_prev = gouraud;
        premier=0;
    }

    if (show_bounds)
        draw_bounds(tgt);

    if (show_cameras) {
        int i;
        for (i = 0; i < f3ds->ncameras; ++i) {
            cam = f3ds->cameras[i];
            lib3ds_matrix_camera(M, cam->position, cam->target, cam->roll);
            lib3ds_matrix_inv(M);

            glPushMatrix();
            glMultMatrixf(&M[0][0]);
            glScalef(size_m / 20, size_m / 20, size_m / 20);
            glCallList(cameraList);
            glPopMatrix();
        }
    }

    if (show_lights) {
        Lib3dsLight *light;
        int i;
//        GLfloat col[4];
//        glGetFloatv(GL_EMISSION, col);
        for (i = 0; i < f3ds->nlights; ++i) {
            light = f3ds->lights[i];
            draw_light(light->position, light->color);
        }
        glMaterialfv(GL_FRONT, GL_EMISSION, black); // Problème si on valide puis invalide show_lights
//        glMaterialfv(GL_FRONT, GL_EMISSION, col);   // Pas mieux !
    }

    glutSwapBuffers();
}


/*!
*
*/
static void reshape(int w, int h)
{
    gl_width  = w;
    gl_height = h;
    glViewport(0, 0, w, h);
}


/*!
*
*/
static void commandes_clavier()
{
    printf("Effets des touches du clavier :\n");
    printf("a : animation, augmente la vitesse de rotation horizontale\n");
    printf("A : animation, diminue  la vitesse de rotation horizontale\n");
    printf("x : augmente la rotation longitudinale\n");
    printf("X : diminue  la rotation longitudinale\n");
    printf("z : augmente la rotation verticale\n");
    printf("Z : diminue  la rotation verticale\n");
    printf("h : Stoppe les rotations\n");
    printf("r : RAZ des angles de rotations\n");
    printf("b : Affiche/masque la boite englobante\n");
    printf("g : Active/desactive le lissage de Gouraud\n");
    printf("n : Affiche/masque les normales\n");
    printf("Ctrl A : Switch antialiasing\n");
    printf("Mouvements en X/Y a la souris, bouton gauche enfonce\n");
    printf("+ : Zoom +5%\n");
    printf("- : Zoom -5%\n");
    printf("Esc : Quitter\n\n");
    printf("Anti aliasing ");
    if (!anti_alias) printf("non ");
    printf("actif\n");
    printf("Lissage de gouraud ");
    if (!gouraud) printf("non ");
    printf("actif\n\n");
}


static void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'h':
        set_halt(!halt);
        break;
    case 'a':
        anim_rotz += .1;
        break;
    case 'A':
        anim_rotz -= .1;
        break;
    case 'r':
        view_rotx = view_roty = view_rotz = anim_rotz = 0.;
        fov_mult = 1.0f;
        break;
    case 'z':
        view_roty += 5.;
        break;
    case 'Z':
        view_roty -= 5.;
        break;
    case 'x':
        view_rotx += 5.;
        break;
    case 'X':
        view_rotx -= 5.;
        break;
    case 'b':
        show_bounds = !show_bounds;
        break;
    case 'o':
        show_object = !show_object;
        break;
    case '\001':                                // <=> Ctrl A
        anti_alias = !anti_alias;
        printf("Anti aliasing ");
        if (!anti_alias) printf("non ");
        printf("actif\n");
        break;
    case 'f':                                   // Ajouté pour voir, mais effet ????
        flush = !flush;
        break;
    case 'n':
        tracer_normales = !tracer_normales;
        if (gouraud_prev == gouraud) gouraud_prev = !gouraud;    // juste une astuce pour forcer le recalcul des listes
        break;
    case 'g':
        gouraud_prev = gouraud;
        gouraud = !gouraud;
        printf("Lissage de Gouraud ");
        if (!gouraud) printf("non ");
        printf("actif\n");
        break;
//    case 'l':
//        light = !light;
//        if (light)
//            glEnable(GL_LIGHTING) ;
//        else
//            glDisable(GL_LIGHTING);
//        break;
    case '-':
        fov_mult += 0.05;
//            printf("fov_mult : %f\n",fov_mult);
        break;
    case '+':
        fov_mult -= 0.05;
        if (fov_mult < 0.05) fov_mult = 0.05;
//            printf("fov_mult : %f\n",fov_mult);
        break;
    }
    lib3ds_file_eval(f3ds, current_frame);
    glutPostRedisplay();
}


/*!
* Respond to mouse buttons.  Action depends on current operating mode.
*/
static void mouse_cb(int button, int state, int x, int y)
{
    mx = x;
    my = y;
    switch (button) {
    case GLUT_LEFT_BUTTON:
        switch (runMode) {
        case ROTATING:
            rotating = state == GLUT_DOWN;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


/*!
* Respond to mouse motions; left button rotates the image or performs
* other action according to current operating mode.
*/
static void drag_cb(int x, int y)
{
    if (rotating) {
        view_rotz += MOUSE_SCALE * (x - mx);
        view_rotx += MOUSE_SCALE * (y - my);
        mx = x;
        my = y;
        glutPostRedisplay();
    }
}


/*!
* Create camera and light icons
*/
static void create_icons()
{
    GLUquadricObj *qobj;

#define CBX .25 // camera body dimensions
#define CBY 1.5
#define CBZ 1.

    qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

// Caméras
    cameraList = glGenLists(1);
    glNewList(cameraList, GL_COMPILE);
        glMaterialfv(GL_FRONT, GL_AMBIENT,  dgrey);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  lgrey);
        glMaterialfv(GL_FRONT, GL_SPECULAR, black);
        glEnable(GL_CULL_FACE);
        solidBox(CBX, CBY, CBZ);
        glPushMatrix();
        glTranslatef(0., .9, 1.8);
        glRotatef(90., 0., 1., 0.);
        solidCylinder(1., CBX*2, 12);
        glTranslatef(0., -1.8, 0.);
        solidCylinder(1., CBX*2, 12);
        glPopMatrix();
        glDisable(GL_CULL_FACE);
        glPushMatrix();
        glTranslated(0., CBY, 0.);
        glRotated(-90., 1., 0., 0.);
        gluCylinder(qobj, .2, .5, 1., 12, 1);
        glPopMatrix();
    glEndList();

// Lumières
    lightList = glGenLists(1);
    glNewList(lightList, GL_COMPILE);
        glEnable(GL_CULL_FACE);                     // Ajout GD
        glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT,  dgrey);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  dgrey);
        glMaterialfv(GL_FRONT, GL_SPECULAR, grey);
//        glEnable(GL_CULL_FACE);                   // Suppression GD
        gluSphere(qobj, .5, 12., 6.);
        glRotated(180., 1., 0., 0.);
        glMaterialfv(GL_FRONT, GL_EMISSION, dgrey);
        gluCylinder(qobj, .2, .2, 1., 12, 1);
        glPopMatrix();
        glDisable(GL_CULL_FACE);                    // Ajout GD
    glEndList();
}


void decompose_datapath(const char *fn)
{
    const char *ptr = strrchr(fn, '/');

    if (ptr == NULL) {
        strcpy(datapath, ".");
        strcpy(filename, fn);
    } else {
        strcpy(filename, ptr + 1);
        strcpy(datapath, fn);
        datapath[ptr - fn] = '\0';
    }
}


/*!
*
*/
int main(int argc, char** argv)
{
    char *progname = argv[0];

    glutInit(&argc, argv);

    for (++argv; --argc > 0; ++argv) {
        if (strcmp(*argv, "-help") ==  0 || strcmp(*argv, "--help") == 0) {
            fputs("View a 3DS model file using OpenGL.\n", stderr);
            fputs("Usage: 3dsplayer [-nodb|-aa|-flush] <filename>\n", stderr);
            exit(0);
        } else if (strcmp(*argv, "-nodb") == 0)
            dbuf = 0;                               // -nodb <=> pas de doible buffer
        else if (strcmp(*argv, "-aa") == 0)
            anti_alias = 1;                         // -aa   <=> activer anti_aliasing
        else if (strcmp(*argv, "-flush") == 0)
            flush = 1;
        else {
            filepath = *argv;
            decompose_datapath(filepath);
        }
    }

    if (filepath == NULL) {
        fputs("3dsplayer: Error: No 3DS file specified\n", stderr);
        exit(1);
    }

    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | (dbuf ? GLUT_DOUBLE : 0));
    glutInitWindowSize(800, 800);   // 500,500
    glutInitWindowPosition(100, 100);
    glutCreateWindow(filepath != NULL ? Basename(filepath) : progname);

    init();
    create_icons();
    load_model();

    commandes_clavier();

    build_menu();
    glutAttachMenu(2);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_cb);
    glutMotionFunc(drag_cb);
    glutTimerFunc(1000 / FRAMES_PER_SECOND, timer_cb, 0);
    glutMainLoop();
    return(0);
}


/* A few small utilities, so generic that they probably
* don't even belong in this file.
*/


/*!
* Render a box, centered at 0,0,0
*
* Box may be rendered with face culling enabled.
*/
static void solidBox(double bx, double by, double bz)
{
    glBegin(GL_POLYGON);
        glNormal3d(0.,   0.,  1.);
        glVertex3d( bx,  by,  bz);
        glVertex3d(-bx,  by,  bz);
        glVertex3d(-bx, -by,  bz);
        glVertex3d( bx, -by,  bz);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.,   0., -1.);
        glVertex3d(-bx,  by, -bz);
        glVertex3d( bx,  by, -bz);
        glVertex3d( bx, -by, -bz);
        glVertex3d(-bx, -by, -bz);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.,  -1.,  0.);
        glVertex3d(-bx,  by,  bz);
        glVertex3d( bx,  by,  bz);
        glVertex3d( bx,  by, -bz);
        glVertex3d(-bx,  by, -bz);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.,  -1.,  0.);
        glVertex3d( bx, -by,  bz);
        glVertex3d(-bx, -by,  bz);
        glVertex3d(-bx, -by, -bz);
        glVertex3d( bx, -by, -bz);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(1.,   0.,  0.);
        glVertex3d(bx,   by,  bz);
        glVertex3d(bx,  -by,  bz);
        glVertex3d(bx,  -by, -bz);
        glVertex3d(bx,   by, -bz);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(-1.,  0., 0.);
        glVertex3d(-bx,  by, -bz);
        glVertex3d(-bx, -by, -bz);
        glVertex3d(-bx, -by,  bz);
        glVertex3d(-bx,  by,  bz);
    glEnd();
}


/*!
* Render a cylinder with end caps, along the Z axis centered at 0,0,0
*
* Cylinder may be rendered with face culling enabled.
*/
static void solidCylinder(double r, double h, int slices)
{
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    glPushMatrix();
    glTranslated(0., 0., -h / 2);
    gluCylinder(qobj, r, r, h, slices, 1);
    glPushMatrix();
    glRotated(180., 1., 0., 0.);
    gluDisk(qobj, 0., r, slices, 1);
    glPopMatrix();
    glTranslated(0., 0., h);
    gluDisk(qobj, 0., r, slices, 1);
    glPopMatrix();
}


static const char * Basename(const char *filename)
{
    const char *ptr = strrchr(filename, '/');
    return ptr != NULL ? ptr + 1 : filename;
}


/*
* This module is a crude front end to the GLUT menu system, allowing for
* slightly more sophisticated callbacks.
*/

#include <stdio.h>

#define MAX_CALLBACKS 100

typedef struct {
    void (*cb)(int, int, void *);
    void *client;
} Callback;


static Callback callbacks[MAX_CALLBACKS];
static int ncb = 0;

/*!
* Register a callback, returning an integer value suitable for
* passing to glutAddMenuEntry()
*
* \param cb Callback function to be called.
* \param client Data to be passed to the callback.
*
* \return integer callback id
*/
static int callback(void (*cb)(int, int, void *client), void *client)
{
    if (ncb == 0) {
        for (int i = 0; i < (int)NA(callbacks); ++i)
            callbacks[i].cb = NULL;
    } else if (ncb >= (int)NA(callbacks)) {
        fprintf(stderr,
                "callback() out of callbacks, try changing MAX_CALLBACKS\n");
    }

    callbacks[ncb].cb = cb;
    callbacks[ncb].client = client;
    return ncb++;
}


/*!
* Call the indexed callback.
*
* \param idx Callback index.
* \param data Data to be passed to the callback
*/
static void call_callback(int idx, int data)
{
    if (idx >= 0 && idx < (int)NA(callbacks) && callbacks[idx].cb != NULL)
        callbacks[idx].cb(idx, data, callbacks[idx].client);
}
