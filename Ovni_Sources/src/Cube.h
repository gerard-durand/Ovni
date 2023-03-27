#ifndef CUBE_H
#define CUBE_H

//#include <wx/string.h>
//#include <float.h>

//#include <vector>

//(*Headers(Cube)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class Cube: public wxDialog
{
	public:

		Cube(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Cube();
//		wxWindow* Cube_Parent;
		int numeroGroupe  = 0;
		int numeroMateriau= 0;

		//(*Declarations(Cube)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxTextCtrl* TextCtrl_Arete;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Cube)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL4;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT6;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL2;
		//*)

	private:

		//(*Handlers(Cube)
		void OnInit(wxInitDialogEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		//*)
		OvniFrame *MAIN;

        float centre_primitive[3];
		float arete;

		void Genere_Cube();
		void Genere_Facettes(BddInter*, Object*);//int);
		void Genere_Sommets (BddInter*, Object*);//int);
//      void Genere_Luminances      (BddInter*, int);
//      void Genere_Normales_Sommets (BddInter*, int);

		DECLARE_EVENT_TABLE()
};

#endif
