// AppPropertySheet.cpp: implementation of the CAppPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppPropertySheet.h"

//////////////////////////////////////////////////////////////////////
// Construction

CAppPropertySheet::CAppPropertySheet ( _U_STRINGorID title, UINT uStartPage, 
                                       HWND hWndParent ) :
    CPropertySheetImpl<CAppPropertySheet> ( title, uStartPage, hWndParent ),
    m_bCentered(false)
{
    m_psh.dwFlags |= PSH_NOAPPLYNOW;

    AddPage ( m_pgBackground );
    AddPage ( m_pgAbout );
}


//////////////////////////////////////////////////////////////////////
// Message handlers

LRESULT CAppPropertySheet::OnPageInit ( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( !m_bCentered )
        {
        m_bCentered = true;
        CenterWindow ( m_psh.hwndParent );
        }

    return 0;
}
