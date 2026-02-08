#ifndef CYLINDRE_H
#define CYLINDRE_H

//(*Headers(Cylindre)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "Interface.h"

class Cylindre: public wxDialog
{
	public:

		Cylindre(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Cylindre();
//		wxWindow* Cylindre_Parent;
//		int numeroGroupe  = 0;
//		int numeroMateriau= 0;

		//(*Declarations(Cylindre)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxCheckBox* CheckBox_FermerCylindre;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxSpinCtrl* SpinCtrl_NbSecteurs;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl_Arete;
		wxTextCtrl* TextCtrl_NbPoints;
		wxTextCtrl* TextCtrl_Rayon;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Cylindre)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_TEXTCTRL1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_TEXTCTRL2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_TEXTCTRL3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_TEXTCTRL4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_TEXTCTRL5;
		static const wxWindowID ID_TEXTCTRL6;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_BUTTON1;
		static const wxWindowID ID_BUTTON2;
		static const wxWindowID ID_CHECKBOX1;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_SPINCTRL1;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_SPINCTRL2;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_SPINCTRL3;
		//*)

	private:

		//(*Handlers(Cylindre)
		void OnClose(wxCloseEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		void OnSpinCtrl_NbSecteursChange(wxSpinEvent& event);
		//*)

		OvniFrame* MAIN;

        float centre_primitive[3];
		float arete;
		float rayon;
		int   NbPoints;
		int   n_slices;
		int   n_secteurs;

		int numeroGroupe   = 0;
		int numeroMateriau = 0;

		void genereCylindre();
		void Genere_Facettes        (BddInter*, Object*);
		void Genere_Sommets         (BddInter*, Object*);
        void Genere_Luminances      (BddInter*);
        void Genere_Normales_Sommets(BddInter*, Object*);

		DECLARE_EVENT_TABLE()
};

#endif
