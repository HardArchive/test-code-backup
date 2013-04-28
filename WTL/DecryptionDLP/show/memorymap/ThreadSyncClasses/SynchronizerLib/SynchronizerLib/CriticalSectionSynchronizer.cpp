#include "stdafx.h"
#include "Synchronizer/CriticalSectionSynchronizer.h"

CriticalSectionSynchronizer::CriticalSectionSynchronizer(void)
{
    ::InitializeCriticalSection(&m_csObject);
}

DWORD CriticalSectionSynchronizer::Lock(DWORD dwTimeOut)
{
    ::EnterCriticalSection(&m_csObject);
    m_bLockAcquired = true;
    return ERROR_SUCCESS;
}

DWORD CriticalSectionSynchronizer::UnLock()
{
    if( !m_bLockAcquired )
        return ERROR_SUCCESS;
    
    ::LeaveCriticalSection(&m_csObject);
    m_bLockAcquired = false;
    
    return ERROR_SUCCESS;
}

CriticalSectionSynchronizer::~CriticalSectionSynchronizer(void)
{
    ::DeleteCriticalSection(&m_csObject);
}
