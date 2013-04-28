#pragma once
#include "CommonDefs.h"
#include <vector>
#include <Windows.h>
#include <atlbase.h>

/*/////////////////////////////////////////////////////////////////////////////////////
  MultiObjectLock - This class handles the automatic lock acquire and release by using
  constructor and destructor. It ensures that the lock is properly released with the
  help of destructor. It works with the multiple object wait function such as the
  WaitForMultipleObjects and MsgWaitForMultipleObjects. It throws the SyncObjectWaitTimedOut,
  SyncObjectWaitFailed, SyncObjectWaitAbandoned depending on the return value of the wait 
  function. This object can be used to wait in a worker thread as well as UI thread.
  In case UI thread is specified, it will be using the MsgWaitForMultipleObjects internally.
  The LockHolder keeps the synchronization objects. The sync objects should be added to the
  LockHolder and is passed to MultiObjectLock instance. The LockHolder can be reused with
  the local instances of MultiObjectLock. See the below documentation for LockHolder too.
  
  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SynchronizerBase;
class LockHolder;

class SYNCHRONIZERLIB_API MultiObjectLock
{
public:
    MultiObjectLock(LockHolder* phlLockHolder,BOOL bWaitAll,DWORD dwTimeOut,
                    BOOL bUIWait,DWORD dwUIWaitFlags);
    bool HasMessageInQueue();
    ~MultiObjectLock(void);
private:
    DWORD Lock(BOOL bAWaitAll,DWORD dwTimeOut,BOOL bUIWait,DWORD dwUIWaitFlags);
    void SetAllObjectsAcquired();
    
    bool m_bHasMessageInQueue;
    LockHolder* m_plhLockHolder;
};

typedef std::vector<SynchronizerBase*> SYNCHRONIZER_VEC;
/*/////////////////////////////////////////////////////////////////////////////////////
  LockHolder - Holds the list of synchronization objects. It is used with the 
  MultiObjectLock. The synchronizer list is separated because, we can create local
  instances of MultiObjectLock while the LockHolder object can be reused. So no need
  add the synchronizer objects again while creating another instance of MultiObjectLock.
  This especially handy when we use a UI wait. In such case MultiObjectLock internally
  uses MsgWaitForMultipleObjects. It needs to loop around, when a message is in queue
  till the required kernel object is signaled. In such cases, only the MultiObjectLock
  will be created locally. See CThreadSyncClassesDemoDlg::OnBnClickedButtonUiMultiWait()
  for sample usage.

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API LockHolder
{
public:
    LockHolder();
    int AddSynchronizer(SynchronizerBase* pSyncBase);
    void RemoveSynchronizer(SynchronizerBase* pSyncBase);
    SynchronizerBase* GetObjectAt(int nIndex);
    int GetObjectCount()
    {
        return m_pvSyncBase->size();
    }
    inline HANDLE* GetRawHandles(int& nHandleCount);
    ~LockHolder();
private:
    HANDLE* m_pHandles;  
    SYNCHRONIZER_VEC* m_pvSyncBase;
};
