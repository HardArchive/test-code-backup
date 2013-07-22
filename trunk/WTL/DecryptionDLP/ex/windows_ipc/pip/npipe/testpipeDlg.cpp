#include "stdafx.h"
#include "testpipe.h"
#include "testpipeDlg.h"
#include "npipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestpipeDlg::CTestpipeDlg(CWnd* pParent)	: CDialog(CTestpipeDlg::IDD, pParent),
                                            m_sServer(_T(".")),
                                           	m_hIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME))
{
}

void CTestpipeDlg::DoDataExchange(CDataExchange* pDX)
{
  //Let the base class do its thing
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SERVER, m_sServer);
}

BEGIN_MESSAGE_MAP(CTestpipeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GETTIME, OnGettime)
END_MESSAGE_MAP()

BOOL CTestpipeDlg::OnInitDialog()
{
  //Let the base class do its thing
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestpipeDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); //device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		//Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		//Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTestpipeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestpipeDlg::OnGettime() 
{
  CWaitCursor wc;

  UpdateData();
	CNamedPipe clientPipe;

  try
  {
    CNamedPipe::ServerAvailable(m_sServer, _T("TEST_PJTIME"), 1000);
  }
  catch(CNamedPipeException* pEx)
  {
    pEx->Delete();
    AfxMessageBox(_T("instance of the specified named pipe is not available to be connected to"));
    return;
  }

  SYSTEMTIME st;
  memset(&st, 0, sizeof(SYSTEMTIME));


  //Connect to the server using Create and a Read call
  ///*
  try
  {
    clientPipe.Open(m_sServer, _T("TEST_PJTIME"), GENERIC_READ | FILE_SHARE_READ, 0, NULL, 0);
  }
  catch(CNamedPipeException* pEx)
  {
    AfxMessageBox(_T("Failed to connect to the specified named pipe"));
    pEx->Delete();
    return;
  }

  try
  {
    DWORD dwBytesRead = clientPipe.Read(&st, sizeof(st), NULL);
    dwBytesRead;
  }
  catch(CNamedPipeException* pEx)
  {
    AfxMessageBox(_T("Failed in call to read from the specified named pipe"));
    pEx->Delete();
    return;
  }
  //*/


  ////Connect to the server using the Call function
  /*
  try
  {
    DWORD dwBytesRead = CNamedPipe::Call(m_sServer, _T("TEST_PJTIME"), NULL, 0, &st, sizeof(SYSTEMTIME), dwBytesRead, 5000);
  }
  catch(CNamedPipeException* pEx)
  {
    AfxMessageBox(_T("Failed during connection to the server"));
    pEx->Delete();
    return;
  }
  */

  //Update the values on the UI
  TCHAR sTime[256];
  sTime[0] = _T('\0');
  GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sTime, 256);
  GetDlgItem(IDC_TIME)->SetWindowText(sTime);

  TCHAR sDate[256];
  sDate[0] = _T('\0');
  GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sDate, 256);
  GetDlgItem(IDC_DATE)->SetWindowText(sDate);

  //Close the pipe once were finished with it
  clientPipe.Close();
}
