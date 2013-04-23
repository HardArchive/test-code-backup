#pragma once

class CThreadBase
{

public:
	CThreadBase();
	~CThreadBase();

	void	CloseThread();
	bool	OpenThread(LPVOID pParam = NULL);

	void    ReturnThreadInfo();
	bool    IsRunThread()		{ return m_bRun; }

	static   DWORD  WINAPI ThreadProc(LPVOID lpParameter);
	virtual  bool   Initialization()= 0;
	virtual	 void   WorkThread(LPVOID pParam = NULL) = 0;

public:
	HANDLE          m_hThread;
	HANDLE          m_hMutex;
	volatile bool	m_bRun;
	void*           m_ThreadParam;
protected:
	DWORD           m_nThreadId;
	volatile bool   m_bStop;
};


