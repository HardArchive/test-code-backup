#ifndef _IPLOOKUP_SOCKET_H_
#define _IPLOOKUP_SOCKET_H_

#include "comm_protocol.h"
#include "Socket.h"

namespace QNA
{
	class CIpLookupSocket
	{
	public:
		CIpLookupSocket()
		{
			ZeroMemory(m_byDISendBuffer, 1024);
			ZeroMemory(m_byNDISendBuffer, 1024);
		}
		~CIpLookupSocket()
		{
			ZeroMemory(m_byDISendBuffer, 1024);
			ZeroMemory(m_byNDISendBuffer, 1024);
		}

	public:
		//�ӷַ����ķ�������ȡIP������
		int DownLoadIpSection(LPCTSTR lpszIP, UINT uiPort, IPSection& stuIPSection)
		{
			Head stuHead = {0};
			//IPSection stuIPSection = {0};    //IP������

			BYTE byBuffer[1024] = {0};
			QNA::CSocket objSocket;

			objSocket.Connect(lpszIP, uiPort);  //����

			//������������
			stuHead.dwFlag = 0x11223344;
			stuHead.dwCommand = 1;
			memcpy(byBuffer, &stuHead, sizeof(stuHead));
			if (1 != objSocket.Send(objSocket.GetSocket(), byBuffer, 1024))
			{
				TRACE("������������ʧ�ܡ���\r\n");
				return -1;
			}

			ZeroMemory(&stuHead, sizeof(stuHead));
			ZeroMemory(byBuffer, 1024);
			//����IP������
			if (1 != objSocket.Recv(objSocket.GetSocket(), byBuffer, 1024))
			{
				TRACE("��������ʧ�ܡ���\r\n");
				return -2;
			}
			memcpy(&stuHead, byBuffer, sizeof(stuHead));
			if (0x11223344 != stuHead.dwFlag)
			{
				TRACE("�����������ݳ�����\r\n");
				return -3
			}

			if (2 != stuHead.dwCommand)
			{
				TRACE("�����������ݳ���������IP������\r\n");
				return -4;
			}
			memcpy(&stuIPSection, byBuffer+sizeof(Head), sizeof(stuIPSection));

			return 1;
		}

		//����������Ϣ��bIsEnd ��ǰ������Ϣ���Ƿ����
		bool SendDomainInfo(DomainInfo stuDomainInfo, bool bIsEnd)
		{

			return true;
		}


		//����������Ϣ��bIsEnd ��ǰ������Ϣ���Ƿ����
		bool SendNoDomainInfo(NoDomainInfo stuNoDomainInfo, bool bIsEnd)
		{

			return true;
		}

	private:
		//�����ǰ��������С��1024���ֽ����ܷ�׷��һ����Ϣ��
		//����false��������buffer����׷�ӣ� ture��ȴ�����׷��
		bool AppendData(DomainInfo stuDomainInfo, bool bIsEnd)
		{
			if (m_iDISBLen+sizeof(DomainInfo)>1024)
			{
				return false;
			}
			memcpy(m_byDISendBuffer+m_iDISBLen, &stuDomainInfo, sizeof(DomainInfo));
			m_iDISBLen += sizeof(DomainInfo);

			if (bIsEnd)
			{
				return false;
			}
			
		}

		bool AppendData(NoDomainInfo stuNoDomainInfo, bool bIsEnd)
		{

		}

		bool SendBuffer(int iConnectFlag)
		{
			if (1 == iConnectFlag)
			{

			}

		}
	
	private:
		//QNA::CSocket m_clsSocket;
		int m_iDISBLen;               //��ǰbuffer��DISB������
		int m_iDISBFlag;              //��������״̬   1Ϊû�����ݣ� 2Ϊ����δ���� 3Ϊ���Է���
		BYTE m_byDISendBuffer[1024];  //���͹�����̨��������

		int m_iNDISBLen;                //��ǰ�����γ���
		int m_iNDISBFlag;               //����״̬
		BYTE m_byNDISendBuffer[1024];   //���͹�����̨����������
	}
}
#endif  //_IPLOOKUP_SOCKET_H_