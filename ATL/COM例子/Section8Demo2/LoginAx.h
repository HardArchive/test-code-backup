// LoginAx.h : CLoginAx 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>
#include "Section8Demo2.h"
#include "_ILoginAxEvents_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif


// CLoginAx
class ATL_NO_VTABLE CLoginAx :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ILoginAx, &IID_ILoginAx, &LIBID_Section8Demo2Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CLoginAx>,
	public IOleControlImpl<CLoginAx>,
	public IOleObjectImpl<CLoginAx>,
	public IOleInPlaceActiveObjectImpl<CLoginAx>,
	public IViewObjectExImpl<CLoginAx>,
	public IOleInPlaceObjectWindowlessImpl<CLoginAx>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CLoginAx>,
	public CProxy_ILoginAxEvents<CLoginAx>,
	public IPersistStorageImpl<CLoginAx>,
	public ISpecifyPropertyPagesImpl<CLoginAx>,
	public IQuickActivateImpl<CLoginAx>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CLoginAx>,
#endif
	public IProvideClassInfo2Impl<&CLSID_LoginAx, &__uuidof(_ILoginAxEvents), &LIBID_Section8Demo2Lib>,
#ifdef _WIN32_WCE // 要在 Windows CE 上正确加载该控件，要求 IObjectSafety
	public IObjectSafetyImpl<CLoginAx, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CLoginAx, &CLSID_LoginAx>,
	public CComCompositeControl<CLoginAx>
{
public:


	CLoginAx()
	{
		m_varbJudgeNull = VARIANT_FALSE;
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_LOGINAX)


BEGIN_COM_MAP(CLoginAx)
	COM_INTERFACE_ENTRY(ILoginAx)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // 要在 Windows CE 上正确加载该控件，要求 IObjectSafety
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
END_COM_MAP()

BEGIN_PROP_MAP(CLoginAx)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CLoginAx)
	CONNECTION_POINT_ENTRY(__uuidof(_ILoginAxEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CLoginAx)
	COMMAND_HANDLER(IDC_BTN_LOGIN, BN_CLICKED, OnBnClickedBtnLogin)
	CHAIN_MSG_MAP(CComCompositeControl<CLoginAx>)
END_MSG_MAP()
// 处理程序原型:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CLoginAx)
	//请确保事件处理程序含有 __stdcall 调用约定
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CLoginAx>::OnAmbientPropertyChange(dispid);
	}
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_ILoginAx,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ILoginAx

	enum { IDD = IDD_LOGINAX };

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}


private:
	//是否判断输入的内容为空
	VARIANT_BOOL m_varbJudgeNull;
	STDMETHOD(get_JudgeNull)(VARIANT_BOOL* pVal);
	STDMETHOD(put_JudgeNull)(VARIANT_BOOL newVal);
	LRESULT OnBnClickedBtnLogin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	STDMETHOD(SetText)(BSTR bstrUserName, BSTR bstrPassword, BSTR bstrLogin);
};

OBJECT_ENTRY_AUTO(__uuidof(LoginAx), CLoginAx)
