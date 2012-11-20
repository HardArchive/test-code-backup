/*
Module : IPENUM.H
Purpose: Interface for an MFC class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/


////////////////////////////////// Macros ///////////////////////////

#ifndef __IPENUM_H__
#define __IPENUM_H__



class CIPEnum
{
public:
//constructors / Destructors
  CIPEnum();
  ~CIPEnum();

//methods
  BOOL Enumerate();

protected:
  virtual BOOL EnumCallbackFunction(int nAdapter, const in_addr& address) = 0;
  BOOL m_bWinsockInitialied;
};




#endif //__IPENUM_H__