// regOCXDlg.h : header file
// 
//���ܣ��ؼ�ע�����Ĵ����࣬�ڴ˼���ע�ᣬע���ؼ��Լ���ʾ����ȹ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��8��
//

#if !defined(AFX_REGOCXDLG_H__C1D2BFA6_73CA_11D5_9008_A4FA56D9ADF0__INCLUDED_)
#define AFX_REGOCXDLG_H__C1D2BFA6_73CA_11D5_9008_A4FA56D9ADF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegOCXDlg dialog

//����������д�����
#define MAXNUM_RUNWINDOW 256

#include "WBButton.h" //λͼ��ť��

class CRegOCXDlg : public CDialog
{
// Construction
public:
	CRegOCXDlg(CWnd* pParent = NULL);	// standard constructor

	//��С����ԭ�������д���ָ�����鼰��Ŀ
	CWnd * m_pWnd[MAXNUM_RUNWINDOW];
    int m_nWndNum;

// Dialog Data
	//{{AFX_DATA(CRegOCXDlg)
	enum { IDD = IDD_REGOCX_DIALOG };
	CWBButton	m_Cancel;   //����Ϊλͼ��ť��
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
	CRgn m_rgn ; // ������������Ĵ������
 
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
