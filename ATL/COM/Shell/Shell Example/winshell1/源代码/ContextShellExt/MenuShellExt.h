// MenuShellExt.h : Declaration of the CMenuShellExt

#ifndef __MENUSHELLEXT_H_
#define __MENUSHELLEXT_H_

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>
#include <vector>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// CMenuShellExt

class ATL_NO_VTABLE CMenuShellExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMenuShellExt, &CLSID_MenuShellExt>,
	public IDispatchImpl<IMenuShellExt, &IID_IMenuShellExt, &LIBID_CONTEXTSHELLEXTLib>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CMenuShellExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MENUSHELLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMenuShellExt)
	COM_INTERFACE_ENTRY(IMenuShellExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

// IMenuShellExt
public:
	// IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	
    // IContextMenu
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
private:
	//保存文件名
	std::vector<std::string> m_FileName;
};

#endif //__MENUSHELLEXT_H_
