#include "StdAfx.h"
#include "MainDlg.h"


LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	SetAppState(ST_STARTED);
	DWORD dwPort = GetServerPort();

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	SetAppState(ST_STOPED);

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