#if !defined(AFX_DRAGDROPVIEW_H__06478682_741E_11D6_9CD8_444553540000__INCLUDED_)
#define AFX_DRAGDROPVIEW_H__06478682_741E_11D6_9CD8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DragDropView.h : header file
//

#include "TreeCtrlDrag.h"

/////////////////////////////////////////////////////////////////////////////
// CDragDropView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDragDropView : public CFormView
{
protected:
	CDragDropView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDragDropView)

// Form Data
public:
	//{{AFX_DATA(CDragDropView)
	enum { IDD = IDD_FORMVIEW };
	CButton	m_fbtnUp;
	CButton	m_fbtnDown;
	CTreeCtrlDrag	m_Tree;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDragDropView();

protected:
    CImageList        m_imgList;

	// Generated message map functions
	//{{AFX_MSG(CDragDropView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
    afx_msg void OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnNewFolder();
	afx_msg void OnBtnRename();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROPVIEW_H__06478682_741E_11D6_9CD8_444553540000__INCLUDED_)
