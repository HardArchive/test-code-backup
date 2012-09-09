// Property.h : CMyProperty 的声明

#pragma once
#include "resource.h"       // 主符号

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
	// 参数："属性名称", 接口属性序号(见IDL文件), 属性页对话窗
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
	// str 属性组件内部对应的成员变量
	CComBSTR m_str;
	// integer 属性组件内部对应的成员变量
	long m_integer;
	// 这个成员变量，是 IPersistPropertyBagImpl 所必须的
	bool m_bRequiresSave;
};

OBJECT_ENTRY_AUTO(__uuidof(Property), CMyProperty)
