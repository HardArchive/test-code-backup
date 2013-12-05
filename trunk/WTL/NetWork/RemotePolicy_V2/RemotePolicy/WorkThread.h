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
	CWorkThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWorkThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Accept(CListenSocket& ListenSocket);// ��ӦSOCKET������Ϣ
	BOOL OpenDatabase();					 // �����ݿ�
	BOOL CloseDatabase();					 // �ر����ݿ�

	static _ConnectionPtr m_pConn4RemoteCenter;
	static _ConnectionPtr m_pConn4Primary2004;

protected:
	HANDLE m_hPoolMutex;			// ȷ��ֻ��һ���߳�ʵ������
	CListenSocket m_ListenSocket;	// �����׽���
	int m_nPort;					// �����˿ں�

};


