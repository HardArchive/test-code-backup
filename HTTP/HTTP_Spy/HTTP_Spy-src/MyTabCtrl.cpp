// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HTTP Spy.h"
#include "MyTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_Dialog[0] = new CHttpResDlg();
	m_Dialog[1] = new CHttpResDlg();
	m_TabCount = 2;
}

CMyTabCtrl::~CMyTabCtrl()
{
	for(int i = 0; i< m_TabCount; i++)
	{
		if(m_Dialog[i]!=NULL)
			delete m_Dialog[i];
	}
	
}

void CMyTabCtrl::setDialogText(int dlgID, CString txt)
{
  if(dlgID < 0 || dlgID >= m_TabCount)
	return;
  
 
   
  CEdit *cedit = (CEdit *)m_Dialog[dlgID]->GetDlgItem(IDC_EDIT_HTTPRES);
  cedit->SetWindowText(txt);
  /*while(index!=-1)
  {
	  CString strResLine = txt.Mid(start, (index-(start)));
	  if(strResLine.GetLength() > 0)
	  {
		  if(start ==0)
			  cbox->AddString(strResLine);
		  else
			  cbox->AddString(strResLine);
	  }
	  start = index + 2;
	  index = txt.Find("\r",start);
	  //index = -1;
  }*/
  //cbox->AddString(txt);
}

void CMyTabCtrl::InitDialogs()
{
	m_Dialog[0]->Create(IDD_RESPONSE_DIALOG,GetParent());
	/*CListBox *cbox = (CListBox *)m_Dialog[0]->GetDlgItem(IDC_RESPONSE_LIST);
	cbox->AddString("Box 1"); */
	m_Dialog[1]->Create(IDD_RESPONSE_DIALOG,GetParent());
    /*cbox = (CListBox *)m_Dialog[1]->GetDlgItem(IDC_RESPONSE_LIST);
	cbox->AddString("Box 2");*/

	CRect l_rectClient;
	CRect l_rectWnd;

	GetClientRect(l_rectClient);
	AdjustRect(FALSE,l_rectClient);
	GetWindowRect(l_rectWnd);
	GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	for(int nCount=0; nCount < m_TabCount; nCount++){
		m_Dialog[nCount]->SetWindowPos(&wndTop, l_rectClient.left,l_rectClient.top,l_rectClient.Width(),l_rectClient.Height(),SWP_HIDEWINDOW);
	}

	m_Dialog[0]->ShowWindow(SW_SHOW);
	m_Dialog[1]->ShowWindow(SW_HIDE);

}

BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers



void CMyTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	ActivateTabDialogs();
	*pResult = 0;
}

void CMyTabCtrl::ActivateTabDialogs()
{
   int nSel = GetCurSel();
   if(m_Dialog[nSel]->m_hWnd)
		m_Dialog[nSel]->ShowWindow(SW_HIDE);

//	m_Dialog[nSel]->SetWindowPos(&wndTop,l_rectClient.left,l_rectClient.top,l_rectClient.Width(),l_rectClient.Height(),SWP_SHOWWINDOW);
//	m_Dialog[nSel]->ShowWindow(SW_SHOW);
}

void CMyTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
   int nSel = GetCurSel();	
   if(m_Dialog[nSel]->m_hWnd)
      m_Dialog[nSel]->ShowWindow(SW_SHOW);
	*pResult = 0;
}
