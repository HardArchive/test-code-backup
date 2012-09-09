// SendToCloneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SendToCloneDlg.h"
#include "CShellFileOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CALLBACK BrowseCallbackProc ( HWND hwnd, UINT uMsg,
                                  LPARAM lParam, LPARAM lpData );

/////////////////////////////////////////////////////////////////////////////
// CSendToCloneDlg dialog


CSendToCloneDlg::CSendToCloneDlg(const CStringList* pFileList, CWnd* pParent /*=NULL*/)
	: CDialog(CSendToCloneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendToCloneDlg)
	m_sTargetDir = _T("");
	m_nOperation = -1;
	//}}AFX_DATA_INIT

    m_pDroppedFiles = pFileList;
}


void CSendToCloneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendToCloneDlg)
	DDX_Control(pDX, IDC_FILE_LIST, m_ItemList);
	DDX_Text(pDX, IDC_TARGET_DIR, m_sTargetDir);
	DDV_MaxChars(pDX, m_sTargetDir, MAX_PATH-2);
	DDX_Radio(pDX, IDC_COPY, m_nOperation);
	//}}AFX_DATA_MAP

    if ( pDX->m_bSaveAndValidate )
        {
        pDX->PrepareEditCtrl ( IDC_TARGET_DIR );

        if ( m_sTargetDir.GetLength() == 0 )
            {
            AfxMessageBox ( _T("Please enter a target directory") );
            pDX->Fail();
            }
        }
}


BEGIN_MESSAGE_MAP(CSendToCloneDlg, CDialog)
	//{{AFX_MSG_MAP(CSendToCloneDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendToCloneDlg message handlers

BOOL CSendToCloneDlg::OnInitDialog() 
{
POSITION pos;

	CDialog::OnInitDialog();

    m_sTargetDir = AfxGetApp()->GetProfileString ( _T("Settings"),
                                                   _T("LastTargetDir") );


    // Radio button initialization
    m_nOperation = 0;                   // Always default to Copy.

    // List control initialization
    m_ItemList.InsertColumn ( 0, _T(""), LVCFMT_LEFT, 0, 0 );

    pos = m_pDroppedFiles->GetHeadPosition();
    ASSERT ( NULL != pos );

    while ( NULL != pos )
        {
        CString sNextItem = m_pDroppedFiles->GetNext ( pos );

        // Insert at index 0, and let the list sort the items itself.
        m_ItemList.InsertItem ( 0, sNextItem );
        }

    m_ItemList.SetColumnWidth ( 0, LVSCW_AUTOSIZE );

    UpdateData ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSendToCloneDlg::OnBrowse() 
{
TCHAR         szDir [MAX_PATH];
CString       sCurrentTargetDir;
OSVERSIONINFO rOSInfo = { sizeof(OSVERSIONINFO) };
LPITEMIDLIST  pidlTarget;
LPMALLOC      pMalloc;

BROWSEINFO bi = { AfxGetMainWnd()->GetSafeHwnd(),
                  NULL, szDir, _T("Select the target directory"),
                  BIF_RETURNONLYFSDIRS, BrowseCallbackProc };

    // If we're on Win 2K, add the BIF_USENEWUI flag to get the spiffy
    // new browse dialog.

    GetVersionEx ( &rOSInfo );

    if ( VER_PLATFORM_WIN32_NT == rOSInfo.dwPlatformId  &&
         rOSInfo.dwMajorVersion >= 5 )
        {
        bi.ulFlags |= 0x50;     // 0x50 == BIF_USENEWUI
        }

    GetDlgItemText ( IDC_TARGET_DIR, sCurrentTargetDir );
    bi.lParam = (LPARAM)(LPCTSTR) sCurrentTargetDir;

    pidlTarget = SHBrowseForFolder ( &bi );

    if ( NULL != pidlTarget )
        {
        SHGetMalloc ( &pMalloc );

        if ( SHGetPathFromIDList ( pidlTarget, szDir ))
            {
            SetDlgItemText ( IDC_TARGET_DIR, szDir );
            }

        pMalloc->Free ( pidlTarget );
        pMalloc->Release();
        }
}

void CSendToCloneDlg::OnOK() 
{
CShellFileOp fileop;
POSITION     pos;
BOOL         bJunk;

    if ( !UpdateData() )
        return;

    pos = m_pDroppedFiles->GetHeadPosition();
    ASSERT ( NULL != pos );

    while ( NULL != pos )
        {
        CString sNextItem = m_pDroppedFiles->GetNext ( pos );

        fileop.AddSourceFile ( sNextItem );
        }

    fileop.AddDestFile ( m_sTargetDir );
    fileop.SetOperationFlags ( m_nOperation ? FO_MOVE : FO_COPY,
                               this, FOF_NOCONFIRMMKDIR );

    fileop.Go ( &bJunk );

    AfxGetApp()->WriteProfileString ( _T("Settings"), 
                                      _T("LastTargetDir"),
                                      m_sTargetDir );

    CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CSendToCloneDlg dialog

int CALLBACK BrowseCallbackProc ( HWND hwnd, UINT uMsg,
                                  LPARAM lParam, LPARAM lpData )
{
    if ( BFFM_INITIALIZED == uMsg )
        {
        SendMessage ( hwnd, BFFM_SETSELECTION, TRUE, lpData );
        }

    return 0;
}
