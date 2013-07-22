/*
Module : APIPE.H
Purpose: Defines the interface for a C++ wrapper class for Win32 Anonymous Pipes

Copyright (c) 1998 - 2008 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

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

#ifndef __APIPE_H__
#define __APIPE_H__

#ifndef CANONYMOUSPIPE_EXT_CLASS
#define CANONYMOUSPIPE_EXT_CLASS
#endif


/////////////////////////////// Classes ///////////////////////////////////////

class CANONYMOUSPIPE_EXT_CLASS CAnonymousPipe
{
public:
//Constructors / Destructors
  CAnonymousPipe();
  ~CAnonymousPipe();

//Creation & Opening
  BOOL Create(LPSECURITY_ATTRIBUTES lpPipeAttributes = NULL, DWORD dwSize = 0);

//General functions
  BOOL Write(LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, DWORD& dwNumberOfBytesWritten);	
  BOOL Read(LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, DWORD& dwNumberOfBytesRead);
  BOOL Peek(LPVOID lpBuffer, DWORD dwBufferSize, DWORD* lpdwBytesRead, DWORD* lpdwTotalBytesAvail, DWORD* lpdwBytesLeftThisMessage);
  BOOL Close();

//State accessors
  BOOL IsOpen() const { return ( m_hWrite != INVALID_HANDLE_VALUE); };
  HANDLE GetWriteHandle() const { return m_hWrite; };
  HANDLE GetReadHandle() const { return m_hRead; };

protected:
//Member variables
  HANDLE m_hWrite;
  HANDLE m_hRead;
};

#endif //__APIPE_H__
