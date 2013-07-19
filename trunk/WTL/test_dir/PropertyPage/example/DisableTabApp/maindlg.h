// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "atltabctrlwithdisable.h"

#if !defined(AFX_MAINDLG_H__7F1D9850_2E6B_4668_B17C_0EE566A9AFC6__INCLUDED_)
#define AFX_MAINDLG_H__7F1D9850_2E6B_4668_B17C_0EE566A9AFC6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyTabCtrlWithDisable : public CTabCtrlWithDisable
{
public:
    BOOL IsTabEnabled(int nTab)
    {
        return (nTab != 2);
    }
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
			IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);


        CString sBuff;
        //m_ctlTab.Attach(GetDlgItem(IDC_TAB1));
        m_ctlTab.SubclassDlgItem(IDC_TAB1, *this);
        TCITEM item = { 0 };
        item.mask = TCIF_TEXT;   
        sBuff = _T("Tab 1");
        item.pszText = sBuff.GetBuffer(sBuff.GetLength());
        sBuff.ReleaseBuffer(sBuff.GetLength());
        m_ctlTab.InsertItem(0, &item);
        sBuff = _T("Tab 2");
        item.pszText = sBuff.GetBuffer(sBuff.GetLength());
        sBuff.ReleaseBuffer(sBuff.GetLength());
        m_ctlTab.InsertItem(1, &item);
        sBuff = _T("Tab 3");
        item.pszText = sBuff.GetBuffer(sBuff.GetLength());
        sBuff.ReleaseBuffer(sBuff.GetLength());
        m_ctlTab.InsertItem(2, &item);
        sBuff = _T("Tab 4");
        item.pszText = sBuff.GetBuffer(sBuff.GetLength());
        sBuff.ReleaseBuffer(sBuff.GetLength());
        m_ctlTab.InsertItem(2, &item);
        sBuff = _T("Tab 5");
        item.pszText = sBuff.GetBuffer(sBuff.GetLength());
        sBuff.ReleaseBuffer(sBuff.GetLength());
        m_ctlTab.InsertItem(2, &item);
        m_ctlTab.InitTabStatus();

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		CloseDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}

protected:
    CMyTabCtrlWithDisable m_ctlTab;
    CButton m_ctlButton;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__7F1D9850_2E6B_4668_B17C_0EE566A9AFC6__INCLUDED_)
