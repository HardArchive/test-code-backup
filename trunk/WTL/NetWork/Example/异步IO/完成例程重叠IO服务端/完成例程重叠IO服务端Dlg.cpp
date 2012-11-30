// ��������ص�IO�����Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "��������ص�IO�����.h"
#include "��������ص�IO�����Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct//��չ�ص��ṹ β������
{
	WSAOVERLAPPED overlap;//ԭ�ص��ṹ	
	WSABUF        Buffer;
	char          szMessage[MAX_BUF_SIZE];//���ݽ��ջ���
	DWORD         NumberOfBytesRecvd;
	DWORD         Flags; 
	SOCKET        sClient;
	CIODlg *      my_win;	//����ָ��
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;
SOCKET g_sNewClientConnection;
BOOL   g_bNewConnectionArrived;//�����ӵ���

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
// CIODlg dialog

CIODlg::CIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIODlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIODlg, CDialog)
	//{{AFX_MSG_MAP(CIODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIODlg message handlers

BOOL CIODlg::OnInitDialog()
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
	g_bNewConnectionArrived=FALSE;
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIODlg::OnPaint() 
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
HCURSOR CIODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//********************************************************************************
BOOL CIODlg::InItSocket()//װ���׽��ֿ�
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
void CIODlg::OnButton2() //���������
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	//�˴�this�Ǵ���ȥ�ı�����ָ�� ����ȡһЩֵ���ǿ��ƿؼ�	
}

DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CIODlg *pDlg=(CIODlg *)pParam;//ȡ���������Ĵ���ָ��
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

	CreateThread(NULL, 0, WorkerThread,pParam, 0, NULL); //�������߳� ������ָ��
	
	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		//��������
		g_sNewClientConnection = accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);		
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//����IP

        pDlg->cilentin.cinsock=g_sNewClientConnection;//���붯̬������
		pDlg->cilentin.cinIP=cilentIPstr;
		pDlg->m_Cilentinfo.Add(pDlg->cilentin);	
		
		g_bNewConnectionArrived=TRUE;//���µ�����,��״̬

		CString cstr;
		cstr.Format("IP:%s�������ˣ�",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);//����ʾ   
	}
	
__Error_End:
	closesocket(pDlg->sSever);
	return TRUE;
}
DWORD WINAPI WorkerThread(LPVOID lpParameter)
{
	CIODlg *pDlg=(CIODlg *)lpParameter;//ȡ���������Ĵ���ָ��
	
	LPPER_IO_OPERATION_DATA lpPerIOData = NULL;
    while (TRUE)//����ѭ��
	{
		if (g_bNewConnectionArrived)//����������Ϊ��ʱ����� ����һ��������ʱ
		{
			lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				sizeof(PER_IO_OPERATION_DATA));//�����ص��ṹ���ڴ�

			lpPerIOData->Buffer.len=MAX_BUF_SIZE;
			lpPerIOData->Buffer.buf=lpPerIOData->szMessage;
			lpPerIOData->sClient=g_sNewClientConnection;//�������׽��ַŵ��ص��ṹ��
			lpPerIOData->my_win=pDlg;//������ָ��		
						
			//�ؼ� WSARecv���׽��ֺ�������̹�������			
			WSARecv(lpPerIOData->sClient,
				&lpPerIOData->Buffer,
				1,
				&lpPerIOData->NumberOfBytesRecvd,
				&lpPerIOData->Flags,
				&lpPerIOData->overlap,
				CompletionROUTINE); 	
			
			g_bNewConnectionArrived= FALSE;//����������Ϊ���Խ�����һ��          
			
		}
		SleepEx(1000,TRUE);//�������ʱ����״̬		
	}	
	return TRUE;
}

//�������
void CALLBACK CompletionROUTINE(DWORD dwError,DWORD cbTransferred,LPWSAOVERLAPPED lpOverlapped,DWORD dwFlags)
{
	LPPER_IO_OPERATION_DATA lpPerIOData=(LPPER_IO_OPERATION_DATA)lpOverlapped;//ȡ���������Ľṹ
	int i=0;

	if (dwError != 0 || cbTransferred == 0)
	{
		// Connection was closed by client
		//�ͻ��˹ر��׽���
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//���׽���Ϊ�꣬�ҳ���̬������ӦIP
		{
			if (lpPerIOData->sClient==lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock)
			{
				break;
			}
		}          
		CString cip;
	    cip.Format("�ͻ�%s�뿪",lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinIP);
		lpPerIOData->my_win->m_Cilentinfo.RemoveAt(i);//�Ӷ�̬������ɾ����� �ڵ�		
		lpPerIOData->my_win->SetDlgItemText(IDC_EDIT1,cip);
		
		closesocket(lpPerIOData->sClient);//�ر�����׽���
		HeapFree(GetProcessHeap(), 0, lpPerIOData);//�ͷ��������չ�ص��ṹ�ڴ�
	}
	else
	{		
		//********************************************************************
		//������ͻ�����Ϣ ���������￪��һ���̣߳��������ڴ��ļ�
		lpPerIOData->szMessage[cbTransferred] = '\0';//���յ�����Ϣ
			
		//���ظ��ͻ���
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//���׽���Ϊ�꣬�ҳ���̬������ӦIP
		{
			if (lpPerIOData->sClient==lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock)
			{
				break;
			}
		}   
		CString sMsg;
		sMsg.Format("%s:%s",lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinIP,lpPerIOData->szMessage);
		lpPerIOData->my_win->SetDlgItemText(IDC_EDIT1,sMsg);
		
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//�ش������ж�̬�����еĿͻ�
		{
			send(lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock, sMsg, strlen(sMsg), 0);
		}		
		//********************************************************************    
		// Launch another asynchronous operation
		//���¼�����һ��WSARecv�첽����
		//����һ����Ϣ
		memset(&lpPerIOData->overlap, 0, sizeof(WSAOVERLAPPED));//���overlap����
		lpPerIOData->Buffer.len = MAX_BUF_SIZE;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;    
		WSARecv(lpPerIOData->sClient,
			&lpPerIOData->Buffer,
			1,
			&lpPerIOData->NumberOfBytesRecvd,
			&lpPerIOData->Flags,
			&lpPerIOData->overlap,
			CompletionROUTINE);
	}	
}

void CIODlg::OnButton1() //����
{
	if (m_Cilentinfo.GetSize()>0)
	{		
		CString strMsgt;
		GetDlgItemText(IDC_EDIT2,strMsgt);
		SetDlgItemText(IDC_EDIT2,"");
		CString strMsg;
		strMsg.Format("Sever:%s",strMsgt);
		for (int i=0;i<m_Cilentinfo.GetSize();i++)//����̬�����е��׽���ÿ����
		{
			send(m_Cilentinfo.GetAt(i).cinsock, strMsg, strlen(strMsg), 0);
		}
	}
}
