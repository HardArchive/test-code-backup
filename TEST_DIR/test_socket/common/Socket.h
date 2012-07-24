#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
#include <DebugTrace.h>
namespace QNA
{

	class CSocket
	{
		CSocket(BYTE minorVer = 2, BYTE majorVer = 2)
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
		}

		~CSocket()
		{
			Close();
			m_sock = NULL;
		}

		//连接服务器
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
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
		bool Send(LPCTSTR lpszSendBuf, size_t szBufLen)
		{
			return true;
		}

		//接收数据
		bool Recv()
		{
			return true;
		}

		//关闭连接
		void Close()
		{
			closesocket(m_sock);
			WSACleanup();
		}

	private:
		SOCKET m_sock;
	};
}
#endif  //_SOCKET_H_