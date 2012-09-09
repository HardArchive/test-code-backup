// Use5Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use5.h"
#include "Use5Dlg.h"
#include ".\use5dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse5Dlg �Ի���



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


// CUse5Dlg ��Ϣ�������

BOOL CUse5Dlg::OnInitDialog()
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

void CUse5Dlg::OnPaint() 
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
HCURSOR CUse5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUse5Dlg::OnBnClickedButton8()
{
	// ������ stdafx.h �� #import ��ʱ��û��ʹ�� no_namespace ���Ҫʹ�������ռ�
	// �����ռ�� Simple2Lib ,������������ IDL �ļ� Library ָ����
	try
	{
		// ���ʹ������ָ��Ĺ��캯�������������д�򵥡�
		// ��Ҳ��ȱ�㣬��Ϊ���ʧ�ܵĻ�����֪������ԭ��

//		Simple2Lib::IFunPtr spFun( L"Simple2.fun.1" );				// ProgID ��ʽ
		Simple2Lib::IFunPtr spFun( __uuidof(Simple2Lib::Fun) );		// CLSID ��ʽ

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
