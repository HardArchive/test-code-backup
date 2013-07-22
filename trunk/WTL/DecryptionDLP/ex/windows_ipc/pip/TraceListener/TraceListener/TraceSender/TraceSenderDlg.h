// TraceSenderDlg.h : header file
//

#if !defined(AFX_TRACESENDERDLG_H__ABC99A8F_1DB8_43CA_9835_2AAFF135461F__INCLUDED_)
#define AFX_TRACESENDERDLG_H__ABC99A8F_1DB8_43CA_9835_2AAFF135461F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Tracer.h"


/////////////////////////////////////////////////////////////////////////////
// CTraceSenderDlg dialog

class CTraceSenderDlg : public CDialog
{
// Construction
public:
	CTraceSenderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTraceSenderDlg)
	enum { IDD = IDD_TRACESENDER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceSenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTraceSenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSendingOneTrace();
	afx_msg void OnSendingMultipleTraces();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CTracer m_Tracer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACESENDERDLG_H__ABC99A8F_1DB8_43CA_9835_2AAFF135461F__INCLUDED_)
