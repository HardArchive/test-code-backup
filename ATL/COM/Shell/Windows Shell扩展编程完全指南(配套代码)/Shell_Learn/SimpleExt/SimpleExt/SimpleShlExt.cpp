// SimpleShlExt.cpp : CSimpleShlExt 的实现

#include "stdafx.h"
#include "SimpleShlExt.h"

#include "atlconv.h"
// 为使用 ATL 字符串转换宏而包含的头文件 

// CSimpleShlExt
//我们要做的是取得被右击选择的文件名,再把该文件名显示在弹出消息框中。
//可能会有多个文件同时被选择右击, 你可以用pDataObj 接口指针获取所有的文件名, 但现在为简单起见, 我们只获取第一个文件名. 
//add by ray 2014-05-15 17:45
HRESULT CSimpleShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{ 
	//文件名的存放格式与你拖放文件到带WS_EX_ACCEPTFILES风格的窗口时使用的文件名格式是一样的。 
	// 这就是说我们可以使用同样的API来获取文件名: DragQueryFile(). 
	// 首先我们先获取包含在IdataObject中的数据句柄: 

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop; 

	// 在数据对象内查找 CF_HDROP 型数据. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}

	// 获得指向实际数据的指针
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	// 检查非NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	//请注意错误检查，特别是指针的检查。 
	// 由于我们的扩展运行在 Explorer 进程内, 要是我们的代码崩溃了, Explorer也会随之崩溃. 在Win 9x上, 这样的一个崩溃可能导致需要重启系统. 
	// 所以, 现在我们有了一个 HDROP 句柄, 我们就可以获取我们需要的文件名了： 


	// 有效性检查 – 保证最少有一个文件名. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	// 取得第一个文件名并把它保存在类成员m_szFile 中.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ))
	{ 
		hr = E_INVALIDARG; 
	} 
	GlobalUnlock ( stg.hGlobal ); 
	ReleaseStgMedium ( &stg ); 
	return hr; 
} 

//add by ray 2014-05-15 17:48
HRESULT CSimpleShlExt::QueryContextMenu ( HMENU hmenu,UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{          
	// 如果标志包含 CMF_DEFAULTONLY 我们不作任何事情. 
	if ( uFlags & CMF_DEFAULTONLY ) 
	{ 
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 ); 
	} 

	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T("SimpleShlExt Test Item") ); 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}



HRESULT CSimpleShlExt::GetCommandString( UINT idCmd, UINT uFlags,UINT* pwReserved, LPSTR pszName, UINT cchMax ) 
{
	USES_CONVERSION; 
	//检查 idCmd, 它必须是0，因为我们仅有一个添加的菜单项. 
	if ( 0 != idCmd ) 
		return E_INVALIDARG; 

	// 如果 Explorer 要求帮助字符串，就将它拷贝到提供的缓冲区中. 
	if ( uFlags & GCS_HELPTEXT ) 
	{
		LPCTSTR szText = _T("This is the simple shell extension's help");              
		if ( uFlags & GCS_UNICODE )
		{ 
			// 我们需要将 pszName 转化为一个 Unicode 字符串, 接着使用Unicode字符串拷贝 API. 
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax ); 
		} 
		else
		{ 
			// 使用 ANSI 字符串拷贝API 来返回帮助字符串. 
			lstrcpynA ( pszName, T2CA(szText), cchMax ); 
		} 
		return S_OK;
	} 
	return E_INVALIDARG; 
}


HRESULT CSimpleShlExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	// 如果lpVerb 实际指向一个字符串, 忽略此次调用并退出.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;
	// 点击的命令索引 – 在这里，唯一合法的索引为0.
	switch ( LOWORD( pCmdInfo->lpVerb ))
	{
	case 0:
		{
			TCHAR szMsg [MAX_PATH + 32];
			wsprintf ( szMsg, _T("The selected file was:\n\n%s"), m_szFile );
			MessageBox ( pCmdInfo->hwnd, szMsg, _T("SimpleShlExt"),
				MB_ICONINFORMATION );
			return S_OK;
		}
		break;
	default:
		return E_INVALIDARG;
		break;
	}
}
