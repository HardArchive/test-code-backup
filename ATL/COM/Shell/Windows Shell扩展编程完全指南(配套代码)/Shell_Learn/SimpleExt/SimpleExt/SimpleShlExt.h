// SimpleShlExt.h : CSimpleShlExt 的声明

#pragma once
#include "resource.h"       // 主符号

#include "SimpleExt_i.h"
#include "shlobj.h"      //add by ray 2014-05-15 17:40
#include "comdef.h"      //add by ray 2014-05-15 17:40

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
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

//COM_MAP是ATL实现 QueryInterface()机制的宏,它包含的列表告诉ATL其它外部程序用QueryInterface()能从我们的 COM对象获取哪些接口
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
	//当我们的shell扩展被加载时, Explorer 将调用我们所实现的COM对象的 QueryInterface() 函数以取得一个 IShellExtInit 接口指针.
	//该接口仅有一个方法 Initialize(), 其函数原型为：

	//Explorer 使用该方法传递给我们各种各样的信息.
	// PidlFolder是用户所选择操作的文件所在的文件夹的 PIDL 变量. (一个 PIDL [指向ID 列表的指针] 是一个数据结构，它唯一地标识了在Shell命名空间的任何对象, 一个Shell命名空间中的对象可以是也可以不是真实的文件系统中的对象.)
	// pDataObj 是一个 IDataObject 接口指针，通过它我们可以获取用户所选择操作的文件名。
	// hProgID 是一个HKEY 注册表键变量，可以用它获取我们的DLL的注册数据. 
	// 在这个简单的扩展例子中, 我们将只使用到 pDataObj 参数. 
	// IShellExtInit 
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);   //add by ray 2014-05-15 17:45


	//add by ray 2014-05-15 17:48
	// IContextMenu
 
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);  //修改上下文菜单
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO); 

	//hmenu 上下文菜单句柄.
	// uMenuIndex 是我们应该添加菜单项的起始位置.
	// uidFirstCmd 和 uidLastCmd 是我们可以使用的菜单命令ID值的范围. 
	// uFlags 标识了Explorer 调用QueryContextMenu()的原因, 
	// 这我以后会说到的.
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT); 

protected:
	TCHAR m_szFile [MAX_PATH];  //add by ray 2014-05-15 17:45
};

OBJECT_ENTRY_AUTO(__uuidof(SimpleShlExt), CSimpleShlExt)
