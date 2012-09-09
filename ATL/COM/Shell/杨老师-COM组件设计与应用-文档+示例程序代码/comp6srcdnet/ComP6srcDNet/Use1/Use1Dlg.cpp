// Use1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use1.h"
#include "Use1Dlg.h"
#include ".\use1dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse1Dlg 对话框



CUse1Dlg::CUse1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUse1Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
END_MESSAGE_MAP()


// CUse1Dlg 消息处理程序

BOOL CUse1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUse1Dlg::OnPaint() 
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
HCURSOR CUse1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "..\Simple2\simple2.h"
#include "..\Simple2\Simple2_i.c"

void CUse1Dlg::OnBnClickedButton1()
{
	::CoInitialize( NULL );

	IUnknown * pUnk = NULL;
	IFun * pFun = NULL;
	HRESULT hr;

	try
	{
		hr = ::CoCreateInstance(
			CLSID_Fun,
			NULL,
			CLSCTX_INPROC_SERVER,	// 以进程内组件 DLL 方式加载
			IID_IUnknown,			// 想要取得 IUnknown 接口指针
			(LPVOID *) &pUnk);
		if( FAILED( hr ) )	throw( _T("没注册吧？") );

		hr = pUnk->QueryInterface(	// 从 IUnknown 得到其它接口指针
			IID_IFun,				// 想要取得 IFun 接口指针
			(LPVOID *)&pFun );
		if( FAILED( hr ) )	throw( _T("居然没有接口？") );

		long nSum;
		hr = pFun->Add( 1, 2, &nSum );	// IFun::Add()
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		BSTR s1 = ::SysAllocString( L"Hello" );
		BSTR s2 = ::SysAllocString( L" world" );
		BSTR s3 = NULL;

		hr = pFun->Cat( s1, s2, &s3 );	// IFun::Cat()
		if( SUCCEEDED( hr ) )
		{
			CString sMsg( s3 );
			AfxMessageBox( sMsg );
		}
		
		// IFun::Cat() 最后一个参数是 [out] 方向属性，因此需要调用者释放
		if( s3 ) ::SysFreeString( s3 );
	}
	catch( LPCTSTR lpErr )
	{
		AfxMessageBox( lpErr );
	}

	if( pUnk )	pUnk->Release();
	if( pFun )	pFun->Release();

	::CoUninitialize();
}

// 包含使用 CComBSTR 需要的头文件
#include <atlbase.h>

void CUse1Dlg::OnBnClickedButton2()
{
	::CoInitialize( NULL );

	IFun * pFun = NULL;
	HRESULT hr;

	try
	{
		hr = ::CoCreateInstance(
			CLSID_Fun,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFun,				// 不再经过 IUnknown, 直接得到 IFun 接口指针
			(LPVOID *) &pFun);
		if( FAILED( hr ) )	throw( _T("没注册吧 或 没有接口？") );

		long nSum;
		hr = pFun->Add( 1, 2, &nSum );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		CComBSTR s1( "Hello" );		// 不再使用 API 方式操作 BSTR
		CComBSTR s2( " world" );	// 使用 CComBSTR 比较简单，并且
		CComBSTR s3;				// 最大的好处是，不用咱们自己来释放

		hr = pFun->Cat( s1, s2, &s3 );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg( s3 );
			AfxMessageBox( sMsg );
		}
	}
	catch( LPCTSTR lpErr )
	{
		AfxMessageBox( lpErr );
	}

	if( pFun )	pFun->Release();

	::CoUninitialize();	
}
