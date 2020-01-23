#ifndef CHANGERECHELLEBDD_H
#define CHANGERECHELLEBDD_H

//(*Headers(ChangerEchelleBdd)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ChangerEchelleBdd: public wxDialog
{
	public:

		ChangerEchelleBdd(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ChangerEchelleBdd();
//		wxWindow* ChangerEchelleBdd_Parent;

		//(*Declarations(ChangerEchelleBdd)
		wxButton* Button_Inverser;
		wxButton* Button_OK;
		wxButton* Button_Quitter;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* TextCtrl_scale;
		//*)

	protected:

		//(*Identifiers(ChangerEchelleBdd)
		static const long ID_STATICTEXT1;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICLINE2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(ChangerEchelleBdd)
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_InverserClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)
        OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
