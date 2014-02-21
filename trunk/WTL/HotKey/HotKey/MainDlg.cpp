// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"


//WTL的CHotCtrl把MOD_SHIFT和MOD_ALT两个值搞反了
//这个函数把它们调过来才行
//对CHotCtrl设置快捷键之前，或者读取快捷键之后，都要用这个函数将快捷键的MOD参数处理一下
//tianzuo,2008-11-27
WORD FIX_CHOTKEY_BUG(WORD dwMod)
{
	if (dwMod&MOD_SHIFT)
	{
		dwMod &= ~MOD_SHIFT;
		dwMod |= MOD_ALT;
	}
	else if (dwMod&MOD_ALT)
	{
		dwMod &= ~MOD_ALT;
		dwMod |= MOD_SHIFT;
	}
	return dwMod;
}



BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
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


	m_hotkeySystem = GetDlgItem(IDC_HOTKEY_SYSTEM);

	UIAddChildWindowContainer(m_hWnd);

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

LRESULT CMainDlg::OnBnClickedButtonRegistryWindowHotkey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码


	//获取按键键值
	WORD wVirtualKeyCode = 0;
	WORD wModifiers = 0;
	ATLASSERT(::IsWindow(GetDlgItem(IDC_HOTKEY_WINDOW)));
	DWORD dw = (DWORD)::SendMessage(GetDlgItem(IDC_HOTKEY_WINDOW), HKM_GETHOTKEY, 0, 0L);
	wVirtualKeyCode = LOBYTE(LOWORD(dw));
	wModifiers = HIBYTE(LOWORD(dw));
	wModifiers = FIX_CHOTKEY_BUG(wModifiers);

	::MessageBoxA(NULL, "12323423423", "Window", MB_OK);
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonRegistrySystemHotkey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	WORD dw1,dw2;
	m_hotkeySystem.GetHotKey(dw1, dw2);
	dw2 = FIX_CHOTKEY_BUG(dw2);
	
	//还可以这样注册
	//HK_ID = GlobalAddAtom(L"ALT + 2");								//show display setting dialog
	//RegisterHotKey(pBackgndWnd->GetSafeHWND(), HK_ID, MOD_ALT, 0x32);


	//dw1 = 83;
	//dw2 = 3;
    //dw2的值 MOD_CONTROL | MOD_ALT | MOD_SHIFT | MOD_WIN
	//dw1的值可以 VK_END查到 
	//GlobalAddAtom

	//s检查热键冲突
	if (!RegisterHotKey(m_hWnd, 20131202, dw2, dw1))
	{
		DWORD dwError = GetLastError();
		//Hot key is already registered.  1409 
		if (ERROR_HOTKEY_ALREADY_REGISTERED == dwError)
		{
			::MessageBoxA(NULL, "注册热键失败,原因：热键已经被注册！！！", "System", MB_OK);
		}

		//Hot key is not registered.  1419
		if (ERROR_HOTKEY_NOT_REGISTERED == dwError)
		{
		}
		
	}

	////这样也可以
	//long ltem = MAKELONG(dw1,dw2);
	//ATLTRACE("dw1=%d,%d\n",LOWORD(ltem),HIWORD(ltem));
	//RegisterHotKey(m_hWnd, 20131202, HIWORD(ltem), LOWORD(ltem));
	::MessageBoxA(NULL, "注册热键成功！！！", "System", MB_OK);
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonUnregistryHotkey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	//反注册快捷键
	UnregisterHotKey(m_hWnd, 20131202);
	::MessageBoxA(NULL, "UnregisterHotKey Ok!!!", "UnRegistry", MB_OK);
	return 0;
}


//快捷键处理
LRESULT CMainDlg::OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//id为你自己定义的一个ID值，
	//对一个线程来讲其值必需在0x0000 - 0xBFFF范围之内，
	//对DLL来讲其值必需在0xC000 - 0xFFFF 范围之内，在同一进程内该值必须唯一
	if (wParam==20131202)
	{
		CAboutDlg dlg;
		dlg.DoModal();
	}

	return 0;
}
