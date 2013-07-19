// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__8AD166C4_CF9A_4D34_B260_273B66681362__INCLUDED_)
#define AFX_MAINFRM_H__8AD166C4_CF9A_4D34_B260_273B66681362__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
        public CMessageFilter, public CIdleHandler
{
public:
    DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    CPSheetsView m_view;

    BOOL PreTranslateMessage(MSG* pMsg);
    BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(CMainFrame)
        UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(CMainFrame)
        MSG_WM_CREATE(OnCreate)
        COMMAND_ID_HANDLER_EX(ID_APP_EXIT, OnFileExit)
        COMMAND_ID_HANDLER_EX(ID_VIEW_TOOLBAR, OnViewToolBar)
        COMMAND_ID_HANDLER_EX(ID_VIEW_STATUS_BAR, OnViewStatusBar)
        COMMAND_ID_HANDLER_EX(ID_APP_ABOUT, OnAppAbout)
        COMMAND_ID_HANDLER_EX(IDC_OPTIONS, OnOptions)
        COMMAND_ID_HANDLER_EX(IDC_OPTIONS_WIZARD, OnOptionsWizard)
        CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
    END_MSG_MAP()

    LRESULT OnCreate ( LPCREATESTRUCT lpcs );
    void    OnFileExit ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnViewToolBar ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnViewStatusBar ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnAppAbout ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnOptions ( UINT uCode, int nID, HWND hwndCtrl );
    void    OnOptionsWizard ( UINT uCode, int nID, HWND hwndCtrl );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__8AD166C4_CF9A_4D34_B260_273B66681362__INCLUDED_)
