#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <wx/app.h>
#include <wx/dcclient.h>
#include <wx/frame.h>
#include <wx/wx.h>
#include <wx/dialog.h>
#include "wx/glcanvas.h"
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include "wx/wfstream.h"
#include "wx/txtstrm.h"
#include "trackball.h"
#include <GL/freeglut.h>
//#include <GL/glext.h>       // Pour tests antialising des polygones (définition de GL_MULTISAMPLE entre autres) sinon à supprimer car inclus dans freeglut.h
//#include <GL/gl.h>        // Pas utile car fait dans glut.h
//#include <GL/glu.h>       // Idem
#include "GLCanvas.h"
#include <float.h>
#include <vector> //Ne pas oublier !
#include <list>
#include <algorithm>
#include <ctime>
#include "vector3d.h"
#include "lib3ds/lib3ds.h"
#include "libexpat/expat.h"
#include "charset.h"

#define BUFSIZE 4096
#define gris_def 0.5f   // 0.35f version jdias
#define groupe_def      -123
#define codmatface_def  -234
#define maxGroupes 16

#ifndef GL_MULTISAMPLE                  // pour éviter le include de glew.h et GL_MULTISAMPLE_ARB, sinon GL_MULTISAMPLE existe dans gl.h mais n'est pas activé sous Windows !
    #define GL_MULTISAMPLE      0x809D  // il faudrait glext.h et aussi peut-être glext.dll, mais en 64 bits !
#endif // GL_MULTISAMPLE
#ifndef GL_SAMPLE_BUFFERS
    #define GL_SAMPLE_BUFFERS   0x80A8
#endif // GL_SAMPLE_BUFFERS
#ifndef GL_SAMPLES
    #define GL_SAMPLES          0x80A9
#endif // GL_SAMPLES


class Aide_html;
class BddInter;
class OvniFrame;
class CentreRotation;
class ChangerEchelleBdd;
class ChoixAffichageObjets;
class CouleursGroupes;
class Cone;
class Cube;
class Cylindre;
class DeplacerBdd;
class Ellipsoide;
class Facette;
class Icosaedre;
class ModificationPanel;
class PositionObs_AzimutSite;
class PositionSource;
class Prefs_Dialog;
class ReperageFacette;
class ReperageGroupe;
class ReperageMateriau;
class ReperageObjet;
class ReperagePoint;
class SelectionPanel;
class SelectionManuelleFacettes;
class SelectionManuelleObjets;
class ManipulationsPanel;
class TranslationPanel;
class RotationPanel;
class ScalePanel;
class Sphere;
class ZoomSpecifique;

class Face;
class Sommet;
class Luminance;
class Vecteur;
class Points;
class Aretes;

#include "OvniMain.h"

//using namespace std;    // Si absent, vector ne semble pas toujours reconnu ! il faut std::vector dans ce cas partout ???? mais aussi sur string, list, ...

// Dans la version Tcl, mais ne semble pas bien adapté ici !
char *iso_To_ibm (char *) ;
char *ibm_To_iso (char *) ;
// mieux ...
char *utf8_To_ibm (char *) ;

// Les classes
//************

class Object {
    wxCharBuffer name;
    wxString wxname;
    int value;                  // Numéro de l'objet
public:
    bool afficher = true;
//    bool show    = true;      // Non utilisé ???
    bool deleted  = false;      // Objet à supprimer à l'enregistrement, mais toujours en mémoire
    bool selected = false;      // Objet sélectionné
    bool primitive= false;      // Si true, l'objet est une des primitives ajoutées
    bool flat     = true;       // Si true, l'objet sera considéré comme ne contenant que des facettes planes
                                // => smooth sans effet, pas besoin de "Luminance" ni de "vecteur" (normales aux sommets)
    bool mat_position = false;  // Par défaut, pas de matrice POSITION
    double matrice[16];

    unsigned int  Nb_facettes;
    unsigned int  Nb_facettes_original;
    unsigned int  Nb_sommets;
    unsigned int  Nb_aspects;
    unsigned int  Nb_normales;
    unsigned int  Nb_luminances;
    unsigned int  Nb_vecteurs;
    unsigned int  Nb_aretes;
    unsigned int  Nb_matrices;
    clock_t       Temps_Calcul_Aretes;
    std::vector<Face>    Facelist;
    std::vector<Sommet>  Sommetlist;
    std::vector<Vecteur> Vecteurlist;
    std::vector<Points>  Pointslist;
    std::vector<Aretes>  Areteslist;

// Méthodes, fonctions de la classe Object

    Object() {
    }
    void initObject(wxString& name) {
        this->name   = name.mb_str();
        this->wxname = name;
        Nb_facettes  = Nb_facettes_original = 0;
        Nb_sommets   = 0;
        Nb_normales  = 0;
        Nb_aspects   = 0;
        Nb_luminances= 0;
        Nb_vecteurs  = 0;
        Nb_aretes    = 0;
        Nb_matrices  = 0;
        Temps_Calcul_Aretes = 0;
    }
    Object(wxString& name, wxString& value) {
        initObject(name);
        this->value = wxAtoi(value);
    }
    Object(wxString& name, int& value) {
        initObject(name);
        this->value = value;
    }
    int GetValue() {
        return this->value;
    }
    void SetValue(int numero) {
        this->value = numero;
        return ;
    }
    char* GetName() {
        return this->name.data();
    }
    void SetName(wxString& name) {
        this->name   = name.mb_str();   // Attention ici : souci s'il y a des caractères accentués !
        this->wxname = name;
    }
    wxString GetwxName() {
        return this->wxname;
    }
};

class Points {                  // Est-ce utile que Points soit une classe ?
public:
    std::vector<int> IndicesFacettes;
};

class Aretes {
public:
    bool afficher = true;       // Booléen pour afficher (ou pas) l'arête
    bool deleted  = false;      // Booléen pour marquer une arête comme supprimée (car doublon notamment en méthode de détection 2 !)
    unsigned int ind_a, ind_b;  // Indices des 2 points extrémités de l'arête
//    std::vector<float> *point_a;     // Pointeur sur les coordonnées du point a  // A supprimer car non utilisé ???
//    std::vector<float> *point_b;     // Pointeur sur les coordonnées du point b

    Aretes() {
    }
};

class Face {
public:
    bool afficher;                  // true : afficher la facette, false : la masquer
    bool deleted;                   // Facette supprimée (en fait à supprimer à l'enregistrement, car toujours présente en mémoire)
    bool selected;                  // Facette sélectionnée si true
    bool flat;                      // Si true, la facette est plane (pas de smooth)
    int  Numero;                    // Numéro de la facette
    int  Nb_Sommets_F;              // Nombre de sommets de la facette (équivallent de Nb_Sommets_L pour Luminance)
    std::vector<float> normale_b ;  // Normale au barycentre de la facette
    std::vector<int> F_sommets;     // Numéros des sommets composant la facette
    int  groupe;                    // Numéro de groupe de la facette
    int  codmatface;                // Numéro de matériau de la facette
//    bool show;                    // ATTENTION, différent de show pour les points. Utilisé dans souderPoints ... mais au final synonyme de !deleted !!!!
    int  toshow;                    // ?? Semble servir dans souder gotodivise et dans le undo général
//    int  Rtoshow;                 // ?? N'est pas utilisé actuellement
    GLfloat color[3];
    int  Nb_Sommets_L=0;            // Nombre de sommets en luminances (équivallent de Nb_Sommets_F pour les sommets de facettes)
    std::vector<int> L_sommets;     // Numéros des normales aux sommets

// Méthodes, fonctions de la classe Face (traitements d'une facette)

    Face() {
    }

    void initFace() {
        afficher= true;                     // Par défaut, afficher la facette
        deleted = false;                    // Par défaut, la facette n'est pas à supprimer
        selected= false;                    // Par défaut, la facette n'est pas sélectionnée
        flat    = true;                     // Par défaut, la facette est plane
        color[0]= color[1]= color[2]= gris_def;
        normale_b.resize(3);                // Une normale au barycentre a 3 composantes flottantes
        groupe     = groupe_def;            // Numéro de groupe
        codmatface = codmatface_def;        // Numéro de matériau (codmatface pour le format SDM)
//        show    = true;                   // Ne sert plus, doublon de !deleted
        toshow  = 0;
//        Rtoshow = 0;
        F_sommets.clear();                  // RAZ du tableau des sommets de la facette
        Nb_Sommets_F = 0;                   // Nombre de sommets de la facette
        L_sommets.clear();                  // RAZ du tableau des sommets pour les normales aux sommets
        Nb_Sommets_L = 0;                   // Nombre de sommets pour les Luminances
    }
    Face(std::vector<wxString> wxStringlist) {
        initFace();
        Numero       = wxAtoi(wxStringlist[0]);
        Nb_Sommets_F = wxAtoi(wxStringlist[1]);
        for(int i=0; i<Nb_Sommets_F; i++) {
            F_sommets.push_back(wxAtoi(wxStringlist[i+2]));
        }
    }
    Face(int number, std::vector<int> NumerosSommets) {
        initFace();
        Numero       = number;
        Nb_Sommets_F = NumerosSommets.size();
        for(int i=0; i<Nb_Sommets_F; i++) {
            F_sommets.push_back(NumerosSommets[i]);
        }
    }
    int getSize() {                         // Ne sert pas ?
        return this->F_sommets.size();
    };
    int getNumero() {
        return this->Numero;
    };
    int getNb_Sommets_F() {
        return this->Nb_Sommets_F;
    };
    std::vector<int> getF_sommets() {
        return this->F_sommets;
    };
    std::vector<float> getNormale_b() {
        return this->normale_b;
    };
    void setFsommet(std::vector<int> new_point) {
        F_sommets    = new_point;
        Nb_Sommets_F = F_sommets.size();
    }
    void setNormale_b(std::vector<float> new_normale) {
        normale_b = new_normale;
    }
    void setNormale_b(float new_normale[3]) {
        for (int i=0; i<3; i++) normale_b[i] = new_normale[i];
    }
    std::vector<int> getL_sommets() {
        return this->L_sommets;
    };
    void setLsommet(std::vector<int> new_point) {
        L_sommets    = new_point;
        Nb_Sommets_L = L_sommets.size();
    }
    int getGroupe() {
        return this->groupe;
    };
    int getCodmatface() {
        return this->codmatface;
    };
    void setGroupe(wxString& groupe) {
        this->groupe = wxAtoi(groupe);
    };
    int setGroupe(int groupe) {                     // Ne sert plus. Devrait être avec retour void plutôt que int
        return this->groupe = groupe;
    };
    void setCodmatface(wxString& codmatface) {
        this->codmatface = wxAtoi(codmatface);
    };
    int setCodmatface(int codmatface) {             // Idem setgroupe
        return this->codmatface = codmatface;
    };
    void setNewSommet_F(int k,int new_s) {
        F_sommets[k] = new_s;
    };
    void setNewSommet_L(int k,int new_s) {
        L_sommets[k] = new_s;
    };
    void deleteone(int toUp) {
        F_sommets.erase(F_sommets.begin()+toUp);
        Nb_Sommets_F--;
    };
};

class Sommet {
public:
    int  Numero;                // Numéro du point (en fait, c'est indice du sommet +1 en général). Initialisé, mis à jour mais au final non utilisé ! A supprimer ?
    int  NNumber;               // ?? ne sert que dans Simplification_BDD_old ... A supprimer ???? Quelle est sa fonction ???
    std::vector<float> point;   // Coordonnées x, y et z du point
    int  toshow;
    bool show;
    bool selected;
//    GLfloat color[3];

// Méthodes, fonctions de la classe Sommet (traitements d'un sommet)

    Sommet() {
        NNumber=-1;
    }
    void initSommet() {
        show     = true;
        NNumber  = -1;
        toshow   = 0;
        selected = false;
//        color = bleu; // Pas encore défini !!!
//        color[0]=0. ; color[1]=0. ; color[2]=1. ;
    }
    Sommet(std::vector<wxString>wxStringlist) {
        double a;
        initSommet();
        Numero = wxAtoi(wxStringlist[0]);
        for(int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Sommet(int& number, float xyz[3]) {
        initSommet();
        Numero = number;
        for(int i=0; i<3; i++) point.push_back(xyz[i]);
     }
    Sommet(int& number, std::vector<float> xyz) {
        initSommet();
        Numero = number;
        for(int i=0; i<3; i++) point.push_back(xyz[i]);
    }
    int getNumero() {
        return this->Numero;
    };
    std::vector<float> getPoint() {
        return this->point;
    };
    void setPoint(std::vector<float> new_point) {
        point=new_point;
    }
};

class Normale {
public:
    int Numero = 0;             // Numéro de la normale : init GD
    std::vector<float> point;   // Coordonnées de la normale en x, y et z
    int toshow = 0;             // init GD
    bool show  = false;         // init GD

// Méthodes, fonctions de la classe Normale (traitements d'une normale au barycentre)

    Normale() {
    }
    void initNormale() {
        show  = true;
        toshow= 1;
    }
    Normale(std::vector<wxString>wxStringlist) {
        double  a;
        initNormale();
        Numero = wxAtoi(wxStringlist[0]);
        for(int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Normale(int& number, float xyz[3]) {
        initNormale();
        Numero = number;
        for(int i=0; i<3; i++) point.push_back(xyz[i]);
    }
    int getNumero() {
        return this->Numero;
    };
    std::vector<float> getPoint() {
        return this->point;
    };
    void setPoint(std::vector<float> new_point) {
        point = new_point;
    }
};

class Aspect_face {
public:
    int  groupe;
    int  codmatface;
    int  Numero;        // Numéro de la facette
    int  toshow;
    bool show;

// Méthodes, fonctions de la classe Aspect_face (traitements du numéro de matériau et/ou de groupe d'une facette)

    Aspect_face() {
        show=true;
    }
    Aspect_face(std::vector<wxString>wxStringlist) {
        show=true;
        Numero=wxAtoi(wxStringlist[0]); // On ne récupère que le numéro. Pour groupe et codmatface, sera fait plus tard
//        wxString code=wxStringlist[1] ;
//        if (code.Contains(_T("<GROUPE>"))) {
//            groupe    = wxAtoi(wxStringlist[2]);
//            codematface = -1;
//        } else {
//            codmatface= wxAtoi(wxStringlist[2]);
//        }
        //printf("\ntest %d %d",Numero,groupe);
//        codmatface=-1;
    }
    int getNumero() {
        return this->Numero;
    };
    int getGroupe() {
        return this->groupe;
    };
    int getCodmatface() {
        return this->codmatface;
    };
    int setNumero(wxString& Numero) {
        return this->Numero = wxAtoi(Numero);
    };
    int setPoint(wxString& groupe) {
        return this->groupe = wxAtoi(groupe);
    };
    void setGroupe(wxString& groupe) {
        this->groupe=wxAtoi(groupe);
    };
    int setGroupe(int groupe) {
        return this->groupe = groupe;
    };
    void setCodmatface(wxString& codmatface) {
        this->codmatface = wxAtoi(codmatface);
    };
    int setCodmatface(int codmatface) {
        return this->codmatface = codmatface;
    };
};

class Luminance {
public:
    int  Numero;                // Numéro de facette
    int  Nb_Sommets_L=0;        // Nombre de sommets (équivallent de Nb_Sommets_F pour Face)
    std::vector<int> L_sommets; // Numéros des normales aux sommets
    int  toshow;
    bool show;

// Méthodes, fonctions de la classe Luminance (traitements d'une Luminance <=> facette)

    Luminance() {
    }
    Luminance(std::vector<wxString>wxStringlist) {
        show         = true;
        Numero       = wxAtoi(wxStringlist[0]);
        Nb_Sommets_L = wxAtoi(wxStringlist[1]);
        for(int i(0); i<Nb_Sommets_L; i++) {
            L_sommets.push_back(wxAtoi(wxStringlist[i+2]));
        }
    }
    Luminance(int number, std::vector<int> NumerosSommets) {
        show         = true;
        Numero       = number;
        Nb_Sommets_L = NumerosSommets.size();
        for(int i=0; i<Nb_Sommets_L; i++) {
            L_sommets.push_back(NumerosSommets[i]);
        }
    }
//    void setNewSommet_L(int k,int new_s) {
//        L_sommets[k] = new_s;
//    };
//    int getNumero() {
//        return this->Numero;
//    };
//    std::vector<int> getL_sommets() {
//        return this->L_sommets;
//    };
//    void setLsommet(std::vector<int> new_point) {
//        L_sommets = new_point;
//    }
};

class Vecteur {
public:
    int  Numero;                // Numéro de normale au sommet (ou VECTEUR) (en fait, c'est indice du sommet +1 en général). Initialisé, mis à jour mais au final non utilisé !
//    int  NNumber;             // Sert à quoi ???
    std::vector<float> point;   // Coordonnées en x, y et z de cette normale
    int  toshow;
//    bool show;                // Ne sert plus !

// Méthodes, fonctions de la classe Vecteur (traitements d'un Vecteur pour les normales aux sommets)

    Vecteur() {
//        NNumber=-1;
    }
    void initVecteur() {
//        show    = true;
//        NNumber = -1;
        toshow  = 0;
    }
    Vecteur(std::vector<wxString>wxStringlist) {
        double a;
        initVecteur();
        Numero = wxAtoi(wxStringlist[0]);
        for(int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Vecteur(int& number, float xyz[3]) {
        initVecteur();
        Numero = number;
        for(int i=0; i<3; i++) point.push_back(xyz[i]);
    }
    Vecteur(int& number, std::vector<float> xyz) {
        initVecteur();
        Numero = number;
        for(int i=0; i<3; i++) point.push_back(xyz[i]);
    }
    int getNumero() {
        return this->Numero;
    };
    std::vector<float> getPoint() {
        return this->point;
    };
    void setPoint(std::vector<float> new_point) {
        point = new_point;
    }
};

class ListFace {
public:
    int objet;
    int face_sommet;
    ListFace(int objet,int face_sommet) {
        this->objet       = objet;
        this->face_sommet = face_sommet;    // C'est un numéro de facette dans le cas général, mais c'est un numéro de sommet en mode selection_point
    }
};

class ListeSelected {
    unsigned int i;
    bool verrouiller_liste;

public:
    std::vector<ListFace> ListeSelect;

    ListeSelected() {
    };
    void verrouiller_ListeSelect(bool verrou) {
        verrouiller_liste = verrou;           // Pour empêcher de recommencer une sélection de points tant qu'on n'a pas relaché la souris
    };
    bool getVerrou_ListeSelect() {
        return verrouiller_liste;
    };
    bool check_if_in_ListeSelect(int objet,int face_sommet) {
        if(ListeSelect.size()==0) {
            return false;
        }
        for(i=0; i<ListeSelect.size(); i++) {
            if((ListeSelect[i].objet==objet) && (ListeSelect[i].face_sommet==face_sommet)) {
                return true;
            }
        }
        return false;
    };
    void add_one(int objet,int face_sommet) {
        ListFace *Make=new ListFace(objet,face_sommet);
        ListeSelect.push_back(*Make);
    };
    void erase_one_ListeSelect(int objet,int face_sommet) {
        if (check_if_in_ListeSelect(objet,face_sommet)) ListeSelect.erase(ListeSelect.begin()+i); // On récupère l'indice i trouvé de ListeSelected
    };
};

class Sommetmemory {
public:
    int objet;
    int sommet;
    Sommetmemory(int objet,int sommet) {
        this->objet = objet;
        this->sommet= sommet;
    }
};

class BddInter: public wxGLCanvas {
// Variables et fonctions internes à la gestion, manipulation des BDDs.
    wxString file;
    wxString first_file;
    wxString token;
    wxCharBuffer buffer;
//    Sommetmemory *Smemory;
///    std::vector<std::vector<StoF> > StofM;

    std::vector<wxString>     wxStringlist;

    bool finishdraw = false;    // Sert dans OnMouse à tout court-circuiter si rien d'affiché à l'écran. Pourrait être supprimé car pas de souci si toujours à true !
    float matquat[4][4];
//    double diagonale_save ;
    double FoV_auto;
    float  longueur_normales;
    GLint xd_sel,yd_sel,xa_sel,ya_sel;      // Coordonnées écran pour le tracé d'un rectangle de sélection (d=début - a=arrivée)
//	GLdouble select_largeur=1.0, select_hauteur=1.0;    // doivent être d'au moins 1 pixel
//	GLdouble xmil,ymil;

    int  undo_memory;
    int  objet_under_mouse;
    int  face_under_mouse;
    int  point_under_mouse;
    int  line_under_mouse;
    int  objet_under_mouse_old = -1;
    int  face_under_mouse_old  = -1;
    int  point_under_mouse_old = -1;
    int  line_under_mouse_old  = -1;

// Déclarations / Initialisations pour OpenGL

    GLfloat width_pixel = 1.;       // Largeur de zone pour sélectionner 1 pixel (sélection de facettes ou d'objets)
    GLfloat width_point = 5.;       // Largeur de zone pour sélectionner 1 point
    GLfloat width_ligne = 4.;       // Largeur de zone pour sélectionner 1 ligne
    GLint   offset_pointeur = 4;    // Pour rattraper un petit décalage en Y entre le pointeur de souris et un sommet pointé (devrait être nul !!!).
    DXFRenderer m_renderer;
//    GLfloat Light0Position_def[4]= {4.0f,4.0f,2.0f,0.0f};
//    GLfloat light0_dir[3]= {0.0,0.0,0.0};
    GLfloat Light0Ambient[4]      = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat Light0Diffuse[4]      = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat Light0Specular[4]     = {0.3f, 0.3f, 0.3f, 1.0f};

/*! Paramètres du matériau utilisé pour l'avion général (gris)*/
    GLfloat MatAmbient_avion[4]   = {0.5f, 0.5f, 0.5f, 0.8f};
    GLfloat MatDiffuse_avion[4]   = {0.9f, 0.9f, 0.9f, 0.8f};
    GLfloat MatSpecular_avion[4]  = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat MatShininess_avion[1] = {5.0f};
/*! Variante */
    GLfloat MatSpecular_avionG[4] = {1.0f, 1.0f, 1.0f, 1.0f};   // Réflexion spéculaire plus forte
    GLfloat MatShininess_avionG[1]= {50.0f};                    // Idem brillance

/*! Paramètres du matériau utilisé pour le repérage d'un groupe de facettes (bleu)*/
    GLfloat MatAmbient_avionB[4]  = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat MatDiffuse_avionB[4]  = {0.0f, 0.0f, 0.9f, 1.0f};
    GLfloat MatSpecular_avionB[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat MatShininess_avionB[1]= {50.0f};

/*! Paramètres du matériau utilisé pour le repérage en Bleu-grisé */
    GLfloat MatAmbient_avionBG[4] = {0.4f, 0.4f, 0.9f, 1.0f};
    GLfloat MatDiffuse_avionBG[4] = {0.4f, 0.4f, 0.9f, 1.0f};

/*! Paramètres du matériau utilisé pour l'avion Rouge_grisé */
    GLfloat MatAmbient_avionRG[4] = {1.0f, 0.4f, 0.4f, 1.0f};
    GLfloat MatDiffuse_avionRG[4] = {1.0f, 0.4f, 0.4f, 1.0f};

/*! Paramètres du matériau utilisé pour le repérage d'une facette particulière (jaune)*/
    GLfloat MatAmbient_avionJ[4]  = {0.5f, 0.5f, 0.0f, 1.0f};
    GLfloat MatDiffuse_avionJ[4]  = {0.9f, 0.9f, 0.0f, 1.0f};
    GLfloat MatSpecular_avionJ[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat MatShininess_avionJ[1]= {50.0f};

/*! Paramètres du matériau utilisé pour l'avion rouge (objet)*/
    GLfloat MatAmbient_avionR[4]  = {0.5f, 0.0f, 0.0f, 1.0f};
    GLfloat MatDiffuse_avionR[4]  = {0.9f, 0.0f, 0.0f, 1.0f};
    GLfloat MatSpecular_avionR[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat MatShininess_avionR[1]= {50.0f};

/////////////////////////////////////

    GLfloat rouge[3] = {1.0f, 0.0f, 0.0f};
    GLfloat vert[3]  = {0.0f, 1.0f, 0.0f};
    GLfloat vert_f[3]= {0.0f, 0.5f, 0.0f};              // Vert foncé
    GLfloat vert_c[3]= {0.5f, 1.0f, 0.5f};              // Vert clair
    GLfloat bleu[3]  = {0.0f, 0.0f, 1.0f};
    GLfloat blanc[3] = {1.0f, 1.0f, 1.0f};
    GLfloat noir[3]  = {0.0f, 0.0f, 0.0f};
    GLfloat jaune[3] = {1.0f, 1.0f, 0.0f};
    GLfloat violet[3]= {1.0f, 0.0f, 1.0f};
    GLfloat cyan[3]  = {0.0f, 1.0f, 1.0f};
    GLfloat cyan_f[3]= {0.0f, 0.5f, 0.5f};              // Cyan foncé
    GLfloat cyan_c[3]= {0.5f, 1.0f, 1.0f};              // Cyan clair
    GLfloat gris[3]  = {gris_def, gris_def, gris_def};  // Gris par défaut

public :
    unsigned int Numero_base;
    double diagonale_save ;

/*! Paramètres du matériau utilisé pour la colorisation de l'avion par groupes et/ou matériaux */
    static const int nb_couleurs=32;

    // Ces 2 tableaux pourraient être en private mais il faut que nb_couleurs reste en public. à donc déclarer avant !
    // => réordonner complètement public, private et protected
    GLfloat MatAmbient_avionG_def[nb_couleurs][4] ={{0.5f, 0.5f, 0.5f, 1.0f} ,  // Groupe 0
                                                    {0.9f, 0.3f, 0.0f, 1.0f} ,  // Groupe 1
                                                    {0.7f, 0.3f, 0.1f, 1.0f} ,  // Groupe 2
                                                    {0.5f, 0.3f, 0.2f, 1.0f} ,  // Groupe 3
                                                    {0.5f, 0.8f, 0.0f, 1.0f} ,  // Groupe 4
                                                    {0.9f, 0.2f, 0.0f, 1.0f} ,  // Groupe 5
                                                    {0.7f, 0.2f, 0.2f, 1.0f} ,  // Groupe 6
                                                    {0.5f, 0.2f, 0.2f, 1.0f} ,  // Groupe 7
                                                    {0.8f, 0.0f, 0.4f, 1.0f} ,  // Groupe 8
                                                    {0.0f, 0.0f, 0.5f, 1.0f} ,  // Groupe 9
                                                    {1.0f, 0.0f, 0.0f, 1.0f} ,  // Groupe 10
                                                    {0.6f, 1.0f, 0.0f, 1.0f} ,  // Groupe 11
                                                    {1.0f, 0.98f,0.0f, 1.0f} ,  // Groupe 12
                                                    {0.2f, 0.2f, 0.5f, 1.0f} ,  // Groupe 13
                                                    {0.3f, 0.3f, 1.0f, 1.0f} ,  // Groupe 14
                                                    {0.3f, 0.3f, 0.3f, 1.0f} ,  // Groupe 15
                                                    {0.4f, 0.4f, 0.4f, 1.0f} ,  // Groupe 15
                                                    {0.6f, 0.6f, 0.6f, 1.0f} ,  // Groupe 17
                                                    {0.7f, 0.7f, 0.7f, 1.0f} ,  // Groupe 18
                                                    {0.8f, 0.8f, 0.8f, 1.0f} ,  // Groupe 19
                                                    {0.6f, 0.7f, 0.8f, 1.0f} ,  // Groupe 20
                                                    {0.6f, 0.7f, 0.8f, 1.0f} ,  // Groupe 21
                                                    {0.7f, 0.8f, 0.6f, 1.0f} ,  // Groupe 22
                                                    {0.8f, 0.6f, 0.7f, 1.0f} ,  // Groupe 23
                                                    {0.2f, 0.4f, 0.6f, 1.0f} ,  // Groupe 24
                                                    {0.4f, 0.6f, 0.2f, 1.0f} ,  // Groupe 25
                                                    {0.6f, 0.2f, 0.4f, 1.0f} ,  // Groupe 26
                                                    {0.1f, 0.4f, 0.7f, 1.0f} ,  // Groupe 27
                                                    {0.4f, 0.7f, 0.1f, 1.0f} ,  // Groupe 28
                                                    {0.7f, 0.1f, 0.4f, 1.0f} ,  // Groupe 29
                                                    {0.5f, 0.5f, 0.5f, 1.0f} ,  // Groupe 30
                                                    {0.9f, 0.9f, 0.9f, 3.0f}    // Groupe 31
    };

    GLfloat MatDiffuse_avionG_def[nb_couleurs][4] ={{0.5f, 0.5f, 0.5f, 1.0f} ,	// Groupe 0
                                                    {0.9f, 0.3f, 0.0f, 1.0f} ,  // Groupe 1
                                                    {0.7f, 0.3f, 0.1f, 1.0f} ,  // Groupe 2
                                                    {0.5f, 0.3f, 0.2f, 1.0f} ,  // Groupe 3
                                                    {0.5f, 0.8f, 0.0f, 1.0f} ,  // Groupe 4
                                                    {0.9f, 0.2f, 0.0f, 1.0f} ,  // Groupe 5
                                                    {0.7f, 0.2f, 0.2f, 1.0f} ,  // Groupe 6
                                                    {0.5f, 0.2f, 0.2f, 1.0f} ,  // Groupe 7
                                                    {0.8f, 0.0f, 0.4f, 1.0f} ,  // Groupe 8
                                                    {0.0f, 0.0f, 0.5f, 1.0f} ,  // Groupe 9
                                                    {1.0f, 0.0f, 0.0f, 1.0f} ,  // Groupe 10
                                                    {0.6f, 1.0f, 0.0f, 1.0f} ,  // Groupe 11
                                                    {1.0f, 0.98f,0.0f, 1.0f} ,  // Groupe 12
                                                    {0.2f, 0.2f, 0.5f, 1.0f} ,  // Groupe 13
                                                    {0.3f, 0.3f, 1.0f, 1.0f} ,  // Groupe 14
                                                    {0.3f, 0.3f, 0.3f, 1.0f} ,  // Groupe 15
                                                    {0.4f, 0.4f, 0.4f, 1.0f} ,  // Groupe 15
                                                    {0.6f, 0.6f, 0.6f, 1.0f} ,  // Groupe 17
                                                    {0.7f, 0.7f, 0.7f, 1.0f} ,  // Groupe 18
                                                    {0.6f, 0.7f, 0.8f, 1.0f} ,  // Groupe 20
                                                    {0.6f, 0.7f, 0.8f, 1.0f} ,  // Groupe 21
                                                    {0.7f, 0.8f, 0.6f, 1.0f} ,  // Groupe 22
                                                    {0.8f, 0.6f, 0.7f, 1.0f} ,  // Groupe 23
                                                    {0.2f, 0.4f, 0.6f, 1.0f} ,  // Groupe 24
                                                    {0.4f, 0.6f, 0.2f, 1.0f} ,  // Groupe 25
                                                    {0.6f, 0.2f, 0.4f, 1.0f} ,  // Groupe 26
                                                    {0.1f, 0.4f, 0.7f, 1.0f} ,  // Groupe 27
                                                    {0.4f, 0.7f, 0.1f, 1.0f} ,  // Groupe 28
                                                    {0.7f, 0.1f, 0.4f, 1.0f} ,  // Groupe 29
                                                    {0.5f, 0.5f, 0.5f, 1.0f} ,  // Groupe 30
                                                    {0.9f, 0.9f, 0.9f, 2.0f}    // Groupe 31
    };

    GLfloat MatAmbient_avionG[nb_couleurs][4];
    GLfloat MatDiffuse_avionG[nb_couleurs][4];

    GLenum style= GL_POLYGON ; //GL_LINE_LOOP;     /*!reglage mode par defaut*/
    typedef enum {points,aretes,standard} MODE;

    MODE  modeGL;
    Sommetmemory *Smemory;
    unsigned int indiceObjet_courant;
    std::vector<Object> Objetlist;

    int type    = 0;
    int type_new=-1;
    int N_elements;
    float xyz[3];
    wxString str;
    std::string ligne;
    std::vector<int>   NumerosSommets;
    std::vector<float> centre_auto;          // Coordonnées du centre de rotation en automatique (centre de la boîte englobante)
    std::vector<float> centreRot;            // Coordonnées du centre de rotation effectif

    bool m_loaded = false;
    bool New_aspect_face = false ;

    ListeSelected ToSelect;

    typedef enum {selection_point,selection_arete,selection_facette,selection_objet,aucune} SELECTION;
    SELECTION mode_selection;

    void Setxyz(float x, float y, float z) {
        xyz[0] = x; xyz[1] = y; xyz[2] = z;
    }
    void Set_numeros(std::vector<int> Numeros) {
        NumerosSommets = Numeros;
    }

    float x_min, x_max, y_min, y_max, z_min, z_max;
    bool  AfficherNormaleFacette  = false ;
    bool  AfficherNormalesSommets = false ;
    bool  centrageRotAuto = true;

// Valeurs par défaut

    float len_axe_def       = 0.25;
    float len_normales_def  = 0.05;
    float ray_sun_def       = 0.03;
    float tolerance_def     = 0.001;                // En % de la diagonale de la boîte englobante
    float angle_Gouraud_def = 35.0f;
    float fmult_Gouraud_def =  1.6f;
    int   svg_time_def      = 5;                    // en minutes
    bool  test_seuil_gouraud_def  = false;

    bool  antialiasing_soft_def        = false;     // Antialiasing par défaut non fait par OpenGL pour éviter les soucis avec cartes graphiques ne le supportant pas
    bool  Forcer_1_Seul_Objet_def      = false;     // Pour forcer la lecture des fichiers .obj dans 1 seul Objet 3D
    bool  lect_obj_opt_def             = false;     // Pour activer une lecture optimisée des fichiers .obj (suppression des sommets/vecteurs inutiles sur objets multiples)
    bool  test_decalage3ds_def         = true;      // Tenir compte d'un éventuel décalage d'objet 3ds (prise en compte du pivot). Ne marche pas toujours bien ...
    bool  CalculNormalesLectureBdd_def = false;     // Pour forcer le calcul (recalcul) des normales dès la lecture du fichier
    bool  Enr_Normales_Seuillees_def   = false;
    bool  reset_zoom                   = true;
    bool  bdd_modifiee                 = false;     // Pour indiquer que la base de données a été modifiée => proposer de l'enregistrer si ça n'a pas été fait
    bool  creation_facette             = false;     // Pour traiter différement le sélection de points en mode de création de facettes
    bool  CreerBackup_def              = false;     // Pour renommer l'original d'un fichier de bdd en .bak
    bool  SupprBackup_def              = false;     // Pour supprimer le .bak en sortie de programme
    bool  Raz_Selection_F_def          = false;     // Indicateur de Reset de sélection de facettes après une inversion de normales ('S' automatique après un 'I' ou un 'P')
    bool  NotFlat_def                  = false;     // Lors  d'un recalcul des normales, ne change pas les facettes déclarées plane. Si true, lissage de Gouraud actif sur la/les facette(s)
    bool  msg_warning_def              = true;      // Affiche un message sous forme de dialogue si la carte graphique ne supporte pas WX_GL_SAMPLE_BUFFERS (antialiasing par multisample)

    GLfloat Light0Position_def[4]= {4.0f, 4.0f, 2.0f, 0.0f};    // a paramétrer / diagonale surtout si petits objets
                                                                // OK avec modif dans AfficherSource : Ce sont des coordonnées absolues
                                                                // sur M2000 et pour les autres Bdd donnera la même position sur l'écran
    int   mode_Trackball_def=1;                     // mode par défaut : 1 = Trackball, 0 = Rotation directe
    int   methode_Triangulation_def=1;              // mode par défaut 0, 1 ou 2
    float fmult_diag_def = 10.0f;                   // Pour calcul de la distance de visée comme multiple de la diagonale de boîte englobante

// Valeurs de travail
    float len_axe;
    float len_normales;
    float ray_sun;
    float tolerance;
    bool  test_seuil_gouraud       = test_seuil_gouraud_def;
    float angle_Gouraud            = angle_Gouraud_def;
    float fmult_Gouraud            = fmult_Gouraud_def;
    float seuil_Gouraud;
    float angle_Gouraud2           = angle_Gouraud*fmult_Gouraud;
    float seuil_Gouraud2;
    int   svg_time                 = svg_time_def;
    bool  antialiasing_soft        = antialiasing_soft_def;
    bool  Forcer_1_Seul_Objet      = Forcer_1_Seul_Objet_def;
    bool  lect_obj_opt             = lect_obj_opt_def;
    bool  test_decalage3ds         = test_decalage3ds_def;
    int   methode_Triangulation    = methode_Triangulation_def;
    bool  CalculNormalesLectureBdd = CalculNormalesLectureBdd_def;
    bool  Enr_Normales_Seuillees   = Enr_Normales_Seuillees_def;
    bool  CreerBackup              = CreerBackup_def;
    bool  SupprBackup              = SupprBackup_def;
    bool  OK_FichierCree           = false;
    bool  Raz_Selection_F          = Raz_Selection_F_def;
    bool  NotFlat                  = NotFlat_def;
    bool  msg_warning              = msg_warning_def;

    GLfloat Light0Position[4];      // a paramétrer / diagonale surtout si petits objets car 4 * 4 * 2 m peut être trop loin

    unsigned int nb_objets;
    unsigned int nb_objets_reels;   // Pour compter seulement les objets non supprimés (calculé dans searchMin_Max)
    unsigned int nb_sommets;
    unsigned int nb_facettes;
    unsigned int nb_aretes;
    unsigned int nb_3points;
    unsigned int nb_4points;
    unsigned int nb_sommets_max;
    unsigned int numero_facette_max;
    unsigned int numero_objet_max;
    unsigned int Numero_Facette_Surlignee;
    unsigned int Numero_Objet_a_Surligner;
    bool Facette_Surlignee = false;
    bool Sommets_Surlignes = false;
    int  Numero_Sommet_a_Surligner;
    wxPoint mouse_position;

    bool Rotation_Objets = false; // Pour activer la rotation graphique des objets dans Sélections et Déplacements / Objets / Manipulations d'Objets / Rotations
    bool Changer_Echelle = false; // Pour activer la mise à l'échelle graphique des objets dans Sélections et Déplacements / Objets / Manipulations d'Objets / Mise à l'échelle
    bool Symetrie_Objets = false; // Pour activer la création par symétrisation
    double Rot_X, Rot_Y, Rot_Z;             // Angles en degrés
    double Scale_X, Scale_Y, Scale_Z;       // Facteurs d'échelles sur les 3 axes
    double Centre_X, Centre_Y, Centre_Z;    // Centre de rotation du/des objets(s)
    double x1_b1,x2_b1,y1_b1,y2_b1,z1_b1,z2_b1; // Boite 1 (originale)
    double x1_b2,x2_b2,y1_b2,y2_b2,z1_b2,z2_b2; // Boite 2 (symétrique)

    // Données pour calcul et affichage du nombre de frames par secondes (Fps)
    int timebase, frames, delta_time ;
    float fps ;
    bool  viewFps_def=true, viewFps = viewFps_def ;
    char  Message_fps[20];    // Pour stocker le message des FPS

    std::list<int> listeGroupes;
    std::list<int> listeMateriaux;
    std::list<int> listeObjets;
    std::list<int> listePoints;
    std::list<int> listeFacettes;

    int indiceAspect = 0;
    int GroupeMateriau[2] = {0,0};   // 1 pour repérer par une couleur les groupes, 2 pour repérer les matériaux, 0 sinon
    unsigned int SelectionObjet=0;

    bool show_CW_CCW = false;

#if wxCHECK_VERSION(3,0,0)
    wxGLContext * m_glRC;
#endif // wxCHECK_VERSION

    GLData m_gldata;
    GLint  m_gllist;    // GLuint
    GLint glliste_objets = 1;
    GLint glliste_lines  = 2;
    GLint glliste_points = 3;
    GLint glliste_boite  = 4;
    GLint glliste_repere = 5;
    GLint glliste_select = 6;
    GLint glliste_segment= 7;   // Pas utilisé. Pas sûr qu'une liste soit utile dans ce cas

    bool   materials = false;
    bool   groupes   = false;
    bool   smooth    = false;

    bool segment_surligne   = false;
    bool click_sur_segment  = false;
    bool Elements_Masques   = false; // à l'initialisation, rien n'est masqué
    bool Elements_Supprimes = false; // ni supprimé. Ces 2 indicateurs sont globaux. Pour les détails, voir dans chaque objet et chaque facette

    ModificationPanel       *MPanel   = nullptr;
    SelectionPanel          *MSelect  = nullptr;
    ManipulationsPanel      *MManip   = nullptr;
    TranslationPanel        *MTrans   = nullptr;
    RotationPanel           *MRotation= nullptr;
    ScalePanel              *MScale   = nullptr;
    Prefs_Dialog            *MPrefs   = nullptr;
    PositionObs_AzimutSite  *MPosObs  = nullptr;
    PositionSource          *MPosLight= nullptr;
    SelectionManuelleObjets *MSelObj  = nullptr; // Non utilisé ?
    CentreRotation          *MPosCRot = nullptr;
    ZoomSpecifique          *MZoomSpec= nullptr;
//    ReperageFacette         *MRepFacet=nullptr;
//    ReperageGroupe          *MRepGrp=nullptr; // Pas utile !
//    ReperageMateriau        *MRepMat=nullptr; // idem
    bool OK_ToSave = false; // Mis à true lors de la lecture d'un fichier et testé dans SaveTo (pas de save si OK_ToSave = false !!!)
    bool verbose= false;    // Pour activer à l'écran certaines sorties intermédiaires (switch via la lettre v ou V au clavier) : init via OvniMain.h
    void SaveTo (wxString);
    void SaveBDD(wxString);
    void SaveOBJ(wxString);
    void SaveOFF(wxString);
    void SaveG3D(wxString);

    bool show_light = false, show_box    = true, show_axes = true, show_plein = true;
    bool show_lines = false, show_points = false;
    bool show_star  = false;
    bool test_rectangle_selection = false;
    int  TypeSelection = 0;
    std::vector<float> point_star;
    wxSlider* Slider_x = nullptr;
    wxSlider* Slider_y = nullptr;
    wxSlider* Slider_z = nullptr;
    float alpha, beta, gamma;

    wxFileInputStream input(wxString file);

    BddInter(wxWindow *parent, wxWindowID id = wxID_ANY,
             const int* AttribList = NULL,
             const wxPoint& pos    = wxDefaultPosition,
             const wxSize& size    = wxDefaultSize,
             long style            = 0,
             bool main_verbose     = false,
             const wxString& name  = wxT("TestGLCanvas"));
    wxString wxWorkDir, wxOvniPath;     // Doit-on garder les 2 ? à l'usage semble être la même chose ! Pas sûr, notamment si OvniPath est protégé en écriture !
                                        // Peut-être créer aussi un wxBddPath pour conserver le dernier path d'accès à une bdd !
    wxString wxNomsFichiers;            // Pour assembler les noms des fichiers (sans Path). Utile en cas de fusion de Bdds.
    wxString Fichier_Autosvg = _T("Autosave.bdd");  // Nom du fichier de sauvegarde automatique utilisé via le wxTimer
    bool exists_Autosave = false;                   // Sera mis à true par la première sauvegarde automatique

    Lib3dsFile *f3ds=0;                             // Spécifique aux fichier de type 3ds
    int o_3ds;                                                  // A PLACER AILLEURS !!!
    int nb_mat_3ds = 0;

    char *Lire_chaine(char *);
    #define nb_mat_max 4000
    char *tab_mat[nb_mat_max];
    char *mtllib_nom;
    bool mtllib_OK = false;
    bool ini_file_modified = false;     // Indicateur de modification du fichier Ovni.ini
    bool pal_file_modified = false;     // Indicateur de modification du fichier de palette .pal

    wxString get_file();                // Fichier courant (égal à first_file pour le premier fichier ouvert ou si c'est le seul !)
    wxString get_firstFile();           // Premier fichier lu en cas de fusion
    void colorface(int objet,int face,bool OnOff);
    void set_file(wxString file);
    void set_firstFile(wxString file);
    void create_bdd();
    void Resize();
    void ResetData();
    void Ouvrir_ini_file();
    void Stocker_ini_file();
    void ResetProjectionMode();
    void MakeAngle();
    void fghCircleTable_local(double **,double **,const int );
    void glutSolidSphere_local(GLdouble , GLint , GLint );
    void AfficherSource();
    void UNDO_ONE();
    void DisplayMessage(wxString , bool );
    void Inverse_Selected_Normales();
    void Inverser_Parcours_Selected();
    void Inverser_Parcours_Facettes_Objet(unsigned int , bool);
    void Inverser_Toutes_les_Normales();
    void Flat_Selected_Facettes();
    void NotFlat_Selected_Facettes();
    void Inverser_les_Normales_Objet(unsigned int);
    void InverseX();
    void InverseY();
    void InverseZ();
    void XtoY();
    void XtoZ();
    void YtoZ();
    void XtoYtoZ();
    void searchMin_Max();
    void Simplification_BDD();
    void Calcul_All_Normales();
    void Calcul_Normale_Barycentre(int ,int );
    void CalculMatriceRot();
    void Reset_Sliders();
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyLeftRight(wxKeyEvent& event, int signe);
    void OnKeyUpDown   (wxKeyEvent& event, int signe);
    void SetPosObs(bool );
    void Tracer_normale(const std::vector <float> & , const std::vector <float> &, int );
    void coloriserFacette(unsigned int, unsigned int, bool, GLfloat couleur[3]);
    void draw_rectangle_selection();
    void Selection_rectangle(GLint, GLint);

    void GenereTableauPointsFacettes(Object *);
    void GenereTableauAretes(Object *);
    void GenereListeGroupesMateriaux(unsigned int);
    bool Calcul_Normale_Seuillee(int, int, int, std::vector<float> &, std::vector<float> &);
//    bool Calcul_Normale_Seuillee(int, int, int, std::vector<float> &, std::vector<float> &, Vector3D & );
//    bool Calcul_Normale_Seuillee(int, int, int, Vector3D & );

    void makeobjet();
    void makesommet();
    void make1sommet();
    void makeface();
    void make1face();
    void makenormale();
    void make1normale();
    void makeaspect_face();
    void make1aspect_face();
    void makeluminance();
    void make1luminance();
    void makevecteur();
    void make1vecteur();

    void drawOpenGL();

    int  convert_rotx_LSI();
    int  convert_rotz_LAZ();

    bool compare_normales(int, int, int);
    bool points_egaux(const std::vector<float> &, const std::vector<float> &, float);
    void simplification_doublons_points(unsigned int);
    void simplification_facettes(unsigned int);
    void TraiterMatricePosition(unsigned int);
    void genereAttributsFacettes(int, int, int, int );
    void genereNormalesFacettes (int, int);
    void genereLuminances(int, int ) ;

// Fonctions communes Sphere - Ellipsoide
    void genereFacettesSphere(int, int, bool);
    void genereSommetsSphere (int, int, float *, float, float, float, float);
    void genereNormalesSommetsSphere(int, int, float, float, float);
    void GenereNormale_1_Sommet(Object *, unsigned int , unsigned int );
    void GenereNormalesAuxSommets(unsigned int , int );

    void buildAllLines();
    void buildBoite();
    void buildRepereOXYZ();

    void TracerBoite(double, double, double, double, double, double);

//    void Forcer_OnPaint(wxPaintEvent& event);


protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseWheelMoved(wxMouseEvent& event);
    void OnMouseZoom(wxMouseEvent& event, int signe, bool Touche_Maj);

private :
    void ShowSelectedFace();
//clear bdd
    void clearall();
//read BDD
    void LoadBDD();
    void Load3DS();
    int  compter_nodes (Lib3dsNode *);//, Lib3dsFile *);
    int  decoder_node  (Lib3dsNode *);//, Lib3dsFile *);
    int  mesh_count_smoothing(Lib3dsMesh *);
    void Affiche_Matrice(float Mat4x4[4][4]);
    void LoadG3D();
    void LectureXML_G3d (FILE *);
    void LoadOBJ();
    void Optimiser_Obj_Sommets (Object *, int, bool &);
    void Optimiser_Obj_Vecteurs(Object *, int);
    void LoadPLY();
    void LoadOFF();
    void LoadM3D();

    void makeposition();
//    void make1position();

    //opengl selection
//    void selectMode(int selection1);
    void processHits(GLint , bool);//, GLuint *);
    GLuint* selectBuffer;
    void testPicking(int, int, int, bool) ; //, int todoagain);
    void stopPicking();
    bool ifexist_facette(int, int) ;
    bool ifexist_sommet (int, int) ;
    void souderPoints(int, int);
    void diviserArete(int, int, int);
    bool letscheckthemouse(int , int); //, GLuint * );

    //opengl affichage
    void showAllPoints();
    void showAllLines();
    void showSegment();
    void showPoint();
    void buildAllPoints();
    void buildAllFacettesSelected();

    double Norme3(float x, float y, float z);
    void GenereEtoile();
    void resetMin_Max();
    void setMin_Max(float x, float y, float z);
    void InitGL();

    void CalculAngles(float *, float &, float &, float &);
    void Clamp(float &, const float, const float);
    void output_Glut_Msg(GLfloat , GLfloat , char *);
    float produit_scalaire(const std::vector<float> &, const std::vector<float> &);

    OvniFrame *MAIN_b;
    DECLARE_EVENT_TABLE()

};


#endif // INTERFACE_H_INCLUDED
