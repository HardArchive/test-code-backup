// Property.h : CMyProperty ������

#pragma once
#include "resource.h"       // ������

#include "Simple18.h"
#include "atlcomcli.h"


// CMyProperty

class ATL_NO_VTABLE CMyProperty : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyProperty, &CLSID_Property>,
	public IDispatchImpl<IProperty, &IID_IProperty, &LIBID_Simple18Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistPropertyBagImpl<CMyProperty>
{
public:
	CMyProperty()
		: m_integer(0)
		, m_bRequiresSave(false)
	{

	}

DECLARE_REGISTRY_RESOURCEID(IDR_PROPERTY)


BEGIN_COM_MAP(CMyProperty)
	COM_INTERFACE_ENTRY(IProperty)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IPersistPropertyBag)
END_COM_MAP()

BEGIN_PROP_MAP(CMyProperty)
	// ������"��������", �ӿ��������(��IDL�ļ�), ����ҳ�Ի���
	PROP_ENTRY("str", 1, CLSID_NULL)
	PROP_ENTRY("integer", 2, CLSID_NULL)
END_PROP_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(get_str)(BSTR* pVal);
	STDMETHOD(put_str)(BSTR newVal);
	STDMETHOD(get_integer)(LONG* pVal);
	STDMETHOD(put_integer)(LONG newVal);
	// str ��������ڲ���Ӧ�ĳ�Ա����
	CComBSTR m_str;
	// integer ��������ڲ���Ӧ�ĳ�Ա����
	long m_integer;
	// �����Ա�������� IPersistPropertyBagImpl �������
	bool m_bRequiresSave;
};

OBJECT_ENTRY_AUTO(__uuidof(Property), CMyProperty)
