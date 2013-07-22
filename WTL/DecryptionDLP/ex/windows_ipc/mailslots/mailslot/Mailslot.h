/*
Module : MAILSLOT.H
Purpose: Defines the interface for a wrapper class for Win32 Mailslots
Created: PJN / 18-06-1997

Copyright (c) 1997 - 2008 by PJ Naughter.  (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __MAILSLOT_H__
#define __MAILSLOT_H__

#ifndef CMAILSLOT_EXT_CLASS
#define CMAILSLOT_EXT_CLASS
#endif
                 
                 
/////////////////////////////// Classes ///////////////////////////////////////                      

class CMAILSLOT_EXT_CLASS CServerMailslot
{
public:
//Constructors / Destructors
  CServerMailslot();
  ~CServerMailslot();

//Operations
  BOOL  Open(LPCTSTR lpszName, DWORD dwMaxMessageSize = 0, DWORD dwReadTimeout = 0,
             LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
  BOOL  Close();
  BOOL  Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead);
  BOOL  MessageWaiting();
  DWORD MessageCount();
  DWORD SizeOfWaitingMessage();
  DWORD MaximumMessageSize();
  BOOL  SetReadTimeout(DWORD dwReadTimeout);
  BOOL  GetCreationTime(LPFILETIME lpCreationTime);
  BOOL  GetLastAccessTime(LPFILETIME lpLastAccessTime);
  BOOL  GetLastWriteTime(LPFILETIME lpLastWriteTime);
  BOOL  IsOpen() const;

//If you really must have access to the internal handle then here
//it is. You should only use this as a last resource!
  operator HANDLE() { return m_hMailslot; };

protected:
//Member variables
  HANDLE m_hMailslot;
};

class CMAILSLOT_EXT_CLASS CClientMailslot
{
public:
//Constructors / Destructors
  CClientMailslot();
  ~CClientMailslot();

//Operations
  BOOL Open(LPCTSTR lpszComputerOrDomainName, LPCTSTR lpszName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
  BOOL Close();
  BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten);	
  BOOL IsOpen() const;

//If you really must have access to the internal handle then here
//it is. You should only use this as a last resource
  operator HANDLE() { return m_hMailslot; };

protected:
//Member variables
  HANDLE m_hMailslot;
};

#endif //__MAILSLOT_H__
