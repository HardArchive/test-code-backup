// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <sstream>
#include <iomanip>
#include <atltime.h>
#include <Listctrl/ListCtrl.h>
//#include <atlctrl.h>
class CMainDlg : public CBkDialogImpl<CMainDlg>
{
public:
	CMainDlg() : CBkDialogImpl<CMainDlg>(IDR_BK_MAIN_DIALOG)
	{
	}

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MAX, OnMaxWindow)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnMinWindow)
		//BK_NOTIFY_ID_COMMAND(IDC_BTN_TEST, OnTest)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		CHAIN_MSG_MAP(CBkDialogImpl<CMainDlg>)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

public:
	void OnBkBtnClose()
	{
		EndDialog(IDCANCEL);
	}

	LRESULT OnMaxWindow();
	LRESULT OnMinWindow();
	void	OnSysCommand(UINT nID, CPoint point);
	LRESULT OnInitDialog(HWND, LPARAM);
	//LRESULT OnTest();

	CFont m_fntCustomFont1;
	CFont m_fntCustomFont2;
	CEdit m_edit;
	CListCtrl m_wndListCtrl;
};
