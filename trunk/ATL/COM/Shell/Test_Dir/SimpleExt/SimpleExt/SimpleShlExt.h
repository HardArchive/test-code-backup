// SimpleShlExt.h : CSimpleShlExt ������

#pragma once
#include "resource.h"       // ������

#include "SimpleExt_i.h"

//add by rg & 2013-11-29 16:42:10 
#include "shlobj.h"
#include "comdef.h" 


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CSimpleShlExt

class ATL_NO_VTABLE CSimpleShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSimpleShlExt, &CLSID_SimpleShlExt>,
	public IDispatchImpl<ISimpleShlExt, &IID_ISimpleShlExt, &LIBID_SimpleExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	//add by rg & 2013-11-29 16:52:49
	public IShellExtInit
{
public:
	CSimpleShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLESHLEXT)


BEGIN_COM_MAP(CSimpleShlExt)
	COM_INTERFACE_ENTRY(ISimpleShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	//add by rg & 2013-11-29 16:52:59
	COM_INTERFACE_ENTRY(IShellExtInit) 
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
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	
protected: 
	//add by rg & 2013-11-29 17:47:25
	TCHAR m_szFile [MAX_PATH];    //�����ļ���


};

OBJECT_ENTRY_AUTO(__uuidof(SimpleShlExt), CSimpleShlExt)
