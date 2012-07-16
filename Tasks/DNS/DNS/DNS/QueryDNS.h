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
		TRACE("初始化套接字...-QNA\n");
		if (WSAStartup(MAKEWORD(2,2),&firstsock) != 0)
		{
			TRACE("初始化套接字错误码：%d-QNA",WSAGetLastError());
			return;
		} 
		TRACE("初始化完毕.-QNA\n");
	}

	//DNS验证  域名-IP验证
	int VerifyDNS(char* pszDomain, char* pszIp)
	{
		int iRet = 0;
		int iPacektLen = 0;
		BYTE szPacket[PACKET_ALL_LEN] = {0};
		TRACE("DNS验证  域名:%s-IP:%s 验证-QNA\n", pszDomain, pszIp);
		
		//_asm int 3;

		iRet = QueryDNS((BYTE*)pszDomain, szPacket, &iPacektLen);
		if (1 != iRet)
		{
			return iRet;
		}
		if (!AnalyticQueryDNS(szPacket, iPacektLen))
		{
			TRACE("VerifyDNS——分析数据失败-QNA\n");
			return -1;
		}
		return 1;
	}	
private:

	/******************************************************************************* 
	1、 函数名称： QueryDNS
	2、 功能描述： 查询DNS
	3、 输入参数： host主机名， Packet 回复数据包内容， iPacketLen 回复数据包长度
	4、 返 回 值： 成功返回1，错误返回-1， 失败返回0
	5、 动态内存： 无
	6、 代码设计：  阿毛
	7、 开发日期： 2012-4-26 10:17:16
	8、 备    注： 
	********************************************************************************/ 
	int QueryDNS(BYTE *host, BYTE* Packet, int* piPacketLen)
	{
		BYTE buf[PACKET_ALL_LEN] = {0}, *pQuestionName = NULL;
		int iLen = 0;

		SOCKET NDS_Socket = 0;
		struct sockaddr_in dest;  //指针，指向目的套接字的地址 

		PDNS_HEADER pDnsHeader = NULL;                                //DNS 头结构
		PDNS_WIRE_QUESTION pDnsQusetion = NULL;                       //查询问题结构

		if (!(host && Packet))
		{
			TRACE("QueryDNS-域名或接收数据指针错误-QNA\n");
			return -1;
		}
		strcpy((char*)m_szDomain, (char*)host);

		NDS_Socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //DNS请求查询包为UDP类型的

		dest.sin_family = AF_INET;
		dest.sin_port = htons(53);
		dest.sin_addr.s_addr = inet_addr(m_DnsServers[0]);  //DNS服务器IP

		//设置DNS标准查询报文
		pDnsHeader = (PDNS_HEADER)&buf;       //查询报文内存	
		pDnsHeader->Xid = (unsigned short) htons((u_short)GetCurrentProcessId()); //当前进程id
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

		//指向DNS头后的查询问题第一部分  查询名
		pQuestionName =(BYTE *)&buf[sizeof(DNS_HEADER)];      
		ConvertDomainToDNSName(pQuestionName, m_szDomain); 
		pDnsQusetion = (PDNS_WIRE_QUESTION)&buf[sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName) + 1)]; //fill it
		pDnsQusetion->QuestionType = htons(1);  //查询类型we are requesting the ipv4 address
		pDnsQusetion->QuestionClass = htons(1); //查询类 its internet (lol)

		int iSendLen = sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION);
		iSendLen = sendto(
			NDS_Socket,
			(char*)buf,
			sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION),
			0,
			(struct sockaddr*)&dest,
			sizeof(dest));
		//发送查询数据包
		if(iSendLen==SOCKET_ERROR)
		{
			TRACE("发送错误码：%d-QNA\n",WSAGetLastError());
			return -2;
		}
		TRACE("已发送查询包-QNA\n");

		ZeroMemory(buf, PACKET_ALL_LEN);
		iLen = sizeof(dest);
		TRACE("接收回复...-QNA\r\n");
		int iRevcLen = recvfrom (NDS_Socket, (char*)buf, PACKET_ALL_LEN, 0, (struct sockaddr*)&dest, &iLen);
		//_asm int 3;
		if(iRevcLen==SOCKET_ERROR)
		{
			TRACE("接收错误码： %d-QNA\r\n", WSAGetLastError());
			return -3;
		}
		TRACE("已接收-QNA\n");
		memcpy(Packet, buf, iRevcLen);
		*piPacketLen = iRevcLen;
		return 1;
	}

private:
	BYTE m_szDomain[PACKET_ALL_LEN];
};