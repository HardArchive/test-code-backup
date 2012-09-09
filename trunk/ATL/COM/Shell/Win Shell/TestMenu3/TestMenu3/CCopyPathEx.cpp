// CCopyPathEx.cpp : CCCopyPathEx 的实现

#include "stdafx.h"
#include "CCopyPathEx.h"


void LogTrace(LPCTSTR pszFormat, ...)   
{
	va_list pArgs; 
	char szMessageBuffer[16380]={0}; 
	va_start( pArgs, pszFormat );  
	_vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs);
	va_end( pArgs ); 
	OutputDebugString(szMessageBuffer); 
} 

// CCCopyPathEx
//初始化接口
HRESULT CCCopyPathEx::Initialize (LPCITEMIDLIST pidlFolder,
								  LPDATAOBJECT pDataObj,
								  HKEY hProgID)
{
	LogTrace("进入Shell 初始化……");
	/*
	try
	{
		LogTrace("进入Shell 初始化……");
		if (pDataObj == NULL)
			return E_INVALIDARG;

		// 在数据对象内查找 CF_HDROP 型数据.
		STGMEDIUM medium;
		FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

		HRESULT hr = pDataObj->GetData( &fe, &medium );
		if ( FAILED( hr ) )
			return E_INVALIDARG;

		//得到选择文件数量
		m_cFiles = DragQueryFile( reinterpret_cast<HDROP>(medium.hGlobal), 0xFFFFFFFF, NULL, NULL );
		LogTrace("得到选择文件数量");

		// Build the list of file names
		m_lstFiles.clear();
		for ( int iFile = 0; iFile < m_cFiles; iFile++ )
		{
			//得到选择文件名
			LogTrace("得到选择文件名");
			memset( m_szFile, 0, sizeof m_szFile );
			DragQueryFile(
				reinterpret_cast<HDROP>(medium.hGlobal),
				iFile, 
				m_szFile, 
				sizeof(m_szFile)/sizeof(m_szFile[0]));
			m_lstFiles.push_back( m_szFile );
		}

		//释放
		LogTrace("释放……");
		ReleaseStgMedium(&medium);
		return hr;

	} 
	catch ( ... ) 
	{
		LogTrace("初始化异常………………");
		return E_FAIL;
	}
*/
	//////////////////////////////////////////////////////
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop; 

	LogTrace("在数据对象内查找 CF_HDROP 型数据");
	// 在数据对象内查找 CF_HDROP 型数据. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}
    LogTrace("获得指向实际数据的指针");
	// 获得指向实际数据的指针
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	LogTrace(" 检查非NULL");
	// 检查非NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	LogTrace(" 有效性检查 – 保证最少有一个文件名");
	// 有效性检查 – 保证最少有一个文件名. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	LogTrace("取得第一个文件名并把它保存在类成员m_szFile 中");
	// 取得第一个文件名并把它保存在类成员m_szFile 中.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ))
	{ 
		hr = E_INVALIDARG; 
	} 
	LogTrace("初始化完成……，释放数据");
	GlobalUnlock ( stg.hGlobal ); 
	ReleaseStgMedium ( &stg ); 
	return hr; 

}


//修改上下文菜单
HRESULT CCCopyPathEx::QueryContextMenu (HMENU hmenu,
										UINT uMenuIndex,
										UINT uidFirstCmd,
										UINT uidLastCmd,
										UINT uFlags)
{   
	/*
	try
	{
		UINT idCmd = uidFirstCmd;
		LogTrace("菜单值：%d", hmenu);        
		// Add the new item
		::InsertMenu(hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, idCmd++, 
			(1 == m_cFiles ? _T("Copy &Path2") : _T("Copy All &Path2") ) );
		LogTrace("菜单值：%d", hmenu);

		HMENU	hmenu1 =::CreatePopupMenu();
		UINT idCmd1 = 0;
		::InsertMenu(hmenu1, uMenuIndex, MF_STRING , idCmd1++, "test1"); 
		::InsertMenu(hmenu1, uMenuIndex, MF_STRING , idCmd1++, "test2"); 
		::AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hmenu1, "test");



		// Very carefully, add the bitmap.  This bitmap is XORed by the shell, so it won't
		// look very pretty, but it's better than nothing.  The next step is apparently to
		// create an ownerdraw menu and handle painting the icon with IContextMenu3::HandleMenuMsg2
		// so the icons will look pretty like the 'Send To' icons.
		//const HINSTANCE hinst = _AtlBaseModule.GetResourceInstance();



		//if ( hinst )
		//{

		//	// Add the bitmap for the unchecked state, since that's what it is by default.
		//	m_hbmpUnchecked = ::LoadBitmap( hinst, MAKEINTRESOURCE(IDB_COPY_PATH) );
		//	if ( m_hbmpUnchecked ) 
		//	{
		//		::SetMenuItemBitmaps( hmenu, uMenuIndex, MF_BYPOSITION, m_hbmpUnchecked, NULL ); 
		//	}
		//}
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, idCmd - uidFirstCmd);

	} catch ( ... ) 
	{
		return E_FAIL;
	}
	*/
	// 如果标志包含 CMF_DEFAULTONLY 我们不作任何事情. 
	LogTrace("进入添加菜单项……");
	if ( uFlags & CMF_DEFAULTONLY ) 
	{ 
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 ); 
	} 
	LogTrace("添加菜单项…………");
	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T(" Test Munu3") ); 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}

//如果用户是在浏览器窗口中右击文本文件，或选中一个文本文件后单击文件菜单时,状态栏会显示提示帮助.
HRESULT CCCopyPathEx::GetCommandString( UINT idCmd, UINT uFlags,UINT* pwReserved, LPSTR pszName, UINT cchMax ) 
{
    /*
	try {

		USES_CONVERSION;

		// We don't care about the command ID, since we have a single item

		// This is rather funky, but GCS_HELPTEXTA refers to a single bit (4) and 
		// GCS_UNICODE refers to single bit (1), but GCS_HELPTEXTW is a combination
		// of those values (5).  This works on both NT and 95.
		if ( uFlags & GCS_HELPTEXTA ) 
		{

			// I'm assuming that this value always contains the number of bytes
			// available for the message, just to be safe.  So half it for Unicode
			// characters.
			const int cChars = cchMax / sizeof(TCHAR);

			// Create some storage on the stack for building the help text
			LPTSTR szHelpText = reinterpret_cast<LPTSTR>( _alloca( cChars ) );
			memset( szHelpText, 0, cChars );

			// Create a generic version of the help text depending on the number
			// of file selected
			if ( 1 == m_cFiles ) 
			{
				_sntprintf( szHelpText, cChars - 1, _T("Copy full path of file to clipboard (%s)"), m_szFile );
			}
			else 
			{
				const LPTSTR szMultiFileHelpText = _T("Copy full paths of selected files to clipboard");

				_tcsncpy( szHelpText, szMultiFileHelpText, min(cChars - 1, (int)_tcslen( szMultiFileHelpText ) ) );
			}

			// Do character conversion depending on flag
			if ( uFlags & GCS_UNICODE )
			{
				wcsncpy( reinterpret_cast<LPWSTR>(pszName), T2W(szHelpText), cChars );
			} else 
			{
				strncpy(pszName, T2A(szHelpText), cchMax);
			}
		}
	} 
	catch ( ... ) 
	{
		return E_FAIL;
	}

	return S_OK;
	*/

	////////////////////////////////////////////////////////////////////////////////////
	USES_CONVERSION; 
	//检查 idCmd, 它必须是0，因为我们仅有一个添加的菜单项. 
	LogTrace("检查 idCmd, 它必须是0，因为我们仅有一个添加的菜单项. ");
	if ( 0 != idCmd ) 
		return E_INVALIDARG; 
	LogTrace("如果 Explorer 要求帮助字符串，就将它拷贝到提供的缓冲区中");
	// 如果 Explorer 要求帮助字符串，就将它拷贝到提供的缓冲区中. 
	if ( uFlags & GCS_HELPTEXT ) 
	{
		LPCTSTR szText = _T("This is the simple shell extension's help");              
		if ( uFlags & GCS_UNICODE )
		{ 
			// 我们需要将 pszName 转化为一个 Unicode 字符串, 接着使用Unicode字符串拷贝 API. 
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax ); 
			LogTrace("我们需要将 pszName 转化为一个 Unicode 字符串, 接着使用Unicode字符串拷贝 API");
		} 
		else
		{ 
			// 使用 ANSI 字符串拷贝API 来返回帮助字符串. 
			lstrcpynA ( pszName, T2CA(szText), cchMax ); 
			LogTrace("使用 ANSI 字符串拷贝API 来返回帮助字符串");
		} 
		LogTrace("GetCommandString:%s", pszName);
		return S_OK;
	} 
	return E_INVALIDARG; 
}

//当用户点击我们添加的菜单项时该方法将被调用
HRESULT CCCopyPathEx::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	/*
	HWND m_pWnd = NULL;
	LogTrace("菜单ID：%d", LOWORD(pCmdInfo->lpVerb));

	m_pWnd = FindWindow(NULL,_T("Spring                                {8192000D-A7B6-433a-8B40-53A3FC3EC52A}")); // 查找DataRecv进程.
	if(m_pWnd == NULL)
	{
		MessageBox(NULL, TEXT("Unable to find DataRecv."), NULL, MB_OK);
		//return;
	}
	COPYDATASTRUCT cpd = {0}; // 给COPYDATASTRUCT结构赋值.
	cpd.dwData = 0;

	try {
		// copy the string to the clipboard
		if (!::OpenClipboard(pCmdInfo->hwnd))
		{
			// Fail silently
			return S_OK;
		}

		int cTextBytes = 0;
		if ( 1 == m_cFiles ) 
		{
			cTextBytes = (_tcslen( m_szFile ) + 1) * sizeof(TCHAR);
		} 
		else 
		{
			for ( int iFile = 0; iFile < m_cFiles; iFile++ ) 
			{
				cTextBytes += ((m_lstFiles[ iFile ].length() + 2 ) * sizeof(TCHAR));
			}
			cTextBytes += sizeof(TCHAR); // null terminator
		}

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cTextBytes );
		if (hGlobal != NULL)
		{
			LPVOID lpText = GlobalLock(hGlobal);
			memset( lpText, 0, cTextBytes );
			if ( 1 == m_cFiles ) 
			{
				memcpy(lpText, m_szFile, cTextBytes);
				LogTrace("选择一个文件，文件名:%s;", m_szFile);
				//MessageBox(NULL, m_szFile, "Tips", MB_OK);
				cpd.cbData = strlen(m_szFile);
				cpd.lpData = (void*)lpText;
				::SendMessage(m_pWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
				//m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// 发送.
			} 
			else 
			{
				LPTSTR szText = (LPTSTR)lpText;
				for ( int iFile = 0; iFile < m_cFiles; iFile++ ) 
				{
					_tcscat( szText, m_lstFiles[ iFile ].c_str() );
					_tcscat( szText, _T("\r\n") );

				}
				LogTrace("选择%d个文件，文件名:\r\n%s;", iFile, szText);
				//MessageBox(NULL, szText, "Tips", MB_OK);
				cpd.cbData = strlen(szText);
				cpd.lpData = (void*)szText;
				//m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// 发送.
				::SendMessage(m_pWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
			}

			EmptyClipboard();
			GlobalUnlock(hGlobal);

#ifdef _UNICODE
			SetClipboardData(CF_UNICODETEXT, hGlobal);
#else
			SetClipboardData(CF_TEXT, hGlobal);
#endif
		}

		CloseClipboard();

	} 
	catch ( ... ) 
	{
		return E_FAIL;
	}

	return S_OK;
	*/
	///////////////////////////////////////////////////////////////////////
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
			MessageBox (pCmdInfo->hwnd, szMsg, _T("CCCopyPathEx"),
				MB_ICONINFORMATION );
			return S_OK;
		}
		break;
	default:
		return E_INVALIDARG;
		break;
	}
}
