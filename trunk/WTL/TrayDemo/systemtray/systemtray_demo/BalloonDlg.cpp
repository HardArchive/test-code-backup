// BalloonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "systemtraydemo.h"
#include "BalloonDlg.h"
#include "systemtray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBalloonDlg dialog


CBalloonDlg::CBalloonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBalloonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBalloonDlg)
	m_sBalloonText = _T("");
	m_sBalloonTitle = _T("");
	m_uTimeout = 0;
	//}}AFX_DATA_INIT
}


void CBalloonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBalloonDlg)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Text(pDX, IDC_BALLOON_TEXT, m_sBalloonText);
	DDX_Text(pDX, IDC_BALLOON_TITLE, m_sBalloonTitle);
	DDX_Text(pDX, IDC_TIMEOUT, m_uTimeout);
	DDV_MinMaxUInt(pDX, m_uTimeout, 10, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBalloonDlg, CDialog)
	//{{AFX_MSG_MAP(CBalloonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBalloonDlg message handlers

BOOL CBalloonDlg::OnInitDialog() 
{
    m_uTimeout = 10;

	CDialog::OnInitDialog();

    ((CSpinButtonCtrl*) GetDlgItem ( IDC_TIMEOUT_SPIN ))->SetRange ( 10, 30 );

    m_imglist.Create ( 16, 16, ILC_COLOR4 | ILC_MASK, 4, 1 );

    m_imglist.Add ( ::LoadIcon ( NULL, IDI_INFORMATION ));
    m_imglist.Add ( ::LoadIcon ( NULL, IDI_WARNING ));
    m_imglist.Add ( ::LoadIcon ( NULL, IDI_ERROR ));

    m_combo.SetImageList ( &m_imglist );

    COMBOBOXEXITEM item;

    item.mask = CBEIF_TEXT;
    item.iItem = 0;
    item.pszText = _T("None");
    m_combo.InsertItem ( &item );

    item.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
    item.iItem = 1;
    item.pszText = _T("Information");
    item.iImage = item.iSelectedImage = 0;
    m_combo.InsertItem ( &item );

    item.pszText = _T("Warning");
    item.iItem = 2;
    item.iImage = item.iSelectedImage = 1;
    m_combo.InsertItem ( &item );

    item.pszText = _T("Error");
    item.iItem = 3;
    item.iImage = item.iSelectedImage = 2;
    m_combo.InsertItem ( &item );

    m_combo.SetCurSel ( 0 );

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBalloonDlg::OnOK() 
{
#ifdef SYSTEMTRAY_USEW2K
    switch ( m_combo.GetCurSel() )
        {
        case 0: m_dwIcon = NIIF_NONE;    break;
        case 1: m_dwIcon = NIIF_INFO;    break;
        case 2: m_dwIcon = NIIF_WARNING; break;
        case 3: m_dwIcon = NIIF_ERROR;   break;
        }
#endif

	CDialog::OnOK();
}
