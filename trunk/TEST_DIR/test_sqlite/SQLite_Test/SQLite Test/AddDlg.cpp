// AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SQLite Test.h"
#include "AddDlg.h"


// CAddDlg dialog

IMPLEMENT_DYNAMIC(CAddDlg, CDialog)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDlg::IDD, pParent)
{
    m_pCars = NULL;
}


void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

bool CAddDlg::MyDoModal(CCarsTbl* pCars) 
{ 
    m_pCars = pCars; 
    return DoModal() != 0; 
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddDlg message handlers

void CAddDlg::OnBnClickedOk()
{
    CString cs;
    if (m_pCars) {
        m_pCars->AddNew();
        
        GetDlgItemText(IDC_OWNER, cs);
        m_pCars->m_owner = cs;

        GetDlgItemText(IDC_MAKE, cs);
        m_pCars->m_make = cs;

        GetDlgItemText(IDC_MODEL, cs);
        m_pCars->m_model = cs;

        GetDlgItemText(IDC_YEAR, cs);
        m_pCars->m_year = cs;

        m_pCars->Update();
        
        EndDialog(TRUE);
    }
}

void CAddDlg::OnBnClickedCancel()
{
    EndDialog(FALSE);
}
