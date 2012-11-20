// HttpResDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HTTP Spy.h"
#include "HttpResDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpResDlg dialog


CHttpResDlg::CHttpResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHttpResDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHttpResDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHttpResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHttpResDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHttpResDlg, CDialog)
	//{{AFX_MSG_MAP(CHttpResDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpResDlg message handlers
