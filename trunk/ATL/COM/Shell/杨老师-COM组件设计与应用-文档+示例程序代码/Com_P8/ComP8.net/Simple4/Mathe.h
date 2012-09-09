// Mathe.h : CMathe ������

#pragma once
#include "resource.h"       // ������

#include "Simple4.h"


// CMathe

class ATL_NO_VTABLE CMathe : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMathe, &CLSID_Mathe>,
	public IMathe,
	public IStr,	// �ֹ����
	public IMathe2	// �ֹ����
{
public:
	CMathe()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MATHE)


BEGIN_COM_MAP(CMathe)
	COM_INTERFACE_ENTRY(IMathe)
	COM_INTERFACE_ENTRY(IStr)		// �ֹ����
	COM_INTERFACE_ENTRY(IMathe2)		// �ֹ����
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

	STDMETHOD(Add)(LONG n1, LONG n2, LONG* pnVal);
	STDMETHOD(Cat)(BSTR s1, BSTR s2, BSTR* psVal);
	STDMETHOD(Mul)(LONG n1, LONG n2, LONG* pnVal);
};

OBJECT_ENTRY_AUTO(__uuidof(Mathe), CMathe)
