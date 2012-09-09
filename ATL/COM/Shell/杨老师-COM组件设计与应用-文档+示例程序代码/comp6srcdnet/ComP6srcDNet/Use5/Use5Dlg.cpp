// Use5Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use5.h"
#include "Use5Dlg.h"
#include ".\use5dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse5Dlg 对话框



CUse5Dlg::CUse5Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse5Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUse5Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
END_MESSAGE_MAP()


// CUse5Dlg 消息处理程序

BOOL CUse5Dlg::OnInitDialog()
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

void CUse5Dlg::OnPaint() 
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
HCURSOR CUse5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUse5Dlg::OnBnClickedButton8()
{
	// 由于在 stdafx.h 中 #import 的时候，没有使用 no_namespace 因此要使用命名空间
	// 命名空间叫 Simple2Lib ,这个名称是组件 IDL 文件 Library 指定的
	try
	{
		// 这次使用智能指针的构造函数启动组件，书写简单。
		// 但也有缺点，因为如果失败的话，不知道错误原因

//		Simple2Lib::IFunPtr spFun( L"Simple2.fun.1" );				// ProgID 方式
		Simple2Lib::IFunPtr spFun( __uuidof(Simple2Lib::Fun) );		// CLSID 方式

		long nSum = spFun->Add( 1, 2 );

		CString sMsg;
		sMsg.Format( _T("1+2=%d"), nSum );
		AfxMessageBox( sMsg );

		_bstr_t sCat = spFun->Cat( _T("Hello"), _T(" world") );
		AfxMessageBox( sCat );
	}
	catch( _com_error &e )
	{
		e;
		AfxMessageBox( _T("Error") );
	}
}
