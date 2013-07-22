// NamePipeCltView.cpp : implementation of the CNamePipeCltView class
//

#include "stdafx.h"
#include "NamePipeClt.h"

#include "NamePipeCltDoc.h"
#include "NamePipeCltView.h"
#include ".\namepipecltview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BUFSIZE		4096
/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView

IMPLEMENT_DYNCREATE(CNamePipeCltView, CView)

BEGIN_MESSAGE_MAP(CNamePipeCltView, CView)
	//{{AFX_MSG_MAP(CNamePipeCltView)
	ON_COMMAND(IDM_PIPE_CONNECT, OnPipeConnect)
	ON_COMMAND(IDM_PIPE_WRITE, OnPipeWrite)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(IDM_PIPE_DISCONNECT, OnPipeDisconnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView construction/destruction

CNamePipeCltView::CNamePipeCltView()
{
	

}

CNamePipeCltView::~CNamePipeCltView()
{
	
}

BOOL CNamePipeCltView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView drawing

void CNamePipeCltView::OnDraw(CDC* pDC)
{
	CNamePipeCltDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView printing

BOOL CNamePipeCltView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNamePipeCltView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNamePipeCltView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView diagnostics

#ifdef _DEBUG
void CNamePipeCltView::AssertValid() const
{
	CView::AssertValid();
}

void CNamePipeCltView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNamePipeCltDoc* CNamePipeCltView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNamePipeCltDoc)));
	return (CNamePipeCltDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNamePipeCltView message handlers

void CNamePipeCltView::OnPipeConnect() 
{
	m_pipe.Create("CMDPipe",false);
}

void CNamePipeCltView::OnPipeWrite() 
{
	char Buff[BUFSIZE]="I'm Client!";
	m_pipe.SendData(Buff,BUFSIZE);
}

void CNamePipeCltView::OnPipeDisconnect()
{
	m_pipe.Close ();
	
}
