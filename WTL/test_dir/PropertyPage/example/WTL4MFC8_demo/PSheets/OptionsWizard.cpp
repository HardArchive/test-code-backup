// OptionsWizard.cpp: implementation of the COptionsWizard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionsWizard.h"

//////////////////////////////////////////////////////////////////////
// Construction

COptionsWizard::COptionsWizard ( HWND hWndParent ) :
    CPropertySheetImpl<COptionsWizard> ( 0U, 0, hWndParent ),
    m_bCentered(false)
{
    SetWizardMode();

    AddPage ( m_pgIntro );
    AddPage ( m_pgBkcolor );
    AddPage ( m_pgBkpicture );
    AddPage ( m_pgFinish );
}

LRESULT COptionsWizard::OnPageInit ( UINT uMsg, WPARAM wParam ,LPARAM lParam )
{
    if ( !m_bCentered )
        {
        m_bCentered = true;
        CenterWindow ( m_psh.hwndParent );
        }

    return 0;
}
