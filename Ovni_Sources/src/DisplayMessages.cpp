#include "DisplayMessages.h"

// Appels centralis�s des wxGenericMessageDialog plut�t que wxMessageDialog pour �tre compatibles du Darkmode
// DisplayBox plut�t que wxMessageBox : il n'existe pas (encore) de wxGenericMessageBox !

void DisplayMessage(wxString wxMessage, bool bip)
{
    long style     = wxOK | wxICON_QUESTION;        // Avec l'ic�ne wxICON_QUESTION, l'affichage reste silencieux (wxICON_INFORMATION + logique, mais bruyant !!)
    if (bip) style = wxOK | wxICON_INFORMATION;

    wxGenericMessageDialog *query = new wxGenericMessageDialog(nullptr, wxMessage, _T("Avertissement"), style);
    query->ShowModal();
    query->Destroy();
}

long DisplayMessage(wxString wxMessage, wxString title, long style)
{
    wxGenericMessageDialog *query = new wxGenericMessageDialog(nullptr, wxMessage, title, style);
#if wxCHECK_VERSION(3,0,0)
    query->SetYesNoLabels(_T("Oui"),_T("Non"));     // Sinon par d�faut on a Yes No (car appel� avec au moins le style wxYES_NO)
#endif // wxCHECK_VERSION
    long retour_Show = query->ShowModal();
    query->Destroy();
    return retour_Show;
}

void DisplayBox(wxString wxMessage, wxString title)
{
// <=> wxMessageBox, sauf que la premi�re ligne n'est en mise en valeur de la m�me fa�on

    long style = wxOK | wxICON_INFORMATION;

    wxGenericMessageDialog *query = new wxGenericMessageDialog(nullptr, wxMessage, title, style);

    wxString extendedMessage = query->GetExtendedMessage();

    // Recopie/adaptation de ce qu'il y a dans src/msw/msgdlg.cpp (ligne 631 et +)
    // Teste si wxMessage � plusieurs lignes et si la premi�re peut �tre consid�r�e comme un titre principal du message

    if ( extendedMessage.empty() ) {
        // Check if there is a blank separating line after the first line (this
        // is not the same as searching for "\n\n" as we want the automatically
        // recognized main message be single line to avoid embarrassing false
        // positives).
        const size_t posNL = wxMessage.find('\n');
        if ( posNL != wxString::npos            &&
             posNL < wxMessage.length() - 1     &&
             wxMessage[posNL + 1 ] == '\n' )    {
            extendedMessage.assign(wxMessage, posNL + 2, wxString::npos);
            wxMessage.erase(posNL);
            // Reset de wxMessage et extendedMessage dans query
            query->SetMessage(wxMessage);               // 1�re ligne en gras
            query->SetExtendedMessage(extendedMessage); // puis la suite
        }
    }

    query->ShowModal();
    query->Destroy();
}

