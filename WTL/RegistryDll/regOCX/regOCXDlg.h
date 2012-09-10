// regOCXDlg.h : header file
// 
//功能：控件注册器的窗体类，在此加入注册，注销控件以及显示桌面等功能
// 作者：徐景周 　
//日期：2001年7月8日
//

#if !defined(AFX_REGOCXDLG_H__C1D2BFA6_73CA_11D5_9008_A4FA56D9ADF0__INCLUDED_)
#define AFX_REGOCXDLG_H__C1D2BFA6_73CA_11D5_9008_A4FA56D9ADF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegOCXDlg dialog

//桌面最大运行窗体数
#define MAXNUM_RUNWINDOW 256

#include "WBButton.h" //位图按钮类

class CRegOCXDlg : public CDialog
{
// Construction
public:
	CRegOCXDlg(CWnd* pParent = NULL);	// standard constructor

	//最小化或还原桌面所有窗体指针数组及数目
	CWnd * m_pWnd[MAXNUM_RUNWINDOW];
    int m_nWndNum;

// Dialog Data
	//{{AFX_DATA(CRegOCXDlg)
	enum { IDD = IDD_REGOCX_DIALOG };
	CWBButton	m_Cancel;   //设置为位图按钮类
	CWBButton	m_UnRegOCX;
	CWBButton	m_ShowDesktop;
	CWBButton	m_Browse;
	CWBButton	m_About;
	CWBButton	m_RegOCX;
	CString	m_FileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegOCXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

private:
	CRgn m_rgn ; // 建立球形区域的窗体变量
 
	// Generated message map functions
	//{{AFX_MSG(CRegOCXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRegocx();
	afx_msg void OnUnregocx();
	afx_msg void OnShowdesktop();
	afx_msg void OnAbout();
	afx_msg void OnBrowse();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGOCXDLG_H__C1D2BFA6_73CA_11D5_9008_A4FA56D9ADF0__INCLUDED_)
