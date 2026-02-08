#ifndef ZOOMSPECIFIQUE_H
#define ZOOMSPECIFIQUE_H

//(*Headers(ZoomSpecifique)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ZoomSpecifique: public wxDialog
{
	public:

		ZoomSpecifique(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ZoomSpecifique();

		//(*Declarations(ZoomSpecifique)
		wxButton* Button_Appliquer;
		wxButton* Button_Quit;
		wxButton* Button_Reset;
		wxSpinCtrl* SpinCtrl_LAZ;
		wxSpinCtrl* SpinCtrl_LSI;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText_Warning;
		wxTextCtrl* TextCtrl_Distance;
		wxTextCtrl* TextCtrl_FoV;
		//*)

	protected:

		//(*Identifiers(ZoomSpecifique)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_SPINCTRL1;
		static const wxWindowID ID_SPINCTRL2;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_TEXTCTRL2;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_STATICTEXT8;
		//*)

	private:

		//(*Handlers(ZoomSpecifique)
		void OnButton_QuitClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnSpinCtrl_LAZChange(wxSpinEvent& event);
		void OnSpinCtrl_LSIChange(wxSpinEvent& event);
		void OnTextCtrl_DistanceText(wxCommandEvent& event);
		void OnTextCtrl_FoVText(wxCommandEvent& event);
		void OnButton_AppliquerClick(wxCommandEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		//*)

		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
