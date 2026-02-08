#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

//(*Headers(PropertiesPanel)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
//*)

class PropertiesPanel: public wxDialog
{
	public:

		PropertiesPanel(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PropertiesPanel();

		//(*Declarations(PropertiesPanel)
		wxButton* BoutonOK;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* StaticTextNomBdd;
		wxStaticText* Texte_Box;
		wxStaticText* nb_3Points;
		wxStaticText* nb_4Points;
		wxStaticText* nb_aretes;
		wxStaticText* nb_facettes;
		wxStaticText* nb_groupes;
		wxStaticText* nb_materiaux;
		wxStaticText* nb_maxPoints;
		wxStaticText* nb_objets;
		wxStaticText* nb_points;
		wxStaticText* numero_facette;
		wxStaticText* numero_objet;
		//*)
//		wxString str;

	protected:

		//(*Identifiers(PropertiesPanel)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_STATICTEXT10;
		static const wxWindowID ID_STATICTEXT11;
		static const wxWindowID ID_STATICTEXT12;
		static const wxWindowID ID_STATICTEXT13;
		static const wxWindowID ID_STATICTEXT14;
		static const wxWindowID ID_STATICTEXT15;
		static const wxWindowID ID_STATICTEXT16;
		static const wxWindowID ID_STATICTEXT17;
		static const wxWindowID ID_STATICTEXT18;
		static const wxWindowID ID_STATICTEXT19;
		static const wxWindowID ID_STATICTEXT20;
		static const wxWindowID ID_STATICTEXT21;
		static const wxWindowID ID_STATICTEXT22;
		static const wxWindowID ID_STATICTEXT23;
		static const wxWindowID ID_STATICTEXT24;
		static const wxWindowID ID_STATICTEXT25;
		static const wxWindowID ID_STATICTEXT26;
		static const wxWindowID ID_STATICTEXT27;
		static const wxWindowID ID_BUTTON1;
		//*)

	private:

		//(*Handlers(PropertiesPanel)
		void OnBoutonOKClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
