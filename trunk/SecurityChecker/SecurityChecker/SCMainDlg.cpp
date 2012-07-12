// MainDlg.cpp : implementation of the CSCMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

//#include "aboutdlg.h"
#include "SCMainDlg.h"

#include "bkui/bkwin/bkatldefine.h"

CSCMainDlg::CSCMainDlg()
: CBkDialogImpl<CSCMainDlg>(IDR_BK_MAIN_DIALOG)
{
	//m_pobjCCHandler = new CClassifiedCheckUIHandler(this);
}

CSCMainDlg::~CSCMainDlg()
{
	//delete m_pobjCCHandler;
}

LRESULT CSCMainDlg::OnMaxWindow()
{
	if (WS_MAXIMIZE == (GetStyle() & WS_MAXIMIZE))
	{		
		SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION, 0);
	}
	else
	{		
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION, 0);
	}
	return 0;
}

LRESULT CSCMainDlg::OnMinWindow()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return 0;
}

//LRESULT CSCMainDlg::OnTest()
//{
//	::MessageBox(NULL, L"测试按钮成功", L"提示", MB_OK);
//	return 0;
//}

void CSCMainDlg::OnSysCommand(UINT nID, CPoint point)
{
	SetMsgHandled(FALSE);

	switch (nID & 0xFFF0)
	{
	case SC_CLOSE:
		SetMsgHandled(TRUE);
		//OnClose();
		EndDialog(IDOK);
		return;
		break;
	case SC_RESTORE:
		{
			DWORD dwStyle = GetStyle();
			if (WS_MINIMIZE == (dwStyle & WS_MINIMIZE))
				break;

			if (WS_MAXIMIZE == (dwStyle & WS_MAXIMIZE))
			{
				SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_max");
				break;
			}
		}
	case SC_MAXIMIZE:
		SetItemAttribute(IDC_BTN_MAX, "skin", "dlg_btn_normal");
		break;
	default:
		break;
	}
}

LRESULT CSCMainDlg::OnInitDialog(HWND, LPARAM)
{
	// center the dialog on the screen
	CenterWindow();
	//m_wndListView.Create( 
	//	GetViewHWND(), NULL,NULL, 
	//	WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL , 
	//	0, IDC_LIST_CONTROL, NULL);



	return TRUE;
}
