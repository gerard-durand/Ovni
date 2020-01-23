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
		wxString str;

	protected:

		//(*Identifiers(PropertiesPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		static const long ID_STATICTEXT21;
		static const long ID_STATICTEXT22;
		static const long ID_STATICTEXT23;
		static const long ID_STATICTEXT24;
		static const long ID_STATICTEXT25;
		static const long ID_STATICTEXT26;
		static const long ID_STATICTEXT27;
		static const long ID_BUTTON1;
		//*)

	private:

		//(*Handlers(PropertiesPanel)
		void OnBoutonOKClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
