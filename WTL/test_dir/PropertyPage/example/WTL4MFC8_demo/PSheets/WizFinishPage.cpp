// WizFinishPage.cpp: implementation of the CWizFinishPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WizFinishPage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CWizFinishPage::CWizFinishPage() :
    CPropertyPageImpl<CWizFinishPage>( IDS_WIZARD_TITLE )
{
}


//////////////////////////////////////////////////////////////////////
// Notification handlers

int CWizFinishPage::OnSetActive()
{
    SetWizardButtons ( PSWIZB_BACK | PSWIZB_FINISH );
    return 0;
}
