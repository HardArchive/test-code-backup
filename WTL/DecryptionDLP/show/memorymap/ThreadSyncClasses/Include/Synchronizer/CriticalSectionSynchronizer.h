#pragma once
#include "SynchronizerBase.h"

/*/////////////////////////////////////////////////////////////////////////////////////
  CriticalSectionSynchronizer - This class wraps the critical section. It serves as facilitator
  for the automatic lock handling class, the SingleObjectLock. The SingleObjectLock's
  constructor calls Lock and destructor calls UnLock.
  

  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API CriticalSectionSynchronizer:public SynchronizerBase
{
public:
    CriticalSectionSynchronizer(void);
    DWORD Lock(DWORD dwTimeOut);
    DWORD UnLock();
    ~CriticalSectionSynchronizer(void);
private:
    CRITICAL_SECTION m_csObject;
};

