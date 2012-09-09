// UseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use.h"
#include "UseDlg.h"
#include ".\usedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUseDlg 对话框



CUseDlg::CUseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUseDlg::IDD, pParent)
	, m_str(_T(""))
	, m_integer(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STR, m_str);
	DDX_Text(pDX, IDC_INTEGER, m_integer);
}

BEGIN_MESSAGE_MAP(CUseDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BEGIN, OnBnClickedBegin)
	ON_BN_CLICKED(IDC_END, OnBnClickedEnd)
	ON_BN_CLICKED(IDC_SET, OnBnClickedSet)
	ON_BN_CLICKED(IDC_GET, OnBnClickedGet)
END_MESSAGE_MAP()


// CUseDlg 消息处理程序

BOOL CUseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText( IDC_BAG, _T("str=hello\r\ninteger=100") );

	GetDlgItem( IDC_SET )->EnableWindow( FALSE );
	GetDlgItem( IDC_GET )->EnableWindow( FALSE );
	GetDlgItem( IDC_END )->EnableWindow( FALSE );
	GetDlgItem( IDC_BEGIN )->EnableWindow( TRUE );
	GetDlgItem( IDC_STR )->EnableWindow( FALSE );
	GetDlgItem( IDC_INTEGER )->EnableWindow( FALSE );

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUseDlg::OnPaint() 
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
HCURSOR CUseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUseDlg::OnBnClickedBegin()
{
	HRESULT hr = m_spObj.CreateInstance( _T("Simple18.Property.1") );
	if( FAILED( hr ) )
	{
		AfxMessageBox( _T("没有注册组件？没有初始化？") );
		return;
	}

	m_PropertyBag.SetEditWnd( (CEdit *)GetDlgItem( IDC_BAG ) );

	CComQIPtr < IPersistPropertyBag > spBag = m_spObj;
	if( !spBag )
	{
		AfxMessageBox( _T("组件没有提供 IPersistPropertyBag 接口") );
		return;
	}
	spBag->Load( &m_PropertyBag, NULL );

	GetDlgItem( IDC_SET )->EnableWindow( TRUE );
	GetDlgItem( IDC_GET )->EnableWindow( TRUE );
	GetDlgItem( IDC_END )->EnableWindow( TRUE );
	GetDlgItem( IDC_BEGIN )->EnableWindow( FALSE );
	GetDlgItem( IDC_STR )->EnableWindow( TRUE );
	GetDlgItem( IDC_INTEGER )->EnableWindow( TRUE );
	GetDlgItem( IDC_BAG )->EnableWindow( FALSE );
	GetDlgItem( IDC_STR )->SetFocus();
	((CEdit *)GetDlgItem( IDC_STR ))->SetSel( 0, -1 );
}

void CUseDlg::OnBnClickedEnd()
{
	SetDlgItemText( IDC_BAG, NULL );	// 清屏
	m_PropertyBag.SetEditWnd( (CEdit *)GetDlgItem( IDC_BAG ) );
	
	CComQIPtr < IPersistPropertyBag > spBag = m_spObj;
	if( !spBag )
	{
		AfxMessageBox( _T("组件没有提供 IPersistPropertyBag 接口") );
		return;
	}
	spBag->Save( &m_PropertyBag, TRUE, TRUE );
	
	m_spObj.Release();

	GetDlgItem( IDC_SET )->EnableWindow( FALSE );
	GetDlgItem( IDC_GET )->EnableWindow( FALSE );
	GetDlgItem( IDC_END )->EnableWindow( FALSE );
	GetDlgItem( IDC_BEGIN )->EnableWindow( TRUE );
	GetDlgItem( IDC_STR )->EnableWindow( FALSE );
	GetDlgItem( IDC_INTEGER )->EnableWindow( FALSE );
	GetDlgItem( IDC_BAG )->EnableWindow( TRUE );
	GetDlgItem( IDC_BAG )->SetFocus();
}

void CUseDlg::OnBnClickedSet()
{
	UpdateData();

	BSTR bstr = m_str.AllocSysString();
	m_spObj->put_str( bstr );
	::SysFreeString( bstr );

	m_spObj->Putinteger( m_integer );

	m_str.Empty();
	m_integer = 0;
	UpdateData( FALSE );

	GetDlgItem( IDC_STR )->SetFocus();
}

void CUseDlg::OnBnClickedGet()
{
	BSTR bstr;
	m_spObj->get_str( &bstr );
	m_str = CString( bstr );
	::SysFreeString( bstr );

	m_integer = m_spObj->Getinteger();

	UpdateData( FALSE );

	GetDlgItem( IDC_STR )->SetFocus();
	((CEdit *)GetDlgItem( IDC_STR ))->SetSel( 0, -1 );
}
