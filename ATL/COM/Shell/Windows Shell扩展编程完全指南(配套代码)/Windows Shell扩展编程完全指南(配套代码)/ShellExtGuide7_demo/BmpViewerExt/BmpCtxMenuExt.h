// BmpCtxMenuExt.h : Declaration of the CBmpCtxMenuExt

#ifndef __BMPCTXMENUEXT_H_
#define __BMPCTXMENUEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// CBmpCtxMenuExt

class ATL_NO_VTABLE CBmpCtxMenuExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CBmpCtxMenuExt, &CLSID_BmpCtxMenuExt>,
	public IDispatchImpl<IBmpCtxMenuExt, &IID_IBmpCtxMenuExt, &LIBID_BMPVIEWEREXTLib>,
    public IShellExtInit,
    public IContextMenu3
{
public:
	CBmpCtxMenuExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_BMPCTXMENUEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CBmpCtxMenuExt)
	COM_INTERFACE_ENTRY(IBmpCtxMenuExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
	COM_INTERFACE_ENTRY(IContextMenu2)
	COM_INTERFACE_ENTRY(IContextMenu3)
END_COM_MAP()

public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);

    // IContextMenu2
    STDMETHOD(HandleMenuMsg)(UINT, WPARAM, LPARAM);

    // IContextMenu3
    STDMETHOD(HandleMenuMsg2)(UINT, WPARAM, LPARAM, LRESULT*);

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

    STDMETHOD(MenuMessageHandler)(UINT, WPARAM, LPARAM, LRESULT*);

    STDMETHOD(OnMeasureItem)(MEASUREITEMSTRUCT*, LRESULT*);
    STDMETHOD(OnDrawItem)(DRAWITEMSTRUCT*, LRESULT*);
};

#endif //__BMPCTXMENUEXT_H_
