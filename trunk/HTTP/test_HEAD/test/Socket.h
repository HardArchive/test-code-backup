#ifndef _SOCKET_H_
#define _SOCKET_H_

/******************************************************************************* 
* 1�� Class      �� Socket
* 2�� Version    �� 1.00
* 3�� Description�� ����TCP���շ��׽��ַ�װ��(�����ڷ������Ϳͻ���)
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-8-2 17:14:30
* 6�� History    �� 
* 7�� Remark     �� �׽����ڹ�������ʼ�����۹������ͷ�  ��������Ҫ��������
********************************************************************************/ 

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
#include <DebugTrace.h>


namespace QNA
{
	//typedef struct _DATA_PACKET
	//{
	//	BYTE	  byType;			//�����ͣ�
	//	UINT	  uTotalLen;		//���δ������ݵ��ܳ���;
	//	UINT	  uCKMIndex;		//byBuffer���ֽ���uTotalLen�е�ƫ����;
	//	WORD	  wLength;		    //byBuffer����Ч���ȣ�
	//	//__int64   i64Cipher;		//64λУ����/��Կ
	//	BYTE      byBuffer[1024];	//����Buffer��;
	//}DPACKET, *PDPACKET;

	class CSocket
	{
	public:
		CSocket(bool bIsClient = true, BYTE minorVer = 2, BYTE majorVer = 2)
		{
			int iNetTimeout = 5000;  //5��
			// ��ʼ��WS2_32.dll
			WSADATA wsaData;
			WORD sockVersion = MAKEWORD(minorVer, majorVer);
			if(::WSAStartup(sockVersion, &wsaData) != 0)
			{
				TRACE(_T("Socket2.0��ʼ��ʧ�ܣ�Exit!"));
				exit(0);
			}
			m_sock = NULL;
			if (!bIsClient)
			{
				m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(m_sock == INVALID_SOCKET)
				{
					printf("Failed socket() \n");
					exit(0);
				}
				//���ý��ճ�ʱ
				setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iNetTimeout, sizeof(int));
				//���÷��ͳ�ʱ
				setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iNetTimeout, sizeof(int));
			}
			m_bIsClient = bIsClient;
		}

		~CSocket()
		{
			Close();
			m_sock = NULL;
		}

	public:
		//�˿ڼ��
		bool CheckPort(DWORD dwIPValue, UINT uiPort)    //�߳�����
		{
			FD_SET		mask;
			u_long		value;
			SOCKET		sockfd;
			TIMEVAL		timeout;   //��ʱ΢��
			SOCKADDR_IN	addr;      //TCP  ���ݽṹ
			sockfd = socket(AF_INET, SOCK_STREAM, 0);  //AF_INET��ַ��    SOCK_STREAM(TCP)ʹ�õ�Э��  0�ض��ļ���Э����ص�Э��
			value=1;
			ioctlsocket(sockfd, FIONBIO, &value); //�����׽ӿ�   value 1 ���������ģʽ Ϊ0��ֹ������ģʽ
			addr.sin_family	= AF_INET;                               //ָ��Э���壬��socket�����ֻ����AF_INET 
			addr.sin_port =	htons(uiPort);                           //�˿ں�
			addr.sin_addr.s_addr = dwIPValue;                        //�洢IP��ַ
			connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));  //����socket����

			timeout.tv_sec = 2;        //��ʱ5΢��
			timeout.tv_usec = 0;
			FD_ZERO(&mask);           //��ʼ���׽���
			FD_SET(sockfd, &mask);    //����stdout
			value = select(sockfd+1, NULL, &mask, NULL, &timeout);  //ȷ��һ�������׽ӿڵ�״̬

			//���Բ���   �ȴ��ɶ����׽��ֽӿ�    �ȴ���д���׽��ֽӿ�   �ȴ���������׽ӿ�   ���ȴ�ʱ��
			closesocket(sockfd);  //�ر�һ���׽ӿ�
			if(value &&	value != SOCKET_ERROR)  //SOCKET_ERROR  ����ʧ��
			{  //���ӳɹ�
				return true;
			}
			return false;
		}
		//���ӷ�����  ���ǿͻ����õĺ���
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
			int iNetTimeout = 5000;  //5��
			if (!m_bIsClient)
			{
				TRACE(_T("�ͻ����˺����������޷����ã�Bind!"));
				return -1;
			}
			//�����׽���
			m_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket ����ʧ�ܣ�Exit!"));
				return -2;
			}

			//���ý��ճ�ʱ
			setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iNetTimeout, sizeof(int));
			//���÷��ͳ�ʱ
			setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iNetTimeout, sizeof(int));

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
				return -3;
			}
			else
			{
				TRACE(_T("connect succeed!"));
			}
			return 1;
		}

		//���ӷ�����  ���ǿͻ����õĺ���
		int Connect(DWORD dwIPValue, UINT uiPort)
		{
			int iNetTimeout = 5000;  //5��
//struct timeval
			if (!m_bIsClient)
			{
				TRACE(_T("�ͻ����˺����������޷����ã�Bind!"));
				return -1;
			}
			//�����׽���
			m_sock = socket(AF_INET,SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket ����ʧ�ܣ�Exit!"));
				return -2;
			}

			//���ý��ճ�ʱ
			setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&iNetTimeout, sizeof(int));
			//���÷��ͳ�ʱ
			setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&iNetTimeout, sizeof(int));

			//�����ַ
			sockaddr_in myaddr; 
			memset(&myaddr, 0, sizeof(myaddr));
			myaddr.sin_family = AF_INET;
			myaddr.sin_addr.s_addr = dwIPValue;   //�����и����� ����ǿ��ַ���inet_addr��֧��
			//�˿ںű���Ϳͻ������Ķ˿ں�һ��
			myaddr.sin_port = htons(uiPort);

			//connect
			if (connect(m_sock, (sockaddr*)&myaddr, sizeof(myaddr)) == SOCKET_ERROR)
			{
				TRACE(_T("connect failed, exit!"));
				closesocket(m_sock);
				WSACleanup();
				return -3;
			}
			else
			{
				TRACE(_T("connect succeed!"));
			}

			return 1;
		}

		//��������
		int Send(SOCKET sock, PBYTE pbRecvBuf, UINT uiBufLen)
		{
			int	  iSent = 0;       //ÿ�η��Ͱ�����
			int   iRes = 0;        //WSAGetLastError��ֵ
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
					return 1;//�������
				
				if( iSent < (int)uiBufLen)
					uiBufLen -= iSent;
			}
		}

		//��������
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
					TRACE("�ֶν���...");
					uiBufLen-=iRecved;
				}
			}
		}

		//�󶨱����˿�
		bool Bind(UINT uiPort)
		{
			// ���sockaddr_in�ṹ
			sockaddr_in sin;
			if (m_bIsClient)
			{
				TRACE(_T("�������˺����ͻ����޷����ã�Bind!"));
				return false;
			}

			sin.sin_family = AF_INET;
			sin.sin_port = htons(uiPort);
			sin.sin_addr.S_un.S_addr = INADDR_ANY;

			// ������׽��ֵ�һ�����ص�ַ
			if(::bind(m_sock, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
			{
				printf("Failed bind()WSAGetLastError: %d\n", WSAGetLastError());
				return false;
			}
			return true;
		}
		
		//�������ģʽ
		//backlog �����������(queue of pending connections)����󳤶ȣ�һ����2��4������SOMAXCONN����ϵͳȷ��
		bool Listen(int iBackLog)
		{
			if (m_bIsClient)
			{
				TRACE(_T("�������˺����ͻ����޷����ã�Listen!"));
				return false;
			}
			if (0 == iBackLog)
				return false;

			// �������ģʽ
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
				TRACE(_T("�������˺����ͻ����޷����ã�Accept!"));
				return false;
			}
			// ����һ��������
			skClient = ::accept(m_sock, (SOCKADDR*)&remoteAddr, &nAddrLen);
			//printf(" ���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			return skClient;
		}
		//��ȡ������������SOCKET
		SOCKET GetSocket()
		{
			return m_sock;
		}

		//�ر�����
		void Close()
		{
			closesocket(m_sock);
			WSACleanup();
		}

	private:
		SOCKET m_sock;
		bool m_bIsClient;        //�ͻ��˱�־��trueΪ�ͻ��ˣ�falseΪ�����
	};
}
#endif  //_SOCKET_H_