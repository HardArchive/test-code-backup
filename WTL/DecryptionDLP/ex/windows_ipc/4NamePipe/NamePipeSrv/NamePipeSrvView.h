// NamePipeSrvView.h : interface of the CNamePipeSrvView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEPIPESRVVIEW_H__DCD599F5_6044_4FA2_858D_7F567CAC7670__INCLUDED_)
#define AFX_NAMEPIPESRVVIEW_H__DCD599F5_6044_4FA2_858D_7F567CAC7670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "YWpipe.h"

class CNamePipeSrvView : public CView
{
protected: // create from serialization only
	CNamePipeSrvView();
	DECLARE_DYNCREATE(CNamePipeSrvView)

// Attributes
public:
	CNamePipeSrvDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamePipeSrvView)
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
	virtual ~CNamePipeSrvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamePipeSrvView)
	afx_msg void OnPipeCreate();
	afx_msg void OnPipeWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//HANDLE hPipe;
	CYWPipe m_pipe;

public:
	afx_msg void OnPipeClose();
};

#ifndef _DEBUG  // debug version in NamePipeSrvView.cpp
inline CNamePipeSrvDoc* CNamePipeSrvView::GetDocument()
   { return (CNamePipeSrvDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEPIPESRVVIEW_H__DCD599F5_6044_4FA2_858D_7F567CAC7670__INCLUDED_)
