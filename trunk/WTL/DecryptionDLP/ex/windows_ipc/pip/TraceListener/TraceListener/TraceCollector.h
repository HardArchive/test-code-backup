//---------------------------------------------------------------------------
//
//	"TraceCollector.h"
//
//  listening to multiple named pipes
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------



#ifndef __TRACECOLLECTOR_H__
#define __TRACECOLLECTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "PipeListener.h"
#include "CriticalSection.h"


typedef BOOL (*TraceCallBackFkt)(const char* pStr, DWORD nSize);
typedef std::list<CPipeListener*> TPipeListenerList;



class CTraceCollector : public IPipeDataDest 
{
public:
	CTraceCollector(TraceCallBackFkt callBackFkt);
	virtual ~CTraceCollector();

	bool Run();

protected:
	virtual void OnConnectingPipe();
	virtual void OnDisConnectingPipe(CPipeListener* pListener);

	virtual void OnIncomingData(const char* pStr, DWORD nSize);

private:
	bool RunPipeReader();

	TPipeListenerList	m_PipeListenerList;
	CCriticalSection	m_CriticalSectionData;
	CCriticalSection	m_CriticalSectionSys;
	TraceCallBackFkt	m_CallBack;
};

#endif
