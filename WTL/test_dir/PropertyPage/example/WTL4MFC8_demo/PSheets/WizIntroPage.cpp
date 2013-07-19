// WizIntroPage.cpp: implementation of the CWizIntroPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WizIntroPage.h"

//////////////////////////////////////////////////////////////////////
// Construction

CWizIntroPage::CWizIntroPage() :
    CPropertyPageImpl<CWizIntroPage>( IDS_WIZARD_TITLE )
{
}


//////////////////////////////////////////////////////////////////////
// Message handlers

BOOL CWizIntroPage::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
    GetPropertySheet().SendMessage ( UWM_CENTER_SHEET );
    return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Notification handlers

int CWizIntroPage::OnSetActive()
{
    SetWizardButtons ( PSWIZB_NEXT );
    return 0;
}
