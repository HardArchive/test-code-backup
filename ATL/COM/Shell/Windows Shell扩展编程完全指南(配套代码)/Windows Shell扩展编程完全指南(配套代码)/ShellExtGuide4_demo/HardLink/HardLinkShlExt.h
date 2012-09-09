// HardLinkShlExt.h : Declaration of the CHardLinkShlExt

#ifndef __HARDLINKSHLEXT_H_
#define __HARDLINKSHLEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CHardLinkShlExt

class ATL_NO_VTABLE CHardLinkShlExt : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CHardLinkShlExt, &CLSID_HardLinkShlExt>,
    public IDispatchImpl<IHardLinkShlExt, &IID_IHardLinkShlExt, &LIBID_HARDLINKLib>,
    public IShellExtInit,
    public IContextMenu
{
public:
    CHardLinkShlExt()
        {
        }

DECLARE_REGISTRY_RESOURCEID(IDR_HARDLINKSHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHardLinkShlExt)
    COM_INTERFACE_ENTRY(IHardLinkShlExt)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IShellExtInit)
    COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

protected:
    // IHardLinkShlExt
    CBitmap     m_bitmap;
    TCHAR       m_szFolderDroppedIn [MAX_PATH];
    CStringList m_lsDroppedFiles;

public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT) { return E_NOTIMPL; }
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
};

#endif //__HARDLINKSHLEXT_H_
