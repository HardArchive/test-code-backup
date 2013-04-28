#include "MutexManager.h"
#include "Mutex.h"

namespace STK
{
CMutex::CMutex(CMutexManager *pManager, DWORD dwIndex)
    : m_pMutex (NULL)
    , m_pManager (NULL)
{
    Create(pManager, dwIndex);
}

CMutex::~CMutex()
{
    Release();
}

bool CMutex::Lock()
{
    if ((m_pMutex == NULL) || (m_pManager == NULL))
    {
        return false;
    }

    //Get the process and thread IDs
    DWORD dwProcessId = GetCurrentProcessId();
    DWORD dwThreadId  = GetCurrentThreadId();
    //The event object
    CSysEvent *pEvent = NULL;

    while (true)
    {
        //Enter the critical section
        EnterCriticalSection();

        //Check if the mutex is locked
        if (m_pMutex->bLocked == false)
        {
            //Set the lock flag
            m_pMutex->bLocked = true;
            //Increase the lock reference count
            m_pMutex->dwLockRefCount++;
            //Assign the process and thread IDs
            m_pMutex->dwProcessId = dwProcessId;
            m_pMutex->dwThreadId  = dwThreadId;
            //Free the event
            delete pEvent;
            //Leave the critical section
            LeaveCriticalSection();
            return true;
        } 
        
        //Check if the same thread calls the lock again
        if ((m_pMutex->dwProcessId == dwProcessId) &&
            (m_pMutex->dwThreadId  == dwThreadId))
        {
            //Increase the lock reference count
            m_pMutex->dwLockRefCount++;
            //Free the event
            delete pEvent;
            //Leave the critical section
            LeaveCriticalSection();
            return true;
        }

        //Increase the waiter count
        m_pMutex->dwThreadsWaiting++;
        //Create the event if it's not created before, then wait on the event
        if (pEvent == NULL)
        {
            //Get the event name
            std::wstring strName;
            if (!GetEventName(strName))
            {
                //Leave the critical section
                LeaveCriticalSection();
                return false;
            }
            //Create the event
            pEvent = new (std::nothrow) CSysEvent(false, false, strName.c_str(), NULL);
            if (pEvent == NULL)
            {
                //Leave the critical section
                LeaveCriticalSection();
                return false;
            }
        }

        //Leave the critical section
        LeaveCriticalSection();
        //Wait on the event
        if (!pEvent->Lock(INFINITE))
        {
            delete pEvent;
            return false; 
        }
    }

    //Free the event
    delete pEvent;
    //Leave the critical section
    LeaveCriticalSection();

    return true;
}

bool CMutex::Unlock()
{
    if ((m_pMutex == NULL) || (m_pManager == NULL))
    {
        return false;
    }

    //Enter the critical section
    EnterCriticalSection();

    //Get the process and thread IDs
    DWORD dwProcessId = GetCurrentProcessId();
    DWORD dwThreadId  = GetCurrentThreadId();

    //Check if the thread is the owner
    if ((m_pMutex->dwProcessId != dwProcessId) ||
        (m_pMutex->dwThreadId  != dwThreadId))
    {
        LeaveCriticalSection();
        return false;
    }

    //Check if the mutex is locked
    if (m_pMutex->bLocked == false)
    {
        LeaveCriticalSection();
        return false;
    }

    //Decrease the lock reference count
    m_pMutex->dwLockRefCount--;
    //Check if the lock reference count is not 0
    if (m_pMutex->dwLockRefCount > 0)
    {
        LeaveCriticalSection();
        return true;
    }

    //Release the lock
    m_pMutex->bLocked = false;
    //Reset the process and thread IDs
    m_pMutex->dwProcessId = 0;
    m_pMutex->dwThreadId  = 0;
   
    //Check if there is a thread waitting on the lock
    if (m_pMutex->dwThreadsWaiting > 0)
    {
        //Get the event name
        std::wstring strName;
        if (!GetEventName(strName))
        {
            LeaveCriticalSection();
            return false;
        }
        //Create the event
        CSysEvent *pEvent = new (std::nothrow)CSysEvent(false, false, strName.c_str(), NULL);
        if (pEvent == NULL)
        {
            LeaveCriticalSection();
            return false;
        }
        //Set the event state to signaled
		if (!pEvent->SetEvent ())
        {
            LeaveCriticalSection();
            delete pEvent;
            return false;
        }
        //Free the event
		delete pEvent;
        //Decrease the waiter count
        m_pMutex->dwThreadsWaiting--;
    }
    //Leave the critical section
    LeaveCriticalSection();

    return true;
}

bool CMutex::EnterCriticalSection()
{
    if (m_pMutex == 0)
    {
        return false;
    }

    //Spin and get the spin lock. Each thread only owns the spin lock 
    //for a very short of period
    while (InterlockedExchange(&(m_pMutex->lSpinLock), 1) != 0)
    {
        //Yield execution to another thread that is ready to run on 
        //the current processor.
        SwitchToThread();
    }

    return true;
}

bool CMutex::LeaveCriticalSection()
{
    if (m_pMutex == NULL)
    {
        return false;
    }
    //Rlease the spin lock
    InterlockedExchange(&(m_pMutex->lSpinLock), 0);

    return true;
}

bool CMutex::Create (CMutexManager *pManager, DWORD dwIndex)
{
    //Release the mutex
    Release();
    //Get the mutex
    if (pManager)
    {
        m_pManager = pManager;
        //Get the mutex
        return m_pManager->GetMutex(dwIndex, &m_pMutex);
    }

    return false;
}

bool CMutex::Release()
{
    return true;
}

bool CMutex::GetEventName(std::wstring &strName)
{
    if ((m_pMutex == NULL) || (m_pManager == NULL))
    {
        return false;
    }

    const wchar_t *pName = NULL;
    //Get the manager name
    m_pManager->GetName(&pName);
    //Set the capacity of the string
    strName.reserve (MAX_PATH+1);

    wchar_t szBuffer[11];
    //For unnamed mutex manager, the event name is STK.Event.processid.mutexid
    //otherwise the event name is managername.STK.Event.mutexid
    if (pName == NULL)
    {
        strName = L"STK.Event.";
        strName += _ltow(GetCurrentProcessId(), szBuffer, 10);
        strName += '.';
    }
    else
    {
        strName = pName;
        strName += L".STK.Event.";
    }
    //Append the mutex ID
    strName += _ltow(m_pMutex->dwIndex, szBuffer, 10);

    return true;
}
}