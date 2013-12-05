// SimpleShlExt.h : CSimpleShlExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "SimpleExt_i.h"

//add by rg & 2013-11-29 16:42:10 
#include "shlobj.h"
#include "comdef.h" 


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CSimpleShlExt

class ATL_NO_VTABLE CSimpleShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSimpleShlExt, &CLSID_SimpleShlExt>,
	public IDispatchImpl<ISimpleShlExt, &IID_ISimpleShlExt, &LIBID_SimpleExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	//add by rg & 2013-11-29 16:52:49
	public IShellExtInit
{
public:
	CSimpleShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLESHLEXT)


BEGIN_COM_MAP(CSimpleShlExt)
	COM_INTERFACE_ENTRY(ISimpleShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	//add by rg & 2013-11-29 16:52:59
	COM_INTERFACE_ENTRY(IShellExtInit) 
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
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	
protected: 
	//add by rg & 2013-11-29 17:47:25
	TCHAR m_szFile [MAX_PATH];    //保存文件名


};

OBJECT_ENTRY_AUTO(__uuidof(SimpleShlExt), CSimpleShlExt)
