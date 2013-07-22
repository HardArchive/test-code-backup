// NamePipeSrvDoc.cpp : implementation of the CNamePipeSrvDoc class
//

#include "stdafx.h"
#include "NamePipeSrv.h"

#include "NamePipeSrvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvDoc

IMPLEMENT_DYNCREATE(CNamePipeSrvDoc, CDocument)

BEGIN_MESSAGE_MAP(CNamePipeSrvDoc, CDocument)
	//{{AFX_MSG_MAP(CNamePipeSrvDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvDoc construction/destruction

CNamePipeSrvDoc::CNamePipeSrvDoc()
{
	// TODO: add one-time construction code here

}

CNamePipeSrvDoc::~CNamePipeSrvDoc()
{
}

BOOL CNamePipeSrvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvDoc serialization

void CNamePipeSrvDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvDoc diagnostics

#ifdef _DEBUG
void CNamePipeSrvDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNamePipeSrvDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvDoc commands
