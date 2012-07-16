//DNS Query Program
//Author : Prasshhant Pugalia (prasshhant.p@gmail.com)
//Dated  : 26/2/2007
#pragma warning( disable : 4996)   //屏蔽可strcpy等警告
//Header Files
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "vld.h"
#pragma comment(lib,"ws2_32.lib")   //Winsock Library

//本机DNS服务器IP列表
char dns_servers[10][100] = {0};

//Type field of Query and Answer
#define T_A		    1		/* host address IP地址*/
#define T_NS		2		/* authoritative server 名字服务器*/
#define T_CNAME		5		/* canonical name 规范名称*/
#define T_SOA		6		/* start of authority zone */
#define T_PTR		12		/* domain name pointer */
#define T_MX		15		/* mail routing information */

//Function Prototypes
void           ngethostbyname                (unsigned char*);
void           ChangetoDnsNameFormat         (unsigned char*,unsigned char*);//转换域名格式: www.google.com ——>  3www6google3com
unsigned char* ReadName                      (unsigned char*,unsigned char*, int*);
void           RetrieveDnsServersFromRegistry(void);  //从注册表获取DNS服务器IP
unsigned char* PrepareDnsQueryPacket         (unsigned char*);

//DNS 头结构
typedef struct _DNS_HEADER
{
	WORD    Xid;                    // 标识
	///////////////////////////报文首部的标志字段16位/////////////////////////////////
	BYTE    RecursionDesired : 1;   // 递归查询模式 
	BYTE    Truncation : 1;         // 完整封包信息｛为1时表示可截断的，使用UDP时，表示总应答长度超过512字节时，只返回前512字节｝
	BYTE    Authoritative : 1;      // 认证回应 1
	BYTE    Opcode : 4;             // 操作码{通常值为0（标准查询），其他值为1（反向查询）和2（服务器状态请求）}
	BYTE    IsResponse : 1;         // 0查询/1回应标志

	BYTE    ResponseCode : 4;       // 回应码 0没有差错、3名字差错
	BYTE    CheckingDisabled : 1;   // 检查无效
	BYTE    AuthenticatedData : 1;  // 认证数据
	BYTE    Reserved : 1;           // 预留
	BYTE    RecursionAvailable : 1; // 能否进行递归查询
	///////////////////////////报文首部的标志字段16位/////////////////////////////////

	WORD    QuestionCount;          // 问题数目
	WORD    AnswerCount;            // 回答数目
	WORD    NameServerCount;        // 权威回答数目
	WORD    AdditionalCount;        // 附加数目
}DNS_HEADER, *PDNS_HEADER;

//查询问题结构
typedef struct _DNS_WIRE_QUESTION
{
	WORD    QuestionType;  // 查询的资源类型
	WORD    QuestionClass; // 查询网络类型
}DNS_WIRE_QUESTION, *PDNS_WIRE_QUESTION;

//查询回应结构
#pragma pack(push, 1)

typedef struct _DNS_WIRE_RECORD
{
	WORD    RecordType;  // 查询回应类型
	WORD    RecordClass; // 查询回应的网络类型
	DWORD   TimeToLive;  // 在DNS服务器的存活时间
	WORD    DataLength;  // 数据的长度
}DNS_WIRE_RECORD, *PDNS_WIRE_RECORD;
#pragma pack(pop)

//资源记录内容指针
typedef struct _RES_RECORD
{
	BYTE *ResName;                      // 资源名称
	PDNS_WIRE_RECORD ResRecord;  // 资源数据
	BYTE *ResData;                        // 查询结果
}RES_RECORD, *PRES_RECORD;

//查询结构
typedef struct _QUERY
{
	BYTE *QueryName;                        // 资源名称
	PDNS_WIRE_QUESTION QueryQuesTion;       // 问题
} QUERY, *PQUERY;



int main()  //do you know what is int main() ?
{
	BYTE hostname[MAX_PATH] = {0};
	WSADATA firstsock;
	
	RetrieveDnsServersFromRegistry();
	printf("初始化套接字...\n");
	if (WSAStartup(MAKEWORD(2,2),&firstsock) != 0)
	{
		printf("初始化套接字错误码：%d",WSAGetLastError());
		return 1;
	} 
	printf("初始化完毕.\n");
    
	printf("输入主机名查找(输入QNA退出)：");
	gets((char*)hostname);
	while (strcmp((char*)hostname, "QNA"))
	{
		if (!strlen((char*)hostname))
		{
			strcpy((char*)hostname, "baidu.com");
			printf("baidu.com\n");
		}
		ngethostbyname(hostname);
		printf("输入主机名查找(输入QNA退出)：");
		ZeroMemory(hostname, sizeof(hostname));
		gets((char*)hostname);
	}
    
	return 0;
}

void ngethostbyname(unsigned char *host)
{
	BYTE buf[65536] = {0}, *pQuestionName = NULL,*reader = NULL;
	int i = 0, j = 0, stop = 0;
	int iLen = 0;
	
	SOCKET NDS_Socket = 0;
	struct sockaddr_in a;
	struct sockaddr_in dest;  //指针，指向目的套接字的地址 

	RES_RECORD answers[20] = {0},auth[20] = {0},addit[20] = {0};  //资源记录内容指针	
	PDNS_HEADER pDnsHeader = NULL;                                //DNS 头结构
	PDNS_WIRE_QUESTION pDnsQusetion = NULL;                       //查询问题结构

	NDS_Socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //DNS请求查询包为UDP类型的

	dest.sin_family = AF_INET;
	dest.sin_port = htons(53);
	dest.sin_addr.s_addr = inet_addr(dns_servers[0]);  //DNS服务器IP

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
	ChangetoDnsNameFormat(pQuestionName, host); 
	pDnsQusetion = (PDNS_WIRE_QUESTION)&buf[sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName) + 1)]; //fill it

	pDnsQusetion->QuestionType = htons(1);  //查询类型we are requesting the ipv4 address
	pDnsQusetion->QuestionClass = htons(1); //查询类 its internet (lol)

	//发送查询数据包
	if(sendto(
		NDS_Socket,
		(char*)buf,
		sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION),
		0,
		(struct sockaddr*)&dest,
		sizeof(dest))==SOCKET_ERROR)
	{
		printf("发送错误码：%d\n",WSAGetLastError());
	}
	printf("已发送查询包\n");

	ZeroMemory(buf, 65536);
	iLen = sizeof(dest);
	printf("接收回复...\r\n");
	int iRecvLen = recvfrom (NDS_Socket, (char*)buf, 65536, 0, (struct sockaddr*)&dest, &iLen);
	if(iRecvLen==SOCKET_ERROR)
	{
		printf("接收错误码： %d\r\n", WSAGetLastError());
	}
	printf("已接收\n");
	
	pDnsHeader = (DNS_HEADER*)buf;
	
	int iSizeTem = sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION);
	//除去DNS查询头和问题字段 得到回复问题结构
	reader = &buf[sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION)];

    printf("DNS服务器响应包括：\n");
	printf(" %d 个问题.\n", ntohs(pDnsHeader->QuestionCount));
	printf(" %d 个回答.\n", ntohs(pDnsHeader->AnswerCount));
	printf(" %d 个授权服务器.\n", ntohs(pDnsHeader->NameServerCount));
	printf(" %d 个额外信息.\n\n", ntohs(pDnsHeader->AdditionalCount));

	
	//解析回复结构
	stop = 0;	
	//ntohs 将一个无符号短整形数从网络字节顺序转换为主机字节顺序
	for(i=0; i<ntohs(pDnsHeader->AnswerCount); i++)
	{		
		answers[i].ResName = ReadName(reader, buf, &stop);
		reader = reader + stop;
		
		answers[i].ResRecord = (PDNS_WIRE_RECORD)(reader);
		reader = reader + sizeof(DNS_WIRE_RECORD);
	
		if(ntohs(answers[i].ResRecord->RecordType) == 1) //if its an ipv4 address
		{
			answers[i].ResData = (unsigned char*)malloc(ntohs(answers[i].ResRecord->DataLength));
			
			for(j=0 ; j<ntohs(answers[i].ResRecord->DataLength) ; j++)
				answers[i].ResData[j]=reader[j];
			
			answers[i].ResData[ntohs(answers[i].ResRecord->DataLength)] = '\0';
			
			reader = reader + ntohs(answers[i].ResRecord->DataLength);
		
		}
		else
		{
			answers[i].ResData = ReadName(reader,buf,&stop);
		    reader = reader + stop;
		}		
	}
/*	
	//read authorities
	for(i=0;i<ntohs(pDnsHeader->NameServerCount);i++)
	{
		auth[i].ResName=ReadName(reader,buf,&stop);
		reader+=stop;
		
		auth[i].ResRecord=(PDNS_WIRE_RECORD)(reader);
		reader+=sizeof(DNS_WIRE_RECORD);
	
		auth[i].ResData=ReadName(reader,buf,&stop);
		reader+=stop;
	}

	//read additional
	for(i=0;i<ntohs(pDnsHeader->AdditionalCount);i++)
	{
		addit[i].ResName=ReadName(reader,buf,&stop);
		reader+=stop;
		
		addit[i].ResRecord=(PDNS_WIRE_RECORD)(reader);
		reader+=sizeof(DNS_WIRE_RECORD);
	
		if(ntohs(addit[i].ResRecord->RecordType)==1)
		{
			addit[i].ResData = (unsigned char*)malloc(ntohs(addit[i].ResRecord->DataLength));
			for(j=0;j<ntohs(addit[i].ResRecord->DataLength);j++)
				addit[i].ResData[j]=reader[j];

			addit[i].ResData[ntohs(addit[i].ResRecord->DataLength)]='\0';
			reader+=ntohs(addit[i].ResRecord->DataLength);
		
		}
		else
		{
			addit[i].ResData=ReadName(reader,buf,&stop);
		    reader+=stop;
		}
	}
*/
	
	//print answers
	for(i=0;i<ntohs(pDnsHeader->AnswerCount);i++)
	{
		//printf("\nAnswer : %d",i+1);
		printf("域名：%s \r",answers[i].ResName);
		free(answers[i].ResName);
			
		if(ntohs(answers[i].ResRecord->RecordType)==1)   //IPv4 address
		{
			
			long *p;
			p=(long*)answers[i].ResData;
			a.sin_addr.s_addr=(*p);    //working without ntohl
			printf("IP地址：%s\n",inet_ntoa(a.sin_addr));
		}
		if(ntohs(answers[i].ResRecord->RecordType)==5)   //Canonical name for an alias
			printf("别名：%s\n",answers[i].ResData);
		//free(answers[i].ResData);
				
		printf("\n");
	}
/*
	//print authorities
	for(i=0;i<ntohs(pDnsHeader->NameServerCount);i++)
	{
		//printf("\nAuthorities : %d",i+1);
		printf("Name  :  %s ",auth[i].ResName);
		if(ntohs(auth[i].ResRecord->RecordType)==2)
			printf("has authoritative nameserver : %s",auth[i].ResData);
		printf("\n");
	}

	//print additional resource records
	for(i=0;i<ntohs(pDnsHeader->AdditionalCount);i++)
	{
		//printf("\nAdditional : %d",i+1);
		printf("Name  :  %s ",addit[i].ResName);
		if(ntohs(addit[i].ResRecord->RecordType)==1)
		{
			long *p;
			p=(long*)addit[i].ResData;
			a.sin_addr.s_addr=(*p);    //working without ntohl
			printf("has IPv4 address :  %s",inet_ntoa(a.sin_addr));
		}
		printf("\n");
	}
*/
    return;
}

BYTE* ReadName(BYTE* reader, BYTE* buffer, int* count)
{
	BYTE *pQueryName = NULL;
	WORD p=0, jumped=0, offset;
	int i = 0, j = 0;

	*count = 1;
	pQueryName = (BYTE*)malloc(MAX_PATH);
	ZeroMemory(pQueryName, MAX_PATH);

	//读取在3www6google3com格式的名称
	//baidu.com: type A, class IN, addr 220.181.111.86
	//c0 0c 00 01 00 01 00 00 00 99 00 04 dc b5 6f 56  
	while(*reader!=0)
	{
		if(*reader>=192)
		{
			DWORD dwtem = *reader;
			dwtem = *(reader+1);
			offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000  ;偏移
			reader = buffer + offset - 1;
			jumped = 1;  //we have jumped to another location so counting wont go up!
		}
		else 
			pQueryName[p++]=*reader;
		
		reader=reader+1;		
		if(jumped==0)
			*count = *count + 1; //if we havent jumped to another location then we can count up
	}
	
	pQueryName[p]='\0';    //string complete
	if(jumped==1)
		*count = *count + 1;  //number of steps we actually moved forward in the packet
	
	//now convert 3www6google3com0 to www.google.com
	for(i=0;i<(int)strlen((const char*)pQueryName);i++)
	{
		p= pQueryName[i];
		for(j=0;j<(int)p;j++)
		{
			pQueryName[i]= pQueryName[i+1];
			i=i+1;
		}
		pQueryName[i]='.';
	}
	pQueryName[i-1]='\0';	  //remove the last dot
	return pQueryName;		
}


//从注册表获取DNS服务器IP
void RetrieveDnsServersFromRegistry()
{
	HKEY hkey=0;
	char name[256];
	char *path="SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
	char *fullpath[256];
	unsigned long s=sizeof(name);
	int  dns_count=0 , err , i , j;
	HKEY inter;
	unsigned long count;


	//Open the registry folder
	RegOpenKeyEx(HKEY_LOCAL_MACHINE , path , 0 , KEY_READ , &hkey );
		
	
	
	//how many interfaces
	RegQueryInfoKey(hkey, 0 , 0 , 0 , &count , 0 , 0 , 0 , 0 , 0 , 0 , 0 );
	
	for(i=0;i<(int)count;i++)
	{
		s=256;
		//Get the interface subkey name
		RegEnumKeyEx(hkey , i , (char*)name , &s , 0 , 0 , 0 , 0 );
		
		//Make the full path
		strcpy((char*)fullpath,path);
		strcat((char*)fullpath,"\\");
		strcat((char*)fullpath,name);
				
		//Open the full path name
		RegOpenKeyEx(HKEY_LOCAL_MACHINE , (const char*)fullpath , 0 , KEY_READ , &inter );
				
		//Extract the value in Nameserver field
		s=256;
		err=RegQueryValueEx(inter , "NameServer" , 0 , 0 , (unsigned char*)name ,	&s );
		
		if(err==ERROR_SUCCESS && strlen(name)>0) strcpy(dns_servers[dns_count++],name);
	}

	for(i=0;i<dns_count;i++)
	{
		for(j=0;j<(int)strlen(dns_servers[i]);j++)
		{
			if(dns_servers[i][j]==',' || dns_servers[i][j]==' ')
			{
				strcpy(dns_servers[dns_count++],dns_servers[i]+j+1);
				dns_servers[i][j]=0;
			}
		}
	}

	printf("在您的系统中发现以下的DNS服务器 ...\n");
	for(i=0;i<dns_count;i++)
	{
		printf("%d)  %s \n",i+1,dns_servers[i]);
	}
}


//转换域名格式: www.google.com ——>  3www6google3com
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host)
{
	int lock=0 , i;
	
	strcat((char*)host,".");
	
	for(i=0;i<(int)strlen((char*)host);i++)
	{
		if(host[i]=='.')
		{
			*dns++=i-lock;
			for(;lock<i;lock++)
			{
				*dns++=host[lock];
			}
			lock++; //or lock=i+1;
		}
	}
    *dns++='\0';
}



