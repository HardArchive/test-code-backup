// Mathe.h : Declaration of the CMathe

#ifndef __MATHE_H_
#define __MATHE_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMathe
class ATL_NO_VTABLE CMathe : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMathe, &CLSID_Mathe>,
	public IMathe,
	public IStr,
	public IMathe2
{
public:
	CMathe()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MATHE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMathe)
	COM_INTERFACE_ENTRY(IMathe)
	COM_INTERFACE_ENTRY(IStr)
	COM_INTERFACE_ENTRY(IMathe2)
END_COM_MAP()

// IMathe
public:
	STDMETHOD(Mul)(/*[in]*/ long n1, /*[in]*/ long n2, /*[out,retval]*/ long *pnVal);
	STDMETHOD(Cat)(/*[in]*/ BSTR s1, /*[in]*/ BSTR s2, /*[out,retval]*/ BSTR *psVal);
	STDMETHOD(Add)(/*[in]*/ long n1, /*[in]*/ long n2, /*[out,retval]*/ long *pnVal);
};

#endif //__MATHE_H_
