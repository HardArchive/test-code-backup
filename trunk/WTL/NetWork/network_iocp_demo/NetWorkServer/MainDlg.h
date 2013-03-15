// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "AboutDlg.h"
//#include "../common/com_module/socket/IocpServer.h"
#include "../global/helper.h"
#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		NOTIFY_HANDLER_EX(IDC_LIST_MESSAGE, NM_CLICK, OnListViewCtrlMessageSelected)
		COMMAND_HANDLER(IDC_BUTTON_START, BN_CLICKED, OnBnClickedButtonStart)
		COMMAND_HANDLER(IDC_BUTTON_SEND, BN_CLICKED, OnBnClickedButtonSend)
		COMMAND_HANDLER(IDC_BUTTON_STOP, BN_CLICKED, OnBnClickedButtonClose)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

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

		m_ListViewCtrlMessage.InsertItem (0, _T("00000001"));
		m_ListViewCtrlMessage.SetItemText (0, 1, _T("send"));
		m_ListViewCtrlMessage.SetItemText (0, 2, _T("1000"));
		m_ListViewCtrlMessage.SetItemText (0, 3, _T("编写 Windows Socket TCP 客户端其实并不困难，ket 通信和业务逻辑混在一起，剪不断理还乱。"));
		


		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

	LRESULT OnListViewCtrlMessageSelected(LPNMHDR lpNMHDR)
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

public:
	void SetAppState(EnAppState state);
private:
	DWORD GetServerPort();
public:
	CListViewCtrl m_ListViewCtrlClient;
	CListViewCtrl m_ListViewCtrlMessage;
	bool m_bIsEditShow;     //标志 当其为true时 点击m_ListViewCtrlMessage控件会将其内容显示在Edit上
	LRESULT OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
