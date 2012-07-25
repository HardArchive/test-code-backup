#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
#include <DebugTrace.h>


namespace QNA
{
	typedef struct _DATA_PACKET
	{
		BYTE	  byType;			//包类型；
		UINT	  uTotalLen;		//本次传输内容的总长度;
		UINT	  uCKMIndex;		//byBuffer首字节在uTotalLen中的偏移量;
		WORD	  wLength;		    //byBuffer的有效长度；
		//__int64   i64Cipher;		//64位校验码/密钥
		BYTE      byBuffer[1024];	//内容Buffer区;
	}DPACKET, *PDPACKET;

	class CSocket
	{
	public:
		CSocket(bool bIsClient = true, BYTE minorVer = 2, BYTE majorVer = 2)
		{
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
			{
				m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(m_sock == INVALID_SOCKET)
				{
					printf("Failed socket() \n");
				}
			}
			m_bIsClient = bIsClient;
		}

		~CSocket()
		{
			Close();
			m_sock = NULL;
		}

	public:
		//连接服务器  这是客户端用的函数
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
			if (!m_bIsClient)
			{
				TRACE(_T("客户器端函数服务器无法调用，Bind!"));
				return false;
			}
			//创建套接字
			m_sock = socket(AF_INET,SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket 创建失败，Exit!"));
				return -1;
			}

			//定义地址
			sockaddr_in myaddr; 
			memset(&myaddr, 0, sizeof(myaddr));
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.s_addr = inet_addr(lpszIP);   //这里有个问题 如果是宽字符的inet_addr不支持
			//端口号必须和客户发往的端口号一致
			myaddr.sin_port = htons(uiPort);

			//connect
			if (connect(m_sock, (sockaddr*)&myaddr, sizeof(myaddr)) == SOCKET_ERROR)
			{
				TRACE(_T("connect failed, exit!"));
				closesocket(m_sock);
				WSACleanup();
				return -2;
			}
			else
			{
				TRACE(_T("connect succeed!"));
			}

			return 1;
		}

		//发送数据
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

		//接收数据
		int Recv(SOCKET sock, PBYTE pbRecvBuf, UINT uiBufLen)
		{
			int		iRecved = 0;
			int     iRes = 0;
			DWORD	dwSentLen = 0;
			DWORD   Flag = 0;

			if (uiBufLen >= 0xffff||uiBufLen<=0)
				return -1;

			if (!pbRecvBuf)
				return -2;

			while(true)
			{
				iRecved = recv(sock, (TCHAR*)pbRecvBuf + iRecved, uiBufLen, 0);
				iRes=WSAGetLastError();

				if(SOCKET_ERROR == iRecved)
					return -3;
				
				if((iRes != 0) && (iRes != WSA_IO_PENDING) && (iRecved == 0))
					return -4;

				if(iRecved == 0)
					return -5;
				
				if(iRecved == uiBufLen)
					return 1;

				if( iRecved < (int)uiBufLen)
				{		
					TRACE("分段接收...");
					uiBufLen-=iRecved;
				}
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