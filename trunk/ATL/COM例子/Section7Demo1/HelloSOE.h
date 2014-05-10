// HelloSOE.h : CHelloSOE ������

#pragma once
#include "resource.h"       // ������

#include "Section7Demo1.h"
#include "_IHelloSOEEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CHelloSOE

class ATL_NO_VTABLE CHelloSOE :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloSOE, &CLSID_HelloSOE>,
	public IConnectionPointContainerImpl<CHelloSOE>,
	public CProxy_IHelloSOEEvents<CHelloSOE>,
	public IDispatchImpl<IHelloSOE, &IID_IHelloSOE, &LIBID_Section7Demo1Lib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHelloSOE()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HELLOSOE)


BEGIN_COM_MAP(CHelloSOE)
	COM_INTERFACE_ENTRY(IHelloSOE)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CHelloSOE)
	CONNECTION_POINT_ENTRY(__uuidof(_IHelloSOEEvents))
END_CONNECTION_POINT_MAP()


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
	STDMETHOD(Add)(LONG lA, LONG lB);
};

OBJECT_ENTRY_AUTO(__uuidof(HelloSOE), CHelloSOE)
