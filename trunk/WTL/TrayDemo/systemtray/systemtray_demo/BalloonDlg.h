#if !defined(AFX_BALLOONDLG_H__D7C677EE_194E_4281_A2EB_13C04C59B3EB__INCLUDED_)
#define AFX_BALLOONDLG_H__D7C677EE_194E_4281_A2EB_13C04C59B3EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BalloonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBalloonDlg dialog

class CBalloonDlg : public CDialog
{
// Construction
public:
	CBalloonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBalloonDlg)
	enum { IDD = IDD_TRAY_ICON_BALLOON };
	CComboBoxEx	m_combo;
	CString	m_sBalloonText;
	CString	m_sBalloonTitle;
	UINT	m_uTimeout;
	//}}AFX_DATA

    DWORD m_dwIcon;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBalloonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CImageList m_imglist;

	// Generated message map functions
	//{{AFX_MSG(CBalloonDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLOONDLG_H__D7C677EE_194E_4281_A2EB_13C04C59B3EB__INCLUDED_)
