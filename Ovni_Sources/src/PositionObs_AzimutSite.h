#ifndef POSITIONOBS_AZIMUTSITE_H
#define POSITIONOBS_AZIMUTSITE_H

//(*Headers(PositionObs_AzimutSite)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

#include "OvniMain.h"

class PositionObs_AzimutSite: public wxDialog
{
	public:

		PositionObs_AzimutSite(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PositionObs_AzimutSite();

		//(*Declarations(PositionObs_AzimutSite)
		wxButton* Button_Quit;
		wxSpinCtrl* SpinCtrl_LAZ;
		wxSpinCtrl* SpinCtrl_LSI;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText_Warning;
		//*)
//        wxWindow* Pos_Parent;

	protected:

		//(*Identifiers(PositionObs_AzimutSite)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL1;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT6;
		//*)

	private:

		//(*Handlers(PositionObs_AzimutSite)
		void OnSpinCtrl_LAZChange(wxSpinEvent& event);
		void OnSpinCtrl_LSIChange(wxSpinEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_QuitClick(wxCommandEvent& event);
		//*)

		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
