/*
Module : APIPE.CPP
Purpose: Defines the implementation for an C++ wrapper class for Win32 anonymous Pipes
Created: PJN / 02-08-1998
History: PJN / 29-12-2006 1. Updated copyright details
                          2. Optimized CAnonymousPipe constructor code
                          3. Addition of a CANONYMOUSPIPE_EXT_CLASS preprocessor macro to allow the classes
                          to be more easily added to an extension dll
                          4. Remove derivation from CObject as it was not really required
                          5. Updated the documentation to use the same style as the web site.
                          6. Addition of a GetWriteHandle and GetReadHandle methods to provide access to
                          the low level handles which the class encapsulates.
                          7. Updated the code to clean compile on VC 2005
         PJN / 02-09-2008 1. Updated copyright details
                          2. Code now compiles cleanly using Code Analysis (/analyze)
                          3. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          4. Updated sample app to clean compile on VC 2008
                          5. The code has now been updated to support VC 2005 or later only. 
                          6. Reworked some of the Peek parameters to be pointers rather than references.
                          
Copyright (c) 1998 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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
#include "Apipe.h"


///////////////////////////////// Macros / Defines ////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


///////////////////////////////// Implementation //////////////////////////////

CAnonymousPipe::CAnonymousPipe() : m_hWrite(INVALID_HANDLE_VALUE),
                                   m_hRead(INVALID_HANDLE_VALUE)
{
}

CAnonymousPipe::~CAnonymousPipe()
{
  Close();
}

BOOL CAnonymousPipe::Create(LPSECURITY_ATTRIBUTES lpPipeAttributes, DWORD dwSize)
{
  ASSERT(!IsOpen()); //Pipe must be open

  BOOL bSuccess = ::CreatePipe(&m_hRead, &m_hWrite, lpPipeAttributes, dwSize);
  if (!bSuccess)
    TRACE1("CAnonymousPipe::Create() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten)
{
  ASSERT(IsOpen()); //Pipe must be open

  BOOL bSuccess = ::WriteFile(m_hWrite, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, NULL);
  if (!bSuccess)
    TRACE1("CAnonymousPipe::Write() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead)
{
  ASSERT(IsOpen()); //Pipe must be open

  BOOL bSuccess = ::ReadFile(m_hRead, lpBuffer, dwNumberOfBytesToRead, &dwNumberOfBytesRead, NULL);
  if (!bSuccess)
    TRACE1("CAnonymousPipe::Read() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD* lpdwBytesRead, DWORD* lpdwTotalBytesAvail, DWORD* lpdwBytesLeftThisMessage)
{
  ASSERT(IsOpen()); //Pipe must be open

  //预览一个管道中的数据，或取得与管道中的数据有关的信息
  BOOL bSuccess = ::PeekNamedPipe(m_hRead, lpBuffer, dwBufferSize, lpdwBytesRead, lpdwTotalBytesAvail, lpdwBytesLeftThisMessage);
  if (!bSuccess)
    TRACE1("CNamedPipe::Peek() failed, GetLastError returned %d\n", ::GetLastError());

  return bSuccess;
}

BOOL CAnonymousPipe::Close()
{
  //What will be the return value from this function (assume the best)
  BOOL bSuccess = TRUE;

  if (IsOpen())
  {
    BOOL bSuccess1 = CloseHandle(m_hWrite);
    if (!bSuccess1)
      TRACE1("CAnonymousPipe::Close() failed, GetLastError returned %d\n", GetLastError());
    m_hWrite = INVALID_HANDLE_VALUE;

    BOOL bSuccess2 = CloseHandle(m_hRead);
    if (!bSuccess2)
      TRACE1("CAnonymousPipe::Close() failed, GetLastError returned %d\n", GetLastError());
    m_hRead = INVALID_HANDLE_VALUE;

    bSuccess = bSuccess1 && bSuccess2;
  }

  return bSuccess;
}
