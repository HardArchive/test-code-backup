// NamePipeCltDoc.cpp : implementation of the CNamePipeCltDoc class
//

#include "stdafx.h"
#include "NamePipeClt.h"

#include "NamePipeCltDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltDoc

IMPLEMENT_DYNCREATE(CNamePipeCltDoc, CDocument)

BEGIN_MESSAGE_MAP(CNamePipeCltDoc, CDocument)
	//{{AFX_MSG_MAP(CNamePipeCltDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltDoc construction/destruction

CNamePipeCltDoc::CNamePipeCltDoc()
{
	// TODO: add one-time construction code here

}

CNamePipeCltDoc::~CNamePipeCltDoc()
{
}

BOOL CNamePipeCltDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltDoc serialization

void CNamePipeCltDoc::Serialize(CArchive& ar)
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
// CNamePipeCltDoc diagnostics

#ifdef _DEBUG
void CNamePipeCltDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNamePipeCltDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltDoc commands
