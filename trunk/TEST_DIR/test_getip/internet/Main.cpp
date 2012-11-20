#include "stdafx.h"
#include <stdio.h>
#include "ipenum.h"


class CMyIPEnum : public CIPEnum
{
  virtual BOOL EnumCallbackFunction(int nAdapter, const in_addr& address);
};

BOOL CMyIPEnum::EnumCallbackFunction(int nAdapter, const in_addr& address)
{
  printf("%0d        %d.%d.%d.%d\n", nAdapter, address.S_un.S_un_b.s_b1, 
         address.S_un.S_un_b.s_b2, address.S_un.S_un_b.s_b3, address.S_un.S_un_b.s_b4);

  //in this case where we are just displaying the IP address 
  //always continue enumeration
  return TRUE;
}

void _tmain(int argc, TCHAR *argv[])
{                      
  if ((argc > 1) && (_tcsstr(argv[1], _T("H")) || _tcsstr(argv[1], _T("h")) || _tcsstr(argv[1], _T("?"))))
  {
    _tprintf(_T("IPEnum enumerates the IP address for the local machine\n"));
    _tprintf(_T("Just run IPEnum without any paramters to run it\n"));
  }
  else
  {
    _tprintf(_T("Adapter  IP Address\n"));
    try
    {
      CMyIPEnum ip;
      ip.Enumerate();
    }
    catch(CNotSupportedException*)
    {
      _tprintf(_T("An error occurred while trying to detect the IP address\n"));
      _tprintf(_T("You should check to see to make sure TCPIP is installed correctly\n"));
    }
  }
  getchar();
}