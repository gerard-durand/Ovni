#ifndef REPERAGEMATERIAU_H
#define REPERAGEMATERIAU_H

//(*Headers(ReperageMateriau)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ReperageMateriau: public wxDialog
{
	public:

		ReperageMateriau(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ReperageMateriau();
//		wxWindow* Mat_Parent;

		//(*Declarations(ReperageMateriau)
		wxButton* Button_OK;
		wxButton* Button_Quit;
		wxSpinButton* SpinButton1;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(ReperageMateriau)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_SPINBUTTON1;
		//*)

	private:

		//(*Handlers(ReperageMateriau)
		void OnSpinButton1Change(wxSpinEvent& event);
		void OnButton_QuitClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)
		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
