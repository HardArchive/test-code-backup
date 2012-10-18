// stdafx.cpp : source file that includes just the standard includes
//	SimpleScanner.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
//computer the checksum of the data 
USHORT checksum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	
	while(size >1) 
	{ 
		cksum+=*buffer++; 	
		size -=sizeof(USHORT); 
	} 
	
	if (size) 
	{ 
		cksum += *(UCHAR*)buffer; 	
	} 
	
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	
	return (USHORT)(~cksum); 
}

//获得机器地址
LPSTR GetLocalIpAddr(LPSTR ip)
{
	  WORD wVersionRequested;
      WSADATA wsaData;
      char name[255];
      PHOSTENT hostinfo;
      wVersionRequested = MAKEWORD( 2, 0 );

      if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
      {

            if( gethostname ( name, sizeof(name)) == 0)
            {
                  if((hostinfo = gethostbyname(name)) != NULL)
                  {
                        ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
						return ip;
                  }else return NULL;
            }else return NULL;
            
            WSACleanup( );
      } else return NULL;
}


