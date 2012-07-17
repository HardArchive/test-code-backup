// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
// 模版制作：天下我最笨 (QQ:272922032)
#pragma once

class CMainDlg  : public CBkDialogImpl<CMainDlg>
				, public CWHRoundRectFrameHelper<CMainDlg>
{
public:
	CMainDlg():CBkDialogImpl<CMainDlg>(IDR_BK_MAIN_DIALOG)
	{
	}

	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDCANCEL, OnBtnClose)
		BK_NOTIFY_ID_COMMAND(2003, OnMinBtnClick)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP(CMainDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
		CHAIN_MSG_MAP(CBkDialogImpl<CMainDlg>)
		CHAIN_MSG_MAP(CWHRoundRectFrameHelper<CMainDlg>)
		MSG_WM_SYSCOMMAND(OnCommand)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

protected:
	LRESULT OnCommand(UINT wParam, CPoint point);
	BOOL OnInitDialog(CWindow wndFocus, LPARAM lInitParam);
	void OnBtnClose();
	void OnMinBtnClick();
};
