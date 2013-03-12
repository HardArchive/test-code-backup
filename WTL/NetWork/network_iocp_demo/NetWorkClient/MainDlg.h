// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "resource.h"
#include "AboutDlg.h"
#pragma once
#include "Public_define.h"
#include <Socket.h>

#define WM_SHOW_MESSAGE WM_USER + 102		//自定义显示信息消息

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
		COMMAND_HANDLER(IDC_BUTTON_CLEAR, BN_CLICKED, OnBnClickedButtonClear)
		COMMAND_HANDLER(IDC_BUTTON_SEND, BN_CLICKED, OnBnClickedButtonSend)
		COMMAND_HANDLER(IDC_BUTTON_CONNECT, BN_CLICKED, OnBnClickedButtonConnect)
		COMMAND_HANDLER(IDC_BUTTON_CLOSE, BN_CLICKED, OnBnClickedButtonClose)
		MESSAGE_HANDLER(WM_SHOW_MESSAGE, OnShowMessage)
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
		char szIP[32] = {0};
		QNA::GetLocalHostNameAndIP(szIP);

		SetDlgItemText(IDC_IPADDRESS, szIP);
		SetDlgItemText(IDC_EDIT_PORT, "1125");
		ZeroMemory(g_szRecvbuf, 512);

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

	//接收消息线程
	static DWORD WINAPI ThreadRecv(LPVOID lpParam)
	{
		int iRet = 0;
		CMainDlg* pDlg = (CMainDlg*)lpParam;
		DWORD          dwTotal = 0;
		timeval        timeout = {0};//超时
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		FD_SET  readSet = {0};//存放Socket的集合
		//_asm int 3;
		while(TRUE)
		{		
			FD_ZERO(&readSet);
			FD_SET(pDlg->m_clsSocket.GetSocket(), &readSet); //把套接字放进集合内
			dwTotal = select(0, &readSet, NULL, NULL, &timeout);

			if (SOCKET_ERROR != dwTotal)
			{
				if (0 == dwTotal)
				{
					TRACE("接收等待\n");
					continue;
				}
				ZeroMemory(g_szRecvbuf, 512);
				iRet = pDlg->m_clsSocket.Recv(pDlg->m_clsSocket.GetSocket(), (PBYTE)g_szRecvbuf, 512);	//接收数据
				if (iRet)
				{
					QNA::TRACE(_T("收到消息：%s;长度:%d\r\n"), g_szRecvbuf, iRet);
					SendMessage(pDlg->m_hWnd, WM_SHOW_MESSAGE, iRet, 0);
				}
			}
			Sleep(1);
		}
		return 0;
	}

	//增加显示消息
	void AddShowMessage(MSGTAG& stuMsgTag)
	{
		QNA::TRACE(_T("增加显示消息函数调用成功！！！\r\n"));
		::SetDlgItemText(m_hWnd, IDC_RICHEDIT2_SHOW, stuMsgTag.szContent);
	}

	LRESULT OnShowMessage( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/ )
	{
		MSGTAG stuMsgTag = {0};
		QNA::TRACE(_T("消息响应函数调用成功！！！\r\n"));
		memcpy(stuMsgTag.szContent, g_szRecvbuf, (size_t)lParam);
		AddShowMessage(stuMsgTag);
		return 0;
	}

	LRESULT OnBnClickedButtonClear(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		::SetDlgItemText(m_hWnd, IDC_RICHEDIT2_SHOW, "");		

		return 0;
	}

	LRESULT OnBnClickedButtonSend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		int iRet = 0;
		char szSend[1024] = {0};
		iRet = ::GetDlgItemText(m_hWnd, IDC_RICHEDIT2_INPUT, szSend, 1024);
		if (!iRet)
		{
			return 0;
		}

		m_clsSocket.Send(m_clsSocket.GetSocket(), (BYTE*)szSend, iRet);
		::SetDlgItemText(m_hWnd, IDC_RICHEDIT2_INPUT, "");	
		

		return 0;
	}

	LRESULT OnBnClickedButtonConnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		int iRet = 0;
		DWORD dwPort = 0;
		char szIP[32] = {0};

		::GetDlgItemText(this->m_hWnd, IDC_EDIT_PORT, szIP, 32);
		dwPort = atoi(szIP);
		ZeroMemory(szIP, 32);
		iRet = ::GetDlgItemText(this->m_hWnd, IDC_IPADDRESS, szIP, 32);
		if (!(iRet && dwPort))
		{
			::MessageBox(NULL, "IP或端口为空，请填入IP和端口！！！", "MSG提示", MB_OK);
			return 0;
		}

		iRet = m_clsSocket.Connect(szIP, dwPort);
		if (1 != iRet)
		{
			QNA::TRACE(_T("连接出错值:%d(1 成功 -1 调用出错 -2 Socket 创建失败 -3 设置发送超时失败 -4 设置接收超时失败 -5 连接超时 -6 连接出错)\r\n"), iRet);
			::MessageBox(NULL, "连接收错！！！", "MSG提示", MB_OK);

			return -1;
		}

		CreateThread(NULL,0,ThreadRecv,this,0,NULL);//创建线程循环接收消息
		return 0;
	}

	LRESULT OnBnClickedButtonClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: 在此添加控件通知处理程序代码
		m_clsSocket.Close();
		return 0;
	}


public:
	QNA::CSocket m_clsSocket;
	//CRichEditCtrl m_RichEditCtrlShow;   //显示消息控件
	//CRichEditCtrl m_RichEditCtrlSend;   //发送消息控件
	
};
