
#pragma once
#include "SynchronizerBase.h"

/*/////////////////////////////////////////////////////////////////////////////////////
  MutexSynchronizer - This class wraps the mutex kernel object. It throws 
  SyncObjectCreationFailed, SyncObjectWaitFailed, SyncObjectWaitTimedOut, SyncObjectWaitAbandoned
  exceptions depending on the mutex creation or return of the WaitForSingleObject function.
  It acts a facilitator for the automatic lock acquire and release by the SingleObjectLock and
  MultiObjectLock classes. The constructor of the SingleObjectLock calls the Lock and the
  destructor calls the UnLock. The usage of MutexSynchronizer with MultiObjectLock is slightly
  different (see the documentation for MultiObjectLock).

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API MutexSynchronizer:public SynchronizerBase
{
public:
    MutexSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                      BOOL bInheritHandle, LPCTSTR lpctszMutexName=NULL);
    DWORD Lock(DWORD dwTimeOut);
    DWORD UnLock();
    ~MutexSynchronizer(void);
};

