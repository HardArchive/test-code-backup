// NamePipeCltDoc.h : interface of the CNamePipeCltDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEPIPECLTDOC_H__EE832359_90D0_413A_AEA5_78C6CD25E12D__INCLUDED_)
#define AFX_NAMEPIPECLTDOC_H__EE832359_90D0_413A_AEA5_78C6CD25E12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNamePipeCltDoc : public CDocument
{
protected: // create from serialization only
	CNamePipeCltDoc();
	DECLARE_DYNCREATE(CNamePipeCltDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamePipeCltDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamePipeCltDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamePipeCltDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEPIPECLTDOC_H__EE832359_90D0_413A_AEA5_78C6CD25E12D__INCLUDED_)
