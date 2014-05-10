// PasswordSpObj.h : CPasswordSpObj 的声明

#pragma once
#include "resource.h"       // 主符号

#include "Section8Demo1.h"
#include "_IPasswordSpObjEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CPasswordSpObj

class ATL_NO_VTABLE CPasswordSpObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPasswordSpObj, &CLSID_PasswordSpObj>,
	public IConnectionPointContainerImpl<CPasswordSpObj>,
	public CProxy_IPasswordSpObjEvents<CPasswordSpObj>,
	public IDispatchImpl<IPasswordSpObj, &IID_IPasswordSpObj, &LIBID_Section8Demo1Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CPasswordSpObj()
	{
		m_lErrorCountAllowed = 3;
		m_lErrorCount = 0;
		m_bstrUser = L"admin";
		m_bstrPwd = L"123456";
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PASSWORDSPOBJ)


BEGIN_COM_MAP(CPasswordSpObj)
	COM_INTERFACE_ENTRY(IPasswordSpObj)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CPasswordSpObj)
	CONNECTION_POINT_ENTRY(__uuidof(_IPasswordSpObjEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

private:
	//允许错误的次数
	LONG m_lErrorCountAllowed;

	//错误次数
	LONG m_lErrorCount;

	//用户名
	CComBSTR m_bstrUser;

	//密码
	CComBSTR m_bstrPwd;


	STDMETHOD(get_ErrorCountAllowed)(LONG* pVal);
	STDMETHOD(put_ErrorCountAllowed)(LONG newVal);
	STDMETHOD(CheckPassword)(BSTR bstrUser, BSTR bstrPwd, VARIANT_BOOL* pvarbResult);
};

OBJECT_ENTRY_AUTO(__uuidof(PasswordSpObj), CPasswordSpObj)
