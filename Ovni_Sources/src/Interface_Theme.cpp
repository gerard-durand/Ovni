#include "Interface.h"

// Bascule à l'exécution entre les modes Windows Standard et Windows Sombre (DarkMode)
// Versions pour wxWidgets version 3.3 et plus, puis tests sur version 3.2.x et moins
// ***********************************************************************************

// Encore beaucoup de code en commentaires (tests divers) car le mode Dark, est encore en développement sous wxWidgets 3.3

void BddInter::Switch_theme_menus(bool darkmode)
{
/* Note : dans OvniMain.cpp dans la rubrique //(*Initialize(OvniFrame), juste avant le MenuFile->Append(Menu_Open); on pourrait ajouter quelque chose comme
   if (local_darkmode) MenuFile->SetTextColour(*wxCYAN); // mais voir comment récupérer ce wxCYAN de façon plus globale pour pouvoir le changer facilement au niveau général.
   Mais ça se trouve dans un bloc de lignes généré automatiquement par wxSmith.
   Tant qu'on ne change rien dans Ovniframe.wxs, ça marche, mais à la génération suivante, suite à une modification dans Ovniframe.wxs, ce sera perdu.
   L'option Code Complémentaire dans OvniFrame.wxs aurait pu faire l'affaire, mais le code est ajouté après le Append et donc, ça ne va pas. Dommage.
*/

    size_t nb,num_menu,nb_menus,nb_item;
    wxMenu*     menu;
    wxMenuItem* item;

    wxColour Forg;
//    wxColour Back;

    if (darkmode) {
        Forg = New_Forg;
//        Back = New_Back;
    } else {
        Forg = wxNullColour;    // <> couleur standard
//        Back = wxNullColour;
    }

#define methode_boucle 0
// Methode_boucle à 0 <=> false, à 1 <=> true

#if methode_boucle

/* Parcourir en boucle les menus fonctionne mais amene souvent un plantage si on active plusieurs fois cette option.
   C'est la succession de Remove, Prepend qui fait planter, mais sans ça, pas de changement de couleur !
   Parcourir 1 par 1 avec les noms n'a pas ce souci, mais pas beaucoup d'intérêt car plus lourd à programmer.
   Après le redémarrage d'Ovni, s'il est en Darkmode, ça se passera bien !
*/
// Astuce obtenue via google Mode IA avec la recherche "GetMenuItems wxwidgets" en y ajoutant "modifier dynamiquement" dans poser une question

    nb_menus = this->MAIN_b->MenuBar_Globale->GetMenuCount();               // Nombre de menus dans la barre (Fichier Affichage ...)
    for (num_menu=0; num_menu<nb_menus; num_menu++) {
        menu     = this->MAIN_b->MenuBar_Globale->GetMenu(num_menu);
        wxMenuItemList& listeItems = menu->GetMenuItems();
        nb_item  = menu->GetMenuItemCount();                                // Nombre de sous-menus pour chacun des menus de la barre
        if (verbose)
            printf("num_menu %lld, nb_item %lld\n",num_menu,nb_item);
//        for (nb=0; nb<nb_item; nb++,node=node->GetNext()) {
        nb = 0;
        for (wxMenuItemList::iterator it = listeItems.begin(); it != listeItems.end(); ++it) {
//        for (wxMenuItem* item : menu->GetMenuItems()) {
            item = *it;
            if (item->IsSeparator()) continue;      // Si c'est un séparateur, passer au menu suivant, car les Set*Colour n'y fonctionnent pas bien
            item->SetTextColour(Forg);
//            item->SetBackgroundColour(Back);
            if (nb == 0) {
                menu->Remove(item);
                menu->Prepend(item);
                nb++;
            }
         }
         menu->UpdateUI();                          // N'est plus indispensable semble t-il
       }

#else // <=> !methode_boucle

/* Ici, c'est la méthode bourrin où on parcourt 1 à 1 tous les sous-menus par leur nom */

    this->MAIN_b->Menu_Open         ->SetTextColour(Forg);

    this->MAIN_b->MenuFile->Remove (this->MAIN_b->Menu_Open);       // Il en faut au moins 1 pour que UpdateUI() fonctionne
    this->MAIN_b->MenuFile->Prepend(this->MAIN_b->Menu_Open);

    this->MAIN_b->Menu_ReOpen       ->SetTextColour(Forg);
    if (this->MAIN_b->Menu_ReOpen3ds != nullptr) {
        this->MAIN_b->Menu_ReOpen3ds->SetTextColour(Forg);
    }
    this->MAIN_b->Menu_AddFile      ->SetTextColour(Forg);
    this->MAIN_b->Menu_Enregistrer  ->SetTextColour(Forg);
    this->MAIN_b->Menu_Enregistrer_Sous->SetTextColour(Forg);
    this->MAIN_b->Menu_Proprietes   ->SetTextColour(Forg);
    this->MAIN_b->Menu_Preferences  ->SetTextColour(Forg);
    this->MAIN_b->Menu_Hardware3D   ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_Quitter  ->SetTextColour(Forg);

//    this->MAIN_b->MenuFile->UpdateUI();

    this->MAIN_b->Menu_Affichage_Points   ->SetTextColour(Forg);
    this->MAIN_b->Menu_Affichage->Remove (this->MAIN_b->Menu_Affichage_Points); // Il en faut au moins 1, sinon UpdateUI ne fait rien !
    this->MAIN_b->Menu_Affichage->Prepend(this->MAIN_b->Menu_Affichage_Points);
    this->MAIN_b->Menu_Affichage_Filaire  ->SetTextColour(Forg);
    this->MAIN_b->Menu_Affichage_Plein    ->SetTextColour(Forg);
    this->MAIN_b->Menu_Affichage_Axes     ->SetTextColour(Forg);
    this->MAIN_b->Menu_Affichage_Boite    ->SetTextColour(Forg);
    this->MAIN_b->Menu_Affichage_Source   ->SetTextColour(Forg);
    this->MAIN_b->Menu_Origine            ->SetTextColour(Forg);
    this->MAIN_b->Menu_VueDeFace          ->SetTextColour(Forg);
    this->MAIN_b->Menu_VueDeProfil        ->SetTextColour(Forg);
    this->MAIN_b->Menu_VueDeDessus        ->SetTextColour(Forg);
    this->MAIN_b->Menu_PositionObservateur->SetTextColour(Forg);
    this->MAIN_b->Menu_ZoomSpecifique     ->SetTextColour(Forg);
    this->MAIN_b->Menu_CentreRotation     ->SetTextColour(Forg);
    this->MAIN_b->Menu_PositionSource     ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Affichage->UpdateUI();

    this->MAIN_b->Menu_AjouteCone         ->SetTextColour(Forg);
    this->MAIN_b->Menu_Primitive->Remove (this->MAIN_b->Menu_AjouteCone);
    this->MAIN_b->Menu_Primitive->Prepend(this->MAIN_b->Menu_AjouteCone);
    this->MAIN_b->Menu_AjouteCube         ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteCylindre     ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteEllipsoide   ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteFacette      ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteSphere       ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteTore         ->SetTextColour(Forg);
    this->MAIN_b->Menu_AjouteIcosaedre    ->SetTextColour(Forg);
    this->MAIN_b->Menu_SupprimerDerniere  ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Primitive->UpdateUI();

    this->MAIN_b->Menu_ReperagePoint                ->SetTextColour(Forg);
    this->MAIN_b->Menu_Reperage->Remove (this->MAIN_b->Menu_ReperagePoint);
    this->MAIN_b->Menu_Reperage->Prepend(this->MAIN_b->Menu_ReperagePoint);
    this->MAIN_b->Menu_ReperageFacette              ->SetTextColour(Forg);
    this->MAIN_b->Menu_ReperageGroupe               ->SetTextColour(Forg);
    this->MAIN_b->Menu_ReperageMateriau             ->SetTextColour(Forg);
    this->MAIN_b->Menu_ReperageObjet                ->SetTextColour(Forg);
    this->MAIN_b->Menu_SensDesNormales              ->SetTextColour(Forg);
    this->MAIN_b->Menu_Reperage_Couleurs_Facettes   ->SetTextColour(Forg);
    this->MAIN_b->Menu_Reperage_Couleurs_Groupes    ->SetTextColour(Forg);
    this->MAIN_b->Menu_Reperage_Couleurs_Materiaux  ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Reperage->UpdateUI();

    this->MAIN_b->MenuItem_ImageJpeg->SetTextColour(Forg);
    this->MAIN_b->Menu_Image->Remove (this->MAIN_b->MenuItem_ImageJpeg);
    this->MAIN_b->Menu_Image->Prepend(this->MAIN_b->MenuItem_ImageJpeg);
    this->MAIN_b->MenuItem_ImagePng ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_ImagePpm ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Image->UpdateUI();

    this->MAIN_b->Menu_RAZ_SelectionFacettes->SetTextColour(Forg);
    this->MAIN_b->Menu_Outils->Remove (this->MAIN_b->Menu_RAZ_SelectionFacettes);
    this->MAIN_b->Menu_Outils->Prepend(this->MAIN_b->Menu_RAZ_SelectionFacettes);
    this->MAIN_b->Menu_MasquerFacettes      ->SetTextColour(Forg);
    this->MAIN_b->Menu_SupprimerFacettes    ->SetTextColour(Forg);
    this->MAIN_b->Outils_Selec_Depl         ->SetTextColour(Forg);
    this->MAIN_b->Outils_Modifications      ->SetTextColour(Forg);
    this->MAIN_b->Outils_choix_afficher     ->SetTextColour(Forg);
    this->MAIN_b->Outils_Reafficher         ->SetTextColour(Forg);
    this->MAIN_b->Outils_Supprimer_Masques  ->SetTextColour(Forg);
    this->MAIN_b->Outils_UnDelete           ->SetTextColour(Forg);
    this->MAIN_b->Menu_Retracer3D           ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Outils->UpdateUI();

    this->MAIN_b->MenuItem_SigneX ->SetTextColour(Forg);
    this->MAIN_b->Menu_Transformations->Remove (this->MAIN_b->MenuItem_SigneX);
    this->MAIN_b->Menu_Transformations->Prepend(this->MAIN_b->MenuItem_SigneX);
    this->MAIN_b->MenuItem_SigneY ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_SigneZ ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_PermXY ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_PermXZ ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_PermYZ ->SetTextColour(Forg);
    this->MAIN_b->MenuItem_PermXYZ->SetTextColour(Forg);
    this->MAIN_b->Inverser_Toutes_les_Normales  ->SetTextColour(Forg);
    this->MAIN_b->Inverse_All_Selected_Normales ->SetTextColour(Forg);
    this->MAIN_b->Menu_DeplacerBdd              ->SetTextColour(Forg);
    this->MAIN_b->Menu_FacteurEchelleBdd        ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Transformations->UpdateUI();

    this->MAIN_b->Menu_CouleurDesGroupes ->SetTextColour(Forg);
    this->MAIN_b->Menu_Options->Remove (this->MAIN_b->Menu_CouleurDesGroupes);
    this->MAIN_b->Menu_Options->Prepend(this->MAIN_b->Menu_CouleurDesGroupes);
    this->MAIN_b->Menu_RelirePalette     ->SetTextColour(Forg);
    this->MAIN_b->Menu_EnregistrerPalette->SetTextColour(Forg);
    this->MAIN_b->Menu_ZoomAuto          ->SetTextColour(Forg);
    this->MAIN_b->Menu_CentrageAuto      ->SetTextColour(Forg);

//    this->MAIN_b->Menu_Options->UpdateUI();

    this->MAIN_b->MenuItem_Aide ->SetTextColour(Forg);
    this->MAIN_b->Menu_Aide->Remove (this->MAIN_b->MenuItem_Aide);
    this->MAIN_b->Menu_Aide->Prepend(this->MAIN_b->MenuItem_Aide);
    this->MAIN_b->MenuItem_About->SetTextColour(Forg);

//    this->MAIN_b->Menu_Aide->UpdateUI();

#endif  // !methode_boucle

}

#if wxCHECK_VERSION(3,3,0)                  // Version de Switch_theme pour wxWidgets 3.3 et +

void BddInter::Switch_theme(bool darkmode)  // Plutôt que Switch_theme_wx33, donner le même nom que si version inférieure: choix fait via #if wxCHECK_VERSION(3,3,0)
{
// Pour basculer au vol entre un thème clair et un thème foncé pendant l'exécution d'Ovni. Pas besoin si thème choisi au démarrage.
// Version pour wxWidgets 3.3 et +

// La colorisation faite ici affecte surtout l'interface principale (sinon il faut re-coloriser 1 à 1 les élements !). La colorisation complète ne sera obtenue qu'au redémarrage d'Ovni.
// Fait aussi sur la boîte de dialogue Modifications, mais plutôt fastidieux

// A ce niveau, il faut balayer certains éléments déjà créés pour les re-coloriser

    wxColour Forg;
    wxColour Back;

    if (darkmode) {
        Forg = New_Forg;
        Back = New_Back;
    } else {
        Forg = wxNullColour;    // <> couleur standard
        Back = wxNullColour;
    }

// Fenêtre principale (on ajuste des éléments 1 par 1)

//    this->MAIN_b->MenuBar_Globale->Refresh();
///    Back = this->MAIN_b->MenuBar_Globale->GetBackgroundColour();
//    printf("Back 0x%06x\n",Back.GetRGB());
//    wxColour test = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
//    printf("test 0x%06x\n",test.GetRGB());
///    Forg = this->MAIN_b->MenuBar_Globale->GetForegroundColour();
//    printf("Forg 0x%06x\n",Forg.GetRGB());
//    test = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
//    printf("test 0x%06x\n",test.GetRGB());
//    Back = this->MAIN_b->Button_Gauche->GetBackgroundColour();

    this->MAIN_b->SetForegroundColour(Forg);
//    this->MAIN_b->SetBackgroundColour(Back);  // Inutile car fait automatiquement

    this->MAIN_b->Button_Droite     ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Gauche     ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Haut       ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Bas        ->SetForegroundColour(Forg);
    this->MAIN_b->Button_ZoomPlus   ->SetForegroundColour(Forg);
    this->MAIN_b->Button_ZoomMoins  ->SetForegroundColour(Forg);
    this->MAIN_b->MenuBar_Globale   ->SetForegroundColour(Forg);    // Ne marche pas (ou il manque quelquechose ?)

    this->MAIN_b->StaticText1       ->SetForegroundColour(Forg);
    this->MAIN_b->StaticText2       ->SetForegroundColour(Forg);

#if 1   /// si 0, colorisation standard des modes clair et sombre, si 1, colorise diverses parties en cyan quand on est en mode sombre
        /// En fait, c'est pour des changements dynamiques via la touche W ou le choix de thème dans Préférences.
        /// Au démarrage suivant, si on est en darkmode ce sera fait d'office par le système.
        /// La plupart des SetBackgroundColour ne sont plus utiles car faits semble t-il par le système.

// Boîte de dialogue Préférences

    this->MPrefs->StaticText1                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText2                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText3                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText4                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText5                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText7                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText8                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText9                   ->SetForegroundColour(Forg);
    this->MPrefs->StaticText_Gouraud            ->SetForegroundColour(Forg);
    this->MPrefs->StaticText_Gouraud2           ->SetForegroundColour(Forg);
    this->MPrefs->TextCtrl_WorkDir              ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_axes           ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_norm           ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_src            ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud   ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud2  ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrl_PasSvg               ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrl_Threads              ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_1SeulObjet3D         ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_LectureOptimisee     ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_TestDecalage3DS      ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_Seuillage            ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_AntialiasingSoft     ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_CalculNormales       ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_DisplayFps           ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_NotFlat              ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_TraiterDoublonsAretes->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_RecNormales_Seuillees->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_CreerBackup          ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_SupprBackup          ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_Trackball            ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_Triangulation        ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_IconSize             ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_DarkMode             ->SetForegroundColour(Forg);
    this->MPrefs->Button_tmp_rep                ->SetForegroundColour(Forg);
    this->MPrefs->Button_tmp_rep                ->SetBackgroundColour(Back);
    this->MPrefs->Button_Reset                  ->SetForegroundColour(Forg);
    this->MPrefs->Button_Reset                  ->SetBackgroundColour(Back);
    this->MPrefs->Button_OK                     ->SetForegroundColour(Forg);
    this->MPrefs->Button_OK                     ->SetBackgroundColour(Back);

// Boîte de dialogue Modifications  (on ajuste des éléments 1 par 1)

    this->MPanel->CheckBox_FacettePlane     ->SetForegroundColour(Forg);
    this->MPanel->CheckBox_NotFlat          ->SetForegroundColour(Forg);
    this->MPanel->CheckBox_Transparence     ->SetForegroundColour(Forg);

    this->MPanel->StaticText1               ->SetForegroundColour(Forg);
    this->MPanel->StaticText2               ->SetForegroundColour(Forg);
    this->MPanel->StaticText3               ->SetForegroundColour(Forg);
    this->MPanel->StaticText4               ->SetForegroundColour(Forg);
    this->MPanel->StaticText5               ->SetForegroundColour(Forg);
    this->MPanel->StaticText6               ->SetForegroundColour(Forg);
    this->MPanel->StaticText7               ->SetForegroundColour(Forg);
    this->MPanel->StaticText8               ->SetForegroundColour(Forg);
    this->MPanel->TextCtrl_Tolerance        ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Ajouter      ->SetBackgroundColour(Back);    // là, il le faut !
    this->MPanel->ToggleButton_Ajouter      ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_CreerFacette ->SetBackgroundColour(Back);
    this->MPanel->ToggleButton_CreerFacette ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Diviser      ->SetBackgroundColour(Back);
    this->MPanel->ToggleButton_Diviser      ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Souder       ->SetBackgroundColour(Back);
    this->MPanel->ToggleButton_Souder       ->SetForegroundColour(Forg);
    this->MPanel->Button_Annuler            ->SetBackgroundColour(Back);
    this->MPanel->Button_Annuler            ->SetForegroundColour(Forg);
    this->MPanel->Button_InverserNormale    ->SetBackgroundColour(Back);
    this->MPanel->Button_InverserNormale    ->SetForegroundColour(Forg);
    this->MPanel->Button_InverserTout       ->SetBackgroundColour(Back);
    this->MPanel->Button_InverserTout       ->SetForegroundColour(Forg);
    this->MPanel->Button_RecalculerAretes   ->SetBackgroundColour(Back);
    this->MPanel->Button_RecalculerAretes   ->SetForegroundColour(Forg);
    this->MPanel->Button_RecalculerNormales ->SetBackgroundColour(Back);
    this->MPanel->Button_RecalculerNormales ->SetForegroundColour(Forg);
    this->MPanel->Button_Simplification     ->SetBackgroundColour(Back);
    this->MPanel->Button_Simplification     ->SetForegroundColour(Forg);
    this->MPanel->Button_SupprimerFacette   ->SetBackgroundColour(Back);
    this->MPanel->Button_SupprimerFacette   ->SetForegroundColour(Forg);
    this->MPanel->Button_Trianguler         ->SetBackgroundColour(Back);
    this->MPanel->Button_Trianguler         ->SetForegroundColour(Forg);
    this->MPanel->Button_Undo               ->SetBackgroundColour(Back);
    this->MPanel->Button_Undo               ->SetForegroundColour(Forg);
    this->MPanel->Button_Quitter            ->SetBackgroundColour(Back);
    this->MPanel->Button_Quitter            ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_Groupe           ->SetBackgroundColour(Back);
    this->MPanel->SpinCtrl_Groupe           ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_Materiau         ->SetBackgroundColour(Back);
    this->MPanel->SpinCtrl_Materiau         ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_NbSegments       ->SetBackgroundColour(Back);
    this->MPanel->SpinCtrl_NbSegments       ->SetForegroundColour(Forg);

// Boîte de dialogue Sélections et Déplacements

    this->MSelect->RadioButton_Selection_Points     ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Selection_Facettes   ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Selection_Objets     ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Both   ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Avant  ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Arriere->SetForegroundColour(Forg);
    this->MSelect->CheckBox_ForcerFlat              ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Grp                  ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Mat                  ->SetForegroundColour(Forg);

// Boîtes de dialogue des Primitives

    this->MCone     ->CheckBox_FermerCone       ->SetForegroundColour(Forg);
    this->MCylindre ->CheckBox_FermerCylindre   ->SetForegroundColour(Forg);
    this->MEllips   ->CheckBox_NewSphere        ->SetForegroundColour(Forg);
    this->MSphere   ->CheckBox_NewSphere        ->SetForegroundColour(Forg);

// Boîte de dialogue Repérage Objets

    this->MRepObj->CheckBox_masquer     ->SetForegroundColour(Forg);
    this->MRepObj->CheckBox_supprimer   ->SetForegroundColour(Forg);
    this->MRepObj->CheckBox_renommer    ->SetForegroundColour(Forg);

// Boîte de dialogue Repérage Points

    this->MRepPoint->CheckBox_X         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Y         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Z         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Laisser   ->SetForegroundColour(Forg);

// Boîte de dialogue Repérage Facettes

    this->MRepFacet->CheckBox_Laisser               ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuNormale           ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuSommets           ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuNormales_Sommets  ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_Laisser               ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_Laisser               ->SetForegroundColour(Forg);

// Autres

    this->MRotation->RadioBox_Centre    ->SetForegroundColour(Forg);
    this->MScale->CheckBox_ScaleUnique  ->SetForegroundColour(Forg);

    Switch_theme_menus(darkmode);

#endif // 0 ou 1

// Ajustement de la couleur Bleue utilisée par quelques éléments car elle est trop foncée sur fond sombre : remplacer par du Cyan
    wxColour New_blue;
    if (darkmode)
        New_blue = *wxCYAN;
    else
        New_blue = *wxBLUE;

    this->MAIN_b    ->Slider_z   ->SetForegroundColour(New_blue);
    this->MPosCRot  ->StaticText4->SetForegroundColour(New_blue);
    this->MDeplacer ->StaticText4->SetForegroundColour(New_blue);
    this->MManip    ->CheckBox_Z ->SetForegroundColour(New_blue);
    this->MPosLight ->Pos_Z      ->SetForegroundColour(New_blue);
    this->MTrans    ->StaticText7->SetForegroundColour(New_blue);
    this->MTrans    ->StaticText8->SetForegroundColour(New_blue);

// Refresh des divers panels

    this->MAIN_b    ->MenuBar_Globale->Refresh();   // Utile ?
    this->MPanel    ->Refresh();
    this->MPrefs    ->Refresh();
    this->MPosCRot  ->Refresh();
    this->MCone     ->Refresh();
    this->MCube     ->Refresh();
    this->MCylindre ->Refresh();
    this->MEllips   ->Refresh();
    this->MFacet    ->Refresh();
    this->MSphere   ->Refresh();
    this->MIcosa    ->Refresh();
    this->MTore     ->Refresh();
    this->MDeplacer ->Refresh();
    this->MManip    ->Refresh();
    this->MPosObs   ->Refresh();
    this->MPosLight ->Refresh();
    this->MRepFacet ->Refresh();
    this->MRepObj   ->Refresh();
    this->MRepPoint ->Refresh();
    this->MRepGrp   ->Refresh();
    this->MRepMat   ->Refresh();
    this->MRotation ->Refresh();
    this->MScale    ->Refresh();
    this->MSelect   ->Refresh();
    this->MSelFac   ->Refresh();
    this->MSelObj   ->Refresh();
    this->MTrans    ->Refresh();

    this->MScale_0  ->Refresh();
    this->MChoice_O ->Refresh();
    this->MCGroup   ->Refresh();
    this->MProps    ->Refresh();
    this->MZoomSpec ->Refresh();
    this->MHelp     ->Refresh();

    this->MAIN_b    ->Refresh();

}

#else
// Ci-dessous, devrait pouvoir être supprimé (ou à fusionner) une fois la bascule vers wxWidgets 3.3 définitive... MAIS GARDÉ por le moment car semble un peu mieux pour un fonctionnement au vol (mais si compliqué !!!!)
// Peut-être conserver cette version Switch_theme_menus qui a l'avantage de pouvoir fonctionner de faon plus fiable, bien que plus complexe

void BddInter::Switch_theme(bool darkmode)
{
// Pour basculer entre un thème clair et un thème foncé pendant l'exécution d'Ovni.
// Version pour wxWidgets avant la 3.3 et > 3.0. Mais fonctionne aussi si > 3.3
// Certains éléments, comme les separators dans les menus, la barre de menu principale,... restent sur fond clair !
// Mieux géré avec wxWidgets 3.3 et +, mais avec d'autres soucis !

// Note : comme pour la version ci-dessus, les SetBackGroundColour ne sont pas tous utiles. A vérifier éventuellement

// Test pour Basculer du thème standard vers un thème sombre. Mais semble assez (oups, très) laborieux !

//    int next;
    wxColour Forg;
    wxColour Back;
    wxColour Gris;
    wxColour Bleu_svg ;
    bool methode_boucle = false;    // Pour tester 2 méthodes, une avec boucles (true), + élégante mais qui plante parfois, et une très bourrine, mais plus fiable !

    if (darkmode) {
        Bleu_svg= this->MAIN_b->Slider_z->GetForegroundColour(); // Sur ce slider/curseur en Z, le bleu original est trop foncé
        Forg    = New_Forg;
        Back    = New_Back;
        Gris    = New_Gris;
    } else {
        Forg    = wxNullColour;
        Back    = wxNullColour;
        Gris    = wxNullColour;
    }

//this->SetThemeEnabled(true);
//this->MAIN_b->SetThemeEnabled(true);

// Colorisation de l'interface de base et des menus
    this->MAIN_b->SetForegroundColour(Forg);
    this->MAIN_b->SetBackgroundColour(Back);
    this->MAIN_b->Button_Droite     ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Droite     ->SetBackgroundColour(Gris);
    this->MAIN_b->Button_Gauche     ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Gauche     ->SetBackgroundColour(Gris);
    this->MAIN_b->Button_Haut       ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Haut       ->SetBackgroundColour(Gris);
    this->MAIN_b->Button_Bas        ->SetForegroundColour(Forg);
    this->MAIN_b->Button_Bas        ->SetBackgroundColour(Gris);
    this->MAIN_b->Button_ZoomPlus   ->SetForegroundColour(Forg);
    this->MAIN_b->Button_ZoomPlus   ->SetBackgroundColour(Gris);
    this->MAIN_b->Button_ZoomMoins  ->SetForegroundColour(Forg);
    this->MAIN_b->Button_ZoomMoins  ->SetBackgroundColour(Gris);
    this->MAIN_b->Panel1            ->SetBackgroundColour(Back);
    this->MAIN_b->StaticText1       ->SetForegroundColour(Forg);
    this->MAIN_b->StaticText2       ->SetForegroundColour(Forg);
//    this->MAIN_b->Button_Points     ->SetBackgroundColour(Back);    // Ne donne qu'un petit liseré noir autour de l'icône
//    this->MAIN_b->Button_Filaire    ->SetBackgroundColour(Back);

    this->MAIN_b->MenuBar_Globale   ->SetForegroundColour(Forg);    // Ne marche pas (ou il manque quelquechose ?)
    this->MAIN_b->MenuBar_Globale   ->SetBackgroundColour(Gris);    //
//    this->MAIN_b->MenuBar_Globale->SetThemeEnabled(true);
//    this->MAIN_b->MenuBar_Globale->Remove(0);
//    this->MAIN_b->MenuBar_Globale->Insert(0,this->MAIN_b->MenuFile,"Fichier");
//    this->MAIN_b->MenuBar_Globale->UpdateWindowUI();
//    this->MAIN_b->MenuBar_Globale->Update();
//    this->MAIN_b->MenuBar_Globale->UpdateMenus();
//    this->MAIN_b->MenuBar_Globale->Refresh();

    if (darkmode)
        this->MAIN_b->Slider_z->SetForegroundColour(*wxCYAN);   // Bleu initial trop foncé, mettre plutôt Cyan pour darkmode
    else
        this->MAIN_b->Slider_z->SetForegroundColour(Bleu_svg);  // Reset de la couleur originale

    this->MAIN_b->StatusBar1  ->SetBackgroundColour(Back);

// Popup menus : fait directement dans OnMouse, if (event.RightUp() ...

// Colorisation des dialogues
// CentreRotation
    this->MPosCRot->SetForegroundColour(Forg);
    this->MPosCRot->SetBackgroundColour(Back);
    this->MPosCRot->StaticText1->SetForegroundColour(Forg);
    if (darkmode)
        this->MPosCRot->StaticText4->SetForegroundColour(New_Forg);
    else
        this->MPosCRot->StaticText4->SetForegroundColour(Bleu_svg);

    this->MPosCRot->SpinCtrlDouble_X->SetForegroundColour(Forg);
    this->MPosCRot->SpinCtrlDouble_Y->SetForegroundColour(Forg);
    this->MPosCRot->SpinCtrlDouble_Z->SetForegroundColour(Forg);
    this->MPosCRot->SpinCtrlDouble_X->SetBackgroundColour(Gris);
    this->MPosCRot->SpinCtrlDouble_Y->SetBackgroundColour(Gris);
    this->MPosCRot->SpinCtrlDouble_Z->SetBackgroundColour(Gris);
    this->MPosCRot->Button_Defs     ->SetForegroundColour(Forg);
    this->MPosCRot->Button_Defs     ->SetBackgroundColour(Gris);
    this->MPosCRot->Button_OK       ->SetForegroundColour(Forg);
    this->MPosCRot->Button_OK       ->SetBackgroundColour(Gris);
    this->MPosCRot->Refresh();

// ChangerEchelleBdd
    this->MScale_0->SetForegroundColour(Forg);
    this->MScale_0->SetBackgroundColour(Back);
    this->MScale_0->StaticText1     ->SetForegroundColour(Forg);
    this->MScale_0->StaticText2     ->SetForegroundColour(Forg);
    this->MScale_0->TextCtrl_scale  ->SetForegroundColour(Forg);
    this->MScale_0->TextCtrl_scale  ->SetBackgroundColour(Gris);
    this->MScale_0->Button_OK       ->SetForegroundColour(Forg);
    this->MScale_0->Button_OK       ->SetBackgroundColour(Gris);
    this->MScale_0->Button_Inverser ->SetForegroundColour(Forg);
    this->MScale_0->Button_Inverser ->SetBackgroundColour(Gris);
    this->MScale_0->Button_Quitter  ->SetForegroundColour(Forg);
    this->MScale_0->Button_Quitter  ->SetBackgroundColour(Gris);
    this->MScale_0->Refresh();

// ChoixAffichageObjets
    this->MChoice_O->SetForegroundColour(Forg);
    this->MChoice_O->SetBackgroundColour(Back);
    this->MChoice_O->StaticText1    ->SetForegroundColour(Forg);
//    this->MChoice_O->CheckListBox1->SetForegroundColour(Forg);    // Ne marche pas
//    this->MChoice_O->CheckListBox1  ->SetBackgroundColour(Back);
    this->MChoice_O->Button_Aucun   ->SetForegroundColour(Forg);
    this->MChoice_O->Button_Aucun   ->SetBackgroundColour(Gris);
    this->MChoice_O->Button_Tous    ->SetForegroundColour(Forg);
    this->MChoice_O->Button_Tous    ->SetBackgroundColour(Gris);
    this->MChoice_O->Button_Quitter ->SetForegroundColour(Forg);
    this->MChoice_O->Button_Quitter ->SetBackgroundColour(Gris);
    this->MChoice_O->Refresh();

// Cone
    this->MCone->SetForegroundColour(Forg);
    this->MCone->SetBackgroundColour(Back);
    this->MCone->StaticText1->SetForegroundColour(Forg);
    this->MCone->StaticText2->SetForegroundColour(Forg);
    this->MCone->StaticText3->SetForegroundColour(Forg);
    this->MCone->StaticText4->SetForegroundColour(Forg);
    this->MCone->StaticText5->SetForegroundColour(Forg);
    this->MCone->StaticText6->SetForegroundColour(Forg);
    this->MCone->StaticText7->SetForegroundColour(Forg);
    this->MCone->StaticText8->SetForegroundColour(Forg);
    this->MCone->StaticText9->SetForegroundColour(Forg);
    this->MCone->TextCtrl_Longueur  ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_Longueur  ->SetBackgroundColour(Gris);
    this->MCone->TextCtrl_Rayon     ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_Rayon     ->SetBackgroundColour(Gris);
    this->MCone->TextCtrl_X         ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_X         ->SetBackgroundColour(Gris);
    this->MCone->TextCtrl_Y         ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_Y         ->SetBackgroundColour(Gris);
    this->MCone->TextCtrl_Z         ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_Z         ->SetBackgroundColour(Gris);
    this->MCone->TextCtrl_NbPoints  ->SetForegroundColour(Forg);
    this->MCone->TextCtrl_NbPoints  ->SetBackgroundColour(Gris);
    this->MCone->SpinCtrl_Groupe    ->SetForegroundColour(Forg);
    this->MCone->SpinCtrl_Groupe    ->SetBackgroundColour(Gris);
    this->MCone->SpinCtrl_Materiau  ->SetForegroundColour(Forg);
    this->MCone->SpinCtrl_Materiau  ->SetBackgroundColour(Back);
    this->MCone->SpinCtrl_NbSecteurs->SetForegroundColour(Forg);
    this->MCone->SpinCtrl_NbSecteurs->SetBackgroundColour(Gris);
    this->MCone->CheckBox_FermerCone->SetForegroundColour(Forg);
    this->MCone->CheckBox_FermerCone->SetBackgroundColour(Gris);
    this->MCone->Button_OK          ->SetForegroundColour(Forg);
    this->MCone->Button_OK          ->SetBackgroundColour(Gris);
    this->MCone->Button_Annuler     ->SetForegroundColour(Forg);
    this->MCone->Button_Annuler     ->SetBackgroundColour(Gris);
    this->MCone->Refresh();

// CouleursGroupes
    this->MCGroup->SetForegroundColour(Forg);
    this->MCGroup->SetBackgroundColour(Back);
    this->MCGroup->StaticText1          ->SetForegroundColour(Forg);
    this->MCGroup->StaticText2          ->SetForegroundColour(Forg);
    this->MCGroup->StaticText3          ->SetForegroundColour(Forg);
    this->MCGroup->StaticText4          ->SetForegroundColour(Forg);
    this->MCGroup->StaticText5          ->SetForegroundColour(Forg);
    this->MCGroup->SpinCtrl_Groupes     ->SetForegroundColour(Forg);
    this->MCGroup->SpinCtrl_Groupes     ->SetBackgroundColour(Gris);
    this->MCGroup->TextCtrl_Materiau    ->SetForegroundColour(Forg);
    this->MCGroup->TextCtrl_Materiau    ->SetBackgroundColour(Gris);
    this->MCGroup->Button_Reset_Couleur ->SetForegroundColour(Forg);
    this->MCGroup->Button_Reset_Couleur ->SetBackgroundColour(Gris);
    this->MCGroup->Button_Reset_ALL     ->SetForegroundColour(Forg);
    this->MCGroup->Button_Reset_ALL     ->SetBackgroundColour(Gris);
    this->MCGroup->Button_Quitter       ->SetForegroundColour(Forg);
    this->MCGroup->Button_Quitter       ->SetBackgroundColour(Gris);
    this->MCGroup->Refresh();

// Cube
    this->MCube->SetForegroundColour(Forg);
    this->MCube->SetBackgroundColour(Back);
    this->MCube->StaticText1        ->SetForegroundColour(Forg);
    this->MCube->StaticText2        ->SetForegroundColour(Forg);
    this->MCube->StaticText3        ->SetForegroundColour(Forg);
    this->MCube->StaticText4        ->SetForegroundColour(Forg);
    this->MCube->StaticText5        ->SetForegroundColour(Forg);
    this->MCube->StaticText6        ->SetForegroundColour(Forg);
    this->MCube->StaticText7        ->SetForegroundColour(Forg);
    this->MCube->TextCtrl_Arete     ->SetForegroundColour(Forg);
    this->MCube->TextCtrl_Arete     ->SetBackgroundColour(Gris);
    this->MCube->TextCtrl_X         ->SetForegroundColour(Forg);
    this->MCube->TextCtrl_X         ->SetBackgroundColour(Gris);
    this->MCube->TextCtrl_Y         ->SetForegroundColour(Forg);
    this->MCube->TextCtrl_Y         ->SetBackgroundColour(Gris);
    this->MCube->TextCtrl_Z         ->SetForegroundColour(Forg);
    this->MCube->TextCtrl_Z         ->SetBackgroundColour(Gris);
    this->MCube->SpinCtrl_Groupe    ->SetForegroundColour(Forg);
    this->MCube->SpinCtrl_Groupe    ->SetBackgroundColour(Gris);
    this->MCube->SpinCtrl_Materiau  ->SetForegroundColour(Forg);
    this->MCube->SpinCtrl_Materiau  ->SetBackgroundColour(Gris);
    this->MCube->Button_OK          ->SetForegroundColour(Forg);
    this->MCube->Button_OK          ->SetBackgroundColour(Gris);
    this->MCube->Button_Annuler     ->SetForegroundColour(Forg);
    this->MCube->Button_Annuler     ->SetBackgroundColour(Gris);
    this->MCube->Refresh();

// Cylindre
    this->MCylindre->SetForegroundColour(Forg);
    this->MCylindre->SetBackgroundColour(Back);
    this->MCylindre->StaticText1        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText2        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText3        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText4        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText5        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText6        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText7        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText8        ->SetForegroundColour(Forg);
    this->MCylindre->StaticText9        ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_Arete     ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_Arete     ->SetBackgroundColour(Gris);
    this->MCylindre->TextCtrl_Rayon     ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_Rayon     ->SetBackgroundColour(Gris);
    this->MCylindre->TextCtrl_X         ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_X         ->SetBackgroundColour(Gris);
    this->MCylindre->TextCtrl_Y         ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_Y         ->SetBackgroundColour(Gris);
    this->MCylindre->TextCtrl_Z         ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_Z         ->SetBackgroundColour(Gris);
    this->MCylindre->TextCtrl_NbPoints  ->SetForegroundColour(Forg);
    this->MCylindre->TextCtrl_NbPoints  ->SetBackgroundColour(Gris);
    this->MCylindre->SpinCtrl_Groupe    ->SetForegroundColour(Forg);
    this->MCylindre->SpinCtrl_Groupe    ->SetBackgroundColour(Gris);
    this->MCylindre->SpinCtrl_Materiau  ->SetForegroundColour(Forg);
    this->MCylindre->SpinCtrl_Materiau  ->SetBackgroundColour(Gris);
    this->MCylindre->SpinCtrl_NbSecteurs->SetForegroundColour(Forg);
    this->MCylindre->SpinCtrl_NbSecteurs->SetBackgroundColour(Gris);
    this->MCylindre->CheckBox_FermerCylindre->SetForegroundColour(Forg);
    this->MCylindre->CheckBox_FermerCylindre->SetBackgroundColour(Back);
    this->MCylindre->Button_OK          ->SetForegroundColour(Forg);
    this->MCylindre->Button_OK          ->SetBackgroundColour(Gris);
    this->MCylindre->Button_Annuler     ->SetForegroundColour(Forg);
    this->MCylindre->Button_Annuler     ->SetBackgroundColour(Gris);
    this->MCylindre->Refresh();

// DeplacerBdd
    this->MDeplacer->SetForegroundColour(Forg);
    this->MDeplacer->SetBackgroundColour(Back);
    this->MDeplacer->StaticText1  ->SetForegroundColour(Forg);
    if (darkmode) {
        this->MDeplacer->StaticText4->SetForegroundColour(New_Forg);
     } else {
        this->MDeplacer->StaticText4->SetForegroundColour(Bleu_svg);
     }
    this->MDeplacer->TextCtrl_DeplX ->SetForegroundColour(Forg);
    this->MDeplacer->TextCtrl_DeplX ->SetBackgroundColour(Gris);
    this->MDeplacer->TextCtrl_DeplY ->SetForegroundColour(Forg);
    this->MDeplacer->TextCtrl_DeplY ->SetBackgroundColour(Gris);
    this->MDeplacer->TextCtrl_DeplZ ->SetForegroundColour(Forg);
    this->MDeplacer->TextCtrl_DeplZ ->SetBackgroundColour(Gris);
    this->MDeplacer->Button_Centrer ->SetForegroundColour(Forg);
    this->MDeplacer->Button_Centrer ->SetBackgroundColour(Gris);
    this->MDeplacer->Button_OK      ->SetForegroundColour(Forg);
    this->MDeplacer->Button_OK      ->SetBackgroundColour(Gris);
    this->MDeplacer->Button_Inverser->SetForegroundColour(Forg);
    this->MDeplacer->Button_Inverser->SetBackgroundColour(Gris);
    this->MDeplacer->Button_Quitter ->SetForegroundColour(Forg);
    this->MDeplacer->Button_Quitter ->SetBackgroundColour(Gris);
    this->MDeplacer->Refresh();

// Ellipsoide
    this->MEllips->SetForegroundColour(Forg);
    this->MEllips->SetBackgroundColour(Back);
    this->MEllips->StaticText1          ->SetForegroundColour(Forg);
    this->MEllips->StaticText2          ->SetForegroundColour(Forg);
    this->MEllips->StaticText3          ->SetForegroundColour(Forg);
    this->MEllips->StaticText4          ->SetForegroundColour(Forg);
    this->MEllips->StaticText5          ->SetForegroundColour(Forg);
    this->MEllips->StaticText6          ->SetForegroundColour(Forg);
    this->MEllips->StaticText7          ->SetForegroundColour(Forg);
    this->MEllips->StaticText8          ->SetForegroundColour(Forg);
    this->MEllips->StaticText9          ->SetForegroundColour(Forg);
    this->MEllips->StaticText10         ->SetForegroundColour(Forg);
    this->MEllips->StaticText11         ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_Rayon       ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_Rayon       ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_X           ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_X           ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_Y           ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_Y           ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_Z           ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_Z           ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_NbMeridiens ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_NbMeridiens ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_NbParalleles->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_NbParalleles->SetBackgroundColour(Gris);
    this->MEllips->CheckBox_NewSphere   ->SetForegroundColour(Forg);
    this->MEllips->CheckBox_NewSphere   ->SetBackgroundColour(Back);
    this->MEllips->TextCtrl_CoefX       ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_CoefX       ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_CoefY       ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_CoefY       ->SetBackgroundColour(Gris);
    this->MEllips->TextCtrl_CoefZ       ->SetForegroundColour(Forg);
    this->MEllips->TextCtrl_CoefZ       ->SetBackgroundColour(Gris);
    this->MEllips->SpinCtrl_Groupe      ->SetForegroundColour(Forg);
    this->MEllips->SpinCtrl_Groupe      ->SetBackgroundColour(Gris);
    this->MEllips->SpinCtrl_Materiau    ->SetForegroundColour(Forg);
    this->MEllips->SpinCtrl_Materiau    ->SetBackgroundColour(Gris);
    this->MEllips->Button_OK            ->SetForegroundColour(Forg);
    this->MEllips->Button_OK            ->SetBackgroundColour(Gris);
    this->MEllips->Button_Annuler       ->SetForegroundColour(Forg);
    this->MEllips->Button_Annuler       ->SetBackgroundColour(Gris);
    this->MEllips->Refresh();

// Facette
    this->MFacet->SetForegroundColour(Forg);
    this->MFacet->SetBackgroundColour(Back);
    this->MFacet->StaticText1       ->SetForegroundColour(Forg);
    this->MFacet->StaticText2       ->SetForegroundColour(Forg);
    this->MFacet->StaticText3       ->SetForegroundColour(Forg);
    this->MFacet->StaticText4       ->SetForegroundColour(Forg);
    this->MFacet->StaticText5       ->SetForegroundColour(Forg);
    this->MFacet->StaticText6       ->SetForegroundColour(Forg);
    this->MFacet->StaticText7       ->SetForegroundColour(Forg);
    this->MFacet->StaticText8       ->SetForegroundColour(Forg);
    this->MFacet->StaticText9       ->SetForegroundColour(Forg);
    this->MFacet->StaticText10      ->SetForegroundColour(Forg);
    this->MFacet->StaticText11      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P1X      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P1X      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P1Y      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P1Y      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P1Z      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P1Z      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P2X      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P2X      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P2Y      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P2Y      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P2Z      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P2Z      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P3X      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P3X      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P3Y      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P3Y      ->SetBackgroundColour(Gris);
    this->MFacet->TextCtrl_P3Z      ->SetForegroundColour(Forg);
    this->MFacet->TextCtrl_P3Z      ->SetBackgroundColour(Gris);
    this->MFacet->SpinCtrl_Groupe   ->SetForegroundColour(Forg);
    this->MFacet->SpinCtrl_Groupe   ->SetBackgroundColour(Gris);
    this->MFacet->SpinCtrl_Materiau ->SetForegroundColour(Forg);
    this->MFacet->SpinCtrl_Materiau ->SetBackgroundColour(Gris);
    this->MFacet->Button_OK         ->SetForegroundColour(Forg);
    this->MFacet->Button_OK         ->SetBackgroundColour(Gris);
    this->MFacet->Button_Annuler    ->SetForegroundColour(Forg);
    this->MFacet->Button_Annuler    ->SetBackgroundColour(Gris);
    this->MFacet->Refresh();

// Icosaedre
    this->MIcosa->SetForegroundColour(Forg);
    this->MIcosa->SetBackgroundColour(Back);
    this->MIcosa->StaticText1       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText2       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText3       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText4       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText5       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText7       ->SetForegroundColour(Forg);
    this->MIcosa->StaticText8       ->SetForegroundColour(Forg);
    this->MIcosa->TextCtrl_X        ->SetForegroundColour(Forg);
    this->MIcosa->TextCtrl_X        ->SetBackgroundColour(Gris);
    this->MIcosa->TextCtrl_Y        ->SetForegroundColour(Forg);
    this->MIcosa->TextCtrl_Y        ->SetBackgroundColour(Gris);
    this->MIcosa->TextCtrl_Z        ->SetForegroundColour(Forg);
    this->MIcosa->TextCtrl_Z        ->SetBackgroundColour(Gris);
    this->MIcosa->TextCtrl_Rayon    ->SetForegroundColour(Forg);
    this->MIcosa->TextCtrl_Rayon    ->SetBackgroundColour(Gris);
    this->MIcosa->SpinCtrl_Subdiv   ->SetForegroundColour(Forg);
    this->MIcosa->SpinCtrl_Subdiv   ->SetBackgroundColour(Gris);
    this->MIcosa->SpinCtrl_Groupe   ->SetForegroundColour(Forg);
    this->MIcosa->SpinCtrl_Groupe   ->SetBackgroundColour(Gris);
    this->MIcosa->SpinCtrl_Materiau ->SetForegroundColour(Forg);
    this->MIcosa->SpinCtrl_Materiau ->SetBackgroundColour(Gris);
    this->MIcosa->Button_OK         ->SetForegroundColour(Forg);
    this->MIcosa->Button_OK         ->SetBackgroundColour(Gris);
    this->MIcosa->Button_Annuler    ->SetForegroundColour(Forg);
    this->MIcosa->Button_Annuler    ->SetBackgroundColour(Gris);
    this->MIcosa->Refresh();

// ManipulationsPanel
    this->MManip->SetForegroundColour(Forg);
    this->MManip->SetBackgroundColour(Back);
    this->MManip->Button_Creer      ->SetForegroundColour(Forg);
    this->MManip->Button_Creer      ->SetBackgroundColour(Gris);
    this->MManip->Button_Quitter    ->SetForegroundColour(Forg);
    this->MManip->Button_Quitter    ->SetBackgroundColour(Gris);
    this->MManip->Button_Raz        ->SetForegroundColour(Forg);
    this->MManip->Button_Raz        ->SetBackgroundColour(Gris);
    this->MManip->Button_Rotation   ->SetForegroundColour(Forg);
    this->MManip->Button_Rotation   ->SetBackgroundColour(Gris);
    this->MManip->Button_Translation->SetForegroundColour(Forg);
    this->MManip->Button_Translation->SetBackgroundColour(Gris);
    this->MManip->Button_Scale      ->SetForegroundColour(Forg);
    this->MManip->Button_Scale      ->SetBackgroundColour(Gris);
    this->MManip->Button_NewObjet   ->SetForegroundColour(Forg);
    this->MManip->Button_NewObjet   ->SetBackgroundColour(Gris);
    if (darkmode) {
        this->MManip->StaticText1   ->SetForegroundColour(New_Back);
        this->MManip->StaticText1   ->SetBackgroundColour(New_Forg);
        this->MManip->StaticText2   ->SetForegroundColour(New_Back);
        this->MManip->StaticText2   ->SetBackgroundColour(New_Forg);
        this->MManip->CheckBox_Z    ->SetForegroundColour(New_Forg);
    } else {
        this->MManip->StaticText1   ->SetForegroundColour(Blanc);
        this->MManip->StaticText1   ->SetBackgroundColour(Noir);
        this->MManip->StaticText2   ->SetForegroundColour(Blanc);
        this->MManip->StaticText2   ->SetBackgroundColour(Noir);
        this->MManip->CheckBox_Z    ->SetForegroundColour(Bleu_svg);
    }
    this->MManip->Refresh();

// ModificationPanel
    this->MPanel->SetForegroundColour(Forg);
    this->MPanel->SetBackgroundColour(Back);
    this->MPanel->CheckBox_FacettePlane ->SetForegroundColour(Forg);
    this->MPanel->CheckBox_NotFlat      ->SetForegroundColour(Forg);
    this->MPanel->CheckBox_Transparence ->SetForegroundColour(Forg);

    if (darkmode) {
        this->MPanel->StaticText1   ->SetForegroundColour(New_Back);
        this->MPanel->StaticText1   ->SetBackgroundColour(New_Forg);
        this->MPanel->StaticText3   ->SetForegroundColour(New_Back);
        this->MPanel->StaticText3   ->SetBackgroundColour(New_Forg);
        this->MPanel->StaticText6   ->SetForegroundColour(New_Back);
        this->MPanel->StaticText6   ->SetBackgroundColour(New_Forg);
        this->MPanel->StaticText7   ->SetForegroundColour(New_Back);
        this->MPanel->StaticText7   ->SetBackgroundColour(New_Forg);
        this->MPanel->Panel9        ->SetBackgroundColour(New_Forg);
    } else {
        this->MPanel->StaticText1   ->SetForegroundColour(Blanc);
        this->MPanel->StaticText1   ->SetBackgroundColour(Noir);
        this->MPanel->StaticText3   ->SetForegroundColour(Blanc);
        this->MPanel->StaticText3   ->SetBackgroundColour(Noir);
        this->MPanel->StaticText6   ->SetForegroundColour(Blanc);
        this->MPanel->StaticText6   ->SetBackgroundColour(Noir);
        this->MPanel->StaticText7   ->SetForegroundColour(Blanc);
        this->MPanel->StaticText7   ->SetBackgroundColour(Noir);
        this->MPanel->Panel9        ->SetBackgroundColour(Noir);
    }
    this->MPanel->StaticText2               ->SetForegroundColour(Forg);
    this->MPanel->StaticText4               ->SetForegroundColour(Forg);
    this->MPanel->StaticText5               ->SetForegroundColour(Forg);
    this->MPanel->StaticText8               ->SetForegroundColour(Forg);
    this->MPanel->TextCtrl_Tolerance        ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Ajouter      ->SetBackgroundColour(Gris);
    this->MPanel->ToggleButton_Ajouter      ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_CreerFacette ->SetBackgroundColour(Gris);
    this->MPanel->ToggleButton_CreerFacette ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Diviser      ->SetBackgroundColour(Gris);
    this->MPanel->ToggleButton_Diviser      ->SetForegroundColour(Forg);
    this->MPanel->ToggleButton_Souder       ->SetBackgroundColour(Gris);
    this->MPanel->ToggleButton_Souder       ->SetForegroundColour(Forg);
    this->MPanel->Button_Annuler            ->SetBackgroundColour(Gris);
    this->MPanel->Button_Annuler            ->SetForegroundColour(Forg);
    this->MPanel->Button_InverserNormale    ->SetBackgroundColour(Gris);
    this->MPanel->Button_InverserNormale    ->SetForegroundColour(Forg);
    this->MPanel->Button_InverserTout       ->SetBackgroundColour(Gris);
    this->MPanel->Button_InverserTout       ->SetForegroundColour(Forg);
    this->MPanel->Button_RecalculerAretes   ->SetBackgroundColour(Gris);
    this->MPanel->Button_RecalculerAretes   ->SetForegroundColour(Forg);
    this->MPanel->Button_RecalculerNormales ->SetBackgroundColour(Gris);
    this->MPanel->Button_RecalculerNormales ->SetForegroundColour(Forg);
    this->MPanel->Button_Simplification     ->SetBackgroundColour(Gris);
    this->MPanel->Button_Simplification     ->SetForegroundColour(Forg);
    this->MPanel->Button_SupprimerFacette   ->SetBackgroundColour(Gris);
    this->MPanel->Button_SupprimerFacette   ->SetForegroundColour(Forg);
    this->MPanel->Button_Trianguler         ->SetBackgroundColour(Gris);
    this->MPanel->Button_Trianguler         ->SetForegroundColour(Forg);
    this->MPanel->Button_Undo               ->SetBackgroundColour(Gris);
    this->MPanel->Button_Undo               ->SetForegroundColour(Forg);
    this->MPanel->Button_Quitter            ->SetBackgroundColour(Gris);
    this->MPanel->Button_Quitter            ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_Groupe           ->SetBackgroundColour(Gris);
    this->MPanel->SpinCtrl_Groupe           ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_Materiau         ->SetBackgroundColour(Gris);
    this->MPanel->SpinCtrl_Materiau         ->SetForegroundColour(Forg);
    this->MPanel->SpinCtrl_NbSegments       ->SetBackgroundColour(Gris);
    this->MPanel->SpinCtrl_NbSegments       ->SetForegroundColour(Forg);
    this->MPanel->StaticLine1               ->SetForegroundColour(Forg);    // Pas d'effet ?
    this->MPanel->StaticLine1               ->SetBackgroundColour(Back);

    this->MPanel->TextCtrl_Tolerance        ->SetBackgroundColour(Gris);
    this->MPanel->CheckBox_FacettePlane     ->SetBackgroundColour(Back);
    this->MPanel->CheckBox_NotFlat          ->SetBackgroundColour(Back);
    this->MPanel->CheckBox_Transparence     ->SetBackgroundColour(Back);

    this->MPanel->Refresh();

// PositionObs_AzimutSite
    this->MPosObs->SetForegroundColour(Forg);
    this->MPosObs->SetBackgroundColour(Back);
    this->MPosObs->StaticText1      ->SetForegroundColour(Forg);
    this->MPosObs->StaticText2      ->SetForegroundColour(Forg);
    this->MPosObs->StaticText3      ->SetForegroundColour(Forg);
    this->MPosObs->StaticText4      ->SetForegroundColour(Forg);
    this->MPosObs->StaticText5      ->SetForegroundColour(Forg);
    this->MPosObs->SpinCtrl_LAZ     ->SetBackgroundColour(Gris);
    this->MPosObs->SpinCtrl_LAZ     ->SetForegroundColour(Forg);
    this->MPosObs->SpinCtrl_LSI     ->SetBackgroundColour(Gris);
    this->MPosObs->SpinCtrl_LSI     ->SetForegroundColour(Forg);
    this->MPosObs->Button_Quit      ->SetForegroundColour(Forg);
    this->MPosObs->Button_Quit      ->SetBackgroundColour(Gris);
//    this->MPosObs->StaticText_Warn->SetForegroundColour(Forg);    // Ne pas changer, reste en rouge
    this->MPosObs->Refresh();

// PositionSource
    this->MPosLight->SetForegroundColour(Forg);
    this->MPosLight->SetBackgroundColour(Back);
    this->MPosLight->StaticText1->SetForegroundColour(Forg);
    this->MPosLight->Pos_W      ->SetForegroundColour(Forg);
    if (darkmode) {
        this->MPosLight->Pos_Z  ->SetForegroundColour(*wxCYAN); // Couleur bleue initiale trop foncée en darkmode
    } else {
        this->MPosLight->Pos_Z  ->SetForegroundColour(Bleu_svg);
    }
    this->MPosLight->SpinCtrlDouble_PosX->SetBackgroundColour(Gris);
    this->MPosLight->SpinCtrlDouble_PosX->SetForegroundColour(Forg);
    this->MPosLight->SpinCtrlDouble_PosY->SetBackgroundColour(Gris);
    this->MPosLight->SpinCtrlDouble_PosY->SetForegroundColour(Forg);
    this->MPosLight->SpinCtrlDouble_PosZ->SetBackgroundColour(Gris);
    this->MPosLight->SpinCtrlDouble_PosZ->SetForegroundColour(Forg);
    this->MPosLight->SpinCtrlDouble_PosW->SetBackgroundColour(Gris);
    this->MPosLight->SpinCtrlDouble_PosW->SetForegroundColour(Forg);
    this->MPosLight->Button_Quitter     ->SetForegroundColour(Forg);
    this->MPosLight->Button_Quitter     ->SetBackgroundColour(Gris);
    this->MPosLight->Button_Defaut      ->SetForegroundColour(Forg);
    this->MPosLight->Button_Defaut      ->SetBackgroundColour(Gris);
    this->MPosLight->Refresh();

// Prefs_Dialog
    this->MPrefs->SetForegroundColour(Forg);
    this->MPrefs->SetBackgroundColour(Back);
    this->MPrefs->StaticText1           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText2           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText3           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText4           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText5           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText7           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText8           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText9           ->SetForegroundColour(Forg);
    this->MPrefs->StaticText_Gouraud    ->SetForegroundColour(Forg);
    this->MPrefs->StaticText_Gouraud2   ->SetForegroundColour(Forg);
    this->MPrefs->TextCtrl_WorkDir      ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_axes   ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_norm   ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_src            ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud   ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud2  ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrl_PasSvg           ->SetForegroundColour(Forg);
    this->MPrefs->SpinCtrl_Threads          ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_1SeulObjet3D     ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_LectureOptimisee ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_TestDecalage3DS  ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_Seuillage        ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_AntialiasingSoft ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_CalculNormales   ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_DisplayFps       ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_NotFlat          ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_TraiterDoublonsAretes->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_RecNormales_Seuillees->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_CreerBackup      ->SetForegroundColour(Forg);
    this->MPrefs->CheckBox_SupprBackup      ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_Trackball        ->SetForegroundColour(Forg);    // Les 2 choix restent en noir => On ne peut pas les lire ! BUG signalé
    this->MPrefs->RadioBox_Triangulation    ->SetForegroundColour(Forg);    // Les 3 choix restent en noir => Idem
    this->MPrefs->RadioBox_IconSize         ->SetForegroundColour(Forg);
    this->MPrefs->RadioBox_DarkMode         ->SetForegroundColour(Forg);
    this->MPrefs->Button_tmp_rep            ->SetForegroundColour(Forg);
    this->MPrefs->Button_tmp_rep            ->SetBackgroundColour(Gris);
    this->MPrefs->Button_Reset              ->SetForegroundColour(Forg);
    this->MPrefs->Button_Reset              ->SetBackgroundColour(Gris);
    this->MPrefs->Button_OK                 ->SetBackgroundColour(Gris);
    this->MPrefs->Button_OK                 ->SetForegroundColour(Forg);

    this->MPrefs->TextCtrl_WorkDir              ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrlDouble_axes           ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrlDouble_norm           ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrlDouble_src            ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud   ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrlDouble_SeuilGouraud2  ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrl_PasSvg               ->SetBackgroundColour(Gris);
    this->MPrefs->SpinCtrl_Threads              ->SetBackgroundColour(Gris);
    this->MPrefs->CheckBox_1SeulObjet3D         ->SetBackgroundColour(Back);
    this->MPrefs->CheckBox_AntialiasingSoft     ->SetBackgroundColour(Back);
    this->MPrefs->CheckBox_CalculNormales       ->SetBackgroundColour(Back);
    this->MPrefs->CheckBox_CreerBackup          ->SetBackgroundColour(Back);
    this->MPrefs->CheckBox_SupprBackup          ->SetBackgroundColour(Back);
    this->MPrefs->RadioBox_Trackball            ->SetBackgroundColour(Back);
    this->MPrefs->RadioBox_Triangulation        ->SetBackgroundColour(Back);
    this->MPrefs->RadioBox_IconSize             ->SetBackgroundColour(Back);
    this->MPrefs->RadioBox_DarkMode             ->SetBackgroundColour(Back);
    this->MPrefs->Refresh();

// PropertiesPanel
    this->MProps->SetForegroundColour(Forg);
    this->MProps->SetBackgroundColour(Back);
    if (darkmode) {
        this->MProps->StaticText1       ->SetForegroundColour(New_Back);
        this->MProps->StaticText1       ->SetBackgroundColour(New_Forg);
        this->MProps->StaticTextNomBdd  ->SetForegroundColour(New_Back);
        this->MProps->StaticTextNomBdd  ->SetBackgroundColour(New_Forg);
        this->MProps->StaticText8       ->SetForegroundColour(New_Back);
        this->MProps->StaticText8       ->SetBackgroundColour(New_Forg);
        this->MProps->StaticText14      ->SetForegroundColour(New_Back);
        this->MProps->StaticText14      ->SetBackgroundColour(New_Forg);
    } else {
        this->MProps->StaticText1       ->SetForegroundColour(Blanc);
        this->MProps->StaticText1       ->SetBackgroundColour(Noir);
        this->MProps->StaticTextNomBdd  ->SetForegroundColour(Blanc);
        this->MProps->StaticTextNomBdd  ->SetBackgroundColour(Noir);
        this->MProps->StaticText8       ->SetForegroundColour(Blanc);
        this->MProps->StaticText8       ->SetBackgroundColour(Noir);
        this->MProps->StaticText14      ->SetForegroundColour(Blanc);
        this->MProps->StaticText14      ->SetBackgroundColour(Noir);
    }
    this->MProps->StaticText2   ->SetForegroundColour(Forg);
    this->MProps->StaticText3   ->SetForegroundColour(Forg);
    this->MProps->StaticText4   ->SetForegroundColour(Forg);
    this->MProps->StaticText5   ->SetForegroundColour(Forg);
    this->MProps->StaticText6   ->SetForegroundColour(Forg);
    this->MProps->StaticText7   ->SetForegroundColour(Forg);
    this->MProps->StaticText9   ->SetForegroundColour(Forg);
    this->MProps->StaticText10  ->SetForegroundColour(Forg);
    this->MProps->StaticText11  ->SetForegroundColour(Forg);
    this->MProps->StaticText12  ->SetForegroundColour(Forg);
    this->MProps->StaticText13  ->SetForegroundColour(Forg);
    this->MProps->Texte_Box     ->SetForegroundColour(Forg);
    this->MProps->nb_objets     ->SetForegroundColour(Forg);
    this->MProps->nb_points     ->SetForegroundColour(Forg);
    this->MProps->nb_facettes   ->SetForegroundColour(Forg);
    this->MProps->nb_aretes     ->SetForegroundColour(Forg);
    this->MProps->nb_groupes    ->SetForegroundColour(Forg);
    this->MProps->nb_materiaux  ->SetForegroundColour(Forg);
    this->MProps->nb_3Points    ->SetForegroundColour(Forg);
    this->MProps->nb_4Points    ->SetForegroundColour(Forg);
    this->MProps->nb_maxPoints  ->SetForegroundColour(Forg);
    this->MProps->numero_facette->SetForegroundColour(Forg);
    this->MProps->numero_objet  ->SetForegroundColour(Forg);
    this->MProps->BoutonOK      ->SetForegroundColour(Forg);
    this->MProps->BoutonOK      ->SetBackgroundColour(Gris);
    this->MProps->Refresh();

// ReperageFacette
    this->MRepFacet->SetForegroundColour(Forg);
    this->MRepFacet->SetBackgroundColour(Back);
    this->MRepFacet->StaticText1    ->SetForegroundColour(Forg);
    this->MRepFacet->StaticText2    ->SetForegroundColour(Forg);
    this->MRepFacet->StaticText3    ->SetForegroundColour(Forg);
    this->MRepFacet->StaticText4    ->SetForegroundColour(Forg);
//    this->MRepFacet->StaticText_Warning->SetForegroundColour(Forg);   // Laisser tel que
    this->MRepFacet->Text_NomObjet  ->SetForegroundColour(Forg);
    this->MRepFacet->Text_NomObjet  ->SetBackgroundColour(Gris);
    this->MRepFacet->Text_NbSommets ->SetForegroundColour(Forg);
    this->MRepFacet->Text_NbSommets ->SetBackgroundColour(Gris);
    this->MRepFacet->Text_NumeroObjet   ->SetForegroundColour(Forg);
    this->MRepFacet->Text_NumeroObjet   ->SetBackgroundColour(Gris);
    this->MRepFacet->SpinCtrl_IndiceFacette ->SetForegroundColour(Forg);
    this->MRepFacet->SpinCtrl_IndiceFacette ->SetBackgroundColour(Gris);
    this->MRepFacet->SpinCtrl_IndiceObjet   ->SetForegroundColour(Forg);
    this->MRepFacet->SpinCtrl_IndiceObjet   ->SetBackgroundColour(Gris);
    this->MRepFacet->SpinCtrl_IndiceSommet  ->SetForegroundColour(Forg);
    this->MRepFacet->SpinCtrl_IndiceSommet  ->SetBackgroundColour(Gris);
    this->MRepFacet->CheckBox_Laisser       ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_Laisser       ->SetBackgroundColour(Back);
    this->MRepFacet->CheckBox_VisuNormale   ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuNormale   ->SetBackgroundColour(Back);
    this->MRepFacet->CheckBox_VisuSommets   ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuSommets   ->SetBackgroundColour(Back);
    this->MRepFacet->CheckBox_VisuNormales_Sommets  ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_VisuNormales_Sommets  ->SetBackgroundColour(Back);
    this->MRepFacet->CheckBox_Laisser   ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_Laisser   ->SetBackgroundColour(Back);
    this->MRepFacet->CheckBox_Laisser   ->SetForegroundColour(Forg);
    this->MRepFacet->CheckBox_Laisser   ->SetBackgroundColour(Back);
    this->MRepFacet->Button_InvNormale  ->SetForegroundColour(Forg);
    this->MRepFacet->Button_InvNormale  ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_DelFacette  ->SetForegroundColour(Forg);
    this->MRepFacet->Button_DelFacette  ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_CentrerRotation ->SetForegroundColour(Forg);
    this->MRepFacet->Button_CentrerRotation ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_UndoDel ->SetForegroundColour(Forg);
    this->MRepFacet->Button_UndoDel ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_Masquer ->SetForegroundColour(Forg);
    this->MRepFacet->Button_Masquer ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_Permuter->SetForegroundColour(Forg);
    this->MRepFacet->Button_Permuter->SetBackgroundColour(Gris);
    this->MRepFacet->Button_Reset   ->SetForegroundColour(Forg);
    this->MRepFacet->Button_Reset   ->SetBackgroundColour(Gris);
    this->MRepFacet->Button_Quitter ->SetForegroundColour(Forg);
    this->MRepFacet->Button_Quitter ->SetBackgroundColour(Gris);
    this->MRepFacet->Refresh();

// ReperageGroupe
    this->MRepGrp->SetForegroundColour(Forg);
    this->MRepGrp->SetBackgroundColour(Back);
    this->MRepGrp->StaticText1->SetForegroundColour(Forg);
    this->MRepGrp->TextCtrl1  ->SetForegroundColour(Forg);
    this->MRepGrp->TextCtrl1  ->SetBackgroundColour(Gris);
    this->MRepGrp->SpinButton1->SetForegroundColour(Forg);
    this->MRepGrp->SpinButton1->SetBackgroundColour(Gris);
    this->MRepGrp->Button_OK  ->SetForegroundColour(Forg);
    this->MRepGrp->Button_OK  ->SetBackgroundColour(Gris);
    this->MRepGrp->Button_Quit->SetForegroundColour(Forg);
    this->MRepGrp->Button_Quit->SetBackgroundColour(Gris);
    this->MRepGrp->Refresh();

// ReperageMateriau
    this->MRepMat->SetForegroundColour(Forg);
    this->MRepMat->SetBackgroundColour(Back);
    this->MRepMat->StaticText1->SetForegroundColour(Forg);
    this->MRepMat->TextCtrl1  ->SetForegroundColour(Forg);
    this->MRepMat->TextCtrl1  ->SetBackgroundColour(Gris);
    this->MRepMat->SpinButton1->SetForegroundColour(Forg);
    this->MRepMat->SpinButton1->SetBackgroundColour(Gris);
    this->MRepMat->Button_OK  ->SetForegroundColour(Forg);
    this->MRepMat->Button_OK  ->SetBackgroundColour(Gris);
    this->MRepMat->Button_Quit->SetForegroundColour(Forg);
    this->MRepMat->Button_Quit->SetBackgroundColour(Gris);
    this->MRepMat->Refresh();

// ReperageObjet
    this->MRepObj->SetForegroundColour(Forg);
    this->MRepObj->SetBackgroundColour(Back);
    this->MRepObj->StaticText1            ->SetForegroundColour(Forg);
    this->MRepObj->StaticText2            ->SetForegroundColour(Forg);
    this->MRepObj->TextCtrl_NumObjet      ->SetForegroundColour(Forg);
    this->MRepObj->TextCtrl_NumObjet      ->SetBackgroundColour(Gris);
    this->MRepObj->TextCtrl_NomObjet      ->SetForegroundColour(Forg);
    this->MRepObj->TextCtrl_NomObjet      ->SetBackgroundColour(Gris);
    this->MRepObj->TextCtrl_indice        ->SetForegroundColour(Forg);
    this->MRepObj->TextCtrl_indice        ->SetBackgroundColour(Gris);
    this->MRepObj->SpinButton_indice      ->SetForegroundColour(Forg);
    this->MRepObj->SpinButton_indice      ->SetBackgroundColour(Gris);
    this->MRepObj->CheckBox_renommer      ->SetForegroundColour(Forg);
    this->MRepObj->CheckBox_renommer      ->SetBackgroundColour(Back);
    this->MRepObj->CheckBox_masquer       ->SetForegroundColour(Forg);
    this->MRepObj->CheckBox_masquer       ->SetBackgroundColour(Back);
    this->MRepObj->CheckBox_supprimer     ->SetForegroundColour(Forg);
    this->MRepObj->CheckBox_supprimer     ->SetBackgroundColour(Back);
    this->MRepObj->Button_renommer        ->SetForegroundColour(Forg);
    this->MRepObj->Button_renommer        ->SetBackgroundColour(Gris);
    this->MRepObj->Button_OK              ->SetForegroundColour(Forg);
    this->MRepObj->Button_OK              ->SetBackgroundColour(Gris);
    this->MRepObj->Button_InverserNormales->SetForegroundColour(Forg);
    this->MRepObj->Button_InverserNormales->SetBackgroundColour(Gris);
    this->MRepObj->Refresh();

// ReperagePoint
    this->MRepPoint->SetForegroundColour(Forg);
    this->MRepPoint->SetBackgroundColour(Back);
    this->MRepPoint->StaticText1        ->SetForegroundColour(Forg);
    this->MRepPoint->StaticText2        ->SetForegroundColour(Forg);
    this->MRepPoint->StaticText3        ->SetForegroundColour(Forg);
    this->MRepPoint->StaticText4        ->SetForegroundColour(Forg);
    this->MRepPoint->StaticText5        ->SetForegroundColour(Forg);
    this->MRepPoint->StaticText6        ->SetForegroundColour(Forg);
    this->MRepPoint->Text_NomObjet      ->SetForegroundColour(Forg);
    this->MRepPoint->Text_NomObjet      ->SetBackgroundColour(Gris);
    this->MRepPoint->Text_NumeroObjet   ->SetForegroundColour(Forg);
    this->MRepPoint->Text_NumeroObjet   ->SetBackgroundColour(Back);
    this->MRepPoint->Text_ValeurX       ->SetForegroundColour(Forg);
    this->MRepPoint->Text_ValeurX       ->SetBackgroundColour(Gris);
    this->MRepPoint->Text_ValeurY       ->SetForegroundColour(Forg);
    this->MRepPoint->Text_ValeurY       ->SetBackgroundColour(Gris);
    this->MRepPoint->Text_ValeurZ       ->SetForegroundColour(Forg);
    this->MRepPoint->Text_ValeurZ       ->SetBackgroundColour(Gris);
    this->MRepPoint->CheckBox_X         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_X         ->SetBackgroundColour(Back);
    this->MRepPoint->CheckBox_Y         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Y         ->SetBackgroundColour(Back);
    this->MRepPoint->CheckBox_Z         ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Z         ->SetBackgroundColour(Back);
    this->MRepPoint->CheckBox_Laisser   ->SetForegroundColour(Forg);
    this->MRepPoint->CheckBox_Laisser   ->SetBackgroundColour(Back);
    this->MRepPoint->Text_NumeroObjet   ->SetForegroundColour(Forg);
    this->MRepPoint->Text_NumeroObjet   ->SetBackgroundColour(Gris);
    this->MRepPoint->Text_NomObjet      ->SetForegroundColour(Forg);
    this->MRepPoint->Text_NomObjet      ->SetBackgroundColour(Gris);
    this->MRepPoint->SpinCtrl_IndiceObjet   ->SetForegroundColour(Forg);
    this->MRepPoint->SpinCtrl_IndiceObjet   ->SetBackgroundColour(Gris);
    this->MRepPoint->SpinCtrl_IndicePoint   ->SetForegroundColour(Forg);
    this->MRepPoint->SpinCtrl_IndicePoint   ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_ModifierX   ->SetForegroundColour(Forg);
    this->MRepPoint->Button_ModifierX   ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_ModifierY   ->SetForegroundColour(Forg);
    this->MRepPoint->Button_ModifierY   ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_ModifierZ   ->SetForegroundColour(Forg);
    this->MRepPoint->Button_ModifierZ   ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_UndoX       ->SetForegroundColour(Forg);
    this->MRepPoint->Button_UndoX       ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_UndoY       ->SetForegroundColour(Forg);
    this->MRepPoint->Button_UndoY       ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_UndoZ       ->SetForegroundColour(Forg);
    this->MRepPoint->Button_UndoZ       ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_Centrer     ->SetForegroundColour(Forg);
    this->MRepPoint->Button_Centrer     ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_Reset       ->SetForegroundColour(Forg);
    this->MRepPoint->Button_Reset       ->SetBackgroundColour(Gris);
    this->MRepPoint->Button_Quitter     ->SetForegroundColour(Forg);
    this->MRepPoint->Button_Quitter     ->SetBackgroundColour(Gris);
    this->MRepPoint->Refresh();

// RotationPanel
    this->MRotation->SetForegroundColour(Forg);
    this->MRotation->SetBackgroundColour(Back);
    this->MRotation->StaticText1        ->SetForegroundColour(Forg);
    this->MRotation->StaticText2        ->SetForegroundColour(Forg);
    this->MRotation->StaticText3        ->SetForegroundColour(Forg);
    this->MRotation->StaticText4        ->SetForegroundColour(Forg);
    this->MRotation->StaticText5        ->SetForegroundColour(Forg);
    this->MRotation->StaticText6        ->SetForegroundColour(Forg);
    this->MRotation->StaticText7        ->SetForegroundColour(Forg);
    this->MRotation->RadioBox_Centre    ->SetForegroundColour(Forg);
    this->MRotation->RadioBox_Centre    ->SetBackgroundColour(Back);
    this->MRotation->TextCtrl_PasAngulaire->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_PasAngulaire->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_AngleX    ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_AngleX    ->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_AngleY    ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_AngleY    ->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_AngleZ    ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_AngleZ    ->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_X         ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_X         ->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_Y         ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_Y         ->SetBackgroundColour(Gris);
    this->MRotation->TextCtrl_Z         ->SetForegroundColour(Forg);
    this->MRotation->TextCtrl_Z         ->SetBackgroundColour(Gris);
    this->MRotation->Button_Annuler     ->SetForegroundColour(Forg);
    this->MRotation->Button_Annuler     ->SetBackgroundColour(Gris);
    this->MRotation->Button_Appliquer   ->SetForegroundColour(Forg);
    this->MRotation->Button_Appliquer   ->SetBackgroundColour(Gris);
    this->MRotation->Button_ValiderPas  ->SetForegroundColour(Forg);
    this->MRotation->Button_ValiderPas  ->SetBackgroundColour(Gris);
    this->MRotation->Button_ValiderPoint->SetForegroundColour(Forg);
    this->MRotation->Button_ValiderPoint->SetBackgroundColour(Gris);
    this->MRotation->Refresh();

// ScalePanel
    this->MScale->SetForegroundColour(Forg);
    this->MScale->SetBackgroundColour(Back);
    this->MScale->StaticText1       ->SetForegroundColour(Forg);
    this->MScale->StaticText2       ->SetForegroundColour(Forg);
    this->MScale->StaticText3       ->SetForegroundColour(Forg);
    this->MScale->StaticText4       ->SetForegroundColour(Forg);
    this->MScale->TextCtrl_Increment->SetForegroundColour(Forg);
    this->MScale->TextCtrl_Increment->SetBackgroundColour(Gris);
    this->MScale->TextCtrl_ScaleX   ->SetForegroundColour(Forg);
    this->MScale->TextCtrl_ScaleX   ->SetBackgroundColour(Gris);
    this->MScale->TextCtrl_ScaleY   ->SetForegroundColour(Forg);
    this->MScale->TextCtrl_ScaleY   ->SetBackgroundColour(Gris);
    this->MScale->TextCtrl_ScaleZ   ->SetForegroundColour(Forg);
    this->MScale->TextCtrl_ScaleZ   ->SetBackgroundColour(Gris);
    this->MScale->SpinButton_X      ->SetForegroundColour(Forg);
    this->MScale->SpinButton_X      ->SetBackgroundColour(Gris);
    this->MScale->SpinButton_Y      ->SetForegroundColour(Forg);
    this->MScale->SpinButton_Y      ->SetBackgroundColour(Gris);
    this->MScale->SpinButton_Z      ->SetForegroundColour(Forg);
    this->MScale->SpinButton_Z      ->SetBackgroundColour(Gris);
    this->MScale->CheckBox_ScaleUnique->SetForegroundColour(Forg);
    this->MScale->CheckBox_ScaleUnique->SetBackgroundColour(Back);
    this->MScale->Button_Valider    ->SetForegroundColour(Forg);
    this->MScale->Button_Valider    ->SetBackgroundColour(Gris);
    this->MScale->Button_Appliquer  ->SetForegroundColour(Forg);
    this->MScale->Button_Appliquer  ->SetBackgroundColour(Gris);
    this->MScale->Button_Annuler    ->SetForegroundColour(Forg);
    this->MScale->Button_Annuler    ->SetBackgroundColour(Gris);
    this->MScale->Refresh();

// SelectionPanel
    this->MSelect->SetForegroundColour(Forg);
    this->MSelect->SetBackgroundColour(Back);
    if (darkmode) {
        this->MSelect->StaticText1              ->SetForegroundColour(New_Back);    // <=> *wxBLACK (en fait sur ces items, Forg/Back inversés)
        this->MSelect->StaticText1              ->SetBackgroundColour(New_Forg);    // <=> *wxWHITE
        this->MSelect->StaticText2              ->SetForegroundColour(New_Back);
        this->MSelect->StaticText2              ->SetBackgroundColour(New_Forg);
        this->MSelect->StaticText9              ->SetForegroundColour(New_Back);
        this->MSelect->StaticText9              ->SetBackgroundColour(New_Forg);
        this->MSelect->StaticText_TypeSelection ->SetForegroundColour(New_Back);
        this->MSelect->StaticText_TypeSelection ->SetBackgroundColour(New_Forg);
    } else {
        this->MSelect->StaticText1              ->SetForegroundColour(Blanc);
        this->MSelect->StaticText1              ->SetBackgroundColour(Noir);
        this->MSelect->StaticText2              ->SetForegroundColour(Blanc);
        this->MSelect->StaticText2              ->SetBackgroundColour(Noir);
        this->MSelect->StaticText9              ->SetForegroundColour(Blanc);
        this->MSelect->StaticText9              ->SetBackgroundColour(Noir);
        this->MSelect->StaticText_TypeSelection ->SetForegroundColour(Blanc);
        this->MSelect->StaticText_TypeSelection ->SetBackgroundColour(Noir);
    }
    this->MSelect->StaticText3                  ->SetForegroundColour(Forg);
    this->MSelect->StaticText4                  ->SetForegroundColour(Forg);
    this->MSelect->StaticText_Fac               ->SetForegroundColour(Forg);
    this->MSelect->StaticText_Selection         ->SetForegroundColour(Forg);
    this->MSelect->StaticText_Grp               ->SetForegroundColour(Forg);
    this->MSelect->StaticText_Mat               ->SetForegroundColour(Forg);
    this->MSelect->StaticText_NumerosUtilises   ->SetForegroundColour(Forg);
    this->MSelect->StaticText_Changer           ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Selection_Points     ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Selection_Facettes   ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Selection_Objets     ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Both   ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Avant  ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_TypeSelection_Arriere->SetForegroundColour(Forg);
    this->MSelect->CheckBox_ForcerFlat              ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Grp                  ->SetForegroundColour(Forg);
    this->MSelect->RadioButton_Mat                  ->SetForegroundColour(Forg);
    this->MSelect->Button_SelectionManuelleFacettes ->SetBackgroundColour(Gris);
    this->MSelect->Button_SelectionManuelleFacettes ->SetForegroundColour(Forg);
    this->MSelect->Button_OuvrirReperage        ->SetBackgroundColour(Gris);
    this->MSelect->Button_OuvrirReperage        ->SetForegroundColour(Forg);
    this->MSelect->Button_Appliquer             ->SetBackgroundColour(Gris);
    this->MSelect->Button_Appliquer             ->SetForegroundColour(Forg);
    this->MSelect->Button_InverserNormales      ->SetBackgroundColour(Gris);
    this->MSelect->Button_InverserNormales      ->SetForegroundColour(Forg);
    this->MSelect->Button_UndoNormales          ->SetBackgroundColour(Gris);
    this->MSelect->Button_UndoNormales          ->SetForegroundColour(Forg);
    this->MSelect->Button_Delete                ->SetBackgroundColour(Gris);
    this->MSelect->Button_Delete                ->SetForegroundColour(Forg);
    this->MSelect->Button_UndoDelete            ->SetBackgroundColour(Gris);
    this->MSelect->Button_UndoDelete            ->SetForegroundColour(Forg);
    this->MSelect->Button_InverserParcours      ->SetBackgroundColour(Gris);
    this->MSelect->Button_InverserParcours      ->SetForegroundColour(Forg);
    this->MSelect->Button_Masquer               ->SetBackgroundColour(Gris);
    this->MSelect->Button_Masquer               ->SetForegroundColour(Forg);
    this->MSelect->Button_UndoMasquer           ->SetBackgroundColour(Gris);
    this->MSelect->Button_UndoMasquer           ->SetForegroundColour(Forg);
    this->MSelect->Button_Reafficher            ->SetBackgroundColour(Gris);
    this->MSelect->Button_Reafficher            ->SetForegroundColour(Forg);
    this->MSelect->Button_Centrer               ->SetBackgroundColour(Gris);
    this->MSelect->Button_Centrer               ->SetForegroundColour(Forg);
    this->MSelect->Button_Manipulations         ->SetBackgroundColour(Gris);
    this->MSelect->Button_Manipulations         ->SetForegroundColour(Forg);
    this->MSelect->Button_Etendre               ->SetBackgroundColour(Gris);
    this->MSelect->Button_Etendre               ->SetForegroundColour(Forg);
    this->MSelect->Button_Quitter               ->SetBackgroundColour(Gris);
    this->MSelect->Button_Quitter               ->SetForegroundColour(Forg);
    this->MSelect->Button_Fusionner             ->SetBackgroundColour(Gris);
    this->MSelect->Button_Fusionner             ->SetForegroundColour(Forg);
    this->MSelect->Refresh();

// SelectionManuelleFacettes
    this->MSelFac->SetForegroundColour(Forg);
    this->MSelFac->SetBackgroundColour(Back);
    this->MSelFac->StaticText_NomObjet->SetForegroundColour(Forg);
    this->MSelFac->StaticText_Numeros ->SetForegroundColour(Forg);
    this->MSelFac->StaticText1      ->SetForegroundColour(Forg);
    this->MSelFac->StaticText2      ->SetForegroundColour(Forg);
    this->MSelFac->SpinCtrl_NumObjet->SetForegroundColour(Forg);
    this->MSelFac->SpinCtrl_NumObjet->SetBackgroundColour(Gris);
    this->MSelFac->TextCtrl_Numeros ->SetForegroundColour(Forg);
    this->MSelFac->TextCtrl_Numeros ->SetBackgroundColour(Gris);
    this->MSelFac->Button_Valider   ->SetForegroundColour(Forg);
    this->MSelFac->Button_Valider   ->SetBackgroundColour(Gris);
    this->MSelFac->Button_Reset     ->SetForegroundColour(Forg);
    this->MSelFac->Button_Reset     ->SetBackgroundColour(Gris);
    this->MSelFac->Button_Quitter   ->SetForegroundColour(Forg);
    this->MSelFac->Button_Quitter   ->SetBackgroundColour(Gris);
    this->MSelFac->Refresh();

// SelectionManuelleObjets
    this->MSelObj->SetForegroundColour(Forg);
    this->MSelObj->SetBackgroundColour(Back);
    this->MSelObj->Button_Aucun     ->SetForegroundColour(Forg);
    this->MSelObj->Button_Aucun     ->SetBackgroundColour(Gris);
    this->MSelObj->Button_Tous      ->SetForegroundColour(Forg);
    this->MSelObj->Button_Tous      ->SetBackgroundColour(Gris);
    this->MSelObj->Button_Quitter   ->SetForegroundColour(Forg);
    this->MSelObj->Button_Quitter   ->SetBackgroundColour(Gris);
    this->MSelObj->Refresh();

// Sphere
    this->MSphere->SetForegroundColour(Forg);
    this->MSphere->SetBackgroundColour(Back);
    this->MSphere->StaticText1          ->SetForegroundColour(Forg);
    this->MSphere->StaticText2          ->SetForegroundColour(Forg);
    this->MSphere->StaticText3          ->SetForegroundColour(Forg);
    this->MSphere->StaticText4          ->SetForegroundColour(Forg);
    this->MSphere->StaticText5          ->SetForegroundColour(Forg);
    this->MSphere->StaticText6          ->SetForegroundColour(Forg);
    this->MSphere->StaticText7          ->SetForegroundColour(Forg);
    this->MSphere->StaticText8          ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_Rayon       ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_Rayon       ->SetBackgroundColour(Gris);
    this->MSphere->TextCtrl_X           ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_X           ->SetBackgroundColour(Gris);
    this->MSphere->TextCtrl_Y           ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_Y           ->SetBackgroundColour(Gris);
    this->MSphere->TextCtrl_Z           ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_Z           ->SetBackgroundColour(Gris);
    this->MSphere->TextCtrl_NbMeridiens ->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_NbMeridiens ->SetBackgroundColour(Gris);
    this->MSphere->TextCtrl_NbParalleles->SetForegroundColour(Forg);
    this->MSphere->TextCtrl_NbParalleles->SetBackgroundColour(Gris);
    this->MSphere->CheckBox_NewSphere   ->SetForegroundColour(Forg);
    this->MSphere->CheckBox_NewSphere   ->SetBackgroundColour(Gris);
    this->MSphere->SpinCtrl_Groupe      ->SetForegroundColour(Forg);
    this->MSphere->SpinCtrl_Groupe      ->SetBackgroundColour(Gris);
    this->MSphere->SpinCtrl_Materiau    ->SetForegroundColour(Forg);
    this->MSphere->SpinCtrl_Materiau    ->SetBackgroundColour(Gris);
    this->MSphere->Button_OK            ->SetForegroundColour(Forg);
    this->MSphere->Button_OK            ->SetBackgroundColour(Gris);
    this->MSphere->Button_Annuler       ->SetForegroundColour(Forg);
    this->MSphere->Button_Annuler       ->SetBackgroundColour(Gris);
    this->MSphere->Refresh();

// Tranlation Panel
    this->MTrans->SetForegroundColour(Forg);
    this->MTrans->SetBackgroundColour(Back);
    this->MTrans->StaticText1->SetForegroundColour(Forg);
    this->MTrans->StaticText2->SetForegroundColour(Forg);
    if (darkmode) {
        this->MTrans->StaticText7->SetForegroundColour(New_Forg);
        this->MTrans->StaticText8->SetForegroundColour(New_Forg);
     } else {
        this->MTrans->StaticText7->SetForegroundColour(Bleu_svg);
        this->MTrans->StaticText8->SetForegroundColour(Bleu_svg);
     }
    this->MTrans->TextCtrl_PasFin       ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_PasFin       ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_PasGrossier  ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_PasGrossier  ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_XG   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_XG   ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_XF   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_XF   ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_YG   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_YG   ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_YF   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_YF   ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_ZG   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_ZG   ->SetBackgroundColour(Gris);
    this->MTrans->TextCtrl_ZF   ->SetForegroundColour(Forg);
    this->MTrans->TextCtrl_ZF   ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_XG ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_XG ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_XF ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_XF ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_YG ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_YG ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_YF ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_YF ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_ZG ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_ZG ->SetBackgroundColour(Gris);
    this->MTrans->SpinButton_ZF ->SetForegroundColour(Forg);
    this->MTrans->SpinButton_ZF ->SetBackgroundColour(Gris);
    this->MTrans->Button_Valider->SetForegroundColour(Forg);
    this->MTrans->Button_Valider->SetBackgroundColour(Gris);
    this->MTrans->Button_OK     ->SetForegroundColour(Forg);
    this->MTrans->Button_OK     ->SetBackgroundColour(Gris);
    this->MTrans->Button_Annuler->SetForegroundColour(Forg);
    this->MTrans->Button_Annuler->SetBackgroundColour(Gris);
    this->MTrans->Refresh();

// ZoomSpecifique
    this->MZoomSpec->SetForegroundColour(Forg);
    this->MZoomSpec->SetBackgroundColour(Back);
    this->MZoomSpec->StaticText1        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText2        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText3        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText4        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText5        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText6        ->SetForegroundColour(Forg);
    this->MZoomSpec->StaticText7        ->SetForegroundColour(Forg);
    this->MZoomSpec->SpinCtrl_LAZ       ->SetForegroundColour(Forg);
    this->MZoomSpec->SpinCtrl_LAZ       ->SetBackgroundColour(Gris);
    this->MZoomSpec->SpinCtrl_LSI       ->SetForegroundColour(Forg);
    this->MZoomSpec->SpinCtrl_LSI       ->SetBackgroundColour(Gris);
    this->MZoomSpec->TextCtrl_Distance  ->SetForegroundColour(Forg);
    this->MZoomSpec->TextCtrl_Distance  ->SetBackgroundColour(Gris);
    this->MZoomSpec->TextCtrl_FoV       ->SetForegroundColour(Forg);
    this->MZoomSpec->TextCtrl_FoV       ->SetBackgroundColour(Gris);
    this->MZoomSpec->Button_Appliquer   ->SetForegroundColour(Forg);
    this->MZoomSpec->Button_Appliquer   ->SetBackgroundColour(Gris);
    this->MZoomSpec->Button_Reset       ->SetForegroundColour(Forg);
    this->MZoomSpec->Button_Reset       ->SetBackgroundColour(Gris);
    this->MZoomSpec->Button_Quit        ->SetForegroundColour(Forg);
    this->MZoomSpec->Button_Quit        ->SetBackgroundColour(Gris);
    this->MZoomSpec->Refresh();

// Aide_html
    this->MHelp->SetForegroundColour(Forg);
    this->MHelp->SetBackgroundColour(Back);
    this->MHelp->Button_OK->SetForegroundColour(Forg);
    this->MHelp->Button_OK->SetBackgroundColour(Gris);
    this->MHelp->Refresh();

// About : c'était un wxMessageBox dans void OvniFrame::OnAbout : ne semble pas pouvoir se coloriser via SetForegroundColour / SetBackgroundColour

//    this->MAIN_b->Refresh();

// Colorisation des menus

//    Switch_theme_menus(darkmode);   // à utiliser seulement si methode_boucle sur false

}

#endif // wxCHECK_VERSION

