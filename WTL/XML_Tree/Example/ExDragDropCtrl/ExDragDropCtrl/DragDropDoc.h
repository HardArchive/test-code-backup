// DragDropDoc.h : interface of the CDragDropDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGDROPDOC_H__3DEF942D_73BE_11D6_9CD8_444553540000__INCLUDED_)
#define AFX_DRAGDROPDOC_H__3DEF942D_73BE_11D6_9CD8_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDragDropDoc : public CDocument
{
protected: // create from serialization only
	CDragDropDoc();
	DECLARE_DYNCREATE(CDragDropDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDragDropDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDragDropDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROPDOC_H__3DEF942D_73BE_11D6_9CD8_444553540000__INCLUDED_)
