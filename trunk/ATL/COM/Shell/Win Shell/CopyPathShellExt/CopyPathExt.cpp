// CopyPathExt.cpp : Implementation of CCopyPathExt
//
// Author: Glenn Carr (GlennCarr@email.com)
// Please send any bugs or fixes to GlennCarr@email.com.
//
// One nice addition to this code would be to check for 'extended' commands, that is
// commands are available when the user holds down the SHIFT key while right-clicking.
// From the documentation, it appears that CMF_EXTENDEDVERBS should be an indicator of
// this, but it doesn't appear to work, and I cannot find an example of it.
//
// This code was based on code written by Dino Esposito in 
// his book, Visual C++ Windows Shell Programming

#include "stdafx.h"
#include "GncShellExt.h"
#include "CopyPathExt.h"

void LogTrace(LPCTSTR pszFormat, ...)   
{
	va_list pArgs; 
	char szMessageBuffer[16380]={0}; 
	va_start( pArgs, pszFormat );  
	_vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs);
	va_end( pArgs ); 
	OutputDebugString(szMessageBuffer); 
} 
/////////////////////////////////////////////////////////////////////////////
// CCopyPathExt

// InvokeCommand::QueryContextMenu

HRESULT CCopyPathExt::QueryContextMenu(
	HMENU	hmenu, 
	UINT	indexMenu, 
	UINT	idCmdFirst, 
	UINT	/*idCmdLast*/, 
	UINT	/*uFlags*/ )
{
	try {
	
		UINT idCmd = idCmdFirst;

        LogTrace("菜单值：%d", hmenu);        
		// Add the new item
		::InsertMenu(hmenu, indexMenu, MF_STRING | MF_BYPOSITION, idCmd++, 
						(1 == m_cFiles ? _T("Copy &Path") : _T("Copy All &Path") ) );
		LogTrace("菜单值：%d", hmenu);

		HMENU	hmenu1 =::CreatePopupMenu();
		UINT idCmd1 = 0;
		::InsertMenu(hmenu1, indexMenu, MF_STRING , idCmd1++, "test1"); 
		::InsertMenu(hmenu1, indexMenu, MF_STRING , idCmd1++, "test2"); 
		::AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hmenu1, "test");
		


		// Very carefully, add the bitmap.  This bitmap is XORed by the shell, so it won't
		// look very pretty, but it's better than nothing.  The next step is apparently to
		// create an ownerdraw menu and handle painting the icon with IContextMenu3::HandleMenuMsg2
		// so the icons will look pretty like the 'Send To' icons.
		const HINSTANCE hinst = _Module.GetResourceInstance();
		if ( hinst )
		{

			// Add the bitmap for the unchecked state, since that's what it is by default.
			m_hbmpUnchecked = ::LoadBitmap( hinst, MAKEINTRESOURCE( IDB_CopyPath ) );
			if ( m_hbmpUnchecked ) 
			{
				::SetMenuItemBitmaps( hmenu, indexMenu, MF_BYPOSITION, m_hbmpUnchecked, NULL ); 
			}
		}
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, idCmd - idCmdFirst);

	} catch ( ... ) 
	{
		return E_FAIL;
	}

}

// IContextMenu::InvokeCommand
HRESULT CCopyPathExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
	LogTrace("菜单ID：%d", LOWORD(lpcmi->lpVerb));
	HWND m_pWnd = NULL;
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
		if (!::OpenClipboard(lpcmi->hwnd))
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
				cTextBytes += ((m_lstFiles[ iFile ].length() + 2 /*\r\n*/) * sizeof(TCHAR));
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
}

// IContextMenu::GetCommandString
HRESULT CCopyPathExt::GetCommandString(UINT /*idCmd*/, UINT uFlags, UINT* /*pwReserved*/, LPSTR pszText, UINT cchMax)
{
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
				wcsncpy( reinterpret_cast<LPWSTR>(pszText), T2W(szHelpText), cChars );
			} else 
			{
				strncpy(pszText, T2A(szHelpText), cchMax);
			}
		}
	} 
	catch ( ... ) 
	{
		return E_FAIL;
	}

	return S_OK;
}

// IShellExtInit::Initialize
HRESULT CCopyPathExt::Initialize(LPCITEMIDLIST /*pidlFolder*/, LPDATAOBJECT lpdobj, HKEY /*hKeyProgID*/)
{
	try 
	{

		if ( lpdobj == NULL )
			return E_INVALIDARG;

		// Get the data as CF_HDROP
		STGMEDIUM medium;
		FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		HRESULT hr = lpdobj->GetData( &fe, &medium );
		if ( FAILED( hr ) )
			return E_INVALIDARG;

		// Get the number of selected files
		//得到选择文件数量
		m_cFiles = DragQueryFile( reinterpret_cast<HDROP>(medium.hGlobal), 0xFFFFFFFF, NULL, NULL );
		
		// Build the list of file names
		m_lstFiles.clear();
		for ( int iFile = 0; iFile < m_cFiles; iFile++ )
		{
			//得到选择文件名
			// Get the name of the selected file
			memset( m_szFile, 0, sizeof m_szFile );
			DragQueryFile( reinterpret_cast<HDROP>(medium.hGlobal), 
						   iFile, 
						   m_szFile, 
						   sizeof(m_szFile)/sizeof(m_szFile[0]) );
			m_lstFiles.push_back( m_szFile );
		}

		//释放菜单
		ReleaseStgMedium(&medium);
		return hr;

	} 
	catch ( ... ) 
	{
		return E_FAIL;
	}
};

// CComObjectRootEx overrides
void CCopyPathExt::FinalRelease()
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
