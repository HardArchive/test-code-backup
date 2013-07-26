// FileWatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileWatcher.h"
#include "FileWatcherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileWatcherDlg dialog

CFileWatcherDlg::CFileWatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileWatcherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileWatcherDlg)
	m_csStrPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileWatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileWatcherDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_csListData);
	DDX_Control(pDX, IDC_EDIT_PATH, m_csEdtPath);
	DDX_Control(pDX, IDC_BTN_CTRL, m_csBtnCtrl);
	DDX_Text(pDX, IDC_EDIT_PATH, m_csStrPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileWatcherDlg, CDialog)
	//{{AFX_MSG_MAP(CFileWatcherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DATA, OnRclickListData)
	ON_COMMAND(IDR_MNU_OPEN, OnMnuOpen)
	ON_COMMAND(IDR_MNU_DELSEL, OnMnuDelsel)
	ON_COMMAND(IDR_MNU_DELALL, OnMnuDelall)
	ON_BN_CLICKED(IDC_BTN_CTRL, OnBtnCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileWatcherDlg message handlers

BOOL CFileWatcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//=======> ��ʼ����ʶ��
	this->m_bStart = FALSE;
	//======> ��ʼ����ͼ�ؼ�
	this->InitializeListCtrl();
	//======> ��������ͼ��
	this->NotifyTray(TRUE);
	//=====> �����ö�
	this->SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileWatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFileWatcherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileWatcherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���ʼ����ͼ�б�ؼ�
//����������
//��������������Ŀ��������չ��ʽ
void CFileWatcherDlg::InitializeListCtrl()
{
	this->m_csListData.InsertColumn(0, "ʱ��", LVCFMT_LEFT, 75);
	this->m_csListData.InsertColumn(1, "����", LVCFMT_LEFT, 50);
	this->m_csListData.InsertColumn(2, "����", LVCFMT_LEFT, 75);
	this->m_csListData.InsertColumn(3, "��Ϣ", LVCFMT_LEFT, 500);

	//========> ������չ��ʽ
	this->m_csListData.SetExtendedStyle(
		LVS_EX_GRIDLINES |
		LVS_EX_FULLROWSELECT |
		LVS_EX_FLATSB 
		);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�ѡ��Ŀ¼·��
//����������
//������������SH����������Ŀ¼ѡ��Ի���ѡ��Ŀ¼
CString CFileWatcherDlg::SelectPath()
{
	BROWSEINFO stBrowseInfo;  
	ITEMIDLIST *lpstIDLst = NULL;
	char szPath[MAX_PATH] = {0};
   
	//===========> ��ʼ���ṹ
	stBrowseInfo.hwndOwner = this->m_hWnd;  
	stBrowseInfo.pidlRoot = NULL;  
	stBrowseInfo.pszDisplayName = NULL;
	stBrowseInfo.lpszTitle = "��ѡ��Ŀ¼";  
	stBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS;  
	stBrowseInfo.lpfn = NULL;  
	stBrowseInfo.lParam = 0;  
	stBrowseInfo.iImage = 0;

	//=======> ����Ŀ¼ѡ��Ի���
	if ((lpstIDLst = ::SHBrowseForFolder(&stBrowseInfo)) == FALSE)
	{
		return "";
	}
	//========> ȡ��Ŀ¼·��
	if (::SHGetPathFromIDList(lpstIDLst, szPath) == FALSE)
	{
		return "";
	}

	return szPath;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�������������
//��������bCreate ���TRUE���������̣���֮��������
//��������Ϊ���򴴽����̣���ʾ���������·�
BOOL CFileWatcherDlg::NotifyTray(BOOL bCreate)
{
	NOTIFYICONDATA stNotify = {0};

	stNotify.cbSize = sizeof(NOTIFYICONDATA);
	stNotify.hIcon = this->m_hIcon;
	stNotify.hWnd = this->GetSafeHwnd();
	strcpy(stNotify.szTip, "Ŀ¼��س���˫����ʾ���ڣ�");
	stNotify.uCallbackMessage = WM_MY_NOTIFY;
	stNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	stNotify.uID = IDR_MAINFRAME;

	if (bCreate == TRUE)
	{
		return ::Shell_NotifyIcon(NIM_ADD, &stNotify);
	}
	return ::Shell_NotifyIcon(NIM_DELETE, &stNotify);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���ͼ�ؼ��һ�����
//����������
//���������һ����Ƿ���ʾ�����˵�
void CFileWatcherDlg::OnRclickListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	CMenu csMenu;
	POINT stPos = {0};

	//======> ȡ�õ�ǰ���λ��
	::GetCursorPos(&stPos);

	//====> ������Դ�˵�
	csMenu.LoadMenu(IDR_MNU_POP);
	//===========> �ж��Ƿ�ѡ������Ŀ
	if (this->m_csListData.GetSelectionMark() == -1)
	{
		csMenu.EnableMenuItem(IDR_MNU_OPEN, MF_GRAYED);
		csMenu.EnableMenuItem(IDR_MNU_DELSEL, MF_GRAYED);
	}
	//==========> �ж��Ƿ������Ŀ
	if (this->m_csListData.GetItemCount() == 0)
	{
		csMenu.EnableMenuItem(IDR_MNU_DELALL, MF_GRAYED);
	}
	//======> �����˵�
	csMenu.GetSubMenu(0)->TrackPopupMenu(
		TPM_LEFTALIGN,
		stPos.x,
		stPos.y,
		this);	
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ��˵����򿪡�����
//����������
//������������ͼ�ؼ��������е�·��
void CFileWatcherDlg::OnMnuOpen() 
{
	int nItem = 0;
	DWORD dwAct = 0;
	CString csStrShell, csStrInfo;
	HINSTANCE hInst = NULL;

	this->UpdateData();

	nItem = this->m_csListData.GetSelectionMark();
	dwAct = this->m_csListData.GetItemData(nItem);
	switch (dwAct)
	{
	case CWatcher::actRenameNew:
		{
			//=======> ȡ������
			csStrInfo = this->m_csListData.GetItemText(nItem, 3);
			//======== ����·��
			csStrShell = 
				this->m_csStrPath + 
				"\\" + 
				csStrInfo.Mid(csStrInfo.Find(" -> ", 0), csStrInfo.GetLength());
		}
		break;
	case CWatcher::actRemote:
		{
			MessageBox("������ݻָ���?");
			// ����
			return;
		}
		break;
	case CWatcher::actCreate:
	case CWatcher::actChange:
		{
			//======> ����·��
			csStrShell = 
				this->m_csStrPath + 
				"\\" + 
				this->m_csListData.GetItemText(nItem, 3);
		}
		break;
	}		
	//======> ��
	hInst = ::ShellExecute(NULL, 
		"open", 
		csStrShell,
		NULL,
		NULL,
		SW_SHOWNORMAL);
	if (hInst == NULL)
	{
		MessageBox("�㵱����������?");
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ��˵���ɾ��ѡ�񡱹���
//����������
//��������ɾ����ͼ�ؼ���ǰѡ�е���Ŀ
void CFileWatcherDlg::OnMnuDelsel() 
{
	this->m_csListData.DeleteItem(
		this->m_csListData.GetSelectionMark()
		);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ��˵���ɾ�����С�����
//����������
//��������ɾ����ͼ�ؼ�������Ŀ
void CFileWatcherDlg::OnMnuDelall() 
{
	this->m_csListData.DeleteAllItems();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���ʼ\����Ŀ¼����
//����������
//������������CWatcher::Create��ʼ����Ŀ¼����
void CFileWatcherDlg::OnBtnCtrl() 
{
	this->UpdateData();

	if (this->m_bStart == FALSE)
	{
		if (this->m_csWatcher.Create(this->m_csStrPath) == FALSE)
		{
			this->MessageBox("����Ŀ¼ʧ��");
		}
		else
		{
			this->m_bStart = TRUE;
			this->m_csEdtPath.EnableWindow(FALSE);
			this->m_csBtnCtrl.SetWindowText("ֹͣ");
		}
	}
	else
	{
		this->m_csWatcher.Destroy();
		this->m_bStart = FALSE;
		this->m_csEdtPath.EnableWindow(TRUE);
		this->m_csBtnCtrl.SetWindowText("��ʼ");
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�δת������Ϣ�������
//����������
//���������̳д����⺯����������Edit�����˫���¼�
BOOL CFileWatcherDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		if (pMsg->hwnd == this->m_csEdtPath.m_hWnd)
		{
			//=========> �ж��Ƿ��Ѿ���ʼ
			if (this->m_bStart == FALSE)
			{
				//======> ȡ��ѡ��Ŀ¼��·��
				this->m_csEdtPath.SetWindowText(this->SelectPath());
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���Ϣ�������
//����������
//���������̳д����⺯�������������������˫���¼�
LRESULT CFileWatcherDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_MY_NOTIFY)
	{
		if (lParam == WM_LBUTTONDBLCLK)
		{
			//=====> ��ʾ����
			ShowWindow(SW_RESTORE);
			//=====> �����
			SetForegroundWindow();
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ������˳�����
//����������
//��������ɾ���˵��û����OnOK�¼������ⰴ�س��˳�����
void CFileWatcherDlg::OnOK()
{
	// CDialog::OnOK() ȡ���˴��¼�
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ������˳�����
//����������
//���������˳�ʱ�򣬵�����Ϣ����ʾ�û�ʱ���˳������ټ��ӣ�ɾ������ͼ��
void CFileWatcherDlg::OnCancel()
{
	if (::AfxMessageBox("���Ƿ�Ҫ�˳�����?", MB_YESNO | MB_ICONSTOP) == IDYES)
	{
		this->NotifyTray(FALSE);
		this->m_csWatcher.Destroy();
		CDialog::OnCancel();
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ����ڳߴ�ı����
//����������
//����������Ҫ���ô˹�����ɳ�����С����ʱ������
void CFileWatcherDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MINIMIZED)
	{
		this->ShowWindow(SW_HIDE);
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ��ж��Ƿ���һ���ļ���
//��������lpszPath �ж�·��
//��������ȡ���ļ������ԣ�Ȼ���� �Ƚϣ�����
BOOL CFileWatcherDlg::IsFolder(LPCTSTR lpszPath)
{
	DWORD dwAttrib = 0;
	dwAttrib = GetFileAttributes(lpszPath);
	return (dwAttrib != -1) && 
		((dwAttrib & FILE_ATTRIBUTE_DIRECTORY)?TRUE:FALSE);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���ع���
//��������dwAction �������͡�csStrName1, csStrName2 �������
//������������������Ҫ����Destroy������CWatcherת��
void CFileWatcherDlg::OnWatch(DWORD dwAct, CString csStrName1, CString csStrName2)
{
	CString csStrTime, csStrAct, csStrType, 
		csStrPath , csStrInfo;
	int nItem = 0;

	//=======> ȡ��ʱ��
	csStrTime = ::CTime::GetCurrentTime().Format("%H��%M��%S");
	//=========> ȡ��ȫ·��
	csStrPath = this->m_csStrPath + "\\" + csStrName1; // Only 1
	//========> �ж����ͣ������ַ���
	if (this->IsFolder(csStrPath) == TRUE)
	{
		csStrType = "�ļ���";
	}
	else
	{
		csStrType = "�ļ�";
	}

	//==========> �ж�����
	switch (dwAct)
	{
	case CWatcher::actCreate:
		{
			csStrAct = "����";
			csStrInfo = csStrName1;
		}
		break;
	case CWatcher::actRemote:
		{
			csStrAct = "ɾ��";
			csStrInfo = csStrName1;
		}
		break;
	case CWatcher::actRenameNew:
		{
			csStrAct = "����";
			csStrInfo = csStrName1 + " -> " + csStrName2;
		}
		break;
	case CWatcher::actChange:
		{
			csStrAct = "����";
			csStrInfo = csStrName1;
		}
		break;
	}
	
	//============> �����Ϣ����ͼ�ؼ�
	nItem = this->m_csListData.InsertItem(0, csStrTime);
	this->m_csListData.SetItemText(nItem, 1, csStrAct);
	this->m_csListData.SetItemText(nItem, 2, csStrType);
	this->m_csListData.SetItemText(nItem, 3, csStrInfo);
	//=========> ���涯�����ؼ��ڲ� 
	this->m_csListData.SetItemData(nItem, dwAct);
}

