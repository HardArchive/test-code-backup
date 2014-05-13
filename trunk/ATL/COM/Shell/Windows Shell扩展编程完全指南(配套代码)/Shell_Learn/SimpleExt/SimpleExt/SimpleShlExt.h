// SimpleShlExt.h : CSimpleShlExt ������

#pragma once
#include "resource.h"       // ������

#include "SimpleExt_i.h"
#include "shlobj.h"      //add by ray 2014-05-15 17:40
#include "comdef.h"      //add by ray 2014-05-15 17:40

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif


// CSimpleShlExt

class ATL_NO_VTABLE CSimpleShlExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSimpleShlExt, &CLSID_SimpleShlExt>,
	public IDispatchImpl<ISimpleShlExt, &IID_ISimpleShlExt, &LIBID_SimpleExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,  //add by ray 2014-05-15 17:40
	public IContextMenu    //add by ray 2014-05-15 17:45
{
public:
	CSimpleShlExt()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SIMPLESHLEXT)

//COM_MAP��ATLʵ�� QueryInterface()���Ƶĺ�,���������б����ATL�����ⲿ������QueryInterface()�ܴ����ǵ� COM�����ȡ��Щ�ӿ�
BEGIN_COM_MAP(CSimpleShlExt)
	COM_INTERFACE_ENTRY(ISimpleShlExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)     //add by ray 2014-05-15 17:40
	COM_INTERFACE_ENTRY(IContextMenu)      //add by ray 2014-05-15 17:45
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
	//�����ǵ�shell��չ������ʱ, Explorer ������������ʵ�ֵ�COM����� QueryInterface() ������ȡ��һ�� IShellExtInit �ӿ�ָ��.
	//�ýӿڽ���һ������ Initialize(), �亯��ԭ��Ϊ��

	//Explorer ʹ�ø÷������ݸ����Ǹ��ָ�������Ϣ.
	// PidlFolder���û���ѡ��������ļ����ڵ��ļ��е� PIDL ����. (һ�� PIDL [ָ��ID �б��ָ��] ��һ�����ݽṹ����Ψһ�ر�ʶ����Shell�����ռ���κζ���, һ��Shell�����ռ��еĶ��������Ҳ���Բ�����ʵ���ļ�ϵͳ�еĶ���.)
	// pDataObj ��һ�� IDataObject �ӿ�ָ�룬ͨ�������ǿ��Ի�ȡ�û���ѡ��������ļ�����
	// hProgID ��һ��HKEY ע��������������������ȡ���ǵ�DLL��ע������. 
	// ������򵥵���չ������, ���ǽ�ֻʹ�õ� pDataObj ����. 
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);   //add by ray 2014-05-15 17:45


	//add by ray 2014-05-15 17:48
	// IContextMenu
 
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);  //�޸������Ĳ˵�
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO); 

	//hmenu �����Ĳ˵����.
	// uMenuIndex ������Ӧ����Ӳ˵������ʼλ��.
	// uidFirstCmd �� uidLastCmd �����ǿ���ʹ�õĲ˵�����IDֵ�ķ�Χ. 
	// uFlags ��ʶ��Explorer ����QueryContextMenu()��ԭ��, 
	// �����Ժ��˵����.
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT); 

protected:
	TCHAR m_szFile [MAX_PATH];  //add by ray 2014-05-15 17:45
};

OBJECT_ENTRY_AUTO(__uuidof(SimpleShlExt), CSimpleShlExt)
