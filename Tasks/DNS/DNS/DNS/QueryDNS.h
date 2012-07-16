#include "stdafx.h"
#include "struct_define.h"
#include "AnalyticDNS.h"

class CQueryDNS:public CAnalyticDNS
{
public:

	CQueryDNS(void)
	{
		ZeroMemory(m_szDomain, sizeof(m_szDomain));
	}

	~CQueryDNS(void)
	{
		ZeroMemory(m_szDomain, sizeof(m_szDomain));
	}
public:
	void Init()
	{
		WSADATA firstsock;
		ZeroMemory(m_DnsServers, sizeof(m_DnsServers));
		RetrieveDnsServersFromRegistry();
		TRACE("��ʼ���׽���...-QNA\n");
		if (WSAStartup(MAKEWORD(2,2),&firstsock) != 0)
		{
			TRACE("��ʼ���׽��ִ����룺%d-QNA",WSAGetLastError());
			return;
		} 
		TRACE("��ʼ�����.-QNA\n");
	}

	//DNS��֤  ����-IP��֤
	int VerifyDNS(char* pszDomain, char* pszIp)
	{
		int iRet = 0;
		int iPacektLen = 0;
		BYTE szPacket[PACKET_ALL_LEN] = {0};
		TRACE("DNS��֤  ����:%s-IP:%s ��֤-QNA\n", pszDomain, pszIp);
		
		//_asm int 3;

		iRet = QueryDNS((BYTE*)pszDomain, szPacket, &iPacektLen);
		if (1 != iRet)
		{
			return iRet;
		}
		if (!AnalyticQueryDNS(szPacket, iPacektLen))
		{
			TRACE("VerifyDNS������������ʧ��-QNA\n");
			return -1;
		}
		return 1;
	}	
private:

	/******************************************************************************* 
	1�� �������ƣ� QueryDNS
	2�� ���������� ��ѯDNS
	3�� ��������� host�������� Packet �ظ����ݰ����ݣ� iPacketLen �ظ����ݰ�����
	4�� �� �� ֵ�� �ɹ�����1�����󷵻�-1�� ʧ�ܷ���0
	5�� ��̬�ڴ棺 ��
	6�� ������ƣ�  ��ë
	7�� �������ڣ� 2012-4-26 10:17:16
	8�� ��    ע�� 
	********************************************************************************/ 
	int QueryDNS(BYTE *host, BYTE* Packet, int* piPacketLen)
	{
		BYTE buf[PACKET_ALL_LEN] = {0}, *pQuestionName = NULL;
		int iLen = 0;

		SOCKET NDS_Socket = 0;
		struct sockaddr_in dest;  //ָ�룬ָ��Ŀ���׽��ֵĵ�ַ 

		PDNS_HEADER pDnsHeader = NULL;                                //DNS ͷ�ṹ
		PDNS_WIRE_QUESTION pDnsQusetion = NULL;                       //��ѯ����ṹ

		if (!(host && Packet))
		{
			TRACE("QueryDNS-�������������ָ�����-QNA\n");
			return -1;
		}
		strcpy((char*)m_szDomain, (char*)host);

		NDS_Socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //DNS�����ѯ��ΪUDP���͵�

		dest.sin_family = AF_INET;
		dest.sin_port = htons(53);
		dest.sin_addr.s_addr = inet_addr(m_DnsServers[0]);  //DNS������IP

		//����DNS��׼��ѯ����
		pDnsHeader = (PDNS_HEADER)&buf;       //��ѯ�����ڴ�	
		pDnsHeader->Xid = (unsigned short) htons((u_short)GetCurrentProcessId()); //��ǰ����id
		pDnsHeader->IsResponse = 0;      
		pDnsHeader->Opcode = 0; 
		pDnsHeader->Authoritative = 0;     
		pDnsHeader->Truncation = 0;    
		pDnsHeader->RecursionDesired = 1;     
		pDnsHeader->RecursionAvailable = 0;      
		pDnsHeader->Reserved  = 0;
		pDnsHeader->AuthenticatedData = 0;
		pDnsHeader->CheckingDisabled = 0;
		pDnsHeader->ResponseCode = 0;
		pDnsHeader->QuestionCount = htons(1);   
		pDnsHeader->AnswerCount  = 0;
		pDnsHeader->NameServerCount = 0;
		pDnsHeader->AdditionalCount  = 0;

		//ָ��DNSͷ��Ĳ�ѯ�����һ����  ��ѯ��
		pQuestionName =(BYTE *)&buf[sizeof(DNS_HEADER)];      
		ConvertDomainToDNSName(pQuestionName, m_szDomain); 
		pDnsQusetion = (PDNS_WIRE_QUESTION)&buf[sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName) + 1)]; //fill it
		pDnsQusetion->QuestionType = htons(1);  //��ѯ����we are requesting the ipv4 address
		pDnsQusetion->QuestionClass = htons(1); //��ѯ�� its internet (lol)

		int iSendLen = sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION);
		iSendLen = sendto(
			NDS_Socket,
			(char*)buf,
			sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION),
			0,
			(struct sockaddr*)&dest,
			sizeof(dest));
		//���Ͳ�ѯ���ݰ�
		if(iSendLen==SOCKET_ERROR)
		{
			TRACE("���ʹ����룺%d-QNA\n",WSAGetLastError());
			return -2;
		}
		TRACE("�ѷ��Ͳ�ѯ��-QNA\n");

		ZeroMemory(buf, PACKET_ALL_LEN);
		iLen = sizeof(dest);
		TRACE("���ջظ�...-QNA\r\n");
		int iRevcLen = recvfrom (NDS_Socket, (char*)buf, PACKET_ALL_LEN, 0, (struct sockaddr*)&dest, &iLen);
		//_asm int 3;
		if(iRevcLen==SOCKET_ERROR)
		{
			TRACE("���մ����룺 %d-QNA\r\n", WSAGetLastError());
			return -3;
		}
		TRACE("�ѽ���-QNA\n");
		memcpy(Packet, buf, iRevcLen);
		*piPacketLen = iRevcLen;
		return 1;
	}

private:
	BYTE m_szDomain[PACKET_ALL_LEN];
};