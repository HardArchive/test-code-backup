// Use1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use1.h"
#include "Use1Dlg.h"
#include ".\use1dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse1Dlg �Ի���



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


// CUse1Dlg ��Ϣ�������

BOOL CUse1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUse1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
			CLSCTX_INPROC_SERVER,	// �Խ�������� DLL ��ʽ����
			IID_IUnknown,			// ��Ҫȡ�� IUnknown �ӿ�ָ��
			(LPVOID *) &pUnk);
		if( FAILED( hr ) )	throw( _T("ûע��ɣ�") );

		hr = pUnk->QueryInterface(	// �� IUnknown �õ������ӿ�ָ��
			IID_IFun,				// ��Ҫȡ�� IFun �ӿ�ָ��
			(LPVOID *)&pFun );
		if( FAILED( hr ) )	throw( _T("��Ȼû�нӿڣ�") );

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
		
		// IFun::Cat() ���һ�������� [out] �������ԣ������Ҫ�������ͷ�
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

// ����ʹ�� CComBSTR ��Ҫ��ͷ�ļ�
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
			IID_IFun,				// ���پ��� IUnknown, ֱ�ӵõ� IFun �ӿ�ָ��
			(LPVOID *) &pFun);
		if( FAILED( hr ) )	throw( _T("ûע��� �� û�нӿڣ�") );

		long nSum;
		hr = pFun->Add( 1, 2, &nSum );
		if( SUCCEEDED( hr ) )
		{
			CString sMsg;
			sMsg.Format( _T("1 + 2 = %d"), nSum );
			AfxMessageBox( sMsg );
		}

		CComBSTR s1( "Hello" );		// ����ʹ�� API ��ʽ���� BSTR
		CComBSTR s2( " world" );	// ʹ�� CComBSTR �Ƚϼ򵥣�����
		CComBSTR s3;				// ���ĺô��ǣ����������Լ����ͷ�

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
