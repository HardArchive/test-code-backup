// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"


//WTL��CHotCtrl��MOD_SHIFT��MOD_ALT����ֵ�㷴��
//������������ǵ���������
//��CHotCtrl���ÿ�ݼ�֮ǰ�����߶�ȡ��ݼ�֮�󣬶�Ҫ�������������ݼ���MOD��������һ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	//��ȡ������ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WORD dw1,dw2;
	m_hotkeySystem.GetHotKey(dw1, dw2);
	dw2 = FIX_CHOTKEY_BUG(dw2);
	
	//����������ע��
	//HK_ID = GlobalAddAtom(L"ALT + 2");								//show display setting dialog
	//RegisterHotKey(pBackgndWnd->GetSafeHWND(), HK_ID, MOD_ALT, 0x32);


	//dw1 = 83;
	//dw2 = 3;
    //dw2��ֵ MOD_CONTROL | MOD_ALT | MOD_SHIFT | MOD_WIN
	//dw1��ֵ���� VK_END�鵽 
	//GlobalAddAtom

	//s����ȼ���ͻ
	if (!RegisterHotKey(m_hWnd, 20131202, dw2, dw1))
	{
		DWORD dwError = GetLastError();
		//Hot key is already registered.  1409 
		if (ERROR_HOTKEY_ALREADY_REGISTERED == dwError)
		{
			::MessageBoxA(NULL, "ע���ȼ�ʧ��,ԭ���ȼ��Ѿ���ע�ᣡ����", "System", MB_OK);
		}

		//Hot key is not registered.  1419
		if (ERROR_HOTKEY_NOT_REGISTERED == dwError)
		{
		}
		
	}

	////����Ҳ����
	//long ltem = MAKELONG(dw1,dw2);
	//ATLTRACE("dw1=%d,%d\n",LOWORD(ltem),HIWORD(ltem));
	//RegisterHotKey(m_hWnd, 20131202, HIWORD(ltem), LOWORD(ltem));
	::MessageBoxA(NULL, "ע���ȼ��ɹ�������", "System", MB_OK);
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonUnregistryHotkey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ע���ݼ�
	UnregisterHotKey(m_hWnd, 20131202);
	::MessageBoxA(NULL, "UnregisterHotKey Ok!!!", "UnRegistry", MB_OK);
	return 0;
}


//��ݼ�����
LRESULT CMainDlg::OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//idΪ���Լ������һ��IDֵ��
	//��һ���߳�������ֵ������0x0000 - 0xBFFF��Χ֮�ڣ�
	//��DLL������ֵ������0xC000 - 0xFFFF ��Χ֮�ڣ���ͬһ�����ڸ�ֵ����Ψһ
	if (wParam==20131202)
	{
		CAboutDlg dlg;
		dlg.DoModal();
	}

	return 0;
}
