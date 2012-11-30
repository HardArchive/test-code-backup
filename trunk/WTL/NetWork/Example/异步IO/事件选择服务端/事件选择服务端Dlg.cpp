// �¼�ѡ������Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "�¼�ѡ������.h"
#include "�¼�ѡ������Dlg.h"

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
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
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
	TotalConn =0; //��������
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyDlg::OnPaint() 
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
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//********************************************************************************
BOOL CMyDlg::InItSocket()//װ���׽��ֿ�
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

void CMyDlg::OnButton1() //�򿪷����
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	//�˴�this�Ǵ���ȥ�ı�����ָ�� ����ȡһЩֵ���ǿ��ƿؼ�
}
DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CMyDlg *pDlg=(CMyDlg *)pParam;//ȡ���������Ĵ���ָ��
	ASSERT(pDlg != NULL);//�׳��쳣
//***********************************************	
	//װ��socket��
	if (!pDlg->InItSocket())
	{
		AfxMessageBox("װ���׽��ֿ����");
		return FALSE;
	}
	//
	pDlg->sSever=socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); //�����׽���
    if (INVALID_SOCKET==pDlg->sSever)
    {
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		return FALSE;
    }
	//
	sockaddr_in serve;
	serve.sin_family=AF_INET;
	serve.sin_addr.s_addr = INADDR_ANY;
	serve.sin_port=htons(1125);
	if (SOCKET_ERROR==bind( pDlg->sSever,(sockaddr*)&serve,sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("�󶨶˿�ʧ�ܣ�"));
		goto __Error_End;
	}
	//
	if (SOCKET_ERROR==listen(pDlg->sSever,5))
    {
		AfxMessageBox(_T("����ʧ�ܣ�"));
		goto __Error_End;
    }
	//***************************
	pDlg->SetDlgItemText(IDC_EDIT1,"����˿����ɹ�!");

	CreateThread(NULL, 0, WorkerThread,pParam, 0, NULL); //�������߳�

	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		pDlg->sCilent=accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);//��������
		//�������ȡ�ÿͻ��˵�IP
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//����IP
		CString cstr;
		cstr.Format("IP:%s�������ˣ�",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);//����ʾ
		pDlg->CliSocketArr[pDlg->TotalConn].clientsocket=pDlg->sCilent;
		pDlg->CliSocketArr[pDlg->TotalConn].clientIP=cilentIPstr;
        pDlg->clientevent[pDlg->TotalConn]=WSACreateEvent();//����һ���¼����󲢱��浽����
                                                            //��Ҫ����������� WSAWaitForMultipleEvents
		//��Ҫ���� �¼�ѡ��
        WSAEventSelect(pDlg->CliSocketArr[pDlg->TotalConn].clientsocket,
                   pDlg->clientevent[pDlg->TotalConn],
                   FD_READ | FD_CLOSE);//��������������

		pDlg->TotalConn++;
	}

__Error_End:
	closesocket(pDlg->sSever);

	return TRUE;
}
DWORD WINAPI WorkerThread(LPVOID lpParameter)//�������߳�
{	
	CMyDlg *pDlg=(CMyDlg *)lpParameter;//ȡ���������Ĵ���ָ��
	
	int  ret, index;
	WSANETWORKEVENTS NetworkEvents;//�¼�����
	char    szMessage[MAX_BUF_SIZE];
	while (TRUE)
	{
		//�ؼ�API
		ret = WSAWaitForMultipleEvents(pDlg->TotalConn, pDlg->clientevent, FALSE, 200, FALSE);
		
		if (ret==WSA_WAIT_FAILED || ret==WSA_WAIT_TIMEOUT)
		{
			continue;//�������ֵ�Ǵ�����ǳ�ʱ����ô����
		}

		index = ret - WSA_WAIT_EVENT_0;//ȡ�������¼��Ķ�Ӧ��������ʵ���������±�
		//�ؼ�API
		WSAEnumNetworkEvents(pDlg->CliSocketArr[index].clientsocket, pDlg->clientevent[index], 
			&NetworkEvents);//�����ʲô�¼�

		if (NetworkEvents.lNetworkEvents == FD_READ)//ȡ��FD_READ�ķ���
		{
			// Receive message from client�ӿͻ��˽�����Ϣ
			//�ɼ���һЩ�������¼�����������ļ��������ļ�
			ret = recv(pDlg->CliSocketArr[index].clientsocket, szMessage, MAX_BUF_SIZE, 0);//����
			szMessage[ret] = '\0';
			CString msgstr;
			msgstr.Format("%s:%s",pDlg->CliSocketArr[index].clientIP,szMessage);
			
			pDlg->SetDlgItemText(IDC_EDIT1,msgstr);		
			if (ret == 0 || (ret==SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
			{
				pDlg->Cleanup(index);//����\�˳��Ĵ�������
			}
			else
			{				
				for (int i=0;i < pDlg->TotalConn; i++)//�ѽ��յ�����Ϣ������ͻ��˷���
				{
					send(pDlg->CliSocketArr[i].clientsocket, msgstr, strlen(msgstr), 0);//���ؿͻ���һ������Ϣ
				}
			}
		}
		if (NetworkEvents.lNetworkEvents==FD_CLOSE)//�ͻ��˹ر�
		{
			CString tempstr;
			tempstr.Format("IP��%s�뿪��",pDlg->CliSocketArr[index].clientIP);
			pDlg->SetDlgItemText(IDC_EDIT1,tempstr);	
			pDlg->Cleanup(index);
		}
	}
  return TRUE;
}
void CMyDlg::Cleanup(int index)//�رմ���
{
  closesocket(CliSocketArr[index].clientsocket);//�رն�Ӧ�����±���׽���
  WSACloseEvent(clientevent[index]);//�رն�Ӧ���¼�����
  if (TotalConn==1)
  {
	  TotalConn--;
  }else
  {
	  CliSocketArr[index]=CliSocketArr[TotalConn-1];
	  TotalConn--;
  }
}

void CMyDlg::OnButton2() //����
{
	if (TotalConn!=0)//���� ��ʵ��������±�
	{
		CString strMsgt;
		GetDlgItemText(IDC_EDIT2,strMsgt);
		SetDlgItemText(IDC_EDIT2,"");
		CString strMsg;
        strMsg.Format("Sever:%s",strMsgt);
        for (int i=0; i < TotalConn; i++)//�������е������׽��ַ���
		{
			send(CliSocketArr[i].clientsocket, strMsg, strlen(strMsg), 0);
		}		
	}
}
