// SendToShlExt.h : Declaration of the CSendToShlExt

#ifndef __SENDTOSHLEXT_H_
#define __SENDTOSHLEXT_H_

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CSendToShlExt
class ATL_NO_VTABLE CSendToShlExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSendToShlExt, &CLSID_SendToShlExt>,
	public IDispatchImpl<ISendToShlExt, &IID_ISendToShlExt, &LIBID_SENDTOCLONELib>,
    public IPersistFile,
    public IDropTarget
{
public:
	CSendToShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SENDTOSHLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSendToShlExt)
	COM_INTERFACE_ENTRY(ISendToShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistFile)
	COM_INTERFACE_ENTRY(IDropTarget)
END_COM_MAP()

protected:
    // ISendToShlExt
    CStringList m_lsDroppedFiles;

public:
    // IPersistFile
    STDMETHOD(GetClassID)(LPCLSID)      { return E_NOTIMPL; }
    STDMETHOD(IsDirty)()                { return E_NOTIMPL; }
    STDMETHOD(Load)(LPCOLESTR, DWORD)   { return S_OK;      }
    STDMETHOD(Save)(LPCOLESTR, BOOL)    { return E_NOTIMPL; }
    STDMETHOD(SaveCompleted)(LPCOLESTR) { return E_NOTIMPL; }
    STDMETHOD(GetCurFile)(LPOLESTR*)    { return E_NOTIMPL; }

    // IDropTarget
    STDMETHOD(DragEnter)(IDataObject* pDataObj, DWORD grfKeyState,
                         POINTL pt, DWORD* pdwEffect);

    STDMETHOD(DragOver)(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
        { return E_NOTIMPL; }

    STDMETHOD(DragLeave)()
        { return S_OK; }

    STDMETHOD(Drop)(IDataObject* pDataObj, DWORD grfKeyState,
                    POINTL pt, DWORD* pdwEffect);
};

#endif //__SENDTOSHLEXT_H_
