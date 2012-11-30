// ��ɶ˿ڷ����Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "��ɶ˿ڷ����.h"
#include "��ɶ˿ڷ����Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MSGSIZE 1024

typedef enum//ö�� ״̬
{
  RECV_POSTED
}OPERATION_TYPE;

typedef struct//��չ�ص��ṹ
{
 WSAOVERLAPPED  overlap;
 WSABUF         Buffer;
 char           szMessage[MSGSIZE];
 DWORD          NumberOfBytesRecvd;
 DWORD          Flags;
 OPERATION_TYPE OperationType;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

struct CompletionPortAndMyHwnd 
{//Ŀ�����ڹ����߳��еõ�����ָ��
	HANDLE CompletionPort;
	CMyDlg* my_win;
}compleportandHwnd;

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
//*********************************************************************************
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

void CMyDlg::OnButton1() 
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	//�˴�this�Ǵ���ȥ�ı�����ָ�� ����ȡһЩֵ���ǿ��ƿؼ�		
}
DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CMyDlg *pDlg=(CMyDlg *)pParam;//ȡ���������Ĵ���ָ��
	ASSERT(pDlg != NULL);//�׳��쳣
	//***********************************************
	
	DWORD    dwThreadId;
	int    iaddrSize = sizeof(SOCKADDR_IN);//SOCKADDR_IN�ṹ��С
	//HANDLE  CompletionPort = INVALID_HANDLE_VALUE;//��ɶ˿�
	SYSTEM_INFO     systeminfo;//ϵ����Ϣ����Ҫ������ȡ��CPU����
	LPPER_IO_OPERATION_DATA lpPerIOData = NULL;//��չ�ص��ṹ
	
	//����һ����ɶ˿ڣ�����ֻ��Ϊ�˵õ����ľ���������ٺ��׽��ֹ�������
	compleportandHwnd.CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	compleportandHwnd.my_win=pDlg;//���������߳��еĴ���ָ��
		//�����������߳�
	GetSystemInfo(&systeminfo);
	for (unsigned int i=0;i<systeminfo.dwNumberOfProcessors;i++)//��CPU������������
	{
		CreateThread(NULL, 0, WorkerThread, &compleportandHwnd, 0, &dwThreadId);
		//����ɶ˿ھ������ȥ�����˴���ָ��
	}	
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
	
	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		//��������
		pDlg->sClient = accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//����IP
		CString cstr;
		cstr.Format("IP:%s�������ˣ�",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);//����ʾ  
		//���浽��̬����
		pDlg->cilentin.cinsock=pDlg->sClient;//�׽���
        pDlg->cilentin.cinIP= cilentIPstr;
		pDlg->m_Cilentinfo.Add(pDlg->cilentin);	//���붯̬������
		
		//�½��ܵ������׽��ֹ�������ɶ˿�
		CreateIoCompletionPort((HANDLE)pDlg->sClient,compleportandHwnd.CompletionPort, (DWORD)pDlg->sClient, 0);
		//����һ��WSARecv�첽����
		lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			sizeof(PER_IO_OPERATION_DATA));//�ص��ṹ�����ڴ�
		lpPerIOData->Buffer.len = MSGSIZE;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
		lpPerIOData->OperationType = RECV_POSTED;
		WSARecv(pDlg->sClient,
			&lpPerIOData->Buffer,
			1,
			&lpPerIOData->NumberOfBytesRecvd,
			&lpPerIOData->Flags,
			&lpPerIOData->overlap,
			NULL);
	}
	
__Error_End:
	//����ɶ˿��е��̷߳����˳��ź�
	PostQueuedCompletionStatus(compleportandHwnd.CompletionPort, 0xFFFFFFFF, 0, NULL);
	closesocket(pDlg->sSever);
	CloseHandle(compleportandHwnd.CompletionPort);//�ر���ɶ˿�
	WSACleanup();//��ж�׽���
	return TRUE;
}
//
DWORD WINAPI WorkerThread(LPVOID CompletionPortID)//�������߳�
{
  HANDLE CompletionPort=(HANDLE)compleportandHwnd.CompletionPort;//ȡ����ɶ˿ھ�� 
  CMyDlg* pDlg=compleportandHwnd.my_win;//ȡ������ָ��
  SOCKET   sClient;
  DWORD  dwBytesTransferred;//�ص��ṹ�е��ֽ���  
  LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

  while (TRUE)
  {
	  //ȡ��ɶ˿�״̬
	  GetQueuedCompletionStatus(CompletionPort,&dwBytesTransferred,(unsigned long *)&sClient,
		  (LPOVERLAPPED *)&lpPerIOData,INFINITE);
	  int i=0;
	  if (dwBytesTransferred == 0xFFFFFFFF)//������˳����ź�,��PostQueuedCompletionStatus������
	  {		  
		  return TRUE;//����
	  }
	  if (lpPerIOData->OperationType == RECV_POSTED)//�ص��ṹ�е�ö��ֵ
	  {
		  if (dwBytesTransferred == 0)
		  {
			  //�ͻ��˹ر�			  
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//���׽���Ϊ�꣬�ҳ���̬������ӦIP
			  {
				  if (sClient==pDlg->m_Cilentinfo.GetAt(i).cinsock)
				  {
					  break;
				  }
			  }          
			  CString cip;
			  cip.Format("�ͻ�%s�뿪",pDlg->m_Cilentinfo.GetAt(i).cinIP);
			  pDlg->m_Cilentinfo.RemoveAt(i);//�Ӷ�̬������ɾ����� �ڵ�
 			  pDlg->SetDlgItemText(IDC_EDIT1,cip);

			  closesocket(sClient);//�رտͻ����׽���
			  HeapFree(GetProcessHeap(),0,lpPerIOData); //�ͷ���ǰ������ڴ�       
		  }else
		  {
			  //**********************************************************************
			  //����ͻ�������,��������߳�ʲô�����ڴ��ļ�֮���
			  lpPerIOData->szMessage[dwBytesTransferred] = '\0';//�ͻ��˷���������Ϣ
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//���׽���Ϊ�꣬�ҳ���̬������ӦIP
			  {
				  if (sClient==pDlg->m_Cilentinfo.GetAt(i).cinsock)
				  {
					  break;
				  }
			  }         
			  CString sMsg;
			  sMsg.Format("%s:%s",pDlg->m_Cilentinfo.GetAt(i).cinIP,lpPerIOData->szMessage);
			  pDlg->SetDlgItemText(IDC_EDIT1,sMsg);
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//�ش������ж�̬�����еĿͻ�
			  {
				  send(pDlg->m_Cilentinfo.GetAt(i).cinsock, sMsg, strlen(sMsg), 0);
			  }				  
			  //**********************************************************************  
			  
			  //���¼�����һ��WSARecv�첽����
			  memset(lpPerIOData, 0, sizeof(PER_IO_OPERATION_DATA));
			  lpPerIOData->Buffer.len = MSGSIZE;
			  lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
			  lpPerIOData->OperationType = RECV_POSTED;
			  WSARecv(sClient,
				  &lpPerIOData->Buffer,
				  1,
				  &lpPerIOData->NumberOfBytesRecvd,
				  &lpPerIOData->Flags,
				  &lpPerIOData->overlap,
				  NULL);
		  }
	}
  }
  return TRUE;
 }

void CMyDlg::OnButton2() //����
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
