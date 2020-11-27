#ifndef SELECTIONPANEL_H
#define SELECTIONPANEL_H

//(*Headers(SelectionPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class SelectionPanel: public wxDialog
{
	public:

		SelectionPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SelectionPanel();
//        wxWindow* Selection_Parent;
//        ListeSelected UndoListe;

		//(*Declarations(SelectionPanel)
		wxButton* Button_Appliquer;
		wxButton* Button_Centrer;
		wxButton* Button_Delete;
		wxButton* Button_Etendre;
		wxButton* Button_Fusionner;
		wxButton* Button_InverserNormales;
		wxButton* Button_InverserParcours;
		wxButton* Button_Manipulations;
		wxButton* Button_Masquer;
		wxButton* Button_OuvrirReperage;
		wxButton* Button_Quitter;
		wxButton* Button_Reafficher;
		wxButton* Button_SelectionManuelleFacettes;
		wxButton* Button_UndoDelete;
		wxButton* Button_UndoMasquer;
		wxButton* Button_UndoNormales;
		wxCheckBox* CheckBox_ForcerFlat;
		wxRadioBox* RadioBox_GrpMat;
		wxRadioBox* RadioBox_Selection;
		wxRadioBox* RadioBox_TypeSelection;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText9;
		wxStaticText* StaticText_Changer;
		wxStaticText* StaticText_Fac;
		wxStaticText* StaticText_Grp;
		wxStaticText* StaticText_Mat;
		wxStaticText* StaticText_NumerosUtilises;
		wxStaticText* StaticText_Selection;
		wxStaticText* StaticText_TypeSelection;
		wxTextCtrl* TextCtrl_GrpMat;
		wxTextCtrl* TextCtrl_IndFacette;
		wxTextCtrl* TextCtrl_IndObjet;
		wxTextCtrl* TextCtrl_NomObjet;
		wxTextCtrl* TextCtrl_NumFacette;
		wxTextCtrl* TextCtrl_NumGroupe;
		wxTextCtrl* TextCtrl_NumMateriau;
		wxTextCtrl* TextCtrl_NumObjet;
		wxTextCtrl* TextCtrl_NumerosUtilises;
		wxTextCtrl* TextCtrl_Selection;
		//*)

	protected:

		//(*Identifiers(SelectionPanel)
		static const long ID_RADIOBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL9;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL10;
		static const long ID_TEXTCTRL4;
		static const long ID_TEXTCTRL5;
		static const long ID_TEXTCTRL6;
		static const long ID_RADIOBOX2;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT11;
		static const long ID_TEXTCTRL8;
		static const long ID_BUTTON3;
		static const long ID_CHECKBOX1;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		static const long ID_BUTTON6;
		static const long ID_BUTTON7;
		static const long ID_BUTTON8;
		static const long ID_BUTTON9;
		static const long ID_BUTTON15;
		static const long ID_BUTTON10;
		static const long ID_BUTTON11;
		static const long ID_BUTTON12;
		static const long ID_RADIOBOX3;
		static const long ID_STATICTEXT12;
		static const long ID_BUTTON13;
		static const long ID_BUTTON14;
		static const long ID_BUTTON16;
		//*)

	private:

		//(*Handlers(SelectionPanel)
		void OnInit(wxInitDialogEvent& event);
		void OnRadioBox2Select(wxCommandEvent& event);
		void OnRadioBox1Select(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnRadioBox_SelectionSelect(wxCommandEvent& event);
		void OnRadioBox_GrpMatSelect(wxCommandEvent& event);
		void OnButton_AppliquerClick(wxCommandEvent& event);
		void OnButton_InverserNormalesClick(wxCommandEvent& event);
		void OnButton_UndoNormalesClick(wxCommandEvent& event);
		void OnButton_DeleteClick(wxCommandEvent& event);
		void OnButton_UndoDeleteClick(wxCommandEvent& event);
		void OnButton_PermutationClick(wxCommandEvent& event);
		void OnButton_MasquerClick(wxCommandEvent& event);
		void OnButton_ReafficherClick(wxCommandEvent& event);
		void OnButton_CentrerClick(wxCommandEvent& event);
		void OnButton_ManipulationsClick(wxCommandEvent& event);
		void OnRadioBox_TypeSelectionSelect(wxCommandEvent& event);
		void OnButton_EtendreClick(wxCommandEvent& event);
		void OnButton_UndoMasquerClick(wxCommandEvent& event);
		void OnButton_InversionParcoursClick(wxCommandEvent& event);
		void OnButton_InverserParcoursClick(wxCommandEvent& event);
		void OnTextCtrl_NomObjetText(wxCommandEvent& event);
		void OnButton_SelectionManuelleFacettesClick(wxCommandEvent& event);
		void OnButton_OuvrirReperageClick(wxCommandEvent& event);
		void OnCheckBox_ForcerFlatClick(wxCommandEvent& event);
		void OnButton_FusionnerClick(wxCommandEvent& event);
		//*)
		void InitPanel();
		void Reset_ListeObjets();
		void ToDo();
        OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
