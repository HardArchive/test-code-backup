#include "stdafx.h"
#include "Synchronizer/MutexSynchronizer.h"
#include <tchar.h>

MutexSynchronizer::MutexSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                                     BOOL bInheritHandle, LPCTSTR lpctszMutexName)
{
    m_hSyncObject = ::OpenMutex(SYNCHRONIZE,bInheritHandle,lpctszMutexName);
    if( 0 != m_hSyncObject )
    {
        return;
    }
    m_hSyncObject = CreateMutex(lpSecAttrib,bInheritHandle,lpctszMutexName);
    if( 0 == m_hSyncObject )
        throw SyncObjectCreationFailed(_T("Mutex creation failed"),GetLastError());
}

DWORD MutexSynchronizer::Lock(DWORD dwTimeOut)
{
    DWORD dwWaitRet = ::WaitForSingleObject(m_hSyncObject,dwTimeOut);
    switch(dwWaitRet)
    {
    case WAIT_OBJECT_0:
        m_bLockAcquired = true;
        return WAIT_OBJECT_0;
    case WAIT_TIMEOUT:
        throw SyncObjectWaitTimeOut(_T("Mutex wait timedout"),GetLastError());
    case WAIT_FAILED:
        throw SyncObjectWaitFailed(_T("Mutex wait failed"),GetLastError());
    case WAIT_ABANDONED:
        throw SyncObjectWaitAbandoned(_T("Mutex wait abandoned"),GetLastError());
    }
    return dwWaitRet;
}

DWORD MutexSynchronizer::UnLock()
{
    if( !m_bLockAcquired )
        return ERROR_SUCCESS;
    
    if( !m_hSyncObject )
        return static_cast<DWORD>(-1);

    if( FALSE == ::ReleaseMutex(m_hSyncObject) )
        return GetLastError();
    
    m_bLockAcquired = false;
    
    return ERROR_SUCCESS;
}

MutexSynchronizer::~MutexSynchronizer(void)
{
    if (!m_hSyncObject )
        return;
    ::CloseHandle(m_hSyncObject);
}
