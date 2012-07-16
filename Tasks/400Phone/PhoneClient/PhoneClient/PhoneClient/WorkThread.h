#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

// 工作线程基类
class CWorkThread
{
public:
	// 构造函数
	CWorkThread();
	// 构造函数
	CWorkThread(void* pParam);
	// 析构函数
	virtual ~CWorkThread();
	// Operations
public:
	// 开始一个线程
	int                Start();
	// 停止线程
	void               Stop();
	// 设置线程优先级
	void               SetPriority(int nPriority){ SetThreadPriority(m_hThread, nPriority); }
	// 终止线程
	void               Terminate();
	// 是否正在运行
	bool               IsRunning() { return m_bIsRunning; }
	// 获取线程句柄
	HANDLE             GetHandle(){ return m_hThread; }
	// 获取线程ID
	UINT               GetID(){ return m_nThreadID; }
protected:
	// 是否退出线程
	bool m_bQuit;
	// 线程输入参数指针
	void* m_pParam;
	virtual int        DoWork(){return 0;}
	virtual void       DoStop(){ if (!m_bQuit) m_bQuit = true; }
	// 初始化线程
	virtual bool       InitInstance(){return true;}
	// 退出线程
	virtual int        ExitInstance(){return 0;}
private:
	// 线程句柄
	HANDLE             m_hThread;
	// 线程ID
	UINT               m_nThreadID;
	// 是否正在运行
	bool               m_bIsRunning;
	// 主线程中运行
	static DWORD WINAPI MainThreadProc( LPVOID lpParam );
};
#endif