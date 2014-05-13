// DllRegShlExt.cpp : CDllRegShlExt 的实现

#include "stdafx.h"
#include "DllRegShlExt.h"

#include "dllmain.h"
// CDllRegShlExt
#include "ProgressDlg.h"
CDllRegShlExt::CDllRegShlExt()
{
	//add by ray 2014-05-13 11:22
	m_hRegBmp = LoadBitmap ( _Module.GetModuleInstance(),
		MAKEINTRESOURCE(IDB_REGISTERBMP) );
	m_hUnregBmp = LoadBitmap ( _Module.GetModuleInstance(),
		MAKEINTRESOURCE(IDB_UNREGISTERBMP) );
}

HRESULT CDllRegShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{
	TCHAR szFile [MAX_PATH];
	TCHAR szFolder [MAX_PATH];
	TCHAR szCurrDir [MAX_PATH];
	TCHAR* pszLastBackslash;
	UINT uNumFiles;
	HDROP hdrop;
	FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HINSTANCE hinst;
	bool bChangedDir = false;
	HRESULT (STDAPICALLTYPE* pfn)();

	// 从数据对象中读取文件列表. 他们存储在HDROP 格式中
	// 因此，取得 HDROP 句柄，并使用拖放API
	if ( FAILED( pDataObj->GetData (&etc, &stg)))
		return E_INVALIDARG;

	//取得HDROP 句柄.
	hdrop = (HDROP)GlobalLock ( stg.hGlobal );
	if ( NULL == hdrop )
	{
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}

	// 检查在该操作中有几个文件被选择.
	uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 );

	for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
	{
		//取得下一个文件名.
		if ( 0 == DragQueryFile ( hdrop, 
			uFile, szFile, MAX_PATH ))
			continue;
		//试着加载文件.
		hinst = LoadLibrary ( szFile ); 
		if ( NULL == hinst)
			continue; 

		// 获取 DllRegisterServer()函数的地址;
		(FARPROC&) pfn = GetProcAddress ( hinst, "DllRegisterServer" );
		// 如果没找着, 跳过该文件.
		if ( NULL == pfn )
		{
			FreeLibrary ( hinst );
			continue;
		}
		// 获取DllUnregisterServer()函数的地址;
		(FARPROC&) pfn = GetProcAddress ( hinst, "DllUnregisterServer" );
		// 如果有，我们就可以处理该文件, 因此添加其到我们的文件名列表中去(m_lsFiles).
		if ( NULL != pfn )
		{
			m_lsFiles.push_back ( szFile );
		}
		FreeLibrary ( hinst );
	} // end for 

	// 释放资源.
	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );
	// 如果我们找到可以操作的文件, 返回 S_OK.
	// 否则,返回E_INVALIDARG，我们的代码就不会再被右击事件调用。
	return ( m_lsFiles.size() > 0 ) ? S_OK : E_INVALIDARG;
}


HRESULT CDllRegShlExt::QueryContextMenu ( HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{
	UINT uCmdID = uidFirstCmd; 

	// 如果 CMF_DEFAULTONLY 标志被设置我们不作任何操作. 
	if ( uFlags & CMF_DEFAULTONLY ) 
	{
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	}

	// 添加 register/unregister 项.
	InsertMenu (hmenu, uMenuIndex,MF_STRING | MF_BYPOSITION, uCmdID++, _T("Register server(s)"));

	// 为register项设置位图.
	if ( NULL != m_hRegBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hRegBmp, NULL );
	}

	uMenuIndex++;

	InsertMenu (hmenu,uMenuIndex,MF_STRING | MF_BYPOSITION, uCmdID++, _T("Unregister server(s)"));

	// 设置 unregister 项的位图.
	if ( NULL != m_hUnregBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hUnregBmp, NULL
			);
	}

	//最后, 我们告诉浏览器我们添加了几个菜单项.
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 2 );
}


#include <atlconv.h>
HRESULT CDllRegShlExt::GetCommandString ( UINT uCmdID, UINT uFlags,              UINT* puReserved, LPSTR szName, UINT cchMax )
{
	LPCTSTR szPrompt;
	USES_CONVERSION; 
	if ( uFlags & GCS_HELPTEXT )
	{
		switch ( uCmdID ) 
		{ 
		case 0: szPrompt = _T("Register all selected COM servers");
			break; 
		case 1: szPrompt = _T("Unregister all selected COM servers"); 
			break; 
		default:  return E_INVALIDARG; 
			break; 
		}

		// 拷贝帮助字符串到提供的缓冲区中. 如果Shell需要Unicode字符串，我们需要转化szName 到 LPCWSTR.
		if ( uFlags & GCS_UNICODE )
		{
			lstrcpynW ( (LPWSTR) szName, T2CW(szPrompt), cchMax );
		}
		else
		{
			lstrcpynA ( szName, T2CA(szPrompt), cchMax );
		}
	}
	else if ( uFlags & GCS_VERB )
	{
		// Copy the verb name into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.

		switch ( uCmdID )
		{
		case 0:
			if ( uFlags & GCS_UNICODE )
			{
				lstrcpynW ( (LPWSTR) szName, L"DllRegSvr", cchMax );
			}
			else
			{
				lstrcpynA ( szName, "DllRegSvr", cchMax );
			}
			break;

		case 1:
			if ( uFlags & GCS_UNICODE )
			{
				lstrcpynW ( (LPWSTR) szName, L"DllUnregSvr", cchMax );
			}
			else
			{
				lstrcpynA ( szName, "DllUnregSvr", cchMax );
			}
			break;

		default:
			ATLASSERT(0);           // should never get here
			return E_INVALIDARG;
			break;
		}
	}

	return S_OK;

}



HRESULT CDllRegShlExt::InvokeCommand (LPCMINVOKECOMMANDINFO pCmdInfo)
{
	// 如果 lpVerb 指向一字符串, 忽略此次调用. 
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;

	// 检查lpVerb 是不是我们添加的命令(0 或 1) 
	switch ( LOWORD( pCmdInfo->lpVerb )) 
	{ 
	case 0: 
	case 1:                        {
		CProgressDlg dlg ( &m_lsFiles, pCmdInfo );
		dlg.DoModal();
		return S_OK; 
								   }
								   break;
	default: 
		return E_INVALIDARG; 
		break; 
	}
}        
