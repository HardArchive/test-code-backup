// RemotePolicy.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RemotePolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRemotePolicyApp

BEGIN_MESSAGE_MAP(CRemotePolicyApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRemotePolicyApp ����

CRemotePolicyApp::CRemotePolicyApp(): CNTService("RemotePolicy","RemotePolicy")
{
	// NTService ��ʼ��
	m_dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	m_pszDependencies = _T("MSSQLSERVER\0");
	m_dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
	m_hStop = NULL;

	// ��ʼ����Ա����
	m_hServerMutex = NULL;
}
// Ψһ��һ�� CRemotePolicyApp ����

CRemotePolicyApp theApp;


// CRemotePolicyApp ��ʼ��

BOOL CRemotePolicyApp::InitInstance()
{
	CWinApp::InitInstance();
	::OutputDebugString(_T("RemotePolicy InitInstance\n"));

	// ʹ��Mutexȷ������ֻ��һ��ʵ������
	m_hServerMutex = ::CreateMutex(NULL, FALSE,"RemotePolicy_Mutex");
	if(m_hServerMutex==NULL)
	{
		return FALSE;
	}
	else
	{
		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{
			::OutputDebugString(_T("RemotePolicy is already running!\n"));
			return FALSE;
		}
	}

	m_hStop = CreateEvent(0, TRUE, FALSE, 0);
	if(m_hStop == NULL)	return FALSE;

	// Socket ��ʼ��
	if(!AfxSocketInit()) return FALSE;

	//����ע������
/*	if(LoadRegistry())
	{
		//ע���Ľ��ղ�������ɹ�
		if(m_RegParam.bRecvOK)
		{
			//�߳�����1����ʼ���տͻ�������--CPoolThread
//			dwThreads++;
			TRACE(_T("Begin Listening Client Thread!\n"));
//			m_pPoolThread = (CPoolThread*)AfxBeginThread(RUNTIME_CLASS(CPoolThread));
		}
		else
		{
			//����������
			TRACE(_T("One of Receiver param is error,so the NDReceiver exit!\n"));
			return FALSE;
		}
	}
	else
	{
		//����ע��������
		TRACE(_T("Read Registory error!\n"));
		return FALSE;
	}	
*/
	// NTSercie ע�����
	RegisterService(__argc, __argv);
	return TRUE;
}

// CRemotePolicyApp ����
int CRemotePolicyApp::ExitInstance()
{
	// �ر�Mutex
	if(m_hServerMutex!=NULL)
	{
		CloseHandle(m_hServerMutex);
	}

	::OutputDebugString(_T("RemotePolicy ExitInstance\n"));
	return CWinApp::ExitInstance();
}


////////////////////////////////////////////////////////////////////////////
// NTService �麯��
void CRemotePolicyApp::Run(DWORD dwArgc, LPTSTR * ppszArgv)
{
	::OutputDebugString(_T("NDIdentity Run!\n"));
	ReportStatus(SERVICE_START_PENDING);
	
	::OutputDebugString(_T("Begin Listening Client Thread!\n"));
	m_pWorkThread = (CWorkThread*)AfxBeginThread(RUNTIME_CLASS(CWorkThread));

	//����������(Ԫ�ظ���Ϊ�߳�����1)
	HANDLE* pHandle = new HANDLE[2];
	if(pHandle == NULL)
		return;

	//�߳����鸳ֵ
	pHandle[0] = m_hStop;
	pHandle[1] = m_pWorkThread->m_hThread;

	//��ʾ������������Ϣ
	ReportStatus(SERVICE_RUNNING);
	//����ȴ�
	DWORD dwRet = WaitForMultipleObjects(2, pHandle, FALSE, INFINITE);

	if(dwRet == WAIT_OBJECT_0) //ֹͣ��Ϣ
	{
		m_pWorkThread->PostThreadMessage(WM_QUIT,0,0);
		::WaitForSingleObject(m_pWorkThread->m_hThread,28 * 1000);
	}

	if(m_hStop != NULL)
	{
		CloseHandle(m_hStop);
		m_hStop = NULL;
	}

	delete[] pHandle;
}

// NTService service stop
void CRemotePolicyApp::Stop()
{
	// NTService
	if(m_hStop != NULL)
	{
		SetEvent(m_hStop);
	}
	
	ReportStatus(SERVICE_STOP_PENDING);
	::OutputDebugString(_T("Receiver Stop Command,so exit!\n"));
}

// NTService �麯����д��������ӦNDBrowser����������SMCת������Ϣ
void CRemotePolicyApp::UserControlFunc()
{
	//// ...�Զ��幦��

}

//
BOOL CRemotePolicyApp::LoadRegistry()
{
	CRegKey Reg;
	TCHAR szData[MAX_PATH] = {0};
	ULONG uCount = 0;
	
	//��ע�����
	if(Reg.Open(HKEY_LOCAL_MACHINE,lpSubKey) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Open Registery error!\n"));
		return FALSE;
	}

	//Database IP
	uCount = MAX_PATH * sizeof(TCHAR);
	ZeroMemory(szData,uCount);
	if(Reg.QueryStringValue(_T("DB4IP"),szData,&uCount) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read ServerIP error!\n"));
		return FALSE;
	}
	m_RegParam.strDB4IP = szData;
	
	//Database Uid.
	uCount = MAX_PATH * sizeof(TCHAR);
	ZeroMemory(szData,uCount);
	if(Reg.QueryStringValue(_T("DB4Uid"),szData,&uCount) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read Database error!\n"));
		return FALSE;
	}
	m_RegParam.strDB4Uid = szData;

	//Database 4 Pwd
	uCount = MAX_PATH * sizeof(TCHAR);
	ZeroMemory(szData,uCount);
	if(Reg.QueryStringValue(_T("DB4Pwd"),szData,&uCount) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read UID error!\n"));
		return FALSE;
	}
	m_RegParam.strDB4Pwd = szData;

	//Database 4 Primary2004
	uCount = MAX_PATH * sizeof(TCHAR);
	ZeroMemory(szData,uCount);
	if(Reg.QueryStringValue(_T("DB4Primary2004"),szData,&uCount) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDReceiverApp::LoadRegistry() --> Read Password error!\n"));
		return FALSE;
	}
	m_RegParam.strDBName4Primary2004 = szData;	

	//Database 4 RemoteCenter
	uCount = MAX_PATH * sizeof(TCHAR);
	ZeroMemory(szData,uCount);
	if(Reg.QueryStringValue(_T("DB4RemoteCenter"),szData,&uCount) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDReceiverApp::LoadRegistry() --> Read Password error!\n"));
		return FALSE;
	}
	m_RegParam.strDBName4RemoteCenter = szData;	

	//Port
	if(Reg.QueryDWORDValue(_T("ListeningPort"),m_RegParam.nListeningPort) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read Port error!\n"));
		return FALSE;
	}
	
	

/*	//CheckTime
	if(Reg.QueryDWORDValue(_T("CheckTime"),m_RegParam.dwCheckTime) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read CheckTime error!\n"));
		return FALSE;
	}

	//Common:���ò���
	//WaitTime�ȴ���ʱ
	if(Reg.QueryDWORDValue(_T("WaitTime"),m_RegParam.dwWaitTime) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read WaitTime error!\n"));
		return FALSE;
	}
	//BufferSize��������С
	if(Reg.QueryDWORDValue(_T("BufferSize"),m_RegParam.dwBufferSize) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read BufferSize error!\n"));
		return FALSE;
	}
	//FailureTimes
	if(Reg.QueryDWORDValue(_T("FailureTimes"),m_RegParam.dwFailureTimes) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read FailureTimes error!\n"));
		return FALSE;
	}
	
	if(m_RegParam.strDBName.IsEmpty() || m_RegParam.strDBIP.IsEmpty())
	{
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Database Name or Database IP is empty,so can't startup!\n"));
		m_RegParam.bRecvOK = false;
	}
	else
	{
		m_RegParam.bRecvOK = true;
		if(m_RegParam.strSendDestIP.IsEmpty())
		{
			m_RegParam.bSendOK = false;
		}
		else
			m_RegParam.bSendOK = true;
	}
*/
	m_RegParam.bRecvOK = 1;
	//�ر�ע���
	Reg.Close();
	return true;	
}