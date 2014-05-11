// HelloSampleObject.h : Declaration of the CHelloSampleObject

#pragma once
#include "resource.h"       // main symbols

#include "SampleObject_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CHelloSampleObject

class ATL_NO_VTABLE CHelloSampleObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHelloSampleObject, &CLSID_HelloSampleObject>,
	public IDispatchImpl<IHelloSampleObject, &IID_IHelloSampleObject, &LIBID_SampleObjectLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHelloSampleObject()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HELLOSAMPLEOBJECT)


BEGIN_COM_MAP(CHelloSampleObject)
	COM_INTERFACE_ENTRY(IHelloSampleObject)
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

	STDMETHOD(Sumlong)(LONG lA, LONG lB, LONG* plSum);

};

OBJECT_ENTRY_AUTO(__uuidof(HelloSampleObject), CHelloSampleObject)
