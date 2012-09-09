// BmpCtxMenuExt.h : Declaration of the CBmpCtxMenuExt

#ifndef __BMPCTXMENUEXT_H_
#define __BMPCTXMENUEXT_H_

/////////////////////////////////////////////////////////////////////////////
// CBmpCtxMenuExt

class ATL_NO_VTABLE CBmpCtxMenuExt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CBmpCtxMenuExt, &CLSID_BmpCtxMenuExt>,
    public IShellExtInit,
    public IContextMenu3
{
public:
    CBmpCtxMenuExt() { }

    BEGIN_COM_MAP(CBmpCtxMenuExt)
        COM_INTERFACE_ENTRY(IShellExtInit)
        COM_INTERFACE_ENTRY(IContextMenu)
        COM_INTERFACE_ENTRY(IContextMenu2)
        COM_INTERFACE_ENTRY(IContextMenu3)
    END_COM_MAP()

    DECLARE_REGISTRY_RESOURCEID(IDR_BMPCTXMENUEXT)

public:
    // IShellExtInit
    STDMETHODIMP Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHODIMP QueryContextMenu(HMENU, UINT, UINT, UINT, UINT);
    STDMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO);
    STDMETHODIMP GetCommandString(UINT, UINT, UINT*, LPSTR, UINT);

    // IContextMenu2
    STDMETHODIMP HandleMenuMsg(UINT, WPARAM, LPARAM);

    // IContextMenu3
    STDMETHODIMP HandleMenuMsg2(UINT, WPARAM, LPARAM, LRESULT*);

protected:
    TCHAR   m_szFile[MAX_PATH];
    CBitmap m_bmp;
    UINT    m_uOurItemID;

    LONG m_lItemWidth, m_lItemHeight;
    LONG m_lBmpWidth, m_lBmpHeight;

    static const LONG m_lMaxThumbnailSize;
    static const LONG m_l3DBorderWidth;
    static const LONG m_lMenuItemSpacing;
    static const LONG m_lTotalBorderSpace;

    // Helper functions for handling the menu-related messages.
    HRESULT MenuMessageHandler(UINT, WPARAM, LPARAM, LRESULT*);

    HRESULT OnMeasureItem(MEASUREITEMSTRUCT*, LRESULT*);
    HRESULT OnDrawItem(DRAWITEMSTRUCT*, LRESULT*);
};

#endif //__BMPCTXMENUEXT_H_
