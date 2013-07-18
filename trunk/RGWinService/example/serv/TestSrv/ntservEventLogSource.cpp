/*  
Module : NTSERVEVENTLOGSOURCE.CPP
Purpose: Implementation for the class CNTEventLogSource
Created: PJN / 14-07-1997
History: PJN / 23-07-2005 1. Provided an overriden version of Report which has 3 string parameters
                          2. GetStringArrayFromRegistry and SetStringArrayIntoRegistry methods
                          are now public as they are helpful to have available externally
         PJN / 11-01-2006 1. Addition of a default constructor for CNTEventLogSource 
                          2. Addition of CNTEventLogSource::SetServerName, CNTEventLogSource::GetServerName,
                          CNTEventLogSource::SetSourceName and CNTEventLogSource::GetSourceName methods
                          3. Addition of a DWORD* parameter to the CNTEventLogSource::GetStringArrayFromRegistry
                          and CNTEventLogSource::SetStringArrayFromRegistry functions. This allow the details
                          of the last error to be returned to client code.
         PJN / 03-06-2006 1. If CNTEventLogSource::Uninstall fails to remove the HKLM\System\CurrentControlSet\Services\EventLog\"Display Name"
                          registry key, then it aborts before doing any other work. Thanks to Matthias Miller
                          for reporting this issue.
         PJN / 13-06-2006 1. Addition of a logname parameter to the CNTEventLogSource constructor to support user defined event logs
                          instead of logging to the standard "Application" event log. Please note that the code added only enables the CNTService 
                          framework to use non "Application" event logs, but it does not contain code to create a custom event log. You will need 
                          to handle that in your application's install. For further information on doing this, please see 
                          http://msdn.microsoft.com/library/default.asp?url=/library/en-us/eventlog/base/eventlog_key.asp which contains details
                          on the various registry values which you need to create. Thanks to Jan Bartels for this addition.
                          2. Also optimized the construction of member variables in the CNTEventLogSource constructors
         PJN / 24-06-2006 1. Updated code to compile cleanly using VC 2005.
         PJN / 25-06-2006 1. Code now uses newer C++ style casts instead of C style casts.
         PJN / 01-08-2006 1. Fix for two buffer size calculations using the new _tcscpy_s functions, one in CNTEventLogSource::SetStringArrayIntoRegistry and the other in
                          CNTService::GetDependencies. Thanks to DmityShm on the codeproject forums for reporting this issue.
         PJN / 21-09-2006 1. Minor update to use %X instead of %x in various CNTEventLogSource::Report calls.
         PJN / 13-11-2007 1. Minor update to display HRESULT's correctly.
         PJN / 01-08-2010 1. Fixed a minor issue in CNTEventLogSource::Report with the use of printf format identifiers
                          2. Reworked all the internal registry management code to use ATL::CRegKey
                          3. CNTEventLogSource::Install method now supports CategoryCount, CategoryMessageFile and ParameterMessageFile settings for event sources

Copyright (c) 1996 - 2010 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////////////// Includes ////////////////////////////////////

#include "stdafx.h"
#include "ntservEventLogSource.h"
#include "ntserv_msg.h"


///////////////////////////////// Macros //////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////// Includes ////////////////////////////////////

CNTEventLogSource::CNTEventLogSource() : m_hEventSource(NULL),
                                         m_sLogName(_T("Application"))
{
}

CNTEventLogSource::CNTEventLogSource(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpLogName) : m_hEventSource(NULL),
                                                                                                         m_sServerName(lpUNCServerName),
                                                                                                         m_sSourceName(lpSourceName),
                                                                                                         m_sLogName(lpLogName)

{
}

CNTEventLogSource::CNTEventLogSource(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) : m_hEventSource(NULL),
                                                                                      m_sServerName(lpUNCServerName),
                                                                                      m_sSourceName(lpSourceName),
                                                                                      m_sLogName(_T("Application"))
{
}

CNTEventLogSource::~CNTEventLogSource()
{
  Deregister();
}

CNTEventLogSource::operator HANDLE() const
{
  return m_hEventSource;
}

BOOL CNTEventLogSource::Attach(HANDLE hEventSource)
{
  if (m_hEventSource != hEventSource)
    Deregister();

  m_hEventSource = hEventSource;
  return TRUE;
}

HANDLE CNTEventLogSource::Detach()
{
  HANDLE hReturn = m_hEventSource;
  m_hEventSource = NULL;
  return hReturn;
}

BOOL CNTEventLogSource::Register(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName)
{
  Deregister();
  m_hEventSource = RegisterEventSource(lpUNCServerName, lpSourceName);
  return (m_hEventSource != NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, WORD wCategory, DWORD dwEventID, PSID lpUserSid,
                               WORD wNumStrings, DWORD dwDataSize, LPCTSTR* lpStrings, LPVOID lpRawData)
{
  CSingleLock sl(&m_csReport, TRUE);

  ASSERT(m_hEventSource == NULL);
  Register(m_sServerName, m_sSourceName);

  //Call the SDK version of the function
  BOOL bSuccess = ReportEvent(m_hEventSource, wType,	wCategory, dwEventID, lpUserSid,
                              wNumStrings, dwDataSize, lpStrings, lpRawData);
  Deregister();  

  return bSuccess;
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID)
{
  return Report(wType, 0, dwEventID, NULL, 0, 0, NULL, NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, LPCTSTR lpszString)
{
  return Report(wType, CNTS_MSG_SERVICE_FULLSTRING, lpszString);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, LPCTSTR lpszString)
{
  ASSERT(lpszString);
  return Report(wType, 0, dwEventID, NULL, 1, 0, &lpszString, NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, LPCTSTR lpszString1, LPCTSTR lpszString2)
{
  ASSERT(lpszString1);
  ASSERT(lpszString2);
  LPCTSTR lpStrings[2];
  lpStrings[0] = lpszString1;
  lpStrings[1] = lpszString2;
  return Report(wType, 0, dwEventID, NULL, 2, 0, lpStrings, NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, LPCTSTR lpszString1, LPCTSTR lpszString2, DWORD dwCode, BOOL bReportAsHex)
{
  ASSERT(lpszString1);
  ASSERT(lpszString2);
  LPCTSTR lpStrings[3];
  lpStrings[0] = lpszString1;
  lpStrings[1] = lpszString2;
  CString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  lpStrings[2] = sError;
  return Report(wType, 0, dwEventID, NULL, 3, 0, lpStrings, NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, LPCTSTR lpszString1, LPCTSTR lpszString2, LPCTSTR lpszString3)
{
  ASSERT(lpszString1);
  ASSERT(lpszString2);
  ASSERT(lpszString3);
  LPCTSTR lpStrings[3];
  lpStrings[0] = lpszString1;
  lpStrings[1] = lpszString2;
  lpStrings[2] = lpszString3;
  return Report(wType, 0, dwEventID, NULL, 3, 0, lpStrings, NULL);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, DWORD dwCode, BOOL bReportAsHex)
{
  CString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  return Report(wType, dwEventID, sError);
}

BOOL CNTEventLogSource::Report(WORD wType, DWORD dwEventID, LPCTSTR lpszString, DWORD dwCode, BOOL bReportAsHex)
{
  CString sError;
  if (bReportAsHex)
    sError.Format(_T("%08X"), dwCode);
  else
    sError.Format(_T("%u"), dwCode);
  return Report(wType, dwEventID, lpszString, sError);
}

BOOL CNTEventLogSource::Deregister()
{
  BOOL bSuccess = TRUE;
  if (m_hEventSource != NULL)
  {
    bSuccess = DeregisterEventSource(m_hEventSource);
    m_hEventSource = NULL;
  }

  return bSuccess;
}

BOOL CNTEventLogSource::Install(LPCTSTR lpszSourceName, LPCTSTR lpszEventMessageFile, LPCTSTR pszEventCategoryMessageFile, LPCTSTR pszEventParameterMessageFile, DWORD dwTypesSupported, DWORD dwCategoryCount)
{
  //Just delegate the work to the other version of "Install"
  return Install(_T("Application"), lpszSourceName, lpszEventMessageFile, pszEventCategoryMessageFile, pszEventParameterMessageFile, dwTypesSupported, dwCategoryCount);
}

BOOL CNTEventLogSource::Install(LPCTSTR lpszLogName, LPCTSTR lpszSourceName, LPCTSTR lpszEventMessageFile, LPCTSTR pszEventCategoryMessageFile, LPCTSTR pszEventParameterMessageFile, DWORD dwTypesSupported, DWORD dwCategoryCount)
{
  //Validate our parameters
  AFXASSUME(lpszLogName);
  ASSERT(_tcslen(lpszLogName));
  AFXASSUME(lpszSourceName);
  ASSERT(_tcslen(lpszSourceName));
  AFXASSUME(lpszEventMessageFile);

  //What will be the return value from this function, assume the worst
  BOOL bSuccess = FALSE;

  //Make the necessary updates to the registry
  CString sKey;
  sKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s"), lpszLogName);
  ATL::CRegKey appKey;
  if (appKey.Open(HKEY_LOCAL_MACHINE, sKey, KEY_WRITE | KEY_READ) == ERROR_SUCCESS)
  {
    ATL::CRegKey sourceKey;
    if (sourceKey.Create(appKey, lpszSourceName, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL) == ERROR_SUCCESS)
		{
      //Write the EventMessageFile string value
      bSuccess = sourceKey.SetStringValue(_T("EventMessageFile"), lpszEventMessageFile) == ERROR_SUCCESS;

      //Write the TypesSupported value
      bSuccess = bSuccess && sourceKey.SetDWORDValue(_T("TypesSupported"), dwTypesSupported) == ERROR_SUCCESS;

      //Write the CategoryCount value if required
      if (dwCategoryCount && bSuccess)
        bSuccess = sourceKey.SetDWORDValue(_T("CategoryCount"), dwCategoryCount) == ERROR_SUCCESS;

      //Write the CategoryMessageFile string if required
      if (pszEventCategoryMessageFile && _tcslen(pszEventCategoryMessageFile) && bSuccess)
        bSuccess = sourceKey.SetStringValue(_T("CategoryMessageFile"), pszEventCategoryMessageFile) == ERROR_SUCCESS;

      //Write the ParameterMessageFile string if required
      if (pszEventParameterMessageFile && _tcslen(pszEventParameterMessageFile) && bSuccess)
        bSuccess = sourceKey.SetStringValue(_T("ParameterMessageFile"), pszEventParameterMessageFile) == ERROR_SUCCESS;

      //Update the sources registry key so that the event viewer can filter on the events which we write to the event log
      if (bSuccess)
      {
        CStringArray sources;
        if (GetStringArrayFromRegistry(appKey, _T("Sources"), sources))
        {
          //If our name is not in the array then add it
          BOOL bFoundMyself = FALSE;
          for (int i=0; i<sources.GetSize() && !bFoundMyself; i++)
            bFoundMyself = (sources.GetAt(i) == lpszSourceName);
          if (!bFoundMyself)
          {
            sources.Add(lpszSourceName);
            SetStringArrayIntoRegistry(appKey, _T("Sources"), sources);
          }
        }
      }
    }
  }

  return bSuccess;
}

BOOL CNTEventLogSource::Uninstall(LPCTSTR lpSourceName)
{
  //Just delegate the work to the other version of "Install"
  return Uninstall(_T("Application"), lpSourceName);
}

BOOL CNTEventLogSource::Uninstall(LPCTSTR lpszLogName, LPCTSTR lpszSourceName)
{
  //Validate our parameters
  AFXASSUME(lpszLogName);
  ASSERT(_tcslen(lpszLogName));
  AFXASSUME(lpszSourceName);
  ASSERT(_tcslen(lpszSourceName));

  //Remove the settings from the registry
  CString sSubKey;
  sSubKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s\\%s"), lpszLogName, lpszSourceName);
  long nSuccess = RegDeleteKey(HKEY_LOCAL_MACHINE, sSubKey);
  if (nSuccess != ERROR_SUCCESS) //If we cannot delete this registry key, then abort this function before we go any further
  {
    SetLastError(nSuccess); //Make the last error value available to our callers 
    return FALSE;
  }

  //Remove ourself from the "Sources" registry key
  sSubKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s"), lpszLogName);
  ATL::CRegKey appKey;
  if (appKey.Open(HKEY_LOCAL_MACHINE, sSubKey, KEY_WRITE | KEY_READ) == ERROR_SUCCESS)
  {
    CStringArray sources;
    if (GetStringArrayFromRegistry(appKey, _T("Sources"), sources))
    {
      //If our name is in the array then remove it
      BOOL bFoundMyself = FALSE;
      for (int i=0; i<sources.GetSize() && !bFoundMyself; i++)
      {
        bFoundMyself = (sources.GetAt(i) == lpszSourceName);
        if (bFoundMyself)
          sources.RemoveAt(i);
      }
      if (bFoundMyself)
        SetStringArrayIntoRegistry(appKey, _T("Sources"), sources);
    }
  }

  return TRUE;
}

BOOL CNTEventLogSource::GetStringArrayFromRegistry(ATL::CRegKey& key, LPCTSTR lpszEntry, CStringArray& array, DWORD* pLastError)
{
  //Validate our parameters
	ASSERT(lpszEntry != NULL);

  //What will be the return value from this function, assume the worst
  BOOL bSuccess = FALSE;

  //Empty the array before we go any further
  array.RemoveAll();

	DWORD dwType = 0;
  ULONG nBytes = 0;
  LONG lResult = key.QueryValue(lpszEntry, &dwType, NULL, &nBytes);
	if (lResult == ERROR_SUCCESS)
	{
    //Allocate some memory for the API
    ATL::CHeapPtr<TCHAR> lpBuffer;
    ULONG nChars = nBytes / sizeof(TCHAR);
    if (!lpBuffer.Allocate(nChars))
    {
      SetLastError(ERROR_OUTOFMEMORY);
      if (pLastError)
        *pLastError = ERROR_OUTOFMEMORY;
      return FALSE;
    }

    lResult = key.QueryMultiStringValue(lpszEntry, lpBuffer, &nChars);      
    if (lResult == ERROR_SUCCESS)
    {
      LPTSTR lpszStrings = lpBuffer.m_pData;
      while (lpszStrings[0] != 0)
      {
        array.Add(lpszStrings);
        lpszStrings += (_tcslen(lpszStrings) + 1);
      }

      bSuccess = TRUE;
    }
    else
    {
      if (pLastError)
        *pLastError = lResult;
    }
	}
  else
  {
    if (pLastError)
      *pLastError = lResult;
  }

  return bSuccess;
}

BOOL CNTEventLogSource::SetStringArrayIntoRegistry(ATL::CRegKey& key, LPCTSTR lpszEntry, const CStringArray& array, DWORD* pLastError)
{   
  //Validate our input parameters
  ASSERT(lpszEntry != NULL);

  //Work out the size of the buffer we will need
  DWORD dwSize = 0;
  INT_PTR nStrings = array.GetSize();
  for (INT_PTR i=0; i<nStrings; i++)
    dwSize += array.GetAt(i).GetLength() + 1; //1 extra for each NULL terminator

  //Need one second NULL for the double NULL at the end
  dwSize++;

  //Allocate some memory for the API
  ATL::CHeapPtr<TCHAR> lpBuffer;
  if (!lpBuffer.Allocate(dwSize))
  {
    SetLastError(ERROR_OUTOFMEMORY);
    if (pLastError)
      *pLastError = ERROR_OUTOFMEMORY;
    return FALSE;
  }

  //Now copy the strings into the buffer
  int nCurOffset = 0;
  LPTSTR lpszString = lpBuffer.m_pData;
  for (INT_PTR i=0; i<nStrings; i++)
  {
    CString sText(array.GetAt(i));
    int nCurrentStringLength = sText.GetLength();
    _tcscpy_s(&lpszString[nCurOffset], nCurrentStringLength+1, sText);
    nCurOffset += (nCurrentStringLength + 1);
  }
  //Don't forgot to doubly NULL terminate
  lpszString[nCurOffset] = _T('\0');

  //Finally write it into the registry
  LONG lResult = key.SetMultiStringValue(lpszEntry, lpBuffer);
	BOOL bSuccess = (lResult == ERROR_SUCCESS);
  if (!bSuccess && pLastError)
    *pLastError = lResult;

  return bSuccess;
}
