// testhookDllDlg.h : header file
//

#if !defined(AFX_TESTHOOKDLLDLG_H__557A8773_64D6_465A_8698_EF2280E852D0__INCLUDED_)
#define AFX_TESTHOOKDLLDLG_H__557A8773_64D6_465A_8698_EF2280E852D0__INCLUDED_

#include<tlhelp32.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTesthookDllDlg dialog

class CTesthookDllDlg : public CDialog
{
// Construction
public:
	void loadCofigINI(CString path);
	INT     ReadIni(CString section, CString key,CString path);
	void    writeIni(CString section,CString key,CString value,CString path);
	bool    INIOperation();
	void    InitEx();
	void    unhookdll1();
	void    unhookFun();

    void EixtProc();
	int KKillByName(HANDLE hProcessSnap, PROCESSENTRY32 pe32, char *szStr);
	bool KIfequalStrAndProcess(char *szStr ,char *szProcess);
	BOOL KTerminateProcessID(DWORD dwID);

	CTesthookDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTesthookDllDlg)
	enum { IDD = IDD_TESTHOOKDLL_DIALOG };
 
	CButton	m_disall;
	CButton	m_openall;
	CButton	m_tab;
	CButton	m_prtsc;
	CButton	m_taskmgr;
	CButton	m_esc;
	CButton	m_delete;
	CButton	m_ctrl_z;
	CButton	m_ctrl_x;
	CButton	m_ctrl_v;
	CButton	m_ctrl_f;
	CButton	m_ctrl_e;
	CButton	m_ctrl_d;
	CButton	m_ctrl_c;
	CButton	m_ctrl_a;
	BOOL	m_win;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesthookDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTesthookDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBNTShieldingALLHOTKEY();
	afx_msg void OnButtonOpenAll();
	afx_msg void OnButtonDisableAll();
	afx_msg void OnCheckCtrlA();
	afx_msg void OnCheckCtrlC();
	afx_msg void OnCheckCtrlD();
	afx_msg void OnCheckCtrlE();
	afx_msg void OnCheckCtrlF();
	afx_msg void OnCheckCtrlV();
	afx_msg void OnCheckCtrlX();
	afx_msg void OnCheckCtrlZ();
	afx_msg void OnCHECKTaskmgr();
	afx_msg void OnCheckTab();
	afx_msg void OnCHECKPrtSc();
	afx_msg void OnCheckDelte();
	afx_msg void OnCheckEsc();
	afx_msg void OnButtonsavaseting();
	afx_msg void OnBUTTONDefault();
	afx_msg void OnButtonAction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTHOOKDLLDLG_H__557A8773_64D6_465A_8698_EF2280E852D0__INCLUDED_)
