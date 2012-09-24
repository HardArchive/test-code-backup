// SQLite TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SQLite Test.h"
#include "SQLite TestDlg.h"
#include "AddDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CSQLiteTestDlg::CSQLiteTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQLiteTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSQLiteTestDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_list);
    DDX_Control(pDX, IDC_OPEN, m_open);
    DDX_Control(pDX, IDC_CLOSE, m_close);
    DDX_Control(pDX, IDC_NEW, m_new);
    DDX_Control(pDX, IDC_ADD, m_add);
    DDX_Control(pDX, IDC_DEL, m_del);
}

BEGIN_MESSAGE_MAP(CSQLiteTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
    ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
    ON_BN_CLICKED(IDC_NEW, OnBnClickedNew)
    ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
    ON_BN_CLICKED(IDC_DEL, OnBnClickedDel)
    ON_BN_CLICKED(IDC_SNONE, GoSort)
    ON_BN_CLICKED(IDC_OWNER, GoSort)
    ON_BN_CLICKED(IDC_MAKE, GoSort)
    ON_BN_CLICKED(IDC_MODEL, GoSort)
    ON_BN_CLICKED(IDC_YEAR, GoSort)
    ON_BN_CLICKED(IDC_ASC, GoSort)
    ON_BN_CLICKED(IDC_DESC, GoSort)
    ON_BN_CLICKED(IDC_FNONE, GoFilter)
    ON_BN_CLICKED(IDC_CHEVY, GoFilter)
    ON_BN_CLICKED(IDC_FORD, GoFilter)
    ON_BN_CLICKED(IDC_TOYOTA, GoFilter)
    ON_BN_CLICKED(IDC_MAZDA, GoFilter)
END_MESSAGE_MAP()

void CSQLiteTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX){
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
        return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

BOOL CSQLiteTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    m_file_open = false;
    m_path = "";
    
    m_list.InsertColumn(0, "Owner", 10, 200);
    m_list.InsertColumn(1, "Make", 20, 150);
    m_list.InsertColumn(2, "Model", 30, 150);
    m_list.InsertColumn(3, "Year", 40, 150);

    CheckDlgButton(IDC_SNONE, 1);
    CheckDlgButton(IDC_ASC, 1);
    CheckDlgButton(IDC_FNONE, 1);

    Update();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSQLiteTestDlg::Update()
{
    SetDlgItemText(IDS_PATH, m_path);
    m_open.EnableWindow(!m_file_open);
    m_close.EnableWindow(m_file_open);
    m_add.EnableWindow(m_file_open);
    m_del.EnableWindow(m_file_open);

    m_list.EnableWindow(m_file_open);
}

void CSQLiteTestDlg::Clean()
{
    m_list.DeleteAllItems();
}

void CSQLiteTestDlg::LoadList()
{
    m_list.DeleteAllItems();
    if (m_file_open && m_cars.IsOpen()) {
        int count = m_cars.GetCount();
        m_cars.MoveFirst();
        for (int i=0; i<count && !m_cars.IsEOF(); i++) {
            int n = m_list.InsertItem(i, m_cars.m_owner);
            m_list.SetItemText(n, 1, m_cars.m_make);
            m_list.SetItemText(n, 2, m_cars.m_model);
            m_list.SetItemText(n, 3, m_cars.m_year);
            m_cars.MoveNext();
        }
        if (count)
            m_list.SetItemState(0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
    }
}


void CSQLiteTestDlg::OnBnClickedOpen()
{
    OnBnClickedClose();

    CFileDialog openFileDlg(true, "sqlite", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        "SQLite Database (.sqlite)|*.sqlite||");

    if (openFileDlg.DoModal() != IDCANCEL) {
        m_path = openFileDlg.GetPathName(); 
        if (m_cars.Open(m_path)) {
            m_file_open = true;
            GoSort();
            Update();
        }
    }
}

void CSQLiteTestDlg::OnBnClickedClose()
{
    m_file_open = false;
    m_path = "";
    m_cars.Close();
    m_list.DeleteAllItems();
    Update();
}

void CSQLiteTestDlg::OnBnClickedNew()
{
    OnBnClickedClose();

    CFileDialog openFileDlg(false, "sqlite", NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | 
                OFN_PATHMUSTEXIST,
        "SQLite Database (.sqlite)|*.sqlite||");

    if (openFileDlg.DoModal() != IDCANCEL) {
        m_path = openFileDlg.GetPathName(); 
        if (m_cars.Create(m_path)) {
            m_file_open = true;
            Update();
            LoadList();
        } 
    }
}

void CSQLiteTestDlg::OnBnClickedAdd()
{
    CAddDlg add;
    if (add.MyDoModal(&m_cars)) {
        GoSort();
    }
}

void CSQLiteTestDlg::GoSort()
{
    bool asc = GetCheckedRadioButton(IDC_ASC, IDC_DESC) == IDC_ASC;
    int idsort = GetCheckedRadioButton(IDC_SNONE, IDC_YEAR);
    
    CString sort;
    switch (idsort) {
    case IDC_MAKE:
        sort = "Make";
        break;
    case IDC_MODEL:
        sort = "Model";
        break;
    case IDC_YEAR:
        sort = "Year";
        break;
    case IDC_OWNER:
        sort = "Owner";
        break;
    case IDC_SNONE:
    default:
        sort = "";
    }

    CString cs;
    if (!sort.IsEmpty()) {    
        cs.Format("[%s] %s", sort, (asc) ? "ASC" : "DESC");
        //the SQL sort line looks like: "[Foo] ASC, [Boo] DESC"
    }
    m_cars.SetSort(cs);
    m_cars.Query();

    LoadList();
}

void CSQLiteTestDlg::GoFilter()
{
    int idmake = GetCheckedRadioButton(IDC_FNONE, IDC_MAZDA);

    CString make;
    switch (idmake) {
    case IDC_FORD:
        make = "Ford";
        break;
    case IDC_CHEVY:
        make = "Chevy";
        break;
    case IDC_TOYOTA:
        make = "Toyota";
        break;
    case IDC_MAZDA:
        make = "Mazda";
        break;
    case IDC_FNONE:
    default:
        make = "";
    }

    CString cs;
    if (!make.IsEmpty()) {    
        cs.Format("[Make]=\'%s\'", make);
        //the SQL would look like: "[Make]='Ford'"
    }
    m_cars.SetFilter(cs);
    m_cars.Query();

    LoadList();
}


void CSQLiteTestDlg::OnBnClickedDel()
{
    int count = m_list.GetSelectedCount();
    if (count > 0) {
        int nItem = m_list.GetNextItem(-1, LVNI_SELECTED);
        if (nItem != -1) {
            m_cars.Move(nItem+1);
            m_cars.Delete();
            m_cars.Query();
            LoadList();

            int count = m_cars.GetCount();
            if (nItem < count && count > 0) {
                m_list.SetItemState(nItem, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
            }
        }
    }
}


