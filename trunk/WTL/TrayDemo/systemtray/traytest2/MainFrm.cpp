/////////////////////////////////////////////////////////////////////////////
// Project：TRAYTEST
// Author：Paul Dilascia
// Date：星期四, 十一月 07, 2002
// Description：一个简单的系统托盘程序例子，将API函数封装在一个C++类中，实现托盘程序的扩展。
//
/////////////////////////////////////////////////////////////////////////////
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TrayTest.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
// Message ID used for tray notifications
#define WM_MY_TRAY_NOTIFICATION WM_USER+0

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION, OnTrayNotification)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CLEAR,				OnViewClear)
	ON_COMMAND(ID_TOGGLE_ICON,				OnToggleIcon)
	ON_COMMAND(ID_VIEW_NOTIFICATIONS,	OnViewNotifications)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLEAR, OnUpdateViewClear)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NOTIFICATIONS, OnUpdateViewNotifications)
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_OPEN, OnAppOpen)
	ON_COMMAND(ID_APP_SUSPEND, OnAppSuspend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_trayIcon(IDR_TRAYICON)
{
	// TODO: add member initialization code here
	m_bShowTrayNotifications = TRUE;//zxn
	m_bShutdown = FALSE;//zxn
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// Create child edit control for displaying messages
	CRect rc;
	if (!m_wndEdit.Create(
		WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_MULTILINE|ES_READONLY, rc, this, 
		AFX_IDW_PANE_FIRST))

		return -1;

	// Set up tray icon
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	m_iWhichIcon = 1;
	m_trayIcon.SetIcon(IDI_MYICON);

	return 0;
}

//////////////////
// Close window. Unless we are shutting down, just hide it.
void CMainFrame::OnClose() 
{
	if (m_bShutdown)
		CFrameWnd::OnClose();
	else
		ShowWindow(SW_HIDE);
}
//////////////////
// Handle notification from tray icon: display a message.
//
LRESULT CMainFrame::OnTrayNotification(WPARAM uID, LPARAM lEvent)
{
	if (m_bShowTrayNotifications) {
		static LPCSTR MouseMessages[] = { "WM_MOUSEMOVE",
			"WM_LBUTTONDOWN", "WM_LBUTTONUP", "WM_LBUTTONDBLCLK",
			"WM_RBUTTONDOWN", "WM_RBUTTONUP", "WM_RBUTTONDBLCLK",
			"WM_MBUTTONDOWN", "WM_MBUTTONUP", "WM_MBUTTONDBLCLK" };

		CString s;
		s.Format("托盘通知消息: ID=%d, lEvent=0x%04x %s\r\n", 
			uID, lEvent, WM_MOUSEFIRST<=lEvent && lEvent<=WM_MOUSELAST ? 
			MouseMessages[lEvent-WM_MOUSEFIRST] : "(未知消息)");

		m_wndEdit.SetSel(-1, -1);		// end of edit text
		m_wndEdit.ReplaceSel(s);		// append string..
		m_wndEdit.SendMessage(EM_SCROLLCARET); // ..and make visible
	}

	// let tray icon do default stuff
//	return m_trayIcon.OnTrayNotification(uID, lEvent);
	return 0;
}

////////////////////////////////////////////////////////////////
// Command handlers below.
//
void CMainFrame::OnViewClear() 
{
	m_wndEdit.SetWindowText("");	
}

void CMainFrame::OnUpdateViewClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndEdit.GetLineCount() > 1 || m_wndEdit.LineLength() > 0);
}

void CMainFrame::OnToggleIcon() 
{
	m_iWhichIcon=!m_iWhichIcon;
	m_trayIcon.SetIcon(m_iWhichIcon ? IDI_MYICON : IDI_MYICON2);
}

void CMainFrame::OnViewNotifications() 
{
	m_bShowTrayNotifications = !m_bShowTrayNotifications;
}

void CMainFrame::OnUpdateViewNotifications(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bShowTrayNotifications);
}

void CMainFrame::OnAppOpen() 
{
	ShowWindow(SW_NORMAL);	
	SetForegroundWindow();
}

void CMainFrame::OnAppSuspend() 
{
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);	
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
///////////////////////////////////////////////////////////////////////
