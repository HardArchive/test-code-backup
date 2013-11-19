// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__F5477882_057A_45D4_A0F3_F3FF74F2B578__INCLUDED_)
#define AFX_MAINDLG_H__F5477882_057A_45D4_A0F3_F3FF74F2B578__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DROPFILEHANDLER.H" // Include the relevant file...

class CMainDlg : public CDialogImpl<CMainDlg>,
				 // Add CDropFilesHandler to your inheritance list...
				 public CDropFilesHandler<CMainDlg> 
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		// Send WM_DROPFILES to its handler...
		CHAIN_MSG_MAP(CDropFilesHandler<CMainDlg>)
	END_MSG_MAP()
	

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		m_ListBox.Attach(GetDlgItem(IDC_LISTFILES));

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
    
	/////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// CDropFilesHandler requirements' implementation.

	// In this particular example, we'll put all dropped files in a listbox.
	CListBox m_ListBox;

	BOOL IsReadyForDrop(void) 	{ m_ListBox.ResetContent(); return TRUE; }
	BOOL HandleDroppedFile(LPCTSTR szBuff)
	{
		ATLTRACE("%s\n", szBuff);
		// In this particular example, we'll do the VERY LEAST possible.
		m_ListBox.AddString(szBuff);

	    // Return TRUE unless you're done handling files (e.g., if you want 
		// to handle only the first relevant file, and you have already found it).
		return TRUE;
	}
	void EndDropFiles(void)
	{
		// Sometimes, if your file handling is not trivial,  you might want to add all
		// file names to some container (std::list<CString> comes to mind), and do the 
		// handling afterwards, in a worker thread. 
		// If so, use this function to create your worker thread.
#ifndef _ATL_MIN_CRT
		CWindow wnd;
		wnd.Attach(GetDlgItem(IDC_COUNT));
		char fmt[] = "Count of files in the last drop: %d";
		char buff[sizeof(fmt) + 30];
		sprintf(buff, fmt, m_ListBox.GetCount());
		wnd.SetWindowText(buff);
#endif
	}

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__F5477882_057A_45D4_A0F3_F3FF74F2B578__INCLUDED_)
