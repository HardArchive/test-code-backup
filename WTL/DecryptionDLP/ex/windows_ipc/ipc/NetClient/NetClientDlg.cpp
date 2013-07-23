// NetClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NetClient.h"
#include "NetClientDlg.h"
#include "..\\classes\\MyPipe.h"
#include "..\\classes\\MySocket.h"
#include "..\\classes\\MyMailSlots.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CNetClientDlg dialog

CMySocket theSocket;
CMyMailSlots theMailSlots; 
CMyPipe thePipe;

CNetClientDlg::CNetClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetClientDlg)
	m_strMailSlots = _T("");
	m_strPipe = _T("");
	m_strSockets = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetClientDlg)
	DDX_Text(pDX, IDC_TEXT_MAILSLOTS, m_strMailSlots);
	DDX_Text(pDX, IDC_TEXT_PIPE, m_strPipe);
	DDX_Text(pDX, IDC_TEXT_SOCKETS, m_strSockets);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNetClientDlg, CDialog)
	//{{AFX_MSG_MAP(CNetClientDlg)
	ON_BN_CLICKED(IDC_SEND_SOCKETS, OnSendSockets)
	ON_BN_CLICKED(IDC_SEND_PIPE, OnSendPipe)
	ON_BN_CLICKED(IDC_SEND_MAILSLOTS, OnSendMailslots)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetClientDlg message handlers

BOOL CNetClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	thePipe.ClientCreateFile();
	theMailSlots.ClientCreateFile();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNetClientDlg::OnSendSockets() 
{
	UpdateData();	
	// 获得本地计算机IP地址. 
	CString strIP=dealsocket.GetLocalAddr();

	theSocket.SocketClient(strIP,4008,m_strSockets);
	m_strSockets=theSocket.GetReply();
	UpdateData(FALSE);	
}

void CNetClientDlg::OnSendPipe() 
{
	UpdateData();	
    thePipe.ClientSend(m_strPipe);
	m_strPipe=thePipe.GetReply();
	UpdateData(FALSE);	
}

void CNetClientDlg::OnSendMailslots() 
{
	// 没有回应字符串.
	UpdateData();	
    theMailSlots.ClientSend(m_strMailSlots);
}
