#include "PreferencesPanel.h"

//(*InternalHeaders(PreferencesPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(PreferencesPanel)
//*)

BEGIN_EVENT_TABLE(PreferencesPanel,wxFrame)
	//(*EventTable(PreferencesPanel)
	//*)
END_EVENT_TABLE()

PreferencesPanel::PreferencesPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PreferencesPanel)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

PreferencesPanel::~PreferencesPanel()
{
	//(*Destroy(PreferencesPanel)
	//*)
}

