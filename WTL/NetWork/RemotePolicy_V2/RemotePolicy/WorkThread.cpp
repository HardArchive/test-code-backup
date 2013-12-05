// WorkThread.cpp : 实现文件
//

#include "stdafx.h"
#include "RemotePolicy.h"
#include "WorkThread.h"
#include ".\workthread.h"

#include "WorkItem.h"
// CWorkThread

HMODULE hModuleKernel32 = NULL;
			// Kernel32.dll模块句柄
QUEUEUSERWORKITEM *fpQueueUserWorkItem = NULL;
			// QueueUserWorkItem函数指针

//静态成员初始化
_ConnectionPtr CWorkThread::m_pConn4RemoteCenter = NULL;
_ConnectionPtr CWorkThread::m_pConn4Primary2004 = NULL;

IMPLEMENT_DYNCREATE(CWorkThread, CWinThread)

CWorkThread::CWorkThread()
//: m_DataBaseParam(0)
{
	fpQueueUserWorkItem = NULL;

	m_hPoolMutex = NULL;

	CRemotePolicyApp* pApp = static_cast<CRemotePolicyApp*>(AfxGetApp());
	if(pApp == NULL)
	{
		TRACE(_T("Get NDReceiverApp error,so exit!\n"));
		return;
	}

	m_nPort = pApp->m_RegParam.nListeningPort;
}

CWorkThread::~CWorkThread()
{
}

BOOL CWorkThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	m_hPoolMutex = CreateMutex(NULL, FALSE, lpstrPoolMutexName);
	if(m_hPoolMutex == NULL)
	{
		TRACE(_T("CPoolThread::InitInstance(), Pool Thread Exit!\n"));
		return FALSE;
	}
	else
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			TRACE(_T("CPoolThread:: WorkItem Mutex Already Existing, Pool Thread Exit!\n"));
			return FALSE;
		}
	}

	if(LOBYTE(GetVersion())==5)
	{
		hModuleKernel32 = LoadLibrary("Kernel32");
		if(hModuleKernel32!=NULL)
		{
			fpQueueUserWorkItem = (QUEUEUSERWORKITEM*)GetProcAddress(hModuleKernel32, "QueueUserWorkItem");
		}
	}

	// COM(ADO) Initialize.COM(ADO)初始化
	if(!SUCCEEDED(CoInitialize(NULL)))
	{
		TRACE(_T("CPoolThread:: CoInitialize Failed, Pool Thread Exit!\n"));
		return FALSE;
	}

	//打开数据库
	if(!OpenDatabase())
	{
		TRACE(_T("CPoolThread:: Open Database Failed, Pool Thread Exit!\n"));
		return FALSE;
	}

	if (!WorkItem::InitPool())
	{
		TRACE(_T("CPoolThread:: InitPool Failed to Create, Pool Thread Exit!\n"));
		return FALSE;
	}

	//Create Socket.创建指定端口的socket对象
	if(!m_ListenSocket.Create(50000))
	{
		TRACE(_T("CPoolThread:: ListenSocket Failed to Create, Pool Thread Exit!\n"));
		return FALSE;
	}

	// listen监听端口
	if(!m_ListenSocket.Listen())
	{
		TRACE(_T("CPoolThread:: Socket Listen Error, Pool Thread Exit!\n"));
		return FALSE;
	}
	return TRUE;
}

int CWorkThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	if(hModuleKernel32!=NULL)
	{
		FreeLibrary(hModuleKernel32);
	}
	CoUninitialize();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWorkThread, CWinThread)
END_MESSAGE_MAP()


// CWorkThread 消息处理程序
/*................无用...............*/
BOOL CWorkThread::Accept(CListenSocket& ListenSocket)
{
	CAsyncSocket asAccept;
	if(!ListenSocket.Accept(asAccept))
		return FALSE;

	CString strOut;
	strOut.Format(_T("CPoolThread::Accept() socket :%d Accept!\n"),asAccept.m_hSocket);
	TRACE(strOut);
	
	//线程池是否已满
	if(!WorkItem::IsPoolFull())
	{
		if(fpQueueUserWorkItem != NULL)
		{
			//
			fpQueueUserWorkItem(WorkItem::ClientWorkItemThread, (LPVOID)asAccept.Detach(), WT_EXECUTELONGFUNCTION);
		}
		else
		{
			//启动WorkItem类的ClientWorkThread线程
			AfxBeginThread(WorkItem::ClientWorkThread, (LPVOID)asAccept.Detach());
		}
	}
	else
	{
		// refuse request,close socket!拒绝客户端请求，关闭socket
		asAccept.Close();
	}

	return TRUE;
}
/*................无用...............*/

/*................无用...............*/
BOOL CWorkThread::OpenDatabase()
{
	//2004-09-30
	CRemotePolicyApp *pApp = static_cast<CRemotePolicyApp*>(AfxGetApp());
	if(pApp == NULL)
		return false;

/*	//Decode
	char *buf=NULL;

	if(pApp->m_RegParam.strPwd.GetLength()>10)
	{
		UCHAR *buf=NULL;   
		bool rr=DeDataFromBase64Code((BYTE*)(LPCTSTR)pApp->m_RegParam.strPwd,(BYTE**)&buf,(BYTE*)("Rain2004Soft"),pApp->m_RegParam.strPwd.GetLength());

		pApp->m_RegParam.strPwd.Format("%s",buf);//解过密的密码
		FreeDataPtr(buf);
		pApp->m_RegParam.strPwd.Trim();
		if(!rr)
		{
			::OutputDebugString("数据库登录密码解密失败！\n");
			::AfxMessageBox("数据库登录密码解密失败！\n");
			return false;
		}
	}
	//~Decode
*/
	//communication parameters.组装数据库通讯参数
	CString strConn4Primary2004,strConn4RemoteCenter;          //MS SQLSERVER

	//数据库4NDSPrimary2004
	strConn4Primary2004.Format(_T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s"),\
		pApp->m_RegParam.strDB4IP,pApp->m_RegParam.strDBName4Primary2004,pApp->m_RegParam.strDB4Uid,pApp->m_RegParam.strDB4Pwd);

	//数据库4NDRemoteCenter
	strConn4RemoteCenter.Format(_T("Provider=SQLOLEDB;Data Source=%s;Initial Catalog=%s;User ID=%s;Password=%s"),\
		pApp->m_RegParam.strDB4IP,pApp->m_RegParam.strDBName4RemoteCenter,pApp->m_RegParam.strDB4Uid,pApp->m_RegParam.strDB4Pwd);
	
	try
	{
		//Initialize for communication.调用CReceiverData类的Initialize函数
//		CString strSQL;

//		m_strConnection = strConnect;

		HRESULT hr;
		
		//两个数据库
		hr = m_pConn4Primary2004.CreateInstance(__uuidof(Connection));
		if(!SUCCEEDED(hr))
		{
			TRACE(_T("CPoolThread::OpenDatabase Failed to CreateInstance!\n"));
			return false;
		}

		hr = m_pConn4RemoteCenter.CreateInstance(__uuidof(Connection));
		if(!SUCCEEDED(hr))
		{
			TRACE(_T("CPoolThread::OpenDatabase Failed to CreateInstance!\n"));
			return false;
		}

		m_pConn4Primary2004->CursorLocation = adUseClient;
		m_pConn4Primary2004->CommandTimeout = 0;
		hr = m_pConn4Primary2004->Open(_bstr_t(strConn4Primary2004), _T(""), _T(""), adConnectUnspecified);
		if(!SUCCEEDED(hr))
		{
			TRACE(_T("NDIdentity: ADODB.Connection Failed to Open @ CPoolThread::OpenDatabase()\n"));
			//AfxMessageBox("数据库连接失败!请检查配置参数是否正确!");
			return false;
		}		

		m_pConn4RemoteCenter->CursorLocation = adUseClient;
		m_pConn4RemoteCenter->CommandTimeout = 0;
		hr = m_pConn4RemoteCenter->Open(_bstr_t(strConn4RemoteCenter), _T(""), _T(""), adConnectUnspecified);
		if(!SUCCEEDED(hr))
		{
			TRACE(_T("NDIdentity: ADODB.Connection Failed to Open @ CPoolThread::OpenDatabase()\n"));
			//AfxMessageBox("数据库连接失败!请检查配置参数是否正确!");
			return false;
		}		
	}
	catch(_com_error e)
	{
//		DealError(cdwRecvType,e,_T("NDIdentity: CPoolThread::OpenDatabase() error:"),_T(""));
		//AfxMessageBox("数据库连接失败!请检查配置参数是否正确!");
		return FALSE;
	}
	catch(...)
	{
		TRACE(_T("NDIdentity: Error Catched @ CPoolThread::OpenDatabase()!\n"));
		//AfxMessageBox("数据库连接失败!请检查配置参数是否正确!");
		return FALSE;
	}

	return TRUE;
	
}
/*................无用...............*/

/*................无用...............*/
BOOL CWorkThread::CloseDatabase()
{
	try
	{
		m_pConn4Primary2004->Close();
		m_pConn4RemoteCenter->Close();
	}
	catch(_com_error e)
	{
//		DealError(cdwRecvType,e,_T("CPoolThread::CloseDatabase() error:"),_T(""));
		return FALSE;
	}
	catch(...)
	{
//		DealError(cdwRecvType,_T("CPoolThread::CloseDatabase() error!\n"),true);
		return FALSE;
	}

	return TRUE;
}
/*................无用...............*/
