// maindlg.h : interface of the CMainDlg class
//
// Aouther : Huawen Nie
//
// Email : nie173@sohu.com
//
// TimeData : 2003.03.8
//
/////////////////////////////////////////////////////////////////////////////
//include head files
#include "MD5Checksum.h"

//===================================================================
#if !defined(AFX_MAINDLG_H__568C0620_07B4_4D06_BF34_66DDAD886BBA__INCLUDED_)
#define AFX_MAINDLG_H__568C0620_07B4_4D06_BF34_66DDAD886BBA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_EDIT1, EN_CHANGE, OnChangeEdit1)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);

	LRESULT OnChangeEdit1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CString str_ReourceText;
		//get resource text
		HWND  hWndResourceText = GetDlgItem(IDC_EDIT1);
		int int_ReourceText=::GetWindowTextLength(hWndResourceText);
		int_ReourceText++;
		LPTSTR lpStr = str_ReourceText.GetBufferSetLength(int_ReourceText);
        ::GetWindowText(hWndResourceText, lpStr, int_ReourceText);
        str_ReourceText.ReleaseBuffer();
		str_ReourceText =CMD5Checksum::GetMD5((BYTE*)(const char*)str_ReourceText.GetBuffer(0),str_ReourceText.GetLength());
        //set the result text 
		SetDlgItemText(IDC_STATIC_RESUAL,str_ReourceText);
        
		return 0;
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__568C0620_07B4_4D06_BF34_66DDAD886BBA__INCLUDED_)
