#include "MutexManager.h"
#include "AutoLock.h"

namespace STK
{
CMutexManager::CMutexManager(DWORD dwSize, DWORD dwInc, const wchar_t *pName)
    : m_pCtlBlock (NULL)
    , m_pSysMutex (NULL)
    , m_dwBlockCount (0)
    , m_bInitialized (false)
{
    Initialize(dwSize, dwInc, pName);
}

CMutexManager::~CMutexManager()
{
    Release();
}

bool CMutexManager::Initialize(DWORD dwInitCount, DWORD dwIncCount, const wchar_t *pszName)
{
    //Check if the manager has been initialized
    if (m_bInitialized)
    {
        return true;
    }

    //Make sure the initial and incremental counts are valid
    if ((dwInitCount > MAX_MUTEX_COUNT) ||(dwIncCount > MAX_MUTEX_COUNT))
    {
        return false;
    }

    //Get the mutex manager name
    if (pszName != NULL)
    {
        m_strName = pszName;
    }

    //Check the size of the manager name
    if (m_strName.length() >= MAX_MANAGER_NAME_LEN)
    {
        return false;
    }

    //Create the os mutex for the manager. For named mutex manager
    //the mutex name is managername.ManagerMutex
    std::wstring strName;
    if (!m_strName.empty())
    {
        strName = m_strName + L".ManagerMutex";
    }
    //Get the mutex name
    const wchar_t *pszMutexName = strName.empty() ? NULL : strName.c_str();
    //Create the mutex object
    m_pSysMutex = new (std::nothrow) CSysMutex(false, pszMutexName , NULL);
    if (m_pSysMutex == NULL)
    {
        return false;
    }

    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //Create the manager header
    if (!CreateHeader(dwIncCount, pszName))
    {
        return false;
    }

    //Create the mutex block
    if (!CreateMutexBlock(dwInitCount, pszName))
    {
        return false;
    }

    //Set the initialized flag to true
    m_bInitialized = true;

    return true;
}

bool CMutexManager::Release()
{
    //Check if the manager has been intialized
    if (!m_bInitialized)
    {
        return true;
    }

    //Lock the manager
    m_pSysMutex->Lock(INFINITE);

    //Get the mutex control block header
    MutexControlBlock *pCtlBlock = m_sManagerHeader.pCtlBlock;
    //Free all control blocks
    while (pCtlBlock)
    {
        //Get the next control block
        MutexControlBlock *pCtlBlockTemp = pCtlBlock->pCtlBlockNext;
        //Free the curren one
        delete [] ((void*)pCtlBlock);
        pCtlBlock = pCtlBlockTemp;
    }

    //Free the memory blocks
    std::list<CSysMemory*>::iterator iter;
    for (iter = m_listMemory.begin(); iter!= m_listMemory.end(); ++iter)
    {
        delete (*iter);
    }

    //Set the initialize flag to false
    m_bInitialized = false;
    //Empty the manager name
    m_strName.clear();

    //Unlock the manager
    m_pSysMutex->Unlock();
    //Free the system mutex
    delete m_pSysMutex;
    m_pSysMutex = NULL;

    return true;
}

bool CMutexManager::CreateMutexBlock(DWORD dwMutexCount, const wchar_t *pszManagerName)
{
    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //The size of the shared control block plus one mutex
    DWORD  dwCtlSize = sizeof (MutexSharedControlBlock);
    //The size of one mutex
    DWORD  dwMutexSize = sizeof (Mutex);
    //The size of the memory
    size_t nSize = dwCtlSize + dwMutexSize * dwMutexCount;

    //The shared control block name
    std::wstring strName;
    //Set the capacity of the name
    strName.reserve (MAX_PATH+1);
    //For named mutex manager, the shared control block name is 
    //managername.MutexBLock.count
    if ((pszManagerName != NULL) && (pszManagerName[0] != NULL))
    {
        strName = pszManagerName;
        strName += L".MutexBlock.";
        //Append the mutex count
        wchar_t szBuffer[11];
        strName += _ltow(m_dwBlockCount, szBuffer, 10);
    }

    //Get the shared control block name
    const wchar_t *pszBlockName = strName.empty() ?  NULL : strName.c_str();
    //Create the memory object
    CSysMemory *pMemory = new (std::nothrow) CSysMemory(nSize, pszBlockName);
    if (!pMemory)
    {
        return false;
    }
    //Mount the shared control block    
    if (!MountMutexBlock (pMemory, dwMutexCount))
    {
        delete pMemory;
        return false;
    }

    //Increase the block count
    m_dwBlockCount++;
    //Add it into the memory list
    m_listMemory.push_back (pMemory);

    return true;
}

bool CMutexManager::MountMutexBlock(CSysMemory *pMemory, DWORD dwMutexCount)
{
    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //Create the local mutex contol block
    MutexControlBlock *pCtlBlock = 
            (MutexControlBlock *) new (std::nothrow) BYTE[sizeof(MutexControlBlock)];
    if (pCtlBlock == NULL)
    {
        return false;
    }
    //Get the shared control block
    MutexSharedControlBlock *pSharedCtlBlock = NULL;
    if (!pMemory->GetMemory ((void**)(&pSharedCtlBlock)))
    {
        return false;
    }

    //Set the shared block to the control block
    pCtlBlock->pSharedCtlBlock = pSharedCtlBlock;
    //Set the next block to 0
    pCtlBlock->pCtlBlockNext = 0;
    //Add the new control block to the control block list
    if (m_sManagerHeader.pCtlBlock  == NULL)
    {
        m_sManagerHeader.pCtlBlock  = pCtlBlock;
    }
    else
    {
        MutexControlBlock *pCtlBlockTemp = m_sManagerHeader.pCtlBlock ;
        while (pCtlBlockTemp->pCtlBlockNext)
        {
            pCtlBlockTemp = (MutexControlBlock *)(pCtlBlockTemp->pCtlBlockNext);
        }
        pCtlBlockTemp->pCtlBlockNext = pCtlBlock;
    }

    //Initialize the shared control block if it's created
    if (pMemory->GetMemoryType() != eMemoryShareOpen)
    {
        //Get the shared manager header
        MutexSharedManagerHeader *pSharedHeader = m_sManagerHeader.pSharedHeader;

        //Set the mutex count
        pSharedCtlBlock->dwMutexCount = dwMutexCount;
        //Set the start index
        pSharedCtlBlock->dwStartIndex = pSharedHeader->dwMutexCount;  
        //Update the total mutex count
        pSharedHeader->dwMutexCount += dwMutexCount;
        //Update the total free mutex count
        pSharedHeader->dwMutexFree  += dwMutexCount;

        Mutex* pMutexArray = pSharedCtlBlock->pMutexArray;
        Mutex* pMutex = NULL;
        //Link the mutexes
        for (DWORD dwIndex = 0; dwIndex < dwMutexCount; dwIndex++)
        {
		    pMutex = pMutexArray + dwIndex;
		    pMutex->dwMutexNextFree = pSharedCtlBlock->dwStartIndex  + dwIndex + 1;
	    }

	    pMutex->dwMutexNextFree  = INVALID_MUTEX_ID;
        if (pSharedHeader->dwMutexNextFree == INVALID_MUTEX_ID)
        {
            pSharedHeader->dwMutexNextFree = pSharedCtlBlock->dwStartIndex;
        }
    }

    return true;
 }


bool CMutexManager::CreateMutex(DWORD& dwIndex)
{
    //Check if the manager is initialized
    if (m_bInitialized == false)
    {
        return false;
    }

    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //Get the shared manager header
    MutexSharedManagerHeader *pSharedHeader = m_sManagerHeader.pSharedHeader;
    //Check if there is free mutex
	if (pSharedHeader->dwMutexNextFree == INVALID_MUTEX_ID) 
    {
        //Check if we can add more mutex
        if (pSharedHeader->dwMutexCount == MAX_MUTEX_COUNT)
        {
            return false;
        }
        //Determine how many new mutex to add
        DWORD dwCount = MAX_MUTEX_COUNT - pSharedHeader->dwMutexCount;
        dwCount = (dwCount >= pSharedHeader->dwIncCount) ? 
                                pSharedHeader->dwIncCount : dwCount;
        //Create one more mutex block
        if (!CreateMutexBlock(dwCount, m_strName.c_str()))
        {
		    return false;
        }
	}

    //Get the free mutex ID
	dwIndex = pSharedHeader->dwMutexNextFree;
    //Get the mutex address
    Mutex *pMutex = NULL;
    //Get the mutex address
	while (!GetMutex(dwIndex, &pMutex))
    {
        //Create more mutex block
        if (!CreateMutexBlock(pSharedHeader->dwIncCount, m_strName.c_str()))
        {
            return false;
        }
    }
    //Set the next free mutex index
	pSharedHeader->dwMutexNextFree = pMutex->dwMutexNextFree;
    //Decrease the free mutex count
	--pSharedHeader->dwMutexFree;
    //Increase the inuse mutex count
	++pSharedHeader->dwMutexInUse;
    //Update the new max inuse mutex count
	if (pSharedHeader->dwMutexInUse > pSharedHeader->dwMutexInUseMax)
    {
		pSharedHeader->dwMutexInUseMax = pSharedHeader->dwMutexInUse;
    }

	//Set 0 to the mutex created
	memset(pMutex, 0, sizeof(pMutex));
    //Assign the index
    pMutex->dwIndex = dwIndex;
    //Increase the mutex reference count
    pMutex->dwMutexRefCount++;

    return true;
}

bool CMutexManager::OpenMutex(DWORD dwIndex)
{
    //Check if the manager is initialized
    if (m_bInitialized == false)
    {
        return false;
    }

    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //Get the shared manager header
    MutexSharedManagerHeader *pSharedHeader = m_sManagerHeader.pSharedHeader;
    //Check if the mutex index is within the range
    if (dwIndex >= pSharedHeader->dwMutexCount)
    {
        return false;
    }

    Mutex *pMutex = NULL;
    //Get the mutex address
	while (!GetMutex(dwIndex, &pMutex))
    {
        //Create more mutex block
        if (!CreateMutexBlock(pSharedHeader->dwIncCount, m_strName.c_str()))
        {
            return false;
        }
    }

    //Check if the mutex has been created
    if (pMutex->dwMutexRefCount == 0)
    {
        return false;
    }
    //Increase the mutex reference count
    pMutex->dwMutexRefCount++;

    return true;
}

bool CMutexManager::CloseMutex(DWORD dwIndex)
{
    //Check if the manager is initialized
    if (m_bInitialized == false)
    {
        return false;
    }

    //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);

    //Get the shared manager header
    MutexSharedManagerHeader *pSharedHeader = m_sManagerHeader.pSharedHeader;
    //Check if the mutex index is within the range
    if (dwIndex >= pSharedHeader->dwMutexCount)
    {
        return false;
    }

    Mutex *pMutex = NULL;
    //Get the mutex address
	while (!GetMutex(dwIndex, &pMutex))
    {
        //Create more mutex block
        if (!CreateMutexBlock(pSharedHeader->dwIncCount, m_strName.c_str()))
        {
            return false;
        }
    }

    //Decrease the mutex reference count
    pMutex->dwMutexRefCount--;
    //If the reference count is 0, put the mutex back to the free list
    if (pMutex->dwMutexRefCount == 0)
    {
	    pMutex->dwMutexNextFree = pSharedHeader->dwMutexNextFree;
	    pSharedHeader->dwMutexNextFree = pMutex->dwIndex;
        //Increase the free mutex count
	    ++pSharedHeader->dwMutexFree;
        //Decrease the in use mutex count
	    --pSharedHeader->dwMutexInUse;
    }

	return true;
}

bool CMutexManager::GetName (const wchar_t **pName)
{
    if (m_strName.empty())
    {
        *pName = NULL;
    }
    else
    {
        *pName = m_strName.c_str();
    }

    return true;
}


bool CMutexManager::GetMutex(DWORD dwIndex, Mutex **pMutex)
{
    if (m_bInitialized == false)
    {
        return false;
    }

    //The shared control block
    MutexSharedControlBlock *pSharedBlock = NULL;

    //Get the local control block
    MutexControlBlock *pCtlBlock = m_sManagerHeader.pCtlBlock;
    //Search the block that contains the index
    while (pCtlBlock)
    {
        //Get the shared control block
        pSharedBlock = pCtlBlock->pSharedCtlBlock;
        //Check if the index is in this block
        if ((dwIndex >= pSharedBlock->dwStartIndex) &&
            (dwIndex < (pSharedBlock->dwStartIndex + pSharedBlock->dwMutexCount))) 
        {
            break;
        }
        //Next the block
        pCtlBlock = pCtlBlock->pCtlBlockNext;
    }

    //Check if found the block
    if (pCtlBlock == NULL)
    {
        return false;
    }
    //Get the mutex array
    Mutex *pMutexArray = pSharedBlock->pMutexArray;
    //Get the mutex
    *pMutex = pMutexArray + (dwIndex - pSharedBlock->dwStartIndex);

    return true;
}

bool CMutexManager::CreateHeader(DWORD dwIncCount, const wchar_t *pManagerName)
{
    if (m_bInitialized)
    {
        return true;
    }

   //Lock the manager
    CAutoLock<CSysMutex> lockAuto(m_pSysMutex);
    //The header name
    std::wstring strName;
    //For named mutex manager, the header name is managername.Header
    if ((pManagerName != NULL) && (pManagerName[0] != NULL))
    {
        strName = pManagerName;
        strName += L".Header";
    }
    //Get the header name
    const wchar_t *pszHeaderName = strName.empty() ? NULL : strName.c_str();
    //Get the memory size
    size_t nSize = sizeof (MutexSharedManagerHeader);
    //Create the memory object
    CSysMemory *pMemory = new (std::nothrow) CSysMemory(nSize, pszHeaderName);
    if (pMemory == NULL)
    {
        return false;
    }

    //Get the memory
    if (!pMemory->GetMemory((void**)&(m_sManagerHeader.pSharedHeader)))
    {
        delete pMemory;
        return false;
    }
    //If the header is created, initialize the align and mutex size 
    if (pMemory->GetMemoryType() != eMemoryShareOpen) 
    {
        //The mutex incremental size
        m_sManagerHeader.pSharedHeader->dwIncCount  = dwIncCount;
        //The align size
        m_sManagerHeader.pSharedHeader->wMutexAlign = sizeof (__int64);
        //The mutex size
        m_sManagerHeader.pSharedHeader->wMutexSize  = sizeof (Mutex);
    }
    //Initialize the mutex block pointer
    m_sManagerHeader.pCtlBlock = NULL;
    //Put it into the memory list 
    m_listMemory.push_back (pMemory);

    return true;
}

}
