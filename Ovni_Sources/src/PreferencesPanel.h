#ifndef PREFERENCESPANEL_H
#define PREFERENCESPANEL_H

//(*Headers(PreferencesPanel)
#include <wx/frame.h>
//*)

class PreferencesPanel: public wxFrame
{
	public:

		PreferencesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PreferencesPanel();

		//(*Declarations(PreferencesPanel)
		//*)

	protected:

		//(*Identifiers(PreferencesPanel)
		//*)

	private:

		//(*Handlers(PreferencesPanel)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
