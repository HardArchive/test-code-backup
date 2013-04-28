/**
  * Author: Feng Xu 
  * Email:  fxu_36@hotmail.com
  */

#pragma once

#include "MutexCommon.h"
#include "SysEvent.h"

namespace STK
{
class CMutexManager;

/**
  * This is a helper class used to lock and unlock a mutex.
  * 
*/
class CMutex
{
public:
    /**
      * The constructor of the mutex class.
      * @param pManager The mutex manager.
      * @oaram dwIndex The mutex index
      */
    CMutex(CMutexManager *pManager, DWORD dwIndex);

    /**
      * The destructor of the mutex class.
      */
    virtual ~CMutex();

	/**
      * Disable the copy constructor and assignment by default so you will get
	  * compiler errors instead of unexpected behaviour if you pass objects
	  * by value or assign objects.
      */
protected:
	CMutex();

private:
	CMutex(const CMutex& objMutex);
	void operator=(const CMutex& objMutex);

public:
    /**
      * The method waits for ownership of the specified mutex object. The method 
      * returns when the calling thread is granted ownership. After a thread has 
      * ownership of a mutex object, it can make additional calls to Lock() without 
      * blocking its execution. This prevents a thread from deadlocking itself 
      * while waiting for a mutex that it already owns. A thread must call Unlock() 
      * once for each time that it called Lock().
      * @return ture if the method succeeds, otherwise false.
      */
    bool Lock();

    /**
      * The method releases ownership of the specified mutex object.A thread uses 
      * the Lock method to acquire ownership of a mutex object. To release its 
      * ownership, the thread must call Unlock() once for each time that it called 
      * Lock() method.
      * @return ture if the method succeeds, otherwise false.
      */
    bool Unlock();

private:
    bool Create(CMutexManager *pManager, DWORD dwIndex);
    bool Release();
    bool EnterCriticalSection();
    bool LeaveCriticalSection();

private:
    bool GetEventName(std::wstring &strName);

private:
    //The pointer to the mutex manager
    CMutexManager *m_pManager;
    //The pointer to the mutex sturct
    Mutex* m_pMutex;
};
} //namespace STK