/********************************************************************
 *	created:	2006/06/18
 *	created:	18:6:2006   12:23
 *	filename: 	f:\Test\ComFrame\ArtFrame\ArtFrame\MDIChildFrame.h
 *	file path:	f:\Test\ComFrame\ArtFrame\ArtFrame
 *	file base:	MDIChildFrame
 *	file ext:	h
 *	author:		许宗森
 *	
 *	purpose:	管理单个MDI子窗口框架,子窗口可以拥有多个子视图,
 *				子视图接口名称是 IDataView.
 *
 *
*********************************************************************/
#pragma once
#include "resource.h"       // main symbols
#include <List>

#include "..\inc\ArtFrame.h"
#include "_IMDIChildFrameEvents_CP.h"
#include "MDIFrame.h"
#include <atlcrack.h>

// CMDIChildFrame

class ATL_NO_VTABLE CMDIChildFrame : 
	public CMDIChildWindowImpl<CMDIChildFrame>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDIChildFrame, &CLSID_MDIChildFrame>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CMDIChildFrame>,
	public CProxy_IMDIChildFrameEvents<CMDIChildFrame>, 
	public IDispatchImpl<IMDIChildFrame, &IID_IMDIChildFrame, &LIBID_ArtFrameLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	IDataView *m_pActiveDataView;

public:
	CMDIChildFrame() :
		m_pActiveDataView(NULL)
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MDICHILDFRAME)


BEGIN_COM_MAP(CMDIChildFrame)
	COM_INTERFACE_ENTRY(IMDIChildFrame)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CMDIChildFrame)
	CONNECTION_POINT_ENTRY(__uuidof(_IMDIChildFrameEvents))
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
	__COMMDIFrame;
	STDMETHOD(PushDataView)(IDataView* lpDataView);
	STDMETHOD(PopDataView)(IDataView *lpdv);
	STDMETHOD(get_ActiveChildWindow)(IDataView** pVal);
	STDMETHOD(put_ActiveChildWindow)(IDataView* newVal);


//以下是窗口消息处理。

	DECLARE_FRAME_WND_CLASS(L"DockingView", IDR_MDICHILD)

	BEGIN_MSG_MAP(CMDIChildFrame)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER_EX(WM_MDISETMENU, OnMDISetMenu)
		MSG_WM_MDIACTIVATE(MDIActivate)
		MSG_WM_ACTIVATE(OnActivate)
		MSG_WM_COMMAND(OnCommand)
		CHAIN_MSG_MAP(CMDIChildWindowImpl<CMDIChildFrame>)
	END_MSG_MAP()

	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MDIActivate(HWND hwndDeactivate, HWND hwndActivate);
	LRESULT OnActivate(UINT state, BOOL fMinimized, HWND hwndActDeact);
	STDMETHOD(get_Title)(BSTR* pVal);
	STDMETHOD(put_Title)(BSTR newVal);
	LRESULT OnCommand(UINT codeNotify, int id, HWND hwndCtl);
	STDMETHOD(get_CommandBars)(IDispatch** pVal);
};


OBJECT_ENTRY_AUTO(__uuidof(MDIChildFrame), CMDIChildFrame)
