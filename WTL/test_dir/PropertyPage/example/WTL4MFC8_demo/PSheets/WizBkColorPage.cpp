// WizBkColorPage.cpp: implementation of the CWizBkColorPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WizBkColorPage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CWizBkColorPage::CWizBkColorPage() :
    CPropertyPageImpl<CWizBkColorPage>( IDS_WIZARD_TITLE ),
    m_nColor(0), m_bFailDDV(0)
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CWizBkColorPage::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
    DoDataExchange(false);
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Notification handlers

int CWizBkColorPage::OnSetActive()
{
    SetWizardButtons ( PSWIZB_BACK | PSWIZB_NEXT );
    return 0;
}

int CWizBkColorPage::OnKillActive()
{
    if ( !DoDataExchange(true) )
        return TRUE;    // prevent deactivation

    if ( m_bFailDDV )
        {
        MessageBox ( _T("Error box checked, wizard will stay on this page."),
                     _T("PSheets"), MB_ICONERROR );

        return TRUE;    // prevent deactivation
        }

    return FALSE;       // allow deactivation
}
