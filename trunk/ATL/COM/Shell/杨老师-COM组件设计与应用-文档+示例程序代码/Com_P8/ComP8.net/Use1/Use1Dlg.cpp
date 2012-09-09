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
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
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

void CUse1Dlg::OnBnClickedOk()
{
	try
	{
		long nVal;
		CString sMsg;
		_bstr_t bstr;

		// ͨ�����캯���õ� IMathe �ӿ�ָ�룬������ִ���
		// ����û��ע�ᣬ����׳��쳣��ִ�е� catch ģ��
		IMathePtr	spMathe( _T("Simple4.Mathe") );
		IStrPtr		spStr;
		IMathe2Ptr	spMathe2;

		// IMathe::Add()
		nVal = spMathe->Add( 1, 2 );
		sMsg.Format( _T("1 + 2 = %d"), nVal );
		MessageBox( sMsg, _T("���� IMathe::Add()") );

		spStr = spMathe;	// �Զ� QueryInterface(),���ִ�д����ִ�е� catch
		// IStr::Cat()
		bstr = spStr->Cat( "Hello", " world" );
		MessageBox( bstr, _T("���� IStr::Cat()") );

		spMathe2 = spMathe;	// QueryInterface() �õ� IMathe2 �ӿ�ָ��
		// IMathe2::Add(), ��ʵ�����Ϻ� IMathe::Add() ��ͬһ������
		nVal = spMathe->Add( 3, 4 );
		sMsg.Format( _T("3 + 4 = %d"), nVal );
		MessageBox( sMsg, _T("���� IMathe2::Add()") );

		// IMathe2::Mul()
		nVal = spMathe2->Mul( 3, 4 );
		sMsg.Format( _T("3 * 4 = %d"), nVal );
		MessageBox( sMsg, _T("���� IMathe2::Mul()") );
	}
	catch(_com_error &e)
	{
		AfxMessageBox( e.ErrorMessage() );
	}
}
