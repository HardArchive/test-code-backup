// RealClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RealClient.h"
#include "RealClientDlg.h"
#pragma comment(lib,"ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRealClientDlg dialog

CRealClientDlg::CRealClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealClientDlg)
	m_port = 4000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRealClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealClientDlg)
	DDX_Control(pDX, IDC_IPADDRESS1, m_Ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRealClientDlg, CDialog)
	//{{AFX_MSG_MAP(CRealClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SUB, OnBtnSub)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealClientDlg message handlers

BOOL CRealClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//设置界面
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
	m_Ip.SetAddress(127,0,0,1);
	m_port = 4000;
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRealClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRealClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CRealClientDlg::SetDlgText(CString strText)
{
	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->SetSel(-1,-1);
	((CEdit*)GetDlgItem(IDC_EDIT_RECV))->ReplaceSel(strText+"\r\n");

}

void CRealClientDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here
	CString sendBuf;
	CString realBuf;
	((CEdit*)GetDlgItem(IDC_EDIT_SEND))->GetWindowText(sendBuf);
	if (sendBuf.IsEmpty())
	{
		return;
	}
	if(INVALID_SOCKET==send(m_ConnectSock,sendBuf,strlen(sendBuf)+1,0))
	{
		SetDlgText("已与服务器失去连接，发送失败...");
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
		return;
	}
	else
	{
		realBuf.Format("%s say:%s","客户端",sendBuf);
		SetDlgText(realBuf);
	}
	GetDlgItem(IDC_EDIT_SEND)->SetWindowText("");
}

void CRealClientDlg::OnBtnConnect() 
{
	// TODO: Add your control notification handler code here
	m_ConnectSock = socket(AF_INET,SOCK_STREAM,0);
	if (INVALID_SOCKET == m_ConnectSock)
	{
		MessageBox("创建套接字失败");
		return;
	}
	DWORD addrIp;
	UpdateData(TRUE);
	SetDlgText("正在连接服务端......");
	
	m_Ip.GetAddress(addrIp);
	int port = GetDlgItemInt(IDC_EDIT_PORT);
    sockaddr_in addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(addrIp);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	if (connect( m_ConnectSock, (SOCKADDR*) &addrSrv, sizeof(addrSrv) ) == SOCKET_ERROR) 
	{
		CString strError = "连接服务端失败(⊙_⊙)......";
		SetDlgText(strError);
		return;
	}
	else
	{
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(TRUE);
		SetDlgText("连接服务端成功......");
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SUB)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	}
	CreateThread(NULL,0,ThreadRecv,this,0,NULL);//创建线程循环接收消息
}

DWORD WINAPI CRealClientDlg::ThreadRecv(LPVOID lpParameter)
{
	CRealClientDlg*pDlg = (CRealClientDlg*)lpParameter;
	CString recvBuf;
	CString realBuf;
	while(TRUE)
	{
		if(SOCKET_ERROR==recv(pDlg->m_ConnectSock,recvBuf.GetBuffer(0),200,0))
		{
			closesocket(pDlg->m_ConnectSock);
			pDlg->SetDlgText("连接中断!请重新连接...");		
			::EnableWindow(::GetDlgItem(pDlg->m_hWnd,IDC_EDIT_PORT),TRUE);
			::EnableWindow(::GetDlgItem(pDlg->m_hWnd,IDC_BTN_CONNECT),TRUE);
			::EnableWindow(::GetDlgItem(pDlg->m_hWnd,IDC_BTN_SUB),TRUE);
			::EnableWindow(::GetDlgItem(pDlg->m_hWnd,IDC_BTN_ADD),TRUE);
			::EnableWindow(::GetDlgItem(pDlg->m_hWnd,IDC_BTN_DISCONNECT),FALSE);
			break;
		}
		else
		{
			realBuf.Format("%s say:%s","服务端",recvBuf);
			pDlg->SetDlgText(realBuf);
		}
		
	}
	return 0;
}

void CRealClientDlg::OnBtnDisconnect() 
{
	// TODO: Add your control notification handler code here
	closesocket(m_ConnectSock);
	GetDlgItem(IDC_BTN_SUB)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(FALSE);
}

void CRealClientDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_RECV)->SetWindowText("");
}

HBRUSH CRealClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_EDIT_RECV)
		pDC->SetTextColor(RGB(23,120,0));
    if(pWnd->GetDlgCtrlID()==IDC_EDIT_SEND)
		pDC->SetTextColor(RGB(0,0,233));
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CRealClientDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	m_port+=1;
	UpdateData(FALSE);
}

void CRealClientDlg::OnBtnSub() 
{
	// TODO: Add your control notification handler code here
	m_port-=1;
	UpdateData(FALSE);
}
