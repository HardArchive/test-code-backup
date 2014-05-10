// Section6Demo2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Section6Demo2.h"
#include "Section6Demo2Dlg.h"

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


// CSection6Demo2Dlg �Ի���




CSection6Demo2Dlg::CSection6Demo2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSection6Demo2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSection6Demo2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSection6Demo2Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSection6Demo2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSection6Demo2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSection6Demo2Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSection6Demo2Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSection6Demo2Dlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSection6Demo2Dlg ��Ϣ�������

BOOL CSection6Demo2Dlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSection6Demo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSection6Demo2Dlg::OnPaint()
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
HCURSOR CSection6Demo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSection6Demo2Dlg::OnBnClickedButton1()
{
	//CComBSTR�ĳ��ù��췽��

	CComBSTR bstrA;
	CComBSTR bstrB(L"Hello CComBSTR");
	CComBSTR bstrC(bstrB);
}

void CSection6Demo2Dlg::OnBnClickedButton2()
{
	//CComBSTR�ĳ��ø�ֵ����

	CComBSTR bstrA(L"Hello CComBSTR");
	CComBSTR bstrB;
	CComBSTR bstrC;
	CComBSTR bstrD;
	CComBSTR bstrE;


	//CComBSTR�ĳ��ø�ֵ����(=�����з����ط���)
	bstrB = bstrA;
	bstrC = L"Hello CComBSTR";
	bstrD = "Hello CComBSTR";

	//CComBSTR�ĳ��ø�ֵ����(AssignBSTR����);
	BSTR bstr = SysAllocString(L"Hello CComBSTR");
	bstrE.AssignBSTR(bstr);
	SysFreeString(bstr);
}

void CSection6Demo2Dlg::OnBnClickedButton3()
{
	//CComBSTR��BSTR��ת��
	{
		{
			//operator BSTR()����
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA;
			//SysFreeString(bstrB);		//���ܶ�bstrBִ��SysFreeString������
		}

		{
			//HRESULT CopyTo(BSTR* pbstr)����
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrA.CopyTo(&bstrB);
			SysFreeString(bstrB);		//�����bstrBִ��SysFreeString������
		}

		{
			//BSTR Copy()����
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA.Copy();
			SysFreeString(bstrB);		//�����bstrBִ��SysFreeString������
		}

		{
			//BSTR Detach()����
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB;

			bstrB = bstrA.Detach();		//��ʱbstrA���ַ�������Ϊ�գ�
			SysFreeString(bstrB);		//�����bstrBִ��SysFreeString������
		}

		{
			//void Attach(BSTR src)����
			CComBSTR bstrA("Hello CComBSTR");
			BSTR bstrB = SysAllocString(L"Hello BSTR");

			bstrA.Attach(bstrB);		//bstrA������ΪL"Hello BSTR"

			//SysFreeString(bstrB);		//���ܶ�bstrBִ��SysFreeString������
		}
	}
}

void CSection6Demo2Dlg::OnBnClickedButton4()
{
	//CComBSTR������
	
	CComBSTR bstrA;
	bstrA.Append('H');				// Append(char ch)
	bstrA.Append(L'e');				//Append(wchar_t ch)
	bstrA.Append("llo");			//Append(const char* psz)
	bstrA.Append(L" CComBSTR");		//Append(const wchar_t* pwsz)

	CComBSTR bstrB(L"Hello");
	CComBSTR bstrC(L" CComBSTR");
	bstrB.Append(bstrC);			//Append(const CComBSTR& bstrSrc)

	CComBSTR bstrD(L"Hello");
	BSTR bstrE = SysAllocString(L" CComBSTR");
	bstrD.AppendBSTR(bstrE);		//AppendBSTR(BSTR bstr)
	SysFreeString(bstrE);

	CComBSTR bstrF(L"Hello");
	bstrF += L" CComBSTR";			//operator+=(const wchar_t* pwsz )

	CComBSTR bstrG(L"Hello");
	CComBSTR bstrH(L" CComBSTR");
	bstrG += bstrH;					//operator+=(const CComBSTR& bstrSrc)
}

void CSection6Demo2Dlg::OnBnClickedButton5()
{
	//CComBSTR�Ĵ�Сдת��
	
	CComBSTR bstrA(L"AbCdEf");
	CComBSTR bstrB(L"AbCdEf");

	bstrA.ToLower();
	bstrB.ToUpper();
}
