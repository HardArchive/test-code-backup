#include "stdafx.h"
#include "Synchronizer/EventSynchronizer.h"
#include <tchar.h>

EventSynchronizer::EventSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                                     BOOL bInheritHandle,
                                     BOOL bManualReset,BOOL bInitialState,
                                     LPCTSTR lpctszEventName)
{
    m_hSyncObject = ::OpenEvent(SYNCHRONIZE|EVENT_MODIFY_STATE,bInheritHandle,lpctszEventName);
    if( 0 != m_hSyncObject )
        return;
    m_hSyncObject = ::CreateEvent(lpSecAttrib,bManualReset,bInitialState,lpctszEventName);
    if( 0 == m_hSyncObject )
        throw SyncObjectCreationFailed(_T("Event creation failed"),GetLastError());
}

DWORD EventSynchronizer::Lock(DWORD dwTimeOut)
{
    DWORD dwWaitRet = ::WaitForSingleObject(m_hSyncObject,dwTimeOut);
    switch(dwWaitRet)
    {
    case WAIT_OBJECT_0:
        return WAIT_OBJECT_0;
    case WAIT_TIMEOUT:
        throw SyncObjectWaitTimeOut(_T("Event wait timedout"),GetLastError());
    case WAIT_FAILED:
        throw SyncObjectWaitFailed(_T("Event wait failed"),GetLastError());
    }
    return dwWaitRet;
}

DWORD EventSynchronizer::UnLock()
{
    return ERROR_SUCCESS;
}

BOOL EventSynchronizer::SetEvent()
{
    return ::SetEvent(m_hSyncObject);
}
BOOL EventSynchronizer::ResetEvent()
{
    return ::ResetEvent(m_hSyncObject);
}

EventSynchronizer::~EventSynchronizer(void)
{
    if (!m_hSyncObject )
        return;
    ::CloseHandle(m_hSyncObject);
}
