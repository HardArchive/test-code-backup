#include "SysMemory.h"

namespace STK
{
CSysMemory::CSysMemory(size_t nSize, const wchar_t *pszName)
    : m_pMemory (NULL)
    , m_hMemory (NULL)
    , m_eMemoryType (eMemoryInvalid)
{
    Create(nSize, pszName);
}

CSysMemory::~CSysMemory()
{
    Release();
}

bool CSysMemory::Create(size_t nSize, const wchar_t *pszName)
{
    //Check the size
    if (nSize <= 0)
    {
        return false;
    }

    Release();

    //If the name is NULL, allocate from the main memory
    if ((pszName == NULL) || (pszName[0] == NULL))
    {
        //From main memory
        m_pMemory = (void*) new (std::nothrow) BYTE [nSize];
        if (m_pMemory == NULL)
        {
            return false;
        }
        memset(m_pMemory, 0, nSize);
        m_eMemoryType = eMemoryHeap;
        return true;
    }

    ULARGE_INTEGER qwSize;
    qwSize.QuadPart = nSize;
    //Try to create the view
	m_hMemory = CreateFileMappingW(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE,  
                                   qwSize.HighPart, qwSize.LowPart, pszName);
    //Get the error code
    DWORD dwLastError = GetLastError();
    if (m_hMemory == 0)
    {
        return false;
    }

    //Map the view
	m_pMemory = MapViewOfFile(m_hMemory, FILE_MAP_ALL_ACCESS, 0, 0, nSize);
    if (m_pMemory == 0)
    {
        Release();
        return false;
    }
    if (dwLastError == ERROR_ALREADY_EXISTS)
    {
        m_eMemoryType = eMemoryShareOpen;
    }
    else
    {
        m_eMemoryType = eMemoryShareCreate;
        memset(m_pMemory, 0, nSize);
    }

    return true;
}

bool CSysMemory::Release()
{
    if (m_pMemory)
    {
        if (m_eMemoryType == eMemoryInvalid)
        {
            return false;
        } 
        else if (m_eMemoryType == eMemoryHeap)
        {
            delete [] m_pMemory;
        }
        else
        {
            CloseHandle(m_hMemory);
            UnmapViewOfFile(m_pMemory);
        }

        m_hMemory = NULL;
        m_pMemory = NULL;
        m_eMemoryType = eMemoryInvalid;
    }

    return true;
}

bool CSysMemory::GetMemory(void **pMemory)
{
    if (m_eMemoryType == eMemoryInvalid)
    {
        return false;
    }

    *pMemory = m_pMemory;
    return true;
}

MemoryType CSysMemory::GetMemoryType()
{
    return m_eMemoryType;
}
}