#ifndef MODIFICATIONPANEL_H
#define MODIFICATIONPANEL_H

//(*Headers(ModificationPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
//*)

#include "OvniMain.h"

class ModificationPanel: public wxDialog
{
	public:

		ModificationPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ModificationPanel();
        bool Bool_souder  = false;
        bool Bool_diviser = false;
//        int toredraw=0;
        int  division;
        bool activer_transparence = false;
        int  NumeroMateriau = 0, NumeroGroupe = 0;
        bool FacetteCreeePlane = false;
        bool aretes_calculees;
//        wxWindow*  Mod_Parent;

		//(*Declarations(ModificationPanel)
		wxButton* Button_Annuler;
		wxButton* Button_InverserNormale;
		wxButton* Button_InverserTout;
		wxButton* Button_Quitter;
		wxButton* Button_RecalculerAretes;
		wxButton* Button_RecalculerNormales;
		wxButton* Button_Simplification;
		wxButton* Button_SupprimerFacette;
		wxButton* Button_Trianguler;
		wxButton* Button_Undo;
		wxCheckBox* CheckBox_FacettePlane;
		wxCheckBox* CheckBox_NotFlat;
		wxCheckBox* CheckBox_Transparence;
		wxPanel* Panel10;
		wxPanel* Panel2;
		wxPanel* Panel4;
		wxPanel* Panel6;
		wxPanel* Panel8;
		wxPanel* Panel9;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxSpinCtrl* SpinCtrl_NbSegments;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxTextCtrl* TextCtrl_Tolerance;
		wxToggleButton* ToggleButton_Ajouter;
		wxToggleButton* ToggleButton_CreerFacette;
		wxToggleButton* ToggleButton_Diviser;
		wxToggleButton* ToggleButton_Souder;
		//*)

	protected:

		//(*Identifiers(ModificationPanel)
		static const long ID_STATICTEXT1;
		static const long ID_TOGGLEBUTTON1;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TOGGLEBUTTON2;
		static const long ID_PANEL2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL2;
		static const long ID_SPINCTRL3;
		static const long ID_TOGGLEBUTTON4;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_CHECKBOX2;
		static const long ID_PANEL4;
		static const long ID_STATICTEXT6;
		static const long ID_TOGGLEBUTTON3;
		static const long ID_BUTTON8;
		static const long ID_PANEL6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON9;
		static const long ID_BUTTON10;
		static const long ID_BUTTON11;
		static const long ID_BUTTON12;
		static const long ID_BUTTON1;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX3;
		static const long ID_PANEL8;
		static const long ID_BUTTON13;
		static const long ID_PANEL10;
		static const long ID_PANEL9;
		//*)

	private:

		//(*Handlers(ModificationPanel)
		void OnToggleButton_AjouterToggle(wxCommandEvent& event);
		void OnToggleButton_DiviserToggle(wxCommandEvent& event);
		void OnToggleButton_CreerFacetteToggle(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnButton_SupprimerFacetteClick(wxCommandEvent& event);
		void OnButton_InverserNormaleClick(wxCommandEvent& event);
		void OnToggleButton_SouderToggle(wxCommandEvent& event);
		void OnButton_UndoClick(wxCommandEvent& event);
		void OnButton_InverserToutClick(wxCommandEvent& event);
		void OnButton_TriangulerClick(wxCommandEvent& event);
		void OnButton_RecalculerNormalesClick(wxCommandEvent& event);
		void OnButton_SimplificationClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnSpinCtrl_NbSegmentsChange(wxSpinEvent& event);
		void OnCheckBox_TransparenceClick(wxCommandEvent& event);
		void OnTextCtrl_ToleranceText(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		void OnCheckBox_FacettePlaneClick(wxCommandEvent& event);
		void OnButton_RecalculerAretesClick(wxCommandEvent& event);
		void OnCheckBox_NotFlatClick(wxCommandEvent& event);
		//*)
        OvniFrame* MAIN;
        int  old_modeGL;
        int  old_mode_selection;
        bool old_show_points;
        bool old_show_lines;

		DECLARE_EVENT_TABLE()
};

#endif
