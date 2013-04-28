#include "stdafx.h"
#include "Synchronizer/MultiObjectLock.h"
#include "Synchronizer/SynchronizerBase.h"

MultiObjectLock::MultiObjectLock(LockHolder* plhLockHolder,
                                 BOOL bWaitAll,DWORD dwTimeOut,
                                 BOOL bUIWait,DWORD dwUIWaitFlags):
                                 m_bHasMessageInQueue(false),
                                 m_plhLockHolder(plhLockHolder)
{
    //LockHolder should be created before creating this object.
    //It can be reused while creating local instances of
    //MultiObjectLock
    if( 0 == m_plhLockHolder )
        throw SyncObjectCreationFailed(_T("MultiObjecLock failed->NULL lock holder"),0);
    Lock(bWaitAll,dwTimeOut,bUIWait,dwUIWaitFlags);
}


DWORD MultiObjectLock::Lock(BOOL bAWaitAll,DWORD dwTimeOut,BOOL bUIWait,DWORD dwUIWaitFlags)
{
    int nHandleCount = 0;
    HANDLE* pHandles = m_plhLockHolder->GetRawHandles(nHandleCount);
    DWORD dwWaitRet = 0;
    if( !bUIWait )
    {
        dwWaitRet  = ::WaitForMultipleObjects(nHandleCount,pHandles,bAWaitAll,dwTimeOut);
    }
    else
    {
        dwWaitRet  = ::MsgWaitForMultipleObjects(nHandleCount,pHandles,bAWaitAll,dwTimeOut,dwUIWaitFlags);
    }
    
    switch(dwWaitRet)
    {
    case WAIT_OBJECT_0:
        //This means that all the objects are signalled.
        //So the acquired status needs to be set for proper release in destructors.
        if( bAWaitAll )
        {
            SetAllObjectsAcquired();
        }
        return WAIT_OBJECT_0;
    case WAIT_TIMEOUT:
        throw SyncObjectWaitTimeOut(_T("Multiwait timedout"),GetLastError());
    case WAIT_FAILED:
        throw SyncObjectWaitFailed(_T("Multiwait failed"),GetLastError());
    case WAIT_ABANDONED_0:
        throw SyncObjectWaitAbandoned(_T("Mutex wait abandoned"),GetLastError());
    }
    if( !bAWaitAll )
    {
        //There is chance that the thread that owns the mutex can terminate abnormally.
        //This will cause WAIT_ABANDONED to be returned for any handle in the array.
        if( (dwWaitRet > WAIT_ABANDONED_0) && (dwWaitRet < (WAIT_ABANDONED_0+nHandleCount)) )
        {
            throw SyncObjectWaitAbandoned(_T("Mutex wait abandoned"),GetLastError());
        }
        //If any of the handles signalled...
        if( (dwWaitRet > WAIT_OBJECT_0) && (dwWaitRet < (WAIT_OBJECT_0+nHandleCount)) )
        {
            SynchronizerBase* pSyncBase = m_plhLockHolder->GetObjectAt(dwWaitRet-WAIT_OBJECT_0);
            if( pSyncBase )
                pSyncBase->SetLockAcquired(true);
        }

    }
    if( bUIWait )
    {
        //In the case of MsgWaitForMuiltipleObject, when a message arrives in the queue,
        //the return is (WAIT_OBJECT_0+nHandleCount)
        if( (WAIT_OBJECT_0+nHandleCount) == dwWaitRet )
        {
            m_bHasMessageInQueue = true;
        }
    }
    return dwWaitRet;
}

bool MultiObjectLock::HasMessageInQueue()
{
    return m_bHasMessageInQueue;
}
void MultiObjectLock::SetAllObjectsAcquired()
{
    int nObjectCount = m_plhLockHolder->GetObjectCount();
    for(int nIdx = 0; nIdx < nObjectCount; ++nIdx )
    {
        m_plhLockHolder->GetObjectAt(nIdx)->SetLockAcquired(true);
    }
}

MultiObjectLock::~MultiObjectLock(void)
{
    try
    {
        int nObjectCount = m_plhLockHolder->GetObjectCount();
        for(int nIdx = 0; nIdx < nObjectCount; ++nIdx )
        {
            m_plhLockHolder->GetObjectAt(nIdx)->UnLock();
        }
    }
    catch(...)
    {
        //Not to allow exception out of destructor
    }
}

LockHolder::LockHolder():m_pHandles(0),m_pvSyncBase(0)
{
    m_pvSyncBase = new SYNCHRONIZER_VEC;
}
LockHolder::~LockHolder()
{
    delete[] m_pHandles; 
    delete m_pvSyncBase;
}
int LockHolder::AddSynchronizer(SynchronizerBase* pSyncBase)
{
    int nObjectCount = m_pvSyncBase->size();
    
    //Only the maximum object count is supported.
    if( nObjectCount >= MAXIMUM_WAIT_OBJECTS )
        return false;
    
    if( pSyncBase )
    {
        m_pvSyncBase->push_back(pSyncBase);
    }
    return true;
}

void LockHolder::RemoveSynchronizer(SynchronizerBase* pSyncBase)
{
    std::vector<SynchronizerBase*>::iterator itrSync;
    for( itrSync = m_pvSyncBase->begin(); m_pvSyncBase->end() != itrSync; ++itrSync )
    {
        if (*itrSync == pSyncBase )
        {
            m_pvSyncBase->erase(itrSync);
            break;
        }
    }

}

SynchronizerBase* LockHolder::GetObjectAt(int nIndex)
{
    if( ( m_pvSyncBase->size() > 0 ) && ( nIndex >= 0 ) )
    {
        return (*m_pvSyncBase)[nIndex];
    }
    return NULL;
}

inline HANDLE* LockHolder::GetRawHandles(int& nHandleCount)
{
    delete[] m_pHandles;
    nHandleCount = m_pvSyncBase->size();
    m_pHandles = new HANDLE[nHandleCount];
    for(int nIdx = 0; nIdx < nHandleCount; ++nIdx )
    {
        m_pHandles[nIdx] = ((*m_pvSyncBase)[nIdx])->GetHandle();
    }
    return m_pHandles;
}
