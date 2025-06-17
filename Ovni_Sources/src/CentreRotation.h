#ifndef CENTREROTATION_H
#define CENTREROTATION_H

//(*Headers(CentreRotation)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

#include "OvniMain.h"

class CentreRotation: public wxDialog
{
	public:

		CentreRotation(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CentreRotation();

		//(*Declarations(CentreRotation)
		wxButton* Button_Defs;
		wxButton* Button_OK;
		wxSpinCtrlDouble* SpinCtrlDouble_X;
		wxSpinCtrlDouble* SpinCtrlDouble_Y;
		wxSpinCtrlDouble* SpinCtrlDouble_Z;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		//*)
//        wxWindow* Rot_Parent;

	protected:

		//(*Identifiers(CentreRotation)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_SPINCTRLDOUBLE1;
		static const wxWindowID ID_SPINCTRLDOUBLE2;
		static const wxWindowID ID_SPINCTRLDOUBLE3;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		//*)

	private:

		//(*Handlers(CentreRotation)
		void OnInit(wxInitDialogEvent& event);
		void OnSpinCtrlDouble_XChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_YChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_ZChange(wxSpinDoubleEvent& event);
		void OnButton_DefsClick(wxCommandEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)

        OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
