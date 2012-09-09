// Use3Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use3.h"
#include "Use3Dlg.h"
#include ".\use3dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse3Dlg �Ի���



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


// CUse3Dlg ��Ϣ�������

BOOL CUse3Dlg::OnInitDialog()
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

void CUse3Dlg::OnPaint() 
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
HCURSOR CUse3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include <atlbase.h>
#include "..\Simple2\simple2.h"
#include "..\Simple2\Simple2_i.c"

void CUse3Dlg::OnBnClickedButton6()		// ����ָ����ͷ�
{
	::CoInitialize( NULL );		// ������������ COM ��ʼ����Ҫע������ָ����ͷ�

	CComQIPtr < IFun, &IID_IFun > spFun;

	HRESULT hr = spFun.CoCreateInstance( CLSID_Fun );
	ASSERT( SUCCEEDED( hr ) );
	// Ϊ�˼����������ʹ�� if �ж� HRESULT �ˡ�IFun::Add() Ҳû�е���

	CComBSTR s1( "Hello" ), s2( " world" ), s3;
	hr = spFun->Cat( s1, s2, &s3 );
	ASSERT( SUCCEEDED( hr ) );
	CString sMsg( s3 );
	AfxMessageBox( sMsg );

//	spFun->Release();	// ����ش�����
	spFun.Release();	// ����

	::CoUninitialize();
}
