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

//(*AppHeaders
#include "OvniMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(OvniApp);

bool OvniApp::OnInit() {
    char Message[1024];
//    printf("Entree OvniApp::OnInit\n");

#if _WIN32
    system("chcp 65001  > nul");    // Pour coder en UTF-8 les sorties dans la console DOS sous Windows
#endif

// D'après : https://wiki.wxwidgets.org/Converting_everything_to_and_from_wxString
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    snprintf(Message,1024,"Chemin de l'exécutable :\n%s\n",(const char*)appPath.utf8_str());
    printf(Message);
///    printf(utf8_To_ibm(Message));        // Ne sert plus car console windows supporte l'utf8
    wxSetWorkingDirectory(appPath);

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
