#ifndef CONE_H
#define CONE_H

//(*Headers(Cone)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "Interface.h"

class Cone: public wxDialog
{
	public:

		Cone(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Cone();
//		wxWindow* Cone_Parent;
		int numeroGroupe  = 0;
		int numeroMateriau= 0;

		//(*Declarations(Cone)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxCheckBox* CheckBox_FermerCone;
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
		wxTextCtrl* TextCtrl_Longueur;
		wxTextCtrl* TextCtrl_NbPoints;
		wxTextCtrl* TextCtrl_Rayon;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Cone)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL6;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT8;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT9;
		static const long ID_SPINCTRL3;
		//*)

	private:

		//(*Handlers(Cone)
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		void OnClose(wxCloseEvent& event);
		void OnSpinCtrl_NbSecteursChange(wxSpinEvent& event);
		//*)
        OvniFrame* MAIN;

        float centre_primitive[3];
		float longueur;
		float rayon;
		int   NbPoints;
		int   n_slices;
		int   n_secteurs;

		void genereCone();
		void Genere_Facettes        (BddInter*, Object*);
		void Genere_Sommets         (BddInter*, Object*);
        void Genere_Luminances      (BddInter*);
        void Genere_Normales_Sommets(BddInter*, Object*);

		DECLARE_EVENT_TABLE()
};

#endif
