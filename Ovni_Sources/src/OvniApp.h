/***************************************************************
 * Name:      OvniApp.h
 * Purpose:   Defines Application Class
 * Author:    Gerard Durand (Gerard.Durand@onera.fr)
 * Created:   2011-02-10
 * Copyright: Gerard Durand ()
 * License:
 **************************************************************/

#ifndef OVNIAPP_H
#define OVNIAPP_H

#include <wx/app.h>

class OvniApp : public wxApp
{
    public:
        virtual bool OnInit();
        int arg_c;
        wxString par_0, par_1;

        int Getargc() {
            return this->arg_c;
        }
};

DECLARE_APP(OvniApp);   // Ajout pour pouvoir utiliser wxGetApp() dans OvniMain et pas seulement dans OvniApp

#endif // OVNIAPP_H
