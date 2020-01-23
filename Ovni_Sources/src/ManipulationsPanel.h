#ifndef MANIPULATIONSPANEL_H
#define MANIPULATIONSPANEL_H

//(*Headers(ManipulationsPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/statline.h>
#include <wx/stattext.h>
//*)

#include "OvniMain.h"

class ManipulationsPanel: public wxDialog
{
	public:

		ManipulationsPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ManipulationsPanel();

		//(*Declarations(ManipulationsPanel)
		wxButton* Button_Creer;
		wxButton* Button_Quitter;
		wxButton* Button_Raz;
		wxButton* Button_Rotation;
		wxButton* Button_Scale;
		wxButton* Button_Translation;
		wxCheckBox* CheckBox_X;
		wxCheckBox* CheckBox_Y;
		wxCheckBox* CheckBox_Z;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		//*)

	protected:

		//(*Identifiers(ManipulationsPanel)
		static const long ID_STATICTEXT1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_STATICTEXT2;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_STATICLINE1;
		static const long ID_BUTTON6;
		//*)

	private:

		//(*Handlers(ManipulationsPanel)
		void OnButton_TranslationClick(wxCommandEvent& event);
		void OnButton_RotationClick(wxCommandEvent& event);
		void OnButton_ScaleClick(wxCommandEvent& event);
		void OnCheckBox_XClick(wxCommandEvent& event);
		void OnCheckBox_YClick(wxCommandEvent& event);
		void OnCheckBox_ZClick(wxCommandEvent& event);
		void OnButton_CreerClick(wxCommandEvent& event);
		void OnButton_RazClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)
//		void ToDo();
		void CreerBoiteObjet();
        OvniFrame* MAIN;
        bool CheckBox_XYZ;
        double x1b1,x2b1,y1b1,y2b1,z1b1,z2b1; // Boite 1 : originale

		DECLARE_EVENT_TABLE()
};

#endif
