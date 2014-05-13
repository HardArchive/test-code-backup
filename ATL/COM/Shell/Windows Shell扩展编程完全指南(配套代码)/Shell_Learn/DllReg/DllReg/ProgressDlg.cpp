// ProgressDlg.cpp : CProgressDlg µÄÊµÏÖ

#include "stdafx.h"
#include "ProgressDlg.h"


// CProgressDlg

LRESULT CProgressDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LVCOLUMN rCol = { LVCF_FMT | LVCF_TEXT, LVCFMT_LEFT, 0, _T("Filename") };

	// List control initialization.

	m_hList = GetDlgItem ( IDC_LIST );

	ListView_InsertColumn ( m_hList, 0, &rCol );

	rCol.mask |= LVCF_SUBITEM;
	rCol.pszText = _T("Result");
	rCol.iSubItem = 1;

	ListView_InsertColumn ( m_hList, 1, &rCol );

	ListView_SetColumnWidth ( m_hList, 0, LVSCW_AUTOSIZE_USEHEADER );
	ListView_SetColumnWidth ( m_hList, 1, LVSCW_AUTOSIZE_USEHEADER );

	// Show the window.

	CenterWindow ( m_pCmdInfo->hwnd );
	ShowWindow ( SW_SHOW );

	// Process all the files in the string list passed in to our constructor.

	DoWork();

	// Enable the Close button, and disable the Stop button.

	::EnableWindow ( GetDlgItem ( IDCANCEL ), TRUE );
	::EnableWindow ( GetDlgItem ( IDC_STOP ), FALSE );

	return 1;  // Let the system set the focus
}


static std::basic_string<TCHAR> strMessage; // temp work area

void CProgressDlg::DoWork()
{
	HRESULT (STDAPICALLTYPE* pfn)();
	string_list::const_iterator it, itEnd;
	HINSTANCE hinst;
	TCHAR     szMsg [512];
	LPCSTR    pszFnName;
	HRESULT   hr;
	WORD      wCmd;
	LPVOID    pvMsgBuf;
	LVITEM    rItem;
	int       nIndex = 0;
	MSG       msg;
	bool      bSuccess;

	USES_CONVERSION;

	wCmd = LOWORD ( m_pCmdInfo->lpVerb );

	// We only support 2 commands, so check the value passed in lpVerb.

	if ( 0 != wCmd  &&  1 != wCmd )
	{
		ATLASSERT(0);
		return;
	}

	// Determine which function we'll be calling.  Note that these strings are
	// not enclosed in the _T macro, since GetProcAddress() only takes an
	// ANSI string for the function name.

	pszFnName = wCmd ? "DllUnregisterServer" : "DllRegisterServer";

	for ( it = m_pFileList->begin(), itEnd = m_pFileList->end();
		!m_bStopSign && it != itEnd;
		it++ )
	{
		bSuccess = false;
		pvMsgBuf = NULL;
		hinst    = NULL;
		*szMsg   = '\0';

		// We will print a status message into szMsg, which will eventually
		// be stored in the LPARAM of a listview control item.

		//__try
		{
			// Try to load the next file.

			hinst = LoadLibrary ( it->c_str() );

			// If it failed, construct a friendly error message.

			if ( NULL == hinst )
			{
				DWORD dwLastErr = GetLastError();

				FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, dwLastErr, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &pvMsgBuf, 0, NULL );

				wsprintf ( szMsg, _T("LoadLibrary failed on this module.\n\nError 0x%08lX (%s)"),
					dwLastErr, 
					pvMsgBuf ? pvMsgBuf : _T("No description available") );

				//continue;
				goto LOOP;
			}

			// Get the address of the register/unregister function.

			(FARPROC&) pfn = GetProcAddress ( hinst, pszFnName );

			// If it wasn't found, construct an error message.

			if ( NULL == pfn )
			{
				wsprintf ( szMsg, _T("%s not found in this module."),
					A2CT(pszFnName) );

				//continue;
				goto LOOP;
			}

			// Call the function!

			hr = pfn();

			// Construct a message listing the result of the function call.

			if ( SUCCEEDED(hr) )
			{
				bSuccess = true;

				wsprintf ( szMsg, _T("%s succeeded on %s"), A2CT(pszFnName),
					(LPCTSTR) it->c_str() );
			}
			else
			{
				LPVOID pvMsgBuf;

				FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, hr, 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &pvMsgBuf, 0, NULL );

				wsprintf ( szMsg, _T("%s failed on %s\n\nError 0x%08lX: %s"),
					A2CT(pszFnName),
					(LPCTSTR) it->c_str(), 
					(DWORD) hr,
					NULL != pvMsgBuf ? pvMsgBuf : _T("(No description available)") );
			}
		}   // end __try
		//__finally
		{
LOOP:
			// Fill in the LVITEM struct.  The item text will be the filename,
			// and the LPARAM will point to a copy of the szMsg status message.

			strMessage = szMsg;

			m_lsStatusMessages.push_back ( strMessage );

			rItem.mask     = LVIF_PARAM | LVIF_TEXT;
			rItem.iItem    = nIndex;
			rItem.iSubItem = 0;
			rItem.pszText  = const_cast<LPTSTR>( it->c_str() );
			rItem.lParam   = (LPARAM) m_lsStatusMessages.back().c_str();

			ListView_InsertItem ( m_hList, &rItem );

			// Set the text in column 2 to "succeeded" or "failed" depending 
			// on the outcome of the function call.

			ListView_SetItemText ( m_hList, nIndex++, 1, 
				bSuccess ? _T("Succeeded") : _T("Failed") );

			if ( NULL != hinst )
				FreeLibrary ( hinst );

			if ( NULL != pvMsgBuf )
				LocalFree ( pvMsgBuf );
		}

		// Empty our message queue - this is much easier than doing it "the
		// real way" with a worker thread. :)   This is how we detect a click
		// on the Stop button.

		while ( PeekMessage ( &msg, m_hWnd, 0, 0, PM_NOREMOVE ))
		{
			GetMessage ( &msg, m_hWnd, 0, 0 );
			TranslateMessage ( &msg );
			DispatchMessage ( &msg );
		}

		// Resize the list columns.

		ListView_SetColumnWidth ( m_hList, 0, LVSCW_AUTOSIZE_USEHEADER );
		ListView_SetColumnWidth ( m_hList, 1, LVSCW_AUTOSIZE_USEHEADER );
	}   // end for
}


LRESULT CProgressDlg::OnItemchangedList ( int idCtrl, LPNMHDR pnmh, BOOL& bHandled )
{
	NMLISTVIEW* pNMLV = (NMLISTVIEW*) pnmh;
	LVITEM      rItem = { 0 };

	// This handler is called when something about a list item changes.
	// We first check if there is a selection.  If not, ignore the message.

	if ( -1 == pNMLV->iItem )
	{
		bHandled = false;
		return 0;
	}

	// If the item changing is becoming selected, display its associated
	// status message in the dialog.

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		rItem.mask  = LVIF_PARAM;
		rItem.iItem = pNMLV->iItem;

		ListView_GetItem ( m_hList, &rItem );

		SetDlgItemText ( IDC_STATUS, (LPCTSTR) rItem.lParam );
	}

	return 0;
}
