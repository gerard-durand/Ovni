#ifndef TRANSLATIONPANEL_H
#define TRANSLATIONPANEL_H

//(*Headers(TranslationPanel)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class TranslationPanel: public wxDialog
{
	public:

		TranslationPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TranslationPanel();

		//(*Declarations(TranslationPanel)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxButton* Button_Valider;
		wxSpinButton* SpinButton_XF;
		wxSpinButton* SpinButton_XG;
		wxSpinButton* SpinButton_YF;
		wxSpinButton* SpinButton_YG;
		wxSpinButton* SpinButton_ZF;
		wxSpinButton* SpinButton_ZG;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxTextCtrl* TextCtrl_PasFin;
		wxTextCtrl* TextCtrl_PasGrossier;
		wxTextCtrl* TextCtrl_XF;
		wxTextCtrl* TextCtrl_XG;
		wxTextCtrl* TextCtrl_YF;
		wxTextCtrl* TextCtrl_YG;
		wxTextCtrl* TextCtrl_ZF;
		wxTextCtrl* TextCtrl_ZG;
		//*)

	protected:

		//(*Identifiers(TranslationPanel)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_SPINBUTTON1;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_SPINBUTTON2;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_SPINBUTTON3;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL6;
		static const long ID_SPINBUTTON4;
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL7;
		static const long ID_SPINBUTTON5;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL8;
		static const long ID_SPINBUTTON6;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(TranslationPanel)
		void OnTextCtrl1Text(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_ValiderClick(wxCommandEvent& event);
		void OnSpinButton_XGChange(wxSpinEvent& event);
		void OnSpinButton_XFChange(wxSpinEvent& event);
		void OnSpinButton_YGChange(wxSpinEvent& event);
		void OnSpinButton_YFChange(wxSpinEvent& event);
		void OnSpinButton_ZFChange(wxSpinEvent& event);
		void OnSpinButton_ZGChange(wxSpinEvent& event);
		void OnTextCtrl_PasFinText(wxCommandEvent& event);
		void OnTextCtrl_PasGrossierText(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinButton_XGChangeUp(wxSpinEvent& event);
		void OnSpinButton_XGChangeDown(wxSpinEvent& event);
		void OnSpinButton_YGChangeUp(wxSpinEvent& event);
		void OnSpinButton_YGChangeDown(wxSpinEvent& event);
		void OnSpinButton_ZGChangeUp(wxSpinEvent& event);
		void OnSpinButton_ZGChangeDown(wxSpinEvent& event);
		void OnSpinButton_XFChangeUp(wxSpinEvent& event);
		void OnSpinButton_XFChangeDown(wxSpinEvent& event);
		void OnSpinButton_YFChangeUp(wxSpinEvent& event);
		void OnSpinButton_YFChangeDown(wxSpinEvent& event);
		void OnSpinButton_ZFChangeUp(wxSpinEvent& event);
		void OnSpinButton_ZFChangeDown(wxSpinEvent& event);
		void OnTextCtrl_XGTextEnter(wxCommandEvent& event);
		void OnTextCtrl_YGTextEnter(wxCommandEvent& event);
		void OnTextCtrl_ZGTextEnter(wxCommandEvent& event);
		//*)
		void Appliquer_Translation(double, double, double);
//		void ToDo();
		double Pas_G, Pas_F;
		double Cumul_X, Cumul_XG, Cumul_XF;
		double Cumul_Y, Cumul_YG, Cumul_YF;
		double Cumul_Z, Cumul_ZG, Cumul_ZF;
		double tx, ty, tz;
		wxString wxTexte;
        OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
