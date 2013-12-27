#ifndef _SOCKET_H_
#define _SOCKET_H_

/******************************************************************************* 
* 1、 Class      ： Socket
* 2、 Version    ： 1.00
* 3、 Description： 基于TCP的收发套接字封装类(兼用于服务器和客户端)
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-8-2 17:14:30
* 6、 History    ： 
* 7、 Remark     ： 套接字在构函数初始化，折构函数释放  适用于需要长期连接
********************************************************************************/ 

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
#include "Common/DebugTrace.h"
//#include <TraceUtil.h>
//using namespace QNA;

namespace QNA
{
	//typedef struct _DATA_PACKET
	//{
	//	BYTE	  byType;			//包类型；
	//	UINT	  uTotalLen;		//本次传输内容的总长度;
	//	UINT	  uCKMIndex;		//byBuffer首字节在uTotalLen中的偏移量;
	//	WORD	  wLength;		    //byBuffer的有效长度；
	//	//__int64   i64Cipher;		//64位校验码/密钥
	//	BYTE      byBuffer[1024];	//内容Buffer区;
	//}DPACKET, *PDPACKET;

#define TIMEOUT 30000        //超时时间

	class CSocket
	{
	public:
		CSocket(bool bIsClient = true, BYTE minorVer = 2, BYTE majorVer = 2)
		{
			int iNetTimeout = TIMEOUT;  //超时时间3秒
			// 初始化WS2_32.dll
			WSADATA wsaData;
			WORD sockVersion = MAKEWORD(minorVer, majorVer);
			if(::WSAStartup(sockVersion, &wsaData) != 0)
			{
				TRACE(_T("Socket2.0初始化失败，Exit!"));
				exit(0);
			}
			m_sock = NULL;
			if (!bIsClient)
			{//服务端创建套接字
				m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(m_sock == INVALID_SOCKET)
				{
					printf("Failed socket() \n");
					exit(0);
				}
				//设置接收超时
				if (::setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&iNetTimeout, sizeof(int)) == SOCKET_ERROR)
					exit(0);
				//设置发送超时
				if (::setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&iNetTimeout, sizeof(int)) == SOCKET_ERROR)
					exit(0);	
			}
			m_bIsClient = bIsClient;
		}

		~CSocket()
		{
			Close();
			m_sock = NULL;
		}

	public:
		//连接服务器  这是客户端用的函数  dwIPValue为IP值 uiPort端口
		//1 成功 -1 调用出错 -2 Socket 创建失败 -3 设置发送超时失败 -4 设置接收超时失败 -5 连接超时 -6 连接出错
		int Connect(DWORD dwIPValue, UINT uiPort)
		{
			FD_SET	    FS_ReadSocket;
			int         iLen = sizeof(int);
			DWORD       dwError = 0;
			DWORD       dwFlag = 1;          //dwFlag 1 允许非阻塞模式 为0禁止非阻塞模式
			TIMEVAL	    timeout;             //定时微妙
			SOCKADDR_IN	addr = {0};          //TCP  数据结构
			int iNetTimeout = TIMEOUT;       //超时时间5秒
			if (!m_bIsClient)
			{
				TRACE(_T("客户器端函数服务器无法调用，Bind!"));
				return -1;
			}

			//创建套接字 AF_INET地址族    SOCK_STREAM(TCP)使用的协议  0特定的家族协议相关的协议
			m_sock = socket(AF_INET,SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket 创建失败，Exit!"));
				return -2;
			}
			//控制套接口   dwFlag 1 允许非阻塞模式 为0禁止非阻塞模式
			::ioctlsocket(m_sock, FIONBIO, &dwFlag);

			//设置发送 接收超时
			if (::setsockopt(m_sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
				return -3;
			if (::setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
				return -4;


			addr.sin_family	= AF_INET;                               //指代协议族，在socket编程中只能是AF_INET 			
			addr.sin_addr.s_addr = dwIPValue;                //存储IP地址
			addr.sin_port =	htons(uiPort);                           //端口号必须和客户发往的端口号一致

			//建立socket连接
			if (SOCKET_ERROR == connect(m_sock, (sockaddr*)&addr, sizeof(addr)))
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					timeout.tv_sec = 15;        //连接超时5秒
					timeout.tv_usec = 0;
					FD_ZERO(&FS_ReadSocket);            //初始化套接字
					FD_SET(m_sock, &FS_ReadSocket);     //设置stdout
					//忽略参数   等待可读的套接字接口    等待可写的套接字接口   等待错误检查的套接口   最多等待时间
					dwFlag = select(0, NULL, &FS_ReadSocket, NULL, &timeout);  //确定一个或多个套接口的状态

					if(dwFlag > 0) 
					{ 
						getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&dwError, &iLen); 
						if(dwError != 0) 
							return -6;
					} 
					if(!dwFlag) return -5;                   //连接超时
					if (dwFlag == SOCKET_ERROR) return -6;   //连接出错 
				}
				else
				{
					return -6;  //连接异常
				}
			}

			dwFlag = 0;
			::ioctlsocket(m_sock, FIONBIO, &dwFlag); 
			return 1;
		}

		//连接服务器  这是客户端用的函数  IP地址为字符串
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
			return Connect(inet_addr(lpszIP), uiPort);
		}

		//发送数据 1成功， -1发送指针为空， -2 长度错误 -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent
		int Send(SOCKET sock, PBYTE pbRecvBuf, UINT uiBufLen)
		{
			int	  iSent = 0;       //每次发送包长度
			int   iRes = 0;        //WSAGetLastError的值
			DWORD dwSentLen = 0;
			if (!pbRecvBuf)
				return -1;

			if (uiBufLen >= 0xffff||uiBufLen <= 0)
				return -2;

			while(true)
			{
				iSent = send(sock, (TCHAR*)pbRecvBuf + iSent, uiBufLen, 0);

				iRes = WSAGetLastError();

				if(SOCKET_ERROR == iSent)
					return -3;

				if( (iRes != WSA_IO_PENDING) && (iSent == 0 ) )
					return -4;

				if(iSent == 0)
					return -5;

				if(iSent == uiBufLen)
					return 1;//发送完成
				
				if( iSent < (int)uiBufLen)
					uiBufLen -= iSent;
			}
		}

		//接收数据   返回-1数据长度错误 -2接收缓冲区为空 -3 接收出错 0接收数据完成
		//当不能保持长连接时接收完就断开 这种方式最好
		// 但是处于长连接状态下  接收时又未能注明所接收的数据长度 估计得有会有超时处理
		// 所以最理想的情况是 接收时知道数据长度，接收完成就退出了
		int Recv(SOCKET sock, PBYTE pbRecvBuf, UINT uiBufLen)
		{
			int     iRet = 0;
			int		iRecvedLen = 0;
			int     iRes = 0;

			if (uiBufLen >= 0xffff||uiBufLen<=0) return -1;

			if (!pbRecvBuf)	return -2;

			//使用while的目的是为了分段接收 如果数据一次接收不完则分两次接收
			while(true)
			{
				//返回读入的字节数。如果连接已中止，返回0。否则的话，返回SOCKET_ERROR错误
				iRet = recv(sock, (TCHAR*)pbRecvBuf + iRecvedLen, uiBufLen-iRecvedLen, 0);

				if(SOCKET_ERROR == iRet)
				{
					iRes = WSAGetLastError();
					TracePrint(LOG_INFO, "recv-WSAGetLastError值:%d;\r\n", iRes);
					if ((iRes == 0) && (iRes == WSA_IO_PENDING))
						continue;
					return -3;
				}	

				iRecvedLen += iRet;                     //已经接收的数据长度
				if (iRecvedLen == uiBufLen)	return 0;  //内存溢出缓冲区已满需再次调用
				if(0 == iRet) return iRecvedLen;	   //连接终止				
			}
		}

		//绑定本机端口
		bool Bind(UINT uiPort)
		{
			// 填充sockaddr_in结构
			sockaddr_in sin;
			if (m_bIsClient)
			{
				TRACE(_T("服务器端函数客户端无法调用，Bind!"));
				return false;
			}

			sin.sin_family = AF_INET;
			sin.sin_port = htons(uiPort);
			sin.sin_addr.S_un.S_addr = INADDR_ANY;

			// 绑定这个套节字到一个本地地址
			if(::bind(m_sock, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
			{
				printf("Failed bind()WSAGetLastError: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}
		
		//进入监听模式
		//backlog 连接请求队列(queue of pending connections)的最大长度（一般由2到4）。用SOMAXCONN则由系统确定
		bool Listen(int iBackLog)
		{
			if (m_bIsClient)
			{
				TRACE(_T("服务器端函数客户端无法调用，Listen!"));
				return false;
			}
			if (0 == iBackLog)
				return false;

			// 进入监听模式
			if(::listen(m_sock, iBackLog) == SOCKET_ERROR)
			{
				printf("Failed listen()WSAGetLastError: %d\n", WSAGetLastError());
				return false;
			}
			
			return true;
		}
		
		SOCKET Accept()
		{
			SOCKET skClient ;
			sockaddr_in remoteAddr; 
			int nAddrLen = sizeof(remoteAddr);
			if (m_bIsClient)
			{
				TRACE(_T("服务器端函数客户端无法调用，Accept!"));
				return false;
			}
			// 接受一个新连接
			skClient = ::accept(m_sock, (SOCKADDR*)&remoteAddr, &nAddrLen);
			//printf(" 接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			return skClient;
		}
		//获取本机所创建的SOCKET
		SOCKET GetSocket()
		{
			return m_sock;
		}

		//关闭连接
		void Close()
		{
			closesocket(m_sock);
			WSACleanup();
		}

	private:
		SOCKET m_sock;
		bool m_bIsClient;        //客户端标志，true为客户端，false为服务端
	};
}
#endif  //_SOCKET_H_