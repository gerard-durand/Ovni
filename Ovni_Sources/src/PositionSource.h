#ifndef POSITIONSOURCE_H
#define POSITIONSOURCE_H


//(*Headers(PositionSource)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
//*)

#include "OvniMain.h"

class PositionSource: public wxDialog
{
	public:

		PositionSource(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PositionSource();

		//(*Declarations(PositionSource)
		wxButton* Button_Defaut;
		wxButton* Button_Quitter;
		wxSpinCtrlDouble* SpinCtrlDouble_PosW;
		wxSpinCtrlDouble* SpinCtrlDouble_PosX;
		wxSpinCtrlDouble* SpinCtrlDouble_PosY;
		wxSpinCtrlDouble* SpinCtrlDouble_PosZ;
		wxStaticText* Pos_W;
		wxStaticText* Pos_X;
		wxStaticText* Pos_Y;
		wxStaticText* Pos_Z;
		wxStaticText* StaticText1;
		//*)
//        wxWindow* Pos_Parent;

	protected:

		//(*Identifiers(PositionSource)
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_SPINCTRLDOUBLE1;
		static const long ID_SPINCTRLDOUBLE2;
		static const long ID_SPINCTRLDOUBLE3;
		static const long ID_SPINCTRLDOUBLE4;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(PositionSource)
		void OnSpinCtrlDouble_PosXChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_PosYChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_PosZChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_PosWChange(wxSpinDoubleEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_DefautClick(wxCommandEvent& event);
		//*)
		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
