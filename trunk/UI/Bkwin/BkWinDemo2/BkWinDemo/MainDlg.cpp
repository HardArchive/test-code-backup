// ģ��������������� (QQ:272922032)
#include "stdafx.h"
#include "resource.h"
#include "wtlhelper/whwindow.h"
#include "MainDlg.h"

//��С����ť
void CMainDlg::OnMinBtnClick()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

//�رհ�ť
void CMainDlg::OnBtnClose()
{
	EndDialog(IDCANCEL);
}

//ϵͳ����
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

//�Ի����ʼ��
BOOL CMainDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	SetIcon(::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_BIG)));
	SetIcon(::LoadIcon((HMODULE)&__ImageBase, MAKEINTRESOURCE(IDI_SMALL)), FALSE);

	return TRUE;
}
