// 213465Dlg.h : header file
//

#if !defined(AFX_213465DLG_H__DF717CE0_81A4_4BCA_A438_1CD2C010885E__INCLUDED_)
#define AFX_213465DLG_H__DF717CE0_81A4_4BCA_A438_1CD2C010885E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy213465Dlg dialog

class CMy213465Dlg : public CDialog
{
// Construction
public:
	CMy213465Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy213465Dlg)
	enum { IDD = IDD_MY213465_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy213465Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy213465Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_213465DLG_H__DF717CE0_81A4_4BCA_A438_1CD2C010885E__INCLUDED_)
