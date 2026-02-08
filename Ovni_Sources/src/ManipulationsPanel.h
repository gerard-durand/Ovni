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
		void NewObjet_SelectedFacets(wxCommandEvent& event);

		//(*Declarations(ManipulationsPanel)
		wxButton* Button_Creer;
		wxButton* Button_NewObjet;
		wxButton* Button_Quitter;
		wxButton* Button_Raz;
		wxButton* Button_Rotation;
		wxButton* Button_Scale;
		wxButton* Button_Translation;
		wxCheckBox* CheckBox_X;
		wxCheckBox* CheckBox_Y;
		wxCheckBox* CheckBox_Z;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		//*)

	protected:

		//(*Identifiers(ManipulationsPanel)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_CHECKBOX1;
		static const wxWindowID ID_CHECKBOX2;
		static const wxWindowID ID_CHECKBOX3;
		static const wxWindowID ID_BUTTON4;
		static const wxWindowID ID_BUTTON5;
		static const wxWindowID ID_STATICLINE1;
		static const wxWindowID ID_BUTTON6;
		static const wxWindowID ID_BUTTON7;
		static const wxWindowID ID_STATICLINE2;
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
		void OnButton_NewObjetClick(wxCommandEvent& event);
		//*)

        OvniFrame* MAIN;

//		void ToDo();
		void CreerBoiteObjet();

        bool CheckBox_XYZ;
        double x1b1,x2b1,y1b1,y2b1,z1b1,z2b1; // Boite 1 : originale

		DECLARE_EVENT_TABLE()
};

#endif
