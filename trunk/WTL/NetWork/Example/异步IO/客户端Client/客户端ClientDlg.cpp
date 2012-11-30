// �ͻ���ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "�ͻ���Client.h"
#include "�ͻ���ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MAX_BUF_SIZE 1024
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_hConnectThread=NULL;
	sClient=INVALID_SOCKET;
	SetDlgItemText(IDC_IPADDRESS1,"192.168.1.100");
	SetDlgItemText(IDC_EDIT3,"1125");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CClientDlg::OnButton1() //���ӷ����
{
	if (sClient==INVALID_SOCKET)
	{
		m_hConnectThread=CreateThread(NULL,0,ConnectThreadFunc,this,0,NULL);
		//�˴�this�Ǵ���ȥ�ı�����ָ��
		
		SetDlgItemText(IDC_EDIT1,"�����̣߳�");
	}	
}
DWORD WINAPI ConnectThreadFunc(LPVOID pParam)//�߳�
{
	CClientDlg *pDlg=(CClientDlg *)pParam;//ȡ���������Ĵ���ָ��
	ASSERT(pDlg != NULL);//�׳��쳣
//***********************************************	
	//װ��socket��
	if (!pDlg->InItSocket())
	{
		AfxMessageBox("װ���׽��ֿ����");
		return FALSE;
	}
	//
	pDlg->sClient=socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); //�����׽���
    if (INVALID_SOCKET==pDlg->sClient)
    {
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return FALSE;
    }
	//
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(pDlg->GetDlgItemInt(IDC_EDIT3));//�ṹ���˿ں�
	CString ipstr;
    pDlg->GetDlgItemText(IDC_IPADDRESS1,ipstr);
	server.sin_addr.s_addr = inet_addr(ipstr);
	//����
	if ( connect(pDlg->sClient, (struct sockaddr *)&server, sizeof(struct sockaddr)) == SOCKET_ERROR )
	{		
		AfxMessageBox(_T("����ʧ�ܣ������ԣ�"));		
		goto __Error_End;
	}else
	{pDlg->SetDlgItemText(IDC_EDIT1,"�����ӵ������!");}
//********************************************************************
//////////////////////////////////////////////////////////////////////
	while (TRUE)
	{
		if (pDlg->Socket_Select(pDlg->sClient))
		{
			char szBuf[MAX_BUF_SIZE]; //���ջ���
			memset(szBuf,0,MAX_BUF_SIZE);//��������0
			int iRet = recv(pDlg->sClient,szBuf, MAX_BUF_SIZE, 0);//����
			if (iRet>0)
			{
				//***********
				//���յ�������������ʾ
				//������ʵ���Լ���ܶණ����һЩ����ͨ�ŵĹ��ܿ��Է�������紫�ļ�
				CString recMsg=szBuf;
				pDlg->SetDlgItemText(IDC_EDIT1,recMsg);
			}else			
			{
				pDlg->SetDlgItemText(IDC_EDIT1,"��������ߣ�����������");	
				break;
			}
		}else
		{
			continue;
		}
	}
__Error_End:
    closesocket(pDlg->sClient);
    pDlg->sClient=INVALID_SOCKET;
    return TRUE;
}
BOOL CClientDlg::InItSocket()//װ���׽��ֿ�
{
	WSADATA data = {0};
	if(WSAStartup(MAKEWORD(2, 2), &data))//2.2���׽���
		return FALSE;
	if ( LOBYTE(data.wVersion) !=2 || HIBYTE(data.wVersion) != 2 ){
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
BOOL CClientDlg::Socket_Select(SOCKET hSocket)
{
	fd_set fdset;
	timeval tv;
	FD_ZERO(&fdset);//��0
	FD_SET(hSocket, &fdset);//�����׽��ֵ�SELECT�����С���������ֻ��һ��
	tv.tv_sec  = 0;
	tv.tv_usec = 200;//��ʱʱ��200����
	int iSelRet = select(0, &fdset, NULL , NULL, &tv);//ֻ���ɶ�
    if (iSelRet<=0)
    {
		return FALSE;
    }else if (FD_ISSET(hSocket, &fdset))//iSelRet>0 �����׽�����������
	{
		return TRUE;
	}
	return FALSE;
}

void CClientDlg::OnButton2() //����
{
	if (m_hConnectThread!=NULL)//�߳��Ƿ���
	{
		CString strMsg;
		GetDlgItemText(IDC_EDIT2,strMsg);
		SetDlgItemText(IDC_EDIT2,"");
		int iSend=send(sClient, (char *)strMsg.GetBuffer(sizeof(strMsg)), strMsg.GetLength(), 0);		
	}	
}
