// CCopyPathEx.h : CCCopyPathEx 的声明

#pragma once
#include "resource.h"       // 主符号

#include "CopyPath.h"
#include <shlobj.h>
#include <comdef.h>

struct __declspec(uuid("000214e4-0000-0000-c000-000000000046")) IContextMenu; 
_COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu)); 

// CCCopyPathEx

class ATL_NO_VTABLE CCCopyPathEx : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCCopyPathEx, &CLSID_CCopyPathEx>,
	public IDispatchImpl<ICCopyPathEx, &IID_ICCopyPathEx, &LIBID_CopyPathLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CCCopyPathEx():
	  m_cFiles( 0 ),
	  m_bExtended( false ),
	  m_hbmpUnchecked( NULL )
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CCOPYPATHEX)


BEGIN_COM_MAP(CCCopyPathEx)
	COM_INTERFACE_ENTRY(ICCopyPathEx)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu) //上下文菜单交互的接口
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
		// Destroy the bitmap that we created in IContextMenu::QueryContextMenu
		try 
		{
			::DeleteObject( m_hbmpUnchecked );
		} 
		catch ( ... ) 
		{
		}
	}

public:
//protected: 
	TCHAR m_szFile [MAX_PATH];       //用以保存文件名
public: 
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);    //初始化接口
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT); //如果用户是在浏览器窗口中右击文本文件，或选中一个文本文件后单击文件菜单时,状态栏会显示提示帮助.
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);             //当用户点击我们添加的菜单项时该方法将被调用
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);  //修改上下文菜单

private:
	int					m_cFiles;
	bool				m_bExtended;
	vector< string >	m_lstFiles;
	HBITMAP				m_hbmpUnchecked;

};

OBJECT_ENTRY_AUTO(__uuidof(CCopyPathEx), CCCopyPathEx)
