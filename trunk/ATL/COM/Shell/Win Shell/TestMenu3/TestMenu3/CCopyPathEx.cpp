// CCopyPathEx.cpp : CCCopyPathEx ��ʵ��

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
//��ʼ���ӿ�
HRESULT CCCopyPathEx::Initialize (LPCITEMIDLIST pidlFolder,
								  LPDATAOBJECT pDataObj,
								  HKEY hProgID)
{
	LogTrace("����Shell ��ʼ������");
	/*
	try
	{
		LogTrace("����Shell ��ʼ������");
		if (pDataObj == NULL)
			return E_INVALIDARG;

		// �����ݶ����ڲ��� CF_HDROP ������.
		STGMEDIUM medium;
		FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

		HRESULT hr = pDataObj->GetData( &fe, &medium );
		if ( FAILED( hr ) )
			return E_INVALIDARG;

		//�õ�ѡ���ļ�����
		m_cFiles = DragQueryFile( reinterpret_cast<HDROP>(medium.hGlobal), 0xFFFFFFFF, NULL, NULL );
		LogTrace("�õ�ѡ���ļ�����");

		// Build the list of file names
		m_lstFiles.clear();
		for ( int iFile = 0; iFile < m_cFiles; iFile++ )
		{
			//�õ�ѡ���ļ���
			LogTrace("�õ�ѡ���ļ���");
			memset( m_szFile, 0, sizeof m_szFile );
			DragQueryFile(
				reinterpret_cast<HDROP>(medium.hGlobal),
				iFile, 
				m_szFile, 
				sizeof(m_szFile)/sizeof(m_szFile[0]));
			m_lstFiles.push_back( m_szFile );
		}

		//�ͷ�
		LogTrace("�ͷš���");
		ReleaseStgMedium(&medium);
		return hr;

	} 
	catch ( ... ) 
	{
		LogTrace("��ʼ���쳣������������");
		return E_FAIL;
	}
*/
	//////////////////////////////////////////////////////
	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL }; 
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP hDrop; 

	LogTrace("�����ݶ����ڲ��� CF_HDROP ������");
	// �����ݶ����ڲ��� CF_HDROP ������. 
	if ( FAILED( pDataObj->GetData ( &fmt, &stg ))) 
	{ 
		// Nope! Return an "invalid argument" error back to Explorer.
		return E_INVALIDARG; 
	}
    LogTrace("���ָ��ʵ�����ݵ�ָ��");
	// ���ָ��ʵ�����ݵ�ָ��
	hDrop = (HDROP) GlobalLock ( stg.hGlobal );

	LogTrace(" ����NULL");
	// ����NULL. 
	if ( NULL == hDrop ) 
	{ 
		return E_INVALIDARG;
	} 

	LogTrace(" ��Ч�Լ�� �C ��֤������һ���ļ���");
	// ��Ч�Լ�� �C ��֤������һ���ļ���. 
	UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 ); 
	if ( 0 == uNumFiles )
	{ 
		GlobalUnlock ( stg.hGlobal );
		ReleaseStgMedium ( &stg ); 
		return E_INVALIDARG; 
	} 
	HRESULT hr = S_OK; 

	LogTrace("ȡ�õ�һ���ļ������������������Աm_szFile ��");
	// ȡ�õ�һ���ļ������������������Աm_szFile ��.
	if ( 0 == DragQueryFile ( hDrop, 0, m_szFile, MAX_PATH ))
	{ 
		hr = E_INVALIDARG; 
	} 
	LogTrace("��ʼ����ɡ������ͷ�����");
	GlobalUnlock ( stg.hGlobal ); 
	ReleaseStgMedium ( &stg ); 
	return hr; 

}


//�޸������Ĳ˵�
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
		LogTrace("�˵�ֵ��%d", hmenu);        
		// Add the new item
		::InsertMenu(hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, idCmd++, 
			(1 == m_cFiles ? _T("Copy &Path2") : _T("Copy All &Path2") ) );
		LogTrace("�˵�ֵ��%d", hmenu);

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
	// �����־���� CMF_DEFAULTONLY ���ǲ����κ�����. 
	LogTrace("������Ӳ˵����");
	if ( uFlags & CMF_DEFAULTONLY ) 
	{ 
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 ); 
	} 
	LogTrace("��Ӳ˵��������");
	InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T(" Test Munu3") ); 
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}

//����û�����������������һ��ı��ļ�����ѡ��һ���ı��ļ��󵥻��ļ��˵�ʱ,״̬������ʾ��ʾ����.
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
	//��� idCmd, ��������0����Ϊ���ǽ���һ����ӵĲ˵���. 
	LogTrace("��� idCmd, ��������0����Ϊ���ǽ���һ����ӵĲ˵���. ");
	if ( 0 != idCmd ) 
		return E_INVALIDARG; 
	LogTrace("��� Explorer Ҫ������ַ������ͽ����������ṩ�Ļ�������");
	// ��� Explorer Ҫ������ַ������ͽ����������ṩ�Ļ�������. 
	if ( uFlags & GCS_HELPTEXT ) 
	{
		LPCTSTR szText = _T("This is the simple shell extension's help");              
		if ( uFlags & GCS_UNICODE )
		{ 
			// ������Ҫ�� pszName ת��Ϊһ�� Unicode �ַ���, ����ʹ��Unicode�ַ������� API. 
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax ); 
			LogTrace("������Ҫ�� pszName ת��Ϊһ�� Unicode �ַ���, ����ʹ��Unicode�ַ������� API");
		} 
		else
		{ 
			// ʹ�� ANSI �ַ�������API �����ذ����ַ���. 
			lstrcpynA ( pszName, T2CA(szText), cchMax ); 
			LogTrace("ʹ�� ANSI �ַ�������API �����ذ����ַ���");
		} 
		LogTrace("GetCommandString:%s", pszName);
		return S_OK;
	} 
	return E_INVALIDARG; 
}

//���û����������ӵĲ˵���ʱ�÷�����������
HRESULT CCCopyPathEx::InvokeCommand ( LPCMINVOKECOMMANDINFO pCmdInfo )
{
	/*
	HWND m_pWnd = NULL;
	LogTrace("�˵�ID��%d", LOWORD(pCmdInfo->lpVerb));

	m_pWnd = FindWindow(NULL,_T("Spring                                {8192000D-A7B6-433a-8B40-53A3FC3EC52A}")); // ����DataRecv����.
	if(m_pWnd == NULL)
	{
		MessageBox(NULL, TEXT("Unable to find DataRecv."), NULL, MB_OK);
		//return;
	}
	COPYDATASTRUCT cpd = {0}; // ��COPYDATASTRUCT�ṹ��ֵ.
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
				LogTrace("ѡ��һ���ļ����ļ���:%s;", m_szFile);
				//MessageBox(NULL, m_szFile, "Tips", MB_OK);
				cpd.cbData = strlen(m_szFile);
				cpd.lpData = (void*)lpText;
				::SendMessage(m_pWnd, WM_COPYDATA, NULL, (LPARAM)&cpd);
				//m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// ����.
			} 
			else 
			{
				LPTSTR szText = (LPTSTR)lpText;
				for ( int iFile = 0; iFile < m_cFiles; iFile++ ) 
				{
					_tcscat( szText, m_lstFiles[ iFile ].c_str() );
					_tcscat( szText, _T("\r\n") );

				}
				LogTrace("ѡ��%d���ļ����ļ���:\r\n%s;", iFile, szText);
				//MessageBox(NULL, szText, "Tips", MB_OK);
				cpd.cbData = strlen(szText);
				cpd.lpData = (void*)szText;
				//m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// ����.
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
	// ���lpVerb ʵ��ָ��һ���ַ���, ���Դ˴ε��ò��˳�.
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;
	// ������������� �C �����Ψһ�Ϸ�������Ϊ0.
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
