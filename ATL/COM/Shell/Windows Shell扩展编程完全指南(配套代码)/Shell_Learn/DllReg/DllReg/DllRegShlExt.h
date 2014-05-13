// DllRegShlExt.h : CDllRegShlExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "DllReg_i.h"

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>
#include <shlguid.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CDllRegShlExt

class ATL_NO_VTABLE CDllRegShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDllRegShlExt, &CLSID_DllRegShlExt>,
	public IDispatchImpl<IDllRegShlExt, &IID_IDllRegShlExt, &LIBID_DllRegLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CDllRegShlExt();

DECLARE_REGISTRY_RESOURCEID(IDR_DLLREGSHLEXT)


BEGIN_COM_MAP(CDllRegShlExt)
	COM_INTERFACE_ENTRY(IDllRegShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
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
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO); 
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT); 


protected:
	//add by ray 2014-05-13 11:22 CDllRegShlExt 类需要一些成员变量:
	HBITMAP m_hRegBmp;
	HBITMAP m_hUnregBmp;
	string_list m_lsFiles;
	TCHAR m_szDir [MAX_PATH];
};

OBJECT_ENTRY_AUTO(__uuidof(DllRegShlExt), CDllRegShlExt)
