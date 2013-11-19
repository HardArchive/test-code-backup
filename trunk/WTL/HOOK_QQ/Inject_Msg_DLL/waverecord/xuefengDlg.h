// xuefengDlg.h : header file
//

#if !defined(AFX_XUEFENGDLG_H__A3A05BA2_9E37_4E7B_9A50_074A45BF4E85__INCLUDED_)
#define AFX_XUEFENGDLG_H__A3A05BA2_9E37_4E7B_9A50_074A45BF4E85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HwMCI.h"

/////////////////////////////////////////////////////////////////////////////
// CXuefengDlg dialog

class CXuefengDlg : public CDialog
{
// Construction
public:
	//~CxuefengDlg();
	CXuefengDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXuefengDlg)
	enum { IDD = IDD_XUEFENG_DIALOG };

	CComboBox	m_BitsPerSample;
	CComboBox	m_Channels;
	CComboBox	m_SamplesPerSec;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXuefengDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int			m_nChannels;//音频通道数
	int			m_nSamplesPerSec;//采样频率
	WORD		m_wBitsPerSample;//采样大小

	// Generated message map functions
	//{{AFX_MSG(CXuefengDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonRec();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CHwMCI m_HwMCI;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XUEFENGDLG_H__A3A05BA2_9E37_4E7B_9A50_074A45BF4E85__INCLUDED_)
