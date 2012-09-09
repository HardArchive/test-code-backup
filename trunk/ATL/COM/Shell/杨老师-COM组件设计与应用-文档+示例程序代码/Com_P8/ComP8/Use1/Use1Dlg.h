// Use1Dlg.h : header file
//

#if !defined(AFX_USE1DLG_H__92474974_FF07_4C5A_B32D_0B0F130A51DE__INCLUDED_)
#define AFX_USE1DLG_H__92474974_FF07_4C5A_B32D_0B0F130A51DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUse1Dlg dialog

class CUse1Dlg : public CDialog
{
// Construction
public:
	CUse1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUse1Dlg)
	enum { IDD = IDD_USE1_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUse1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUse1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USE1DLG_H__92474974_FF07_4C5A_B32D_0B0F130A51DE__INCLUDED_)
