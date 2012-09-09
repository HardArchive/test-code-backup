// CopyPathExt.h : Declaration of the CCopyPathExt
//
// Author: Glenn Carr (GlennCarr@email.com)
// Please send any bugs or fixes to GlennCarr@email.com.
//
// This code was based on code written by Dino Esposito in 
// his book, Visual C++ Windows Shell Programming

#ifndef __COPYPATHEXT_H_
#define __COPYPATHEXT_H_

#include "resource.h"       // main symbols
#include "IContextMenuImpl.h"          // IContextMenu
#include "IShellExtInitImpl.h"         // IShellExtInit
#include <comdef.h>                    // Interface IDs

//#include <shlguid.h>


struct __declspec(uuid("000214e4-0000-0000-c000-000000000046")) IContextMenu; _COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu)); 
/////////////////////////////////////////////////////////////////////////////
// CCopyPathExt
class ATL_NO_VTABLE CCopyPathExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCopyPathExt, &CLSID_CopyPathExt>,
    public IShellExtInitImpl,
    public IContextMenuImpl
{
public:
	CCopyPathExt() : 
		m_cFiles( 0 ), 
		m_bExtended( false ),
		m_hbmpUnchecked( NULL )
	{
	}

	// CComObjectRootEx
	virtual void FinalRelease();

	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT , UINT, UINT);

	// IShellExtInit
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

DECLARE_REGISTRY_RESOURCEID(IDR_COPYPATHEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCopyPathExt)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
	//COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)
END_COM_MAP()

// Implementation
private:
	int					m_cFiles;
	bool				m_bExtended;
	vector< string >	m_lstFiles;
	HBITMAP				m_hbmpUnchecked;
};

#endif //__COPYPATHEXT_H_
