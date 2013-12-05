#pragma once

namespace WorkItem
{
	typedef struct rawThreadData
	{
		DWORD dwThreadID;
		SOCKET socket;
	}THREADDATA;

	extern DWORD g_nEventTime;									// ͨ��Socket�¼��ĵȴ�ʱ��
	extern CArray<THREADDATA,THREADDATA&> g_arrayWorking;		// ����̳߳��̹߳����߳�ID����

	extern int g_nJobMaxNum;				// �̳߳�������߳���
//	extern int g_nJobGuardNum;				// �̳߳ط�æ�ľ�����Ŀ
//	extern int g_nAutoDisconnectNum;		// �����ﾯ����Ŀ֮��ǰg_nAutoDisconnectNum����߳������ر�����
	extern bool g_AllExit;
	

	//Operation
	// �̳߳ع������̣߳���Win2000����°�ʹ�ã���QueueUserWorkItem()����
	DWORD WINAPI ClientWorkItemThread(LPVOID lpParameter);	

	// ͨ�Ź����̣߳���WinNTʹ�ã���AfxBeginThread����
	UINT ClientWorkThread(LPVOID lpParameter);
	// ͨ�Ź�����ʵ�ֺ���
	DWORD ClientWorkFunction(LPVOID lpParameter);	

	// ��ʼ���̳߳�״̬��CPoolThreadʵ������ʱ����
	bool InitPool();
	// �����̳߳�״̬��CPoolThreadʵ������ʱ����
	void ClearPool();

	// �ж��̳߳��Ƿ�������CListenSocket�����¼������������������ǵ���
	bool IsPoolFull();

	//��ӹ����̵߳�g_arrayWorking
	void AddItemRef(THREADDATA &ThreadData);	

	//��g_arrayWorking��ɾ�������߳�
	void ReleaseItem(THREADDATA &ThreadData);

	//�ж��Ƿ�Ҫ�����ر�����
//	bool AutoDisconnect(THREADDATA &ThreadData);
//	bool CloseSocket(SOCKET &WorkSocket);
	//bool ComOfWork(CWorkProc &workproc,void *pData,long lHostIP);
}