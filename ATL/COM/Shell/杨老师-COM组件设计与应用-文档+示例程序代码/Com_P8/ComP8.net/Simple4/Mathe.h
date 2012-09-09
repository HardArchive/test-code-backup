// Mathe.h : CMathe 的声明

#pragma once
#include "resource.h"       // 主符号

#include "Simple4.h"


// CMathe

class ATL_NO_VTABLE CMathe : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMathe, &CLSID_Mathe>,
	public IMathe,
	public IStr,	// 手工添加
	public IMathe2	// 手工添加
{
public:
	CMathe()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MATHE)


BEGIN_COM_MAP(CMathe)
	COM_INTERFACE_ENTRY(IMathe)
	COM_INTERFACE_ENTRY(IStr)		// 手工添加
	COM_INTERFACE_ENTRY(IMathe2)		// 手工添加
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
