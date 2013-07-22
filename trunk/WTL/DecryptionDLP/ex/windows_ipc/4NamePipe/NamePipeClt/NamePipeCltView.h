// NamePipeCltView.h : interface of the CNamePipeCltView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEPIPECLTVIEW_H__9C10050C_DE51_429E_821C_5AB44A515D4E__INCLUDED_)
#define AFX_NAMEPIPECLTVIEW_H__9C10050C_DE51_429E_821C_5AB44A515D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "YWPipe.h"

class CNamePipeCltView : public CView
{
protected: // create from serialization only
	CNamePipeCltView();
	DECLARE_DYNCREATE(CNamePipeCltView)

// Attributes
public:
	CNamePipeCltDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamePipeCltView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamePipeCltView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamePipeCltView)
	afx_msg void OnPipeConnect();
	afx_msg void OnPipeWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//HANDLE hPipe;
	CYWPipe m_pipe;
public:
	afx_msg void OnPipeDisconnect();
};

#ifndef _DEBUG  // debug version in NamePipeCltView.cpp
inline CNamePipeCltDoc* CNamePipeCltView::GetDocument()
   { return (CNamePipeCltDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEPIPECLTVIEW_H__9C10050C_DE51_429E_821C_5AB44A515D4E__INCLUDED_)
