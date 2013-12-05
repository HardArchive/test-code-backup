// RemotePolicy.cpp : 定义应用程序的类行为。
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


// CRemotePolicyApp 构造

CRemotePolicyApp::CRemotePolicyApp(): CNTService("RemotePolicy","RemotePolicy")
{
	// NTService 初始化
	m_dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	m_pszDependencies = _T("MSSQLSERVER\0");
	m_dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
	m_hStop = NULL;

	// 初始化成员变量
	m_hServerMutex = NULL;
}
// 唯一的一个 CRemotePolicyApp 对象

CRemotePolicyApp theApp;


// CRemotePolicyApp 初始化

BOOL CRemotePolicyApp::InitInstance()
{
	CWinApp::InitInstance();
	::OutputDebugString(_T("RemotePolicy InitInstance\n"));

	// 使用Mutex确保程序只有一个实例运行
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

	// Socket 初始化
	if(!AfxSocketInit()) return FALSE;

	//载入注册表参数
/*	if(LoadRegistry())
	{
		//注册表的接收参数载入成功
		if(m_RegParam.bRecvOK)
		{
			//线程数加1，开始接收客户端请求--CPoolThread
//			dwThreads++;
			TRACE(_T("Begin Listening Client Thread!\n"));
//			m_pPoolThread = (CPoolThread*)AfxBeginThread(RUNTIME_CLASS(CPoolThread));
		}
		else
		{
			//参数出错处理
			TRACE(_T("One of Receiver param is error,so the NDReceiver exit!\n"));
			return FALSE;
		}
	}
	else
	{
		//载入注册表出错处理
		TRACE(_T("Read Registory error!\n"));
		return FALSE;
	}	
*/
	// NTSercie 注册服务
	RegisterService(__argc, __argv);
	return TRUE;
}

// CRemotePolicyApp 清理
int CRemotePolicyApp::ExitInstance()
{
	// 关闭Mutex
	if(m_hServerMutex!=NULL)
	{
		CloseHandle(m_hServerMutex);
	}

	::OutputDebugString(_T("RemotePolicy ExitInstance\n"));
	return CWinApp::ExitInstance();
}


////////////////////////////////////////////////////////////////////////////
// NTService 虚函数
void CRemotePolicyApp::Run(DWORD dwArgc, LPTSTR * ppszArgv)
{
	::OutputDebugString(_T("NDIdentity Run!\n"));
	ReportStatus(SERVICE_START_PENDING);
	
	::OutputDebugString(_T("Begin Listening Client Thread!\n"));
	m_pWorkThread = (CWorkThread*)AfxBeginThread(RUNTIME_CLASS(CWorkThread));

	//定义句柄数组(元素个数为线程数加1)
	HANDLE* pHandle = new HANDLE[2];
	if(pHandle == NULL)
		return;

	//线程数组赋值
	pHandle[0] = m_hStop;
	pHandle[1] = m_pWorkThread->m_hThread;

	//显示服务器运行信息
	ReportStatus(SERVICE_RUNNING);
	//互斥等待
	DWORD dwRet = WaitForMultipleObjects(2, pHandle, FALSE, INFINITE);

	if(dwRet == WAIT_OBJECT_0) //停止消息
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

// NTService 虚函数重写，用于响应NDBrowser发出，经由SMC转发的消息
void CRemotePolicyApp::UserControlFunc()
{
	//// ...自定义功能

}

//
BOOL CRemotePolicyApp::LoadRegistry()
{
	CRegKey Reg;
	TCHAR szData[MAX_PATH] = {0};
	ULONG uCount = 0;
	
	//打开注册表项
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

	//Common:公用参数
	//WaitTime等待延时
	if(Reg.QueryDWORDValue(_T("WaitTime"),m_RegParam.dwWaitTime) != ERROR_SUCCESS)
	{
		Reg.Close();
		TRACE(_T("CNDIdentityApp::LoadRegistry() --> Read WaitTime error!\n"));
		return FALSE;
	}
	//BufferSize缓冲区大小
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
	//关闭注册表
	Reg.Close();
	return true;	
}