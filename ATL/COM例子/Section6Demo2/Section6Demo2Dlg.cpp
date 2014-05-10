// Section6Demo2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Section6Demo2.h"
#include "Section6Demo2Dlg.h"

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


// CSection6Demo2Dlg 对话框




CSection6Demo2Dlg::CSection6Demo2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSection6Demo2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSection6Demo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSection6Demo2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSection6Demo2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSection6Demo2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSection6Demo2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSection6Demo2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSection6Demo2Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSection6Demo2Dlg 消息处理程序

BOOL CSection6Demo2Dlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSection6Demo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSection6Demo2Dlg::OnPaint()
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
HCURSOR CSection6Demo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSection6Demo2Dlg::OnBnClickedButton1()
{
	//CComBSTR的常用构造方法

	CComBSTR bstrA;
	CComBSTR bstrB(L"Hello CComBSTR");
	CComBSTR bstrC(bstrB);
}

void CSection6Demo2Dlg::OnBnClickedButton2()
{
	//CComBSTR的常用赋值方法

	CComBSTR bstrA(L"Hello CComBSTR");
	CComBSTR bstrB;
	CComBSTR bstrC;
	CComBSTR bstrD;
	CComBSTR bstrE;


	//CComBSTR的常用赋值方法(=号运行符重载方法)
	bstrB = bstrA;
	bstrC = L"Hello CComBSTR";
	bstrD = "Hello CComBSTR";

	//CComBSTR的常用赋值方法(AssignBSTR方法);
	BSTR bstr = SysAllocString(L"Hello CComBSTR");
	bstrE.AssignBSTR(bstr);
	SysFreeString(bstr);
}

void CSection6Demo2Dlg::OnBnClickedButton3()
{
	//CComBSTR与BSTR的转换
	{
		{
			//operator BSTR()测试
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA;
			//SysFreeString(bstrB);		//不能对bstrB执行SysFreeString操作！
		}

		{
			//HRESULT CopyTo(BSTR* pbstr)测试
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrA.CopyTo(&bstrB);
			SysFreeString(bstrB);		//必需对bstrB执行SysFreeString操作！
		}

		{
			//BSTR Copy()测试
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA.Copy();
			SysFreeString(bstrB);		//必需对bstrB执行SysFreeString操作！
		}

		{
			//BSTR Detach()测试
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA.Detach();		//此时bstrA的字符串内容为空！
			SysFreeString(bstrB);		//必需对bstrB执行SysFreeString操作！
		}

		{
			//void Attach(BSTR src)测试
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB = SysAllocString(L"Hello BSTR");

			bstrA.Attach(bstrB);		//bstrA的内容为L"Hello BSTR"

			//SysFreeString(bstrB);		//不能对bstrB执行SysFreeString操作！
		}
	}
}

void CSection6Demo2Dlg::OnBnClickedButton4()
{
	//CComBSTR的连接
	
	CComBSTR bstrA;
	bstrA.Append('H');				// Append(char ch)
	bstrA.Append(L'e');				//Append(wchar_t ch)
	bstrA.Append("llo");			//Append(const char* psz)
	bstrA.Append(L" CComBSTR");		//Append(const wchar_t* pwsz)

	CComBSTR bstrB(L"Hello");
	CComBSTR bstrC(L" CComBSTR");
	bstrB.Append(bstrC);			//Append(const CComBSTR& bstrSrc)

	CComBSTR bstrD(L"Hello");
	BSTR bstrE = SysAllocString(L" CComBSTR");
	bstrD.AppendBSTR(bstrE);		//AppendBSTR(BSTR bstr)
	SysFreeString(bstrE);

	CComBSTR bstrF(L"Hello");
	bstrF += L" CComBSTR";			//operator+=(const wchar_t* pwsz )

	CComBSTR bstrG(L"Hello");
	CComBSTR bstrH(L" CComBSTR");
	bstrG += bstrH;					//operator+=(const CComBSTR& bstrSrc)
}

void CSection6Demo2Dlg::OnBnClickedButton5()
{
	//CComBSTR的大小写转换
	
	CComBSTR bstrA(L"AbCdEf");
	CComBSTR bstrB(L"AbCdEf");

	bstrA.ToLower();
	bstrB.ToUpper();
}
