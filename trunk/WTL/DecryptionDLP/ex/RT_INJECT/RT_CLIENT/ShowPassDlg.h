//Ö÷½çÃæ
// ShowPassDlg.h : header file

#if !defined(AFX_SHOWPASSDLG_H__E8679DF7_F8F5_4642_B6D2_F7CF48F0EFBB__INCLUDED_)
#define AFX_SHOWPASSDLG_H__E8679DF7_F8F5_4642_B6D2_F7CF48F0EFBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShowPassDlg dialog
#include "gstruct.h"

class CShowPassDlg : public CDialog
{
// Construction
public:
	
	CShowPassDlg(CWnd* pParent = NULL);	// standard constructor
	~CShowPassDlg();

// Dialog Data
	//{{AFX_DATA(CShowPassDlg)
	enum { IDD = IDD_SHOWPASS_DIALOG };
	UINT	m_x;
	UINT	m_y;
	UINT	m_h;
	UINT	m_w;
	CString	m_szDllName;
	CString	m_szBtnName;
	UINT	m_uBtnID;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowPassDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL GetAppName(HWND hWnd, LPSTR szAppName, int n);
	void EndFind();
	void StartFind();
	BOOL DoInit();
	void DoClose();
	
	HICON m_hIcon;
	HICON m_hFindIcon;
	HCURSOR m_hDefCur;
	HBITMAP m_hOldBmp;
	ICONINFO m_ii;
	HCURSOR m_hFindCur;
	BOOL m_bFind;
	HWND m_hPreWnd;
	HWND m_hwndOther;

	

	// Generated message map functions
	//{{AFX_MSG(CShowPassDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonCreatebtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWPASSDLG_H__E8679DF7_F8F5_4642_B6D2_F7CF48F0EFBB__INCLUDED_)
