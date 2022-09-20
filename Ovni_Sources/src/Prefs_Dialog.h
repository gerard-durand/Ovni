#ifndef PREFS_DIALOG_H
#define PREFS_DIALOG_H

//(*Headers(Prefs_Dialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"
//#include <math.h>

class Prefs_Dialog: public wxDialog
{
	public:

		Prefs_Dialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Prefs_Dialog();
//		wxWindow* Prefs_Parent;
//        wxPoint pos_prefs; // Pour la position de la fenêtre préférences

		//(*Declarations(Prefs_Dialog)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxBoxSizer* BoxSizer6;
		wxButton* Button_OK;
		wxButton* Button_Reset;
		wxButton* Button_tmp_rep;
		wxCheckBox* CheckBox_1SeulObjet3D;
		wxCheckBox* CheckBox_AntialiasingSoft;
		wxCheckBox* CheckBox_CalculNormales;
		wxCheckBox* CheckBox_CreerBackup;
		wxCheckBox* CheckBox_DisplayFps;
		wxCheckBox* CheckBox_LectureOptimisee;
		wxCheckBox* CheckBox_NotFlat;
		wxCheckBox* CheckBox_RecNormales_Seuillees;
		wxCheckBox* CheckBox_Seuillage;
		wxCheckBox* CheckBox_SupprBackup;
		wxCheckBox* CheckBox_TestDecalage3DS;
		wxCheckBox* CheckBox_TraiterDoublonsAretes;
		wxRadioBox* RadioBox_IconSize;
		wxRadioBox* RadioBox_Trackball;
		wxRadioBox* RadioBox_Triangulation;
		wxSpinCtrl* SpinCtrl_PasSvg;
		wxSpinCtrl* SpinCtrl_Threads;
		wxSpinCtrlDouble* SpinCtrlDouble_SeuilGouraud2;
		wxSpinCtrlDouble* SpinCtrlDouble_SeuilGouraud;
		wxSpinCtrlDouble* SpinCtrlDouble_axes;
		wxSpinCtrlDouble* SpinCtrlDouble_norm;
		wxSpinCtrlDouble* SpinCtrlDouble_src;
		wxStaticLine* StaticLine10;
		wxStaticLine* StaticLine11;
		wxStaticLine* StaticLine12;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticLine* StaticLine4;
		wxStaticLine* StaticLine5;
		wxStaticLine* StaticLine6;
		wxStaticLine* StaticLine7;
		wxStaticLine* StaticLine8;
		wxStaticLine* StaticLine9;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* StaticText_Gouraud2;
		wxStaticText* StaticText_Gouraud;
		wxTextCtrl* TextCtrl_WorkDir;
		//*)
		void OnSpinCtrl_PasSvgChange_pub(wxSpinEvent& event);
		wxString str;
		wxString defaultPath;

	protected:

		//(*Identifiers(Prefs_Dialog)
		static const long ID_STATICTEXT1;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRLDOUBLE1;
		static const long ID_STATICTEXT3;
		static const long ID_SPINCTRLDOUBLE2;
		static const long ID_STATICLINE4;
		static const long ID_STATICTEXT4;
		static const long ID_SPINCTRLDOUBLE3;
		static const long ID_STATICLINE5;
		static const long ID_CHECKBOX1;
		static const long ID_STATICLINE3;
		static const long ID_CHECKBOX8;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_STATICLINE2;
		static const long ID_CHECKBOX4;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRLDOUBLE4;
		static const long ID_STATICTEXT6;
		static const long ID_SPINCTRLDOUBLE5;
		static const long ID_CHECKBOX6;
		static const long ID_CHECKBOX5;
		static const long ID_CHECKBOX11;
		static const long ID_CHECKBOX12;
		static const long ID_STATICLINE6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_SPINCTRL1;
		static const long ID_STATICLINE7;
		static const long ID_RADIOBOX1;
		static const long ID_RADIOBOX2;
		static const long ID_RADIOBOX3;
		static const long ID_STATICLINE8;
		static const long ID_CHECKBOX7;
		static const long ID_STATICLINE10;
		static const long ID_CHECKBOX9;
		static const long ID_CHECKBOX10;
		static const long ID_STATICLINE11;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL10;
		static const long ID_BUTTON1;
		static const long ID_STATICLINE9;
		static const long ID_STATICTEXT10;
		static const long ID_SPINCTRL2;
		static const long ID_STATICLINE12;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(Prefs_Dialog)
		void OnButton_OKClick(wxCommandEvent& event);
		void OnSpinButton2Change(wxSpinEvent& event);
		void OnSpinButton3Change(wxSpinEvent& event);
		void OnSpinButton4Change(wxSpinEvent& event);
		void OnSpinButton5Change(wxSpinEvent& event);
		void OnSpinButton6Change(wxSpinEvent& event);
		void OnButton_tmp_repClick(wxCommandEvent& event);
		void OnTextCtrl1Text(wxCommandEvent& event);
		void OnCheckBox_AntialiasingSoftClick(wxCommandEvent& event);
		void OnButton_ResetClick(wxCommandEvent& event);
		void OnRadioBox_TrackballSelect(wxCommandEvent& event);
		void OnTextCtrl2Text(wxCommandEvent& event);
		void OnTextCtrl3Text(wxCommandEvent& event);
		void OnCheckBox_DisplayFpsClick(wxCommandEvent& event);
		void OnSpinCtrlDouble_axesChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_srcChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_normChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_SeuilGouraudChange(wxSpinDoubleEvent& event);
		void OnSpinCtrlDouble_SeuilGouraud2Change(wxSpinDoubleEvent& event);
		void OnCheckBox_SeuillageClick(wxCommandEvent& event);
		void OnCheckBox_TestDecalage3DSClick(wxCommandEvent& event);
		void OnCheckBox_LectureOptimiseeClick(wxCommandEvent& event);
		void OnCheckBox_CalculNormalesClick(wxCommandEvent& event);
		void OnCheckBox_RecNormales_SeuilleesClick(wxCommandEvent& event);
		void OnCheckBox_1SeulObjet3DClick(wxCommandEvent& event);
		void OnRadioBox_TriangulationSelect(wxCommandEvent& event);
		void OnCheckBox_CreerBackupClick(wxCommandEvent& event);
		void OnCheckBox_SupprBackupClick(wxCommandEvent& event);
		void OnTextCtrl_WorkDirText(wxCommandEvent& event);
		void OnSpinCtrl_PasSvgChange(wxSpinEvent& event);
		void OnCheckBox_NotFlatClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnSpinCtrl_ThreadsChange(wxSpinEvent& event);
		void OnCheckBox_TraiterDoublonsAretesClick(wxCommandEvent& event);
		void OnRadioBox_IconSizeSelect(wxCommandEvent& event);
		//*)
		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
