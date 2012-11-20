#include "stdafx.h"
#include "ipenum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CIPEnum::CIPEnum()
{
  m_bWinsockInitialied = FALSE;

  //Initialise the winsock stack
  WORD wVersionRequested = MAKEWORD(1, 1);  
  WSADATA wsaData; 
  int err = WSAStartup(wVersionRequested, &wsaData); 
  if (err != 0) 
  {
    TRACE(_T("Failed in call to WSAStartup, return value was %d\n"), err);
    AfxThrowNotSupportedException();
  }

  //remember that we have opened winsock
  m_bWinsockInitialied = TRUE;

  //Code requires at least Winsock 1.1
  if ((LOBYTE(wsaData.wVersion) != 1) || (HIBYTE(wsaData.wVersion) != 1)) 
  { 
    TRACE(_T("Failed to find a usable winsock stack which supports Winsock 1.1\n"));
    AfxThrowNotSupportedException();
  } 
}


CIPEnum::~CIPEnum()
{
  //release our use of winsock stack if we successfully initialised it
  if (m_bWinsockInitialied) 
    WSACleanup(); 
}


BOOL CIPEnum::Enumerate()
{
  //get this machines host name
  char szHostname[256];
  if (gethostname(szHostname, sizeof(szHostname)))
  {
    TRACE(_T("Failed in call to gethostname, WSAGetLastError returns %d\n"), WSAGetLastError());
    return FALSE;
  }

  //get host information from the host name
  HOSTENT* pHostEnt = gethostbyname(szHostname);
  if (pHostEnt == NULL)
  {
    TRACE(_T("Failed in call to gethostbyname, WSAGetLastError returns %d\n"), WSAGetLastError());
    return FALSE;
  }

  //check the length of the IP adress
  if (pHostEnt->h_length != 4)
  {
    TRACE(_T("IP address returned is not 32 bits !!\n"));
    return FALSE;
  }

  //call the virtual callback function in a loop
  int nAdapter = 0;
  BOOL bContinue = TRUE;
  while (pHostEnt->h_addr_list[nAdapter] && bContinue)
  {
    in_addr address;
    CopyMemory(&address.S_un.S_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
    bContinue = EnumCallbackFunction(nAdapter, address);
    nAdapter++;
  }

  return TRUE;
}
