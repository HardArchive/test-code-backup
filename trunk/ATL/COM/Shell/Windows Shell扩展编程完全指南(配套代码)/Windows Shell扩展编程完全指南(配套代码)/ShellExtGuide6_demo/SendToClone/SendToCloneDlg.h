// SendToCloneDlg.h : header file
//

#if !defined(AFX_SENDTOCLONEDLG_H__B7F32414_0E29_11D4_8D3B_80CD3621FB09__INCLUDED_)
#define AFX_SENDTOCLONEDLG_H__B7F32414_0E29_11D4_8D3B_80CD3621FB09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSendToCloneDlg dialog

class CSendToCloneDlg : public CDialog
{
// Construction
public:
	CSendToCloneDlg(const CStringList* pFileList, 
                    CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSendToCloneDlg)
	enum { IDD = IDD_MAINDLG };
	CListCtrl	m_ItemList;
	CString	m_sTargetDir;
	int		m_nOperation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendToCloneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    const CStringList* m_pDroppedFiles;

	// Generated message map functions
	//{{AFX_MSG(CSendToCloneDlg)
	afx_msg void OnBrowse();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDTOCLONEDLG_H__B7F32414_0E29_11D4_8D3B_80CD3621FB09__INCLUDED_)
