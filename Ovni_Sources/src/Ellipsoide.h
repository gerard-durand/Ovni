#ifndef ELLIPSOIDE_H
#define ELLIPSOIDE_H

//(*Headers(Ellipsoide)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class Ellipsoide: public wxDialog
{
	public:

		Ellipsoide(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Ellipsoide();
//		wxWindow* Ellipsoide_Parent;
//		int numeroGroupe  = 0;
//		int numeroMateriau= 0;

		//(*Declarations(Ellipsoide)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxCheckBox* CheckBox_NewSphere;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl_CoefX;
		wxTextCtrl* TextCtrl_CoefY;
		wxTextCtrl* TextCtrl_CoefZ;
		wxTextCtrl* TextCtrl_NbMeridiens;
		wxTextCtrl* TextCtrl_NbParalleles;
		wxTextCtrl* TextCtrl_Rayon;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Ellipsoide)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL6;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT8;
		static const long ID_SPINCTRL2;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT10;
		static const long ID_TEXTCTRL8;
		static const long ID_STATICTEXT11;
		static const long ID_TEXTCTRL9;
		static const long ID_CHECKBOX1;
		//*)

	private:

		//(*Handlers(Ellipsoide)
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		//*)

        OvniFrame* MAIN;

        float centre_primitive[3];
		float arete;
		float rayon;
		int   Nb_Meridiens;
		int   Nb_Paralleles;
		bool  New_typeSphere;
		float coefx, coefy, coefz;

		int numeroGroupe   = 0;
		int numeroMateriau = 0;

		void genereEllipsoide();

		DECLARE_EVENT_TABLE()
};

#endif
