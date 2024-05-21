#ifndef DEPLACERBDD_H
#define DEPLACERBDD_H

//(*Headers(DeplacerBdd)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class DeplacerBdd: public wxDialog
{
	public:

		DeplacerBdd(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~DeplacerBdd();
//		wxWindow* Deplacer_Parent;

		//(*Declarations(DeplacerBdd)
		wxButton* Button_Centrer;
		wxButton* Button_Inverser;
		wxButton* Button_OK;
		wxButton* Button_Quitter;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl_DeplX;
		wxTextCtrl* TextCtrl_DeplY;
		wxTextCtrl* TextCtrl_DeplZ;
		//*)

	protected:

		//(*Identifiers(DeplacerBdd)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_STATICLINE1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_TEXTCTRL2;
		static const wxWindowID ID_TEXTCTRL3;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_BUTTON4;
		//*)

	private:

		//(*Handlers(DeplacerBdd)
		void OnInit(wxInitDialogEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_InverserClick(wxCommandEvent& event);
		void OnButton_QuitClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_CentrerClick(wxCommandEvent& event);
		//*)
        OvniFrame* MAIN;


		DECLARE_EVENT_TABLE()
};

#endif
