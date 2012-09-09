// Fun.h : CFun µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "Simple2.h"


// CFun

class ATL_NO_VTABLE CFun : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFun, &CLSID_Fun>,
	public IFun
{
public:
	CFun()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FUN)


BEGIN_COM_MAP(CFun)
	COM_INTERFACE_ENTRY(IFun)
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

	STDMETHOD(Add)(LONG n1, LONG n2, LONG* pVal);
	STDMETHOD(Cat)(BSTR s1, BSTR s2, BSTR* pVal);
};

OBJECT_ENTRY_AUTO(__uuidof(Fun), CFun)
