// First_ATL.h : CFirst_ATL ������

#pragma once
#include "resource.h"       // ������

#include "Simple_ATL_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CFirst_ATL

class ATL_NO_VTABLE CFirst_ATL :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFirst_ATL, &CLSID_First_ATL>,
	public IDispatchImpl<IFirst_ATL, &IID_IFirst_ATL, &LIBID_Simple_ATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFirst_ATL()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIRST_ATL)


BEGIN_COM_MAP(CFirst_ATL)
	COM_INTERFACE_ENTRY(IFirst_ATL)
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

	STDMETHOD(AddNumbers)(LONG Num1, LONG Num2, LONG* ReturnVal);
};

OBJECT_ENTRY_AUTO(__uuidof(First_ATL), CFirst_ATL)
