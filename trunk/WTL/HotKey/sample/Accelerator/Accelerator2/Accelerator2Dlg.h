// Accelerator2Dlg.h : header file
//

#if !defined(AFX_ACCELERATOR2DLG_H__C7B402E0_DD03_4D21_A28E_1F74EA4390CB__INCLUDED_)
#define AFX_ACCELERATOR2DLG_H__C7B402E0_DD03_4D21_A28E_1F74EA4390CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAccelerator2Dlg dialog

class CAccelerator2Dlg : public CDialog
{
// Construction
public:
	CAccelerator2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAccelerator2Dlg)
	enum { IDD = IDD_ACCELERATOR2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccelerator2Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HACCEL hAcc;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAccelerator2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCtrlQ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELERATOR2DLG_H__C7B402E0_DD03_4D21_A28E_1F74EA4390CB__INCLUDED_)
