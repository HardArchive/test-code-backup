// Use2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use2.h"
#include "Use2Dlg.h"
#include ".\use2dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse2Dlg 对话框



CUse2Dlg::CUse2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUse2Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
END_MESSAGE_MAP()


// CUse2Dlg 消息处理程序

BOOL CUse2Dlg::OnInitDialog()
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

void CUse2Dlg::OnPaint() 
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
HCURSOR CUse2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 包含 CComPtr,CComQIPtr,CComBSTR 所使用的头文件
#include <atlbase.h>

#include "..\Simple2\simple2.h"
#include "..\Simple2\Simple2_i.c"

void CUse2Dlg::OnBnClickedButton3()	// CComPtr 使用举例
{
	// COM 初始化 以 AfxOleInit() 函数调用，放在了 CUse2App::InitInstance() 中了。

	CComPtr < IUnknown > spUnk;		// 定义 IUnknown 智能指针
	CComPtr < IFun > spFun;			// 定义 IFun 智能指针
	HRESULT hr;

	try
	{
		// 可以用 CLSID 启动组件，也可以用 ProgID
		hr = spUnk.CoCreateInstance( CLSID_Fun );
		if( FAILED( hr ) )	throw( _T("没有注册组件吧？") );

		hr = spUnk.QueryInterface( &spFun );
		if( FAILED( hr ) )	throw( _T("居然没有接口？") );

		long nSum;
		hr = spFun->Add( 1, 2, &nSum );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		CComBSTR s1( "Hello" ), s2( " world" ), s3;
		hr = spFun->Cat( s1, s2, &s3 );
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

	// 智能接口指针的最大好处是，我们不用负责释放
}

void CUse2Dlg::OnBnClickedButton4()	// CComQIPtr 使用举例
{
	CComPtr < IUnknown > spUnk;		// 智能指针 IUnknown
	CComQIPtr < IFun > spFun;		// 智能指针 IFun
	HRESULT hr;

	try
	{
		// 使用 ProgID 启动组件
		hr = spUnk.CoCreateInstance( L"Simple2.fun.1" );
		if( FAILED( hr ) )	throw( _T("没有注册吧？") );

		spFun = spUnk;	// CComQIPtr 会帮我们自动调用 QueryInterface
		if( !spFun )	throw( _T("居然没有接口？") );	// 成功与否可以判断 非NULL

		long nSum;
		hr = spFun->Add( 1, 2, &nSum );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		CComBSTR s1( "Hello" ), s2( " world" ), s3;
		hr = spFun->Cat( s1, s2, &s3 );
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

}

void CUse2Dlg::OnBnClickedButton5()		// 不再经过 IUnknown
{
	CComQIPtr < IFun, &IID_IFun > spFun;		// 定义 IFun 智能指针
	HRESULT hr;

	try
	{
		hr = spFun.CoCreateInstance( L"Simple2.fun.1" );
		if( FAILED( hr ) )	throw( _T("没有注册组件 或 没有找到接口") );

		long nSum;
		hr = spFun->Add( 1, 2, &nSum );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		CComBSTR s1( "Hello" ), s2( " world" ), s3;
		hr = spFun->Cat( s1, s2, &s3 );
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
}
