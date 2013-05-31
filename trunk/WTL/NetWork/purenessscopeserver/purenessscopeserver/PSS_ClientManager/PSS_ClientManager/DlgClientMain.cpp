// DlgClientMain.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientMain.h"


// CDlgClientMain dialog

IMPLEMENT_DYNAMIC(CDlgClientMain, CDialog)

CDlgClientMain::CDlgClientMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClientMain::IDD, pParent)
{
  m_pTcpClientConnect = NULL;
}

CDlgClientMain::~CDlgClientMain()
{
}

void CDlgClientMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
	DDX_Control(pDX, IDC_EDIT3, m_txtServerInfo);
	DDX_Control(pDX, IDC_EDIT4, m_txtModuleName);
	DDX_Control(pDX, IDC_LIST1, m_lcModuleList);
	DDX_Control(pDX, IDC_EDIT6, m_txtKey);
	DDX_Control(pDX, IDC_EDIT8, m_txtModuleFile);
}


BEGIN_MESSAGE_MAP(CDlgClientMain, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientMain::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CDlgClientMain::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, &CDlgClientMain::OnBnClickedButton3)
  ON_BN_CLICKED(IDC_BUTTON4, &CDlgClientMain::OnBnClickedButton4)
  ON_BN_CLICKED(IDC_BUTTON5, &CDlgClientMain::OnBnClickedButton5)
  ON_BN_CLICKED(IDC_BUTTON7, &CDlgClientMain::OnBnClickedButton7)
END_MESSAGE_MAP()

CString CDlgClientMain::GetPageTitle()
{
  return _T("�ͻ�����ģ�����");
}


// CDlgClientMain message handlers

void CDlgClientMain::OnBnClickedButton1()
{
  // TODO: Add your control notification handler code here
  CString strServerIP;
  CString strServerPort;
  CString strConsoleKey;

  char szSeverIP[20]     = {'\0'};
  char szServerPort[20]  = {'\0'};
  char szConsoleKey[100] = {'\0'};

  m_txtServerIP.GetWindowText(strServerIP);
  m_txtServerPort.GetWindowText(strServerPort);
  m_txtKey.GetWindowText(strConsoleKey);

  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, strServerIP.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, nSrcLen, szSeverIP, 20, NULL,NULL);
  szSeverIP[nDecLen] = '\0';

  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, strServerPort.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, nSrcLen, szServerPort, 20, NULL,NULL);
  szServerPort[nDecLen] = '\0';

  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, strConsoleKey.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, nSrcLen, szConsoleKey, 100, NULL,NULL);
  szConsoleKey[nDecLen] = '\0';

  if(strlen(szSeverIP) == 0 || strlen(szServerPort) == 0  || strlen(szConsoleKey) == 0)
  {
    MessageBox(_T(MESSAGE_INSERT_NULL) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }

  int nPort = atoi(szServerPort);

  m_pTcpClientConnect->Init(szSeverIP, nPort, szConsoleKey);

  MessageBox(_T(MESSAGE_TITLE_CONNECT), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
}

void CDlgClientMain::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}

void CDlgClientMain::OnBnClickedButton2()
{
  // TODO: Add your control notification handler code here
  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ShowServerInfo -a", m_pTcpClientConnect->GetKey());
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[10 * 1024] = {'\0'};
  int nRecvLen = 10 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    //�����͵�����
    int nPos = 4;

    int nTempLen             = 0;
    int nSerevrID            = 0;
    char szServerName[20]    = {'\0'};
    char szServerVersion[20] = {'\0'};
    int nModuleCount         = 0;
    int nWorkThreadCount     = 0;
    char szPacketVersion[20] = {'\0'};

    memcpy_s(&nSerevrID, 2, &szRecvBuff[nPos], sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
    nPos += sizeof(char);
    memcpy_s(&szServerName, nTempLen, &szRecvBuff[nPos], nTempLen);
    nPos += nTempLen;
    memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
    nPos += sizeof(char);
    memcpy_s(&szServerVersion, nTempLen, &szRecvBuff[nPos], nTempLen);
    nPos += nTempLen;
    memcpy_s(&nModuleCount, 2, &szRecvBuff[nPos], sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&nWorkThreadCount, 2, &szRecvBuff[nPos], sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
    nPos += sizeof(char);
    memcpy_s(&szPacketVersion, nTempLen, &szRecvBuff[nPos], nTempLen);
    nPos += nTempLen;

    char szText[2048] = {'\0'};
    sprintf_s(szText, "������ID:%d.\r\n����������:%s.\r\n�������汾:%s.\r\n����������ģ����:%d.\r\n�����������߳���:%d\r\n�������������汾��:%s.", 
      nSerevrID, szServerName, szServerVersion, nModuleCount, nWorkThreadCount, szPacketVersion);

    //��ʾ����
    wchar_t szTemp[2048] = {'\0'};

    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, szText, -1, szTemp, 2048);
    szTemp[nDecLen] = '\0';
    m_txtServerInfo.SetWindowText((LPCTSTR)szTemp);
  }

}

BOOL CDlgClientMain::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_txtServerIP.SetWindowText(_T("127.0.0.1"));
  m_txtServerPort.SetWindowText(_T("10010"));
  m_txtKey.SetWindowText(_T("freeeyes"));

  m_lcModuleList.InsertColumn(0, _T("��ǰ�Ѽ���ģ��"), LVCFMT_CENTER, 200);
  m_lcModuleList.InsertColumn(1, _T("ģ������"), LVCFMT_CENTER, 100);
  m_lcModuleList.InsertColumn(2, _T("ģ����Ϣ"), LVCFMT_CENTER, 100);
  m_lcModuleList.InsertColumn(3, _T("����ʱ��"), LVCFMT_CENTER, 200);

  return TRUE;
}

void CDlgClientMain::OnBnClickedButton3()
{
  // TODO: Add your control notification handler code here
  m_lcModuleList.DeleteAllItems();

  vecModuleInfo objvecModuleInfo;

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ShowModule -a", m_pTcpClientConnect->GetKey());
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[10 * 1024] = {'\0'};
  int nRecvLen = 10 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    int nStrLen      = 0;
    int nPos         = 4;
    int nModuleCount = 0;
    memcpy_s(&nModuleCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    for(int i = 0; i < nModuleCount; i++)
    {
      //��ʼ��ԭ���ݽṹ
      _ModuleInfo ModuleInfo;
      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ModuleInfo.szModuleName, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ModuleInfo.szModuleName[nStrLen] = '\0';

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ModuleInfo.szModuleFile, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ModuleInfo.szModuleFile[nStrLen] = '\0';

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ModuleInfo.szModuleDesc, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ModuleInfo.szModuleDesc[nStrLen] = '\0';

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ModuleInfo.szModuleCreateDate, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ModuleInfo.szModuleCreateDate[nStrLen] = '\0';

      //��ʾ��������
      wchar_t szModuleFile[200]    = {'\0'};
      wchar_t szModuleName[200]    = {'\0'};
      wchar_t szModuleDesc[200]    = {'\0'};
      wchar_t szModuleTime[200]    = {'\0'};

      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleFile, -1, NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleFile, -1, szModuleFile, 200);

      nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleName, -1, NULL, 0);
      nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleName, -1, szModuleName, 200);

      nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleDesc, -1, NULL, 0);
      nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleDesc, -1, szModuleDesc, 200);

      nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleCreateDate, -1, NULL, 0);
      nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleCreateDate, -1, szModuleTime, 200);

      m_lcModuleList.InsertItem(i, szModuleFile);
      m_lcModuleList.SetItemText(i, 1, szModuleName);
      m_lcModuleList.SetItemText(i, 2, szModuleDesc);
      m_lcModuleList.SetItemText(i, 3, szModuleTime);

      objvecModuleInfo.push_back(ModuleInfo);
    }
  }
}

void CDlgClientMain::OnBnClickedButton4()
{
  //ж��ģ��
  CString strModuleName;

  char szModuleName[100]        = {'\0'};

  m_txtModuleName.GetWindowText(strModuleName);

  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, strModuleName.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, nSrcLen, szModuleName, 100, NULL,NULL);
  szModuleName[nDecLen] = '\0';

  if(strlen(szModuleName) == 0)
  {
    MessageBox(_T(MESSAGE_INSERT_NULL) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s UnLoadModule %s", m_pTcpClientConnect->GetKey(), szModuleName);
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[10 * 1024] = {'\0'};
  int nRecvLen = 10 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    int nStrLen      = 0;
    int nPos         = 4;
    int nResult      = 0;
    memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    if(nResult == 0)
    {
      MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    }
    else
    {
      MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
    }
  }

  OnBnClickedButton3();
}

void CDlgClientMain::OnBnClickedButton5()
{
  //����ģ��
  CString strModuleName;

  char szModuleName[100]        = {'\0'};

  m_txtModuleName.GetWindowText(strModuleName);

  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, strModuleName.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, nSrcLen, szModuleName, 100, NULL,NULL);
  szModuleName[nDecLen] = '\0';

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ReLoadModule %s", m_pTcpClientConnect->GetKey(), szModuleName);
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[10 * 1024] = {'\0'};
  int nRecvLen = 10 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    int nStrLen      = 0;
    int nPos         = 4;
    int nResult      = 0;
    memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    if(nResult == 0)
    {
      MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    }
    else
    {
      MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
    }
  }

  OnBnClickedButton3();
}

void CDlgClientMain::OnBnClickedButton7()
{
	//����ָ��ģ��
	CString strModuleFile;

	char szModuleFile[100]        = {'\0'};

	m_txtModuleFile.GetWindowText(strModuleFile);

	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleFile, strModuleFile.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleFile, nSrcLen, szModuleFile, 100, NULL,NULL);
	szModuleFile[nDecLen] = '\0';

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s LoadModule %s", m_pTcpClientConnect->GetKey(), szModuleFile);
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
		return;
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
		nPos += sizeof(int);

		if(nResult == 0)
		{
			MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
		}
		else
		{
			MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
		}
	}

	OnBnClickedButton3();
}
