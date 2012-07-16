#pragma once
#define PACKET_ALL_LEN 256

/******************************************************************************* 
* 1、 文件名称： AnalyticDNS
* 2、 版　　本： Version *.*
* 3、 描    述： DNS回复分析类，此类为父类专门给DNS包拦截类和查询类继承的
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-4-26 14:03:15
* 6、 修 改 人： 
* 7、 修改日期： 
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
	//转换域名格式: www.google.com ——>  3www6google3com  	
	//BYTE* dns 转化后的域名, BYTE* host 标准域名
	//转换域名格式: www.google.com ——>  3www6google3com
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

	//将读取到的DNS域名转化为标准域名 3www6google3com0 to www.google.com
	bool ConvertDNSNameToDomain(BYTE* pQueryName)
	{
		int i = 0;
		WORD wNum = 0;   //字符个数
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
		pQueryName[i-1] = '\0';	  //移除最后的点
		return true;
	}

	//BYTE* reader,当前域名地址或偏移地址； BYTE* buffer,DNS报文结构首地址  BYTE* pQueryName读到的域名, int* count reader后几位读完
	int ReadName(BYTE* reader, BYTE* buffer, BYTE* pQueryName, int* count)
	{
		bool bIsJump = false;   //表是当前地址是否为偏移地址
		WORD wTem = 0, woffset = 0;
		*count = 1;
		if (!pQueryName)
		{
			return -1;
		}
		//读取在3www6google3com格式的名称
		while(*reader!=0)
		{
			//为偏移地址时 只需转跳两次 
			if(*reader>=192)
			{
				woffset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000  ;偏移
				reader = buffer + woffset - 1;  //得到查询问题结构中的域名地址
				bIsJump = true;
			}
			else 
				pQueryName[wTem++]=*reader;  //这里复制域名

			reader = reader+1;	
			if (!bIsJump)
			{
				//当不为偏移时自增
				*count = *count + 1; 
			}
		}
		pQueryName[wTem]='\0';       //域名字符串结束	

		if (bIsJump)
		{
			//当地址为偏移时自增
			*count = *count + 1;  
		}

		//转化域名
		if (!ConvertDNSNameToDomain(pQueryName))
			return -2;	
		return 1;		
	}

	//解析DNS回复
	bool AnalyticQueryDNS(BYTE* Packet, int iPacketLen)
	{
		int i = 0, j = 0;
		int iOffsetEnd = 0;
		BYTE *pReader = NULL, szQuestionName[PACKET_ALL_LEN] = {0};
		RES_RECORD answers[20] = {0};  //资源记录内容指针	
		PDNS_HEADER pDnsHeader = NULL;        //DNS 头结构
		struct sockaddr_in a;

		pDnsHeader = (PDNS_HEADER)Packet;
		TRACE("DNS服务器响应包括：-QNA\n");
		TRACE(" %d 个问题.-QNA\n", ntohs(pDnsHeader->QuestionCount));
		TRACE(" %d 个回答.-QNA\n", ntohs(pDnsHeader->AnswerCount));
		TRACE(" %d 个授权服务器.-QNA\n", ntohs(pDnsHeader->NameServerCount));
		TRACE(" %d 个额外信息.-QNA\n", ntohs(pDnsHeader->AdditionalCount));
		if (0==ntohs(pDnsHeader->AnswerCount))
		{
			return false;
		}

		//ConvertDomainToDNSName(szQuestionName, m_szDomain);
		pReader = &Packet[sizeof(DNS_HEADER)];
		BYTE szDomain[MAX_PATH] = {0};
		ReadName(pReader, Packet, szDomain, &iOffsetEnd);  //得到发送包数据域名 偏移地址最后位置
		//除去DNS查询头和问题字段 得到回复问题结构
		pReader = &Packet[sizeof(DNS_HEADER) + iOffsetEnd + sizeof(DNS_WIRE_QUESTION)];

		//解析回复结构
		iOffsetEnd = 0;	
		//ntohs 将一个无符号短整形数从网络字节顺序转换为主机字节顺序
		for(int i=0; i<ntohs(pDnsHeader->AnswerCount); i++)
		{
			ZeroMemory(szDomain, sizeof(szDomain));
			//answers[i].ResName = new BYTE[MAX_PATH];
			//ZeroMemory(answers[i].ResName, MAX_PATH);
			answers[i].ResName = szDomain;
			ReadName(pReader, Packet, answers[i].ResName, &iOffsetEnd);
			TRACE("域名：%s \r",answers[i].ResName);
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
				TRACE("IP地址：%s\n",inet_ntoa(a.sin_addr));

				pReader = pReader + ntohs(answers[i].ResRecord->DataLength);
			}
			else
			{
				//answers[i].ResData = new BYTE[MAX_PATH];
				//ZeroMemory(answers[i].ResData, MAX_PATH);
				ZeroMemory(szDomain, sizeof(szDomain));
				answers[i].ResData = szDomain;
				ReadName(pReader, Packet, answers[i].ResData, &iOffsetEnd);
				TRACE("别名：%s\n",answers[i].ResData);
				pReader = pReader + iOffsetEnd;
			}		
		}

		//for(i=0;i<ntohs(pDnsHeader->AnswerCount);i++)
		//{
		//	//printf("\nAnswer : %d",i+1);
		//	TRACE("域名：%s \r",answers[i].ResName);
		//	delete answers[i].ResName;

		//	if(ntohs(answers[i].ResRecord->RecordType)==1)   //IPv4 address
		//	{

		//		long *p;
		//		p=(long*)answers[i].ResData;
		//		a.sin_addr.s_addr=(*p);    //working without ntohl
		//		TRACE("IP地址：%s\n",inet_ntoa(a.sin_addr));
		//	}
		//	if(ntohs(answers[i].ResRecord->RecordType)==5)   //Canonical name for an alias
		//	{
		//		TRACE("别名：%s\n",answers[i].ResData);
		//		delete answers[i].ResData;
		//	}
		//	TRACE("\n");
		//}

		return true;
	}

	// 从注册表读出DNS服务器
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

		//打开注册表 
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &hkey);
		//获取子项数量
		RegQueryInfoKey(hkey, 0, 0, 0, &count , 0, 0, 0, 0, 0, 0, 0);

		for(i=0; i<(int)count; i++)
		{
			s = 256;
			//枚举指定项下方的子项 
			RegEnumKeyEx(hkey, i, (char*)name, &s, 0, 0, 0, 0);
			//Make the full path
			strcpy((char*)fullpath, path);
			strcat((char*)fullpath, "\\");
			strcat((char*)fullpath, name);
			//打开一个指定的注册表键
			RegOpenKeyEx(HKEY_LOCAL_MACHINE, (const char*)fullpath, 0, KEY_READ, &inter);
			//获取一个项的设置值
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
			TRACE("DNS服务器IP：%d)  %s-QNA\n", i+1, m_DnsServers[i]);
		}
	}

public:
	char m_DnsServers[10][100];

};
