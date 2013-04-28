/**
  * Author: Feng Xu 
  * Email:  fxu_36@hotmail.com
  */

#pragma once

#include "Common.h"

namespace STK
{
#define INVALID_MUTEX_ID       (ULONG_MAX - 1)
#define MAX_MUTEX_COUNT        (ULONG_MAX - 1)
#define MAX_MANAGER_NAME_LEN   128

//The mutex could be shared by processes
typedef struct 
{
    //Used to control the exclusive access to this strtucture
    LONG  volatile lSpinLock;
    //The lock flag
    bool  bLocked;
    //The reference count on the lock
    DWORD dwLockRefCount;
    //The reference count on the mutex structure
    DWORD dwMutexRefCount;
    //The waiter count
    LONG  dwThreadsWaiting;
    //The unique ID of the mutex in manager domain
    DWORD dwIndex;
    //The owner process ID
    DWORD dwProcessId;
    //The ownder thread ID
    DWORD dwThreadId;
    //The next available mutex
    DWORD dwMutexNextFree;
} Mutex;

//The mutex control block could be shared by processes
typedef struct 
{
    //The start index of this block 
    DWORD dwStartIndex;
    //The mutex count in this block
    DWORD dwMutexCount;
    //The mutex array in this block
    Mutex pMutexArray[1];
} MutexSharedControlBlock;

//The mutex control block in one process
typedef struct MutexControlBlock
{
    /* The shared control block */
    MutexSharedControlBlock *pSharedCtlBlock;
    /* The next control block */
    MutexControlBlock* pCtlBlockNext;    
} MutexControlBlock;

//The manager header could be shared by processes
typedef struct 
{
    //The mutex count in total
    DWORD dwMutexCount;
    //The memory allocation granularity for extending the mutex pool
    DWORD dwIncCount;
    //The available mutex count
    DWORD dwMutexFree;
    //The next available  mutex
    DWORD dwMutexNextFree;
    //The size of the mutex, reserved
    WORD  wMutexSize;
    //The mutex alignment, reserved
    WORD  wMutexAlign;
    //The count of mutexes in use
    DWORD dwMutexInUse;
    //Maximum count of mutexes ever in use
    DWORD dwMutexInUseMax;	
} MutexSharedManagerHeader;

//The manager header in one process
typedef struct 
{
    /* The shared header */
    MutexSharedManagerHeader *pSharedHeader;
    /* The header of the control block */
    MutexControlBlock *pCtlBlock;        
} MutexManagerHeader;
} //namespace STK