// HelloSimpleObject.h : CHelloSimpleObject 的声明

#pragma once
#include "resource.h"       // 主符号

#include "com_lean4_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CHelloSimpleObject

class ATL_NO_VTABLE CHelloSimpleObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloSimpleObject, &CLSID_HelloSimpleObject>,
	public IDispatchImpl<IHelloSimpleObject, &IID_IHelloSimpleObject, &LIBID_com_lean4Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHelloSimpleObject()
	{
		m_bstrDescription = SysAllocString(L"我是HelloSimpleObject组件，我是简单对象！");
	}

	~CHelloSimpleObject(){}

DECLARE_REGISTRY_RESOURCEID(IDR_HELLOSIMPLEOBJECT)


BEGIN_COM_MAP(CHelloSimpleObject)
	COM_INTERFACE_ENTRY(IHelloSimpleObject)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(Sumlong)(LONG lA, LONG lB, LONG* plSum);
	STDMETHOD(get_Description)(BSTR* pVal);
	STDMETHOD(put_Description)(BSTR newVal);

public:
	BSTR m_bstrDescription;
};

OBJECT_ENTRY_AUTO(__uuidof(HelloSimpleObject), CHelloSimpleObject)
