#ifndef MODIFICATIONPANEL_H
#define MODIFICATIONPANEL_H

//(*Headers(ModificationPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
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
		wxStaticLine* StaticLine1;
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

		// Gettet / Setter
		void SetActiverTransparence(bool value) {
		    activer_transparence = value;
		};
		bool GetActiverTransparence() {
		    return activer_transparence;
		};
		int GetNumeroGroupe() {
		    return NumeroGroupe;
		};
		int GetNumeroMateriau() {
		    return NumeroMateriau;
		};
		bool GetFacetteCreeePlane() {
		    return FacetteCreeePlane;
		};
		void SetAretesCalculees(bool value) {
		    aretes_calculees = value;
		};
		bool GetBoolSouder() {
		    return Bool_souder;
		};
		bool GetBoolDiviser() {
		    return Bool_diviser;
		};
		int GetDivision() {
		    return division;
		};

	protected:

		//(*Identifiers(ModificationPanel)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_TOGGLEBUTTON1;
		static const wxWindowID ID_SPINCTRL1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_TOGGLEBUTTON2;
		static const wxWindowID ID_PANEL2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_SPINCTRL2;
		static const wxWindowID ID_SPINCTRL3;
		static const wxWindowID ID_TOGGLEBUTTON4;
		static const wxWindowID ID_BUTTON4;
		static const wxWindowID ID_BUTTON5;
		static const wxWindowID ID_BUTTON6;
		static const wxWindowID ID_CHECKBOX2;
		static const wxWindowID ID_PANEL4;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_TOGGLEBUTTON3;
		static const wxWindowID ID_BUTTON8;
		static const wxWindowID ID_STATICLINE1;
		static const wxWindowID ID_PANEL6;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_BUTTON9;
		static const wxWindowID ID_BUTTON10;
		static const wxWindowID ID_BUTTON11;
		static const wxWindowID ID_BUTTON12;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_CHECKBOX1;
		static const wxWindowID ID_CHECKBOX3;
		static const wxWindowID ID_PANEL8;
		static const wxWindowID ID_BUTTON13;
		static const wxWindowID ID_PANEL9;
		static const wxWindowID ID_PANEL10;
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
		void OnPanel6Paint(wxPaintEvent& event);
		//*)

        OvniFrame* MAIN;

        int  old_modeGL;
        int  old_mode_selection;
        bool old_show_points;
        bool old_show_lines;
        bool Bool_souder          = false;
        bool Bool_diviser         = false;
        int  division;
        bool activer_transparence = false;
        bool FacetteCreeePlane    = false;
        bool aretes_calculees;

        int  NumeroMateriau = 0, NumeroGroupe = 0;

		DECLARE_EVENT_TABLE()
};

#endif
