// MFC_Test_ArtFrame.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号

#define EventType 1
namespace 
{
static const int	DISPID_SHOW = 1;
static const int	DISPID_SHOW2 = 2;

extern	_ATL_FUNC_INFO OnShowInfo0;
extern	_ATL_FUNC_INFO OnShowInfo1;
extern	_ATL_FUNC_INFO OnShowInfo3;
}

// CFrameApp:
// 有关此类的实现，请参阅 MFC_Test_ArtFrame.cpp
//

class CFrameApp : public CWinApp
				,public CComObjectRootEx<CComSingleThreadModel>
				,public IDispEventImpl<EventType, CFrameApp, &DIID__IMDIFrameEvents, &LIBID_ArtFrameLib>
{
public:
	CFrameApp();
		IMDIFrame *m_lpMdiFrame;

		void WinMain();

	BEGIN_SINK_MAP(CFrameApp)
		//不使用 _ATL_FUNC_INFO结构 iFrmae 在调用事件的时候返回"没有注册的库"。这是为什么？
		//SINK_ENTRY_EX(EventType, DIID__IMDIFrameEvents, 0x1, OnQuit)
		//SINK_ENTRY_EX(EventType, DIID__IMDIFrameEvents, 0x2, NotifyCommand)

		SINK_ENTRY_INFO(EventType, DIID__IMDIFrameEvents, DISPID_SHOW, OnQuit, &OnShowInfo1)
		SINK_ENTRY_INFO(EventType, DIID__IMDIFrameEvents, DISPID_SHOW2, NotifyCommand, &OnShowInfo3)
	END_SINK_MAP()

		NC_BEGIN_MAP()
			NC_COMMAND_ID_HANDLER(ID_FILE_NEW,FileNew)
			NC_COMMAND_ID_HANDLER(ID_FILE_EXIT,OnFileExit)
			NC_COMMAND_ID_HANDLER(ID_HELP_ABOUT,OnAbout)
			NC_COMMAND_ID_HANDLER(ID_HELP_WINDOWS, OnWindowWindows)
		NC_END_MAP()

		HRESULT CreateMain();
		void FileNew(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
		void OnFileExit(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
		void OnAbout(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
		void OnWindowWindows(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
		
	STDMETHODIMP OnQuit(VARIANT_BOOL *vbQuit)
	{
		//MessageBox(0, _T("Events. Quit"), _T(""), 0);
		return S_OK;
	}
// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CFrameApp theApp;

