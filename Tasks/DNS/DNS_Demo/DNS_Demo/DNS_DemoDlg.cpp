
// DNS_DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DNS_Demo.h"
#include "DNS_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "DNS.h"
#pragma comment(lib, "DNS.lib")
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDNS_DemoDlg 对话框




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


// CDNS_DemoDlg 消息处理程序

BOOL CDNS_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_EDIT_DOMAIN, "www.9cpp.com");
	SetDlgItemText(IDC_IPADDRESS, "112.126.107.188");
	InitDNS();     //使用前先初始化

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDNS_DemoDlg::OnPaint()
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
HCURSOR CDNS_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//验证DNS域名
void CDNS_DemoDlg::OnBnClickedButtonVerify()
{
	// TODO: 在此添加控件通知处理程序代码
	char szIP[16] = {0};
	char szDomian[MAX_PATH] = {0};

	GetDlgItemText(IDC_EDIT_DOMAIN, szDomian, MAX_PATH);
	GetDlgItemText(IDC_IPADDRESS, szIP, 16);
	if(1 != DNSVerify(szDomian, szIP))
	{
		::MessageBox(NULL, "DNS验证失败！！！", szDomian, MB_OK);
		return;
	}	
	::MessageBox(NULL, "DNS验证成功！！！", szDomian, MB_OK);

}

void CDNS_DemoDlg::OnBnClickedButtonContorl()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool bIsStart = false;    //为false时关闭
	if (!bIsStart)
	{
		bIsStart = true;		
		SetDlgItemText(IDC_BUTTON_CONTORL, "停止拦截");
	}
	else
	{
		bIsStart = false;		
		SetDlgItemText(IDC_BUTTON_CONTORL, "开始拦截");
	}
	CapPacketDNS(bIsStart);
}
