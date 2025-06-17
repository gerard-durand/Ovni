#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

// Entête général des fichiers Interface*.cpp
// ******************************************

// Les includes pour wxWidgets
#include <wx/app.h>
#include <wx/dcclient.h>
#include <wx/defs.h>
#include <wx/dialog.h>
#include <wx/frame.h>
#include <wx/filename.h>
#include <wx/glcanvas.h>
#include <wx/progdlg.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/wx.h>

#include <GL/freeglut.h>
//#include <GL/glext.h>     // Pour tests antialising des polygones (définition de GL_MULTISAMPLE entre autres) sinon à supprimer car inclus dans freeglut.h
//#include <GL/gl.h>        // Pas utile car fait dans glut.h (et/ou freeglut.h)
//#include <GL/glu.h>       // Idem

// Les includes locaux
#include "trackball.h"
#include "GLCanvas.h"

#include "dxfrenderer.h"

#include "vector3d.h"
#include "lib3ds/lib3ds.h"
//#include "lib3ds/lib3ds_impl.h"
#include "libexpat/expat.h"
#include "charset.h"

// Les includes C++ (ou C)
#include <algorithm>
#include <ctime>
#include <cstdio>      /* printf */
#include <cmath>       /* round, floor, ceil, trunc, M_PI, ... */
#include <cstdlib>
#include <filesystem>
#include <float.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include <omp.h>                        // Header de OpenMP pour la parallélisation des boucles sur plusieurs processeurs/threads

// Define de constantes
#define BUFSIZE 4096
#define gris_def 0.5f                   // 0.35f version jdias
#define groupe_def      -123
#define codmatface_def  -234

#define maxGroupes      32              // a priori, devrait être égal (ou inférieur) à nb_couleurs
#define nb_couleurs     32

#ifndef GL_MULTISAMPLE                  // pour éviter le include de glew.h et GL_MULTISAMPLE_ARB, sinon GL_MULTISAMPLE existe dans gl.h mais n'est pas activé sous Windows !
    #define GL_MULTISAMPLE      0x809D  // il faudrait glext.h et aussi peut-être glext.dll, mais en 64 bits !
#endif // GL_MULTISAMPLE
#ifndef GL_SAMPLE_BUFFERS
    #define GL_SAMPLE_BUFFERS   0x80A8
#endif // GL_SAMPLE_BUFFERS
#ifndef GL_SAMPLES
    #define GL_SAMPLES          0x80A9
#endif // GL_SAMPLES

// Classes des objets, facettes, points,... d'Ovni
class BddInter;
//class OvniFrame;                      // Déplacé dans OvniMain.h : plus logique !
class Object;
class Face;
class Sommet;
class Luminance;
class Vecteur;
class Points;
class Aretes;

// Déclaration des classes des boîtes de dialogues
class Aide_html;
class CentreRotation;
class ChangerEchelleBdd;
class ChoixAffichageObjets;
class Cone;
class CouleursGroupes;
class Cube;
class Cylindre;
class DeplacerBdd;
class Ellipsoide;
class Facette;
class Icosaedre;
class ManipulationsPanel;
class ModificationPanel;
class PositionObs_AzimutSite;
class PositionSource;
class Prefs_Dialog;
class PropertiesPanel;
class ReperageFacette;
class ReperageGroupe;
class ReperageMateriau;
class ReperageObjet;
class ReperagePoint;
class RotationPanel;
class ScalePanel;
class SelectionPanel;
class SelectionManuelleFacettes;
class SelectionManuelleObjets;
class Sphere;
class Tore;
class TranslationPanel;
class ZoomSpecifique;

#include "OvniMain.h"       // Ne pas déclarer trop tôt !

//using namespace std;      // Si absent, vector ne semble pas toujours reconnu ! il faut std::vector dans ce cas partout ???? mais aussi sur string, list, ...

// Dans la version Tcl, mais ne semble pas bien adapté ici !
char *iso_To_ibm (char *) ;
char *ibm_To_iso (char *) ;
// mieux ...
char *utf8_To_ibm (char *) ;

const float to_Deg = 180.0f/M_PI;
const float to_Rad = 1.0f/to_Deg;
const float pas_deg= 5.0f;
const float pas_rad= pas_deg/to_Deg;

static char Message[1024];

unsigned int codegroupe,codemateriau;
int message_multi_sample_actif = 0;

// Les classes, définitions, méthodes,...
//***************************************

class Object {
    wxCharBuffer name;
    wxString wxname;
//    int value;                // Numéro de l'objet. Remplacé par Numero ci-dessous
    int Numero;                 // Numéro de l'objet

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
    std::list<Aretes>    Areteslist;        // Ici, traitements plus rapides que si <vector>

// Méthodes, fonctions de la classe Object

    Object() {
    }
    Object(wxString& name, wxString& value) {
        initObject(name);
//        this->value = wxAtoi(value);
        this->Numero = wxAtoi(value);
    }
    Object(wxString& name, int& value) {
        initObject(name);
//        this->value = value;
        this->Numero = value;
    }
//    int GetValue() {
    int GetNumero() {               // + logique que GetValue
//        return this->value;
        return this->Numero;
    }
    void SetNumero(int value) {    // + logique que SetValue
//        this->value = value;
        this->Numero = value;
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

private:

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
    Face(std::vector<wxString> wxStringlist) {
        initFace();
        Numero       = wxAtoi(wxStringlist[0]);
        Nb_Sommets_F = wxAtoi(wxStringlist[1]);
        for (int i=0; i<Nb_Sommets_F; i++) {
            F_sommets.push_back(wxAtoi(wxStringlist[i+2]));
        }
    }
    Face(int number, std::vector<int> NumerosSommets) {
        initFace();
        Numero       = number;
        Nb_Sommets_F = NumerosSommets.size();
        for (int i=0; i<Nb_Sommets_F; i++) {
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

private:
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
};

class Sommet {
public:
    int  Numero;                // Numéro du point (en fait, c'est indice du sommet +1 en général). Initialisé, mis à jour mais au final non utilisé ! A supprimer ?
//    int  NNumber;               // ?? ne sert que dans Simplification_BDD_old ... A supprimer ???? Quelle est sa fonction ???
    std::vector<float> point;   // Coordonnées x, y et z du point
    int  toshow;
    bool show;
    bool selected;
//    GLfloat color[3];

// Méthodes, fonctions de la classe Sommet (traitements d'un sommet)

    Sommet() {
//        NNumber=-1;
    }
    Sommet(std::vector<wxString>wxStringlist) {
        double a;
        initSommet();
        Numero = wxAtoi(wxStringlist[0]);
        for (int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Sommet(int& number, float xyz[3]) {
        initSommet();
        Numero = number;
        for (int i=0; i<3; i++) point.push_back(xyz[i]);
     }
    Sommet(int& number, std::vector<float> xyz) {
        initSommet();
        Numero = number;
        for (int i=0; i<3; i++) point.push_back(xyz[i]);
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

private:
    void initSommet() {
        show     = true;
//        NNumber  = -1;
        toshow   = 0;
        selected = false;
//        color = bleu; // Pas encore défini !!!
//        color[0]=0. ; color[1]=0. ; color[2]=1. ;
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
    Normale(std::vector<wxString>wxStringlist) {
        double  a;
        initNormale();
        Numero = wxAtoi(wxStringlist[0]);
        for (int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Normale(int& number, float xyz[3]) {
        initNormale();
        Numero = number;
        for (int i=0; i<3; i++) point.push_back(xyz[i]);
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

private:
    void initNormale() {
        show  = true;
        toshow= 1;
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
        show = true;
    }
    Aspect_face(std::vector<wxString>wxStringlist) {
        show = true;
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
//    int  toshow;              // Ne sert plus ?
    bool show;

// Méthodes, fonctions de la classe Luminance (traitements d'une Luminance <=> facette)

    Luminance() {
    }
    Luminance(std::vector<wxString>wxStringlist) {
        show         = true;
        Numero       = wxAtoi(wxStringlist[0]);
        Nb_Sommets_L = wxAtoi(wxStringlist[1]);
        for (int i(0); i<Nb_Sommets_L; i++) {
            L_sommets.push_back(wxAtoi(wxStringlist[i+2]));
        }
    }
    Luminance(int number, std::vector<int> NumerosSommets) {
        show         = true;
        Numero       = number;
        Nb_Sommets_L = NumerosSommets.size();
        for (int i=0; i<Nb_Sommets_L; i++) {
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
    Vecteur(std::vector<wxString>wxStringlist) {
        double a;
        initVecteur();
        Numero = wxAtoi(wxStringlist[0]);
        for (int i=0; i<3; i++) {
            wxStringlist[i+1].ToDouble(&a);
            point.push_back(a);
        }
    }
    Vecteur(int& number, float xyz[3]) {
        initVecteur();
        Numero = number;
        for (int i=0; i<3; i++) point.push_back(xyz[i]);
    }
    Vecteur(int& number, std::vector<float> xyz) {
        initVecteur();
        Numero = number;
        for (int i=0; i<3; i++) point.push_back(xyz[i]);
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

private:
    void initVecteur() {
//        show    = true;
//        NNumber = -1;
        toshow  = 0;
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
        for (i=0; i<ListeSelect.size(); i++) {
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
    void erase_one(int objet,int face_sommet) {
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
    wxGauge *m_gauge = nullptr;
    wxTimer *m_timer = nullptr;
    wxGenericProgressDialog *progress_dialog = nullptr;     // wxGenericProgressDialog plutôt que wxProgressDialog car mieux en Darkmode
    clock_t time_deb_dialog;
    const int display_range = 200;                          // range pour l'affichage de la barre de progression. 200 (300 ou 400) va un peu plus loin que 100 (valeur par défaut)
    int display_test  = display_range-1;                    // Valeur de test pour éviter un dernier Update() trop tôt
    bool progress_dialog_en_cours;

    Lib3dsIo *io;

//    Sommetmemory *Smemory;
//    std::vector<std::vector<StoF> > StofM;

    unsigned int Numero_base;
    double diagonale_save ;

// Tests de thème
    bool theme_b = false;   // Théme dark/clair expérimental pour wxWidgets < version 3.3
    int  darkmode= -1;      // Valeur enregistrée dans Ovni.ini (plus ou moins <=> theme_b pour wxWidgets version >= 3.3)

    const wxColour Noir     = wxColour(*wxBLACK);// = wxColour(0,0,0);//    (*wxBLACK);
    const wxColour Blanc    = wxColour(*wxWHITE);
    const wxColour New_Back = wxColour(*wxBLACK);
    const wxColour New_Gris = wxColour(70,70,70);
    const wxColour New_Forg = wxColour(*wxCYAN); ; //(*wxWHITE);

    std::vector<DXFLayer>  m_layers;
    std::vector<DXFObjet>  m_objets;
    std::vector<std::unique_ptr<DXFEntity>> m_entities;

    std::vector<wxString>     wxStringlist;
    std::ifstream fichierBdd;

    std::string ligne;

    struct stat64 s_statfichier;
    unsigned long long fichierBdd_length;
    bool timer_bis = false;
    int margin = 2;

    bool finishdraw = false;    // Sert dans OnMouse à tout court-circuiter si rien d'affiché à l'écran. Pourrait être supprimé car pas de souci si toujours à true !
    float matquat[4][4];

    double FoV_auto;
    float  longueur_normales;
    GLint  xd_sel, yd_sel, xa_sel, ya_sel;      // Coordonnées écran pour le tracé d'un rectangle de sélection (d=début - a=arrivée)
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

    const GLfloat width_pixel = 1.;       // Largeur de zone pour sélectionner 1 pixel (sélection de facettes ou d'objets)
    const GLfloat width_point = 5.;       // Largeur de zone pour sélectionner 1 point
    const GLfloat width_ligne = 4.;       // Largeur de zone pour sélectionner 1 ligne
    const GLint   offset_pointeur = 4;    // Pour rattraper un petit décalage en Y entre le pointeur de souris et un sommet pointé (devrait être nul !!!).
//    DXFRenderer m_renderer;

// MatAmbient_avionG à partir de MatAmbient_avionG_def ou encore dans OvniMain pour lire/écrire la palette

/*! Paramètres du matériau utilisé pour la colorisation de l'avion par groupes et/ou matériaux */

    const GLfloat MatAmbient_avionG_def[nb_couleurs][4] =  {{0.5f, 0.5f, 0.5f, 1.0f} ,  // Groupe 0
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

    const GLfloat MatDiffuse_avionG_def[nb_couleurs][4] ={  {0.5f, 0.5f, 0.5f, 1.0f} ,	// Groupe 0
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

//    GLfloat light0_dir[3]= {0.0,0.0,0.0};
    const GLfloat Light0Ambient[4]      = {0.2f, 0.2f, 0.2f, 1.0f};
    const GLfloat Light0Diffuse[4]      = {0.7f, 0.7f, 0.7f, 1.0f};
    const GLfloat Light0Specular[4]     = {0.3f, 0.3f, 0.3f, 1.0f};

/*! Paramètres du matériau utilisé pour l'avion général (gris)*/
    const GLfloat MatAmbient_avion[4]   = {0.5f, 0.5f, 0.5f, 0.8f};
    const GLfloat MatDiffuse_avion[4]   = {0.9f, 0.9f, 0.9f, 0.8f};
    const GLfloat MatSpecular_avion[4]  = {0.1f, 0.1f, 0.1f, 1.0f};
    const GLfloat MatShininess_avion[1] = {5.0f};
/*! Variante */
    const GLfloat MatSpecular_avionG[4] = {1.0f, 1.0f, 1.0f, 1.0f};   // Réflexion spéculaire plus forte
    const GLfloat MatShininess_avionG[1]= {50.0f};                    // Idem brillance

/*! Paramètres du matériau utilisé pour le repérage d'un groupe de facettes (bleu)*/
    const GLfloat MatAmbient_avionB[4]  = {0.0f, 0.0f, 0.5f, 1.0f};
    const GLfloat MatDiffuse_avionB[4]  = {0.0f, 0.0f, 0.9f, 1.0f};
    const GLfloat MatSpecular_avionB[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    const GLfloat MatShininess_avionB[1]= {50.0f};

/*! Paramètres du matériau utilisé pour le repérage en Bleu-grisé */
    const GLfloat MatAmbient_avionBG[4] = {0.4f, 0.4f, 0.9f, 1.0f};
    const GLfloat MatDiffuse_avionBG[4] = {0.4f, 0.4f, 0.9f, 1.0f};

/*! Paramètres du matériau utilisé pour l'avion Rouge_grisé */
    const GLfloat MatAmbient_avionRG[4] = {1.0f, 0.4f, 0.4f, 1.0f};
    const GLfloat MatDiffuse_avionRG[4] = {1.0f, 0.4f, 0.4f, 1.0f};

/*! Paramètres du matériau utilisé pour le repérage d'une facette particulière (jaune)*/
    const GLfloat MatAmbient_avionJ[4]  = {0.5f, 0.5f, 0.0f, 1.0f};
    const GLfloat MatDiffuse_avionJ[4]  = {0.9f, 0.9f, 0.0f, 1.0f};
    const GLfloat MatSpecular_avionJ[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    const GLfloat MatShininess_avionJ[1]= {50.0f};

/*! Paramètres du matériau utilisé pour l'avion rouge (objet)*/
    const GLfloat MatAmbient_avionR[4]  = {0.5f, 0.0f, 0.0f, 1.0f};
    const GLfloat MatDiffuse_avionR[4]  = {0.9f, 0.0f, 0.0f, 1.0f};
    const GLfloat MatSpecular_avionR[4] = {0.1f, 0.1f, 0.1f, 1.0f};
    const GLfloat MatShininess_avionR[1]= {50.0f};

/////////////////////////////////////

    const GLfloat rouge[3] = {1.0f, 0.0f, 0.0f};
    const GLfloat vert[3]  = {0.0f, 1.0f, 0.0f};
    const GLfloat vert_f[3]= {0.0f, 0.5f, 0.0f};              // Vert foncé
    const GLfloat vert_c[3]= {0.5f, 1.0f, 0.5f};              // Vert clair
    const GLfloat bleu[3]  = {0.0f, 0.0f, 1.0f};
    const GLfloat blanc[3] = {1.0f, 1.0f, 1.0f};
    const GLfloat noir[3]  = {0.0f, 0.0f, 0.0f};
    const GLfloat jaune[3] = {1.0f, 1.0f, 0.0f};
    const GLfloat violet[3]= {1.0f, 0.0f, 1.0f};
    const GLfloat cyan[3]  = {0.0f, 1.0f, 1.0f};
    const GLfloat cyan_f[3]= {0.0f, 0.5f, 0.5f};              // Cyan foncé
    const GLfloat cyan_c[3]= {0.5f, 1.0f, 1.0f};              // Cyan clair
    const GLfloat gris[3]  = {gris_def, gris_def, gris_def};  // Gris par défaut

    int Groupe_num[maxGroupes] ;

// DXFRenderer
// fonctions/méthodes provenant de DXFRenderer.h
    bool ParseHeaderDXF(wxInputStream& );
    bool ParseTablesDXF(wxInputStream& );
    bool ParseEntitiesDXF(wxInputStream& );
    int  GetLayerColourDXF(const wxString& ) const;
    void NormalizeEntitiesDXF();

    void ClearDXF();
    bool LoadDXF(wxInputStream& );
    void RenderDXF();
    void GetLines(wxTextInputStream& , wxString& , wxString& );
    double ToDoubleDXF(const wxString& );
    wxColour ACIColourToRGB(int);
    int nb3DFACE_DXF, nbLINE_DXF, nbPoints_DXF;

    bool AfficherNormaleFacette   = false;
    bool AfficherNormalesSommets  = false;
    bool centrageRotAuto          = true;
    bool pointsSelected           = false;

    bool New_aspect_face          = false;
    bool m_loaded                 = false;

// Valeurs par défaut
    const float len_axe_def       = 0.25;
    const float len_normales_def  = 0.05;
    const float ray_sun_def       = 0.03;
    const float tolerance_def     = 0.001;            // En % de la diagonale de la boîte englobante
    const float angle_Gouraud_def = 35.0f;
    const float fmult_Gouraud_def =  1.6f;
    const bool  test_seuil_Gouraud_def = true;
    const int   svg_time_def      = 5;                // en minutes
    const int   icon_index_def    = 0;                // Par défaut, icônes en 16*16 pixels
    const int   mode_Trackball_def= 1;                // mode par défaut : 1 = Trackball, 0 = Rotation directe

    const bool  traiter_doublons_aretes_def  = true;  // Si true, traite les doublons d'arêtes à la génération. Mais ça peut être long si ce n'est pas nécessaire !
    const bool  forcer_simplification_doublons_aretes_def = false;
    const bool  antialiasing_soft_def        = false; // Antialiasing par défaut non fait par OpenGL pour éviter les soucis avec cartes graphiques ne le supportant pas
    const bool  afficher_sliders_def         = true;  // Au lancement d'Ovni, affichage ou pas des Sliders/Curseurs en bas de la fenêtre d'Ovni
    const bool  Forcer_1_Seul_Objet_def      = false; // Pour forcer la lecture des fichiers .obj dans 1 seul Objet 3D
    const bool  lect_obj_opt_def             = false; // Pour activer une lecture optimisée des fichiers .obj (suppression des sommets/vecteurs inutiles sur objets multiples)
    const bool  test_decalage3ds_def         = true;  // Tenir compte d'un éventuel décalage d'objet 3ds (prise en compte du pivot). Ne marche pas toujours bien ...
    const bool  CalculNormalesLectureBdd_def = false; // Pour forcer le calcul (recalcul) des normales dès la lecture du fichier
    const bool  Enr_Normales_Seuillees_def   = true;  // Force l'enregistrement des normales seuillées dans le fichier bdd
    const bool  CreerBackup_def              = false; // Pour renommer l'original d'un fichier de bdd en .bak
    const bool  SupprBackup_def              = false; // Pour supprimer le .bak en sortie de programme
    const int   methode_Triangulation_def    = 1;     // mode par défaut 0, 1 ou 2
    const bool  Raz_Selection_F_def          = false; // Indicateur de Reset de sélection de facettes après une inversion de normales ('S' automatique après un 'I' ou un 'P')
    const bool  NotFlat_def                  = false; // Lors  d'un recalcul des normales, ne change pas les facettes déclarées plane. Si true, lissage de Gouraud actif sur la/les facette(s)
    const bool  msg_warning_def              = true;  // Affiche un message sous forme de dialogue si la carte graphique ne supporte pas WX_GL_SAMPLE_BUFFERS (antialiasing par multisample)
    const int   nb_threads_def               = 0;     // Nombre max de threads pour OpenMP. 0 => non limité => sera pris égal au nombre de processeurs/max_threads via omp_set_num_threads
    const bool  viewFps_def                  = true;  // Affichage du nombre de Frames par secondes
    // Les valeurs suivantes devraient plutôt être dans OvniMain.h
    const int   frame_size_x_def             = 816;   // Taille par défaut de la fenêtre principale OvniFrame avec une taille d'icônes de 16
    const int   frame_size_y_def             = 673;   // et une fenêtre OpenGL de 800*400 quand les curseurs/sliders sont affichés

    const float    fmult_diagonale_def       = 10.0f; // Pour calcul de la distance de visée comme multiple de la diagonale de boîte englobante
    const unsigned int nb_sommets_test       = 500000;// Si le nombre total de sommets dépasse cette valeur, on ne génère pas d'emblée la liste OpenGL des sommets
    const unsigned int nb_facettes_test      = 500000;
    const unsigned int nb_aretes_test        = 800000;// Si le nombre total d'arêtes dépasse cette valeur, on ne génère pas d'emblée la liste OpenGL des lignes
    const unsigned int nb_aretes_test_d      = 400000;// Si le nombre d'arêtes à traiter dépasse cette valeur, on court-circuite le traitement des doublons car trop long... faute de mieux !

    const int icon_sizes[4] = {16, 24, 32, 48};       // 4 tailles d'icônes : 16, 24, 32 ou 48 pixels

    const GLfloat Light0Position_def[4] = {4.0f, 4.0f, 2.0f, 0.0f}; // Serait à paramétrer / diagonale surtout si petits objets
                                                                    // OK avec modif dans AfficherSource : Ce sont des coordonnées absolues
                                                                    // sur M2000 et pour les autres Bdd donnera la même position sur l'écran

// Valeurs de travail

    GLenum style= GL_POLYGON ; //GL_LINE_LOOP;     /*!reglage mode par defaut*/

    bool  bdd_modifiee = false;                    // Pour indiquer que la base de données a été modifiée => proposer de l'enregistrer si ça n'a pas été fait

    float xyz[3];                                  // Coordonnées de points

    float len_axe;
    float len_normales;
    float ray_sun;
    float tolerance;
    bool  test_seuil_Gouraud       = test_seuil_Gouraud_def;
    float angle_Gouraud            = angle_Gouraud_def;
    float fmult_Gouraud            = fmult_Gouraud_def;
    float seuil_Gouraud;
    float angle_Gouraud2           = angle_Gouraud*fmult_Gouraud;
    float seuil_Gouraud2;
    int   svg_time                 = svg_time_def;
    int   icon_index               = icon_index_def;
//    int   icon_size                = icon_sizes[icon_index];    // Trop tôt ici => déclaré plus loin, aussi en private
    bool  traiter_doublons_aretes  = traiter_doublons_aretes_def;
    bool  forcer_simplification_doublons_aretes = forcer_simplification_doublons_aretes_def;
    bool  antialiasing_soft        = antialiasing_soft_def;
    bool  afficher_sliders         = afficher_sliders_def;
    bool  Forcer_1_Seul_Objet      = Forcer_1_Seul_Objet_def;
    bool  lect_obj_opt             = lect_obj_opt_def;
    bool  test_decalage3ds         = test_decalage3ds_def;
    bool  CalculNormalesLectureBdd = CalculNormalesLectureBdd_def;
    bool  Enr_Normales_Seuillees   = Enr_Normales_Seuillees_def;
    bool  CreerBackup              = CreerBackup_def;
    bool  SupprBackup              = SupprBackup_def;
    int   methode_Triangulation    = methode_Triangulation_def;
    bool  Raz_Selection_F          = Raz_Selection_F_def;
    bool  NotFlat                  = NotFlat_def;
    bool  msg_warning              = msg_warning_def;
    int   nb_threads               = nb_threads_def;
    int   nb_max_threads;                               // Pas de valeur par défaut
    bool  viewFps                  = viewFps_def;
    int   frame_size_x             = frame_size_x_def;  // Ces valeurs devraient plutôt être dans OvniMain.h
    int   frame_size_y             = frame_size_y_def;

    int   indiceAspect             = 0;
    bool  segment_surligne         = false;
    bool  click_sur_segment        = false;
    bool  test_rectangle_selection = false;

    bool  reset_zoom               = true;

    bool  verbose                  = false; // Pour activer à l'écran certaines sorties intermédiaires (switch via la lettre v ou V au clavier) : initialisé via OvniMain.h

    wxPoint mouse_position;

    // Données pour calcul et affichage du nombre de frames par secondes (Fps)
    int   timebase, frames, delta_time ;
    float fps ;
    char  Message_fps[20];    // Pour stocker le message des FPS

#if wxCHECK_VERSION(3,0,0)
    wxGLContext *m_glRC;
#endif // wxCHECK_VERSION

    typedef enum {Both, Avant, Arriere} TYPESELECTION;
//    int  TypeSelection = 0; // Both
    TYPESELECTION TypeSelection = Both;

    float alpha, beta, gamma;
//    wxFileInputStream input(wxString file);       // Ne sert plus ?

    wxString Dialog_Titre, Dialog_Comment;

    clock_t  Dialog_Delay;                          // Delai avant initialisation et apparition de wxProgressDialog (=> pas d'affichage si c'est plus court)

    int  type_fichier = 0;
    bool type_dxf     = false;

// Variables spécifiques aux fichier de type .3ds
    Lib3dsFile *f3ds=0;
    int o_3ds;                          // A PLACER AILLEURS !!!
    int nb_mat_3ds = 0;                 // ATTENTION : interaction posssible avec nb_mat car on utilise tab_mat ddans LoadOBJ et dans Load3DS (si plusieurs fichiers !)

    char *Lire_chaine(char *);
    #define nb_mat_max 4000
    char *tab_mat[nb_mat_max];

    unsigned int indiceObjet_courant;

// Variables spécifiques aux fichier de type .obj
    char *mtllib_nom;
    bool mtllib_OK = false;

// Rotations, mises à l'échelle, centre pour les rotations
    double Rot_X, Rot_Y, Rot_Z;             // Angles en degrés
    double Scale_X, Scale_Y, Scale_Z;       // Facteurs d'échelles sur les 3 axes
    double Centre_X, Centre_Y, Centre_Z;    // Centre de rotation du/des objets(s)
    bool Rotation_Objets    = false;        // Pour activer la rotation graphique des objets dans Sélections et Déplacements / Objets / Manipulations d'Objets / Rotations
    bool Changer_Echelle    = false;        // Pour activer la mise à l'échelle graphique des objets dans Sélections et Déplacements / Objets / Manipulations d'Objets / Mise à l'échelle
    bool Symetrie_Objets    = false;        // Pour activer la création par symétrisation

    bool Elements_Masques   = false;
    bool Elements_Supprimes = false;

    bool detection_survol_point = false;    // Pour activer/désactiver la détection et colorisation des sommets survolés
    bool detection_survol_arete = false;    // Pour activer/désactiver la détection et colorisation des arêtes survoleés
    bool creation_facette       = false;    // Pour traiter différement le sélection de points en mode de création de facettes

    bool liste_sommets_OK;                  // Pour savoir si la liste OpenGL des sommets a été générée
    bool liste_aretes_OK;                   // Pour savoir si la liste OpenGL des arêtes a été générée
    bool Genere_Tableau_Aretes_OK;

    GLfloat MatAmbient_avionG[nb_couleurs][4];
    GLfloat MatDiffuse_avionG[nb_couleurs][4];
    GLfloat Light0Position[4];              // Serait à paramétrer / diagonale surtout si petits objets car 4 * 4 * 2 m peut être trop loin

    bool show_light = false, show_box    = true, show_axes = true, show_plein = true;
    bool show_lines = false, show_points = false;
    bool show_star  = false;
    std::vector<float> point_star;

    bool show_CW_CCW = false;

    float x_min, x_max, y_min, y_max, z_min, z_max; // Coordonnées min-max de la Boîte englobante

    unsigned int nb_objets;
    unsigned int nb_objets_reels;           // Pour compter seulement les objets non supprimés (calculé dans Search_Min_Max)
    unsigned int nb_sommets;
    unsigned int nb_sommets_max;
    unsigned int nb_facettes;
    unsigned int nb_aretes;
    unsigned int nb_3points;                // Comptage des facettes à 3 sommets
    unsigned int nb_4points;                // Comptage des facettes à 4 sommets et +

    unsigned int numero_objet_max;
    unsigned int numero_facette_max;
    unsigned int Numero_Objet_a_Surligner;
    unsigned int Numero_Facette_Surlignee;
    int          Numero_Sommet_a_Surligner;

    bool Facette_Surlignee = false;
    bool Sommets_Surlignes = false;

    int SelectionObjet     = 0;     // La valeur -1 peut être utilisée

    bool materials = false;
    bool groupes   = false;
    bool smooth    = false;

    int GroupeMateriau[2] = {0, 0};     // 1er indice : 1 pour repérer les groupes par une couleur, 2 pour repérer les matériaux, 0 sinon

    double x1_b1, x2_b1, y1_b1, y2_b1, z1_b1, z2_b1;    // Boite 1 (originale)
    double x1_b2, x2_b2, y1_b2, y2_b2, z1_b2, z2_b2;    // Boite 2 (symétrique)

    bool OK_ToSave = false;             // Mis à true lors de la lecture d'un fichier et testé dans SaveTo (pas de save si OK_ToSave = false !!!)

    wxString wxNomsFichiers;            // Pour assembler les noms des fichiers (sans Path). Utile en cas de fusion de Bdds.

public :

    typedef enum {points,aretes,standard} MODE;
    MODE  modeGL;

    typedef enum {selection_point,selection_arete,selection_facette,selection_objet,aucune} SELECTION;
    SELECTION mode_selection;

    Sommetmemory *Smemory;
    std::vector<Object> Objetlist;

    int N_elements;

    wxString str;
    std::vector<int>   NumerosSommets;
    std::vector<float> centre_auto;          // Coordonnées du centre de rotation en automatique (centre de la boîte englobante)
    std::vector<float> centreRot;            // Coordonnées du centre de rotation effectif

    ListeSelected ToSelect;

    std::list<int> listeGroupes;
    std::list<int> listeMateriaux;
    std::list<int> listeObjets;
    std::list<int> listePoints;
    std::list<int> listeFacettes;

    GLData m_gldata;
    GLint  m_gllist;                // GLuint (GLint pour se réserver la possibilité d'utiliser m_gllist = -1; pour un Refresh() sans aucune regénération de liste)
    enum gllistes {
         glliste_lines = 1,         // Liste pour les lignes/arêtes (ici forcer le numéro 1, sinon enum commence à 0)
         glliste_points ,           // Liste pour les points/sommets
         glliste_points_selected ,  // Liste pour les points/sommets sélectionnés
         glliste_boite  ,           // Liste pour la boîte englobante
         glliste_repere ,           // Liste pour le tracé du repère Oxyz
         glliste_select ,           // Liste pour des sélections de facettes
         glliste_segment,           // Pas utilisé. Pas sûr qu'une liste soit utile dans ce cas car réservé pour des tracés de segments individuels ou en petit nombre
         glliste_objets             // Numéro le plus haut pour pouvoir éventuellement gérer plusieurs listes d'objets (en particulier via OpenMP, mais OpenGL ne semble pas d'accord !)
    };

    Aide_html               *MHelp     = nullptr;
    CentreRotation          *MPosCRot  = nullptr;
    ChangerEchelleBdd       *MScale_0  = nullptr;
    ChoixAffichageObjets    *MChoice_O = nullptr;
    Cone                    *MCone     = nullptr;
    Cube                    *MCube     = nullptr;
    CouleursGroupes         *MCGroup   = nullptr;
    Cylindre                *MCylindre = nullptr;
    DeplacerBdd             *MDeplacer = nullptr;
    Ellipsoide              *MEllips   = nullptr;
    Facette                 *MFacet    = nullptr;
    Icosaedre               *MIcosa    = nullptr;
    ManipulationsPanel      *MManip    = nullptr;
    ModificationPanel       *MPanel    = nullptr;
    PositionObs_AzimutSite  *MPosObs   = nullptr;
    PositionSource          *MPosLight = nullptr;
    Prefs_Dialog            *MPrefs    = nullptr;
    PropertiesPanel         *MProps    = nullptr;
    ReperageFacette         *MRepFacet = nullptr;
    ReperageGroupe          *MRepGrp   = nullptr;
    ReperageMateriau        *MRepMat   = nullptr;
    ReperageObjet           *MRepObj   = nullptr;
    ReperagePoint           *MRepPoint = nullptr;
    RotationPanel           *MRotation = nullptr;
    ScalePanel              *MScale    = nullptr;
    SelectionPanel          *MSelect   = nullptr;
    SelectionManuelleFacettes *MSelFac = nullptr;
    SelectionManuelleObjets *MSelObj   = nullptr;
    Sphere                  *MSphere   = nullptr;
    TranslationPanel        *MTrans    = nullptr;
    ZoomSpecifique          *MZoomSpec = nullptr;

    void SaveTo (wxString, int);
    void WarningAccess(wxString);
    void SaveBDD(wxString);
    void SaveOBJ(wxString);
    void SaveOFF(wxString);
    void SaveG3D(wxString);
    void SaveSTL(wxString, bool);

    wxSlider* Slider_x = nullptr;
    wxSlider* Slider_y = nullptr;
    wxSlider* Slider_z = nullptr;

//#if wxCHECK_VERSION(3,1,0)
    BddInter(wxWindow *parent,
             const wxGLAttributes& AttribList,
             wxWindowID id         = wxID_ANY,
             const wxPoint& pos    = wxDefaultPosition,
             const wxSize& size    = wxDefaultSize,
             long style            = 0,
             bool main_verbose     = false,
             const wxString& name  = wxT("TestGLCanvas"));
//#else
    BddInter(wxWindow *parent,
             const int* AttribList = nullptr,
             wxWindowID id         = wxID_ANY,
             const wxPoint& pos    = wxDefaultPosition,
             const wxSize& size    = wxDefaultSize,
             long style            = 0,
             bool main_verbose     = false,
             const wxString& name  = wxT("TestGLCanvas"));
//#endif // wxCHECK_VERSION
    ~BddInter();

    wxString wxWorkDir, wxOvniPath;     // Doit-on garder les 2 ? à l'usage semble être la même chose ! Pas sûr, notamment si OvniPath est protégé en écriture !
                                        // Peut-être créer aussi un wxBddPath pour conserver le dernier path d'accès à une bdd !

    wxString Fichier_Autosvg = _T("Autosave.bdd");  // Nom du fichier de sauvegarde automatique utilisé via le wxTimer

    unsigned int  nb_mat = 0;           // Nombre de matériaux enregistrés dans tab_mat

    bool ini_file_modified = false;     // Indicateur de modification du fichier Ovni.ini
    bool pal_file_modified = false;     // Indicateur de modification du fichier de palette .pal

    wxString get_file();                // Fichier courant (égal à first_file pour le premier fichier ouvert ou si c'est le seul !)
    void set_file(wxString file);
    wxString get_firstFile();           // Premier fichier lu en cas de fusion

    void colorface(int objet, int face, bool OnOff);
    void set_firstFile(wxString file);
    void create_bdd();
    void Resize();
    void Stocker_ini_file();
    void ResetProjectionMode();
//    void MakeAngle();
    void UNDO_ONE();
//    void DisplayMessage(wxString , bool );    // Déplacé dans Ovni.h, en dehors de OvniFrame
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
    void Search_Min_Max();
    void Simplification_BDD();
    void Calcul_All_Normales();
    void Calcul_Normale_Barycentre(int ,int );
    void CalculMatriceRot();
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyLeftRight(wxKeyEvent& event, int signe);
    void OnKeyUpDown   (wxKeyEvent& event, int signe);
    void SetPosObs(bool );

    void Genere_Tableau_Points_Facettes(Object *);
    void Genere_Tableau_Aretes(Object *);
    void Genere_Liste_Groupes_Materiaux(Object *);
    void Genere_Liste_Groupes_Materiaux(unsigned int);
//    bool Calcul_Normale_Seuillee(int, int, int, std::vector<float> &, std::vector<float> &, Vector3D & );
//    bool Calcul_Normale_Seuillee(int, int, int, Vector3D & );

    void make_objet();
    void make_sommet();
    void make_1_sommet();
    void make_face();
    void make_1_face();
    void make_1_face(int , const std::vector<int> &);
    void make_normale();
    void make_1_normale();
    void make_aspect_face();
    void make_1_aspect_face();
    void make_luminance();
    void make_1_luminance();
    void make_1_luminance(int , const std::vector<int> &);

    void make_vecteur();
    void make_1_vecteur();

    int  Convert_Rotx_LSI();
    int  Convert_Rotz_LAZ();

    void Genere_Attributs_Facettes(int, int, int, int );
    void Genere_Attributs_Facettes(Object *, int, int, int );
    void Genere_Normales_Facettes (int, int);
    void Genere_Normales_Facettes (Object *, int);
    void Genere_Luminances(int, int ) ;
    void Genere_Luminances(Object *, int ) ;

// Fonctions communes Sphere - Ellipsoide - Icosaedre
    void Genere_Facettes_Sphere(int, int, bool);
    void Genere_Sommets_Sphere (int, int, float *, float, float, float, float);
    void Genere_Normales_Sommets_Sphere(int, int, float, float, float);
    void Genere_Normale_1_Sommet(Object *, unsigned int , unsigned int );
    void Genere_Normales_Aux_Sommets(unsigned int , int );

    void buildAllLines();
    void buildBoite();
    void buildRepereOXYZ();

    // Getter / Setter

    bool GetVerbose()      {return verbose;}

    int  GetNumeroBase()   {return Numero_base;}
    void SetNumeroBase(int value) {Numero_base = value;}

    double GetDiagonaleSave()         {return diagonale_save;}

    wxColour GetNewBackgroundColour() {return New_Back;}
    wxColour GetNewForegroundColour() {return New_Forg;}
    void SetThemeB(bool value) {theme_b = value;}
    bool GetThemeB()    {return theme_b;}
    int  GetDarkmode()  {return darkmode;}

    int GetNbCouleurs() {return nb_couleurs;};  // Pas forcément utile car nb_couleurs est initialisé par un #define

    void SetAfficherNormaleFacette (bool value) {AfficherNormaleFacette  = value;}
    void SetAfficherNormalesSommets(bool value) {AfficherNormalesSommets = value;}

    void Setxyz(float x, float y, float z) {
        xyz[0] = x; xyz[1] = y; xyz[2] = z;
    }
    void Set_numeros(std::vector<int> Numeros) {NumerosSommets = Numeros;}

    bool GetCentrageRotAuto()    {return centrageRotAuto;}
    void SetCentrageRotAuto(bool value) {centrageRotAuto = value;}

    bool GetPointsSelected() {return pointsSelected;}

    void SetFileLoaded(bool value) {m_loaded = value;}

    bool GetBddModifiee()   {return bdd_modifiee;}
    void SetBddModifiee(bool value){bdd_modifiee = value;}

    void SetXYZ(float x, float y, float z) {xyz[0] = x; xyz[1] = y; xyz[2] = z;}

    float GetLenAxeDef() {return len_axe_def;}
    float GetLenAxe()    {return len_axe;}
    void  SetLenAxe(float value){len_axe = value;}

    float GetLenNormalesDef() {return len_normales_def;}
    float GetLenNormales()    {return len_normales;}
    void  SetLenNormales(float value){len_normales = value;}

    float GetRaySunDef() {return ray_sun_def;}
    float GetRaySun()    {return ray_sun;}
    void  SetRaySun(float value){ray_sun = value;}

    float GetToleranceDef() {           // N'est pas utilisé par Reset de Prefs_Dialog, serait à faire éventuellement dans ModificationPanel
        return tolerance_def;
    }
    float GetTolerance()     {return tolerance;}
    void  SetTolerance(float value) {tolerance = value;}

    float GetAngleGouraudDef() {return angle_Gouraud_def;}
    float GetAngleGouraud()    {return angle_Gouraud;}
    void  SetAngleGouraud(float value){angle_Gouraud = value;}
    float GetFmultGouraudDef() {return fmult_Gouraud_def;}
    float GetFmultGouraud()    {return fmult_Gouraud;}
    void  SetFmultGouraud(float value){fmult_Gouraud = value;}
    bool  GetTestSeuilGouraudDef() {return test_seuil_Gouraud_def;}
    bool  GetTestSeuilGouraud()    {return test_seuil_Gouraud;}
    void  SetTestSeuilGouraud(bool value) {test_seuil_Gouraud = value;}
    void  SetSeuilGouraud(float value)  {seuil_Gouraud  = value;}
    void  SetSeuilGouraud2(float value) {seuil_Gouraud2 = value;}

    int   GetSvgTimeDef() {return svg_time_def;}
    int   GetSvgTime()    {return svg_time;}
    void  SetSvgTime(int value)  {svg_time = value;}

    int   GetIconIndexDef() {return icon_index_def;}
    int   GetIconIndex()    {return icon_index;}
    void  SetIconIndex(int indice) {icon_index = indice;}
    int   GetIconSize()     {return icon_size;}
    void  SetIconSize(int indice)  {icon_size = icon_sizes[indice];}

    bool  GetTraiterDoublonsAretesDef() {return traiter_doublons_aretes_def;}
    bool  GetTraiterDoublonsAretes()    {return traiter_doublons_aretes;}
    void  SetTraiterDoublonsAretes(bool value) {traiter_doublons_aretes = value;}

    bool  GetAntialiasingSoftDef() {return antialiasing_soft_def;}
    bool  GetAntialiasingSoft()    {return antialiasing_soft;}
    void  SetAntialiasingSoft(bool value) {antialiasing_soft = value;}

    int   GetModeTrackballDef() {return mode_Trackball_def;}

    bool  GetAfficherSlidersDef() {return afficher_sliders_def;};    // Non utilisé (non changé par Préférences / Reset)
    bool  GetAfficherSliders()    {return afficher_sliders;}
    void  SetAfficherSliders(bool value) {afficher_sliders = value;}

    bool  GetForcer_1_SeulObjetDef() {return Forcer_1_Seul_Objet_def;}
    bool  GetForcer_1_SeulObjet()    {return Forcer_1_Seul_Objet;}
    void  SetForcer_1_SeulObjet(bool value) {Forcer_1_Seul_Objet = value;}

    bool  GetLectObjOptDef() {return lect_obj_opt_def;}
    bool  GetLectObjOpt()    {return lect_obj_opt;}
    void  SetLectObjOpt(bool value) {lect_obj_opt = value;}

    bool  GetTestDecalage3dsDef() {return test_decalage3ds_def;}
    bool  GetTestDecalage3ds()    {return test_decalage3ds;}
    void  SetTestDecalage3ds(bool value) {test_decalage3ds = value;}

    bool  GetCalculNormalesLectureBddDef() {return CalculNormalesLectureBdd_def;}
    bool  GetCalculNormalesLectureBdd()    {return CalculNormalesLectureBdd;}
    void  SetCalculNormalesLectureBdd(bool value) {CalculNormalesLectureBdd = value;}

    bool  GetEnrNormalesSeuilleesDef() {return Enr_Normales_Seuillees_def;}
    bool  GetEnrNormalesSeuillees()    {return Enr_Normales_Seuillees;}
    void  SetEnrNormalesSeuillees(bool value) {Enr_Normales_Seuillees = value;}

    bool  GetCreerBackupDef() {return CreerBackup_def;}
    bool  GetCreerBackup()    {return CreerBackup;}
    void  SetCreerBackup(bool value) {CreerBackup = value;}

    bool  GetSupprBackupDef() {return SupprBackup_def;}
    bool  GetSupprBackup()    {return SupprBackup;}
    void  SetSupprBackup(bool value) {SupprBackup = value;}

    int   GetMethodeTriangulationDef() {return methode_Triangulation_def;}
    int   GetMethodeTriangulation()    {return methode_Triangulation;}
    void  SetMethodeTriangulation(int value)  {methode_Triangulation = value;}

    bool  GetRazSelectionFDef() {return Raz_Selection_F_def;};       // Non utilisé (non changé par Préférences / Reset)
    bool  GetRazSelectionF()    {return Raz_Selection_F;}
    void  SetRazSelectionF(bool value) {Raz_Selection_F = value;}

    bool  GetNotFlatDef() {return NotFlat_def;}
    bool  GetNotFlat()    {return NotFlat;}
    void  SetNotFlat(bool value) {NotFlat = value;}

    int   GetNbThreadsDef() {return nb_threads_def;}
    int   GetNbThreads()    {return nb_threads;}
    void  SetNbThreads(int value)  {nb_threads = value;}
    int   GetNbMaxThreads() {return nb_max_threads;}

    bool  GetViewFpsDef() {return viewFps_def;}
    bool  GetViewFps()    {return viewFps;}
    void  SetViewFps(bool value) {viewFps = value;}

    int   GetFrameSizeXDef() {return frame_size_x_def;};     // Ces méthodes devraient plutôt être dans OvniMain.h
    int   GetFrameSizeX()    {return frame_size_x;}
    void  SetFrameSizeX(int value)  {frame_size_x = value;}
    int   GetFrameSizeYDef() {return frame_size_y_def;}
    int   GetFrameSizeY()    {return frame_size_y;}
    void  SetFrameSizeY(int value)  {frame_size_y = value;}

    unsigned int GetIndiceObjetCourant()    {return indiceObjet_courant;}
    void  SetIndiceObjetCourant(unsigned int value) {indiceObjet_courant = value;}

    int   GetTypeFichier()   {return type_fichier;}
    void  SetTypeFichier(int value) {type_fichier = value;}
    bool  GetTypeDxf()    {return type_dxf;}
    void  SetTypeDxf(bool value) {type_dxf = value;}

    bool  GetResetZoom()    {return reset_zoom;}
    void  SetResetZoom(bool value) {reset_zoom = value;}

    float GetFmultDiagonaleDef() {return fmult_diagonale_def;}

    void    ResetLight0Position() {for (int i=0; i<4; i++) Light0Position[i] = Light0Position_def[i];}
    GLfloat GetLight0Position(int indice)          {return Light0Position[indice];}
    void    SetLight0Position(int indice, GLfloat value)  {Light0Position[indice] = value;}

// Rotations, mises à l'échelle, centre pour les rotations
    void   SetRot_X(double value)    {Rot_X = value;}
    void   SetRot_Y(double value)    {Rot_Y = value;}
    void   SetRot_Z(double value)    {Rot_Z = value;}

    void   SetScale_X(double value)  {Scale_X = value;}
    void   SetScale_Y(double value)  {Scale_Y = value;}
    void   SetScale_Z(double value)  {Scale_Z = value;}
    double GetScale_X()       {return Scale_X;}
    double GetScale_Y()       {return Scale_Y;}
    double GetScale_Z()       {return Scale_Z;}

    void   SetCentre_X(double value) {Centre_X = value;}
    void   SetCentre_Y(double value) {Centre_Y = value;}
    void   SetCentre_Z(double value) {Centre_Z = value;}
    double GetCentre_X()      {return Centre_X;}
    double GetCentre_Y()      {return Centre_Y;}
    double GetCentre_Z()      {return Centre_Z;}

    void  SetSymetrieObjets(bool value) {Symetrie_Objets = value;}
    void  SetRotationObjets(bool value) {Rotation_Objets = value;}
    void  SetChangerEchelle(bool value) {Changer_Echelle = value;}

    void  SetElementsMasques(bool value)   {Elements_Masques   = value;}
    bool  GetElementsSupprimes()    {return Elements_Supprimes;}
    void  SetElementsSupprimes(bool value) {Elements_Supprimes = value;}

    bool  GetDetectionSurvolPoint()   {return detection_survol_point;}
    void  SetDetectionSurvolPoint(bool value){detection_survol_point = value;}
    void  SetDetectionSurvolArete(bool value){detection_survol_arete = value;};  // Pas besoin de GetDetectionSurvolArete

    void  SetCreationFacette(bool value){creation_facette = value;}

    bool  GetListeSommets_OK()    {return liste_sommets_OK;}
    void  SetListeSommets_OK(bool value) {liste_sommets_OK = value;}
    bool  GetListeAretes_OK()            {return liste_aretes_OK;}
    bool  GetGenereTableauAretes_OK()    {return Genere_Tableau_Aretes_OK;}
    void  SetGenereTableauAretes_OK(bool value) {Genere_Tableau_Aretes_OK = value;}

/*! Paramètres du matériau utilisé pour la colorisation de l'avion par groupes et/ou matériaux */

    GLfloat GetMatAmbientAvionGDef(int i, int j)   {return MatAmbient_avionG_def[i][j];}
    GLfloat GetMatDiffuseAvionGDef(int i, int j)   {return MatDiffuse_avionG_def[i][j];}
    GLfloat GetMatAmbientAvionG(int i, int j)      {return MatAmbient_avionG[i][j];}
    GLfloat GetMatDiffuseAvionG(int i, int j)      {return MatDiffuse_avionG[i][j];}
    void SetMatAmbientAvionG(int i, int j, GLfloat value) {MatAmbient_avionG[i][j] = value;}
    void SetMatDiffuseAvionG(int i, int j, GLfloat value) {MatDiffuse_avionG[i][j] = value;}
    void ResetMateriauAvionG() {
        for (int j=0; j<4; j++) {
            for (int i=0; i< nb_couleurs; i++) {
                MatAmbient_avionG[i][j] = MatAmbient_avionG_def[i][j];
                MatDiffuse_avionG[i][j] = MatDiffuse_avionG_def[i][j];
            }
        }
    }

    void  SetShowLight(bool value) {show_light = value;}
    void  SetShowBox(bool value)   {show_box   = value;}
    void  SetShowAxes(bool value)  {show_axes  = value;}
    void  SetShowPlein(bool value) {show_plein = value;}
    void  SetShowLines(bool value) {show_lines = value;}
    bool  GetShowLines()    {return show_lines;}
    void  SetShowPoints(bool value){show_points = value;}
    bool  GetShowPoints()   {return show_points;}
    void  SetShowStar(bool value)  {show_star = value;}
    void  SetPointStar(std::vector<float> vecteur) {point_star = vecteur;}

    bool  GetShow_CW_CCW()    {return show_CW_CCW;}
    void  SetShow_CW_CCW(bool value) {show_CW_CCW = value;}

// Boîte englobante
    float Get_X_min_box() {return x_min;}
    float Get_X_max_box() {return x_max;}
    float Get_Y_min_box() {return y_min;}
    float Get_Y_max_box() {return y_max;}
    float Get_Z_min_box() {return z_min;}
    float Get_Z_max_box() {return z_max;}

    unsigned int GetNbObjets() {return nb_objets;}
    unsigned int GetNbObjetsReels()    {return nb_objets_reels;}
    void SetNbObjetsReels(unsigned int value) {nb_objets_reels = value;}
    unsigned int GetNbSommets()        {return nb_sommets;}
    void SetNbSommets(unsigned int value)     {nb_sommets = value;}
    unsigned int GetNbSommetsMax()     {return nb_sommets_max;}
    void SetNbSommetsMax(unsigned int value)  {nb_sommets = value;}
    unsigned int GetNbFacettes()       {return nb_facettes;}
    void SetNbFacettes(unsigned int value)    {nb_facettes = value;}
    unsigned int GetNbAretes()         {return nb_aretes;}
    void SetNbAretes(unsigned int value)      {nb_aretes = value;}
    unsigned int GetNb3Points()        {return nb_3points;}
    void SetNb3Points(unsigned int value)     {nb_3points = value;}
    unsigned int GetNb4Points()        {return nb_4points;}
    void SetNb4Points(unsigned int value)     {nb_4points = value;}

    unsigned int GetNumeroObjetMax()      {return numero_objet_max;}
    void SetNumeroObjetMax(unsigned int value)   {numero_objet_max   = value;}
    unsigned int GetNumeroFacetteMax()    {return numero_facette_max;}
    void SetNumeroFacetteMax(unsigned int value) {numero_facette_max = value;}

    void  SetNumeroObjet_ASurligner(unsigned int value) {Numero_Objet_a_Surligner  = value;}
    void  SetNumeroFacetteSurlignee(unsigned int value) {Numero_Facette_Surlignee  = value;}
    void  SetNumeroSommet_ASurligner(int value)         {Numero_Sommet_a_Surligner = value;}

    void  SetFacetteSurlignee(bool value) {Facette_Surlignee = value;}
    void  SetSommetsSurlignes(bool value) {Sommets_Surlignes = value;}

    int   GetSelectionObjet()   {return SelectionObjet;}
    void  SetSelectionObjet(int value) {SelectionObjet = value;}

    bool  GetMaterials()    {return materials;}
    void  SetMaterials(bool value) {materials = value;}
    bool  GetGroupes()      {return groupes;}
    void  SetGroupes(bool value)   {groupes   = value;}
    bool  GetSmooth()       {return smooth;}
    void  SetSmooth(bool value)    {smooth    = value;}

    int   GetGroupeMateriau(int index)     {return GroupeMateriau[index];}
    void  SetGroupeMateriau(int index, int value) {GroupeMateriau[index] = value;}

    void  SetBoiteSymetrique(double x1b1, double x2b1, double  y1b1, double y2b1, double z1b1, double z2b1) {
        x1_b1 = x1_b2 = x1b1; // Recopie de la boîte originale dans la boîte symétrisée (initialisation)
        x2_b1 = x2_b2 = x2b1;
        y1_b1 = y1_b2 = y1b1;
        y2_b1 = y2_b2 = y2b1;
        z1_b1 = z1_b2 = z1b1;
        z2_b1 = z2_b2 = z2b1;
    }
    void SetBoiteSymetriserX() {x1_b2 *= -1.; x2_b2 *= -1.;}
    void SetBoiteSymetriserY() {y1_b2 *= -1.; y2_b2 *= -1.;}
    void SetBoiteSymetriserZ() {z1_b2 *= -1.; z2_b2 *= -1.;}

    void SetOK_ToSave(bool value) {OK_ToSave = value;}

    wxString GetWxNomsFichiers() {return wxNomsFichiers;}

//    void Forcer_OnPaint(wxPaintEvent& event);Forcer_1_Seul_Obj
protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);
    void OnMouseWheelMoved(wxMouseEvent& event);
    void OnMouseZoom(wxMouseEvent& event, int signe, bool Touche_Maj);

private :
    int   icon_size = icon_sizes[icon_index];

    void ShowSelectedFace();

//clear bdd
    void clearall();

//read BDD
    void LoadBDD();
    void Load3DS();
    int  compter_nodes_mesh  (Lib3dsNode *);
    int  decoder_node        (Lib3dsNode *);
    int  mesh_count_smoothing(Lib3dsMesh *);
    void Affiche_Matrice(float Mat4x4[4][4]);
    void LoadG3D();
    void LectureXML_G3d (FILE *);
    void LoadOBJ();
    void Optimiser_Obj_Sommets (Object *, int, bool &);
    void Optimiser_Obj_Vecteurs(Object *, int);
    void LoadPLY();
    void LoadPLY_Stanford();
    void LoadOFF();
    void LoadM3D();
    void LoadSTL();

    void make_position();
//    void make_1_position();

    //opengl selection
//    void selectMode(int selection1);
    void processHits(GLint, bool);
    GLuint* selectBuffer;
    void testPicking(int, int, int, bool) ;
//    void stopPicking();
    bool ifexist_facette(int, int) ;
    bool ifexist_sommet (int, int) ;
    void souderPoints(int, int);
    void Diviser_Arete(int, int, int);
    bool WhoIsUnderTheMouse(GLint, int);

    //opengl affichage
    void showAllPoints();
    void showAllPoints_Selected();
    void showAllLines();
    void showSegment_Selection();
    void showPoint_Selection();
    void buildAllPoints();
    void buildAllPoints_Selected();
    void BuildAllFacettes_Selected();

    double Norme3(float x, float y, float z);
    void Genere_Etoile();
    void Reset_Min_Max();
    void Set_Min_Max(float x, float y, float z);
    void InitGL();

    void CalculAngles(float *, float &, float &, float &);
    void Clamp(float &, const float, const float);
    void Output_Glut_Msg(GLfloat , GLfloat , char *);
    float Produit_Scalaire(const std::vector<float> &, const std::vector<float> &);

    void TracerBoite(double, double, double, double, double, double);
    void buildGroupes(unsigned int &, int &, bool &, int ) ;

    bool Update_Dialog(long long, long long, bool cancel=false);
    void Fermer_progress_dialog();
    void OnTimer_Bdd(wxTimerEvent& event);

    bool Compare_Normales(int, int, int);
    bool Points_Egaux(const std::vector<float> &, const std::vector<float> &, float);
    void Simplification_Doublons_Points(unsigned int);
    void Simplification_Facettes(unsigned int);
    void TraiterMatricePosition(unsigned int);

    void fghCircleTable_local(double **,double **,const int );
    void glutSolidSphere_local(GLdouble , GLint , GLint );

    void AfficherSource();

    void Ouvrir_ini_file();

    void ResetData();
    void Reset_Sliders();

    void Tracer_normale(const std::vector <float> & , const std::vector <float> &, int );
    bool Calcul_Normale_Seuillee(int, int, int, std::vector<float> &, std::vector<float> &);
    void coloriserFacette(unsigned int, unsigned int, bool, const GLfloat couleur[3]);
    void Draw_Rectangle_Selection();
    void Selection_rectangle(GLint, GLint);

    void Switch_theme(bool);
    void Switch_theme_wx33(bool);

    void DrawOpenGL();

    OvniFrame* MAIN_b;

    DECLARE_EVENT_TABLE()

};


#endif // INTERFACE_H_INCLUDED
