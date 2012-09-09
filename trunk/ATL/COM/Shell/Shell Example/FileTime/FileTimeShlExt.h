// FileTimeShlExt.h : Declaration of the CFileTimeShlExt

#ifndef __FILETIMESHLEXT_H_
#define __FILETIMESHLEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CFileTimeShlExt
class ATL_NO_VTABLE CFileTimeShlExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFileTimeShlExt, &CLSID_FileTimeShlExt>,
	public IDispatchImpl<IFileTimeShlExt, &IID_IFileTimeShlExt, &LIBID_FILETIMELib>,
    public IShellExtInit,
    public IShellPropSheetExt
{
public:
	CFileTimeShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILETIMESHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CFileTimeShlExt)
	COM_INTERFACE_ENTRY(IFileTimeShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IShellPropSheetExt)
END_COM_MAP()

protected:
    // IFileTimeShlExt
    string_list m_lsFiles;

public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IShellPropSheetExt
    STDMETHOD(AddPages)(LPFNADDPROPSHEETPAGE, LPARAM);
    STDMETHOD(ReplacePage)(UINT, LPFNADDPROPSHEETPAGE, LPARAM) { return E_NOTIMPL; }
};

#endif //__FILETIMESHLEXT_H_
