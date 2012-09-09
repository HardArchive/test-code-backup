// InterFaceCls.h : Declaration of the CInterFaceCls

#ifndef __INTERFACECLS_H_
#define __INTERFACECLS_H_

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>

/////////////////////////////////////////////////////////////////////////////
// CInterFaceCls
class ATL_NO_VTABLE CInterFaceCls : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CInterFaceCls, &CLSID_InterFaceCls>,
	public IShellExtInit,
    public IContextMenu
{

public:
	CInterFaceCls()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_INTERFACECLS)
DECLARE_NOT_AGGREGATABLE(CInterFaceCls)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CInterFaceCls)
	COM_INTERFACE_ENTRY(IShellExtInit)
    COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

// IInterFaceCls

public:

  STDMETHODIMP Initialize( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID );

  STDMETHODIMP GetCommandString( UINT idCmd, UINT uFlags, UINT* pwReserved, LPSTR pszName, UINT cchMax );
  
  STDMETHODIMP InvokeCommand( LPCMINVOKECOMMANDINFO pCmdInfo );
  
  STDMETHODIMP QueryContextMenu( HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags );

private:

    CString m_csPath;
};

#endif //__INTERFACECLS_H_
