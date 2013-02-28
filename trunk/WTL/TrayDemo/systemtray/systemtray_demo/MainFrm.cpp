// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SystemTrayDemo.h"

#include "MainFrm.h"

#ifndef _WIN32_WCE
#include "BalloonDlg.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_ICON_NOTIFY			WM_APP+10

#ifndef SYSTEMTRAY_USEW2K
#define NIIF_WARNING 0
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
    ON_COMMAND(ID_POPUP_ANIMATE, OnAnimate)
	ON_COMMAND(IDC_SHOW_ICON, OnShowIcon)
	ON_COMMAND(IDC_HIDE_ICON, OnHideIcon)
	ON_COMMAND(IDC_MOVE_TO_RIGHT, OnMoveToRight)
	ON_COMMAND(IDC_SHOW_BALLOON, OnShowBalloon)
	ON_UPDATE_COMMAND_UI(IDC_HIDE_ICON, OnUpdateHideIcon)
	ON_UPDATE_COMMAND_UI(IDC_SHOW_BALLOON, OnUpdateShowBalloon)
	ON_UPDATE_COMMAND_UI(IDC_MOVE_TO_RIGHT, OnUpdateMoveToRight)
	ON_UPDATE_COMMAND_UI(IDC_SHOW_ICON, OnUpdateShowIcon)
	ON_UPDATE_COMMAND_UI(IDC_SET_FOCUS, OnUpdateSetFocus)
	ON_COMMAND(IDC_SET_FOCUS, OnSetFocusToIcon)
	ON_WM_CLOSE()
	ON_COMMAND(ID_DEMO_MAXIMISE, OnDemoMaximise)
	ON_UPDATE_COMMAND_UI(ID_DEMO_MAXIMISE, OnUpdateDemoMaximise)
	ON_COMMAND(ID_DEMO_MINIMISE, OnDemoMinimise)
	ON_UPDATE_COMMAND_UI(ID_DEMO_MINIMISE, OnUpdateDemoMinimise)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef _WIN32_WCE
    HICON hIcon = ::LoadIcon(NULL, IDI_ASTERISK);  // Icon to use
#else
    HICON hIcon = ::LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));  // Icon to use
#endif

	// Create the tray icon
#ifdef USING_NONMFC_TRAY  // This is set in project settings
	if (!m_TrayIcon.Create(AfxGetInstanceHandle(),
#else
	if (!m_TrayIcon.Create(
#endif
                        NULL,                            // Let icon deal with its own messages
                        WM_ICON_NOTIFY,                  // Icon notify message to use
                        _T("This is a Tray Icon - Right click on me!"),  // tooltip
                        hIcon,
                        IDR_POPUP_MENU,                  // ID of tray icon
                        FALSE,
                        _T("Here's a cool new Win2K balloon!"), // balloon tip
                        _T("Look at me!"),               // balloon title
                        NIIF_WARNING,                    // balloon icon
                        20 ))                            // balloon timeout
    {
		return -1;
    }

	// MFC version of CSystemTray automatically sends messages to the main app window
#ifdef USING_NONMFC_TRAY  // This is set in project settings
	m_TrayIcon.SetTargetWnd(GetSafeHwnd());		// Send all menu messages here.
#endif

    m_TrayIcon.SetMenuDefaultItem(3, TRUE);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame/CSystemTray menu message handlers

void CMainFrame::OnAnimate()
{
    m_TrayIcon.SetIconList(IDI_ICON1, IDI_ICON4);
    m_TrayIcon.Animate(50,2);
}

void CMainFrame::OnShowIcon() 
{
    m_TrayIcon.ShowIcon();
}

void CMainFrame::OnHideIcon() 
{
    m_TrayIcon.HideIcon();
}

void CMainFrame::OnMoveToRight() 
{
    m_TrayIcon.MoveToRight();
}

void CMainFrame::OnShowBalloon() 
{
#ifdef SYSTEMTRAY_USEW2K
    CBalloonDlg dlg;

    if ( IDOK == dlg.DoModal() )
    {
        m_TrayIcon.ShowBalloon ( dlg.m_sBalloonText, dlg.m_sBalloonTitle,
                                 dlg.m_dwIcon, dlg.m_uTimeout );
    }
#else
    MessageBox(_T("Balloon tips only supported in Windows 2000"), _T("Sorry..."),
               MB_ICONEXCLAMATION);
#endif
}

void CMainFrame::OnSetFocusToIcon() 
{
#ifdef SYSTEMTRAY_USEW2K
    m_TrayIcon.SetFocus();
#else
    MessageBox(_T("\"SetFocus\" only supported in Windows 2000"), _T("Sorry..."),
               MB_ICONEXCLAMATION);
#endif
}

void CMainFrame::OnUpdateShowIcon(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( !m_TrayIcon.Visible() );
}

void CMainFrame::OnUpdateHideIcon(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( m_TrayIcon.Visible() );
}

void CMainFrame::OnUpdateMoveToRight(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable ( m_TrayIcon.Visible() );
}

void CMainFrame::OnUpdateShowBalloon(CCmdUI* pCmdUI) 
{
#ifdef SYSTEMTRAY_USEW2K
    pCmdUI->Enable ( m_TrayIcon.Visible() );
#else
    pCmdUI->Enable(FALSE);
#endif
}

void CMainFrame::OnUpdateSetFocus(CCmdUI* pCmdUI) 
{
#ifdef SYSTEMTRAY_USEW2K
    pCmdUI->Enable ( m_TrayIcon.Visible() );
#else
    pCmdUI->Enable(FALSE);
#endif
}

void CMainFrame::OnClose() 
{
    m_TrayIcon.RemoveIcon();	
	CFrameWnd::OnClose();
}

void CMainFrame::OnDemoMaximise() 
{
#ifndef _WIN32_WCE
#ifdef USING_NONMFC_TRAY  // This is set in project settings
    CSystemTray::MaximiseFromTray(GetSafeHwnd());
#else
    CSystemTray::MaximiseFromTray(this);
#endif
    m_TrayIcon.SetMenuDefaultItem(ID_DEMO_MINIMISE, FALSE);
#endif
}

void CMainFrame::OnDemoMinimise() 
{
#ifndef _WIN32_WCE
#ifdef USING_NONMFC_TRAY  // This is set in project settings
    CSystemTray::MinimiseToTray(GetSafeHwnd());
#else
    CSystemTray::MinimiseToTray(this);
#endif
    m_TrayIcon.SetMenuDefaultItem(ID_DEMO_MAXIMISE, FALSE);
#endif
}

void CMainFrame::OnUpdateDemoMaximise(CCmdUI* pCmdUI) 
{
#ifdef _WIN32_WCE
    pCmdUI->Enable(FALSE);
#else
    pCmdUI->Enable(!IsIconic() && !IsWindowVisible());
#endif
}

void CMainFrame::OnUpdateDemoMinimise(CCmdUI* pCmdUI) 
{
#ifdef _WIN32_WCE
    pCmdUI->Enable(FALSE);
#else
    pCmdUI->Enable(!IsIconic() && IsWindowVisible());
#endif
}