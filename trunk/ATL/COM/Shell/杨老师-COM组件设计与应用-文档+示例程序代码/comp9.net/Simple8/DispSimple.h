// DispSimple.h : CDispSimple µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "Simple8.h"


// CDispSimple

class ATL_NO_VTABLE CDispSimple : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDispSimple, &CLSID_DispSimple>,
	public IDispatchImpl<IDispSimple, &IID_IDispSimple, &LIBID_Simple8Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDispSimple()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DISPSIMPLE)


BEGIN_COM_MAP(CDispSimple)
	COM_INTERFACE_ENTRY(IDispSimple)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(Add)(VARIANT v1, VARIANT v2, VARIANT* pVal);
	STDMETHOD(Upper)(BSTR str, BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(DispSimple), CDispSimple)
