#pragma once

#include "Common.h"

namespace STK
{
class CSysMutex
{
public:
	CSysMutex(bool bInitiallyOwn = false, 
              LPCWSTR pszName = NULL,
		      LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
   	virtual ~CSysMutex();

public:
    bool Lock(DWORD dwTime);
	bool Unlock();

private:
    bool Create(bool bInitiallyOwn = false, 
                LPCWSTR pszName = NULL,
		        LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);

    bool Release();

private:
    HANDLE m_hMutex;
};
}