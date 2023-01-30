#include "Aide_html.h"

//(*InternalHeaders(Aide_html)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Aide_html)
const long Aide_html::ID_HTMLWINDOW1 = wxNewId();
const long Aide_html::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Aide_html,wxDialog)
	//(*EventTable(Aide_html)
	//*)
END_EVENT_TABLE()

Aide_html::Aide_html(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
/** \brief Aide_html::Aide_html Affichage d'une aide d'Ovni au format Html
 *
 * \param parent        : Pointeur sur la fenêtre parent
 * \param id            : identifiant de la boîte de dialogue
 * \param pos           : position de la boîte de dialogue
 * \param size          : taille de la boîte de dialogue
 *
 */

	//(*Initialize(Aide_html)
	Create(parent, id, _T("Aide Html"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxFULL_REPAINT_ON_RESIZE, _T("id"));
	SetClientSize(wxSize(640,480));
	Move(wxDefaultPosition);
	HtmlWindow_Aide = new wxHtmlWindow(this, ID_HTMLWINDOW1, wxPoint(8,8), wxSize(624,432), wxHW_SCROLLBAR_AUTO, _T("ID_HTMLWINDOW1"));
	HtmlWindow_Aide->LoadPage(_T("file://../help/Aide.html"));
	Button_OK = new wxButton(this, ID_BUTTON1, _T("Quitter"), wxPoint(544,448), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Aide_html::OnButton_OKClick);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Aide_html::OnClose);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&Aide_html::OnResize);
	//*)
	this->MAIN = dynamic_cast<OvniFrame*>(parent);
	wxSize s;
	s = this->GetClientSize();
	sizex_html = s.GetWidth();      // Sauvegarde de la taille initiale du dialogue
	sizey_html = s.GetHeight();
//	printf("html : %d %d\n",sizex_html,sizey_html);
	wxPoint p;
	p = Button_OK->GetPosition();
	posx_button = p.x;              // Sauvegarde de la position initiale du bouton
	posy_button = p.y;
//	printf("butt : %d %d\n",posx_button,posy_button);
}

Aide_html::~Aide_html()
{
	//(*Destroy(Aide_html)
	//*)
}


void Aide_html::OnClose(wxCloseEvent& event)
{
    Hide();
}

void Aide_html::OnButton_OKClick(wxCommandEvent& event)
{
// Boutton OK <=> OnClose
    wxCloseEvent close_event;
    OnClose(close_event);
}

void Aide_html::OnResize(wxSizeEvent& event)
{
/** \brief Aide_html::OnResize Redimensionner l'aide d'Ovni au format Html
 *
 * \param event
 *
 */

    int offset_x, offset_y;
    wxSize  s;
    wxPoint p;
    s = event.GetSize();    // Récupère la taille de la fenêtre globale
//    printf("NewH : %d %d\n",s.GetWidth(),s.GetHeight()); // s.x = s.GetWidth()
    offset_x = s.x - sizex_html -16;        // Pourquoi soustrait-on ces valeurs ? (sinon bouton pas bien placé)
    offset_y = s.y - sizey_html -38;

    // Déplacement du bouton
    p.x = posx_button + offset_x;
    p.y = posy_button + offset_y;
//    printf("newB : %d %d\n",p.x,p.y);
    Button_OK->SetPosition(p);

    // Ajustement de la taille de la fenêtre html
    s.x = sizex_html + offset_x -16;        // Idem ? (sinon taille trop grande)
    s.y = sizey_html + offset_y -50;
    HtmlWindow_Aide->SetSize(s);
}
