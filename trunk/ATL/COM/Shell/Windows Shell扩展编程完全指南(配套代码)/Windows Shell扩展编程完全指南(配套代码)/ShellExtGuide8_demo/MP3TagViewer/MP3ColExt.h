// MP3ColExt.h : Declaration of the CMP3ColExt

#ifndef __MP3COLEXT_H_
#define __MP3COLEXT_H_

#include "resource.h"       // main symbols

#include <comdef.h>
#include <shlobj.h>
#include <shlguid.h>

struct __declspec(uuid("E8025004-1C42-11d2-BE2C-00A0C9A83DA1")) IColumnProvider;

/////////////////////////////////////////////////////////////////////////////
// CMP3ColExt

class ATL_NO_VTABLE CMP3ColExt : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CMP3ColExt, &CLSID_MP3ColExt>,
    public IMP3ColExt,
    public IColumnProvider
{
public:
    CMP3ColExt() { }

DECLARE_REGISTRY_RESOURCEID(IDR_MP3COLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMP3ColExt)
    COM_INTERFACE_ENTRY(IMP3ColExt)
    COM_INTERFACE_ENTRY(IColumnProvider)
END_COM_MAP()

    // IMP3ColExt
public:

    // IColumnProvider 
public:
    STDMETHOD (Initialize)(LPCSHCOLUMNINIT psci) { return S_OK; }
    STDMETHOD (GetColumnInfo)(DWORD dwIndex, SHCOLUMNINFO* psci);
    STDMETHOD (GetItemData)(LPCSHCOLUMNID pscid, LPCSHCOLUMNDATA pscd, VARIANT* pvarData);

    // Implementation
protected:
    void GetGenreName ( const CID3v1Tag& tag, char* szField, const UINT cchBuffSize );
    bool ReadTagFromFile ( LPCTSTR szFilename, CID3v1Tag* pTag );

    list_ID3Cache m_ID3Cache;
};

#endif //__MP3COLEXT_H_
