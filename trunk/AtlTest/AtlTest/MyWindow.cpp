#include "StdAfx.h"
#include "MyWindow.h"
#include "AboutDlg.h"



LRESULT CMyWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);    
	return 0;
}

LRESULT CMyWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CMyWindow::OnAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	//     MessageBox ( _T("Sample ATL window"), _T("About MyWindow") );
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	return 0;
}

LRESULT CMyWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HMENU hMenu = LoadMenu(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU));
	SetMenu(hMenu);
	return 0;
}
