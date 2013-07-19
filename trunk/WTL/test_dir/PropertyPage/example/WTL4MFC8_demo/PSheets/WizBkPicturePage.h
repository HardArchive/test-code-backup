// WizBkPicturePage.h: interface for the CWizBkPicturePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIZBKPICTUREPAGE_H__C1444411_5E25_40B3_BD93_269E2FA48995__INCLUDED_)
#define AFX_WIZBKPICTUREPAGE_H__C1444411_5E25_40B3_BD93_269E2FA48995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CWizBkPicturePage : public CPropertyPageImpl<CWizBkPicturePage>,
                          public CWinDataExchange<CWizBkPicturePage>
{
public:
    enum { IDD = IDD_WIZARD_BKPICTURE };

    CWizBkPicturePage();

    // Maps
    BEGIN_MSG_MAP(CWizBkPicturePage)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CWizBkPicturePage>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CWizBkPicturePage)
        DDX_RADIO(IDC_ALYSON, m_nPicture)
    END_DDX_MAP()

    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );

    // Notification handlers
    int OnSetActive();
    BOOL OnKillActive();

    // DDX vars
    int m_nPicture;
};

#endif // !defined(AFX_WIZBKPICTUREPAGE_H__C1444411_5E25_40B3_BD93_269E2FA48995__INCLUDED_)
