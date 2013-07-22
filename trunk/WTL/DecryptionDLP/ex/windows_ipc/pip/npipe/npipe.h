/*
Module : NPIPE.H
Purpose: Defines the interface for an MFC wrapper class for Win32 Named Pipes
Created: PJN / 2-08-1998

Copyright (c) 1998 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////// Macros / Defines //////////////////////////////////////

#pragma once

#ifndef __NPIPE_H__
#define __NPIPE_H__

#ifndef CNAMEDPIPE_EXT_CLASS
#define CNAMEDPIPE_EXT_CLASS
#endif


/////////////////////// Classes ///////////////////////////////////////////////

//Exception class
class CNAMEDPIPE_EXT_CLASS CNamedPipeException : public CException
{
public:
//Constructors / Destructors
	CNamedPipeException(DWORD dwError);

//Methods
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL GetErrorMessage(__out_ecount_z(nMaxError) LPTSTR lpstrError, __in UINT nMaxError, __out_opt PUINT pnHelpContext = NULL);
	CString GetErrorMessage();

//Member variables
	DWORD m_dwError;

protected:
	DECLARE_DYNAMIC(CNamedPipeException)
};

//Wrapper class to encapsulate a named pipe
class CNAMEDPIPE_EXT_CLASS CNamedPipe
{
public:
//Constructors / Destructors
  CNamedPipe();
  ~CNamedPipe();

//Creation & Opening
  void Create(LPCTSTR lpszName, DWORD dwOpenMode, DWORD dwPipeMode, DWORD dwMaxInstances, DWORD dwOutBufferSize, 
              DWORD dwInBufferSize, DWORD dwDefaultTimeOut, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
  void Open(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwDesiredAccess, 
            DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, 
            DWORD dwFlagsAndAttributes = 0);
  operator HANDLE() const { return m_hPipe; };
  void Close();
  void Attach(HANDLE hPipe);
  HANDLE Detach();

//General functions
  void  ConnectClient(LPOVERLAPPED lpOverlapped = NULL);
  void  DisconnectClient();
  void  Flush();
  DWORD Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, 
              LPOVERLAPPED lpOverlapped = NULL);	
  void  Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten, 
              LPOVERLAPPED lpOverlapped = NULL);
  void  Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, 
             LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);	
  DWORD Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, 
             LPOVERLAPPED lpOverlapped = NULL);
  void  Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead, 
             LPOVERLAPPED lpOverlapped = NULL);
  void  Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, 
             LPOVERLAPPED lpOverlapped,  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
  DWORD Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD* lpdwTotalBytesAvail, DWORD* lpdwBytesLeftThisMessage);
  void  Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD* lpdwBytesRead, DWORD* lpdwTotalBytesAvail, DWORD* lpdwBytesLeftThisMessage);
  void  Transact(LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer,
                 DWORD dwOutBufferSize, DWORD& dwBytesRead, LPOVERLAPPED lpOverlapped = NULL); 

//State accessors
  BOOL    IsOpen() const { return (m_hPipe != INVALID_HANDLE_VALUE); };
  BOOL    IsBlockingPipe() const;
  BOOL    IsClientPipe() const;
  BOOL    IsServerPipe() const;
  BOOL    IsMessagePipe() const;
  DWORD   GetCurrentInstances() const;
  DWORD   GetMaxCollectionCount() const;
  DWORD   GetCollectionTimeout() const;
  DWORD   GetOutboundBufferSize() const;
  DWORD   GetInboundBufferSize() const;
  CString GetClientUserName() const;
  DWORD   GetMaxInstances() const;

//State accessors
  void SetMode(BOOL bByteMode, BOOL bBlockingMode);
  void SetMaxCollectionCount(DWORD dwCollectionCount);
  void SetCollectionTimeout(DWORD dwDataTimeout);

//Static methods
  static DWORD Call(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, LPVOID lpInBuffer, DWORD dwInBufferSize, LPVOID lpOutBuffer, 
                    DWORD dwOutBufferSize, DWORD dwTimeOut);
  static void ServerAvailable(LPCTSTR lpszServerName, LPCTSTR lpszPipeName, DWORD dwTimeOut);
  static void ThrowNamedPipeException(DWORD dwError = 0);

protected:
//Member variables
  HANDLE m_hPipe;
};

#endif //__NPIPE_H__
