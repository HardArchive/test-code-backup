#include "SysMutex.h"

namespace STK
{
CSysMutex::CSysMutex(bool bInitiallyOwn, LPCWSTR pszName, LPSECURITY_ATTRIBUTES lpsaAttribute)
    : m_hMutex(NULL)
{
    Create(bInitiallyOwn, pszName, lpsaAttribute);
}

CSysMutex::~CSysMutex()
{
    Release();
}

bool CSysMutex::Unlock()
{
    if (m_hMutex == NULL)
    {
        return false;
    }

	return (::ReleaseMutex(m_hMutex) == TRUE);
}

bool CSysMutex::Lock(DWORD dwTimeout)
{
    if (m_hMutex == NULL)
    {
        return false;
    }

	DWORD dwRet = ::WaitForSingleObject(m_hMutex, dwTimeout);
	if ((dwRet == WAIT_OBJECT_0) || 
        (dwRet == WAIT_ABANDONED))
    {
		return true;
    }
	else
    {
		return false;
    }
}

bool CSysMutex::Create(bool bInitiallyOwn, LPCWSTR pszName, LPSECURITY_ATTRIBUTES lpsaAttribute)
{
    Release();
	m_hMutex = ::CreateMutexW(lpsaAttribute, bInitiallyOwn, pszName);
    if (m_hMutex == NULL)
    {
        return false;
    }

    return true;
}

bool CSysMutex::Release()
{
    BOOL bRet = TRUE;
	if (m_hMutex != NULL)
	{
		bRet = ::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

    return (bRet == TRUE);
}
}