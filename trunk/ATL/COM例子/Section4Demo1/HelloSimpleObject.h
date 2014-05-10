// HelloSimpleObject.h : CHelloSimpleObject ������

#pragma once
#include "resource.h"       // ������

#include "Section4Demo1.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CHelloSimpleObject

class ATL_NO_VTABLE CHelloSimpleObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloSimpleObject, &CLSID_HelloSimpleObject>,
	public IDispatchImpl<IHelloSimpleObject, &IID_IHelloSimpleObject, &LIBID_Section4Demo1Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHelloSimpleObject()
	{
		m_bstrDescription = SysAllocString(L"����HelloSimpleObject��������Ǽ򵥶���");
	}
	virtual ~CHelloSimpleObject()
	{
		SysFreeString(m_bstrDescription);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HELLOSIMPLEOBJECT)


BEGIN_COM_MAP(CHelloSimpleObject)
	COM_INTERFACE_ENTRY(IHelloSimpleObject)
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

public:
	STDMETHOD(SumLong)(LONG lA, LONG lB, LONG* plSum);
public:
	STDMETHOD(get_Description)(BSTR* pVal);
public:
	STDMETHOD(put_Description)(BSTR newVal);

private:
	//Description������
	BSTR m_bstrDescription;

};

OBJECT_ENTRY_AUTO(__uuidof(HelloSimpleObject), CHelloSimpleObject)
