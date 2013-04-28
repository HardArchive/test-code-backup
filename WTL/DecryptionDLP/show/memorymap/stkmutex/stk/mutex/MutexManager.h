/**
  * Author: Feng Xu 
  * Email:  fxu_36@hotmail.com
  */

#pragma once

#include "MutexCommon.h"
#include "SysMutex.h"
#include "SysMemory.h"
#include "Mutex.h"

namespace STK
{
/**
  * The mutex manager class is used to manage mutex pool with huge mutex number, 
  * the mutex pool is allocated from the heap or shared memory based on the manager 
  * name specified. The unnamed manager allocates the pool from the heap, the named 
  * manager allocates the pool from the shared memory. The mutexes with named manager 
  * can synchronize threads between processes, the mutexes with unnamed manager can 
  * synchronize threads within a single process. The mutex pool can grow automatically 
  * based on demand, however the pool can not shrink after it grows.
  *
  * The mutex in the pool is implemented using the Windows API InterlockedExchange, 
  * the Windows event object is used to synchronize the threads only if there is 
  * contention. 
  *
  * If you have a page based memory buffer, the page number is huge and each page needs 
  * a mutex; or if you have an in-memory hash table, the hash bucket number is huge and 
  * each bucket needs a mutex, the mutex manager may serve your purpose.
  * 
*/
class CMutexManager
{
public:
    /**
      * The constructor of the mutex manager.
      * @param dwInitCount The initial mutex count created by the manager.
      * @param dwIncCount The memory-allocation granularity for extending the mutex
      * pool, as the pool grows, memory is allocated in units of dwIncCount mutexes.
      * @param pszName The manager name. It is limited to 128 characters. Name 
      * comparison is case sensitive, if the name matches the name of an existing 
      * named mutex manager, dwInitCount and dwIncCount are ignored because they have 
      * already been set by the process that originally created the manager. If 
      * pszName is NULL, the mutex manager is created without a name. The mutexes with 
      * named manager can synchronize threads between processes, the mutexes with 
      * unnamed manager can synchronize threads within a single process.
      */
    CMutexManager(DWORD dwInitCount, DWORD dwIncCount, const wchar_t *pszName);

    /**
      * The destructor of the mutex manager
      */
    virtual ~CMutexManager();

    /**
      * CMutex is the friend of the mutex manager
      */
    friend class CMutex; 

	/**
      * Disable the copy constructor and assignment by default so you will get
	  * compiler errors instead of unexpected behaviour if you pass objects
	  * by value or assign objects.
      */
protected:
	CMutexManager();

private:
	CMutexManager(const CMutexManager& objManager);
	void operator=(const CMutexManager& objManager);

public:
    /**
      * Create a mutex from the mutex pool and set the mutex reference count to one. 
      * A thread must call CloseMutex for each time that it created the mutex.
      * @param dwIndex The index of the mutex being created.
      * @return ture if the method succeeds, otherwise false.
      */
    bool CreateMutex(DWORD& dwIndex);

    /**
      * Open a mutex created before and increase the mutex reference count by one. 
      * A thread must call CloseMutex once for each time that it opened the mutex.
      * @param dwIndex The index of the mutex to open. The mutex must have been 
      * created before.
      * @return ture if the method succeeds, otherwise false.
      */
    bool OpenMutex(DWORD dwIndex);

    /**
      * For a mutex being created or opened before, decrease the mutex reference 
      * count by one. If the reference count reaches 0, the mutex manager put the 
      * mutex back to the mutex pool. A thread must call CloseMutex once for each 
      * time that it created or opened the mutex.
      * @param dwIndex The index of the mutex to close. A matching CreateMutex or 
      * OpenMutex must have been called before.
      * @return ture if the method succeeds, otherwise false.
      */
    bool CloseMutex(DWORD dwIndex);

    /**
      * Get the mutex manager name. The caller should not modify the content
      * returned.
      * @param pName The mutex manaer name. The mutex manager owns the pointer.
      * @return ture if the method succeeds, otherwise false.
      */
    bool GetName (const wchar_t **pName);

protected:
    bool GetMutex(DWORD dwIndex, Mutex **pMutex);

protected:
    bool Initialize(DWORD dwSize, DWORD dwInc, const wchar_t *pName);
    bool Release();
    bool CreateHeader(DWORD dwIncCount, const wchar_t *pszManagerName);
    bool CreateMutexBlock(DWORD dwMutexCount, const wchar_t *pszManagerName);
    bool MountMutexBlock (CSysMemory *pMemory, DWORD dwMutexCount);

private:
    //The mutex manager name
    std::wstring m_strName;
    //The mutex control block
    MutexControlBlock *m_pCtlBlock;
    //THe manager header
    MutexManagerHeader m_sManagerHeader;
    //The mutex used to synchronize the access to the manager
    CSysMutex* m_pSysMutex;
    //The mutex control block count
    DWORD m_dwBlockCount;
    //If the manager object is initialized
    bool volatile m_bInitialized;

private:
    //The memory block list
    std::list<CSysMemory*> m_listMemory;
};
} //namespace STK