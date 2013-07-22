#include "stdafx.h"
#include "resource.h"
#include "WinNotify.h"
#include "WinNotifyDoc.h"
#include "WinNotifyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CWinNotifyView, CFormView)

CWinNotifyView::CWinNotifyView() : CFormView(CWinNotifyView::IDD)
{
}

CWinNotifyView::~CWinNotifyView()
{
}

void CWinNotifyView::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
	CFormView::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_MESSAGE, m_sMessage);
	DDX_Text(pDX, IDC_FROM, m_sFrom);
	DDX_Text(pDX, IDC_TO_DATE, m_sTimeAndTo);
}


BEGIN_MESSAGE_MAP(CWinNotifyView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_MSG, OnUpdateMessage)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CWinNotifyView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWinNotifyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

int CWinNotifyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  //Let the base class do its thing
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Create the timer to check for new mail
	m_nTimerID = SetTimer(1, 2000, NULL);
	
	return 0;
}

void CWinNotifyView::OnDestroy() 
{
  //Let the base class do its thing
	CFormView::OnDestroy();
	
  //Don't forget to destroy the timer
	KillTimer(m_nTimerID);
}

void CWinNotifyView::OnTimer(UINT_PTR /*nIDEvent*/) 
{
  //Check for new mail
  GetDoc()->CheckForNewMail();
}

void CWinNotifyView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/) 
{
  //Update the view based on the current message in the document
  m_sMessage = GetDoc()->GetCurrentMessage();
  if (GetDoc()->GetNumberMessages())
  {
    AfxFormatString2(m_sFrom, IDS_FROM, GetDoc()->GetCurrentSender(), GetDoc()->GetCurrentSenderMachine());
    AfxFormatString2(m_sTimeAndTo,   IDS_TO, GetDoc()->GetCurrentTo(), GetDoc()->GetCurrentMessageTime());

    AfxGetMainWnd()->SetIcon(GetApp()->m_hMessageIcon, FALSE);
    AfxGetMainWnd()->SetIcon(GetApp()->m_hMessageIcon, TRUE);
  }
  else
  {
    VERIFY(m_sFrom.LoadString(IDS_NO_MSGS));
    m_sTimeAndTo.Empty();

    AfxGetMainWnd()->SetIcon(GetApp()->m_hNoMessageIcon, FALSE);
    AfxGetMainWnd()->SetIcon(GetApp()->m_hNoMessageIcon, TRUE);
  }
  UpdateData(FALSE);
}

CWinNotifyDoc* CWinNotifyView::GetDoc() const
{
  return static_cast<CWinNotifyDoc*>(GetDocument());
}

void CWinNotifyView::OnUpdateMessage(CCmdUI* pCmdUI)
{
  //Update the status bar message string
  CString sNum1;
  sNum1.Format(_T("%d"), GetDoc()->GetCurrentMessageCount());
  CString sNum2;
  sNum2.Format(_T("%d"), GetDoc()->GetNumberMessages());
  CString sText;
  AfxFormatString2(sText, IDS_INDICATOR_MSG, sNum1, sNum2);
  pCmdUI->SetText(sText);
}
