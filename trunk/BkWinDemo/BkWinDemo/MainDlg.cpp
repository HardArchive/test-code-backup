// 模版制作：天下我最笨 (QQ:272922032)
#include "stdafx.h"
#include "resource.h"
#include "wtlhelper/whwindow.h"
#include "MainDlg.h"

//最小化按钮
void CMainDlg::OnMinBtnClick()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

//关闭按钮
void CMainDlg::OnBtnClose()
{
	EndDialog(IDCANCEL);
}

//系统命令
LRESULT CMainDlg::OnCommand(UINT wParam, CPoint point)
{
	switch(wParam)
	{
	case SC_CLOSE :
		EndDialog(IDCANCEL);
		break;
	}				
	DefWindowProc();
	return 0;
}

//对话框初始化
BOOL CMainDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	SetIcon(::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_BIG)));
	SetIcon(::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_SMALL)), FALSE);

	return TRUE;
}
