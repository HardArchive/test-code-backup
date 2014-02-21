// SysHotKeyDiyDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "MyROEdit.h"
#include "afxwin.h"
#include "inc.h"
#include "ReportCtrl.h"


// CSysHotKeyDiyDlg dialog
class CSysHotKeyDiyDlg : public CDialog
{
// Construction
public:
	CSysHotKeyDiyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYSHOTKEYDIY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//UINT WM_TASKBARCREATED;
	int gIndex;
	BOOL bStart;
	CReportCtrl m_ListShow;
	CMyROEdit m_EditPath;
	CString m_strEditPath;
	CComboBox m_ComboAsc;
	CStatusBarCtrl m_StatBar;
	BOOL CheckInfoFillRight();
	BOOL GetCurHkInfo(HKNODE *pHkNode);
	DWORD StrAsc2Dword(CString in_StrAsc);
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedSethotkey();
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	CButton m_CtrlCheck;
	CButton m_ShiftCheck;
	CButton m_AltCheck;
	CArray <HKNODE, HKNODE> arrHotKey;
	afx_msg void OnBnClickedResethotkey();
	afx_msg void OnDestroy();
	int MyStrToInt(CString in_Str);
	BOOL RegSetHotKey(HKNODE *pHkNode, BOOL bDelete = FALSE);
	BOOL RegSetHotKey(CArray <HKNODE, HKNODE> *pArrHotKey, BOOL bRemoveAllFirst = TRUE);
	BOOL RegReadHotKey(CArray <HKNODE, HKNODE> *pArrHotKey);
	BOOL ParserStrHkToDword(HKNODE *pOut_Node);
	DWORD Sys2Dword(CString in_Str);
	int FindStringInListCtrl(CString in_Str, DWORD dwFindFlag);
	afx_msg void OnBnClickedCheckAutoStart();
	BOOL ReadAutoStartState();
	CButton m_AStartCheck;
	afx_msg void OnBnClickedCheckInitTrayicon();
	BOOL ReadInitTrayIcon();
	afx_msg LRESULT OnTrayCallBackMsg(WPARAM wParam, LPARAM lParam);
	CButton m_InitIconCheck;
	afx_msg void OnMenuShow();
	afx_msg void OnMenuClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_MinSizeIconCheck;
	afx_msg void OnBnClickedCheckMinsizeTrayicon();
	BOOL ReadMinSizeTrayIcon();
	afx_msg void OnNMClickListShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedResetAllkey();
	LRESULT CSysHotKeyDiyDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
	HMODULE m_hModule;
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedListItemUp();
	afx_msg void OnBnClickedListItemDown();
};
