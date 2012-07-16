#ifndef _RAINSOFT_SENDQUEUE_H_
#define _RAINSOFT_SENDQUEUE_H_
#include <queue>
#include "Tool.h"
#include "WorkThread.h"

class CSendQueue:public CWorkThread
{
private:
	std::queue<FileInfo> m_List;
	CRITICAL_SECTION m_cs;
	HANDLE m_hEvent;
	char m_szServerIP[16];
public:
	CSendQueue()
	{
		ZeroMemory(m_szServerIP, sizeof(m_szServerIP));
		CTool::Get400RegConfig(TEXT("serverip"), m_szServerIP);
		::InitializeCriticalSection(&m_cs);
		m_hEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);

	}
	~CSendQueue()
	{
		::DeleteCriticalSection(&m_cs);
		if(m_hEvent!=NULL)
		{
			::CloseHandle(m_hEvent);
		}
		ZeroMemory(m_szServerIP, sizeof(m_szServerIP));
	}
	void AddFileInfo(FileInfo &fiInfo);
	BOOL SendFileInfo(FileInfo &fiInfo);
	virtual int DoWork();
};
#endif