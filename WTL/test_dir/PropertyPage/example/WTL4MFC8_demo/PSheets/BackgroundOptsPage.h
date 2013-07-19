// BackgroundOptsPage.h: interface for the CBackgroundOptsPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDOPTSPAGE_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
#define AFX_BACKGROUNDOPTSPAGE_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CBackgroundOptsPage : public CPropertyPageImpl<CBackgroundOptsPage>,
                            public CWinDataExchange<CBackgroundOptsPage>
{
public:
    enum { IDD = IDD_BACKGROUND_OPTS };

    // Construction
    CBackgroundOptsPage();
    ~CBackgroundOptsPage();

    // Maps
    BEGIN_MSG_MAP(CBackgroundOptsPage)
        MSG_WM_INITDIALOG(OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CBackgroundOptsPage>)
    END_MSG_MAP()

    BEGIN_DDX_MAP(CBackgroundOptsPage)
        DDX_RADIO(IDC_BLUE, m_nColor)
        DDX_RADIO(IDC_ALYSON, m_nPicture)
    END_DDX_MAP()

    // Message handlers
    BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );

    // Property page notification handlers
    int OnApply();

    // DDX variables
    int m_nColor, m_nPicture;
};

#endif // !defined(AFX_BACKGROUNDOPTSPAGE_H__50D86681_297A_4F56_8081_A34FF5F5DC80__INCLUDED_)
