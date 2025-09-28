#ifndef SELECTIONPANEL_H
#define SELECTIONPANEL_H

//(*Headers(SelectionPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
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
		wxRadioButton* RadioButton_Grp;
		wxRadioButton* RadioButton_Mat;
		wxRadioButton* RadioButton_Selection_Facettes;
		wxRadioButton* RadioButton_Selection_Objets;
		wxRadioButton* RadioButton_Selection_Points;
		wxRadioButton* RadioButton_TypeSelection_Arriere;
		wxRadioButton* RadioButton_TypeSelection_Avant;
		wxRadioButton* RadioButton_TypeSelection_Both;
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
		static const wxWindowID ID_RADIOBUTTON3;
		static const wxWindowID ID_RADIOBUTTON4;
		static const wxWindowID ID_RADIOBUTTON5;
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_TEXTCTRL2;
		static const wxWindowID ID_TEXTCTRL9;
		static const wxWindowID ID_TEXTCTRL3;
		static const wxWindowID ID_TEXTCTRL10;
		static const wxWindowID ID_TEXTCTRL4;
		static const wxWindowID ID_TEXTCTRL5;
		static const wxWindowID ID_TEXTCTRL6;
		static const wxWindowID ID_RADIOBUTTON1;
		static const wxWindowID ID_RADIOBUTTON2;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_STATICTEXT10;
		static const wxWindowID ID_TEXTCTRL7;
		static const wxWindowID ID_STATICTEXT11;
		static const wxWindowID ID_TEXTCTRL8;
		static const wxWindowID ID_BUTTON3;
		static const wxWindowID ID_CHECKBOX1;
		static const wxWindowID ID_BUTTON4;
		static const wxWindowID ID_BUTTON5;
		static const wxWindowID ID_BUTTON6;
		static const wxWindowID ID_BUTTON7;
		static const wxWindowID ID_BUTTON8;
		static const wxWindowID ID_BUTTON9;
		static const wxWindowID ID_BUTTON15;
		static const wxWindowID ID_BUTTON10;
		static const wxWindowID ID_BUTTON11;
		static const wxWindowID ID_BUTTON12;
		static const wxWindowID ID_RADIOBUTTON6;
		static const wxWindowID ID_RADIOBUTTON7;
		static const wxWindowID ID_RADIOBUTTON8;
		static const wxWindowID ID_STATICTEXT12;
		static const wxWindowID ID_BUTTON13;
		static const wxWindowID ID_BUTTON14;
		static const wxWindowID ID_BUTTON16;
		//*)

	private:

		//(*Handlers(SelectionPanel)
		void OnInit(wxInitDialogEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnRadioButton_SelectionSelect(wxCommandEvent& event);
		void OnRadioButton_GrpMatSelect(wxCommandEvent& event);
		void OnButton_AppliquerClick(wxCommandEvent& event);
		void OnButton_InverserNormalesClick(wxCommandEvent& event);
		void OnButton_UndoNormalesClick(wxCommandEvent& event);
		void OnButton_DeleteClick(wxCommandEvent& event);
		void OnButton_UndoDeleteClick(wxCommandEvent& event);
		void OnButton_MasquerClick(wxCommandEvent& event);
		void OnButton_ReafficherClick(wxCommandEvent& event);
		void OnButton_CentrerClick(wxCommandEvent& event);
		void OnButton_ManipulationsClick(wxCommandEvent& event);
		void OnRadioButton_TypeSelectionSelect(wxCommandEvent& event);
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

        OvniFrame* MAIN;

		void InitPanel();
		void Reset_ListeObjets();
//		void ToDo();

		DECLARE_EVENT_TABLE()
};

#endif
