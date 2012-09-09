/********************************************************************
 *
 *	created:	2006/06/14
 *	created:	14:6:2006   23:28
 *	filename: 	f:\Test\ComFrame\ArtFrame\ArtFrame\MDIFrame.h
 *	file path:	f:\Test\ComFrame\ArtFrame\ArtFrame
 *	file base:	MDIFrame
 *	file ext:	h
 *	author:		许宗森
 *	Eamil:		ytf1978@163.com
 *	purpose:	COM MDI应用程序主窗口框架类，是所有MDI子窗口和停靠窗口的
 *				容器
 *				对象名: CMDIFrame 接口名: IMDIFrame.
 *
 ********************************************************************/




#pragma once
#include "resource.h"       // main symbols

#include "..\inc\ArtFrame.h"
#include "_IMDIFrameEvents_CP.h"
#include <atlcrack.h>
#include "baseObject.h"

// CMDIFrame
class CMDIChildFrame;
class CMDIFrame;
extern CMDIFrame *g_MDIFrame;


class ATL_NO_VTABLE CMDIFrame :
	public CMDIFrameWindowImpl<CMDIFrame>,
	public CUpdateUI<CMDIFrame>,
	public CMessageFilter,
	public CIdleHandler,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDIFrame, &CLSID_MDIFrame>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CMDIFrame>,
	public CProxy_IMDIFrameEvents<CMDIFrame>, 
	public IDispatchImpl<IMDIFrame, &IID_IMDIFrame, &LIBID_ArtFrameLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMDIChildFrame *m_pActiveChildFrame;
private:
	CComQIPtr<IMDIChildFrames, &IID_IMDIChildFrames> m_ChildFrames;


	//-------------------
	// 以下是COM代码.
	//-------------------
public:
	CMDIFrame();	
	
	DECLARE_REGISTRY_RESOURCEID(IDR_MDIFRAME)


	BEGIN_COM_MAP(CMDIFrame)
		COM_INTERFACE_ENTRY(IMDIFrame)
		COM_INTERFACE_ENTRY(IDispatch)//, _IMDIFrameEvents)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		//COM_INTERFACE_ENTRY(_IMDIFrameEvents)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CMDIFrame)
		CONNECTION_POINT_ENTRY(__uuidof(_IMDIFrameEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease() 
	{
	}

public:

	STDMETHOD(Run)(void);
	STDMETHOD(CreateWnd)(CreateMDIStruct* lpMDIStruct);
	STDMETHOD(Quit)(void);
	STDMETHOD(ShowMe)(VARIANT_BOOL vbShow);
	__COMMDIFrame;
	STDMETHOD(get_MDIChildFrames)(IDispatch** pVal);

	// _IMDIFrameEvents Methods
public:
	STDMETHOD(OnQuit)()
	{
		// Add your function implementation here.
		return E_NOTIMPL;
	}

	//--------------------------------------------
	// 以下是 MDIWindow 代码段
	//--------------------------------------------
public:
	
	DECLARE_FRAME_WND_CLASS(L"AiMDIFrame", IDR_MDIFRAME)

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	CMDICommandBarCtrl m_CmdBar;

	BEGIN_UPDATE_UI_MAP(CMDIFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMDIFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MSG_WM_CLOSE(OnClose)
		MESSAGE_HANDLER_EX(WM_MDISETMENU, OnMDISetMenu)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		CHAIN_MSG_MAP(CUpdateUI<CMDIFrame>)
		CHAIN_MSG_MAP(CMDIFrameWindowImpl<CMDIFrame>)
	END_MSG_MAP()

	LRESULT	OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(void);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	STDMETHOD(AboutCOMFrame)(void);
	LRESULT OnSysCommand(UINT cmd, CPoint Pt);
	STDMETHOD(MDIWindows)(void);
};

OBJECT_ENTRY_AUTO(__uuidof(MDIFrame), CMDIFrame)

