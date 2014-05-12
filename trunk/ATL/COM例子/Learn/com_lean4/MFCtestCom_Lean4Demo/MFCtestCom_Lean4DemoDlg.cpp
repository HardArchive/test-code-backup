
// MFCtestCom_Lean4DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCtestCom_Lean4Demo.h"
#include "MFCtestCom_Lean4DemoDlg.h"

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


// CMFCtestCom_Lean4DemoDlg 对话框




CMFCtestCom_Lean4DemoDlg::CMFCtestCom_Lean4DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCtestCom_Lean4DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCtestCom_Lean4DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCtestCom_Lean4DemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCtestCom_Lean4DemoDlg 消息处理程序

BOOL CMFCtestCom_Lean4DemoDlg::OnInitDialog()
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

	//简单对象的测试
	HRESULT hr = E_FAIL;
	hr = CoInitialize(NULL);		//使用com组件对象之前要添加这一句，这句一般可以放在CMFCTestSection4Demo1App::InitInstance()里

	if (SUCCEEDED(hr))
	{
		IHelloSimpleObject *pIHelloSimpleObject = NULL;
		hr = CoCreateInstance(CLSID_HelloSimpleObject, NULL, CLSCTX_INPROC_SERVER, IID_IHelloSimpleObject, (void**)&pIHelloSimpleObject);
		if (SUCCEEDED(hr))
		{
			//加法方法测试
			LONG lSum = 0;
			hr = pIHelloSimpleObject->Sumlong(9, 99, &lSum);

			//读描述属性
			BSTR bstrDescriptionA = SysAllocString(L"");
			hr = pIHelloSimpleObject->get_Description(&bstrDescriptionA);
			SysFreeString(bstrDescriptionA);
			bstrDescriptionA = NULL;

			//写描述属性
			BSTR bstrDescriptionB = SysAllocString(L"我运行在MFC里，我是新的描述属性！");
			hr = pIHelloSimpleObject->put_Description(bstrDescriptionB);
			SysFreeString(bstrDescriptionB);
			bstrDescriptionB = NULL;

			//读描述属性
			BSTR bstrDescriptionC = SysAllocString(L"");
			hr = pIHelloSimpleObject->get_Description(&bstrDescriptionC);
			SysFreeString(bstrDescriptionC);
			bstrDescriptionC = NULL;
		}

		pIHelloSimpleObject->Release();		//不使用这个接口，引用计数减一
	}

	CoUninitialize();		//不使用com组件对象时添加这一句，这句一般可以放在CMFCTestSection4Demo1App::ExitInstance()里。

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCtestCom_Lean4DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCtestCom_Lean4DemoDlg::OnPaint()
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
HCURSOR CMFCtestCom_Lean4DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

