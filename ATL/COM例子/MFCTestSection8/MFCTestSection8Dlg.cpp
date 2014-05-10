// MFCTestSection8Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTestSection8.h"
#include "MFCTestSection8Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMFCTestSection8Dlg 对话框




CMFCTestSection8Dlg::CMFCTestSection8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection8Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSink = new CSink(this);
}

CMFCTestSection8Dlg::~CMFCTestSection8Dlg()
{
	m_pSink->Release();
}

void CMFCTestSection8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGINAX1, m_axLogin);
}

BEGIN_MESSAGE_MAP(CMFCTestSection8Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCTestSection8Dlg 消息处理程序

BOOL CMFCTestSection8Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	HRESULT hr = E_FAIL;

	//创建组件A
	hr = m_spPasswordSpObj.CoCreateInstance(CLSID_PasswordSpObj);

	if (SUCCEEDED(hr))
	{
		//设置允许验证连续失败的次数
		hr = m_spPasswordSpObj->put_ErrorCountAllowed(5);

		DWORD dw = 0;

		//注册事件
		hr = m_spPasswordSpObj.Advise(m_pSink, DIID__IPasswordSpObjEvents, &dw);
	}

	//设置需要非空判断
	m_axLogin.put_JudgeNull(TRUE);

	//设置组件B的界面显示文本
	m_axLogin.SetText(_T("用户名："), _T("密码："),_T("登录"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTestSection8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection8Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CMFCTestSection8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CMFCTestSection8Dlg, CDialog)
	ON_EVENT(CMFCTestSection8Dlg, IDC_LOGINAX1, 1, CMFCTestSection8Dlg::OnLoginLoginax1, VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

void CMFCTestSection8Dlg::OnLoginLoginax1(LPCTSTR bstrUserName, LPCTSTR bstrPassword)
{
	HRESULT hr = E_FAIL;

	CComBSTR bstrA(bstrUserName);
	CComBSTR bstrB(bstrPassword);
	VARIANT_BOOL varbResult = VARIANT_FALSE;

	//验证用户
	hr = m_spPasswordSpObj->CheckPassword(bstrA, bstrB, &varbResult);

	if (SUCCEEDED(hr))
	{
		if (varbResult)
		{
			MessageBox(_T("登录成功！"));
			OnOK();
		}
		else
		{
			MessageBox(_T("登录失败！"));
		}
	}
}


void CMFCTestSection8Dlg::OnMsg(BSTR bstrMsg)
{
	MessageBoxW(bstrMsg);
}