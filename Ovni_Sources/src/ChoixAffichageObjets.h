#ifndef CHOIXAFFICHAGEOBJETS_H
#define CHOIXAFFICHAGEOBJETS_H

//(*Headers(ChoixAffichageObjets)
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
//*)

#include "OvniMain.h"

class ChoixAffichageObjets: public wxDialog
{
	public:

		ChoixAffichageObjets(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ChoixAffichageObjets();
//		wxWindow* Chk_Parent;

		//(*Declarations(ChoixAffichageObjets)
		wxButton* Button_Aucun;
		wxButton* Button_Quitter;
		wxButton* Button_Tous;
		wxCheckListBox* CheckListBox1;
		wxStaticText* StaticText1;
		//*)

	protected:

		//(*Identifiers(ChoixAffichageObjets)
		static const long ID_STATICTEXT1;
		static const long ID_CHECKLISTBOX1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(ChoixAffichageObjets)
		void OnCheckListBox1Toggled(wxCommandEvent& event);
		void OnCheckListBox1Toggled1(wxCommandEvent& event);
		void OnButton_AucunClick(wxCommandEvent& event);
		void OnButton_TousClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnClose(wxCloseEvent& event);
		//*)
        OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
