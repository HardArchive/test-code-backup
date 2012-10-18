// TcpScan.cpp: implementation of the CTcpScan class.
//author Asian Chai
// date   2005-5-22
// funtion, Other scanning method such as TCP syn Scan, ACK scan, Null Scan
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleScanner.h"
#include "TcpScan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define RESULT_OK	0
#define RESULT_ERR	1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpScan::CTcpScan()
{
    
	
}

CTcpScan::~CTcpScan()
{

}

// intiate the wsa service 
UINT CTcpScan::InitWsaDll()
{
	WSADATA wsaData;  
    if(WSAStartup(0x0002, &wsaData ))
    {
        AfxMessageBox("Init TCP/IP stack error!");
        return 1;
    }
    if(wsaData.wVersion != 0x0002)
    {
        printf("Winsock version is incorrect!");
        WSACleanup();
        return 1;
    }
	return 0;

}

//The Tcp persuade header used to construct the tcp packege 
typedef struct tsd_hdr //定义TCP伪首部 
{ 
	unsigned long saddr; //源地址 
	unsigned long daddr; //目的地址 
	char mbz; 
	char ptcl; //协议类型 
	unsigned short tcpl; //TCP长度 
}
PSDHEADER;

// initiate the sending socket 
int CTcpScan::InitSendSock()
{
	//ASSERT(m_sockSend == INVALID_SOCKET);

	TCHAR szErr[MAX_PATH];
	DWORD dwErr;
	
	int  timeout = 1000;
	BOOL flag    = TRUE; 
	
	m_sockSend = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (m_sockSend == INVALID_SOCKET)
	{
		goto _RESULT_ERR_LABEL;
	}
	
	// Set timeout
	if (setsockopt(m_sockSend, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
	{
		goto _RESULT_ERR_LABEL;
	}
	
	//设置IP_HDRINCL以自己填充IP首部
	if (setsockopt(m_sockSend,IPPROTO_IP,IP_HDRINCL,(char *)&flag,sizeof(int)) == SOCKET_ERROR)
	{
		goto _RESULT_ERR_LABEL;
	}

	return RESULT_OK;

_RESULT_ERR_LABEL:
	dwErr = WSAGetLastError();
	sprintf(szErr, "Error == %ld ", dwErr);
	AfxMessageBox(szErr);
	
	closesocket(m_sockSend);
	m_sockSend = INVALID_SOCKET;

	return RESULT_ERR;
}

//initiate the receive socket
int CTcpScan::InitRecvSock()
{
	//ASSERT(m_sockRecv == INVALID_SOCKET);
	
	TCHAR szErr[MAX_PATH];
	DWORD dwErr;

	DWORD dwBufferLen[10];
	DWORD dwBufferInLen = 1;
	DWORD dwBytesReturned = 0;
	
	int timeout = 0; //设置同步信息
	
	SOCKADDR_IN sa;
	
	m_sockRecv = WSASocket(AF_INET, SOCK_RAW,IPPROTO_IP,NULL,0,WSA_FLAG_OVERLAPPED);
	if (m_sockRecv == INVALID_SOCKET)
	{
		goto _RESULT_ERR_LABEL;
	}
	
	// Set timeout
	if (setsockopt(m_sockRecv, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
	{
		goto _RESULT_ERR_LABEL;
	}
	
	//get the local address 
	char FAR name[128];	
	gethostname(name, sizeof(name));	
	struct hostent FAR * pHostent;	
	pHostent = gethostbyname(name);

	sa.sin_family		= AF_INET;
	memcpy(&(sa.sin_addr),pHostent->h_addr,pHostent->h_length);
	sa.sin_port			= 0;

	if (bind(m_sockRecv, (const sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		goto _RESULT_ERR_LABEL;
	}


	
	// 设置为混杂模式	
	if ( WSAIoctl(m_sockRecv, SIO_RCVALL, 
					&dwBufferInLen, sizeof(dwBufferInLen),             
					&dwBufferLen, sizeof(dwBufferLen),
					&dwBytesReturned, NULL, NULL) )
	{
		goto _RESULT_ERR_LABEL;
	}


	return RESULT_OK;
	
_RESULT_ERR_LABEL:

	dwErr = WSAGetLastError();
	sprintf(szErr, "Error == %ld ", dwErr);
	AfxMessageBox(szErr);
	
	closesocket(m_sockRecv);
	m_sockSend = INVALID_SOCKET;

	return RESULT_ERR;	
}

//send the icmp packet to the destination 
BOOL CTcpScan::SendIPFrame(TCP *tcp, LPTSTR szData, int dataLen)
{
	if (m_sockSend == INVALID_SOCKET || tcp == NULL)
		return FALSE;
	
	char* buff = new char[MAX_PACKET_SIZE];

	IP ipHead;
	ipHead.ip_verlen	= (4 << 4 | 20 / sizeof(unsigned long)); 
	ipHead.ip_tos		= 0;
	ipHead.ip_len		= htons(20 + 20 + dataLen);
	ipHead.ip_id		= 0;
	ipHead.ip_fragoff	= 0;
	ipHead.ip_ttl		= 128;
	ipHead.ip_proto		= IPPROTO_TCP;
	ipHead.ip_cksum		= 0;
	ipHead.ip_src		= ip_local;
	ipHead.ip_dst		= ip_remote;

	PSDHEADER psdHeader;
	psdHeader.saddr = ip_local;
	psdHeader.daddr	= ip_remote;
	psdHeader.mbz	= 0;
	psdHeader.ptcl	= IPPROTO_TCP;
	psdHeader.tcpl	= htons(TCP_HLEN(tcp) + dataLen);

	// 计算TCP验检和
	memset(buff, 0, MAX_PACKET_SIZE);
	memcpy(buff, &psdHeader, sizeof(psdHeader));
	memcpy(buff + sizeof(psdHeader), tcp, 20);
	memcpy(buff + sizeof(psdHeader) + 20, szData, dataLen);
	tcp->tcp_cksum = checksum((USHORT *)buff, sizeof(psdHeader) + 20 + dataLen);
	
	// 计算IP验检和
	memset(buff, 0, MAX_PACKET_SIZE);
	memcpy(buff, &ipHead, 20);
	memcpy(buff + 20, tcp, 20);
	memset(buff + 20 + 20, 0, 4);
	ipHead.ip_cksum = checksum((USHORT *)buff, 20 + 20); 

	// 填充发送缓冲区
	memset(buff, 0, MAX_PACKET_SIZE);

	memcpy(buff, &ipHead, 20);
	memcpy(buff + 20 , tcp, 20);
	memcpy(buff + 20 + 20, szData, dataLen);
	
	SOCKADDR_IN dest;

	dest.sin_family			= AF_INET;
	dest.sin_addr.s_addr	= ip_remote;
	dest.sin_port			= tcp->tcp_dport;

	unsigned char realData[] = {
			0x45, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
			0x80, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0xf5,
			0xca, 0x65, 0x2c, 0x0a, 0x1f, 0x40, 0x1f, 0x40,
			0x00, 0x0c, 0x20, 0x6f, 0x74, 0x65, 0x73, 0x74
	};


	int ret = sendto(m_sockSend, (const char*)buff, 20 + 20 + dataLen, 0, 
				(const sockaddr*)&dest, sizeof(dest));
	
	if (ret == SOCKET_ERROR)
	{
		DWORD dwErr = ::WSAGetLastError();
	}

	if(ScanDebug)
	{
		AfxMessageBox("数据包发送成功");
	}
	delete buff;


	return ret != SOCKET_ERROR;
}



//analyze the received packge to judge if the port open 
BOOL CTcpScan::CheckPortConnect(CString strHost, int DestPort, int Scanflag)
{
  CString strDebug;
  InitSendSock(); //Initiate the m_sockSen
  ip_remote = inet_addr((const char*)strHost); 
 
  char FAR name[128];	
  gethostname(name, sizeof(name));	
  struct hostent FAR * pHostent;	
  pHostent = gethostbyname(name);

  LPSTR lpAddr = pHostent->h_addr_list[0];
  struct in_addr  inAddr;
  memmove (&inAddr, lpAddr, 4);
	
  CString str = inet_ntoa (inAddr);
  ip_local = inet_addr(str);

  if(ScanDebug)
  {
	   strDebug.Format("本地主机地址%s",str);
	   AfxMessageBox(strDebug);
	   strDebug.Format("%-15s",inet_ntoa(*(in_addr*)&ip_local));
	   AfxMessageBox(strDebug);

  }
  
  TCP tcpHead;

	tcpHead.tcp_sport	= htons(8000);
	tcpHead.tcp_dport	= htons(DestPort);
	tcpHead.tcp_seq		= htonl(rand());
    tcpHead.tcp_ack		= htonl(rand()+1);
	tcpHead.tcp_offset	= (0x05 << 4) & 0xF0;
	
	tcpHead.tcp_code	= 0x00;
	//设置不同的扫描模式
	switch(Scanflag)
	{
	case 0: //SynScan
		if(ScanDebug)
		{
			AfxMessageBox("设置为Syn模式扫描");
		}
		tcpHead.tcp_code = TCPF_SYN; 
		break;
	case 1:  //AckScan
		tcpHead.tcp_code = TCPF_ACK;
		break;
	case 2: //NullScan
        tcpHead.tcp_code = NULL;
		break;
	case 3: //TCPF_FIN|TCPF_URG|TCPF_PSH Scan
		tcpHead.tcp_code=TCPF_FIN|TCPF_URG|TCPF_PSH;
		break;
	}
	tcpHead.tcp_window	= 1024;
	tcpHead.tcp_cksum	= 0;
	tcpHead.tcp_urgptr	=NULL;
	
 //send the ip datagram 
	SendIPFrame(&tcpHead, NULL, 0); //The payload is zero 
    InitRecvSock(); //Initiate the m_sockRecv

	char recvBuffer[MAX_PACKET_SIZE];

	while(true)
	{
		if(ScanDebug)
		{
			AfxMessageBox("开始接收数据包");
		}
		//接收数据包
		int ret = recv(m_sockRecv,recvBuffer,sizeof(recvBuffer),0); 
		if (ret == SOCKET_ERROR && ret != WSAETIMEDOUT)
		{  //接收数据包错误
         #ifdef _DEBUG
			TCHAR szError[MAX_PATH];
			DWORD dwErr = ::WSAGetLastError();
			sprintf(szError, "Error in RecvProcess: %d", dwErr);
  			AfxMessageBox(szError);
         #endif
			break;
		}

		if(ret==0) //连接被关闭
		{
           if(ScanDebug)
		   {
			   AfxMessageBox("连接被关闭");
		   }
           continue;
		}
		
		if(ret>0) //正常接收到数据包
		{
          
           if(ScanDebug)
		   {
			   AfxMessageBox("正常接收到数据包");
		   }
		  RAW_PACKET* rawPacket = new RAW_PACKET();

		  rawPacket->size = (unsigned int)ret;

		  rawPacket->buff = new unsigned char[rawPacket->size];
		  memcpy(rawPacket->buff, recvBuffer, rawPacket->size);

		  ::GetSystemTime(&rawPacket->time);

		  IP* ipHead = (IP*)rawPacket->buff; //获得IP包
		  ASSERT(ipHead != NULL);
          if(ScanDebug)
		  {
			  //str.Format(" %-15s",inet_ntoa(*(in_addr*)&dlg->ip.SrcAddr)); 
			  strDebug.Format("接到的数据包的源地址：%-15s",inet_ntoa(*(in_addr*)&ipHead->ip_src));
			  AfxMessageBox(strDebug);
			  strDebug.Format("接到的数据报的目的地址：%-15s",inet_ntoa(*(in_addr*)&ipHead->ip_dst));
			  AfxMessageBox(strDebug);
			  
			  strDebug.Format("目标地址%-15s",inet_ntoa(*(in_addr*)&ip_remote));
		      AfxMessageBox(strDebug);
			  strDebug.Format("本地地址%-15s",inet_ntoa(*(in_addr*)&ip_local));
	          AfxMessageBox(strDebug);

		  }
		 // 必须是指定是从目标机器接收到的数据
		if( !(ipHead->ip_src == ip_remote && ipHead->ip_dst == ip_local) ) 
		{
			delete rawPacket;
			continue;
		}
		
		// 必须是TCP协议
		if (ipHead->ip_proto != IPPROTO_TCP)
		{
			delete rawPacket;	
			continue;
		}

		// 源端口必须是 DestPort
		TCP* RecvTcpHead = (TCP*)ipHead->ip_data;
		ASSERT(RecvTcpHead!= NULL);

		if (RecvTcpHead->tcp_sport != htons(DestPort))
		{
			delete rawPacket;
			continue;
		}

		 //分析标志位，判断端口是否开放
         return AnalyzeFlag(RecvTcpHead,Scanflag);
		 break;

		} //end of "if(ret>0)"		
		
	} //end of while 
	return false;
}

// analyse the returned contorl flag 
BOOL CTcpScan::AnalyzeFlag(TCP *RecvTcpHead, int ScanFlag)
{
	unsigned char TcpCode = RecvTcpHead->tcp_code;
	switch(ScanFlag) {
	case 0:
		if( TcpCode == TCPF_SYN||TCPF_ACK)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 1:
		if(TcpCode == TCPF_RST)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case 2: 
		if(TcpCode==TCPF_RST)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 3:

		if(TcpCode==TCPF_RST)
		{
			return false;
		}
		else
		{
			return true;
		}        
		break;		
	default:
		return false;
		break;
	}
}
