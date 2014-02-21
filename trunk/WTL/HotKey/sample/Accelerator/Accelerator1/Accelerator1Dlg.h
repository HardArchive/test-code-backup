// Accelerator1Dlg.h : header file
//

#if !defined(AFX_ACCELERATOR1DLG_H__0BCB3F89_7645_4925_8241_669710854157__INCLUDED_)
#define AFX_ACCELERATOR1DLG_H__0BCB3F89_7645_4925_8241_669710854157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAccelerator1Dlg dialog


#define IsCTRLPressed()  ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0)

class CAccelerator1Dlg : public CDialog
{
// Construction
public:
	CAccelerator1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAccelerator1Dlg)
	enum { IDD = IDD_ACCELERATOR1_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccelerator1Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAccelerator1Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCELERATOR1DLG_H__0BCB3F89_7645_4925_8241_669710854157__INCLUDED_)
