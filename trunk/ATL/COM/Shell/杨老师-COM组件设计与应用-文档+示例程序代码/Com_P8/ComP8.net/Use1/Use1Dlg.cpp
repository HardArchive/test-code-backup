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
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
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

void CUse1Dlg::OnBnClickedOk()
{
	try
	{
		long nVal;
		CString sMsg;
		_bstr_t bstr;

		// 通过构造函数得到 IMathe 接口指针，如果出现错误
		// 比如没有注册，则会抛出异常，执行到 catch 模块
		IMathePtr	spMathe( _T("Simple4.Mathe") );
		IStrPtr		spStr;
		IMathe2Ptr	spMathe2;

		// IMathe::Add()
		nVal = spMathe->Add( 1, 2 );
		sMsg.Format( _T("1 + 2 = %d"), nVal );
		MessageBox( sMsg, _T("调用 IMathe::Add()") );

		spStr = spMathe;	// 自动 QueryInterface(),如果执行错误会执行到 catch
		// IStr::Cat()
		bstr = spStr->Cat( "Hello", " world" );
		MessageBox( bstr, _T("调用 IStr::Cat()") );

		spMathe2 = spMathe;	// QueryInterface() 得到 IMathe2 接口指针
		// IMathe2::Add(), 其实本质上和 IMathe::Add() 是同一函数体
		nVal = spMathe->Add( 3, 4 );
		sMsg.Format( _T("3 + 4 = %d"), nVal );
		MessageBox( sMsg, _T("调用 IMathe2::Add()") );

		// IMathe2::Mul()
		nVal = spMathe2->Mul( 3, 4 );
		sMsg.Format( _T("3 * 4 = %d"), nVal );
		MessageBox( sMsg, _T("调用 IMathe2::Mul()") );
	}
	catch(_com_error &e)
	{
		AfxMessageBox( e.ErrorMessage() );
	}
}
