// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "PSheetsView.h"
#include "MainFrm.h"
#include "AppPropertySheet.h"
#include "OptionsWizard.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
        return TRUE;

    return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
    UIUpdateToolBar();
    return FALSE;
}

LRESULT CMainFrame::OnCreate ( LPCREATESTRUCT lpcs )
{
    CreateSimpleToolBar();
    CreateSimpleStatusBar();

    m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

    UIAddToolBar(m_hWndToolBar);
    UISetCheck(ID_VIEW_TOOLBAR, 1);
    UISetCheck(ID_VIEW_STATUS_BAR, 1);

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    return 0;
}

void CMainFrame::OnFileExit ( UINT uCode, int nID, HWND hwndCtrl )
{
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnViewToolBar ( UINT uCode, int nID, HWND hwndCtrl )
{
    BOOL bVisible = !::IsWindowVisible(m_hWndToolBar);
    ::ShowWindow(m_hWndToolBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_TOOLBAR, bVisible);
    UpdateLayout();
}

void CMainFrame::OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl )
{
    BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
    ::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
    UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
    UpdateLayout();
}

void CMainFrame::OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl )
{
CPropertySheet sheet ( _T("About PSheets"), 1 );
CPropertyPage<IDD_ABOUTBOX> pgAbout;

    sheet.AddPage ( pgAbout );
    sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;

    sheet.DoModal();
}

void CMainFrame::OnOptions ( UINT uCode, int nID, HWND hwndCtrl )
{
CAppPropertySheet sheet ( _T("PSheets Options"), 0 );

    sheet.m_pgBackground.m_nColor = m_view.m_nColor;
    sheet.m_pgBackground.m_nPicture = m_view.m_nPicture;

    if ( IDOK == sheet.DoModal() )
        m_view.SetBackgroundOptions ( sheet.m_pgBackground.m_nColor,
                                      sheet.m_pgBackground.m_nPicture );
}

void CMainFrame::OnOptionsWizard ( UINT uCode, int nID, HWND hwndCtrl )
{
COptionsWizard wizard;

    wizard.m_pgBkcolor.m_nColor = m_view.m_nColor;
    wizard.m_pgBkpicture.m_nPicture = m_view.m_nPicture;

    if ( IDOK == wizard.DoModal() )
        m_view.SetBackgroundOptions ( wizard.m_pgBkcolor.m_nColor,
                                      wizard.m_pgBkpicture.m_nPicture );
}
