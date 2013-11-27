// ------------------------------------------------------------------------------------
// Copyright ©2002 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
// ------------------------------------------------------------------------------------
//
// IncomingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AVDConf.h"
#include "defines.h"

#include "IncomingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIncomingDlg dialog


CIncomingDlg::CIncomingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIncomingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIncomingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIncomingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIncomingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIncomingDlg, CDialog)
	//{{AFX_MSG_MAP(CIncomingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIncomingDlg message handlers

BOOL CIncomingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SESSION_DATA	*pData;

	pData = (SESSION_DATA *)GetWindowLong (::GetWindow(m_hWnd, GW_OWNER), GWL_USERDATA);

	// Display the caller name and ID.
	SetDlgItemTextW (m_hWnd, IDC_CALLERNAME, pData->bstrName);
	SetDlgItemTextW (m_hWnd, IDC_CALLERID, pData->bstrURI);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
