#pragma once
#include "CommonDefs.h"

/*/////////////////////////////////////////////////////////////////////////////////////
  SingleObjectLock - This class handles the automatic lock acquire and release by using
  constructor and destructor. It ensures that the lock is properly released with the
  help of UnLock call in the destructor. It works with the single object wait function,
  the WaitForSingleObject.

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/


class SynchronizerBase;
class SYNCHRONIZERLIB_API SingleObjectLock
{
public:
    SingleObjectLock(SynchronizerBase* pSyncBase,DWORD dwTimeout);
    ~SingleObjectLock();
private:
    SynchronizerBase* m_pSyncBase;
};

