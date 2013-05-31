#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

#include "ace/Task.h"
#include <stdio.h>
#include <stdarg.h>
#include "define.h"
#include "MapTemplate.h"
#include "ILogObject.h"
#include "ILogManager.h"

class CLogManager : public ACE_Task<ACE_MT_SYNCH>, public ILogManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	virtual int open (void *args = 0);
	virtual int svc (void);
	int Close();

	void Init(int nThreadCount = 1, int nQueueMax = MAX_MSG_THREADQUEUE);
	int Start();
	int Stop();
	bool IsRun();

	
	int PutLog(int nLogType, ACE_TString *pLogText);
	int RegisterLog(CServerLogger * pServerLogger);
	int UnRegisterLog(CServerLogger * pServerLogger);

	int WriteLog(int nLogType, const char* fmt, ...);

private:
	int ProcessLog(int nLogType, ACE_TString *pLogText);

private:
	bool                              m_blRun;
	int                               m_nThreadCount;
	int                               m_nQueueMax;
	CMapTemplate<int, CServerLogger>  m_mapServerLogger;
	ACE_Recursive_Thread_Mutex        m_Logger_Mutex;
};


typedef ACE_Singleton<CLogManager, ACE_Recursive_Thread_Mutex> AppLogManager; 


#endif
