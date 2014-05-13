// DllRegShlExt.h : CDllRegShlExt ������

#pragma once
#include "resource.h"       // ������

#include "DllReg_i.h"

#include "resource.h"       // main symbols
#include <comdef.h>
#include <shlobj.h>
#include <shlguid.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CDllRegShlExt

class ATL_NO_VTABLE CDllRegShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDllRegShlExt, &CLSID_DllRegShlExt>,
	public IDispatchImpl<IDllRegShlExt, &IID_IDllRegShlExt, &LIBID_DllRegLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CDllRegShlExt();

DECLARE_REGISTRY_RESOURCEID(IDR_DLLREGSHLEXT)


BEGIN_COM_MAP(CDllRegShlExt)
	COM_INTERFACE_ENTRY(IDllRegShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
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
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO); 
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT); 


protected:
	//add by ray 2014-05-13 11:22 CDllRegShlExt ����ҪһЩ��Ա����:
	HBITMAP m_hRegBmp;
	HBITMAP m_hUnregBmp;
	string_list m_lsFiles;
	TCHAR m_szDir [MAX_PATH];
};

OBJECT_ENTRY_AUTO(__uuidof(DllRegShlExt), CDllRegShlExt)
