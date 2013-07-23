// NetServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetServer.h"
#include "NetServerDlg.h"
#include "..\\classes\\MyPipe.h"
#include "..\\classes\\MySocket.h"
#include "..\\classes\\MyMailSlots.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMySocket theSocket;
CMyMailSlots theMailSlots; 
CMyPipe thePipe;
/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg dialog

CNetServerDlg::CNetServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetServerDlg)
	m_strPipe = _T("");
	m_strSockets = _T("");
	m_strMailSlots = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetServerDlg)
	DDX_Text(pDX, IDC_TEXT_RECVPIPE, m_strPipe);
	DDX_Text(pDX, IDC_TEXT_RECVSOCKETS, m_strSockets);
	DDX_Text(pDX, IDC_TEXT_RECVMAILSLOTS, m_strMailSlots);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetServerDlg, CDialog)
	//{{AFX_MSG_MAP(CNetServerDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetServerDlg message handlers

BOOL CNetServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetTimer(1,900,NULL);
	// TODO: Add extra initialization here

	thePipe.ServerCreateNamedPipe("data received!");
	theMailSlots.ServerCreateMailslot();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetServerDlg::OnTimer(UINT nIDEvent) 
{
	theSocket.SocketServer(4008, "data received!");
	m_strSockets=theSocket.GetRequest();

	m_strPipe=thePipe.GetRequest();
	m_strMailSlots=theMailSlots.GetRequest();

	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
