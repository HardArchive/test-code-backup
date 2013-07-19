// WizFinishPage.h: interface for the CWizFinishPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZFINISHPAGE_H__014E7ABF_9FEF_46E2_9726_462398D75EF6__INCLUDED_)
#define AFX_WIZFINISHPAGE_H__014E7ABF_9FEF_46E2_9726_462398D75EF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CWizFinishPage : public CPropertyPageImpl<CWizFinishPage>
{
public:
    enum { IDD = IDD_WIZARD_FINISH };

    CWizFinishPage();

    // Maps
    BEGIN_MSG_MAP(CWizFinishPage)
        CHAIN_MSG_MAP(CPropertyPageImpl<CWizFinishPage>)
    END_MSG_MAP()

    // Notification handlers
    int OnSetActive();
};

#endif // !defined(AFX_WIZFINISHPAGE_H__014E7ABF_9FEF_46E2_9726_462398D75EF6__INCLUDED_)
