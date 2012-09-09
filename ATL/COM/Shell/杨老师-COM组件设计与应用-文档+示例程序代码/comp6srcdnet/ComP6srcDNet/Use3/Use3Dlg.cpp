// Use3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use3.h"
#include "Use3Dlg.h"
#include ".\use3dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse3Dlg 对话框



CUse3Dlg::CUse3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse3Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUse3Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()


// CUse3Dlg 消息处理程序

BOOL CUse3Dlg::OnInitDialog()
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

void CUse3Dlg::OnPaint() 
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
HCURSOR CUse3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <atlbase.h>
#include "..\Simple2\simple2.h"
#include "..\Simple2\Simple2_i.c"

void CUse3Dlg::OnBnClickedButton6()		// 智能指针的释放
{
	::CoInitialize( NULL );		// 如果在这里进行 COM 初始化，要注意智能指针的释放

	CComQIPtr < IFun, &IID_IFun > spFun;

	HRESULT hr = spFun.CoCreateInstance( CLSID_Fun );
	ASSERT( SUCCEEDED( hr ) );
	// 为了简单起见，不再使用 if 判断 HRESULT 了。IFun::Add() 也没有调用

	CComBSTR s1( "Hello" ), s2( " world" ), s3;
	hr = spFun->Cat( s1, s2, &s3 );
	ASSERT( SUCCEEDED( hr ) );
	CString sMsg( s3 );
	AfxMessageBox( sMsg );

//	spFun->Release();	// 大错特错！！！
	spFun.Release();	// 正解

	::CoUninitialize();
}
