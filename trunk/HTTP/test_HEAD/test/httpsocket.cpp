/************************************************************************
HTTP socket - DTS Project
Copyright (C) 2000 by Tair Abdurman, All Rights Reserved
http://www.tair.freeservers.com, dts@tair.freeservers.com
No any part of this code can be used in comercial purposes.
************************************************************************/

/************************************************************************
clicksocket.cpp
************************************************************************/
#include "stdafx.h"
#include "httpsocket.h"

//instance counter
int CHTTPSocket::nInstanceCount=0;

CHTTPSocket::CHTTPSocket()
{
if (nInstanceCount==0)
{
//***********win32 specific **********************************************
	// WSA versions
	BYTE vCount=3;
	int vInfo[3][3]={
		{1,0,0},
		{1,1,0},
		{2,0,0}
	};

	
	WORD vMMVersion=0;
	int nStartupStatus; 
	
	wsaData.wVersion=0;

//******* Dynamically detect winsock version, from newest to oldest
	while(vCount>0)
	{
	  vMMVersion=MAKEWORD(vInfo[vCount-1][0],vInfo[vCount-1][1]);

      nStartupStatus=WSAStartup(vMMVersion,&wsaData);
	  if (nStartupStatus==0)
		  break;
	  else
		  vInfo[vCount-1][2]=0;
	  WSACleanup();
	  vCount--;
	}
//***********win32 specific **********************************************
}

sckHTTPSocket=0;
sinHTTPSocket.sin_port=0;
sinHTTPServer.sin_port=0;

//leave with timewait
m_bNoTimeWait=FALSE;
// recieve timeout to default value
m_nRecvTimeout=HTTPRTIMEOUTDEF;
// send timeout to default value
m_nSendTimeout=HTTPSTIMEOUTDEF;
// remote server host address, max size 64 bytes, 65th set to \0
szcopy(m_szServerHost,"",MAXHOSTLENGTH-1);
// remote server IP address, max size 15 bytes, 16th set to \0
szcopy(m_szServerHostIP,"",MAXIPLENGTH);
// remote server port
m_nServerPort=80;
// use proxy flag
m_bUseProxy=FALSE;
// error code
m_nErrCode=ERR_OK;
// extended error code;
m_nExtErrCode=ERROR_SUCCESS;

m_ulResponseSize=0;
m_szResponse=NULL;


InitInstance();
nInstanceCount++;
};

CHTTPSocket::~CHTTPSocket()
{
nInstanceCount--;
if (nInstanceCount==0)
{
//***********win32 specific **********************************************
	WSACleanup();
//***********win32 specific **********************************************
}
};

void CHTTPSocket::ThrowError(int err, int xerr, const char* errdesc)
{
		  m_nErrCode=err;
		  m_nExtErrCode=xerr;
		  wsprintf(m_nErrInfo,errdesc);
		  OnError();
 };

BOOL CHTTPSocket::InitInstance()
{
	return TRUE;
};

//sprintf like
void CHTTPSocket::szcopy(char* dest,const char* src,int nMaxBytes)
{
	int i_cntr=0;
	while ((src[i_cntr]!='\0') || (i_cntr<nMaxBytes))
	   dest[i_cntr]=src[i_cntr++];
	dest[i_cntr]='\0';
}

void CHTTPSocket::szsent(SOCKET sckDest,const char* szHttp)
{

	   char szSendHeader[MAXHEADERLENGTH];
	   int iLen=strlen(szHttp);
	   szcopy(szSendHeader,szHttp,iLen);
	   if(send (sckHTTPSocket
		        ,(const char FAR *)szSendHeader
                ,iLen
                ,0)==SOCKET_ERROR)
	   {
	      closesocket(sckHTTPSocket);
          ThrowError(ERR_WSAINTERNAL
			         ,WSAGetLastError()
					 ,"Error when send.");
          return;
	   }

}

//set server host
   void  CHTTPSocket::SetServerHost(const char* src)
   {
	   szcopy(m_szServerHost,src,MAXHOSTLENGTH-1);
   };

//set server ip
   void  CHTTPSocket::SetServerHostIP(const char* src)
   {
	   szcopy(m_szServerHostIP,src,MAXIPLENGTH-1);
   };

//on error trigger
   void CHTTPSocket::OnError()
   {
	   memPostup();
   };

//on response trigger
   void CHTTPSocket::OnResponse()
   {
       memPostup();
   };

   bool CHTTPSocket::IsEffective()
   {
	   return false;
   }

//release allocated memory
void CHTTPSocket::memPostup()
{
	   if ((m_ulResponseSize!=0) && (m_szResponse!=NULL))
	   {
           VirtualFree(m_szResponse,m_ulResponseSize,MEM_RELEASE);
		   m_ulResponseSize=0;
		   m_szResponse=NULL;
	   }

}

//perform request
   bool CHTTPSocket::Request(const char* url)
   {
	   m_szResponse=NULL;
	   m_ulResponseSize=0;

      
	   if (url==NULL)
	   {
          ThrowError(ERR_URLNOTHING
			         ,0
					 ,"Error, url is nothing.");
          return false;
	   }
	   else
	   {
	   	   if (strlen(url)==0)
		   {
             ThrowError(ERR_URLNOTHING
			            ,0
				  	    ,"Error, url is nothing.");
             return false;
		   }

	   }

	   if (strlen(url)>MAXURLLENGTH)
	   {
          ThrowError(ERR_URLTOOLONG
			         ,0
					 ,"Error, url too long.");
          return false;
	   }

	   //************* check host - port - URI ****************
	   char tmpSrc[MAXURLLENGTH+1];
	   int iURLSize=0;
	   int iDefPos=0;
	   int iHelpVar=0;
	   int isPort=TRUE;

	   //local copy of URL
	   while (url[iURLSize]!='\0')
	   {
           tmpSrc[iURLSize]=url[iURLSize];
           iURLSize++;
	   }
	   tmpSrc[iURLSize]='\0';
       iURLSize++;

#ifdef DEBON
	   printf("\r\nCached URL: %s\r\n",tmpSrc);
#endif 

	   char* pProto=NULL;
	   char* pHost=NULL;
	   char* pPort=NULL;
	   int nPort=-1;
	   char* pURI=NULL;
	   char* pParams=NULL;

	   //search for "://"
	   iHelpVar=0;
	   iDefPos=0;
	   while ((iHelpVar<iURLSize))
	   {
		   if (tmpSrc[iHelpVar]==':')
		   {
 			   isPort=TRUE;
               //mark as end of string
			   tmpSrc[iHelpVar]='\0';
			   iHelpVar++;
			   if (tmpSrc[iHelpVar]=='/')
			   {
                  iHelpVar++;
			      if (tmpSrc[iHelpVar]=='/')
				  {
					  //mark as proto
					  iHelpVar++;
					  pProto=&tmpSrc[iDefPos];
					  iDefPos=iHelpVar;
				  }
				  else
				  {
                     ThrowError(ERR_PROTOPARSE
			                    ,0
				  	            ,"Error, proto parse failed.");
                     return false;
				  }
			   }
			   else
			   //mark as host
			   {
                 if (pHost==NULL)
				 {
			      pHost=&tmpSrc[iDefPos];
				  iDefPos=iHelpVar;
				 }
			   }

		   }
		   else
		   {
			 if (tmpSrc[iHelpVar]=='/')
			 {
			     isPort=FALSE;
				 tmpSrc[iHelpVar]='\0';
				 if (pHost==NULL)
				    pHost=&tmpSrc[iDefPos];
				 else
                    pPort=&tmpSrc[iDefPos];
				 iDefPos=iHelpVar+1;
				 break;
			 }
		   }
	   iHelpVar++;	   
	   }
	   

	   //tmpSrc[iHelpVar]='\0';
       if(pHost==NULL)
	   {
		 pHost=&tmpSrc[iDefPos];
	   }
	   else
	   {
         if((pPort==NULL) && (isPort))
			   pPort=&tmpSrc[iDefPos];
	     if((pURI==NULL) && (!isPort))
			   pURI=&tmpSrc[iDefPos];
	   }

       
	   if ((pHost==NULL) || (pHost[0]=='\0'))
		  {
		            ThrowError(ERR_BADHOST
		                       ,0
							   ,"Error, bad host in URL.");
                    return false;
		  }
	   if (pPort==NULL)
	   {
		   nPort=80;
	   }
	   else
	   {
	    nPort=atoi((const char*)pPort);
	    if ((nPort<0) || (nPort>65535) || (pPort[0]=='\0'))
		  {
		            ThrowError(ERR_BADPORT
		                       ,0
		                       ,"Error, bad port in URL.");
                    return false;
		  }
	   }

	if (!m_bUseProxy)
	{
           m_szURL[0]='/';
           m_szURL[1]='\0';
		   if (pURI!=NULL)
		   {
		     if (pURI[0]!='\0')
			   szcopy(&m_szURL[1],pURI,strlen(pURI));
		   }
	}
	else
	{
		if (m_szServerHost==NULL)
		{
            ThrowError(ERR_PROXYUNKNOWN
				       ,0
					   ,"Error, proxy not specified.");
			return false;
		}
		else
		{
			if (m_szServerHost[0]=='\0')
			{
               ThrowError(ERR_PROXYUNKNOWN
 				         ,0
					     ,"Error, proxy not specified.");
			   return false;
			}
		}

	}

#ifdef DEBON
	   printf("\r\n");
	   printf("Proto: %s\r\n",pProto);
	   printf("Host: %s\r\n",pHost);
	   printf("Port: %s ==> %d\r\n",pPort,nPort);
	   printf("URI: %s\r\n",pURI);
#endif

   if (m_bUseProxy)
   {
     
	 wsprintf(m_szHost,"%s",pHost);
	 if (pProto==NULL)
	 {
	   wsprintf(m_szURL,"http://%s",url);
	 }
	 else
	 {
		 if (pProto[0]=='\0')
		 {
	       wsprintf(m_szURL,"http://%s",url);
		 }
		 else
	       wsprintf(m_szURL,"%s",url);
	 }

   }
   else
   {
     wsprintf(m_szServerHost,"%s",pHost);
     wsprintf(m_szServerHostIP,"%s",pHost);
     wsprintf(m_szHost,"%s",pHost);
	 wsprintf(m_szURL,"%s",m_szURL);
	 m_nServerPort=nPort;
   }
//************* check host - port - URI ****************

#ifdef DEBON
  printf("Server: %s ServerIP: %s Port: %d \r\n",m_szServerHost,m_szServerHostIP,m_nServerPort);
  printf("Proxy:");
  if (m_bUseProxy)
     printf(" YES\r\n");
  else
     printf(" NO\r\n");
  printf("URL/URI: %s HOST: %s\r\n",m_szURL,m_szHost);
#endif

  sckHTTPSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
       
	   if (sckHTTPSocket==INVALID_SOCKET)
	   {
	      ThrowError(ERR_WSAINTERNAL
		             ,sckHTTPSocket
		             ,"Error when socket.");
          return false;
	   }

	   //off TIME_WAIT
	   struct linger zeroLinger;	
	   zeroLinger.l_onoff = 1;	
	   zeroLinger.l_linger = 0;
	   if(setsockopt(sckHTTPSocket
	                 ,SOL_SOCKET
			         ,SO_LINGER
			         ,(const char *)&zeroLinger
			         ,sizeof(zeroLinger))!=0)
	   {
	      closesocket(sckHTTPSocket);
	      ThrowError(ERR_WSAINTERNAL
		             ,WSAGetLastError()
		             ,"Error when setsockopt(LINGER).");
		  OnError();
          return false;
	   }

	   //set recieve timeout
	   if(setsockopt(sckHTTPSocket
	                 ,SOL_SOCKET
			         ,SO_RCVTIMEO
			         ,(const char *)&m_nRecvTimeout
			         ,sizeof(m_nRecvTimeout))!=0)
	   {
	      closesocket(sckHTTPSocket);
	      ThrowError(ERR_WSAINTERNAL
		             ,WSAGetLastError()
		             ,"Error when setsockopt(RCVTIME).");
          return false;
	   }

	   //set send timeout
	   if(setsockopt(sckHTTPSocket
	          ,SOL_SOCKET
			  ,SO_SNDTIMEO
			  ,(const char *)&m_nSendTimeout
			  ,sizeof(m_nSendTimeout))!=0)
	   {
	      closesocket(sckHTTPSocket);
	      ThrowError(ERR_WSAINTERNAL
		            ,WSAGetLastError()
		            ,"Error when setsockopt(SNDTIMEO).");
          return false;
	   }

       //local settings
	   sinHTTPSocket.sin_addr.s_addr = htonl (INADDR_ANY);
       sinHTTPSocket.sin_family=AF_INET;

       //bind socket
	   if (bind (sckHTTPSocket,                          
                 (const struct sockaddr FAR *)&sinHTTPSocket,  
                 sizeof(sinHTTPSocket))==SOCKET_ERROR)
	   {
	      closesocket(sckHTTPSocket);
	      ThrowError(ERR_WSAINTERNAL
		             ,WSAGetLastError()
		             ,"Error when bind socket.");
          return false;
	   }

       //set HTTP Server's info
	   int tmpErr=0;
	   unsigned long tmpIAddr=0;
	   //try find by name
	   struct hostent FAR * heServHostInfo=NULL;
	   heServHostInfo=gethostbyname((const char FAR *)m_szServerHost);
	   if ((heServHostInfo==NULL) || (strlen(m_szServerHost)==0))
	   {
		   tmpErr=WSAGetLastError();
		   //try find by ip
		   tmpIAddr=inet_addr(m_szServerHostIP);
		   if ((tmpIAddr==INADDR_NONE) || (strlen(m_szServerHostIP)==0))
		   {
	           closesocket(sckHTTPSocket);
	           ThrowError(ERR_BADHOST
			             ,tmpErr
		                 ,"Error when resolve host.");
               return false;
		   }
		   else
			//found by ip
			sinHTTPServer.sin_addr.S_un.S_addr=tmpIAddr;

	   }
	   else
         //found by name
       sinHTTPServer.sin_addr.S_un.S_addr=*(
		                              (long *)(heServHostInfo->h_addr));

       sinHTTPServer.sin_family=AF_INET;
	   sinHTTPServer.sin_port=htons(m_nServerPort);

	   //allow status check when connect performed
	   struct fd_set fdSet;
       struct timeval tmvTimeout={0L,0L};
  
       FD_ZERO(&fdSet);
       FD_SET(sckHTTPSocket, &fdSet); 
  
       if (select(0,&fdSet,NULL,NULL,&tmvTimeout)==SOCKET_ERROR)
	   {
	      closesocket(sckHTTPSocket);
          ThrowError(ERR_WSAINTERNAL
		             ,WSAGetLastError()
		             ,"Error when select.");
          return false;
	   };

       //try to connect
       if (connect(sckHTTPSocket
		           ,(const struct sockaddr FAR *)&sinHTTPServer
				   ,sizeof(sinHTTPServer))==SOCKET_ERROR)
	   {
	      closesocket(sckHTTPSocket);
          ThrowError(ERR_WSAINTERNAL
		             ,WSAGetLastError()
		             ,"Error when connect.");
          return false;
	   }

       
       //send page request
//***********************************************************************
  szsent(sckHTTPSocket,"HEAD ");

#ifdef DEBON
  printf("HEAD ");
#endif
  
  szsent(sckHTTPSocket,m_szURL);

#ifdef DEBON
  printf("%s",m_szURL);
#endif

  szsent(sckHTTPSocket," HTTP/1.0\r\n");

#ifdef DEBON
  printf(" HTTP/1.0\r\n");
#endif
  
  szsent(sckHTTPSocket,"Accept: image/gif, image/x-bitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, application/x-comet, */*\r\n");

#ifdef DEBON
  printf("Accept: image/gif, image/x-bitmap, image/jpeg, image/pjpeg, application/vnd.ms-powerpoint, application/vnd.ms-excel, application/msword, application/x-comet, */*\r\n");
#endif

  szsent(sckHTTPSocket,"Accept-Language: en\r\n");
  
#ifdef DEBON
  printf("Accept-Language: en\r\n");
#endif

  szsent(sckHTTPSocket,"Accept-Encoding: gzip, deflate\r\n");

#ifdef DEBON
  printf("Accept-Encoding: gzip, deflate\r\n");
#endif

  szsent(sckHTTPSocket,"Cache-Control: no-cache\r\n");
  
#ifdef DEBON
  printf("Cache-Control: no-cache\r\n");
#endif

  szsent(sckHTTPSocket,"Proxy-Connection: Keep-Alive\r\n");
  
#ifdef DEBON
  printf("Proxy-Connection: Keep-Alive\r\n");
#endif

  szsent(sckHTTPSocket,"User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)\r\n");
  
#ifdef DEBON
  printf("User-Agent: Mozilla/4.0 (compatible; MSIE 5.0; Windows NT; DigExt; DTS Agent;)\r\n");
#endif

  szsent(sckHTTPSocket,"Host: ");
  szsent(sckHTTPSocket,pHost);
  szsent(sckHTTPSocket,"\r\n");

#ifdef DEBON
  printf("Host: %s\r\n",pHost);
#endif

  szsent(sckHTTPSocket,"\r\n");

#ifdef DEBON
  printf("\r\n");
#endif
//***********************************************************************


	   //recieve
	   char tmpBuffer[MAXBLOCKSIZE];
	   char FAR* defMem;
	   char FAR* prevMem;
	   defMem=prevMem=NULL;
	   ULONG totalSize=0;
	   ULONG estimatedSize=0,tmpVar=0;
	   int  curSize=1;
try
{
	   while((curSize!=SOCKET_ERROR) && (curSize!=0))
	   {
	      curSize=recv (sckHTTPSocket
                        ,(char FAR *)tmpBuffer
                        ,(MAXBLOCKSIZE-1)
				        ,0);
	   
		  estimatedSize=totalSize+curSize;
	   
		  defMem=(char FAR*)VirtualAlloc(NULL,estimatedSize,MEM_COMMIT,PAGE_READWRITE);
	   

          estimatedSize=0;
		  if (prevMem!=NULL)
		  {
			   while(estimatedSize<totalSize)
			   {
                  defMem[estimatedSize]=prevMem[estimatedSize];
				  estimatedSize++;
			   }
			   tmpVar=totalSize;
			   VirtualFree(prevMem,tmpVar,MEM_DECOMMIT);
		  }
		
		  while(estimatedSize<(totalSize+curSize))
		  {
			   defMem[estimatedSize]=tmpBuffer[estimatedSize-totalSize];
			   estimatedSize++;
		  }

		  totalSize+=curSize;
		  prevMem=defMem;

		
		  if (defMem==NULL)
	   	  {
			  throw;
	   	  }

	  };

}
catch(...)
{
		  ThrowError(ERR_WSAINTERNAL
		             ,GetLastError()
		             ,"Error when memory alloc.");
}


	   m_ulResponseSize=totalSize;
	   m_szResponse=(LPVOID)defMem;

	   prevMem=NULL;
	   defMem=NULL;


	   closesocket(sckHTTPSocket);
	   if (curSize!=0)
	   {
		   ThrowError(ERR_WSAINTERNAL
	                  ,WSAGetLastError()
	                  ,"Error when recieve.");
	   }
	   else
	   {
		   OnResponse();

		   return IsEffective();
	   }
	   return false;
   };
   