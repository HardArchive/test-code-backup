#pragma once
#include "stdafx.h"
#include "ListenSocket.h"

//HMODULE hModuleKernel32 = NULL;
typedef BOOL WINAPI QUEUEUSERWORKITEM(LPTHREAD_START_ROUTINE, PVOID Context, ULONG Flags);
extern QUEUEUSERWORKITEM *fpQueueUserWorkItem;

typedef struct tagRegParam
{
	//Database.
	CString strDB4IP;
	CString strDBName4RemoteCenter;
	CString strDBName4Primary2004;
	CString strDB4Uid;
	CString strDB4Pwd;
	DWORD nListeningPort;
	BOOL bRecvOK;
}REG_PARAM;

// CWorkThread
class CWorkThread : public CWinThread
{
	DECLARE_DYNCREATE(CWorkThread)

protected:
	CWorkThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CWorkThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Accept(CListenSocket& ListenSocket);// 响应SOCKET接受消息
	BOOL OpenDatabase();					 // 打开数据库
	BOOL CloseDatabase();					 // 关闭数据库

	static _ConnectionPtr m_pConn4RemoteCenter;
	static _ConnectionPtr m_pConn4Primary2004;

protected:
	HANDLE m_hPoolMutex;			// 确保只有一个线程实例运行
	CListenSocket m_ListenSocket;	// 侦听套接字
	int m_nPort;					// 侦听端口号

};


