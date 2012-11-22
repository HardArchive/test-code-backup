// DragDropDoc.cpp : implementation of the CDragDropDoc class
//

#include "stdafx.h"
#include "DragDrop.h"

#include "DragDropDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDragDropDoc

IMPLEMENT_DYNCREATE(CDragDropDoc, CDocument)

BEGIN_MESSAGE_MAP(CDragDropDoc, CDocument)
	//{{AFX_MSG_MAP(CDragDropDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragDropDoc construction/destruction

CDragDropDoc::CDragDropDoc()
{
	// TODO: add one-time construction code here

}

CDragDropDoc::~CDragDropDoc()
{
}

BOOL CDragDropDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDragDropDoc serialization

void CDragDropDoc::Serialize(CArchive& ar)
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
// CDragDropDoc diagnostics

#ifdef _DEBUG
void CDragDropDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDragDropDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDragDropDoc commands
