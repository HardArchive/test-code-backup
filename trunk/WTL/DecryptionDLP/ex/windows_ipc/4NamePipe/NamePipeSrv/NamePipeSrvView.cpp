// NamePipeSrvView.cpp : implementation of the CNamePipeSrvView class
//

#include "stdafx.h"
#include "NamePipeSrv.h"

#include "NamePipeSrvDoc.h"
#include "NamePipeSrvView.h"
#include ".\namepipesrvview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView

IMPLEMENT_DYNCREATE(CNamePipeSrvView, CView)

BEGIN_MESSAGE_MAP(CNamePipeSrvView, CView)
	//{{AFX_MSG_MAP(CNamePipeSrvView)
	ON_COMMAND(IDM_PIPE_CREATE, OnPipeCreate)
	ON_COMMAND(IDM_PIPE_WRITE, OnPipeWrite)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_PIPE_CLOSE, OnPipeClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView construction/destruction

CNamePipeSrvView::CNamePipeSrvView()
{
	
}

CNamePipeSrvView::~CNamePipeSrvView()
{

}

BOOL CNamePipeSrvView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView drawing

void CNamePipeSrvView::OnDraw(CDC* pDC)
{
	CNamePipeSrvDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView printing

BOOL CNamePipeSrvView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNamePipeSrvView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNamePipeSrvView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView diagnostics

#ifdef _DEBUG
void CNamePipeSrvView::AssertValid() const
{
	CView::AssertValid();
}

void CNamePipeSrvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNamePipeSrvDoc* CNamePipeSrvView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNamePipeSrvDoc)));
	return (CNamePipeSrvDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamePipeSrvView message handlers

void CNamePipeSrvView::OnPipeCreate() 
{
	m_pipe.Create("CMDPipe",true);
}

void CNamePipeSrvView::OnPipeWrite() 
{
	char Buff[BUFSIZE]="I'm Service!";
	m_pipe.SendData(Buff,BUFSIZE);
}

void CNamePipeSrvView::OnPipeClose()
{
	m_pipe.Close ();
}
