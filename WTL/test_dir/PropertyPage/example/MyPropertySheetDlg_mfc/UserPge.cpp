// UserPge.cpp : implementation file
//

#include "stdafx.h"
#include "MyPropertySheetDlg.h"
#include "UserPge.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserPge property page

IMPLEMENT_DYNCREATE(CUserPge, CPropertyPage)

CUserPge::CUserPge() : CPropertyPage(CUserPge::IDD)
{
	//{{AFX_DATA_INIT(CUserPge)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserPge::~CUserPge()
{
}

void CUserPge::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserPge)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserPge, CPropertyPage)
	//{{AFX_MSG_MAP(CUserPge)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserPge message handlers
