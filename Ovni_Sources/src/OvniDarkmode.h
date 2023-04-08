#ifndef OVNIDARKMODE_H_INCLUDED
#define OVNIDARKMODE_H_INCLUDED

// Pour test du darkmode sous Windows à partir de wxWidgets 3.3.0 (beta)
#if defined(__WXMSW__)
#if wxCHECK_VERSION(3,3,0)

#include <wx/msw/darkmode.h>
class MySettings : public wxDarkModeSettings
{
public:
    virtual wxColour GetMenuColour(wxMenuColour which)
    {
        if ( which == wxMenuColour::StandardFg )
            return *wxCYAN;

        return wxDarkModeSettings::GetMenuColour(which);
    }

    wxColour GetColour(wxSystemColour index) override
    {
    switch ( index )
        {
        case wxSYS_COLOUR_MENUTEXT:
        case wxSYS_COLOUR_WINDOWTEXT:
        case wxSYS_COLOUR_CAPTIONTEXT:
        case wxSYS_COLOUR_HIGHLIGHTTEXT:
        case wxSYS_COLOUR_BTNTEXT:
        case wxSYS_COLOUR_INFOTEXT:
        case wxSYS_COLOUR_LISTBOXTEXT:
        case wxSYS_COLOUR_LISTBOXHIGHLIGHTTEXT:
            // Default colour used here is 0xe0e0e0.
            return *wxCYAN;

        default:
            return wxDarkModeSettings::GetColour(index);
        }
    }
};
#endif // wxCHECK_VERSION
#endif // __WXMSW__

#endif // OVNIDARKMODE_H_INCLUDED
