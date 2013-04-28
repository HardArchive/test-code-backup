#include "SysEvent.h"

namespace STK
{
CSysEvent::CSysEvent(bool bInitiallyOwn, bool bManualReset, LPCWSTR pszName,
	                 LPSECURITY_ATTRIBUTES lpsaAttribute)
    : m_hEvent(NULL)
{
    Create(bInitiallyOwn, bManualReset, pszName, lpsaAttribute);
}

CSysEvent::~CSysEvent()
{
    Release();
}

bool CSysEvent::SetEvent()
{
    if (m_hEvent == NULL)
    {
        return false;
    }

    return (::SetEvent(m_hEvent) == TRUE);
}

bool CSysEvent::PulseEvent()
{
    if (m_hEvent == NULL)
    {
        return false;
    }

    return (::PulseEvent(m_hEvent)== TRUE);
}

bool CSysEvent::ResetEvent()
{
    if (m_hEvent == NULL)
    {
        return false;
    }

    return (::ResetEvent(m_hEvent) == TRUE);
}

bool CSysEvent::Lock(DWORD dwTimeout)
{
    if (m_hEvent == NULL)
    {
        return false;
    }

	DWORD dwRet = ::WaitForSingleObject(m_hEvent, dwTimeout);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
    {
		return true;
    }
	else
    {
		return false;
    }
}

bool CSysEvent::Create(bool bInitiallyOwn, bool bManualReset, LPCWSTR pszName, 
                       LPSECURITY_ATTRIBUTES lpsaAttribute)
{
    Release();
	m_hEvent = ::CreateEventW(lpsaAttribute, bManualReset, bInitiallyOwn, pszName);
    if (m_hEvent == NULL)
    {
        return false;
    }

    return true;
}

bool CSysEvent::Release()
{
    BOOL bRet = TRUE;

	if (m_hEvent != NULL)
	{
		bRet = ::CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

    return (bRet == TRUE);
}

} 