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
#include "Common/DebugTrace.h"
//#include <TraceUtil.h>
//using namespace QNA;

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

#define TIMEOUT 30000        //��ʱʱ��

	class CSocket
	{
	public:
		CSocket(bool bIsClient = true, BYTE minorVer = 2, BYTE majorVer = 2)
		{
			int iNetTimeout = TIMEOUT;  //��ʱʱ��3��
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
			{//����˴����׽���
				m_sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if(m_sock == INVALID_SOCKET)
				{
					printf("Failed socket() \n");
					exit(0);
				}
				//���ý��ճ�ʱ
				if (::setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&iNetTimeout, sizeof(int)) == SOCKET_ERROR)
					exit(0);
				//���÷��ͳ�ʱ
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
		//���ӷ�����  ���ǿͻ����õĺ���  dwIPValueΪIPֵ uiPort�˿�
		//1 �ɹ� -1 ���ó��� -2 Socket ����ʧ�� -3 ���÷��ͳ�ʱʧ�� -4 ���ý��ճ�ʱʧ�� -5 ���ӳ�ʱ -6 ���ӳ���
		int Connect(DWORD dwIPValue, UINT uiPort)
		{
			FD_SET	    FS_ReadSocket;
			int         iLen = sizeof(int);
			DWORD       dwError = 0;
			DWORD       dwFlag = 1;          //dwFlag 1 ���������ģʽ Ϊ0��ֹ������ģʽ
			TIMEVAL	    timeout;             //��ʱ΢��
			SOCKADDR_IN	addr = {0};          //TCP  ���ݽṹ
			int iNetTimeout = TIMEOUT;       //��ʱʱ��5��
			if (!m_bIsClient)
			{
				TRACE(_T("�ͻ����˺����������޷����ã�Bind!"));
				return -1;
			}

			//�����׽��� AF_INET��ַ��    SOCK_STREAM(TCP)ʹ�õ�Э��  0�ض��ļ���Э����ص�Э��
			m_sock = socket(AF_INET,SOCK_STREAM, 0);
			if (m_sock == INVALID_SOCKET ) 
			{
				TRACE(_T("Socket ����ʧ�ܣ�Exit!"));
				return -2;
			}
			//�����׽ӿ�   dwFlag 1 ���������ģʽ Ϊ0��ֹ������ģʽ
			::ioctlsocket(m_sock, FIONBIO, &dwFlag);

			//���÷��� ���ճ�ʱ
			if (::setsockopt(m_sock,SOL_SOCKET,SO_SNDTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
				return -3;
			if (::setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
				return -4;


			addr.sin_family	= AF_INET;                               //ָ��Э���壬��socket�����ֻ����AF_INET 			
			addr.sin_addr.s_addr = dwIPValue;                //�洢IP��ַ
			addr.sin_port =	htons(uiPort);                           //�˿ںű���Ϳͻ������Ķ˿ں�һ��

			//����socket����
			if (SOCKET_ERROR == connect(m_sock, (sockaddr*)&addr, sizeof(addr)))
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					timeout.tv_sec = 15;        //���ӳ�ʱ5��
					timeout.tv_usec = 0;
					FD_ZERO(&FS_ReadSocket);            //��ʼ���׽���
					FD_SET(m_sock, &FS_ReadSocket);     //����stdout
					//���Բ���   �ȴ��ɶ����׽��ֽӿ�    �ȴ���д���׽��ֽӿ�   �ȴ���������׽ӿ�   ���ȴ�ʱ��
					dwFlag = select(0, NULL, &FS_ReadSocket, NULL, &timeout);  //ȷ��һ�������׽ӿڵ�״̬

					if(dwFlag > 0) 
					{ 
						getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&dwError, &iLen); 
						if(dwError != 0) 
							return -6;
					} 
					if(!dwFlag) return -5;                   //���ӳ�ʱ
					if (dwFlag == SOCKET_ERROR) return -6;   //���ӳ��� 
				}
				else
				{
					return -6;  //�����쳣
				}
			}

			dwFlag = 0;
			::ioctlsocket(m_sock, FIONBIO, &dwFlag); 
			return 1;
		}

		//���ӷ�����  ���ǿͻ����õĺ���  IP��ַΪ�ַ���
		int Connect(LPCTSTR lpszIP, UINT uiPort)
		{
			return Connect(inet_addr(lpszIP), uiPort);
		}

		//�������� 1�ɹ��� -1����ָ��Ϊ�գ� -2 ���ȴ��� -3 SOCKET_ERROR -4 WSA_IO_PENDING -5 iSent
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

		//��������   ����-1���ݳ��ȴ��� -2���ջ�����Ϊ�� -3 ���ճ��� 0�����������
		//�����ܱ��ֳ�����ʱ������ͶϿ� ���ַ�ʽ���
		// ���Ǵ��ڳ�����״̬��  ����ʱ��δ��ע�������յ����ݳ��� ���Ƶ��л��г�ʱ����
		// ���������������� ����ʱ֪�����ݳ��ȣ�������ɾ��˳���
		int Recv(SOCKET sock, PBYTE pbRecvBuf, UINT uiBufLen)
		{
			int     iRet = 0;
			int		iRecvedLen = 0;
			int     iRes = 0;

			if (uiBufLen >= 0xffff||uiBufLen<=0) return -1;

			if (!pbRecvBuf)	return -2;

			//ʹ��while��Ŀ����Ϊ�˷ֶν��� �������һ�ν��ղ���������ν���
			while(true)
			{
				//���ض�����ֽ����������������ֹ������0������Ļ�������SOCKET_ERROR����
				iRet = recv(sock, (TCHAR*)pbRecvBuf + iRecvedLen, uiBufLen-iRecvedLen, 0);

				if(SOCKET_ERROR == iRet)
				{
					iRes = WSAGetLastError();
					TracePrint(LOG_INFO, "recv-WSAGetLastErrorֵ:%d;\r\n", iRes);
					if ((iRes == 0) && (iRes == WSA_IO_PENDING))
						continue;
					return -3;
				}	

				iRecvedLen += iRet;                     //�Ѿ����յ����ݳ���
				if (iRecvedLen == uiBufLen)	return 0;  //�ڴ�����������������ٴε���
				if(0 == iRet) return iRecvedLen;	   //������ֹ				
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