// TxtIconShlExt.h : Declaration of the CTxtIconShlExt

#ifndef __TXTICONSHLEXT_H_
#define __TXTICONSHLEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>
#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// CTxtIconShlExt

class ATL_NO_VTABLE CTxtIconShlExt : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CTxtIconShlExt, &CLSID_TxtIconShlExt>,
    public IDispatchImpl<ITxtIconShlExt, &IID_ITxtIconShlExt, &LIBID_TXTFILEICONSLib>,
    public IPersistFile,
    public IExtractIcon
{
public:
    CTxtIconShlExt()
    {
    }

DECLARE_REGISTRY_RESOURCEID(IDR_TXTICONSHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTxtIconShlExt)
    COM_INTERFACE_ENTRY(ITxtIconShlExt)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IPersistFile)
    COM_INTERFACE_ENTRY(IExtractIcon)
END_COM_MAP()

public:
    // IPersistFile
    STDMETHOD(GetClassID)( CLSID* ) { return E_NOTIMPL; }
    STDMETHOD(IsDirty)() { return E_NOTIMPL; }
    STDMETHOD(Save)( LPCOLESTR, BOOL ) { return E_NOTIMPL; }
    STDMETHOD(SaveCompleted)( LPCOLESTR ) { return E_NOTIMPL; }
    STDMETHOD(GetCurFile)( LPOLESTR* ) { return E_NOTIMPL; }
    STDMETHOD(Load)( LPCOLESTR wszFile, DWORD )
        { 
        USES_CONVERSION;
        lstrcpyn ( m_szFilename, OLE2CT(wszFile), MAX_PATH );
        return S_OK;
        }

    // IExtractIcon
    STDMETHOD(GetIconLocation)( UINT uFlags, LPTSTR szIconFile, UINT cchMax,
                                int* piIndex, UINT* pwFlags );
    STDMETHOD(Extract)( LPCTSTR pszFile, UINT nIconIndex, HICON* phiconLarge,
                        HICON* phiconSmall, UINT nIconSize );

protected:
    TCHAR     m_szFilename [MAX_PATH];  // Full path to the file in question.
    DWORDLONG m_ldwFileSize;            // Used by extraction method 2.
};

#endif //__TXTICONSHLEXT_H_
