#include "stdafx.h"
#include "MainFrm.h"
#include "AboutDlg.h"

#define SIZE_TABCTRLLEFT	6	// TabCtrl 左侧
#define SIZE_TABCTRLTOP		24	// TabCtrl 顶端
#define SIZE_TABCTRLHEIGHT	59	// TabCtrl 控件的高度

CMainFrame::CMainFrame(void)
{
	TCHAR chFileName[MAX_PATH];
	GetModuleFileName(NULL, chFileName, MAX_PATH);

	if (0 == ExtractIconEx(chFileName, 0, &m_hIcon, &m_hIconSmall, 1))
	{
		m_hIcon = LoadIcon(theApp.m_hInst, MAKEINTRESOURCE(IDR_MAINFRAME));
		PrivateExtractIcons(chFileName, 0, 16, 16, &m_hIconSmall, NULL, 1, 0);
	}
}

/*virtual*/ BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

/*virtual*/ BOOL CMainFrame::OnIdle()
{
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = theApp.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIconSmall, FALSE);
	m_SkinFrame.Attach(m_hWnd);
	m_SkinFrame.SetMinSize(780, 540);

	RECT rcClient;
	GetClientRect(&rcClient);
	int ccx = rcClient.right;
	int ccy = rcClient.bottom;

	if ( !m_Static.Create(m_hWnd, NULL, _T("asasdas"), WS_CHILD | WS_VISIBLE) )
	{
		ATLTRACE(_T("未能创建提示静态文本框!\n"));
		return -1;
	}
	m_Static.SetFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	m_Static.SetWindowPos(NULL, 6, 100, 200, 25, SWP_NOZORDER | SWP_NOACTIVATE);

	if ( !m_SkinTabCtrl.Create(_T("ow_TabCtrl"), NULL, WS_CHILD | WS_VISIBLE, SIZE_TABCTRLLEFT, SIZE_TABCTRLTOP, 
			ccx - SIZE_TABCTRLLEFT * 2, SIZE_TABCTRLHEIGHT, m_hWnd) )
	{
		ATLTRACE(_T("未能创建标签栏窗口!\n"));
		return -1;
	}
	m_SkinTabCtrl.AddItem( _T("首页"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON1)) );
	m_SkinTabCtrl.AddItem( _T("敏感信息"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON2)) );
	m_SkinTabCtrl.AddItem( _T("网络安全"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON3)) );
	m_SkinTabCtrl.AddItem( _T("违规记录"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON4)) );
	m_SkinTabCtrl.AddItem( _T("软件检查"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON5)) );
	m_SkinTabCtrl.AddItem( _T("系统补丁"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON6)) );
	m_SkinTabCtrl.AddItem( _T("主机安全"), LoadIcon(theApp.m_hInstResource, MAKEINTRESOURCE(IDI_TABICON7)) );

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = theApp.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}
LRESULT CMainFrame::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rcClient;
	GetClientRect(&rcClient);
	int ccx = rcClient.right;
	int ccy = rcClient.bottom;

	if ( m_SkinTabCtrl.GetSafeHwnd() )
		::SetWindowPos(m_SkinTabCtrl.GetSafeHwnd(), NULL, SIZE_TABCTRLLEFT, SIZE_TABCTRLTOP, 
				ccx - SIZE_TABCTRLLEFT * 2, SIZE_TABCTRLHEIGHT, SWP_NOZORDER | SWP_NOACTIVATE);

	return 0;
}

LRESULT CMainFrame::OnNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPNMHDR lpNMHDR = reinterpret_cast<LPNMHDR>(lParam);
	if ( !lpNMHDR ) return 0;
	
	if ( m_SkinTabCtrl.GetSafeHwnd() == lpNMHDR->hwndFrom )
	{
		LPSKINTABNOTIFY lpstNotify = reinterpret_cast<LPSKINTABNOTIFY>(lParam);
		switch (lpstNotify->hdr.code)
		{
		case SKINMGR_TCN_SELCHANGE:
			// 标签栏选择改变在这里编写响应代码
			{
				static TCHAR* ppStr[] = {
					_T("首页"), 
					_T("敏感信息"), 
					_T("网络安全"), 
					_T("违规记录"), 
					_T("软件检查"), 
					_T("系统补丁"), 
					_T("主机安全")
				};

				CString strInfo;
				strInfo.Format(_T("当前选中的是： %s"), ppStr[lpstNotify->Index]);
				m_Static.SetWindowText(strInfo);
			}
			break;
		case SKINMGR_TCN_LBUTTONDOWN:
			if ( -1 == lpstNotify->Index)
				SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL);
			break;
		case SKINMGR_TCN_LBUTTONDBLCLK:
			if ( -1 == lpstNotify->Index)
				SendMessage(WM_SYSCOMMAND, (IsZoomed() ? SC_RESTORE : SC_MAXIMIZE), NULL);
			break;
		}
	}
	return 0;
}