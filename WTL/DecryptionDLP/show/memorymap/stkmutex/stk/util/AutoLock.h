#pragma once

#include "Common.h"

namespace STK
{
template <class T> 
class CAutoLock
{
public:
    CAutoLock(T *pLock)
    {
        if (pLock)
        {
            m_pLock = pLock;
            m_pLock->Lock(INFINITE);    
        }
    }

    virtual ~CAutoLock()
    {
        if (m_pLock)
        {
            m_pLock->Unlock();    
            m_pLock = NULL;
        }
    }

private:
    T* m_pLock;
};
}