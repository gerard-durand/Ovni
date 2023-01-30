/***************************************************************
 * Name:      OvniApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Gerard Durand (Gerard.Durand@onera.fr)
 * Created:   2011-02-10
 * Copyright: Gerard Durand ()
 * License:
 **************************************************************/

#include "OvniApp.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>

#include <wx/settings.h>    // Pour test sur IsDark

//(*AppHeaders
#include "OvniMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(OvniApp);

bool OvniApp::OnInit() {
    char Message[1024];
//    printf("Entree OvniApp::OnInit\n");

/** \brief OvniApp::OnInit Initialisation globale d'Ovni
 *
 * \return wxsOK
 *
 */


#if _WIN32
    system("chcp 65001  > nul");    // Pour coder en UTF-8 les sorties dans la console DOS sous Windows
#endif

// D'après : https://wiki.wxwidgets.org/Converting_everything_to_and_from_wxString
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    snprintf(Message,1024,"Chemin de l'exécutable :\n%s\n",(const char*)appPath.utf8_str());
    printf("%s",Message);
//    printf(utf8_To_ibm(Message));        // Ne sert plus car console windows supporte l'utf8
    wxSetWorkingDirectory(appPath);

//#if wxCHECK_VERSION(3,3,0)
//    printf("IsDark       : %d\n",wxSystemSettings::GetAppearance().IsDark() );          // Dark mode en cours dans l'application
//    printf("IsSystemDark : %d\n",wxSystemSettings::GetAppearance().IsSystemDark() );    // Dark mode en cours du système
////    printf("IsUsingDarkBackground : %d\n",wxSystemSettings::GetAppearance().wxSystemAppearance::IsUsingDarkBackground() );
////    wxGetApp().MSWEnableDarkMode(1);                                                    // Pour forcer le mode Dark
////    printf("IsDark       : %d\n",wxSystemSettings::GetAppearance().IsDark() );
//#endif // wxCHECK_VERSION

    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK ) {
        OvniFrame* Frame = new OvniFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
//    printf("Sortie OvniApp::OnInit\n");
    return wxsOK;

}
