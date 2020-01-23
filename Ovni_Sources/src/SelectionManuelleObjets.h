#ifndef SELECTIONMANUELLEOBJETS_H
#define SELECTIONMANUELLEOBJETS_H

//(*Headers(SelectionManuelleObjets)
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/dialog.h>
//*)

#include "OvniMain.h"

class SelectionManuelleObjets: public wxDialog
{
	public:

		SelectionManuelleObjets(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SelectionManuelleObjets();

		//(*Declarations(SelectionManuelleObjets)
		wxButton* Button_Aucun;
		wxButton* Button_Quitter;
		wxButton* Button_Tous;
		wxCheckListBox* CheckListBox1;
		//*)

	protected:

		//(*Identifiers(SelectionManuelleObjets)
		static const long ID_CHECKLISTBOX1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		//*)

	private:

		//(*Handlers(SelectionManuelleObjets)
		void OnClose(wxCloseEvent& event);
		void OnButton_AucunClick(wxCommandEvent& event);
		void OnButton_TousClick(wxCommandEvent& event);
		void OnButton_QuitterClick(wxCommandEvent& event);
		void OnCheckListBox1Toggled(wxCommandEvent& event);
		//*)
		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
