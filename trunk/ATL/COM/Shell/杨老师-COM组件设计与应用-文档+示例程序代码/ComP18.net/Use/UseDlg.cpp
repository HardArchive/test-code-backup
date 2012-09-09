// UseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use.h"
#include "UseDlg.h"
#include ".\usedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUseDlg �Ի���



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


// CUseDlg ��Ϣ�������

BOOL CUseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText( IDC_BAG, _T("str=hello\r\ninteger=100") );

	GetDlgItem( IDC_SET )->EnableWindow( FALSE );
	GetDlgItem( IDC_GET )->EnableWindow( FALSE );
	GetDlgItem( IDC_END )->EnableWindow( FALSE );
	GetDlgItem( IDC_BEGIN )->EnableWindow( TRUE );
	GetDlgItem( IDC_STR )->EnableWindow( FALSE );
	GetDlgItem( IDC_INTEGER )->EnableWindow( FALSE );

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUseDlg::OnPaint() 
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
HCURSOR CUseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUseDlg::OnBnClickedBegin()
{
	HRESULT hr = m_spObj.CreateInstance( _T("Simple18.Property.1") );
	if( FAILED( hr ) )
	{
		AfxMessageBox( _T("û��ע�������û�г�ʼ����") );
		return;
	}

	m_PropertyBag.SetEditWnd( (CEdit *)GetDlgItem( IDC_BAG ) );

	CComQIPtr < IPersistPropertyBag > spBag = m_spObj;
	if( !spBag )
	{
		AfxMessageBox( _T("���û���ṩ IPersistPropertyBag �ӿ�") );
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
	SetDlgItemText( IDC_BAG, NULL );	// ����
	m_PropertyBag.SetEditWnd( (CEdit *)GetDlgItem( IDC_BAG ) );
	
	CComQIPtr < IPersistPropertyBag > spBag = m_spObj;
	if( !spBag )
	{
		AfxMessageBox( _T("���û���ṩ IPersistPropertyBag �ӿ�") );
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
