#ifndef REPERAGEPOINT_H
#define REPERAGEPOINT_H

//(*Headers(ReperagePoint)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ReperagePoint: public wxDialog
{
	public:

		ReperagePoint(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ReperagePoint();
//		wxWindow* Point_Parent;

		//(*Declarations(ReperagePoint)
		wxButton* Button_Centrer;
		wxButton* Button_ModifierX;
		wxButton* Button_ModifierY;
		wxButton* Button_ModifierZ;
		wxButton* Button_Quitter;
		wxButton* Button_Reset;
		wxButton* Button_UndoX;
		wxButton* Button_UndoY;
		wxButton* Button_UndoZ;
		wxCheckBox* CheckBox_Laisser;
		wxCheckBox* CheckBox_X;
		wxCheckBox* CheckBox_Y;
		wxCheckBox* CheckBox_Z;
		wxSpinCtrl* SpinCtrl_IndiceObjet;
		wxSpinCtrl* SpinCtrl_IndicePoint;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxTextCtrl* Text_NomObjet;
		wxTextCtrl* Text_NumeroObjet;
		wxTextCtrl* Text_ValeurX;
		wxTextCtrl* Text_ValeurY;
		wxTextCtrl* Text_ValeurZ;
		//*)

	protected:

		//(*Identifiers(ReperagePoint)
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_TEXTCTRL5;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		static const long ID_CHECKBOX4;
		static const long ID_BUTTON8;
		static const long ID_BUTTON9;
		//*)

	private:

		//(*Handlers(ReperagePoint)
		void OnClose(wxCloseEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnButton_CentrerClick(wxCommandEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		void OnCheckBox_XClick(wxCommandEvent& event);
		void OnCheckBox_YClick(wxCommandEvent& event);
		void OnCheckBox_ZClick(wxCommandEvent& event);
		void OnButton_ModifierXClick(wxCommandEvent& event);
		void OnButton_ModifierYClick(wxCommandEvent& event);
		void OnButton_ModifierZClick(wxCommandEvent& event);
		void OnButton_UndoXClick(wxCommandEvent& event);
		void OnButton_UndoYClick(wxCommandEvent& event);
		void OnButton_UndoZClick(wxCommandEvent& event);
		void OnSpinCtrl_IndicePointChange(wxSpinEvent& event);
		void OnSpinCtrl_IndiceObjetChange(wxSpinEvent& event);
		//*)
		OvniFrame* MAIN;
		float Old_X, Old_Y, Old_Z;

		DECLARE_EVENT_TABLE()
};

#endif
