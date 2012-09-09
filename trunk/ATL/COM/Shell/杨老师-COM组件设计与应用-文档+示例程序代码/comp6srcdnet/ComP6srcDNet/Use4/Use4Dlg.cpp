// Use4Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Use4.h"
#include "Use4Dlg.h"
#include ".\use4dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUse4Dlg �Ի���



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


// CUse4Dlg ��Ϣ�������

BOOL CUse4Dlg::OnInitDialog()
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

void CUse4Dlg::OnPaint() 
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
HCURSOR CUse4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUse4Dlg::OnBnClickedButton7()		// #import ��ʹ��
{
/**********************************************************
1. ��CUse4App::InitInstance() ��ʹ�� AfxOleInit() ���г�ʼ��
2. �� stdafx.h �ļ������� #import �������
   ���� .tlh �� .tlh ������ָ���װ
3. #import ʹ���� no_namespace ��ʾ��ʹ�������ռ�
4. ����ָ��İ�װ��ʽ�ǣ�IxxxPtr��xxx ��ʾ�ӿ���
***********************************************************/
	IFunPtr spFun;
	HRESULT hr = spFun.CreateInstance( L"Simple2.fun.1" );	// ʹ�� ProgID
//	HRESULT hr = spFun.CreateInstance( __uuidof( Fun ) );	// ʹ�� CLSID
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
		// ���������ȡ����ϸ�Ĵ�����Ϣ
		// �Ժ��ڽ��� ISupportErrorInfo �ӿ�ʱ��ϸ˵��
//		e.Description();
//		e.ErrorMessage();
//		e.ErrorInfo();
//		......
		e;	// ����û��ʹ�� e, ��������ֻ��Ϊ��ȡ�����뾯��
		AfxMessageBox( _T("Error") );
	}
}
