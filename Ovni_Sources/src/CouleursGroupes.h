#ifndef COULEURSGROUPES_H
#define COULEURSGROUPES_H

//(*Headers(CouleursGroupes)
#include <wx/button.h>
#include <wx/clrpicker.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class CouleursGroupes: public wxDialog
{
	public:

		CouleursGroupes(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CouleursGroupes();

		//(*Declarations(CouleursGroupes)
		wxButton* Button_Quitter;
		wxButton* Button_Reset_ALL;
		wxButton* Button_Reset_Couleur;
		wxColourPickerCtrl* ColourPickerCtrl_Ambient;
		wxColourPickerCtrl* ColourPickerCtrl_Diffuse;
		wxSpinCtrl* SpinCtrl_Groupes;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxTextCtrl* TextCtrl_Materiau;
		//*)

	protected:

		//(*Identifiers(CouleursGroupes)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_STATICLINE1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_SPINCTRL1;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_COLOURPICKERCTRL1;
		static const wxWindowID ID_COLOURPICKERCTRL2;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_BUTTON2;
		//*)

	private:

		//(*Handlers(CouleursGroupes)
		void OnClose(wxCloseEvent& event);
		void OnSpinCtrl_GroupesChange(wxSpinEvent& event);
		void OnButton_AmbientClick(wxCommandEvent& event);
		void OnButton_DiffuseClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnColourPickerCtrl_AmbientColourChanged(wxColourPickerEvent& event);
		void OnColourPickerCtrl_DiffuseColourChanged(wxColourPickerEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnButton_Reset_CouleurClick(wxCommandEvent& event);
		void OnButton_Reset_ALLClick(wxCommandEvent& event);
		//*)

		OvniFrame* MAIN;

		int Numero_Groupe;

		DECLARE_EVENT_TABLE()
};

#endif
