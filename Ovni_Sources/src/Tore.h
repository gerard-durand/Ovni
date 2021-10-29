#ifndef TORE_H
#define TORE_H

//(*Headers(Tore)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class Tore: public wxDialog
{
	public:

		Tore(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Tore();
		int numeroGroupe  = 0;
		int numeroMateriau= 0;

		//(*Declarations(Tore)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl_NbPts;
		wxTextCtrl* TextCtrl_NbSecteurs;
		wxTextCtrl* TextCtrl_RayonCercle;
		wxTextCtrl* TextCtrl_RayonMoyen;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Tore)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL3;
		static const long ID_TEXTCTRL4;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL6;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_SPINCTRL1;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(Tore)
		void OnClose(wxCloseEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		//*)

		OvniFrame *MAIN;

        float centre_primitive[3];
		float rayon_moyen;
		float rayon_cercle;
		int   NbPoints;
		int   n_secteurs;

		void genereTore();
		void genereFacettes(BddInter*, Object*);
		void genereSommets (BddInter*, Object*);

		DECLARE_EVENT_TABLE()
};

#endif
