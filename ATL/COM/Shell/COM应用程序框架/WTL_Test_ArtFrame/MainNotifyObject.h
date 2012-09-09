/********************************************************************
	created:	2006/06/16
	created:	16:6:2006   17:53
	filename: 	f:\Test\ComFrame\ArtFrame\Test_ArtFrame\MainNotifyObject.h
	file path:	f:\Test\ComFrame\ArtFrame\Test_ArtFrame
	file base:	MainNotifyObject
	file ext:	h
	author:		YTF-1978
	
	purpose:	定义MDI主窗口命令响应类，用来响应全局通一的命令处理。
	
	Class Name: CMainNotifyObject.

*********************************************************************/
#pragma once

#include "..\inc\ArtFrameImpl.h"
#include "resource.h"

#define EventType 1

namespace 
{
static const int	DISPID_SHOW = 1;
static const int	DISPID_SHOW2 = 2;

extern	_ATL_FUNC_INFO OnShowInfo0;
extern	_ATL_FUNC_INFO OnShowInfo1;
extern	_ATL_FUNC_INFO OnShowInfo3;
}


class CMainNotifyObject	: CComObjectRootEx<CComSingleThreadModel>
		,public IDispEventImpl<EventType, CMainNotifyObject, &DIID__IMDIFrameEvents, &LIBID_ArtFrameLib>
{
	//typedef IDispEventImpl<EventType, CMainNotifyObject, &DIID__IMDIFrameEvents, &LIBID_ArtFrameLib>  baseDispEvent;
public:
	CMainNotifyObject(void);
	~CMainNotifyObject(void);

	IMDIFrame *m_lpMdiFrame;

	void WinMain();

	BEGIN_SINK_MAP(CMainNotifyObject)

		//不使用 _ATL_FUNC_INFO结构 iFrmae 在调用事件的时候返回"没有注册的库"。这是为什么？
		//SINK_ENTRY_EX(EventType, DIID__IMDIFrameEvents, 0x1, OnQuit)
		//SINK_ENTRY_EX(EventType, DIID__IMDIFrameEvents, 0x2, NotifyCommand)

		SINK_ENTRY_INFO(EventType, DIID__IMDIFrameEvents, DISPID_SHOW, OnQuit, &OnShowInfo1)
		SINK_ENTRY_INFO(1, DIID__IMDIFrameEvents, DISPID_SHOW2, NotifyCommand, &OnShowInfo3)
	END_SINK_MAP()

	STDMETHODIMP OnQuit(VARIANT_BOOL *vbQuit)
	{
		this->DispEventUnadvise((IUnknown*)this);

		return S_OK;
	}

	//STDMETHODIMP NotifyCommand(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle)
	//{
	//	MessageBox(0, _T(""), _T(""), 0);
	//	//STDMETHODIMP (UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle) 
	//	return S_OK;
	//}

	NC_BEGIN_MAP()
		NC_COMMAND_ID_HANDLER(ID_FILE_NEW, FileNew)
		NC_COMMAND_ID_HANDLER(ID_FILE_EXIT, OnFileExit)
		NC_COMMAND_ID_HANDLER(ID_HELP_ABOUT, OnAbout)
		NC_COMMAND_ID_HANDLER(ID_HELP_WINDOWS, OnWindows)
	NC_END_MAP()

	void FileNew(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnFileExit(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnAbout(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	void OnWindows(UINT codeNotify, UINT cmdID, VARIANT_BOOL *bHandle);
	
	HRESULT CreateMain();
};
