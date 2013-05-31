// LogManager.h
// Log的管理类，负责Log日志的对象管理。
// 一开始想把它作为一个DLL单独实现，但是考虑一下，还是放在框架里面实现比较好。
// add by freeeyes
// 2009-04-04

#include "LogManager.h"

Mutex_Allocator _log_service_mb_allocator; 

CLogManager::CLogManager(void)
{
	m_blRun         = false;
	m_nThreadCount  = 1;
	m_nQueueMax     = MAX_MSG_THREADQUEUE;
}

CLogManager::~CLogManager(void)
{
	OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager].\n"));
	m_mapServerLogger.Clear();
	OUR_DEBUG((LM_ERROR,"[CLogManager::~CLogManager]End.\n"));
}

int CLogManager::open(void *args)
{
	if(args != NULL)
	{
		OUR_DEBUG((LM_ERROR,"[CLogManager::open]args is not NULL.\n"));
	}
	
	if(activate(THR_NEW_LWP | THR_DETACHED, m_nThreadCount) == -1)
	{
		m_blRun = false;
		OUR_DEBUG((LM_ERROR,"[CLogManager::open] activate is error[%d].", errno));
		return -1;
	}

	m_blRun = true;
	return 0;
}

int CLogManager::svc(void)
{
	OUR_DEBUG((LM_INFO,"[CLogManager::svc] svc run.\n"));
	ACE_Message_Block* mb = NULL;
	//ACE_Time_Value     xtime;
	while(m_blRun)
	{
		mb = NULL;
		//xtime=ACE_OS::gettimeofday()+ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::svc] get error errno = [%d].\n", errno));
			m_blRun = false;
			break;
		}

		if (mb == NULL)
		{
			continue;
		}

		ACE_TString* pstrLogText = *((ACE_TString**)mb->base());
		if (!pstrLogText)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::svc] CLogManager mb log == NULL!\n"));
			mb->release();
			continue;
		}

		ProcessLog(mb->msg_type(), pstrLogText);
		mb->release();
		//OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText BEGIN!\n"));
		delete pstrLogText;
		//OUR_DEBUG((LM_ERROR,"[CLogManager::svc] delete pstrLogText END!\n"));

	}
	OUR_DEBUG((LM_INFO,"[CLogManager::svc] CLogManager::svc finish!\n"));
	return 0;
}

int CLogManager::Close()
{
	msg_queue()->deactivate();
	msg_queue()->flush();
	return 0;
}

void CLogManager::Init(int nThreadCount, int nQueueMax)
{
	m_nThreadCount = nThreadCount;
	m_nQueueMax    = nQueueMax;
}

int CLogManager::Start()
{
	if(m_blRun == true)
	{
		return 0;
	}
	else
	{
		return open();
	}
	
}

int CLogManager::Stop()
{
	m_blRun = false;
	return 0;
}

bool CLogManager::IsRun()
{
	return m_blRun;
}

int CLogManager::PutLog(int nLogType, ACE_TString *pLogText)
{
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN (mb,
		static_cast<ACE_Message_Block*>(_log_service_mb_allocator.malloc (sizeof (ACE_Message_Block))),
		ACE_Message_Block
		(	sizeof(ACE_TString *), // size
		nLogType,
		0,
		0,
		&_log_service_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_log_service_mb_allocator,
		&_log_service_mb_allocator
		)
		);		

	if(mb)
	{
		ACE_TString** loadin = (ACE_TString **)mb->base();
		*loadin = pLogText;

		int msgcount = (int)msg_queue()->message_count();
		if (msgcount >= m_nQueueMax) 
		{
			OUR_DEBUG((LM_INFO,"[CLogManager::PutLog] CLogManager queue is full!\n"));
			mb->release();
			return 1;
		}
		ACE_Time_Value xtime;
		xtime = ACE_OS::gettimeofday();
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager putq error(%s)!\n", pLogText->c_str()));
			mb->release();
			return -1;
		}
		return 0;
	}

	OUR_DEBUG((LM_ERROR,"[CLogManager::PutLog] CLogManager new ACE_Message_Block error!\n"));	
	return -1;
}

int CLogManager::RegisterLog(CServerLogger* pServerLogger)
{
	if(pServerLogger == NULL)
	{
		return -1;
	}

	int nLogTypeCount = pServerLogger->GetLogTypeCount();

	for(int i = 0; i < nLogTypeCount; i++)
	{
		int nLogType = pServerLogger->GetLogType(i);
		if(nLogType <= 0)
		{
			continue;
		}

		m_mapServerLogger.AddMapData(nLogType, pServerLogger);
	}

	return 0;
}

int CLogManager::UnRegisterLog(CServerLogger* pServerLogger)
{
	CServerLogger* pTempServerLogger = NULL;
	vector<int>    vecDel;
	int            i = 0;

	int nLogTypeCount = m_mapServerLogger.GetSize();
	for(i = 0; i < nLogTypeCount; i++)
	{
		pTempServerLogger = m_mapServerLogger.GetMapData(i);
		if(pTempServerLogger == pServerLogger)
		{
			int nLogType = m_mapServerLogger.GetMapDataKey(i);
			vecDel.push_back(nLogType);
		}
	}

	for(i = 0; i < (int)vecDel.size(); i++)
	{
		m_mapServerLogger.DelMapData(vecDel[i]);
	}

	return 0;
}

int CLogManager::ProcessLog(int nLogType, ACE_TString* pLogText)
{
	CServerLogger* pServerLogger = NULL;

	pServerLogger = m_mapServerLogger.SearchMapData(nLogType);
	if(NULL == pServerLogger)
	{
		return -1;
	}

	m_Logger_Mutex.acquire();
	pServerLogger->DoLog(nLogType, pLogText);
	m_Logger_Mutex.release();
	return 0;
}

//*****************************************************************************

int CLogManager::WriteLog(int nLogType, const char* fmt, ...)
{
	int  nRet = 0;
	char szTemp[MAX_BUFF_1024*5] = {'\0'};
	va_list ap;
	va_start(ap, fmt);
	ACE_OS::vsnprintf(szTemp,sizeof(szTemp)-1,fmt, ap);
	va_end(ap);
	ACE_TString* pstrLog= new ACE_TString(szTemp,ACE_OS::strlen(szTemp));
	if (pstrLog)
	{

		if (IsRun()) 
		{
			nRet = PutLog(nLogType, pstrLog);
			if (nRet) 
			{
				delete pstrLog;
				pstrLog = NULL;
			}
		} 
		else 
		{
			delete pstrLog;
			pstrLog = NULL;
		}
	}
	else 
	{
		nRet = -1;
	}
	return nRet;
}

