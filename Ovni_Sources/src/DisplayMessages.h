#ifndef DISPLAYMESSAGES_H_INCLUDED
#define DISPLAYMESSAGES_H_INCLUDED

#include <wx/string.h>
#include "wx/msgdlg.h"
#include "wx/generic/msgdlgg.h"

void DisplayMessage(wxString , bool );
long DisplayMessage(wxString , wxString , long);
void DisplayBox    (wxString , wxString );

#endif // DISPLAYMESSAGES_H_INCLUDED
