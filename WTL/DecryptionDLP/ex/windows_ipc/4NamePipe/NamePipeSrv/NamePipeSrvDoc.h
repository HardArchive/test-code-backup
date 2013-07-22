// NamePipeSrvDoc.h : interface of the CNamePipeSrvDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEPIPESRVDOC_H__46BA1380_1C46_432E_9124_DC6EF7362B2C__INCLUDED_)
#define AFX_NAMEPIPESRVDOC_H__46BA1380_1C46_432E_9124_DC6EF7362B2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNamePipeSrvDoc : public CDocument
{
protected: // create from serialization only
	CNamePipeSrvDoc();
	DECLARE_DYNCREATE(CNamePipeSrvDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamePipeSrvDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamePipeSrvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamePipeSrvDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEPIPESRVDOC_H__46BA1380_1C46_432E_9124_DC6EF7362B2C__INCLUDED_)
