#ifndef REPERAGEOBJET_H
#define REPERAGEOBJET_H

//(*Headers(ReperageObjet)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class ReperageObjet: public wxDialog
{
	public:

		ReperageObjet(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ReperageObjet();
//		wxWindow* Obj_Parent;

		//(*Declarations(ReperageObjet)
		wxButton* Button_InverserNormales;
		wxButton* Button_OK;
		wxButton* Button_renommer;
		wxCheckBox* CheckBox_masquer;
		wxCheckBox* CheckBox_renommer;
		wxCheckBox* CheckBox_supprimer;
		wxSpinButton* SpinButton_indice;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* TextCtrl_NomObjet;
		wxTextCtrl* TextCtrl_NumObjet;
		wxTextCtrl* TextCtrl_indice;
		//*)
		OvniFrame* MAIN;

	protected:

		//(*Identifiers(ReperageObjet)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_SPINBUTTON1;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT2;
		static const long ID_BUTTON1;
		static const long ID_CHECKBOX1;
		static const long ID_CHECKBOX2;
		static const long ID_CHECKBOX3;
		static const long ID_BUTTON3;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(ReperageObjet)
		void OnButton_OKClick(wxCommandEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnCheckBox_masquerClick(wxCommandEvent& event);
		void OnCheckBox_supprimerClick(wxCommandEvent& event);
		void OnButton_InverserNormalesClick(wxCommandEvent& event);
		void OnSpinButton_indiceChange(wxSpinEvent& event);
		void OnCheckBox_renommerClick(wxCommandEvent& event);
		void OnButton_renommerClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
