//---------------------------------------------------------------------------
//
//	"TraceCollector.cpp"
//
//  listening to multiple named pipes
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "TraceCollector.h"

#include "PipeListener.h"


CTraceCollector::CTraceCollector(TraceCallBackFkt callBackFkt)
{
	m_CallBack = callBackFkt;
}

CTraceCollector::~CTraceCollector()
{
	CAutoCriticalSection cas(m_CriticalSectionSys);
	for (TPipeListenerList::iterator it = m_PipeListenerList.begin(); it != m_PipeListenerList.end(); it++)
	{
		delete (*it);
	}
}

bool CTraceCollector::Run()
{
	return RunPipeReader();
}

bool CTraceCollector::RunPipeReader()
{
	CPipeListener* pPipeListener = new CPipeListener(this);

	CAutoCriticalSection cas(m_CriticalSectionSys);
	m_PipeListenerList.push_back(pPipeListener);

	return pPipeListener->StartReader();
}

void CTraceCollector::OnConnectingPipe()
{
	RunPipeReader();
}

void CTraceCollector::OnDisConnectingPipe(CPipeListener* pListener)
{
	m_CriticalSectionSys.Lock();
	m_PipeListenerList.remove(pListener);
	m_CriticalSectionSys.Unlock();
	delete pListener;
}

void CTraceCollector::OnIncomingData(const char* pStr, DWORD nSize)
{
	CAutoCriticalSection cas(m_CriticalSectionData);
	if (m_CallBack)
		m_CallBack(pStr, nSize);
}