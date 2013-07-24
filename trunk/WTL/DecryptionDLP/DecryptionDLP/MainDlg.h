// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
//#include "DecryptDLP.h"

#pragma once
//#include "PathUtil.h"
#include "DecryptDLP.h"
//const UINT WM_RG_SHOW_PATH = RegisterWindowMessage("RG_SHOW_PATH");

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_SELEC_ECODE_PATH, BN_CLICKED, OnBnClickedButtonSelecEcodePath)
		COMMAND_HANDLER(IDC_BUTTON_SELECT_DCODE_PATH, BN_CLICKED, OnBnClickedButtonSelectDcodePath)
		COMMAND_HANDLER(IDC_BUTTON_START, BN_CLICKED, OnBnClickedButtonStart)
		COMMAND_HANDLER(IDC_BUTTON_STOP, BN_CLICKED, OnBnClickedButtonStop)		
		MESSAGE_HANDLER(WM_COPYDATA, OnCopyData)
		//MESSAGE_HANDLER(WM_RG_SHOW_PATH, OnShowFilePath)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		SetDlgItemText(IDC_EDIT_PROCESS_NAME, _T("calc.exe")/*_T("devenv.exe")*/);
		//DecryptDLP();
		//������Ҫȡ�ý�����
		TCHAR tszProcessName[MAX_PATH] = {0};
		GetDlgItemText(IDC_EDIT_PROCESS_NAME, tszProcessName, MAX_PATH);
		m_clsDecryptDLP.Init(m_hWnd, tszProcessName);

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
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

	LRESULT OnBnClickedButtonSelecEcodePath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		RG::CPathUtil clsPathUtil;
		clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_ECODE_PATH, _T("ѡ��·��2"));

		return 0;
	}

	LRESULT OnBnClickedButtonSelectDcodePath(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		RG::CPathUtil clsPathUtil;
		clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_DCODE_PATH);
		return 0;
	}

	LRESULT OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		TCHAR tszECodePath[MAX_PATH] = {0};
		TCHAR tszDCodePath[MAX_PATH] = {0};
		GetDlgItemText(IDC_EDIT_ECODE_PATH, tszECodePath, MAX_PATH);
		GetDlgItemText(IDC_EDIT_DCODE_PATH, tszDCodePath, MAX_PATH);

		if (!(_tcslen(tszECodePath) && _tcslen(tszDCodePath)))
		{
			::MessageBox(NULL, _T("�������ѡ��ECode��DCode·��!!!"), _T("TIP_RG"), MB_OK);
			return 0;
		}
		m_clsDecryptDLP.Start(tszDCodePath , tszECodePath);

		return 0;
	}

	LRESULT OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		m_clsDecryptDLP.Stop();
		return 0;
	}

	LRESULT OnCopyData(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		PCOPYDATASTRUCT pstuCPD = (PCOPYDATASTRUCT)wParam; // ��COPYDATASTRUCT�ṹ��ֵ.
		SetDlgItemText(IDC_STATIC_SHOW, (PTCHAR)pstuCPD->lpData);
		return 0;
	}

	////�����Զ�����Ϣ
	//LRESULT OnShowFilePath( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	//{
	//	//ATL::CWindow��MessageBox�ĵ���
	//	//MessageBox( "This is MyMessage Handle" ); //�������Ի���
	//	TCHAR tszECodePath[MAX_PATH] = {0};


	//	SetDlgItemText(IDC_STATIC_SHOW, (TCHAR*)wParam);
	//	
	//	
	//	return 0;
	//}


public:
	CDecryptDLP m_clsDecryptDLP;


};
