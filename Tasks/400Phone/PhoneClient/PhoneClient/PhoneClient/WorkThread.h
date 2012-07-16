#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

// �����̻߳���
class CWorkThread
{
public:
	// ���캯��
	CWorkThread();
	// ���캯��
	CWorkThread(void* pParam);
	// ��������
	virtual ~CWorkThread();
	// Operations
public:
	// ��ʼһ���߳�
	int                Start();
	// ֹͣ�߳�
	void               Stop();
	// �����߳����ȼ�
	void               SetPriority(int nPriority){ SetThreadPriority(m_hThread, nPriority); }
	// ��ֹ�߳�
	void               Terminate();
	// �Ƿ���������
	bool               IsRunning() { return m_bIsRunning; }
	// ��ȡ�߳̾��
	HANDLE             GetHandle(){ return m_hThread; }
	// ��ȡ�߳�ID
	UINT               GetID(){ return m_nThreadID; }
protected:
	// �Ƿ��˳��߳�
	bool m_bQuit;
	// �߳��������ָ��
	void* m_pParam;
	virtual int        DoWork(){return 0;}
	virtual void       DoStop(){ if (!m_bQuit) m_bQuit = true; }
	// ��ʼ���߳�
	virtual bool       InitInstance(){return true;}
	// �˳��߳�
	virtual int        ExitInstance(){return 0;}
private:
	// �߳̾��
	HANDLE             m_hThread;
	// �߳�ID
	UINT               m_nThreadID;
	// �Ƿ���������
	bool               m_bIsRunning;
	// ���߳�������
	static DWORD WINAPI MainThreadProc( LPVOID lpParam );
};
#endif