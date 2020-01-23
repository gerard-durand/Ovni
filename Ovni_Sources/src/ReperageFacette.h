#ifndef REPERAGEFACETTE_H
#define REPERAGEFACETTE_H

//(*Headers(ReperageFacette)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ReperageFacette: public wxDialog
{
	public:

		ReperageFacette(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ReperageFacette();
//		wxWindow* Facette_Parent;

		//(*Declarations(ReperageFacette)
		wxButton* Button_CentrerRotation;
		wxButton* Button_DelFacette;
		wxButton* Button_InvNormale;
		wxButton* Button_Masquer;
		wxButton* Button_Permuter;
		wxButton* Button_Quitter;
		wxButton* Button_Reset;
		wxButton* Button_UndoDel;
		wxCheckBox* CheckBox_Laisser;
		wxCheckBox* CheckBox_VisuNormale;
		wxCheckBox* CheckBox_VisuNormales_Sommets;
		wxCheckBox* CheckBox_VisuSommets;
		wxSpinCtrl* SpinCtrl_IndiceFacette;
		wxSpinCtrl* SpinCtrl_IndiceObjet;
		wxSpinCtrl* SpinCtrl_IndiceSommet;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText_Warning;
		wxTextCtrl* TextCtrl_NumeroSommet;
		wxTextCtrl* Text_NbSommets;
		wxTextCtrl* Text_NomObjet;
		wxTextCtrl* Text_NumeroObjet;
		//*)
		void ButtonsDisable();

	protected:

		//(*Identifiers(ReperageFacette)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL1;
		static const long ID_SPINCTRL2;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL3;
		static const long ID_CHECKBOX4;
		static const long ID_SPINCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		static const long ID_CHECKBOX3;
		static const long ID_BUTTON8;
		static const long ID_BUTTON9;
		static const long ID_STATICTEXT5;
		//*)

	private:

		//(*Handlers(ReperageFacette)
		void OnClose(wxCloseEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnButton_CentrerRotationClick(wxCommandEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		void OnButton_MasquerClick(wxCommandEvent& event);
		void OnButton_DelFacetteClick(wxCommandEvent& event);
		void OnButton_UndoDelClick(wxCommandEvent& event);
		void OnButton_InvNormaleClick(wxCommandEvent& event);
		void OnButton_UndoNormaleClick(wxCommandEvent& event);
		void OnButton_PermuterClick(wxCommandEvent& event);
		void OnCheckBox_VisuNormaleClick(wxCommandEvent& event);
		void OnCheckBox_VisuNormales_SommetsClick(wxCommandEvent& event);
		void OnCheckBox_VisuSommetsClick(wxCommandEvent& event);
		void OnSpinCtrl_IndiceSommetChange(wxSpinEvent& event);
		void OnSpinCtrl_IndiceFacetteChange(wxSpinEvent& event);
		void OnSpinCtrl_IndiceObjetChange(wxSpinEvent& event);
		//*)
		OvniFrame* MAIN;
		void ButtonsEnable();

		DECLARE_EVENT_TABLE()
};

#endif
