#include "stdafx.h"
#include "WaitFor.h"
#include "GeneralHelper.h"
#include "CriticalSection.h"

//֧�� timeGetTime() ����
#ifdef _WIN32_WCE
	#include <Mmsystem.h>
	#pragma comment(lib, "Mmtimer")
#else
	#include <MmSystem.h>
	#pragma comment(lib, "Winmm")
#endif

/*
//class CShareUnSetEvent����WaitWithMessageLoop()��������Ϊ�������Զ�ȴ����¼�
//������߳�ͬʱ����WaitWithMessageLoop()����ʱ, ����ÿ���̶߳�����һ���¼�����
class CShareUnSetEvent
{
public:
	CShareUnSetEvent()
	{
		CCriSecLock lock(m_csEvent);
		if(!m_hEvent)
			m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		ASSERT(m_hEvent);.
		++m_dwRef;
	}

	~CShareUnSetEvent()
	{
		CCriSecLock lock(m_csEvent);
		if((--m_dwRef) == 0)
		{
			::CloseHandle(m_hEvent);
			m_hEvent = NULL;
		}
	}

	HANDLE* operator &() {return &m_hEvent;}

private:
	CShareUnSetEvent(const CShareUnSetEvent&);
	CShareUnSetEvent operator = (const CShareUnSetEvent&);

private:
	static CCriSec	m_csEvent;
	static DWORD	m_dwRef;
	static HANDLE	m_hEvent;
};

CCriSec	CShareUnSetEvent::m_csEvent;
DWORD	CShareUnSetEvent::m_dwRef	= 0;
HANDLE	CShareUnSetEvent::m_hEvent	= NULL;
*/

BOOL PeekMessageLoop(BOOL bDispatchQuitMsg)
{
	BOOL value = TRUE;

	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT && !bDispatchQuitMsg)
		{
			value = FALSE;
			break;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return value;
}

DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	DWORD dwResult		= WAIT_FAILED;
	DWORD dwBeginTime	= (dwMilliseconds == INFINITE) ? INFINITE : ::timeGetTime();

	while(TRUE)
	{
		int iWaitTime;
		if(dwBeginTime != INFINITE)
		{
			iWaitTime	= dwMilliseconds - (::timeGetTime() - dwBeginTime);

			if(iWaitTime <= 0)
			{
				dwResult = WAIT_TIMEOUT;
				break;
			}
		}
		else
			iWaitTime	= INFINITE;

		dwResult = ::MsgWaitForMultipleObjectsEx(dwHandles, szHandles, iWaitTime, dwWakeMask, dwFlags);
		ASSERT(dwResult != WAIT_FAILED);

		if(dwResult == (WAIT_OBJECT_0 + dwHandles))
			PeekMessageLoop();
		else
			break;
		
	}

	return dwResult;

}

BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	DWORD dwResult = WaitForMultipleObjectsWithMessageLoop(1, &hHandle, dwMilliseconds, dwWakeMask, dwFlags);

	switch(dwResult)
	{
	case WAIT_OBJECT_0:
		return TRUE;
	case WAIT_FAILED:
		ASSERT(FALSE);
	case WAIT_TIMEOUT:
		return FALSE;
	default:
		ASSERT(FALSE);
	}

	return FALSE;
}

void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
	static CEvt evWait;

	VERIFY(MsgWaitForSingleObject(evWait, dwMilliseconds, dwWakeMask, dwFlags) == FALSE);
}

void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
	while(*plWorkingItemCount > lMaxWorkingItemCount)
		::Sleep(dwCheckInterval);
}

void WaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
	WaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}

void MsgWaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
	while(*plWorkingItemCount > lMaxWorkingItemCount)
		WaitWithMessageLoop(dwCheckInterval);
}

void MsgWaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
	MsgWaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}
