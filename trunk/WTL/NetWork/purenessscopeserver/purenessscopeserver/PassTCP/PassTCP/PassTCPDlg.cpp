
// PassTCPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PassTCP.h"
#include "PassTCPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

//�߳�ִ��
DWORD WINAPI ThreadProc(LPVOID argv)
{
  CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )argv;
  pClientTcpSocket->Run();

  return 0;
}


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


// CPassTCPDlg �Ի���




CPassTCPDlg::CPassTCPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPassTCPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPassTCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtPort);
	DDX_Control(pDX, IDC_EDIT3, m_txtThreadCount);
	DDX_Control(pDX, IDC_EDIT13, m_txtRecvTimeout);
	DDX_Control(pDX, IDC_EDIT4, m_txtSocketInterval);
	DDX_Control(pDX, IDC_EDIT5, m_txtSendData);
	DDX_Control(pDX, IDC_CHECK1, m_chkIsAlwayConnect);
	DDX_Control(pDX, IDC_CHECK3, m_chkRadomaDelay);
	DDX_Control(pDX, IDC_CHECK2, m_chkIsRecv);
	DDX_Control(pDX, IDC_CHECK4, m_ChkIsBroken);
	DDX_Control(pDX, IDC_EDIT6, m_txtSuccessConnect);
	DDX_Control(pDX, IDC_EDIT7, m_txtSuccessSend);
	DDX_Control(pDX, IDC_EDIT8, m_txtSuccessRecv);
	DDX_Control(pDX, IDC_EDIT9, m_txtCurrConnect);
	DDX_Control(pDX, IDC_EDIT10, m_txtFailConnect);
	DDX_Control(pDX, IDC_EDIT11, m_txtFailSend);
	DDX_Control(pDX, IDC_EDIT12, m_txtFailRecv);
	DDX_Control(pDX, IDC_EDIT14, m_txtRecvLength);
	DDX_Control(pDX, IDC_CHECK5, m_chkRadomSendCount);
	DDX_Control(pDX, IDC_CHECK6, m_chkIsWriteLog);
	DDX_Control(pDX, IDC_CHECK7, m_chkSendOne);
}

BEGIN_MESSAGE_MAP(CPassTCPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON1, &CPassTCPDlg::OnBnClickedButton1)
  ON_WM_CLOSE()
  ON_BN_CLICKED(IDC_BUTTON2, &CPassTCPDlg::OnBnClickedButton2)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BUTTON3, &CPassTCPDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CPassTCPDlg ��Ϣ�������

BOOL CPassTCPDlg::OnInitDialog()
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

  SetTimer(1, 1000, NULL);
  InitRandom();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
  InitView();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPassTCPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPassTCPDlg::OnPaint()
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
HCURSOR CPassTCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPassTCPDlg::OnBnClickedButton1()
{
  //��ʼѹ�����������
  char* pSendData = NULL;
  ClearResult();
  Close();

  CString strData;

  m_txtThreadCount.GetWindowText(strData);
  int nThreadCount = _ttoi((LPCTSTR)strData);

  for(int i = 0; i < nThreadCount; i++)
  {
    //��ȡ�߳���Ϣ
    _Socket_Info* pSocket_Info             = new _Socket_Info();
    _Socket_State_Info* pSocket_State_Info = new _Socket_State_Info();
    m_txtServerIP.GetWindowText(strData);
    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSocket_Info->m_szSerevrIP, MAX_BUFF_20, NULL,NULL);
    pSocket_Info->m_szSerevrIP[nDecLen] = '\0';

    m_txtPort.GetWindowText(strData);
    pSocket_Info->m_nPort = _ttoi((LPCTSTR)strData);
    m_txtRecvTimeout.GetWindowText(strData);
    pSocket_Info->m_nRecvTimeout = _ttoi((LPCTSTR)strData);
    m_txtSocketInterval.GetWindowText(strData);
    pSocket_Info->m_nDelaySecond = _ttoi((LPCTSTR)strData);
    m_txtRecvLength.GetWindowText(strData);
    pSocket_Info->m_nRecvLength = _ttoi((LPCTSTR)strData);
    m_txtSendData.GetWindowText(strData);


    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
    pSendData = new char[nBufferSize];
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
    pSendData[nDecLen] = '\0';

    CConvertBuffer objConvertBuffer;
    //���Ҫת�������ݿ��С
    pSocket_Info->InitSendSize(objConvertBuffer.GetBufferSize(pSendData, nDecLen));
    //�����ݴ�ת���ɶ����ƴ�
    objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), (unsigned char*)pSocket_Info->m_pSendBuff, pSocket_Info->m_nSendLength);
    delete[] pSendData;

    if(m_chkIsAlwayConnect.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsAlwayConnect = true;
    }
    else
    {
      pSocket_Info->m_blIsAlwayConnect = false;
    }

    if(m_chkRadomaDelay.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsRadomaDelay = true;
    }
    else
    {
      pSocket_Info->m_blIsRadomaDelay = false;
    }

    if(m_chkIsRecv.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsRecv = true;
    }
    else
    {
      pSocket_Info->m_blIsRecv = false;
    }

    if(m_ChkIsBroken.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsBroken = true;
    }
    else
    {
      pSocket_Info->m_blIsBroken = false;
    }

    if(m_chkRadomSendCount.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsSendCount = true;
    }
    else
    {
      pSocket_Info->m_blIsSendCount = false;
    }

    if(m_chkIsWriteLog.GetCheck() == BST_CHECKED)
    {
      pSocket_Info->m_blIsWriteFile = true;
    }
    else
    {
      pSocket_Info->m_blIsWriteFile = false;
    }

	if(m_chkSendOne.GetCheck() == BST_CHECKED)
	{
		pSocket_Info->m_blIsSendOne = true;
	}
	else
	{
		pSocket_Info->m_blIsSendOne = false;
	}

    CClientTcpSocket* pClientTcpSocket = new CClientTcpSocket();
    pClientTcpSocket->SetSocketThread(pSocket_Info, pSocket_State_Info);

    DWORD  ThreadID = 0;
    CreateThread(NULL, NULL, ThreadProc, (LPVOID)pClientTcpSocket, NULL, &ThreadID);

    m_vecClientTcpSocket.push_back(pClientTcpSocket);
  }
}

void CPassTCPDlg::InitView()
{
  m_chkIsRecv.SetCheck(BST_CHECKED);
  m_ChkIsBroken.SetCheck(BST_CHECKED);

  m_txtServerIP.SetWindowText(_T("127.0.0.1"));
  m_txtPort.SetWindowText(_T("10002"));
  m_txtThreadCount.SetWindowText(_T("1"));
  m_txtRecvTimeout.SetWindowText(_T("1000"));
  m_txtSocketInterval.SetWindowText(_T("10"));
  m_txtRecvLength.SetWindowText(_T("14"));
  m_txtSendData.SetWindowText(_T("0a 00 00 00 00 10 be cd aa 8f 3c 01 00 00"));

  ClearResult();

  //��ʼ��TCP����
  WSADATA wsaData;
  int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if(nErr != 0)
  {
    MessageBox(_T("����socket�����ʧ�ܣ����鱾��socket��汾"), _T("������Ϣ"), MB_OK);
  }
}

void CPassTCPDlg::ClearResult()
{
  m_txtSuccessConnect.SetWindowText(_T("0"));
  m_txtSuccessSend.SetWindowText(_T("0"));
  m_txtSuccessRecv.SetWindowText(_T("0"));
  m_txtCurrConnect.SetWindowText(_T("0"));
  m_txtFailConnect.SetWindowText(_T("0"));
  m_txtFailSend.SetWindowText(_T("0"));
  m_txtFailRecv.SetWindowText(_T("0"));
}

void CPassTCPDlg::Close()
{
  int nCount = m_vecClientTcpSocket.size();
  for(int i = 0; i < nCount; i++)
  {
    CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
    if(NULL != pClientTcpSocket)
    {
      pClientTcpSocket->Stop();

      DWORD dwSleep = 100;
      Sleep(dwSleep);

      delete pClientTcpSocket;
      pClientTcpSocket = NULL;
    }
  }

  m_vecClientTcpSocket.clear();
}

void CPassTCPDlg::OnClose()
{
  Close();
  WSACleanup();
  CDialog::OnClose();
}

void CPassTCPDlg::OnBnClickedButton2()
{
  // TODO: Add your control notification handler code here
  int nCount = (int)m_vecClientTcpSocket.size();
  for(int i = 0; i < nCount; i++)
  {
    CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
    if(NULL != pClientTcpSocket)
    {
      pClientTcpSocket->Stop();
    }
  }
}

void CPassTCPDlg::OnTimer(UINT_PTR nIDEvent)
{
  if(nIDEvent == 1)
  {
    int nSuccessConnect = 0;
    int nSuccessSend    = 0;
    int nSuccessRecv    = 0;
    int ntCurrConnect   = 0;
    int nFailConnect    = 0;
    int nFailSend       = 0;
    int nFailRecv       = 0;
    int nCurrConnect    = 0;

    for(int i = 0; i < (int)m_vecClientTcpSocket.size(); i++)
    {
      CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
      if(NULL != pClientTcpSocket)
      {
        _Socket_State_Info* pSocket_State_Info = pClientTcpSocket->GetStateInfo();
        if(NULL != pSocket_State_Info)
        {
          nSuccessConnect += pSocket_State_Info->m_nSuccessConnect;
          nSuccessSend    += pSocket_State_Info->m_nSuccessSend;
          nSuccessRecv    += pSocket_State_Info->m_nSuccessRecv;
          nFailConnect    += pSocket_State_Info->m_nFailConnect;
          nFailSend       += pSocket_State_Info->m_nFailSend;
          nFailRecv       += pSocket_State_Info->m_nFailRecv;
          nCurrConnect    += pSocket_State_Info->m_nCurrectSocket;
        }
      }
    }

    //��ʾ�������
    CString strData;
    strData.Format(_T("%d"), nSuccessConnect);
    m_txtSuccessConnect.SetWindowText(strData);
    strData.Format(_T("%d"), nSuccessSend);
    m_txtSuccessSend.SetWindowText(strData);
    strData.Format(_T("%d"), nSuccessRecv);
    m_txtSuccessRecv.SetWindowText(strData);
    strData.Format(_T("%d"), nFailConnect);
    m_txtFailConnect.SetWindowText(strData);
    strData.Format(_T("%d"), nFailSend);
    m_txtFailSend.SetWindowText(strData);
    strData.Format(_T("%d"), nFailRecv);
    m_txtFailRecv.SetWindowText(strData);
    strData.Format(_T("%d"), nCurrConnect);
    m_txtCurrConnect.SetWindowText(strData);
  }

  CDialog::OnTimer(nIDEvent);
}

void CPassTCPDlg::OnBnClickedButton3()
{
  // TODO: Add your control notification handler code here
}
