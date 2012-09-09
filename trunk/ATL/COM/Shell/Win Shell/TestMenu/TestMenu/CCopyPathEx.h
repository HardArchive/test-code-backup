// CCopyPathEx.h : CCCopyPathEx ������

#pragma once
#include "resource.h"       // ������

#include "TestMenu.h"
#include <shlobj.h>
#include <comdef.h>

// CCCopyPathEx

class ATL_NO_VTABLE CCCopyPathEx : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCCopyPathEx, &CLSID_CCopyPathEx>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CCCopyPathEx()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CCOPYPATHEX)


BEGIN_COM_MAP(CCCopyPathEx)
//	COM_INTERFACE_ENTRY(ICCopyPathEx)
	//COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu) //�����Ĳ˵������Ľӿ�
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{

	}

//public:

protected: 
	TCHAR m_szFile [MAX_PATH];       //���Ա����ļ���
public: 
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);    //��ʼ���ӿ�
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT); //����û�����������������һ��ı��ļ�����ѡ��һ���ı��ļ��󵥻��ļ��˵�ʱ,״̬������ʾ��ʾ����.
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);             //���û����������ӵĲ˵���ʱ�÷�����������
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);  //�޸������Ĳ˵�

private:
	int					m_cFiles;
	bool				m_bExtended;
	vector< string >	m_lstFiles;
	HBITMAP				m_hbmpUnchecked;


};

OBJECT_ENTRY_AUTO(__uuidof(CCopyPathEx), CCCopyPathEx)
