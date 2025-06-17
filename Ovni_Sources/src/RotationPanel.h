#ifndef ROTATIONPANEL_H
#define ROTATIONPANEL_H

//(*Headers(RotationPanel)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/spinbutt.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class RotationPanel: public wxDialog
{
	public:

		RotationPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~RotationPanel();

		//(*Declarations(RotationPanel)
		wxButton* Button_Annuler;
		wxButton* Button_Appliquer;
		wxButton* Button_ValiderPas;
		wxButton* Button_ValiderPoint;
		wxRadioBox* RadioBox_Centre;
		wxSpinButton* SpinButton_X;
		wxSpinButton* SpinButton_Y;
		wxSpinButton* SpinButton_Z;
		wxStaticBox* StaticBox1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxTextCtrl* TextCtrl_AngleX;
		wxTextCtrl* TextCtrl_AngleY;
		wxTextCtrl* TextCtrl_AngleZ;
		wxTextCtrl* TextCtrl_PasAngulaire;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)
		void SetRotX(double value) {RotX = value;}
		void SetRotY(double value) {RotY = value;}
		void SetRotZ(double value) {RotZ = value;}

	protected:

		//(*Identifiers(RotationPanel)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_STATICBOX1;
		static const long ID_RADIOBOX1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_BUTTON2;
		static const long ID_TEXTCTRL5;
		static const long ID_SPINBUTTON1;
		static const long ID_TEXTCTRL6;
		static const long ID_SPINBUTTON2;
		static const long ID_TEXTCTRL7;
		static const long ID_SPINBUTTON3;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		//*)

	private:

		//(*Handlers(RotationPanel)
		void OnClose(wxCloseEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnTextCtrl_PasAngulaireText(wxCommandEvent& event);
		void OnButton_ValiderPasClick(wxCommandEvent& event);
		void OnRadioBox_CentreSelect(wxCommandEvent& event);
		void OnTextCtrl_XText(wxCommandEvent& event);
		void OnTextCtrl_YText(wxCommandEvent& event);
		void OnTextCtrl_ZText(wxCommandEvent& event);
		void OnButton_ValiderPointClick(wxCommandEvent& event);
		void OnSpinButton_XChange(wxSpinEvent& event);
		void OnSpinButton_YChange(wxSpinEvent& event);
		void OnSpinButton_ZChange(wxSpinEvent& event);
		void OnButton_AppliquerClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinButton_XChangeUp(wxSpinEvent& event);
		void OnSpinButton_XChangeDown(wxSpinEvent& event);
		void OnSpinButton_YChangeUp(wxSpinEvent& event);
		void OnSpinButton_YChangeDown(wxSpinEvent& event);
		void OnSpinButton_ZChangeUp(wxSpinEvent& event);
		void OnSpinButton_ZChangeDown(wxSpinEvent& event);
		void OnTextCtrl_AngleXText(wxCommandEvent& event);
		void OnTextCtrl_AngleYText(wxCommandEvent& event);
		void OnTextCtrl_AngleZText(wxCommandEvent& event);
		//*)

        OvniFrame* MAIN;

		double RotX, RotY, RotZ;

//		void ToDo();
		void Appliquer_Rotation_Visuelle();
		void Init_Centre_Rotation();
		void Rotation_Objet_X(int, double);
		void Rotation_Objet_Y(int, double);
		void Rotation_Objet_Z(int, double);
		wxString wxTexte;
		double Incr;

		DECLARE_EVENT_TABLE()
};

#endif
