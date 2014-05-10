// MFCTestSection4Demo1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTestSection4Demo1.h"
#include "MFCTestSection4Demo1Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFCTestSection4Demo1Dlg �Ի���




CMFCTestSection4Demo1Dlg::CMFCTestSection4Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection4Demo1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestSection4Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCTestSection4Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestSection4Demo1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCTestSection4Demo1Dlg ��Ϣ�������

BOOL CMFCTestSection4Demo1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//�򵥶���Ĳ���
	HRESULT hr = E_FAIL;
	hr = CoInitialize(NULL);		//ʹ��com�������֮ǰҪ�����һ�䣬���һ����Է���CMFCTestSection4Demo1App::InitInstance()��

	if (SUCCEEDED(hr))
	{
		IHelloSimpleObject *pIHelloSimpleObject = NULL;
		hr = CoCreateInstance(CLSID_HelloSimpleObject, NULL, CLSCTX_INPROC_SERVER, IID_IHelloSimpleObject, (void**)&pIHelloSimpleObject);
		if (SUCCEEDED(hr))
		{
			//�ӷ���������
			LONG lSum = 0;
			hr = pIHelloSimpleObject->SumLong(9, 99, &lSum);

			//����������
			BSTR bstrDescriptionA = SysAllocString(L"");
			hr = pIHelloSimpleObject->get_Description(&bstrDescriptionA);
			SysFreeString(bstrDescriptionA);
			bstrDescriptionA = NULL;

			//д��������
			BSTR bstrDescriptionB = SysAllocString(L"��������MFC������µ��������ԣ�");
			hr = pIHelloSimpleObject->put_Description(bstrDescriptionB);
			SysFreeString(bstrDescriptionB);
			bstrDescriptionB = NULL;

			//����������
			BSTR bstrDescriptionC = SysAllocString(L"");
			hr = pIHelloSimpleObject->get_Description(&bstrDescriptionC);
			SysFreeString(bstrDescriptionC);
			bstrDescriptionC = NULL;
		}

		pIHelloSimpleObject->Release();		//��ʹ������ӿڣ����ü�����һ
	}

	CoUninitialize();		//��ʹ��com�������ʱ�����һ�䣬���һ����Է���CMFCTestSection4Demo1App::ExitInstance()�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCTestSection4Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCTestSection4Demo1Dlg::OnPaint()
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
//
HCURSOR CMFCTestSection4Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCTestSection4Demo1Dlg::OnBnClickedButton1()
{
	//�򵥶���Ĳ���
	HRESULT hr = E_FAIL;
	hr = CoInitialize(NULL);		//ʹ��com�������֮ǰҪ�����һ�䣬���һ����Է���CMFCTestSection4Demo1App::InitInstance()��

	if (SUCCEEDED(hr))
	{
		CComPtr<IHelloSimpleObject> spHelloSimpleObject;
		hr = CoCreateInstance(CLSID_HelloSimpleObject, NULL, CLSCTX_INPROC_SERVER, IID_IHelloSimpleObject, (void**)&spHelloSimpleObject);
		if (SUCCEEDED(hr))
		{
			//�ӷ���������
			LONG lSum = 0;
			hr = spHelloSimpleObject->SumLong(9, 99, &lSum);

			//����������
			BSTR bstrDescriptionA = SysAllocString(L"");
			hr = spHelloSimpleObject->get_Description(&bstrDescriptionA);
			SysFreeString(bstrDescriptionA);
			bstrDescriptionA = NULL;

			//д��������
			BSTR bstrDescriptionB = SysAllocString(L"��������MFC������µ��������ԣ�");
			hr = spHelloSimpleObject->put_Description(bstrDescriptionB);
			SysFreeString(bstrDescriptionB);
			bstrDescriptionB = NULL;

			//����������
			BSTR bstrDescriptionC = SysAllocString(L"");
			hr = spHelloSimpleObject->get_Description(&bstrDescriptionC);
			SysFreeString(bstrDescriptionC);
			bstrDescriptionC = NULL;
		}

		//spHelloSimpleObject->Release();		//����Ҫ���ã�Ҳ���ܵ��ã�
	}

	CoUninitialize();		//��ʹ��com�������ʱ�����һ�䣬���һ����Է���CMFCTestSection4Demo1App::ExitInstance()�
}
