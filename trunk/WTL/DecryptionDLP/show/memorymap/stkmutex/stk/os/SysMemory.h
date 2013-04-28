#pragma once

#include "Common.h"

namespace STK
{
typedef enum
{
    eMemoryInvalid = 0,
    eMemoryHeap,
    eMemoryShareOpen,
    eMemoryShareCreate
} MemoryType;

class CSysMemory
{
public:
    CSysMemory(size_t nSize, const wchar_t *pszName);
    virtual ~CSysMemory();

public:
    bool GetMemory(void **pMemory);
    MemoryType GetMemoryType();

private:
    bool Create(size_t nSize, const wchar_t *pszName);
    bool Release();

private:
    void *m_pMemory;
    HANDLE m_hMemory;
    MemoryType m_eMemoryType;
};
}