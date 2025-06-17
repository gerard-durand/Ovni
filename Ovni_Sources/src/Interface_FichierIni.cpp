#include "Interface.h"

// Lecture et écriture du fichier Ovni.ini
// ***************************************

/* Clés du fichier .ini */
const char *init1="Longueur_axe="       ;
const char *init2="Longueur_normales="  ;
const char *init3="Diam_source="        ;
const char *init4="Antialiasing_Soft="  ;
const char *init5="Forcer_1_Obj="       ;
const char *init6="Lecture_obj_opt="    ;
const char *init7="Decalage_3ds="       ;
const char *init8="Visu_seuils="        ;
const char *init9="Seuil_gouraud="      ;
const char *initA="Coef_2eme_angle="    ;
const char *initB="Enr_Normales_Seuil=" ;
const char *initC="Calcul_normales_S="  ;
const char *initD="Temps_sauveg="       ;
const char *initE="Methode_triangles="  ;
const char *initF="Mode_Trackball="     ;
const char *initG="View_OpenGL_FPS="    ;
const char *initH="Rep_travail="        ;   // Répertoire où est le fichier Bdd (pas Ovni.exe !)
const char *initI="Creer_Backup="       ;
const char *initJ="Suppr_Backup="       ;
const char *initK="Msg_Warning="        ;
const char *initL="Raz_Selection_F="    ;
const char *initM="FacettesNotFlat="    ;
const char *initN="Nb_Threads="         ;
const char *initO="TestDbAretes="       ;
const char *initP="Taille_Icones="      ;
const char *initQ="Dark_Mode="          ;
const char *initR="Sliders_ON_OFF="     ;
const char *initS="Frame_Size_X="       ;
const char *initT="Frame_Size_Y="       ;

FILE *f_init;

void BddInter::Ouvrir_ini_file()
{
    char *Lu, *p_txt_wrk;
    int icmp, ibool, len ;

    f_init = fopen(fichier_init,"r") ;      // Si le fichier n'existe pas, l'ignorer => Utiliser les valeurs par défaut
    if (f_init != nullptr) {
        printf("Ouverture et lecture du fichier d'initialisation : %s\n",fichier_init);
        while ((Lu = fgets(Message,300,f_init)) != nullptr) {
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
                     test_seuil_Gouraud = false;
                else test_seuil_Gouraud = true ;
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
            icmp= strncmp(initH,Message,len) ;                  // Test sur 17ème mot clé
            if (!icmp) {
                p_txt_wrk = Lire_chaine(&Message[len]) ;
                sprintf(Message,"%s",p_txt_wrk) ;               // Récupère le répertoire de travail
                wxWorkDir = wxString::FromUTF8(Message);        // FromAscii(Message); // Lire en utf8 le nom (sinon souci avec les caractères accentués !)
                wxString Last_Char = wxFILE_SEP_PATH ;          // <=> wxFileName::GetPathSeparator()
                if (!wxWorkDir.EndsWith(Last_Char)) wxWorkDir += wxFILE_SEP_PATH ;          // Ajouter le séparateur de Path à la fin s'il n'y est pas déjà !
                sprintf(Message,"Travail dans : %s\n",(const char *)wxWorkDir.utf8_str()) ; // pb avec caractères accentués dans la console windows !
                printf("%s",utf8_To_ibm(Message));
                continue;   // Passer au while suivant
            }
            len = strlen( initI);
            icmp= strncmp(initI,Message,len) ;                  // Test sur 18ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de viewFps
                if (ibool == 0)
                     CreerBackup = false;
                else CreerBackup = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initJ);
            icmp= strncmp(initJ,Message,len) ;                  // Test sur 19ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de viewFps
                if (ibool == 0)
                     SupprBackup = false;
                else SupprBackup = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initK);
            icmp= strncmp(initK,Message,len) ;                  // Test sur 20ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de msg_warning
                if (ibool == 0)
                     msg_warning = false;
                else msg_warning = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initL);
            icmp= strncmp(initL,Message,len) ;                  // Test sur 21ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de Raz_Selection_F
                if (ibool == 0)
                     Raz_Selection_F = false;
                else Raz_Selection_F = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initM);
            icmp= strncmp(initM,Message,len) ;                  // Test sur 22ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de NotFlat
                if (ibool == 0)
                     NotFlat = false;
                else NotFlat = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initN);
            icmp= strncmp(initN,Message,len) ;                  // Test sur 23ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&nb_threads) ;            // Récupère la valeur de nb_threads
                continue;   // Passer au while suivant
            }
            len = strlen( initO);
            icmp= strncmp(initO,Message,len) ;                  // Test sur 24ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de traiter_doublons_aretes
                if (ibool == 0)
                     traiter_doublons_aretes = false;
                else traiter_doublons_aretes = true ;
                continue;   // Passer au while suivant
            }
            len = strlen( initP);
            icmp= strncmp(initP,Message,len) ;                  // Test sur 25ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&icon_size) ;             // Récupère la valeur de icon_size
                icon_index = 0;
                if (icon_size >  16) icon_index = 1 ;
                if (icon_size >= 32) icon_index = 2 ;
                if (icon_size >= 48) icon_index = 3 ;
                icon_size = icon_sizes[icon_index]  ;           // par précaution, n'autoriser que les valeurs de icon_sizes
                continue;   // Passer au while suivant
            }
            len = strlen( initQ);
            icmp= strncmp(initQ,Message,len) ;                  // Test sur 26ème mot clé
            darkmode = -1;                                      // initialisation par défaut
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&darkmode) ;              // Récupère la valeur de DarkMode
                continue;   // Passer au while suivant
            }
            len = strlen( initR);
            icmp= strncmp(initR,Message,len) ;                  // Test sur 27ème mot clé
            ibool = 1;                                          // initialisation par défaut
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&ibool) ;                 // Récupère la valeur de Sliders_ON_OFF
                if (ibool == 0) afficher_sliders = false;
                else            afficher_sliders = true;
                continue;   // Passer au while suivant
            }
            len = strlen( initS);
            icmp= strncmp(initS,Message,len) ;                  // Test sur 28ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&frame_size_x) ;          // Récupère la valeur de la taille en x de frame
                continue;   // Passer au while suivant
            }
            len = strlen( initT);
            icmp= strncmp(initT,Message,len) ;                  // Test sur 29ème mot clé
            if (!icmp) {
                p_txt_wrk = &Message[len] ;
                sscanf(p_txt_wrk,"%d",&frame_size_y) ;          // Récupère la valeur de la taille en y de frame
                continue;   // Passer au while suivant
            }
        }
        ini_file_modified = false;      // Contenu du fichier ini_file non modifié (pas encore !!)
        fclose(f_init);
    } else {
        ini_file_modified = true;       // Contenu du fichier ini_file à créer car n'existait pas (sauf modifs ultérieures, ce sera avec les valeurs par défaut) !
    }
}

void BddInter::Stocker_ini_file()
{

    if (!ini_file_modified) return ;        // Ne rien faire si le fichier n'a pas été modifié

    f_init = fopen(fichier_init,"w") ;

    if (f_init != nullptr) {
        fprintf(f_init,"%s%f\n",init1,len_axe) ;
        fprintf(f_init,"%s%f\n",init2,len_normales);
        fprintf(f_init,"%s%f\n",init3,ray_sun) ;
        fprintf(f_init,"%s%d\n",init4,antialiasing_soft);
        fprintf(f_init,"%s%d\n",init5,Forcer_1_Seul_Objet);
        fprintf(f_init,"%s%d\n",init6,lect_obj_opt);
        fprintf(f_init,"%s%d\n",init7,test_decalage3ds);
        fprintf(f_init,"%s%d\n",init8,test_seuil_Gouraud);
        fprintf(f_init,"%s%f\n",init9,angle_Gouraud);
        fprintf(f_init,"%s%f\n",initA,fmult_Gouraud);
        fprintf(f_init,"%s%d\n",initB,Enr_Normales_Seuillees);
        fprintf(f_init,"%s%d\n",initC,CalculNormalesLectureBdd);
        fprintf(f_init,"%s%d\n",initD,svg_time);
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
        fprintf(f_init,"%s%d\n",initN,nb_threads) ;
        fprintf(f_init,"%s%d\n",initO,traiter_doublons_aretes) ;
        fprintf(f_init,"%s%d\n",initP,icon_size);
        fprintf(f_init,"%s%d\n",initQ,darkmode);
        fprintf(f_init,"%s%d\n",initR,afficher_sliders);
        fprintf(f_init,"%s%d\n",initS,frame_size_x);
        fprintf(f_init,"%s%d\n",initT,frame_size_y);

//        fprintf(f_init,"TEST\n") ;
        fclose(f_init) ;
    }
    else {
        wxString wxMessage=_T("Écriture impossible dans le fichier .ini");
        DisplayMessage(wxMessage,true);
//        return ;
    }
    return ;
}

