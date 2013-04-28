#pragma once
#include "SynchronizerBase.h"
#include <Windows.h>

/*/////////////////////////////////////////////////////////////////////////////////////
  SemaphoreSynchronizer - This class wraps the semaphore kernel object. It throws 
  SyncObjectCreationFailed, SyncObjectWaitFailed, SyncObjectWaitTimedOut exceptions 
  depending on the semaphore creation or return of the WaitForSingleObject function.
  It acts a facilitator for the automatic lock acquire and release by the SingleObjectLock and
  MultiObjectLock classes. The constructor of the SingleObjectLock calls the Lock and the
  destructor calls the UnLock. The usage of SemaphoreSynchronizer with MultiObjectLock is slightly
  different (see the documentation for MultiObjectLock).

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API SemaphoreSynchronizer:public SynchronizerBase
{
public:
    SemaphoreSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                          BOOL bInheritHandle,
                          LONG lIninitialCount,
                          LONG lMaxCount,
                          LONG lReleaseCount,
                          LONG* plPrevCount,
                          LPCTSTR lpctszSemaphoreName=NULL);
    DWORD Lock(DWORD dwTimeOut);
    DWORD UnLock();
    ~SemaphoreSynchronizer(void);
private:
    LONG m_lReleaseCount;
    LONG* m_plPrevCount;
};

