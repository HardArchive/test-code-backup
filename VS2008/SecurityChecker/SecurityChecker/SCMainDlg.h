// MainDlg.h : interface of the CSCMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "ListViewCtrlEx.h"
#include "bkui/wtlhelper/whwindow.h"
#include "bkui/bkres/bkresutil.h"

//#include "ClassifiedCheckUIHandler.h"
#pragma once

class CSCMainDlg : public CBkDialogImpl<CSCMainDlg>
	,public CWHRoundRectFrameHelper<CSCMainDlg>
{
public:
	CSCMainDlg();
	~CSCMainDlg();
//public:
//	CSCMainDlg() : CBkDialogImpl<CSCMainDlg>(IDR_BK_MAIN_DIALOG)
//	{
//	}

public:

	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MAX, OnMaxWindow)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnMinWindow)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(CSCMainDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)		
		CHAIN_MSG_MAP(CBkDialogImpl<CSCMainDlg>)
		CHAIN_MSG_MAP(CWHRoundRectFrameHelper<CSCMainDlg>)
		//CHAIN_MSG_MAP_MEMBER((*m_pobjCCHandler))
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SYSCOMMAND(OnSysCommand)
	END_MSG_MAP()

	//BEGIN_MSG_MAP_EX_DECLARE(CSCMainDlg)

public:
	//CListViewCtrlEx  m_wndListView;
	//CClassifiedCheckUIHandler* m_pobjCCHandler;

public:
	void OnBkBtnClose()
	{
		EndDialog(IDCANCEL);
	}
	//void OnMainMenu()
	//{
	//	::MessageBox(this->m_hWnd, _T("Main Menu!"), _T("Menu!"), MB_OK);
	//}

	//void OnTest()
	//{
	//	::MessageBox(NULL, L"测试按钮成功", L"提示", MB_OK);
	//	return;
	//}
	//void OnMyTest()
	//{
	//	::MessageBox(NULL, L"测试按钮成功", L"提示", MB_OK);
	//	return;
	//}
	LRESULT OnMaxWindow();
	LRESULT OnMinWindow();
	void	OnSysCommand(UINT nID, CPoint point);
	LRESULT OnInitDialog(HWND, LPARAM);
	//LRESULT OnTest();
};