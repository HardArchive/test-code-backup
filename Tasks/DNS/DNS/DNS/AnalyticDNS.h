#pragma once
#define PACKET_ALL_LEN 256

/******************************************************************************* 
* 1�� �ļ����ƣ� AnalyticDNS
* 2�� �桡������ Version *.*
* 3�� ��    ���� DNS�ظ������࣬����Ϊ����ר�Ÿ�DNS��������Ͳ�ѯ��̳е�
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-4-26 14:03:15
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 
class CAnalyticDNS
{
public:

	CAnalyticDNS(void)
	{
		ZeroMemory(m_DnsServers, sizeof(m_DnsServers));
	}

	~CAnalyticDNS(void)
	{
		ZeroMemory(m_DnsServers, sizeof(m_DnsServers));
	}

public:
	//ת��������ʽ: www.google.com ����>  3www6google3com  	
	//BYTE* dns ת���������, BYTE* host ��׼����
	//ת��������ʽ: www.google.com ����>  3www6google3com
	void ConvertDomainToDNSName(BYTE* pDns, BYTE* pDomain)
	{
		int lock = 0;
		BYTE szbHost[MAX_PATH] = {0};
		//sprintf_s((char*)szbHost, strlen((char*)host)+1, "%s.", (char*)host);
		sprintf((char*)szbHost, "%s.", (char*)pDomain);
		for(int i=0; i<(int)strlen((char*)szbHost); i++)
		{
			if(szbHost[i]=='.')
			{
				*pDns++ = i-lock;
				for(; lock<i; lock++)
				{
					*pDns++ = szbHost[lock];
				}
				lock++; //or lock=i+1;
			}
		}
		*pDns++ = '\0';
	}

	//����ȡ����DNS����ת��Ϊ��׼���� 3www6google3com0 to www.google.com
	bool ConvertDNSNameToDomain(BYTE* pQueryName)
	{
		int i = 0;
		WORD wNum = 0;   //�ַ�����
		if (!pQueryName)
		{
			return false;
		}
		for(i=0; i<(int)strlen((const char*)pQueryName); i++)
		{
			wNum = pQueryName[i];
			for(int j=0; j<(int)wNum; j++)
			{
				pQueryName[i] = pQueryName[i+1];
				i = i + 1;
			}
			pQueryName[i] = '.';
		}
		pQueryName[i-1] = '\0';	  //�Ƴ����ĵ�
		return true;
	}

	//BYTE* reader,��ǰ������ַ��ƫ�Ƶ�ַ�� BYTE* buffer,DNS���Ľṹ�׵�ַ  BYTE* pQueryName����������, int* count reader��λ����
	int ReadName(BYTE* reader, BYTE* buffer, BYTE* pQueryName, int* count)
	{
		bool bIsJump = false;   //���ǵ�ǰ��ַ�Ƿ�Ϊƫ�Ƶ�ַ
		WORD wTem = 0, woffset = 0;
		*count = 1;
		if (!pQueryName)
		{
			return -1;
		}
		//��ȡ��3www6google3com��ʽ������
		while(*reader!=0)
		{
			//Ϊƫ�Ƶ�ַʱ ֻ��ת������ 
			if(*reader>=192)
			{
				woffset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000  ;ƫ��
				reader = buffer + woffset - 1;  //�õ���ѯ����ṹ�е�������ַ
				bIsJump = true;
			}
			else 
				pQueryName[wTem++]=*reader;  //���︴������

			reader = reader+1;	
			if (!bIsJump)
			{
				//����Ϊƫ��ʱ����
				*count = *count + 1; 
			}
		}
		pQueryName[wTem]='\0';       //�����ַ�������	

		if (bIsJump)
		{
			//����ַΪƫ��ʱ����
			*count = *count + 1;  
		}

		//ת������
		if (!ConvertDNSNameToDomain(pQueryName))
			return -2;	
		return 1;		
	}

	//����DNS�ظ�
	bool AnalyticQueryDNS(BYTE* Packet, int iPacketLen)
	{
		int i = 0, j = 0;
		int iOffsetEnd = 0;
		BYTE *pReader = NULL, szQuestionName[PACKET_ALL_LEN] = {0};
		RES_RECORD answers[20] = {0};  //��Դ��¼����ָ��	
		PDNS_HEADER pDnsHeader = NULL;        //DNS ͷ�ṹ
		struct sockaddr_in a;

		pDnsHeader = (PDNS_HEADER)Packet;
		TRACE("DNS��������Ӧ������-QNA\n");
		TRACE(" %d ������.-QNA\n", ntohs(pDnsHeader->QuestionCount));
		TRACE(" %d ���ش�.-QNA\n", ntohs(pDnsHeader->AnswerCount));
		TRACE(" %d ����Ȩ������.-QNA\n", ntohs(pDnsHeader->NameServerCount));
		TRACE(" %d ��������Ϣ.-QNA\n", ntohs(pDnsHeader->AdditionalCount));
		if (0==ntohs(pDnsHeader->AnswerCount))
		{
			return false;
		}

		//ConvertDomainToDNSName(szQuestionName, m_szDomain);
		pReader = &Packet[sizeof(DNS_HEADER)];
		BYTE szDomain[MAX_PATH] = {0};
		ReadName(pReader, Packet, szDomain, &iOffsetEnd);  //�õ����Ͱ��������� ƫ�Ƶ�ַ���λ��
		//��ȥDNS��ѯͷ�������ֶ� �õ��ظ�����ṹ
		pReader = &Packet[sizeof(DNS_HEADER) + iOffsetEnd + sizeof(DNS_WIRE_QUESTION)];

		//�����ظ��ṹ
		iOffsetEnd = 0;	
		//ntohs ��һ���޷��Ŷ��������������ֽ�˳��ת��Ϊ�����ֽ�˳��
		for(int i=0; i<ntohs(pDnsHeader->AnswerCount); i++)
		{
			ZeroMemory(szDomain, sizeof(szDomain));
			//answers[i].ResName = new BYTE[MAX_PATH];
			//ZeroMemory(answers[i].ResName, MAX_PATH);
			answers[i].ResName = szDomain;
			ReadName(pReader, Packet, answers[i].ResName, &iOffsetEnd);
			TRACE("������%s \r",answers[i].ResName);
			pReader = pReader + iOffsetEnd;

			answers[i].ResRecord = (PDNS_WIRE_RECORD)(pReader);
			pReader = pReader + sizeof(DNS_WIRE_RECORD);

			if(ntohs(answers[i].ResRecord->RecordType) == 1) //if its an ipv4 address
			{
				BYTE szIP[16] = {0};
				answers[i].ResData = szIP;

				for(j=0 ; j<ntohs(answers[i].ResRecord->DataLength) ; j++)
					answers[i].ResData[j]=pReader[j];

				answers[i].ResData[ntohs(answers[i].ResRecord->DataLength)] = '\0';
				long *p = (long*)answers[i].ResData;
				a.sin_addr.s_addr=(*p);    //working without ntohl
				TRACE("IP��ַ��%s\n",inet_ntoa(a.sin_addr));

				pReader = pReader + ntohs(answers[i].ResRecord->DataLength);
			}
			else
			{
				//answers[i].ResData = new BYTE[MAX_PATH];
				//ZeroMemory(answers[i].ResData, MAX_PATH);
				ZeroMemory(szDomain, sizeof(szDomain));
				answers[i].ResData = szDomain;
				ReadName(pReader, Packet, answers[i].ResData, &iOffsetEnd);
				TRACE("������%s\n",answers[i].ResData);
				pReader = pReader + iOffsetEnd;
			}		
		}

		//for(i=0;i<ntohs(pDnsHeader->AnswerCount);i++)
		//{
		//	//printf("\nAnswer : %d",i+1);
		//	TRACE("������%s \r",answers[i].ResName);
		//	delete answers[i].ResName;

		//	if(ntohs(answers[i].ResRecord->RecordType)==1)   //IPv4 address
		//	{

		//		long *p;
		//		p=(long*)answers[i].ResData;
		//		a.sin_addr.s_addr=(*p);    //working without ntohl
		//		TRACE("IP��ַ��%s\n",inet_ntoa(a.sin_addr));
		//	}
		//	if(ntohs(answers[i].ResRecord->RecordType)==5)   //Canonical name for an alias
		//	{
		//		TRACE("������%s\n",answers[i].ResData);
		//		delete answers[i].ResData;
		//	}
		//	TRACE("\n");
		//}

		return true;
	}

	// ��ע������DNS������
	void RetrieveDnsServersFromRegistry()
	{
		HKEY hkey = 0;
		char name[256] = {0};
		char *path = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
		char *fullpath[256] = {0};
		unsigned long s = sizeof(name);
		int  dns_count=0, err=0, i=0, j=0;
		HKEY inter;
		unsigned long count = 0;

		//��ע��� 
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hkey);
		//��ȡ��������
		RegQueryInfoKey(hkey, 0, 0, 0, &count , 0, 0, 0, 0, 0, 0, 0);

		for(i=0; i<(int)count; i++)
		{
			s = 256;
			//ö��ָ�����·������� 
			RegEnumKeyEx(hkey, i, (char*)name, &s, 0, 0, 0, 0);
			//Make the full path
			strcpy((char*)fullpath, path);
			strcat((char*)fullpath, "\\");
			strcat((char*)fullpath, name);
			//��һ��ָ����ע����
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, (const char*)fullpath, 0, KEY_READ, &inter);
			//��ȡһ���������ֵ
			s = 256;
			err=RegQueryValueEx(inter, "NameServer", 0, 0, (unsigned char*)name,	&s);
			if(err==ERROR_SUCCESS && strlen(name)>0)
				strcpy(m_DnsServers[dns_count++], name);
		}

		for(i=0; i<dns_count; i++)
		{
			for(j=0; j<(int)strlen(m_DnsServers[i]); j++)
			{
				if(m_DnsServers[i][j]==',' || m_DnsServers[i][j]==' ')
				{
					strcpy(m_DnsServers[dns_count++], m_DnsServers[i]+j+1);
					m_DnsServers[i][j] = 0;
				}
			}
		}

		for(i=0; i<dns_count; i++)
		{
			TRACE("DNS������IP��%d)  %s-QNA\n", i+1, m_DnsServers[i]);
		}
	}

public:
	char m_DnsServers[10][100];

};
