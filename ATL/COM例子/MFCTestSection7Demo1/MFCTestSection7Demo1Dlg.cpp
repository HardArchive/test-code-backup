// MFCTestSection7Demo1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTestSection7Demo1.h"
#include "MFCTestSection7Demo1Dlg.h"

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


// CMFCTestSection7Demo1Dlg 对话框




CMFCTestSection7Demo1Dlg::CMFCTestSection7Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection7Demo1Dlg::IDD, pParent)
{
	m_ulCount = 1;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CoInitialize(NULL);
}

CMFCTestSection7Demo1Dlg::~CMFCTestSection7Demo1Dlg()
{
	CoUninitialize();
}

void CMFCTestSection7Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCTestSection7Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCTestSection7Demo1Dlg 消息处理程序

BOOL CMFCTestSection7Demo1Dlg::OnInitDialog()
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
	CComPtr<IHelloSOE> spHelloSOE;

	//创建简单对象
	hr = spHelloSOE.CoCreateInstance(CLSID_HelloSOE);

	if (SUCCEEDED(hr))
	{
		//事件注册
		DWORD dw = 0;
		hr = spHelloSOE.Advise(this, DIID__IHelloSOEEvents, &dw);
		
		if (SUCCEEDED(hr))
		{
			//简单对象的加法
			hr = spHelloSOE->Add(9,99);
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTestSection7Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection7Demo1Dlg::OnPaint()
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
HCURSOR CMFCTestSection7Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::QueryInterface( 
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (riid == IID_IDispatch || riid == IID_IUnknown || riid == DIID__IHelloSOEEvents)
	{
		*ppvObject = static_cast<IDispatch*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::AddRef( void)
{
	return ++m_ulCount;
}

ULONG STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::Release( void)
{
	if (0 == --m_ulCount)
	{
		delete this;
		return 0;
	}

	return m_ulCount;
}

HRESULT STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::GetTypeInfoCount( 
	/* [out] */ UINT *pctinfo)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::GetTypeInfo( 
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::GetIDsOfNames( 
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID *rgDispId)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CMFCTestSection7Demo1Dlg::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	if (dispIdMember == 1)
	{
		OnAddResult(pDispParams->rgvarg[0].lVal);
	}

	return S_OK;
}

void CMFCTestSection7Demo1Dlg::OnAddResult(LONG lResult)
{
	//简单对象的事件触发

	CString str;
	str.Format(_T("加法运算的结果是：%ld"), lResult);

	AfxMessageBox(str);
}