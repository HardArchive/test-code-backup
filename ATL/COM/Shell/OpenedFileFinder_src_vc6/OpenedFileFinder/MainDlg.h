#if !defined(AFX_MAINDLG_H__4D07C7BC_0870_45EB_9823_9A4A081D76B5__INCLUDED_)
#define AFX_MAINDLG_H__4D07C7BC_0870_45EB_9823_9A4A081D76B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

#include <afxctl.h>
#include <afxcmn.h>
#include <afxtempl.h>

struct PROCESS_INFO_t
{
    CString csProcess;
    DWORD dwImageListIndex;
};

// MainDlg dialog
class MainDlg : public CDialog
{
// Construction
public:
	
	MainDlg( CString csPath, CWnd* pParent = NULL );
    
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	
	bool InstallDriver();
    
	void EnumerateOpenedFiles( HANDLE hDriver );
    
	void EnumerateLoadedModules();

	// Dialog Data
	//{{AFX_DATA(MainDlg)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_combobox;
	CListCtrl	m_list;
	//}}AFX_DATA
    void Populate( bool bShowModule = true );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MainDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMainTerminate();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMainTerminateallprocess();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnButtonrefresh();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnButtonGo();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMainCopyfilename();
	afx_msg void OnMainFindtarget();	
	afx_msg void OnMainClosehandle();
	afx_msg void OnMainCloseallhandles();
	afx_msg void OnMainShowloadedmodulesonly();
	afx_msg void OnMainShowloadedfilesonly();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);

private:

    CString		m_csPath;
    bool		m_bSortAscending;
    int			m_nCoumnclicked;
    CToolBar	m_toolBar;
    CToolBar	m_toolBarGo;    
    CImageList	m_imgToolbar1;
    CImageList	m_imgToolbar2;
    CMap<int,int,PROCESS_INFO_t,PROCESS_INFO_t&> m_stProcessInfo;
    CImageList m_imgListCtrl;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__4D07C7BC_0870_45EB_9823_9A4A081D76B5__INCLUDED_)
