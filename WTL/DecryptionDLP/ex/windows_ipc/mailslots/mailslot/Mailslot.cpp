/*
Module : MAILSLOT.CPP
Purpose: Defines the implementation for a wrapper class for Win32 Mailslots
Created: PJN / 18-06-1997
History: PJN / 25-07-1998 1. Provision of Unicode build configurations
                          2. Updated make file to VC 5 format
													3. Fixed a bug in CClientMailslot destructor
													4. CServerMailslot::Close() & CClientMailslot::Close() now work correctly 
                          when the mailslot is already closed
                          5. Fixed a level 4 warning in the CWinNotifyDoc in the	demo program.
													6. Changed 2 functions which used a pointer parameter to now use a C++ reference instead.
         PJN / 10-08-1998 1. Close function of both classes now correctly sets 
                          mailslot handle back to INVALID_HANDLE_VALUE
         PJN / 26-07-2003 1. Updated copyright details.
                          2. Fixed issue in CClientMailslot::Open which was causing it to only allow one 
                          instance of the mailslot to be opened. Thanks to GdP Software for reporting this problem.
         PJN / 22-12-2006 1. Updated copyright details
                          2. Updated the documentation to use the same style as the web site
                          3. Optimized CServerMailslot constructor code
                          4. Addition of a CMAILSLOT_EXT_CLASS preprocessor macro to allow the classes to be more 
                          easily added to an extension dll
                          5. Reviewed all TRACE statements for correctness
                          6. Optimized CClientMailslot constructor code
                          7. Updated the code to clean compile on VC 2005
                          8. Code now uses newer C++ style casts instead of C style casts.
                          9. Changed the default size of the formview window in the sample ap to avoid scroll bars
                          using default fonts.
         PJN / 23-12-2006 1. Code now uses CString instead of raw TCHAR buffers in various functions
                          2. Classes now do not derive from CObject as it was not really required
         PJN / 08-09-2007 1. Updated copyright details.
                          2. Minor code tidy up.
         PJN / 07-09-2008 1. Updated copyright details.
                          2. Code now compiles cleanly using Code Analysis (/analyze)
                          3. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          4. Updated sample app to clean compile on VC 2008
                          5. The code has now been updated to support VC 2005 or later only. 
                          6. Removed VC 6 style AppWizard comments from the code.
                          7. Updated the sample app to improve on the error reporting.

Copyright (c) 1997 - 2007 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


////////////////////////////////// Includes  //////////////////////////////////

#include "stdafx.h"
#include "Mailslot.h"


////////////////////////////////// Macros / Defines ///////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


////////////////////////////////// Implementation /////////////////////////////

CServerMailslot::CServerMailslot() : m_hMailslot(INVALID_HANDLE_VALUE)
{
}

CServerMailslot::~CServerMailslot()
{
  Close();
}

BOOL CServerMailslot::Close()
{
	BOOL bSuccess = TRUE;
  if (IsOpen())
  {
    bSuccess = CloseHandle(m_hMailslot);
    m_hMailslot = INVALID_HANDLE_VALUE;
  }

  if (!bSuccess)
    TRACE(_T("CServerMailslot::Close() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CServerMailslot::IsOpen() const
{
  return (m_hMailslot != INVALID_HANDLE_VALUE);
}

BOOL CServerMailslot::Open(LPCTSTR lpszName, DWORD dwMaxMessageSize, DWORD dwReadTimeout, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  //Validate our parameters
  ASSERT(!IsOpen());

  //the class encapsulates creating the mailslot name, all that is required is
  //a simple name for the mailslot e.g. lpName = PJSLOT will create the mailslot
  //name \\.\mailslot\PJSLOT
  CString sMailslotName(_T("\\\\.\\mailslot\\"));
  sMailslotName += lpszName;

  m_hMailslot = CreateMailslot(sMailslotName, dwMaxMessageSize, dwReadTimeout, lpSecurityAttributes);
  if (m_hMailslot == INVALID_HANDLE_VALUE)
    TRACE(_T("CServerMailslot::Open() failed, GetLastError returned %d\n"), GetLastError());

  return (m_hMailslot != INVALID_HANDLE_VALUE);
}

BOOL CServerMailslot::MessageWaiting()
{
  //Validate our parameters
  ASSERT(IsOpen());

  DWORD dwCount = 0;
  BOOL bSuccess = GetMailslotInfo(m_hMailslot, NULL, NULL, &dwCount, NULL);

  if (!bSuccess)
    TRACE(_T("CServerMailslot::MessageWaiting() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess && (dwCount != 0);
}

DWORD CServerMailslot::MessageCount()
{
  //Validate our parameters
  ASSERT(IsOpen());

  DWORD dwCount = 0;
  BOOL bSuccess = GetMailslotInfo(m_hMailslot, NULL, NULL, &dwCount, NULL);

  if (!bSuccess)
    TRACE(_T("CServerMailslot::MessageCount() failed, GetLastError returned %d\n"), GetLastError());

  if (bSuccess)
    return dwCount;
  else
    return 0;
}

DWORD CServerMailslot::SizeOfWaitingMessage()
{
  //Validate our parameters
  ASSERT(IsOpen());

  DWORD dwSize = 0;
  BOOL bSuccess = GetMailslotInfo(m_hMailslot, NULL, &dwSize, NULL, NULL);

  if (!bSuccess)
    TRACE(_T("CServerMailslot::SizeOfWaitingMessage() failed, GetLastError returned %d\n"), GetLastError());

  if (bSuccess)
    return dwSize;
  else
    return 0;
}

DWORD CServerMailslot::MaximumMessageSize()
{
  //Validate our parameters
  ASSERT(IsOpen());

  DWORD dwMaxSize = 0;
  BOOL bSuccess = GetMailslotInfo(m_hMailslot, &dwMaxSize, NULL, NULL, NULL);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::MaxmimumMessageSize() failed, GetLastError returned %d\n"), GetLastError());

  if (bSuccess)
    return dwMaxSize;
  else
    return 0;
}

BOOL CServerMailslot::SetReadTimeout(DWORD dwReadTimeout)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = SetMailslotInfo(m_hMailslot, dwReadTimeout);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::SetReadTimeout() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CServerMailslot::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = ReadFile(m_hMailslot, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, NULL);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::Read() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CServerMailslot::GetCreationTime(LPFILETIME lpCreationTime)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = GetFileTime(m_hMailslot, lpCreationTime, NULL, NULL);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::GetCreationTime() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CServerMailslot::GetLastAccessTime(LPFILETIME lpLastAccessTime)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = GetFileTime(m_hMailslot, NULL, lpLastAccessTime, NULL);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::GetLastAccessTime() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CServerMailslot::GetLastWriteTime(LPFILETIME lpLastWriteTime)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = GetFileTime(m_hMailslot, NULL, NULL, lpLastWriteTime);
  if (!bSuccess)
    TRACE(_T("CServerMailslot::GetLastWriteTime() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}


CClientMailslot::CClientMailslot() : m_hMailslot(INVALID_HANDLE_VALUE)
{
}

CClientMailslot::~CClientMailslot()
{
  Close();
}

BOOL CClientMailslot::Close()
{
  BOOL bSuccess = TRUE;
  if (IsOpen())
  {
    bSuccess = CloseHandle(m_hMailslot);
    m_hMailslot = INVALID_HANDLE_VALUE;
  }

  if (!bSuccess)
    TRACE(_T("CClientMailslot::Close() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}

BOOL CClientMailslot::IsOpen() const
{
  return (m_hMailslot != INVALID_HANDLE_VALUE);
}

BOOL CClientMailslot::Open(LPCTSTR lpszComputerOrDomainName, LPCTSTR lpszName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
  //Validate our parameters
  ASSERT(!IsOpen());

  //the function encapsulates creating the mailslot name by breaking down the
  //creation of the mailslot name into the ComputerOrDomain and the actual mailslot itself
  //e.g. lpComputerOrDomainName = PJ, lpName = PJSLOT will create the actual mailslot
  //name of "\\PJ\mailslot\PJSLOT"
  CString sMailslotName;
  sMailslotName.Format(_T("\\\\%s\\mailslot\\%s"), lpszComputerOrDomainName, lpszName);

  m_hMailslot = CreateFile(sMailslotName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, lpSecurityAttributes,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (m_hMailslot == INVALID_HANDLE_VALUE)
    TRACE(_T("CClientMailslot::Open() failed, GetLastError returned %d\n"), GetLastError());

  return (m_hMailslot != INVALID_HANDLE_VALUE);
}

BOOL CClientMailslot::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten)
{
  //Validate our parameters
  ASSERT(IsOpen());

  BOOL bSuccess = WriteFile(m_hMailslot, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, NULL);
  if (!bSuccess)
    TRACE(_T("CClientMailslot::Write() failed, GetLastError returned %d\n"), GetLastError());

  return bSuccess;
}
