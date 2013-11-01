// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_ctlList = GetDlgItem(IDC_MSGLIST);
	::SetWindowText(GetDlgItem(IDC_CLASSNAME), "yourclass");

	std::ifstream fin("crack.dat");
	int n = 0;
	CString str;
	CString	sFunc;
	CString	sMsg;
	char sz[255];
	fin.getline(sz, 255);
	str=sz;
	while (str.GetLength()>0)
	{
		str=sz;
		if (str.Find("On")==0)
		{
			MsgData data;
			data.sDefaultName = str;
			fin.getline(sz, 255);
			data.sMsg = sz;
			fin.getline(sz, 255);
			data.sFunc = sz;
			m_aMsg.Add(data);
			m_ctlList.AddString(data.sMsg.Left(data.sMsg.Find('(')));
		}
		fin.getline(sz, 255);
		str=sz;
	}

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedCopy1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (OpenClipboard() == FALSE || EmptyClipboard() == FALSE)
		return 0;

	HGLOBAL	hMem = ::GlobalAlloc
		(
		GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT,
		m_sCopy1.GetLength()*(sizeof(TCHAR)+1)
		);

	if( hMem == NULL )
		return 0;

	LPBYTE lpx = (LPBYTE)::GlobalLock(hMem);

	memcpy(lpx, (LPCTSTR)m_sCopy1, m_sCopy1.GetLength()*sizeof(TCHAR));

	::GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

LRESULT CMainDlg::OnBnClickedCopy2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (OpenClipboard() == FALSE || EmptyClipboard() == FALSE)
		return 0;

	HGLOBAL	hMem = ::GlobalAlloc
		(
		GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT,
		m_sCopy2.GetLength()*(sizeof(TCHAR)+1)
		);

	if( hMem == NULL )
		return 0;

	LPBYTE lpx = (LPBYTE)::GlobalLock(hMem);

	memcpy(lpx, (LPCTSTR)m_sCopy2, m_sCopy2.GetLength()*sizeof(TCHAR));

	::GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

LRESULT CMainDlg::OnBnClickedCopy3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (OpenClipboard() == FALSE || EmptyClipboard() == FALSE)
		return 0;

	HGLOBAL	hMem = ::GlobalAlloc
		(
		GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT,
		m_sCopy3.GetLength()*(sizeof(TCHAR)+1)
		);

	if( hMem == NULL )
		return 0;

	LPBYTE lpx = (LPBYTE)::GlobalLock(hMem);

	memcpy(lpx, (LPCTSTR)m_sCopy3, m_sCopy3.GetLength()*sizeof(TCHAR));

	::GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

LRESULT CMainDlg::OnBnClickedCopy4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (OpenClipboard() == FALSE || EmptyClipboard() == FALSE)
		return 0;

	HGLOBAL	hMem = ::GlobalAlloc
		(
		GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT,
		m_sCopy4.GetLength()*(sizeof(TCHAR)+1)
		);

	if( hMem == NULL )
		return 0;

	LPBYTE lpx = (LPBYTE)::GlobalLock(hMem);

	memcpy(lpx, (LPCTSTR)m_sCopy4, m_sCopy4.GetLength()*sizeof(TCHAR));

	::GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

LRESULT CMainDlg::OnCbnSelchangeMsglist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int n = m_ctlList.GetCurSel();
	if (n==-1)
		return 0;
	char szClass[255];
	::GetWindowText(GetDlgItem(IDC_CLASSNAME), szClass, 255);
	CString		sDefaultName;
	CString		sMsg;
	CString		sFunc;
	MsgData		data;
	data			=	m_aMsg.GetAt(n);
	
	sDefaultName	=	data.sDefaultName;
	sMsg			=	data.sMsg;
	sFunc			=	data.sFunc;
	
	m_sCopy1	=	sMsg;
	m_sCopy2	=	sFunc;
	m_sCopy3	=	sFunc;
	m_sCopy1.Replace("func", sDefaultName);
	m_sCopy1 += "\r\n";
	if (sFunc.Find("(LRESULT)")==0)
	{
		CString str = "LRESULT ";
		str += szClass;
		str += "::";
		m_sCopy2.Replace("(LRESULT)", "LRESULT ");
		m_sCopy3.Replace("(LRESULT)", str);
		m_sCopy3.Replace(";", "\r\n{\r\n\treturn 0;\r\n}\r\n");
		m_sCopy4	=	m_sCopy2;
		m_sCopy4.Replace(";", "\r\n\t{\r\n\t\treturn 0;\r\n\t}\r\n");
		m_sCopy2 +="\r\n";
	}
	else if (sFunc.Find("(BOOL)")==0)
	{
		CString str = "LRESULT ";
		str += szClass;
		str += "::";
		m_sCopy2.Replace("(BOOL)", "BOOL ");
		m_sCopy3.Replace("(BOOL)", str);
		m_sCopy3.Replace(";", "\r\n{\r\n\treturn FALSE;\r\n}\r\n");
		m_sCopy4	=	m_sCopy2;
		m_sCopy4.Replace(";", "\r\n\t{\r\n\t\treturn FALSE;\r\n\t}\r\n");
		m_sCopy2 +="\r\n";
	}
	else if (sFunc.Find("(void)")==0)
	{
		CString str = "void ";
		str += szClass;
		str += "::";
		m_sCopy2 = "void " + m_sCopy2;
		m_sCopy3 = str + m_sCopy3;
		m_sCopy3.Replace(";", "\r\n{\r\n\treturn;\r\n}\r\n");
		m_sCopy4	=	m_sCopy2;
		m_sCopy4.Replace(";", "\r\n\t{\r\n\t\treturn;\r\n\t}\r\n");
		m_sCopy2 +="\r\n";
	}
	else
	{
		CString str = "void ";
		str += szClass;
		str += "::";
		m_sCopy2 = "void " + m_sCopy2;
		m_sCopy3 = str + m_sCopy3;
		m_sCopy3.Replace(";", "\r\n{\r\n\treturn;\r\n}\r\n");
		m_sCopy4	=	m_sCopy2;
		m_sCopy4.Replace(";", "\r\n\t{\r\n\t\treturn;\r\n\t}\r\n");
		m_sCopy2 +="\r\n";
	}
	m_sCopy2.Replace("func", sDefaultName);
	m_sCopy3.Replace("func", sDefaultName);
	m_sCopy4.Replace("func", sDefaultName);
	::SetWindowText
		(
		GetDlgItem(IDC_EDITCODE),
		m_sCopy1+"\r\n"+m_sCopy2+"\r\n"+m_sCopy3+"\r\n"+m_sCopy4
		);

	return 0;
}

