#ifndef SELECTIONMANUELLEFACETTES_H
#define SELECTIONMANUELLEFACETTES_H

//(*Headers(SelectionManuelleFacettes)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class SelectionManuelleFacettes: public wxDialog
{
	public:

		SelectionManuelleFacettes(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SelectionManuelleFacettes();

		//(*Declarations(SelectionManuelleFacettes)
		wxButton* Button_Quitter;
		wxButton* Button_Reset;
		wxButton* Button_Valider;
		wxSpinCtrl* SpinCtrl_NumObjet;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText_NomObjet;
		wxStaticText* StaticText_Numeros;
		wxTextCtrl* TextCtrl_Numeros;
		//*)

	protected:

		//(*Identifiers(SelectionManuelleFacettes)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON3;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(SelectionManuelleFacettes)
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_ValiderClick(wxCommandEvent& event);
		void OnSpinCtrl_NumObjetChange(wxSpinEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		//*)
		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
