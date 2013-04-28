#include "stdafx.h"
#include "Synchronizer/SemaphoreSynchronizer.h"
#include <tchar.h>

SemaphoreSynchronizer::SemaphoreSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                                             BOOL bInheritHandle,
                                             LONG lIninitialCount,
                                             LONG lMaxCount,
                                             LONG lReleaseCount,
                                             LONG* plPrevCount,
                                             LPCTSTR lpctszSemaphoreName):
                                             m_lReleaseCount(lReleaseCount),
                                             m_plPrevCount(plPrevCount)
{
    m_hSyncObject = ::OpenSemaphore(SYNCHRONIZE|SEMAPHORE_MODIFY_STATE,bInheritHandle,
                                    lpctszSemaphoreName);
    if( 0 != m_hSyncObject )
        return;
    m_hSyncObject = ::CreateSemaphore(lpSecAttrib,lIninitialCount,
                                      lMaxCount,lpctszSemaphoreName);
    if( 0 == m_hSyncObject )
        throw SyncObjectCreationFailed(_T("Semaphore creation failed"),GetLastError());
}

DWORD SemaphoreSynchronizer::Lock(DWORD dwTimeOut)
{
    DWORD dwWaitRet = ::WaitForSingleObject(m_hSyncObject,dwTimeOut);
    switch(dwWaitRet)
    {
    case WAIT_OBJECT_0:
        m_bLockAcquired = true;
        return WAIT_OBJECT_0;
    case WAIT_TIMEOUT:
        throw SyncObjectWaitTimeOut(_T("Semaphore wait timedout"),GetLastError());
    case WAIT_FAILED:
        throw SyncObjectWaitFailed(_T("Semaphore wait failed"),GetLastError());
    }
    return dwWaitRet;
}

DWORD SemaphoreSynchronizer::UnLock()
{
    if( !m_bLockAcquired )
        return ERROR_SUCCESS;
    if( !m_hSyncObject )
        return static_cast<DWORD>(-1);
    if( FALSE == ::ReleaseSemaphore(m_hSyncObject,m_lReleaseCount,m_plPrevCount) )
        return GetLastError();
    
    m_bLockAcquired = false;
    
    return ERROR_SUCCESS;
}

SemaphoreSynchronizer::~SemaphoreSynchronizer(void)
{
    if (!m_hSyncObject )
        return;
    ::CloseHandle(m_hSyncObject);
}