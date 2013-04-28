// testsharemutex.cpp : Defines the entry point for the console application.
//
#include "common.h"
#include "MutexManager.h"
#include "Mutex.h"

#define PAGE_NUMBER   100
#define VALUE_NUMBER  10
#define THREAD_NUMBER  3

typedef struct
{
    DWORD dwMutexIndex;
    DWORD arrayValue[VALUE_NUMBER];
} Page;

typedef struct 
{
    DWORD dwPageNumber;
    Page  arrayPage[1];
} Buffer;

typedef struct 
{
    Buffer *pBuffer;
    STK::CMutexManager *pMutexManager;
    DWORD dwMutexIndex;
} ThreadData;

bool PrintPage(bool bNormal, Page *pPage, DWORD dwPageNumber, 
               STK::CMutexManager *pMutexManager, DWORD dwMutexIndex)
{
    //Valide the parameters
    if ((pPage == NULL)         ||
        (pMutexManager == NULL) ||
        (dwMutexIndex == INVALID_MUTEX_ID))
    {
        return false;
    }

    //Synchronize the printing
    STK::CMutex objMutex(pMutexManager, dwMutexIndex);
    objMutex.Lock(); 

    //Print the page number
    std::cout<<"-----------------------------------------------------------------"<<std::endl;
    if (bNormal)
    {
        std::cout<< "Normal page:"<< dwPageNumber;
    }
    else
    {
        std::cout<< "Abnormal page:"<< dwPageNumber;
    }

    //Print teh process and thread IDs
    std::cout<< " Process ID:"<< GetCurrentProcessId();
    std::cout<< " Thread ID:"<< GetCurrentThreadId()<<std::endl;

    //Print the values
    std::cout<<" value: ";
    for (DWORD dwValueIndex = 0; dwValueIndex < VALUE_NUMBER; dwValueIndex++)
    {
        std::cout<< pPage->arrayValue[dwValueIndex] <<" ";  
    }
    std::cout<<std::endl;
    std::cout<<"-----------------------------------------------------------------"<<std::endl<<std::endl;
    //Unlock the printing
    objMutex.Unlock(); 

    return true;
}

DWORD WINAPI ReadWritePages(LPVOID lpParam ) 
{ 
    //Check if the parameter is NULL
    if (lpParam == NULL)
    {
        return 1;
    }

    ThreadData* pData = (ThreadData*)lpParam;
    //Get the buffer
    Buffer* pBuffer = pData->pBuffer; 
    //Get the mutex manager
    STK::CMutexManager* pMutexManager = pData->pMutexManager;
    //Get the mutex for printing
    DWORD dwMutexIndex = pData->dwMutexIndex;

    //Check if the buffer or mutex manager is NULL
    if ((pBuffer == NULL) ||
        (pMutexManager == NULL))
    {
        return 1;
    }

    //The return value
    int iRet = 0;
    //Sets a random starting point.
    srand(GetTickCount());
    //Access the page randomly
    for (DWORD dwIndex = 0; dwIndex < pBuffer->dwPageNumber * 1000; dwIndex++)
    {
        //Get the random number
        DWORD dwRand = rand();
        //Get the page number
        DWORD dwPageNumber = dwRand % pBuffer->dwPageNumber;
        //Get the page pointer
        Page *pPage = pBuffer->arrayPage + dwPageNumber;
        //Lock the page
        STK::CMutex objMutex (pMutexManager, pPage->dwMutexIndex);
        if (!objMutex.Lock())
        {
            iRet = 1;
            break;
        }

        //Lock the page again
        if (!objMutex.Lock())
        {
            iRet = 1;
            break;
        }

        //Check the data integrity
        bool bNormal = true;
        for (DWORD dwValueIndex = 0; dwValueIndex < VALUE_NUMBER; dwValueIndex++)
        {
            if ((dwValueIndex < (VALUE_NUMBER - 1)) &&
                (pPage->arrayValue[dwValueIndex] != pPage->arrayValue[dwValueIndex+1]))
            {
                bNormal = false;
                break;
            }
        }
        //Print out the page values
        PrintPage (bNormal, pPage, dwPageNumber, pMutexManager, dwMutexIndex);

        //Modify the page value
        for (dwValueIndex = 0; dwValueIndex < VALUE_NUMBER; dwValueIndex++)
        {
            SwitchToThread();
            pPage->arrayValue[dwValueIndex] = dwRand;
        }

        //Unlock the page
        if (!objMutex.Unlock())
        {
            iRet = 1;
            break;
        }

        //Unlock the page again
        if (!objMutex.Unlock())
        {
            iRet = 1;
            break;
        }
    }

    return iRet;
}

int main(int argc, char* argv[])
{
    //Create the mutex for the application
    STK::CSysMutex objSysMutex(false, L"TestSharedMutex.Applicaiton", NULL);
    //Synchronize the instances of the applicaiton
    if (!objSysMutex.Lock (INFINITE))
    {
        return 1;
    }
    //Create the shared memory for the buffer
    DWORD dwSize = sizeof (Buffer) + PAGE_NUMBER * sizeof (Page);
    STK::CSysMemory objSysMemory (dwSize, L"TestSharedMutex.Buffer");
    //Get the buffer
    Buffer *pBuffer;
    if (!objSysMemory.GetMemory ((void**)&pBuffer))
    {
        objSysMutex.Unlock(); 
        return 1;
    }

    //Create the named mutex manager, the initial and incremental mutex counts are 10
    STK::CMutexManager objMutexManager (10, 10, L"TestSharedMutex");
    
    //If it's not opened shared memory, initialize it
    if (objSysMemory.GetMemoryType() != STK::eMemoryShareOpen)
    {
        //Set the page number
        pBuffer->dwPageNumber = PAGE_NUMBER;
        //For each page, create the mutex 
        for (DWORD dwPageIndex = 0; dwPageIndex < pBuffer->dwPageNumber; dwPageIndex++)
        {
            DWORD dwMutexIndex = INVALID_MUTEX_ID;
            //Create the mutex
            if (!objMutexManager.CreateMutex (dwMutexIndex))
            {
                objSysMutex.Unlock(); 
                return 1;
            }
            //Get the page
            Page* pPage = pBuffer->arrayPage + dwPageIndex;
            //Set the mutex index to the page
            pPage->dwMutexIndex = dwMutexIndex;
        }
    }
    else
    {
        //For each page, open the mutex
        for (DWORD dwPageIndex = 0; dwPageIndex < pBuffer->dwPageNumber; dwPageIndex++)
        {
            //Get the page
            Page* pPage = pBuffer->arrayPage + dwPageIndex;
            //Open the mutex
            if (!objMutexManager.OpenMutex (pPage->dwMutexIndex))
            {
                objSysMutex.Unlock(); 
                return 1;
            }
        }
    }
    //Unlock the applicaiton mutex
    objSysMutex.Unlock(); 

    //Create the mutex to synchronize the printing 
    DWORD dwMutexIndex = INVALID_MUTEX_ID;
    if (!objMutexManager.CreateMutex (dwMutexIndex))
    {
        return 1;
    }

    //Construct the paramater for the worker threads
    ThreadData tData;
    tData.pBuffer = pBuffer;
    tData.pMutexManager = &objMutexManager;
    tData.dwMutexIndex = dwMutexIndex;

    //Create the worker threads
    DWORD dwThreadId[THREAD_NUMBER];
    HANDLE hThread[THREAD_NUMBER]; 
    for( DWORD dwIndex = 0; dwIndex < THREAD_NUMBER; dwIndex++)
    {
        hThread[dwIndex] = CreateThread(NULL, 0, ReadWritePages, &tData, 0, 
                                        &dwThreadId[dwIndex]);
    }

    //Wait until all threads have terminated.
    WaitForMultipleObjects(THREAD_NUMBER, hThread, TRUE, INFINITE);

    // Close all thread handles
    for(dwIndex = 0; dwIndex< THREAD_NUMBER; dwIndex++)
    {
        CloseHandle(hThread[dwIndex]);
    }

    //Release the mutex for the printing synchronization
    int iRet = 0;
    if (!objMutexManager.CloseMutex (dwMutexIndex))
    {
        iRet = 1;
    }

    //For each page, Release the mutex
    for (DWORD dwPageIndex = 0; dwPageIndex < pBuffer->dwPageNumber; dwPageIndex++)
    {
        Page* pPage = pBuffer->arrayPage + dwPageIndex;
        if (!objMutexManager.CloseMutex (pPage->dwMutexIndex))
        {
            iRet = 1;
        }
    }

    return iRet;
}

