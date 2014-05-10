// MFCTestSection4Demo1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTestSection4Demo1.h"
#include "MFCTestSection4Demo1Dlg.h"


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


// CMFCTestSection4Demo1Dlg 对话框




CMFCTestSection4Demo1Dlg::CMFCTestSection4Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection4Demo1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestSection4Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCTestSection4Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestSection4Demo1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCTestSection4Demo1Dlg 消息处理程序

BOOL CMFCTestSection4Demo1Dlg::OnInitDialog()
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
			hr = pIHelloSimpleObject->SumLong(9, 99, &lSum);

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

void CMFCTestSection4Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection4Demo1Dlg::OnPaint()
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
HCURSOR CMFCTestSection4Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCTestSection4Demo1Dlg::OnBnClickedButton1()
{
	//简单对象的测试
	HRESULT hr = E_FAIL;
	hr = CoInitialize(NULL);		//使用com组件对象之前要添加这一句，这句一般可以放在CMFCTestSection4Demo1App::InitInstance()里

	if (SUCCEEDED(hr))
	{
		CComPtr<IHelloSimpleObject> spHelloSimpleObject;
		hr = CoCreateInstance(CLSID_HelloSimpleObject, NULL, CLSCTX_INPROC_SERVER, IID_IHelloSimpleObject, (void**)&spHelloSimpleObject);
		if (SUCCEEDED(hr))
		{
			//加法方法测试
			LONG lSum = 0;
			hr = spHelloSimpleObject->SumLong(9, 99, &lSum);

			//读描述属性
			BSTR bstrDescriptionA = SysAllocString(L"");
			hr = spHelloSimpleObject->get_Description(&bstrDescriptionA);
			SysFreeString(bstrDescriptionA);
			bstrDescriptionA = NULL;

			//写描述属性
			BSTR bstrDescriptionB = SysAllocString(L"我运行在MFC里，我是新的描述属性！");
			hr = spHelloSimpleObject->put_Description(bstrDescriptionB);
			SysFreeString(bstrDescriptionB);
			bstrDescriptionB = NULL;

			//读描述属性
			BSTR bstrDescriptionC = SysAllocString(L"");
			hr = spHelloSimpleObject->get_Description(&bstrDescriptionC);
			SysFreeString(bstrDescriptionC);
			bstrDescriptionC = NULL;
		}

		//spHelloSimpleObject->Release();		//不需要调用，也不能调用！
	}

	CoUninitialize();		//不使用com组件对象时添加这一句，这句一般可以放在CMFCTestSection4Demo1App::ExitInstance()里。
}
