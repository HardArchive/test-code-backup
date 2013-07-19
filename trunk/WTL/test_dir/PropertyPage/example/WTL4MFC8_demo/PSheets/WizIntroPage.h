// WizIntroPage.h: interface for the CWizIntroPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZINTROPAGE_H__75D3CD66_E712_472A_B04C_AE7C64ED445F__INCLUDED_)
#define AFX_WIZINTROPAGE_H__75D3CD66_E712_472A_B04C_AE7C64ED445F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CWizIntroPage : public CPropertyPageImpl<CWizIntroPage>
{
public:
    enum { IDD = IDD_WIZARD_INTRO };

    // Construction
    CWizIntroPage();

    // Maps
    BEGIN_MSG_MAP(COptionsWizard)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CWizIntroPage>)
    END_MSG_MAP()

    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );

    // Notification handlers
    int OnSetActive();
};

#endif // !defined(AFX_WIZINTROPAGE_H__75D3CD66_E712_472A_B04C_AE7C64ED445F__INCLUDED_)
