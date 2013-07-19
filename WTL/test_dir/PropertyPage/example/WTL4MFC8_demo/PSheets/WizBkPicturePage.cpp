// WizBkPicturePage.cpp: implementation of the CWizBkPicturePage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WizBkPicturePage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CWizBkPicturePage::CWizBkPicturePage() :
    CPropertyPageImpl<CWizBkPicturePage>( IDS_WIZARD_TITLE ),
    m_nPicture(0)
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CWizBkPicturePage::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
    DoDataExchange(false);
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Notification handlers

int CWizBkPicturePage::OnSetActive()
{
    SetWizardButtons ( PSWIZB_BACK | PSWIZB_NEXT );
    return 0;
}

int CWizBkPicturePage::OnKillActive()
{
    if ( !DoDataExchange(true) )
        return TRUE;    // prevent deactivation

    return FALSE;       // allow deactivation
}
