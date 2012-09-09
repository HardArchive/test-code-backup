// MDIFrame.cpp : Implementation of CMDIFrame

#include "stdafx.h"
#include "MDIFrame.h"
#include "..\inc\ArtFrame.h"
#include ".\mdiframe.h"
#include "MDIChildFrame.h"
#include "AboutCOMFrame.h"
#include <ComDef.h>
#include "MDIChildWindows.h"


// CMDIFrame
CMDIFrame *g_MDIFrame = NULL;
IDispatch *g_retMDIFrame = NULL;

CMDIFrame::CMDIFrame():
	m_pActiveChildFrame(NULL)
{
	g_MDIFrame = this;
	g_retMDIFrame = this;
}


BOOL CMDIFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CMDIFrameWindowImpl<CMDIFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	HWND hWnd = MDIGetActive();
	if(hWnd != NULL)
		return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return FALSE;
}

BOOL CMDIFrame::OnIdle()
{
	UIUpdateToolBar();

	if (!FindWindowEx(m_hWndMDIClient, NULL, NULL, NULL))
	{
		m_pActiveChildFrame = NULL;
	}
	
	return FALSE;
}

LRESULT CMDIFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_hMenu = LoadMenu(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	ATLASSERT(m_hMenu);
	SetMenu(m_hMenu);

	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);


	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);	
	CreateSimpleStatusBar();
	CreateMDIClient();
	m_CmdBar.SetMDIClient(m_hWndMDIClient);


	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);


	CMenuHandle hMenuSys = GetSystemMenu(FALSE);
	ATLASSERT(NULL != hMenuSys.m_hMenu);
	hMenuSys.AppendMenu(MF_SEPARATOR);
	hMenuSys.AppendMenu(MF_STRING, ID_APP_ABOUT, L"&About COM 应用程序框架");


	// register object for message filtering and idle updates
	theLoop.AddMessageFilter(this);
	theLoop.AddIdleHandler(this);

	return 0;
}

LRESULT CMDIFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	theLoop.RemoveMessageFilter(this);
	theLoop.RemoveIdleHandler(this);


	// if UI is the last thread, no need to wait
	//if(_Module.GetLockCount() == 1)
	//{
	//_Module.m_dwTimeOut = 0L;
	//_Module.m_dwPause = 0L;
	//}
	//_Module.Unlock();
	return 0;
}



//////////////////////////////////////////////////////////////////////////

STDMETHODIMP CMDIFrame::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMDIFrame
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CMDIFrame::Run(void)
{
	theLoop.Run();

	return S_OK;
}


//建立主框架窗口

STDMETHODIMP CMDIFrame::CreateWnd(CreateMDIStruct* lpMDIStruct)
{
	ATLASSERT(NULL != lpMDIStruct);
	HRESULT hr = 0;

	if (NULL != lpMDIStruct)
	{
		if (NULL == m_hWnd)
		{
			//读入一些信息
			// ....

			CreateEx();
			ATLASSERT(NULL != m_hWnd);
			SetWindowText(lpMDIStruct->lpszWindowName);
			SetMenu(lpMDIStruct->hMenu);
			m_hMenu			= lpMDIStruct->hMenu;

			m_CmdBar.AttachMenu(GetMenu());
			SetMenu(NULL);

			hr = m_ChildFrames.CoCreateInstance(CLSID_MDIChildFrames);
			if (FAILED(hr))
			{
				//错误处理...;
				ATLASSERT(0);
				return S_FALSE;
			}
		}

		return S_OK;
	}

	return S_FALSE;
}


STDMETHODIMP CMDIFrame::Quit(void)
{
	ATLASSERT(NULL != m_hWnd);
	OnClose();
	return S_OK;
}


STDMETHODIMP CMDIFrame::ShowMe(VARIANT_BOOL vbShow)
{
	if (vbShow == VARIANT_TRUE)
	{
		ShowWindow(SW_SHOW);
	}else
	{
		ShowWindow(SW_HIDE);
	}

	return S_OK;
}



LRESULT CMDIFrame::OnClose(void)
{
	VARIANT_BOOL vbQuit = VARIANT_TRUE;

	if (NULL != m_hWnd)
	{
		Fire_OnQuit(&vbQuit);
		if (vbQuit == VARIANT_TRUE)
		{
			//保存一些信息。
			//...
			DestroyWindow();
			PostQuitMessage(0);
			return 0;
		}
	}

	return -1;
}


STDMETHODIMP CMDIFrame::get_MDIChildFrames(IDispatch** pVal)
{
	*pVal = m_ChildFrames;

	return S_OK;
}


LRESULT CMDIFrame::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT codeNotify		= (UINT)HIWORD(wParam);
	int id				= (int)LOWORD(wParam);
	HWND hwndCtl		= (HWND)lParam;
	VARIANT_BOOL _bHandle = VARIANT_FALSE;
	HRESULT hr = S_FALSE;

	if (m_vec.GetSize()>0)
	{
		//把命令送给事件
		hr = this->Fire_NotifyCommand(codeNotify, id, &_bHandle);
		if (FAILED(hr))
		{
			_com_error error(hr);
			MessageBox(error.ErrorMessage(), _T("ArtFrame"), MB_OK);
		}
		if (_bHandle == VARIANT_FALSE)
		{
			return 0;
		}
	}


	if (NULL != m_pActiveChildFrame)
	{
		//把命令给当前活动的MDI子窗口。
		::SendMessage(m_pActiveChildFrame->m_hWnd, WM_COMMAND, wParam, lParam);
	}

	return 0;
}


LRESULT CMDIFrame::OnMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HMENU hFrameMenu = (HMENU)wParam;
	HMENU hWndMenu = (HMENU)lParam;
	
	SetMsgHandled(TRUE);

	return 0;
}


STDMETHODIMP CMDIFrame::AboutCOMFrame(void)
{
	CAboutCOMFrame af;

	af.DoModal(m_hWnd);

	return S_OK;
}



LRESULT CMDIFrame::OnSysCommand(UINT cmd, CPoint Pt)
{
	if (ID_APP_ABOUT == cmd)
	{
		AboutCOMFrame();
		return 0;
	}

	SetMsgHandled(FALSE);

	return 0;
}



STDMETHODIMP CMDIFrame::MDIWindows(void)
{
	CMDIChildWindows ws;

	ws.DoModal(m_hWnd);

	return S_OK;
}
