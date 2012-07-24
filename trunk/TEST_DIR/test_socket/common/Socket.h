#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
#include <DebugTrace.h>
namespace QNA
{

	class CSocket
	{
		CSocket(BYTE minorVer = 2, BYTE majorVer = 2)
		{
			// ��ʼ��WS2_32.dll
			WSADATA wsaData;
			WORD sockVersion = MAKEWORD(minorVer, majorVer);
			if(::WSAStartup(sockVersion, &wsaData) != 0)
			{
				TRACE(_T("Socket2.0��ʼ��ʧ�ܣ�Exit!"));
				exit(0);
			}
			m_sock = NULL;
		}

		~CSocket()
		{
			Close();
			m_sock = NULL;
		}

		//���ӷ�����
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
			//�����׽���
			m_sock = socket(AF_INET,SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket ����ʧ�ܣ�Exit!"));
				return -1;
			}

			//�����ַ
			sockaddr_in myaddr; 
			memset(&myaddr, 0, sizeof(myaddr));
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.s_addr = inet_addr(lpszIP);   //�����и����� ����ǿ��ַ���inet_addr��֧��
			//�˿ںű���Ϳͻ������Ķ˿ں�һ��
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

		//��������
		bool Send(LPCTSTR lpszSendBuf, size_t szBufLen)
		{
			return true;
		}

		//��������
		bool Recv()
		{
			return true;
		}

		//�ر�����
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