
// TestEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//ע�⣺�����ļ�����Scintilla��includeĿ¼ 
#include "scintilla/Scintilla.h"
#include "scintilla/SciLexer.h"

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


// CTestEditorDlg �Ի���




CTestEditorDlg::CTestEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTestEditorDlg ��Ϣ�������

BOOL CTestEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	SetCppStyle();
	SetExample();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestEditorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//m_ScintillaWnd.MoveWindow(0, 0, cx, cy); 
	::MoveWindow(m_hScintillaWnd, 0, 0, cx, cy, FALSE);
}

int CTestEditorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//m_ScintillaWnd.Create(   
	//	WS_EX_CLIENTEDGE,   
	//	WS_CHILD | WS_VISIBLE,   
	//	CRect(0, 0, lpCreateStruct->cx,lpCreateStruct->cy),   
	//	this, 10000); 
	//CWnd::CreateEx(dwExStyle, L"Scintilla", L"", dwStyle, rect, pParentWnd, nID);   
	
	//SciLexer.dll���غ���Զ���"Scintilla"��Ϊ����ע��һ�������࣬����ֻҪֱ�������������������Ϳ�����
	m_hScintillaWnd = ::CreateWindow(
		_T("Scintilla"), 
		NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, 
		0, 0, lpCreateStruct->cx, lpCreateStruct->cy,
		this->m_hWnd,
		NULL, NULL, NULL); 

	//SendEditor(SCI_GETDIRECTFUNCTION, 0, 0);
	//SendEditor(SCI_GETDIRECTPOINTER, 0, 0);

	return 0;
}

//��Scintilla�ؼ������¼�ʱ������WM_NOTITY��Ϣ֪ͨ�����塣
BOOL CTestEditorDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	LPNMHDR pnmh = (LPNMHDR)lParam;   
	struct SCNotification* scn = (struct SCNotification*)lParam;   

	switch(pnmh->code)   
	{    
	//case SCN_CHARADDED:   

		/* Hey, Scintilla just told me that a new */  
		/* character was added to the Edit Control.*/  
		/* Now i do something cool with that char. */  
		//break; 
	case SCN_MODIFIED:   
		//This notification is sent when the text or    
		//styling of the document changes or is about    
		//to change   
	case SCN_ZOOM:   
		//This notification is generated when the user   
		//zooms the display using the keyboard or the   
		//SCI_SETZOOM method is called.   
		UpdateLineNumberWidth();   
		break;  

		//case ...   
	}  

	return CDialog::OnNotify(wParam, lParam, pResult);
}

//��ʾ�кţ�������Ҫ��m_hScintillaWnd�����������������º���������Scintilla֪ͨ������SCN_MODIFIED��SCN_ZOOMʱ����
void CTestEditorDlg::UpdateLineNumberWidth(void)   
{   
	char tchLines[32];   
	int  iLineMarginWidthNow;   
	int  iLineMarginWidthFit;   

	sprintf(tchLines," %i ", SendEditor(SCI_GETLINECOUNT, 0, 0));   
	//�����к�
	iLineMarginWidthNow = SendEditor(SCI_GETMARGINWIDTHN, 0, 0);
	iLineMarginWidthFit = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)tchLines);
	if (iLineMarginWidthNow != iLineMarginWidthFit)     
	{   
		SendEditor(SCI_SETMARGINWIDTHN, 0, iLineMarginWidthFit);     
	}   
} 

//��Scintilla֧���﷨����
void CTestEditorDlg::SetCppStyle()
{
	const char* szKeywords1 =
		"asm auto break case catch class const "
        "const_cast continue default delete do double "
        "dynamic_cast else enum explicit extern false "
        "for friend goto if inline mutable "
        "namespace new operator private protected public "
        "register reinterpret_cast return signed "
        "sizeof static static_cast struct switch template "
        "this throw true try typedef typeid typename "
        "union unsigned using virtual volatile while";
    const char* szKeywords2 = "bool char float int long short void wchar_t";
    // ����ȫ�ַ��
    SendEditor(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Courier New");
    SendEditor(SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
    SendEditor(SCI_STYLECLEARALL);
    //C++�﷨����
    SendEditor(SCI_SETLEXER, SCLEX_CPP);
    SendEditor(SCI_SETKEYWORDS, 0, (sptr_t)szKeywords1);//���ùؼ���
    SendEditor(SCI_SETKEYWORDS, 1, (sptr_t)szKeywords2);//���ùؼ���
    // �������ø����﷨Ԫ�ط��
    SendEditor(SCI_STYLESETFORE, SCE_C_WORD, 0x00FF0000);   //�ؼ���
    SendEditor(SCI_STYLESETFORE, SCE_C_WORD2, 0x00800080);   //�ؼ���
    SendEditor(SCI_STYLESETBOLD, SCE_C_WORD2, TRUE);   //�ؼ���
    SendEditor(SCI_STYLESETFORE, SCE_C_STRING, 0x001515A3); //�ַ���
    SendEditor(SCI_STYLESETFORE, SCE_C_CHARACTER, 0x001515A3); //�ַ�
    SendEditor(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x00808080);//Ԥ���뿪��
    SendEditor(SCI_STYLESETFORE, SCE_C_COMMENT, 0x00008000);//��ע��
    SendEditor(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x00008000);//��ע��
    SendEditor(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x00008000);//�ĵ�ע�ͣ�/**��ͷ��
   
    SendEditor(SCI_SETCARETLINEVISIBLE, TRUE);
    SendEditor(SCI_SETCARETLINEBACK, 0xb0ffff); 
	SendEditor(SCI_SETPROPERTY,(sptr_t)"fold",(sptr_t)"1");  //�����۵�
}

// ��Ǻ�ҳ����ʾ 
void CTestEditorDlg::SetExample()
{
	// ��д10���ı���ȥ
	for(int i=0; i<10; i++)
		SendEditor(SCI_APPENDTEXT, 12, (sptr_t)"hello world\r\n ");
	// 0��ҳ�ߣ����Ϊ9����ʾ0�ű��(0..0001B)
	SendEditor(SCI_SETMARGINTYPEN,0,SC_MARGIN_SYMBOL);
	SendEditor(SCI_SETMARGINWIDTHN,0, 9);
	SendEditor(SCI_SETMARGINMASKN,0, 0x01);
	// 1��ҳ�ߣ����Ϊ9����ʾ1,2�ű��(0..0110B)
	SendEditor(SCI_SETMARGINTYPEN,1, SC_MARGIN_SYMBOL);
	SendEditor(SCI_SETMARGINWIDTHN,1, 9);
	SendEditor(SCI_SETMARGINMASKN,1, 0x06);
	// 2��ҳ�ߣ����Ϊ20����ʾ�к�
	SendEditor(SCI_SETMARGINTYPEN,2, SC_MARGIN_NUMBER);
	SendEditor(SCI_SETMARGINWIDTHN,2, 20);

	for(int i=0; i<10; i++)
	{
		// ǰ10�зֱ����0~2�ű��
		SendEditor(SCI_MARKERADD, i, i%3);
	}

	// ���ñ�ǵ�ǰ��ɫ
	SendEditor(SCI_MARKERSETFORE,0,0x0000ff);//0-��ɫ
	SendEditor(SCI_MARKERSETFORE,1,0x00ff00);//1-��ɫ
	SendEditor(SCI_MARKERSETFORE,2,0xff0000);//2-��ɫ 
}

////�����۵�
//void CTestEditorDlg::SetFold()
//{
//	SendEditor(SCI_SETPROPERTY,(sptr_t)"fold",(sptr_t)"1");
//
//	SendEditor(SCI_SETMARGINTYPEN, MARGIN_FOLD_INDEX, SC_MARGIN_SYMBOL);//ҳ������
//	SendEditor(SCI_SETMARGINMASKN, MARGIN_FOLD_INDEX, SC_MASK_FOLDERS); //ҳ������
//	SendEditor(SCI_SETMARGINWIDTHN, MARGIN_FOLD_INDEX, 11); //ҳ�߿��
//	SendEditor(SCI_SETMARGINSENSITIVEN, MARGIN_FOLD_INDEX, TRUE); //��Ӧ�����Ϣ
//
//	// �۵���ǩ��ʽ
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS); 
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS); 
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND,  SC_MARK_CIRCLEPLUSCONNECTED);
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED);
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE); 
//	SendEditor(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);
//
//	// �۵���ǩ��ɫ
//	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERSUB, 0xa0a0a0);
//	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERMIDTAIL, 0xa0a0a0);
//	SendEditor(SCI_MARKERSETBACK, SC_MARKNUM_FOLDERTAIL, 0xa0a0a0);
//
//	SendEditor(SCI_SETFOLDFLAGS, 16|4, 0); //����۵������۵��е����¸���һ������ 
//}