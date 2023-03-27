#ifndef ICOSAEDRE_H
#define ICOSAEDRE_H

//(*Headers(Icosaedre)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "OvniMain.h"

class Icosaedre: public wxDialog
{
	public:

		Icosaedre(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Icosaedre();
//		wxWindow* Icosaedre_Parent;
		int numeroGroupe  = 0;
		int numeroMateriau= 0;

		//(*Declarations(Icosaedre)
		wxButton* Button_Annuler;
		wxButton* Button_OK;
		wxSpinCtrl* SpinCtrl_Groupe;
		wxSpinCtrl* SpinCtrl_Materiau;
		wxSpinCtrl* SpinCtrl_Subdiv;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxTextCtrl* TextCtrl_Rayon;
		wxTextCtrl* TextCtrl_X;
		wxTextCtrl* TextCtrl_Y;
		wxTextCtrl* TextCtrl_Z;
		//*)

	protected:

		//(*Identifiers(Icosaedre)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL3;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT7;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT8;
		static const long ID_SPINCTRL2;
		//*)

	private:

		//(*Handlers(Icosaedre)
		void OnClose(wxCloseEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnButton_AnnulerClick(wxCommandEvent& event);
		void OnSpinCtrl_GroupeChange(wxSpinEvent& event);
		void OnSpinCtrl_MateriauChange(wxSpinEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)
		OvniFrame* MAIN;
        float centre_primitive[3];
		float rayon;
		void genereIcosaedre();
		void Genere_Sommets         (BddInter*, Object *);
        void Genere_Normales_Sommets(BddInter* , Object *);
        void genereSubdivisions(int);

		DECLARE_EVENT_TABLE()
};

#endif
