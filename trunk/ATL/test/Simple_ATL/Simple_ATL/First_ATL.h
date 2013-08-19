// First_ATL.h : CFirst_ATL 的声明

#pragma once
#include "resource.h"       // 主符号

#include "Simple_ATL_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CFirst_ATL

class ATL_NO_VTABLE CFirst_ATL :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFirst_ATL, &CLSID_First_ATL>,
	public IDispatchImpl<IFirst_ATL, &IID_IFirst_ATL, &LIBID_Simple_ATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFirst_ATL()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIRST_ATL)


BEGIN_COM_MAP(CFirst_ATL)
	COM_INTERFACE_ENTRY(IFirst_ATL)
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

	STDMETHOD(AddNumbers)(LONG Num1, LONG Num2, LONG* ReturnVal);
};

OBJECT_ENTRY_AUTO(__uuidof(First_ATL), CFirst_ATL)
