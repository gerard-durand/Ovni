#ifndef FACETTE_H
#define FACETTE_H

//(*Headers(Facette)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class Facette: public wxDialog
{
	public:

		Facette(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Facette();
//        wxWindow* Facette_Parent;
		int numeroGroupe  = 0;
		int numeroMateriau= 0;


		//(*Declarations(Facette)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
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
		wxTextCtrl* TextCtrl_P1X;
		wxTextCtrl* TextCtrl_P1Y;
		wxTextCtrl* TextCtrl_P1Z;
		wxTextCtrl* TextCtrl_P2X;
		wxTextCtrl* TextCtrl_P2Y;
		wxTextCtrl* TextCtrl_P2Z;
		wxTextCtrl* TextCtrl_P3X;
		wxTextCtrl* TextCtrl_P3Y;
		wxTextCtrl* TextCtrl_P3Z;
		//*)

	protected:

		//(*Identifiers(Facette)
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
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL8;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT10;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT11;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(Facette)
		void OnTextCtrl5Text(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)
        OvniFrame* MAIN;
		void genereFacette();

		DECLARE_EVENT_TABLE()
};

#endif
