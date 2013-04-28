/*/////////////////////////////////////////////////////////////////////////////////////
  SynchronizerBase.h - The Synchronizer group of classes provides facilities for thread
  synchronization using CriticalSection, Event, Mutex and Semaphore. These classes can
  be used the automatic locking and lock releasing objects such as SingleObjectLock and
  MultiObjectLock. There are CriticalSectionSynchronizer, EventSynchronizer, MutexSynchronizer
  and SemaphoreSynchronizer. SynchronizerBase is the base class for this group of classes.
  These classes throw exceptions when an conditions such as wait timeout. As we all know 
  the exception based mechanism avoids the error code checking in the success case. 
  It separates the nomal flow and exceptional flow.

  This file contains the declaration for the SynchronizerBase class.


  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

#pragma once
#include "CommonDefs.h"
#include <windows.h>

#ifdef SYNCHRONIZERLIB_EXPORTS
#define SYNCHRONIZERLIB_API __declspec(dllexport)
#else
#define SYNCHRONIZERLIB_API __declspec(dllimport)
#endif

class MultiObjectLock;

/*/////////////////////////////////////////////////////////////////////////////////////
  GeneralException - It is the base of all exceptions such as SyncObjectCreationFailed, 
  SyncObjectWaitFailed, SyncObjectWaitTimeOut, SyncObjectWaitAbandoned. This class
  get system error string with the help of FormatMessage.

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API GeneralException
{
public:
    GeneralException(LPCTSTR lpctszDescription, DWORD dwLastError);
    GeneralException(const GeneralException& GenExcepObj);
    GeneralException& operator =(const GeneralException& GenExcepObj);
    virtual ~GeneralException();
    TCHAR* GetDescription()
    {
        return m_ptszDescription;
    }
private:
    TCHAR* m_ptszDescription;
};

/*/////////////////////////////////////////////////////////////////////////////////////
  SyncObjectCreationFailed - Thrown when the synchronization object creation fails 
  due to some reason.

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SyncObjectCreationFailed:public GeneralException
{
public:
    SyncObjectCreationFailed(LPCTSTR lpctszDescription,DWORD dwLastError):
                             GeneralException(lpctszDescription,dwLastError)
    {}
};

/*/////////////////////////////////////////////////////////////////////////////////////
  SyncObjectWaitFailed - Thrown when the wait function fails with WAIT_FAILED error code
  
  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SyncObjectWaitFailed:public GeneralException
{
public:
    SyncObjectWaitFailed(LPCTSTR lpctszDescription,DWORD dwLastError):
                         GeneralException(lpctszDescription,dwLastError)
    {}
};

/*/////////////////////////////////////////////////////////////////////////////////////
  SyncObjectWaitTimeOut - Thrown when the wait function fails with WAIT_TIMEOUT 
  error code
  
  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SyncObjectWaitTimeOut:public GeneralException
{
public:
    SyncObjectWaitTimeOut(LPCTSTR lpctszDescription,DWORD dwLastError):
                          GeneralException(lpctszDescription,dwLastError)
    {}
};

/*/////////////////////////////////////////////////////////////////////////////////////
  SyncObjectWaitAbandoned - Thrown when the wait function fails with WAIT_ABANDONED 
  error code
  
  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SyncObjectWaitAbandoned:public GeneralException
{
public:
    SyncObjectWaitAbandoned(LPCTSTR lpctszDescription,DWORD dwLastError):
                            GeneralException(lpctszDescription,dwLastError)
    {}
};

/*/////////////////////////////////////////////////////////////////////////////////////
  The SynchronizerBase povides virtual methods for Lock and UnLock, which is supposed
  to be implemented by the derived classes such as MutexSynchronizer or EventSynchronizer.
  We can attach the handles of kernel object handles and detach when no need to handle 
  the cleanup. The GetHandle function returns the underlying handle.

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SynchronizerBase
{
public:
    SynchronizerBase():m_bLockAcquired(false),
                       m_hSyncObject(0)
    {}
    void Attach(HANDLE hSyncObject)
    {
        m_hSyncObject = hSyncObject;
    }
    HANDLE Detach()
    {
        HANDLE hSyncObject = m_hSyncObject;
        m_hSyncObject = 0;
        return m_hSyncObject;
    }
    HANDLE GetHandle()
    {
        return m_hSyncObject;
    }
    virtual DWORD Lock(DWORD dwTimeOut)=0;
    virtual DWORD UnLock()=0;
    virtual ~SynchronizerBase(void)
    {}
    friend class MultiObjectLock;
protected:
    bool m_bLockAcquired;
    HANDLE m_hSyncObject;
private:
    void SetLockAcquired(bool bLockAcquired)
    {
        m_bLockAcquired = bLockAcquired;
    }
};

