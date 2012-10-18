// Ping.cpp: implementation of the CPing class.
//Author AsianChai
//date   2005-5-20
//function Send icmp package to the destnation to determine wheather
// the victim is alive
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleScanner.h"
#include "Ping.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPing::CPing()
{

}

CPing::~CPing()
{

}

//send the request icmp to the destination 
int CPing::SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr)
{
	static ECHOREQUEST echoReq;
	static u_short nId = 1;
	static u_short nSeq = 1;
	int nRet;

	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ; //设置ICMP报为Ping类型
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= nId; 
	echoReq.icmpHdr.Seq			= nSeq++;

	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' '+nRet;

	// Save tick count when sent
	echoReq.dwTime				= GetTickCount();

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = checksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  								  
	nRet = sendto(s,						/* socket */
				 (LPSTR)&echoReq,			/* buffer */
				 sizeof(ECHOREQUEST),
				 0,							/* flags */
				 (LPSOCKADDR)lpstToAddr, /* destination */
				 sizeof(SOCKADDR_IN));   /* address length */

	return (nRet);

}


//set the time out of the socket 
int CPing::WaitForEchoReply(SOCKET s)
{
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 1;
    Timeout.tv_usec = 0;
	return(select(1, &readfds, NULL, NULL, &Timeout));
}

//Connect to the scanning port, if the port open, return true,
//other
BOOL CPing::HostReachable(CString strHost)
{

   	SOCKET	  rawSocket;
 	struct    sockaddr_in saDest;
// 	struct    sockaddr_in saSrc;
 	LPHOSTENT lpHost;
 	CString str;
 	int nRet;
// 	DWORD	  dwTimeSent,dwElapsed;
// 	u_char    cTTL;
	long	m_laddr;

	//initiate the socket 
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
	{
	
	 AfxMessageBox("WSAStartup() failed: %d\n");
	 return -1;
	}
    rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == INVALID_SOCKET)
	{
	
	AfxMessageBox("WSASocket() failed: %d\n");
	
	return -1;
	}

	
	//get the ip adress	
	m_laddr = inet_addr(strHost);
	if (m_laddr == INADDR_NONE)
	{
	  lpHost = gethostbyname(strHost);
 			if (lpHost == NULL)
 			{ 				
				return false;
			}
 			saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
		}
		else
		{
			saDest.sin_addr.s_addr =m_laddr;
		}
 		saDest.sin_family = AF_INET;
 		saDest.sin_port = 0;

   //send the package
    SendEchoRequest(rawSocket,&saDest);
	//if time out
	nRet = WaitForEchoReply(rawSocket);
	if(!nRet) //time out 
	{
     return false;
	}

	ECHOREPLY echoReply;
	//int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	// Receive the echo reply	
   nRet=recvfrom( rawSocket,	
		            (LPSTR)&echoReply,	// buffer
					sizeof(ECHOREPLY),	// size of buffer
					0,					// flags
					(LPSOCKADDR)&saDest,	// From address
					&nAddrLen);			// pointer to address len

	//close the socket
	 closesocket(rawSocket);
	 WSACleanup() ;
	 
	 
	// Check return value
    if(echoReply.echoRequest.icmpHdr.Code==0)
		return true;
	else
		return false;
}
/*
BOOL CPing::HostReachable(CString strHost)
{
    //CThreadInfo* pThreadInfo = (CThreadInfo*)pParam;
	//HWND	m_hWnd = pThreadInfo->m_hwndNotifyWnd;	

 	SOCKET	  rawSocket;
 	struct    sockaddr_in saDest;
 	struct    sockaddr_in saSrc;
 	LPHOSTENT lpHost;
 	CString str;
 	int nRet;
 	DWORD	  dwTimeSent,dwElapsed;
 	u_char    cTTL;
	long	m_laddr;

 	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	while (TRUE)
	{			
		
		
		m_laddr = inet_addr(strHost);
		if (m_laddr == INADDR_NONE)
		{
			lpHost = gethostbyname(strHost);
 			if (lpHost == NULL)
 			{ 				
				return false;
			}
 			saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
		}
		else
		{
			saDest.sin_addr.s_addr =m_laddr;
		}
 		saDest.sin_family = AF_INET;
 		saDest.sin_port = 0;
 	
 		for (int nLoop = 0; nLoop < 3; nLoop++)
 		{
 			
 			SendEchoRequest(rawSocket, &saDest);
 			nRet = WaitForEchoReply(rawSocket);
 			if (!nRet)
 			{ 	 		
			 return false;
 			}
 			else
 			{
 			
 				dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL);
			
 				dwElapsed = GetTickCount() - dwTimeSent;
				CString str;
				str.Format("%d",dwElapsed);
				AfxMessageBox(str);
				if(dwElapsed<0)
				{
					return false;
				}
 							
                 else
				 {
					 return true;
				 }
 			}			
 		} 	 	
 		
	
	}
	nRet = closesocket(rawSocket);
	
	
}
*/


