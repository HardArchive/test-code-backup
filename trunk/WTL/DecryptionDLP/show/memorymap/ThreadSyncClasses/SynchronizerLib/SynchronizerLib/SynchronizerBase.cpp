#include "stdafx.h"
#include "Synchronizer/SynchronizerBase.h"
#include <tchar.h>
#include <string>

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

GeneralException::GeneralException(LPCTSTR lpctszDescription, DWORD dwLastError)
{
    OutputDebugString(_T("In GeneralException::GeneralException"));
    LPVOID lpMsgBuf = 0;
    tstring tsDescription = lpctszDescription;
    try
    {
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwLastError,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<TCHAR*>(&lpMsgBuf),
            0, NULL );
        tsDescription = tsDescription;
        tsDescription +=_T("->");
        tsDescription += static_cast<TCHAR*>(lpMsgBuf);
        int nDescLen = tsDescription.size();
        m_ptszDescription = new TCHAR[nDescLen+1];
        _tcscpy(m_ptszDescription,tsDescription.c_str());
    }
    catch(...)
    {
    }
    if( lpMsgBuf )
    {
        LocalFree(lpMsgBuf);
    }
}

GeneralException::GeneralException(const GeneralException& GenExcepObj)
{
    *this = GenExcepObj;
}
GeneralException& GeneralException::operator =(const GeneralException& GenExcepObj)
{
    if(this != &GenExcepObj)
    {
        delete[] m_ptszDescription;
        int nLen = _tcslen(GenExcepObj.m_ptszDescription);
        m_ptszDescription = new TCHAR[nLen+1];
        _tcscpy(m_ptszDescription,GenExcepObj.m_ptszDescription);
    }
    return *this;
}

GeneralException::~GeneralException()
{
    delete[] m_ptszDescription;
    m_ptszDescription = 0;
}