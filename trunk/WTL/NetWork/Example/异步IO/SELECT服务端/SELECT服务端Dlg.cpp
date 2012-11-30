// SELECT�����Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SELECT�����.h"
#include "SELECT�����Dlg.h"



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
// CSELECTDlg dialog

CSELECTDlg::CSELECTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSELECTDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSELECTDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSELECTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSELECTDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSELECTDlg, CDialog)
	//{{AFX_MSG_MAP(CSELECTDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSELECTDlg message handlers

BOOL CSELECTDlg::OnInitDialog()
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

void CSELECTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSELECTDlg::OnPaint() 
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
HCURSOR CSELECTDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSELECTDlg::OnButton1() 
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	
		//�˴�this�Ǵ���ȥ�ı�����ָ��	
}

DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CSELECTDlg *pDlg=(CSELECTDlg *)pParam;//ȡ���������Ĵ���ָ��
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
	CreateThread(NULL, 0, WorkerThread,pParam, 0, NULL); 
	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		pDlg->sCilent=accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);
		//�������ȡ�ÿͻ��˵�IP
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//����IP
		CString cstr;
		cstr.Format("IP:%s�������ˣ�",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);
//�ŵ������У�Ȼ��ӹ������߳���ȥ�������        
		pDlg->CliSocketArr[pDlg->TotalConn].cliensocket=pDlg->sCilent;
		pDlg->CliSocketArr[pDlg->TotalConn].clientIP=cilentIPstr;
		pDlg->TotalConn++;
	}

__Error_End:
	closesocket(pDlg->sSever);
	return TRUE;
}
BOOL CSELECTDlg::InItSocket()//װ���׽��ֿ�
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
DWORD WINAPI WorkerThread(LPVOID lpParameter)//�������߳�
{	
	CSELECTDlg *pDlg=(CSELECTDlg *)lpParameter;//ȡ���������Ĵ���ָ��
	
	int i=0;
	fd_set         fdread;//�ṹ
	int            ret;
	struct timeval tv = {0, 200};//��ʱʱ�� SELECTģ�����õ���������200����
	char           szMessage[MAX_BUF_SIZE];//��Ϣ���飬��ʵ�Ͼ��Ǹ�������
	while (TRUE)
	{
		if (pDlg->TotalConn>0)
		{
			FD_ZERO(&fdread);//���fd_set�ṹ
			for (i=0;i<pDlg->TotalConn;i++)//�������׽���������ֵ���ŵ��ṹ���Ա�SELECT���
			{
				FD_SET(pDlg->CliSocketArr[i].cliensocket,&fdread);
			}
			//
			ret = select(0, &fdread, NULL, NULL, &tv);		
			if (ret == 0)
			{
				continue;
			}
			//************************************************
			for (i = 0; i < pDlg->TotalConn; i++)
			{			
				if (FD_ISSET(pDlg->CliSocketArr[i].cliensocket, &fdread))
				{
					// A read event happened on g_CliSocketArr[i]
					//һ���ɶ�����������׽�����
					ret = recv(pDlg->CliSocketArr[i].cliensocket, szMessage,MAX_BUF_SIZE, 0);
					//����������������
					szMessage[ret] = '\0';
					CString msgstr;
					msgstr.Format("%s:%s",pDlg->CliSocketArr[i].clientIP,szMessage);
					pDlg->SetDlgItemText(IDC_EDIT1,msgstr);				
					if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
					{
						// Client socket closed
						//�ͻ��˹ر�					
						closesocket(pDlg->CliSocketArr[i].cliensocket);//�ر�����׽���	
						if (pDlg->TotalConn==1)
						{
                            pDlg->TotalConn--;
						}else
						{
                             pDlg->CliSocketArr[i]=pDlg->CliSocketArr[pDlg->TotalConn-1];
                             pDlg->TotalConn--;
						}
					
					}
					else
					{
						// We received a message from client
						//�������û��������ô�ͽ��յ�һ���ͻ��˵���Ϣ
						szMessage[ret] = '\0';//���һλ��Ϊ'/0'						
						//����ÿ���ͻ���
						for (i = 0; i < pDlg->TotalConn; i++)
						{
							send(pDlg->CliSocketArr[i].cliensocket, msgstr, strlen(msgstr), 0);
						}					
					}
				}
			}
		}
	}
	return TRUE;
}

void CSELECTDlg::OnButton2() 
{
	if (TotalConn!=0)//�߳��Ƿ���
	{
		CString strMsgt;
		GetDlgItemText(IDC_EDIT2,strMsgt);
		SetDlgItemText(IDC_EDIT2,"");
		CString strMsg;
        strMsg.Format("Sever:%s",strMsgt);
        for (int i=0; i < TotalConn; i++)
		{
			send(CliSocketArr[i].cliensocket, strMsg, strlen(strMsg), 0);
		}		
	}	
	
}
