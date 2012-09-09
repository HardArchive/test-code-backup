// TxtInfoShlExt.h : Declaration of the CTxtInfoShlExt

#ifndef __TXTINFOSHLEXT_H_
#define __TXTINFOSHLEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CTxtInfoShlExt

class ATL_NO_VTABLE CTxtInfoShlExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTxtInfoShlExt, &CLSID_TxtInfoShlExt>,
	public IDispatchImpl<ITxtInfoShlExt, &IID_ITxtInfoShlExt, &LIBID_TXTINFOLib>,
    public IPersistFile,
    public IQueryInfo
{
public:
	CTxtInfoShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TXTINFOSHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTxtInfoShlExt)
	COM_INTERFACE_ENTRY(ITxtInfoShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IQueryInfo)
END_COM_MAP()

protected:
    // ITxtInfoShlExt
    CString m_sFilename;

public:
    // IPersistFile
    STDMETHOD(GetClassID)(LPCLSID)      { return E_NOTIMPL; }
    STDMETHOD(IsDirty)()                { return E_NOTIMPL; }
    STDMETHOD(Load)(LPCOLESTR, DWORD);
    STDMETHOD(Save)(LPCOLESTR, BOOL)    { return E_NOTIMPL; }
    STDMETHOD(SaveCompleted)(LPCOLESTR) { return E_NOTIMPL; }
    STDMETHOD(GetCurFile)(LPOLESTR*)    { return E_NOTIMPL; }

    // IQueryInfo
    STDMETHOD(GetInfoFlags)(DWORD*)     { return E_NOTIMPL; }
    STDMETHOD(GetInfoTip)(DWORD, LPWSTR*);
};

#endif //__TXTINFOSHLEXT_H_
