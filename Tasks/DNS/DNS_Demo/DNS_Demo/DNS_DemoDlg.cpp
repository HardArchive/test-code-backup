
// DNS_DemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DNS_Demo.h"
#include "DNS_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "DNS.h"
#pragma comment(lib, "DNS.lib")
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDNS_DemoDlg �Ի���




CDNS_DemoDlg::CDNS_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDNS_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDNS_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDNS_DemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_VERIFY, &CDNS_DemoDlg::OnBnClickedButtonVerify)
	ON_BN_CLICKED(IDC_BUTTON_CONTORL, &CDNS_DemoDlg::OnBnClickedButtonContorl)
END_MESSAGE_MAP()


// CDNS_DemoDlg ��Ϣ�������

BOOL CDNS_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText(IDC_EDIT_DOMAIN, "www.9cpp.com");
	SetDlgItemText(IDC_IPADDRESS, "112.126.107.188");
	InitDNS();     //ʹ��ǰ�ȳ�ʼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDNS_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDNS_DemoDlg::OnPaint()
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
HCURSOR CDNS_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��֤DNS����
void CDNS_DemoDlg::OnBnClickedButtonVerify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szIP[16] = {0};
	char szDomian[MAX_PATH] = {0};

	GetDlgItemText(IDC_EDIT_DOMAIN, szDomian, MAX_PATH);
	GetDlgItemText(IDC_IPADDRESS, szIP, 16);
	if(1 != DNSVerify(szDomian, szIP))
	{
		::MessageBox(NULL, "DNS��֤ʧ�ܣ�����", szDomian, MB_OK);
		return;
	}	
	::MessageBox(NULL, "DNS��֤�ɹ�������", szDomian, MB_OK);

}

void CDNS_DemoDlg::OnBnClickedButtonContorl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static bool bIsStart = false;    //Ϊfalseʱ�ر�
	if (!bIsStart)
	{
		bIsStart = true;		
		SetDlgItemText(IDC_BUTTON_CONTORL, "ֹͣ����");
	}
	else
	{
		bIsStart = false;		
		SetDlgItemText(IDC_BUTTON_CONTORL, "��ʼ����");
	}
	CapPacketDNS(bIsStart);
}
