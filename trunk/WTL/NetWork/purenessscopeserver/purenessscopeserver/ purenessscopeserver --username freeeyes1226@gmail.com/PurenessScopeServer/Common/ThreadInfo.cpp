// MessageService.h
// ��¼�߳��е�һЩ״̬�������Լ�ͷ���
// д�����˵ʲô�ء��Ǻǣ�ʲô������˵�ˣ�����д�ɡ�
// add by freeeyes
// 2009-01-27

#include "ThreadInfo.h"

CThreadInfo::CThreadInfo(void)
{
}

CThreadInfo::~CThreadInfo(void)
{
	OUR_DEBUG((LM_ERROR, "[CThreadInfo::~CThreadInfo].\n"));
	Close();
	OUR_DEBUG((LM_ERROR, "[CThreadInfo::~CThreadInfo]End.\n"));
}

bool CThreadInfo::AddThreadInfo(uint32 u4ThreadID)
{
	_ThreadInfo* pThreadInfo = new _ThreadInfo();

	if(NULL == pThreadInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CThreadInfo::AddThreadInfo] pThreadInfo is NULL.\n"));
		return false;
	}

	pThreadInfo->m_u4ThreadID = u4ThreadID;

	mapThreadInfo::iterator f = m_mapThreadInfo.find(u4ThreadID);
	if(f != m_mapThreadInfo.end())
	{
		OUR_DEBUG((LM_ERROR, "[CThreadInfo::AddThreadInfo] u4ThreadID = %d is exist.\n", u4ThreadID));
		return false;
	}

	m_mapThreadInfo.insert(mapThreadInfo::value_type(u4ThreadID, pThreadInfo));
	return true;
}

void CThreadInfo::Close()
{
	for(mapThreadInfo::iterator b = m_mapThreadInfo.begin(); b != m_mapThreadInfo.end(); b++)
	{
		_ThreadInfo* pThreadInfo = (_ThreadInfo* )b->second;
		if(NULL != pThreadInfo)
		{
			delete pThreadInfo;
			pThreadInfo = NULL;
		}
	}

	m_mapThreadInfo.clear();
}

_ThreadInfo* CThreadInfo::GetThreadInfo(uint32 u4ThreadID)
{
	mapThreadInfo::iterator f = m_mapThreadInfo.find(u4ThreadID);

	if(f != m_mapThreadInfo.end())
	{
		_ThreadInfo* pThreadInfo = (_ThreadInfo* )f->second;
		return pThreadInfo;
	}
	else
	{
		return NULL;
	}
}

bool CThreadInfo::CloseThread(uint32 u4ThreadID)
{
	mapThreadInfo::iterator f = m_mapThreadInfo.find(u4ThreadID);

	if(f != m_mapThreadInfo.end())
	{
		_ThreadInfo* pThreadInfo = (_ThreadInfo* )f->second;
		SAFE_DELETE(pThreadInfo);
		m_mapThreadInfo.erase(f);
		return true;
	}
	else
	{
		return false;
	}
}

int CThreadInfo::GetThreadCount()
{
	return (int)m_mapThreadInfo.size();
}
