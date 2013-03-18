#include "StdAfx.h"
#include "MainDlg.h"


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
	SetDlgItemInt(IDC_EDIT_PORT, 5000, 1);
	m_ListViewCtrlClient.Attach(GetDlgItem(IDC_LIST_CLIENT));
	m_ListViewCtrlClient.SetExtendedListViewStyle(LVS_REPORT | LVS_EX_FULLROWSELECT);	
	m_ListViewCtrlClient.InsertColumn (0, _T("ID"), LVCFMT_LEFT, 70, 0);
	m_ListViewCtrlClient.InsertColumn (1, _T("IP"), LVCFMT_LEFT, 90, 1);
	m_ListViewCtrlClient.InsertColumn (2, _T("Port"), LVCFMT_LEFT, 50, 1);
	m_ListViewCtrlClient.InsertColumn (3, _T("Status"), LVCFMT_LEFT, 70, 1);

	m_ListViewCtrlMessage.Attach(GetDlgItem(IDC_LIST_MESSAGE));
	m_ListViewCtrlMessage.SetExtendedListViewStyle(LVS_REPORT | LVS_EX_FULLROWSELECT);
	m_ListViewCtrlMessage.ModifyStyle(0, LVS_SINGLESEL);       //整行选择样式  只设置了此样式才能获取内容
	m_ListViewCtrlMessage.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 70, 0);
	m_ListViewCtrlMessage.InsertColumn (1, _T("Status"), LVCFMT_LEFT, 50, 1);
	m_ListViewCtrlMessage.InsertColumn (2, _T("length"), LVCFMT_LEFT, 70, 1);
	m_ListViewCtrlMessage.InsertColumn (3, _T("contents"), LVCFMT_LEFT, 285, 1);

	////设置编辑框自动换行
	//HWND hWndEdit = GetDlgItem(IDC_EDIT_SEND);
	//DWORD dwStyle = ::GetWindowLong(hWndEdit, GWL_STYLE);
	//DWORD dwNewStyle = ES_AUTOHSCROLL;
	//dwNewStyle = ~dwNewStyle;
	//dwStyle = dwStyle & dwNewStyle; 
	// dwNewStyle = dwStyle | ES_AUTOVSCROLL | ES_MULTILINE;
	//if(dwStyle != dwNewStyle)
	//{
	//	::SetWindowLong(hWndEdit, GWL_STYLE, dwNewStyle);
	//}
	//照理是这样弄的，但不知为何不起作用


	SetMainWnd(this->m_hWnd);
	SetAppState(ST_STOPED);



	//还需要添加的功能 1 选择m_ListViewCtrlClient 然后就点发送时就自动发送至选择的ID处
	//2  选择m_ListViewCtrlMessage 则可自动将其选择的内容显示到 编辑框中
	//test
	m_bIsEditShow = true;
	m_ListViewCtrlClient.InsertItem (0, _T("00000001"));
	m_ListViewCtrlClient.SetItemText (0, 1, _T("192.168.30.107"));
	m_ListViewCtrlClient.SetItemText (0, 2, _T("5000"));
	m_ListViewCtrlClient.SetItemText (0, 3, _T("connect"));
	m_ListViewCtrlClient.SetItemText (0, 3, _T("disconnect"));



	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
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

LRESULT CMainDlg::OnListViewCtrlMessageSelected(LPNMHDR lpNMHDR)
{
	if (m_bIsEditShow)
	{		
		TCHAR tszContent[2048] = {0};
		int iNum = 0;			//LVS_SINGLESEL
		iNum = m_ListViewCtrlMessage.GetSelectedIndex(); 
		m_ListViewCtrlMessage.GetItemText(iNum, 3, tszContent, 2048);
		SetDlgItemText(IDC_EDIT_SEND, tszContent);		
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwPort = GetServerPort();
	if (m_clsIOCPServerHelper.Start(_T("127.0.0.1"), dwPort))
	{
		SetAppState(ST_STARTED);
	}
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_clsIOCPServerHelper.Stop())
	{
		SetAppState(ST_STOPED);
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	
	return 0;
}

DWORD CMainDlg::GetServerPort()
{
	return (DWORD)GetDlgItemInt(IDC_EDIT_PORT);
}

void CMainDlg::SetAppState(EnAppState state)
{
	::EnableWindow(GetDlgItem(IDC_BUTTON_START), ST_STOPED == state);
	::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), ST_STARTED == state);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STOP), ST_STARTED == state);
	
}

LRESULT CMainDlg::OnUserInfoMsg(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	info_msg* msg = (info_msg*)wParam;
	ShowLogMsg(msg);

	info_msg::Destruct(msg);

	return 0;
}


void CMainDlg::ShowLogMsg(info_msg* msg)
{
	static int iRow = 0;
	TCHAR tszID[16] = {0};
	_stprintf(tszID, _T("%d"), msg->connID);
	m_ListViewCtrlMessage.InsertItem (iRow, tszID);
	m_ListViewCtrlMessage.SetItemText (iRow, 1, msg->evt);
	memset(tszID, 0, 16*sizeof(TCHAR));
	_stprintf(tszID, _T("%d"), msg->contentLength);
	m_ListViewCtrlMessage.SetItemText (iRow, 2, tszID);
	m_ListViewCtrlMessage.SetItemText (iRow++, 3, msg->content);
}