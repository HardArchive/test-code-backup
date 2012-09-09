// Use1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Use1.h"
#include "Use1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUse1Dlg dialog

CUse1Dlg::CUse1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUse1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUse1Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUse1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUse1Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUse1Dlg, CDialog)
	//{{AFX_MSG_MAP(CUse1Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUse1Dlg message handlers

BOOL CUse1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUse1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUse1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUse1Dlg::OnOK() 
{
	try
	{
		long nVal;
		CString sMsg;
		_bstr_t bstr;

		// 通过构造函数得到 IMathe 接口指针，如果出现错误
		// 比如没有注册，则会抛出异常，执行到 catch 模块
		IMathePtr	spMathe( _T("Simple3.Mathe") );
		IStrPtr		spStr;
		IMathe2Ptr	spMathe2;

		// IMathe::Add()
		nVal = spMathe->Add( 1, 2 );
		sMsg.Format( _T("1 + 2 = %d"), nVal );
		MessageBox( sMsg, _T("调用 IMathe::Add()") );

		spStr = spMathe;	// 自动 QueryInterface(),如果出现错误会执行到 catch
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
