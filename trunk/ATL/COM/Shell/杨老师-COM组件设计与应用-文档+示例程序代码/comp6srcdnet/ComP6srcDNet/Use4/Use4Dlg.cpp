// Use4Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Use4.h"
#include "Use4Dlg.h"
#include ".\use4dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse4Dlg 对话框



CUse4Dlg::CUse4Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse4Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUse4Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
END_MESSAGE_MAP()


// CUse4Dlg 消息处理程序

BOOL CUse4Dlg::OnInitDialog()
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

void CUse4Dlg::OnPaint() 
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
HCURSOR CUse4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUse4Dlg::OnBnClickedButton7()		// #import 的使用
{
/**********************************************************
1. 在CUse4App::InitInstance() 中使用 AfxOleInit() 进行初始化
2. 打开 stdafx.h 文件，插入 #import ，后编译
   产生 .tlh 和 .tlh 的智能指针包装
3. #import 使用了 no_namespace 表示不使用命名空间
4. 智能指针的包装形式是：IxxxPtr，xxx 表示接口名
***********************************************************/
	IFunPtr spFun;
	HRESULT hr = spFun.CreateInstance( L"Simple2.fun.1" );	// 使用 ProgID
//	HRESULT hr = spFun.CreateInstance( __uuidof( Fun ) );	// 使用 CLSID
	ASSERT( SUCCEEDED( hr ) );

	try
	{
		long nSum = spFun->Add( 1, 2 );

		CString sMsg;
		sMsg.Format( _T("1+2=%d"), nSum );
		AfxMessageBox( sMsg );

		_bstr_t sCat = spFun->Cat( _T("Hello"), _T(" world") );
		AfxMessageBox( sCat );
	}
	catch( _com_error &e )
	{
		// 在这里可以取得详细的错误信息
		// 以后在介绍 ISupportErrorInfo 接口时详细说明
//		e.Description();
//		e.ErrorMessage();
//		e.ErrorInfo();
//		......
		e;	// 由于没有使用 e, 加上这行只是为了取消编译警告
		AfxMessageBox( _T("Error") );
	}
}
