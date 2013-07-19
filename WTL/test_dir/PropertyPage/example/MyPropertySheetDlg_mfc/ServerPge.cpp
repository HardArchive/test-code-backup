// ServerPge.cpp : implementation file
//

#include "stdafx.h"
#include "MyPropertySheetDlg.h"
#include "ServerPge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerPge property page

IMPLEMENT_DYNCREATE(CServerPge, CPropertyPage)

CServerPge::CServerPge() : CPropertyPage(CServerPge::IDD)
{
	//{{AFX_DATA_INIT(CServerPge)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CServerPge::~CServerPge()
{
}

void CServerPge::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerPge)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerPge, CPropertyPage)
	//{{AFX_MSG_MAP(CServerPge)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerPge message handlers
