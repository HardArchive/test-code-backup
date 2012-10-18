   // SimpleScannerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleScanner.h"
#include "SimpleScannerDlg.h"
#include "Ping.h"
#include "Result.h"
#include "ConnectPort.h"
#include "TcpScan.h"
//#include "IcmpAPI.h"
//#include "IPExport.h"

//the struct of the result infomation for each host



typedef struct _scanresult {
	CString strHost;
	long    *existTcpPort;
	long    *existUdpPort;	
	_scanresult();
	~_scanresult();
}SCANRESULT;
_scanresult::_scanresult(){
	existTcpPort=NULL;
	existUdpPort=NULL;
}
_scanresult::~_scanresult(){
	if(existTcpPort!=NULL){
		delete []existTcpPort;
		existTcpPort=NULL;
	}
	if(existUdpPort!=NULL){
		delete []existUdpPort;
		existUdpPort=NULL;
	}
}
SCANRESULT *g_ScanPortInfo;

BOOL bAllScaned = false; //是否扫描已停止
CCriticalSection g_ScanPortInfoCs;
CEvent g_Event(FALSE,TRUE); //control the m_strIP
CEvent g_thEvent(FALSE,TRUE); //Control the Tcp tread infomation
CEvent g_thUdpEvent(FALSE,TRUE);//Control the Udp tread infomation
int ScanFlag;
int hostNum = 0;
int hostScaned = 0; //the number of hosts that has ping 
CCriticalSection g_hostScanedcs;

CString strExistHost[256]; //the list of the exist hosts

CCriticalSection g_cs;
CCriticalSection g_TCPNumcs;
CCriticalSection g_UDPNumcs;
CCriticalSection g_TCPPortListCs;
CCriticalSection g_UDPPortListCs;
CCriticalSection g_totalScanedCs;
int MaxTcpPort = 0;
int MaxUdpPort = 0;
int totalPortNum = 0;
long ExistTcpNum = 0; //the number of open tcp ports 
long ExistUdpNum = 0; // the number of open udp ports 
long TcpPortList[65536]; //The tcp Port list
long UdpPortList[65536]; //The udp port list 
int TotalPortScanned = 0;
int PortNow; //正在扫描的端口
CCriticalSection g_PortNowCs;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CSimpleScannerDlg dialog

CSimpleScannerDlg::CSimpleScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleScannerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimpleScannerDlg)
	m_nHostSel = 0;
	m_strIP =_T("127.0.0.1");
	m_strTo = _T("");
	m_strFrom = _T("");
	m_IHostNum = 0;
	m_iTcpNum = 0;
	m_iUdpNum = 0;
	m_nScanMode = 0;
	m_bTcpPort = FALSE;
	m_bUdpPort = FALSE;
	m_lTcpFrom = 0;
	m_lTcpTo = 0;
	m_lUdpFrom = 0;
	m_lUdpTo = 0;
	in_TCPthread=NULL;
	in_UdpThread=NULL;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimpleScannerDlg)
	DDX_Control(pDX, IDC_UDPLIST, m_ctrUdpList);
	DDX_Control(pDX, IDC_TCPLIST, m_ctrTcpList);
	DDX_Radio(pDX, IDC_RHOSTSEL, m_nHostSel);
	DDX_Text(pDX, IDC_EIP, m_strIP);
	DDX_Text(pDX, IDC_ETO, m_strTo);
	DDX_Text(pDX, IDC_EFROM, m_strFrom);
	DDX_Text(pDX, IDC_ETCPNUM, m_iTcpNum);
	DDX_Text(pDX, IDC_EUDPNUM, m_iUdpNum);
	DDX_Radio(pDX, IDC_RSCANMODE, m_nScanMode);
	DDX_Check(pDX, IDC_CTCPPORT, m_bTcpPort);
	DDX_Check(pDX, IDC_CUDPPORT, m_bUdpPort);
	DDX_Text(pDX, IDC_ETCPFROM, m_lTcpFrom);
	DDV_MinMaxLong(pDX, m_lTcpFrom, 0, 65535);
	DDX_Text(pDX, IDC_ETCPTO, m_lTcpTo);
	DDV_MinMaxLong(pDX, m_lTcpTo, 0, 65535);
	DDX_Text(pDX, IDC_EUDPFROM, m_lUdpFrom);
	DDV_MinMaxLong(pDX, m_lUdpFrom, 0, 65535);
	DDX_Text(pDX, IDC_EUDPTO, m_lUdpTo);
	DDV_MinMaxLong(pDX, m_lUdpTo, 0, 65535);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimpleScannerDlg, CDialog)
	//{{AFX_MSG_MAP(CSimpleScannerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RHOSTSEL, OnRhostsel)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BPing, OnBPing)
	ON_EN_KILLFOCUS(IDC_EIP, OnKillfocusEip)
	ON_EN_KILLFOCUS(IDC_EFROM, OnKillfocusEfrom)
	ON_EN_KILLFOCUS(IDC_ETO, OnKillfocusEto)
	ON_BN_CLICKED(IDC_BDETAILRESULT, OnBdetailresult)
	ON_BN_CLICKED(IDC_CTCPPORT, OnCtcpport)
	ON_BN_CLICKED(IDC_CUDPPORT, OnCudpport)
	ON_BN_CLICKED(IDC_BADDTCPPORT, OnBaddtcpport)
	ON_BN_CLICKED(IDC_BADDUDPPORT, OnBaddudpport)
	ON_BN_CLICKED(IDC_BDELTCPPORT, OnBdeltcpport)
	ON_BN_CLICKED(IDC_BDELUDPPORT, OnBdeludpport)
	ON_BN_CLICKED(IDC_BSTARTSCAN, OnBstartscan)
	ON_BN_CLICKED(IDC_BSTOPSCAN, OnBstopscan)
	ON_BN_CLICKED(IDC_BVIEWRESULT, OnBviewresult)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimpleScannerDlg message handlers

BOOL CSimpleScannerDlg::OnInitDialog()
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
	//GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	GetDlgItem(IDC_EFROM)->EnableWindow(false);
	GetDlgItem(IDC_ETO)->EnableWindow(false);
	GetDlgItem(IDC_BDETAILRESULT)->EnableWindow(false);
	//make the TCP port /Udp port config be unable 
    GetDlgItem(IDC_ETCPFROM)->EnableWindow(false);
	
	GetDlgItem(IDC_ETCPTO)->EnableWindow(false);
	GetDlgItem(IDC_EUDPFROM)->EnableWindow(false);
	GetDlgItem(IDC_EUDPTO)->EnableWindow(false);
	GetDlgItem(IDC_BADDTCPPORT)->EnableWindow(false);
	GetDlgItem(IDC_BADDUDPPORT)->EnableWindow(false);
	GetDlgItem(IDC_BDELTCPPORT)->EnableWindow(false);
	GetDlgItem(IDC_BDELUDPPORT)->EnableWindow(false);
	GetDlgItem(IDC_TCPLIST)->EnableWindow(false);
	GetDlgItem(IDC_UDPLIST)->EnableWindow(false);
	GetDlgItem(IDC_BSTOPSCAN)->EnableWindow(false);
	GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(false);
	//*(g_ScanPortInfo->existTcpPort)=52;
	InitializeCriticalSection(&cs);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimpleScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimpleScannerDlg::OnPaint() 
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
HCURSOR CSimpleScannerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



CCriticalSection CriticalSection;//建立临界区
//ping the remote hosts, and save the result 
UINT PingThread(LPVOID lpParam)
{
	CSimpleScannerDlg* scanDlg = (CSimpleScannerDlg*)lpParam;
	
	CString strHost = scanDlg->m_strIP;
	g_Event.SetEvent(); //得到参数后解锁

	CPing tempPing;
    for(int i = 0; i< 10; i++)
    {
  
     if(scanDlg->IfHostIn(strHost)&&tempPing.HostReachable(strHost))
	 {
       CString str;
       g_cs.Lock();	     	  
	   strExistHost[hostNum++] = strHost;
	   str.Format("%d",hostNum);	   
	   g_cs.Unlock();
	 }
     
	 g_hostScanedcs.Lock();
	 hostScaned ++;
	 if(hostScaned == scanDlg->GetTotalHost()) //all the hosted been scanned 
	 {      
	  scanDlg->GetDlgItem(IDC_BVIEWRESULT)->EnableWindow(true); 
	  scanDlg->GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(true);
	 }
	
	 g_hostScanedcs.Unlock(); 
	 
	 if(strHost.Compare(scanDlg->m_strTo)==0)
	 {        
		break;
	 }
	 
	 strHost = scanDlg->GetNextHost(strHost,1);
	 Sleep(50);	 
	}
	return 0;
}

// select the single host scan 
void CSimpleScannerDlg::OnRhostsel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_nHostSel==0)
	{
    m_strIP ="127.0.0.1";
	UpdateData(false);
//	GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	GetDlgItem(IDC_EFROM)->EnableWindow(false);
	GetDlgItem(IDC_ETO)->EnableWindow(false);
   
	GetDlgItem(IDC_RHOSTSEL)->EnableWindow(true);
	GetDlgItem(IDC_EIP)->EnableWindow(true);
	
	}
	else
	{
		//GetDlgItem(IDC_RHOSTSEL)->EnableWindow(false);
		GetDlgItem(IDC_EIP)->EnableWindow(false);

        GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	    GetDlgItem(IDC_EFROM)->EnableWindow(true);
	    GetDlgItem(IDC_ETO)->EnableWindow(true);
	}
	 GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(false);
}

//select the mutiple hosts scan 
void CSimpleScannerDlg::OnRadio2() 
{
	UpdateData(true);
	if(m_nHostSel==0)
	{
//	GetDlgItem(IDC_RADIO2)->EnableWindow(false);
	GetDlgItem(IDC_EFROM)->EnableWindow(false);
	GetDlgItem(IDC_ETO)->EnableWindow(false);
   
	GetDlgItem(IDC_RHOSTSEL)->EnableWindow(true);
	GetDlgItem(IDC_EIP)->EnableWindow(true);
	
	}
	else
	{
		//GetDlgItem(IDC_RHOSTSEL)->EnableWindow(false);
		GetDlgItem(IDC_EIP)->EnableWindow(false);

        GetDlgItem(IDC_RADIO2)->EnableWindow(true);
	    GetDlgItem(IDC_EFROM)->EnableWindow(true);
	    GetDlgItem(IDC_ETO)->EnableWindow(true);
	}
	
	 GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(false);
	 //GetDlgItem(IDC_BSTOPSCAN)->EnableWindow(true);
}

//when the ping button pressed 
void CSimpleScannerDlg::OnBPing() 
{
	// TODO: Add your control notification handler code here
   UpdateData(true);
   CPing tempPing;
   GetDlgItem(IDC_BVIEWRESULT)->EnableWindow(false);

   
   m_IHostNum = 0;
   g_cs.Lock();
   hostNum = 0;
   g_cs.Unlock();
   if(m_nHostSel==0)
   {
	if(tempPing.HostReachable(m_strIP))
	{
		//m_IHostNum = 1;
		CString str;
		g_cs.Lock();
	    strExistHost[hostNum++] = m_strIP;
		str.Format("%d",hostNum);
		g_cs.Unlock();
		SetDlgItemText(IDC_EHOSTNUM,str);
		//CResult result;
		//result.WriteHeader(hostNum,ExistTcpNum,ExistUdpNum); 
	    GetDlgItem(IDC_BVIEWRESULT)->EnableWindow(true);
		GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(true);
	}
   }
   else 
   {  
	 
	 int threadNum = GetTotalHost()/10 + 1;
	 m_strIP = m_strFrom;  //the begin ip
	 //AfxMessageBox(m_strIP);
	 for(int i = 0; i < threadNum; i ++)
	 {
		 AfxBeginThread(PingThread,this);
		 g_Event.Lock(); //停止执行直到m_strIP传给线程
		 m_strIP = GetNextHost(m_strFrom,10*(i+1));
	 }    
	 
   }
  
   
   
}

// get the ip adress
void CSimpleScannerDlg::OnKillfocusEip() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
	GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(false);
}

//get the begin ip 
void CSimpleScannerDlg::OnKillfocusEfrom() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	
}

//get the end ip 
void CSimpleScannerDlg::OnKillfocusEto() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CompareIp(m_strFrom,m_strTo);
	
}

// compare two ip string 
void CSimpleScannerDlg::CompareIp(CString strFrom, CString strTo)
{
 CString FromLeft,FromRight;
 CString ToLeft,ToRight;
 FromLeft = strFrom.Left(strFrom.ReverseFind('.'));
 ToLeft = strTo.Left(strTo.ReverseFind('.'));
 //strFrom.TrimRight(FromRight);
 //strTo.TrimRight(ToRight);
 FromRight = strFrom.Right(strFrom.GetLength()-strFrom.ReverseFind('.')-1);
 ToRight = strTo.Right(strTo.GetLength()-strTo.ReverseFind('.')-1);
 //AfxMessageBox(FromRight);
 //AfxMessageBox(ToRight);
 //get the right section of the address 
 int from,to;
 from=atoi(FromRight);
 to = atoi(ToRight);
 if(ToLeft.Compare(FromLeft)!=0)
 {
	 AfxMessageBox("只提供同一个IP段的扫描，请重新设置");
	 return;
 }
 else if ( from > to )
 {
	 AfxMessageBox("终止地址小于起始地址，请重新设置");
	 return;   
 }

	 
}



//view the information in detail 
void CSimpleScannerDlg::OnBdetailresult() 
{
	// TODO: Add your control notification handler code here  
		CResult result;
	
		result.WriteHeader(hostNum,ExistTcpNum,ExistUdpNum);
	
		result.WriteString("<br><font size = 3>扫描结果的详细信息：<br>"); 
	

		//write the open ports to file 
	
		for( int i = 0; i < hostNum; i ++ )

		{
           result.WritePort(g_ScanPortInfo[i].strHost, g_ScanPortInfo[i].existTcpPort, m_iTcpPortNum,
	       g_ScanPortInfo[i].existUdpPort,m_iUdpPortNum);
		}	
		result.SignTime();
        ViewFile(); 
}

//get the total number of the scanned hosts 
int CSimpleScannerDlg::GetTotalHost()
{
  CString FromRight,ToRight;
  int from,to;
  //m_strFrom.TrimRight(FromRight);
  //m_strTo.TrimRight(ToRight);
  FromRight = m_strFrom.Right(m_strFrom.GetLength()-m_strFrom.ReverseFind('.')-1);
  ToRight = m_strTo.Right(m_strTo.GetLength()-m_strTo.ReverseFind('.')-1);

  from = atoi(FromRight);
  to = atoi(ToRight);
  CString str;
  str.Format("totel number %d",to - from + 1);
 // AfxMessageBox(str);
  return (to - from + 1);
}


//get the next host 
CString CSimpleScannerDlg::GetNextHost(CString strHost,int num)
{
	CString str,strRight;
	int rightval;
	str = strHost.Left(strHost.ReverseFind('.'));
	//strHost.TrimRight(strRight);
	strRight = strHost.Right(strHost.GetLength()-strHost.ReverseFind('.')-1);
    
	rightval = atoi(strRight);
	rightval+=num;

	CString temp;
	char buffer[20];
	temp = itoa(rightval,buffer,10);

	return str +"." + temp;

}

// judge if the host is in the arrange of hosts list 
BOOL CSimpleScannerDlg::IfHostIn(CString strHost)
{
	CString strTemp;
	int ip, maxIP;
   
	strTemp =  strHost.Right(strHost.GetLength()-strHost.ReverseFind('.')-1);
	ip = atoi(strTemp);
	
	//AfxMessageBox(strTemp);

	//get the max ip
	strTemp =  m_strTo.Right(m_strTo.GetLength()-m_strTo.ReverseFind('.')-1);
    maxIP = atoi(strTemp);
	
	//AfxMessageBox(strTemp);
   
	if(ip<=maxIP)
	{
		return true;
	}
    else
	{
		return false;
	}
}

// the TCP scan check box selected 
void CSimpleScannerDlg::OnCtcpport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bTcpPort)
	{
		GetDlgItem(IDC_ETCPFROM)->EnableWindow(true);
		GetDlgItem(IDC_ETCPTO)->EnableWindow(true);
		GetDlgItem(IDC_BADDTCPPORT)->EnableWindow(true);
		GetDlgItem(IDC_BDELTCPPORT)->EnableWindow(true);
		GetDlgItem(IDC_TCPLIST)->EnableWindow(true);


	}
	else
	{
        GetDlgItem(IDC_ETCPFROM)->EnableWindow(false);
		GetDlgItem(IDC_ETCPTO)->EnableWindow(false);
		GetDlgItem(IDC_BADDTCPPORT)->EnableWindow(false);
		GetDlgItem(IDC_BDELTCPPORT)->EnableWindow(false);
		GetDlgItem(IDC_TCPLIST)->EnableWindow(false);
    
	}
	
}

// the Udp scan check box selected 
void CSimpleScannerDlg::OnCudpport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_bUdpPort)
	{
		GetDlgItem(IDC_EUDPFROM)->EnableWindow(true);
		GetDlgItem(IDC_EUDPTO)->EnableWindow(true);
		GetDlgItem(IDC_BADDUDPPORT)->EnableWindow(true);
		GetDlgItem(IDC_BDELUDPPORT)->EnableWindow(true);
		GetDlgItem(IDC_UDPLIST)->EnableWindow(true);


	}
	else
	{
        GetDlgItem(IDC_EUDPFROM)->EnableWindow(false);
		GetDlgItem(IDC_EUDPTO)->EnableWindow(false);
		GetDlgItem(IDC_BADDUDPPORT)->EnableWindow(false);
		GetDlgItem(IDC_BDELUDPPORT)->EnableWindow(false);
		GetDlgItem(IDC_UDPLIST)->EnableWindow(false);
    
	}
	
}

//add tcp port/port list to the TCP list control 
void CSimpleScannerDlg::OnBaddtcpport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString str;
	if(m_lTcpFrom > m_lTcpTo)
	{
		AfxMessageBox("开始端口应小于结束端口!");
		return;
	}
    
	else if(m_lTcpFrom == m_lTcpTo)
	{
		if( !CheckAdd(&m_ctrTcpList,m_lTcpFrom,m_lTcpTo) )
		{
				AfxMessageBox("添加的端口与已有的端口有交叉，请重新设置");
				return;
		}
		
		InsertString(&m_ctrTcpList,m_lTcpFrom,m_lTcpTo); // insert new port list 
	}

	else if(m_lTcpFrom < m_lTcpTo)
	{
		if( !CheckAdd(&m_ctrTcpList,m_lTcpFrom,m_lTcpTo) )
		{
				AfxMessageBox("添加的端口与已有的端口有交叉，请重新设置");
				return;
		}
		InsertString(&m_ctrTcpList,m_lTcpFrom,m_lTcpTo); // insert new port list 
	}

	
}

// add the udp port/ports list to the udp list control 
void CSimpleScannerDlg::OnBaddudpport() 
{
	// TODO: Add your control notification handler code here
		UpdateData(true);
	CString str;
	if(m_lUdpFrom > m_lUdpTo)
	{
		AfxMessageBox("开始端口应小于结束端口!");
		return;
	}
    
	else if(m_lUdpFrom == m_lUdpTo)
	{
	   if( !CheckAdd(&m_ctrUdpList,m_lUdpFrom,m_lUdpTo) )
		{
				AfxMessageBox("添加的端口与已有的端口有交叉，请重新设置");
				return;
		}

		InsertString(&m_ctrUdpList,m_lUdpFrom,m_lUdpTo); //Insert the port list 
	}

	else if(m_lUdpFrom < m_lUdpTo)
	{
		 if( !CheckAdd(&m_ctrUdpList,m_lUdpFrom,m_lUdpTo) )
		{
				AfxMessageBox("添加的端口与已有的端口有交叉，请重新设置");
				return;
		}
		InsertString(&m_ctrUdpList,m_lUdpFrom,m_lUdpTo); //Insert the port list 
	}
}

//check the validity of the added port /ports list 
BOOL CSimpleScannerDlg::CheckAdd(CListBox *ctrListBox, long from, long to)
{
 long oldFrom, oldTo;
 int rowNum=0;
 for(int i = 0; i < ctrListBox->GetCount(); i ++)
 {
	 CString str;
	 ctrListBox->GetText(i,str);
	 if( str.Find('-')!= -1)
	 {
	   oldFrom = atol( str.Left(str.Find('-')) );
	   oldTo = atol( str.Right( str.GetLength() - str.ReverseFind('-') -1 ) );
	   if( !(to < oldFrom || from > oldTo)  )
	   {
		   return false;
		   break;
	   }
	   
	 }
	else
	{
		oldFrom = oldTo = atol(str);
		if(from == oldFrom || to == oldTo)
		{
			return false;
			break;
		}
	}
	 
 }
 return true;
}

//insert the port /ports list by the order 
int CSimpleScannerDlg::InsertString(CListBox *ctrListBox, long from, long to)
{
	//get the string to be inserted 
	CString str;
	int ret=0; //the return value 
	
	if(from == to)
	{
		str.Format("%ld",from);
	}
	else if (from < to )
	{
		str.Format("%ld-%ld",from,to);
	}
	
    int ItemNum = ctrListBox->GetCount();
	if(ItemNum==0)
	{
		ret = ctrListBox->AddString(str);
		return ret;
		
	}


	for( int i=0; i< ctrListBox->GetCount(); i ++)
	{
		CString strTemp; // the string in the listbox
		ctrListBox->GetText(i,strTemp);
		

		long oldFrom;
		if(strTemp.Find('-') == -1) // no '-' could be found 
		{
          oldFrom = atol(strTemp);
		}
		else
		{
			oldFrom = atol(strTemp.Left(strTemp.Find('-')));
		}
        
        if( from < oldFrom )
		{
			ret = ctrListBox->InsertString(i,str);
			return ret;
			break;
		}
	}
	ret = ctrListBox->InsertString(-1,str);
	return ret;
  
}

//delete the tcp selected tcp port 
void CSimpleScannerDlg::OnBdeltcpport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	DeleteSelItem(&m_ctrTcpList);
	
}

//delete the selected udp port / ports list 
void CSimpleScannerDlg::OnBdeludpport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	DeleteSelItem(&m_ctrUdpList);
}

//delete the selected item 
void CSimpleScannerDlg::DeleteSelItem(CListBox *ctrListBox)
{
  for(int i = 0; i < ctrListBox->GetCount(); i ++)
  {
	  if( ctrListBox->GetSel(i) > 0 )
	  {
		  ctrListBox->DeleteString(i);
		  break;
	  }
  }
}

//Get the Tcp port list, the return value is the total number of the ports
long CSimpleScannerDlg::GetTcpPortList()
{
	UpdateData(true);
	long num=0;
	if(!m_bTcpPort)
	{
		return 0;
	}
	for( int i = 0; i < m_ctrTcpList.GetCount(); i ++ )
	{
		CString str;
		m_ctrTcpList.GetText(i,str);
		long from, to;
		if( str.Find('-') == -1 )
		{
         from = to = atol(str);
		 TcpPortList[num++] = from;
		}
		else
		{
			from = atol( str.Left(str.Find('-') ) );
			to = atol( str.Right(str.GetLength() - str.ReverseFind('-') -1) );
			for( int k = from; k <= to; k++)
			{
				TcpPortList[num++] = k;
			}

		}
	}
	return num;
}


//Get the Udp port list, the return value is the total number of the ports
long CSimpleScannerDlg::GetUdpPortList()
{
	UpdateData(true);
    long num=0;
	if(!m_bUdpPort)
	{
		num = 0;
		return num;
	}
	for( int i = 0; i < m_ctrUdpList.GetCount(); i ++ )
	{
		CString str;
		m_ctrUdpList.GetText(i,str);
		long from, to;
		if( str.Find('-') == -1 )
		{
         from = to = atol(str);
		 UdpPortList[num++] = from;
		}
		else
		{
			from = atol( str.Left(str.Find('-') ) );
			to = atol( str.Right(str.GetLength() - str.ReverseFind('-') -1) );
			for( int k = from; k <= to; k++)
			{
				UdpPortList[num++] = k;
			}

		}
	}
	return num;
}

//Get the scan flag 
void CSimpleScannerDlg::GetScanFlag()
{
  UpdateData(true);
  m_nScanFlag = m_nScanMode - 1; 

}





//scan the TCP ports by tcp mode 
UINT TcpScanThread(LPVOID lpParam)
{   
   CString strDebug; //the string used for debugging 
   
   THREADINFO *in_Thread = (THREADINFO*)lpParam;   
   int     portIndex = in_Thread->portIndex;
   int     hostIndex = in_Thread->HostIndex;
   CSimpleScannerDlg *Dlg = in_Thread->dlg;
   g_thEvent.SetEvent(); //set the event manual 
  
   //Initiate the host and port
   g_TCPPortListCs.Lock();
   CString strHost = strExistHost[hostIndex];
   int port = TcpPortList[portIndex];
   g_TCPPortListCs.Unlock();
  

  CWnd* pMainWnd = (CWnd*)AfxGetApp()->GetMainWnd();
  
   CConnectPort ConPort;
    
  for( int num = 0; num < 10; num ++)
   {
	  
     if( ScanFlag == -1 )
	 {
	
		if( ConPort.PortConnect(strHost,port,0) )
		{
				
			// update the existTcp number 
			g_TCPNumcs.Lock();
			ExistTcpNum++;	
			CString strTemp;
			strTemp.Format("%d",ExistTcpNum);
			::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_ETCPNUM,strTemp);		
		   
			//如果用两个临界区变量，扫描不结束，不止到是什么原因

			// set the port to the result info
			//g_ScanPortInfoCs.Lock();
			g_ScanPortInfo[hostIndex].existTcpPort[portIndex] = port;
			g_ScanPortInfoCs.Unlock();
		//	g_TCPNumcs.Unlock();
			g_TCPNumcs.Unlock();
			


		}
       
	 }
	 

	 //Set the scan state information 
	 g_totalScanedCs.Lock();
	 TotalPortScanned ++;
	 g_totalScanedCs.Unlock();	

	 CString strScan;
	 strScan.Format("%d",TotalPortScanned);
	 ::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_STATIC_SCANDED,strScan);
	 
	 strScan.Format("%d",totalPortNum-TotalPortScanned);
     ::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_STATIC_REST,strScan);
	
	 if( (totalPortNum - TotalPortScanned) == 0 )
	 {
		  bAllScaned = true;		
		 
		 Dlg->SetUIState(0);
		  break;
	 }
	 if(bAllScaned)
	 {
		 break;
	 }
	


	 if( port == MaxTcpPort)//一个主机的端口列表扫描结束
	 {
		 if( hostIndex == hostNum-1 )
		{		 
		 break;
		}
		 else 
		 {
		 hostIndex ++;
		 g_TCPPortListCs.Lock();
		 strHost = strExistHost[hostIndex];
		 portIndex = 0;
		 port = TcpPortList[portIndex];
		 g_TCPPortListCs.Unlock();
		 }
	 }
	 else
	 {
		 g_TCPPortListCs.Lock();
		 portIndex ++;
		 port = TcpPortList[portIndex];
		 g_TCPPortListCs.Unlock();
	 }

	
      
	 Sleep(25);
   }
  

	return 0;
}


//Udp Scan Thread
//scan the udp PROTS 
UINT UdpScanThread(LPVOID lpParam)
{   	
   CString strDebug; //the string used for debugging  
   THREADINFO *in_Thread = (THREADINFO*)lpParam;   
   int     portIndex = in_Thread->portIndex;
   int     hostIndex = in_Thread->HostIndex;
   CSimpleScannerDlg *Dlg = in_Thread->dlg;
   g_thUdpEvent.SetEvent(); //set the event manual 
  
   //Initiate the host and port
   g_UDPPortListCs.Lock();
   CString strHost = strExistHost[hostIndex];
   int port = UdpPortList[portIndex];
   g_UDPPortListCs.Unlock();
  

  CWnd* pMainWnd = (CWnd*)AfxGetApp()->GetMainWnd();
  

   CConnectPort ConPort;
 
   
   
  for( int num = 0; num < 10; num ++)
   {
	  
     if( ScanFlag == -1 )
	 {
        
	
		if( ConPort.PortConnect(strHost,port,1) )
		{
				
			// update the existTcp number 
			g_UDPNumcs.Lock();
			ExistUdpNum++;	
			CString strTemp;
			strTemp.Format("%d",ExistUdpNum);
			::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_EUDPNUM,strTemp);		
		   
			//如果用两个临界区变量，扫描不结束，不止到是什么原因

			// set the port to the result info
			//g_ScanPortInfoCs.Lock();
			g_ScanPortInfo[hostIndex].existUdpPort[portIndex] = port;
			g_ScanPortInfoCs.Unlock();
		//	g_TCPNumcs.Unlock();
			g_UDPNumcs.Unlock();
		}
  	 }
	
	

	 //Set the scan state information 
	 g_totalScanedCs.Lock();
	 TotalPortScanned ++;
	 g_totalScanedCs.Unlock();	

	 CString strScan;
	 strScan.Format("%d",TotalPortScanned);
	 ::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_STATIC_SCANDED,strScan);
	 
	 strScan.Format("%d",totalPortNum-TotalPortScanned);
     ::SetDlgItemText(Dlg->GetSafeHwnd(),IDC_STATIC_REST,strScan);
	
	 if( (totalPortNum - TotalPortScanned) == 0 )
	 {		 
		  bAllScaned = true;		
	      Dlg->SetUIState(0);		      
		  break;
	 }
	 if(bAllScaned)
	 {
		 break;
	 }

	

	 if( port == MaxUdpPort)//一个主机的端口列表扫描结束
	 {
		 if( hostIndex == hostNum-1 )
		{		 
		 break;
		}
		 else 
		 {
		 hostIndex ++;
		 g_UDPPortListCs.Lock();
		 strHost = strExistHost[hostIndex];
		 portIndex = 0;
		 port = UdpPortList[portIndex];
		 g_UDPPortListCs.Unlock();
		 }
	 }
	 else
	 {
		 g_UDPPortListCs.Lock();
		 portIndex ++;
		 port = UdpPortList[portIndex];
		 g_UDPPortListCs.Unlock();
	 }

	
      
	 Sleep(25);
   }
   

	return 0;
}

// The event funtion of the "开始扫描" button to start the scan thread 
void CSimpleScannerDlg::OnBstartscan() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetScanFlag();
  
	ScanFlag = m_nScanFlag;
	CString strDebug;
	

    m_iTcpPortNum = GetTcpPortList();
    m_iUdpPortNum = GetUdpPortList();
	long TotalTcpPort = hostNum*(m_iTcpPortNum);
	long TotalUdpPort = hostNum*(m_iUdpPortNum);
	MaxTcpPort = TcpPortList[m_iTcpPortNum-1];
	MaxUdpPort = UdpPortList[m_iUdpPortNum-1];
  
	 //set the scan state 
	 ExistTcpNum = ExistUdpNum = 0;
	 totalPortNum = TotalTcpPort + TotalUdpPort;
	 if(totalPortNum == 0)
	 {
		 AfxMessageBox("请输入要扫描的端口！");
		 return;
	 }
	 TotalPortScanned = 0;
	 PortNow = 0;
	 bAllScaned = false;

	 CString strScan;
	 strScan.Format("%d",TotalPortScanned);
	 SetDlgItemText(IDC_STATIC_SCANDED,strScan);
	 strScan.Format("%d",totalPortNum);
     SetDlgItemText(IDC_STATIC_REST,strScan);
	 strScan.Format("%ld",ExistTcpNum);
	 SetDlgItemText(IDC_ETCPNUM,strScan);
	 strScan.Format("%ld",ExistUdpNum);
	 SetDlgItemText(IDC_EUDPNUM,strScan);


	 GetDlgItem(IDC_BDETAILRESULT)->EnableWindow(false);
	// GetDlgItem(IDC_BDETAILRESULT)->EnableWindow(false);

	 GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(false);
	 GetDlgItem(IDC_BSTOPSCAN)->EnableWindow(true);

    //initiate the scan result info 

	g_ScanPortInfo = new SCANRESULT[hostNum];
	//create the scan result 
	for(int i = 0; i < hostNum; i ++ )
	{
		g_ScanPortInfo[i].strHost = strExistHost[i];
		g_ScanPortInfo[i].existTcpPort = new long[m_iTcpPortNum];
		g_ScanPortInfo[i].existUdpPort = new long[m_iUdpPortNum];
	}

	//set all the port to -1
	for(int j = 0; j < hostNum; j ++ )
		 for ( int k = 0; k < m_iTcpPortNum; k ++ )
	{
			 g_ScanPortInfo[j].existTcpPort[k] = -1;
	}

    for ( int m = 0; m < hostNum; m ++ )
		for ( int n = 0; n < m_iUdpPortNum; n ++ )
		{
			g_ScanPortInfo[m].existUdpPort[n] = -1;
		}


    //set for the TCP scan
	int TcpScanThreadNum = TotalTcpPort/10 +1;
   in_TCPthread= new THREADINFO[TcpScanThreadNum];

	in_TCPthread[0].HostIndex = 0;
	in_TCPthread[0].portIndex = 0;
	in_TCPthread[0].dlg       = this;
		


  if( m_bTcpPort )
  {
	
	//start the TCP scan thread
	for( int i = 0; i < TcpScanThreadNum; i ++)
	{
		
      AfxBeginThread(TcpScanThread,&in_TCPthread[i]);
	 // g_thEvent.Lock(); //wait until the in_TCPthread has been passed into the function	 
     
	  int num = ( 10*(i+1) )/m_iTcpPortNum; //compute which host be Scaned
	  if( num >= hostNum)
	  {
		  break;
	  }
	  int portNum = 10*(i+1)%m_iTcpPortNum;// compute which port will be scaned
	 //这里才是问题的真正所在!
	  in_TCPthread[i+1].HostIndex = num;
	  in_TCPthread[i+1].portIndex = portNum;
	  in_TCPthread[i+1].dlg       = this;
	  
	}
  }


	if(m_bUdpPort)
	{
	 int UdpScanThreadNum = TotalUdpPort/10 +1;
	 in_UdpThread=new THREADINFO[UdpScanThreadNum];
	 in_UdpThread[0].HostIndex = 0;
	 in_UdpThread[0].portIndex = 0;
	 in_UdpThread[0].dlg = this;
	 
	//start the UDP scan thread

	
	for( int j = 0; j < UdpScanThreadNum; j ++)
	{
	  AfxBeginThread(UdpScanThread,&in_UdpThread[j]);
    
	  int num = ( 10*(j+1) )/m_iUdpPortNum; //compute which host be Scaned
	  if( num >= hostNum)
	  {
		  break;
	  }
	  int portNum = 10*(j+1)%m_iUdpPortNum;// compute which port will be scaned
	 
	  in_UdpThread[j+1].HostIndex = num;
	  in_UdpThread[j+1].portIndex = portNum;    
	  in_UdpThread[j+1].dlg		 = this;
	}
	}//end of if 


	
}

//event function of the "停止扫描" button, stop the scan threads 
void CSimpleScannerDlg::OnBstopscan() 
{
	// TODO: Add your control notification handler code here
	bAllScaned = true;
	GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(true);
	GetDlgItem(IDC_BSTOPSCAN)->EnableWindow(false);
	GetDlgItem(IDC_BDETAILRESULT)->EnableWindow(true);

	
}

//view the ping result in file way 
void CSimpleScannerDlg::OnBviewresult() 
{
	// TODO: Add your control notification handler code here
	CResult PingResult;
    PingResult.WritePingHeader(hostNum);
	for(int i = 0; i < hostNum; i ++)
	  {
		  CString strTemp;
		  if(i%2==0)
		  {
			 strTemp.Format("<font size= 3 color = red> %s  ",strExistHost[i]);
		  }
		  else 
		  {
             strTemp.Format("<font size= 3 color = green> %s  ",strExistHost[i]); 
		  }
		  
		  if((i+1)%5 == 0)
		  {
			  strTemp += "<br>";
		  }
		  PingResult.WriteString(strTemp);
	  }	
	  PingResult.SignTime();
	  ViewFile();
	
}

//start the ie to see the scan result 
void CSimpleScannerDlg::ViewFile()
{
 	CString sCommandLine;
	char cWindowDirectory[MAX_PATH];
	char cCommandLine[MAX_PATH];
	DWORD dwExitCode;
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {sizeof(si)};

	//get the windows directory
	GetWindowsDirectory(cWindowDirectory,MAX_PATH);
	
	//get the command line of the notepad
    char buffer[MAX_PATH];
	//get the current program name
	GetModuleFileName(NULL,buffer,MAX_PATH);
	CString sPath = CString(buffer);
	sPath = sPath.Left(sPath.ReverseFind('\\'));
    sPath = sPath+"\\ScanResult.htm";
	
	sCommandLine = "C:\\Program Files\\Internet Explorer\\IEXPLORE.EXE ";
	//get the commandline to view the result file
	sCommandLine +=sPath;
	::strcpy(cCommandLine,sCommandLine);

	//start the IE program
	BOOL ret = CreateProcess(NULL,cCommandLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

	if(ret)
	{
		//close the handle of the subprocess
		CloseHandle(pi.hThread);
		//wait the exit of the subprocess
    	//	WaitForSingleObject(pi.hProcess,dwExitCode);
		//close the subprocess
		CloseHandle(pi.hProcess);

	}
}

void CSimpleScannerDlg::SetUIState(DWORD dwState)
{
	EnterCriticalSection(&cs);
	this->GetDlgItem(IDC_BDETAILRESULT)->EnableWindow(true);
	this->GetDlgItem(IDC_BSTARTSCAN)->EnableWindow(true);
	LeaveCriticalSection(&cs);
}

//message funtion when the program exists, prevent the memeory leaking 
void CSimpleScannerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	DeleteCriticalSection(&cs);
	if(this->in_UdpThread!=NULL){
		delete []in_UdpThread;
		in_UdpThread=NULL;
	}
	if(this->in_TCPthread!=NULL){
		delete []in_TCPthread;
		in_TCPthread=NULL;
	}
	
	for(int i(0); i<hostNum; i++){
		if(g_ScanPortInfo[i].existTcpPort!=NULL){
			delete g_ScanPortInfo[i].existTcpPort;
			g_ScanPortInfo[i].existTcpPort=NULL;
		}
		if(g_ScanPortInfo[i].existUdpPort!=NULL){
			delete g_ScanPortInfo[i].existUdpPort;
			g_ScanPortInfo[i].existUdpPort=NULL;
		}
	}

	if(g_ScanPortInfo!=NULL){
		delete []g_ScanPortInfo;
		g_ScanPortInfo=NULL;
	}
}
