#pragma once

#include "Common.h"

namespace STK
{
class CSysEvent
{
public:
	CSysEvent(bool bInitiallyOwn = false, 
              bool bManualReset = false,
		      LPCWSTR pszName = NULL, 
              LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
   	virtual ~CSysEvent();

public:
	bool SetEvent();
	bool PulseEvent();
	bool ResetEvent();
    bool Lock(DWORD dwTimeout);

private:
    bool Create(bool bInitiallyOwn = false, 
                bool bManualReset = false,
		        LPCWSTR pszName = NULL, 
                LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
    bool Release();

private:
    HANDLE m_hEvent;
};
}; //namespace STK