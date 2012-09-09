// BkgndCtxMenuExt.h : Declaration of the CBkgndCtxMenuExt

#ifndef __BKGNDCTXMENUEXT_H_
#define __BKGNDCTXMENUEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CBkgndCtxMenuExt

class ATL_NO_VTABLE CBkgndCtxMenuExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBkgndCtxMenuExt, &CLSID_BkgndCtxMenuExt>,
	public IDispatchImpl<IBkgndCtxMenuExt, &IID_IBkgndCtxMenuExt, &LIBID_DIRBKGNDEXTLib>,
    public IShellExtInit,
    public IContextMenu
{
public:
	CBkgndCtxMenuExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BKGNDCTXMENUEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBkgndCtxMenuExt)
	COM_INTERFACE_ENTRY(IBkgndCtxMenuExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);

protected:
    TCHAR m_szDirClickedIn [MAX_PATH];
};

#endif //__BKGNDCTXMENUEXT_H_
