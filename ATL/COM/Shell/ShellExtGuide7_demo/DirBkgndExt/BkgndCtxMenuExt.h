// BkgndCtxMenuExt.h : Declaration of the CBkgndCtxMenuExt

#ifndef __BKGNDCTXMENUEXT_H_
#define __BKGNDCTXMENUEXT_H_

/////////////////////////////////////////////////////////////////////////////
// CBkgndCtxMenuExt

class ATL_NO_VTABLE CBkgndCtxMenuExt : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CBkgndCtxMenuExt, &CLSID_BkgndCtxMenuExt>,
    public IShellExtInit,
    public IContextMenu
{
public:
    CBkgndCtxMenuExt() { }

    BEGIN_COM_MAP(CBkgndCtxMenuExt)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
    END_COM_MAP()

    DECLARE_REGISTRY_RESOURCEID(IDR_BKGNDCTXMENUEXT)

public:
    // IShellExtInit
    STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
    STDMETHODIMP GetCommandString(UINT, UINT, UINT*, LPSTR, UINT);

protected:
    TCHAR m_szDirClickedIn[MAX_PATH];
};

#endif //__BKGNDCTXMENUEXT_H_
