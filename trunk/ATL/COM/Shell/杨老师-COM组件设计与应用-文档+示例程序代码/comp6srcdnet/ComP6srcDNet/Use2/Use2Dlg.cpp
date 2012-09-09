// Use2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use2.h"
#include "Use2Dlg.h"
#include ".\use2dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse2Dlg �Ի���



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


// CUse2Dlg ��Ϣ�������

BOOL CUse2Dlg::OnInitDialog()
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

void CUse2Dlg::OnPaint() 
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
HCURSOR CUse2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���� CComPtr,CComQIPtr,CComBSTR ��ʹ�õ�ͷ�ļ�
#include <atlbase.h>

#include "..\Simple2\simple2.h"
#include "..\Simple2\Simple2_i.c"

void CUse2Dlg::OnBnClickedButton3()	// CComPtr ʹ�þ���
{
	// COM ��ʼ�� �� AfxOleInit() �������ã������� CUse2App::InitInstance() ���ˡ�

	CComPtr < IUnknown > spUnk;		// ���� IUnknown ����ָ��
	CComPtr < IFun > spFun;			// ���� IFun ����ָ��
	HRESULT hr;

	try
	{
		// ������ CLSID ���������Ҳ������ ProgID
		hr = spUnk.CoCreateInstance( CLSID_Fun );
		if( FAILED( hr ) )	throw( _T("û��ע������ɣ�") );

		hr = spUnk.QueryInterface( &spFun );
		if( FAILED( hr ) )	throw( _T("��Ȼû�нӿڣ�") );

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

	// ���ܽӿ�ָ������ô��ǣ����ǲ��ø����ͷ�
}

void CUse2Dlg::OnBnClickedButton4()	// CComQIPtr ʹ�þ���
{
	CComPtr < IUnknown > spUnk;		// ����ָ�� IUnknown
	CComQIPtr < IFun > spFun;		// ����ָ�� IFun
	HRESULT hr;

	try
	{
		// ʹ�� ProgID �������
		hr = spUnk.CoCreateInstance( L"Simple2.fun.1" );
		if( FAILED( hr ) )	throw( _T("û��ע��ɣ�") );

		spFun = spUnk;	// CComQIPtr ��������Զ����� QueryInterface
		if( !spFun )	throw( _T("��Ȼû�нӿڣ�") );	// �ɹ��������ж� ��NULL

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

void CUse2Dlg::OnBnClickedButton5()		// ���پ��� IUnknown
{
	CComQIPtr < IFun, &IID_IFun > spFun;		// ���� IFun ����ָ��
	HRESULT hr;

	try
	{
		hr = spFun.CoCreateInstance( L"Simple2.fun.1" );
		if( FAILED( hr ) )	throw( _T("û��ע����� �� û���ҵ��ӿ�") );

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
