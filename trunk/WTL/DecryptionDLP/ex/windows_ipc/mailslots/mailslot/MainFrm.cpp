#include "stdafx.h"
#include "WinNotify.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
  ID_INDICATOR_MSG,
};

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  //Let the base class do its thing
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE(_T("Failed to create toolbar\n"));
		return -1;
	}

	if (!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE(_T("Failed to create status bar\n"));
		return -1;      // fail to create
	}

  //Set the first pane of the status band non-strechable
  m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 250);

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |	CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE;

  //Force the size of the main frame to just surround the formview
  cs.cx = 367;
  cs.cy = 296;

	return CFrameWnd::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI) 
{
	lpMMI->ptMaxSize.x = 367;
  lpMMI->ptMaxSize.y = 296;
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  //Let the base class do its thing
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
  CString sText;
  INT_PTR nNum = GetDoc()->GetNumberMessages();
  if (nState == WA_INACTIVE && bMinimized && nNum)
  {
    if (nNum == 1)
      VERIFY(sText.LoadString(AFX_IDS_APP_TITLE3));
    else
    {
      CString sNum;
      sNum.Format(_T("%d"), nNum);
      AfxFormatString1(sText, AFX_IDS_APP_TITLE2, sNum);
    }
  }
  else
    VERIFY(sText.LoadString(AFX_IDS_APP_TITLE));

  SetWindowText(sText);
}

CWinNotifyDoc* CMainFrame::GetDoc()
{
  //What will be the return value from this function
  CWinNotifyDoc* pDoc = NULL;

  POSITION posTemplate = AfxGetApp()->GetFirstDocTemplatePosition();
  CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(posTemplate);
  if (pDocTemplate)
  {
    POSITION posDoc = pDocTemplate->GetFirstDocPosition();
    pDoc = static_cast<CWinNotifyDoc*>(pDocTemplate->GetNextDoc(posDoc));
  }
  
  ASSERT(pDoc);
  return pDoc;
}

void CMainFrame::OnClose() 
{
	if (AfxMessageBox(IDP_EXIT_MSG, MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
  	CFrameWnd::OnClose();
}
