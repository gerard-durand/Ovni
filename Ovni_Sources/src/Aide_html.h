#ifndef AIDE_HTML_H
#define AIDE_HTML_H

//(*Headers(Aide_html)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/html/htmlwin.h>
//*)

//#include "OvniMain.h"

class Aide_html: public wxDialog
{
	public:

		Aide_html(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Aide_html();

		//(*Declarations(Aide_html)
		wxButton* Button_OK;
		wxHtmlWindow* HtmlWindow_Aide;
		//*)

	protected:

		//(*Identifiers(Aide_html)
		static const long ID_HTMLWINDOW1;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(Aide_html)
		void OnClose(wxCloseEvent& event);
		void OnButton_OKClick(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		//*)

        int posx_button, posy_button, sizex_html, sizey_html;
//		OvniFrame* MAIN;

		DECLARE_EVENT_TABLE()
};

#endif
