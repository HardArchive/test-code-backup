// WizBkColorPage.h: interface for the CWizBkColorPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZBKCOLORPAGE_H__4534DD99_E00D_438F_ACEC_156B8CBF385A__INCLUDED_)
#define AFX_WIZBKCOLORPAGE_H__4534DD99_E00D_438F_ACEC_156B8CBF385A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CWizBkColorPage : public CPropertyPageImpl<CWizBkColorPage>,
                        public CWinDataExchange<CWizBkColorPage>
{
public:
    enum { IDD = IDD_WIZARD_BKCOLOR };

    CWizBkColorPage();

    // Maps
    BEGIN_MSG_MAP(CWizBkColorPage)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CWizBkColorPage>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CWizBkColorPage)
        DDX_RADIO(IDC_BLUE, m_nColor)
        DDX_CHECK(IDC_FAIL_DDV, m_bFailDDV)
    END_DDX_MAP()
    
    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );

    // Notification handlers
    int OnSetActive();
    BOOL OnKillActive();

    // DDX vars
    int m_nColor;

protected:
    // DDX vars
    int m_bFailDDV;
};

#endif // !defined(AFX_WIZBKCOLORPAGE_H__4534DD99_E00D_438F_ACEC_156B8CBF385A__INCLUDED_)
