#pragma once
#include "SynchronizerBase.h"

/*/////////////////////////////////////////////////////////////////////////////////////
  EventSynchronizer - This class wraps the event kernel object. It serves as facilitator
  for the automatic lock handling class, such as the SingleObjectLock or MultiObjectLock.
  The SingleObjectLock's constructor calls Lock and destructor calls UnLock. The usage of 
  EventSynchronizer with MultiObjectLock is slightly different (see the documentation for 
  MultiObjectLock).
  
  @written by :  Sudheesh.P.S
  @version    :  1.0            
  @date       :  12-12-2012
  @info       :  Initial version
//////////////////////////////////////////////////////////////////////////////////////*/

class SYNCHRONIZERLIB_API EventSynchronizer:public SynchronizerBase
{
public:
    EventSynchronizer(LPSECURITY_ATTRIBUTES lpSecAttrib,
                      BOOL bInheritHandle,
                      BOOL bManualReset,BOOL bInitialState,
                      LPCTSTR lpctszEventName=NULL);
    DWORD Lock(DWORD dwTimeOut);
    DWORD UnLock();
    BOOL SetEvent();
    BOOL ResetEvent();
    ~EventSynchronizer(void);
};


