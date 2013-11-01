// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	typedef struct _MsgData
	{
		CString	sDefaultName;
		CString	sMsg;
		CString	sFunc;
	} MsgData;

	CString		m_sCopy1;
	CString		m_sCopy2;
	CString		m_sCopy3;
	CString		m_sCopy4;

	CAtlArray<MsgData> m_aMsg;

	CComboBox m_ctlList;

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_COPY1, BN_CLICKED, OnBnClickedCopy1)
		COMMAND_HANDLER(IDC_COPY2, BN_CLICKED, OnBnClickedCopy2)
		COMMAND_HANDLER(IDC_COPY3, BN_CLICKED, OnBnClickedCopy3)
		COMMAND_HANDLER(IDC_COPY4, BN_CLICKED, OnBnClickedCopy4)
		COMMAND_HANDLER(IDC_MSGLIST, CBN_SELCHANGE, OnCbnSelchangeMsglist)
	END_MSG_MAP()

//	Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	void CloseDialog(int nVal);
	LRESULT OnBnClickedCopy1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCopy2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCopy3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCopy4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCbnSelchangeMsglist(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
