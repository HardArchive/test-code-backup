//DNS Query Program
//Author : Prasshhant Pugalia (prasshhant.p@gmail.com)
//Dated  : 26/2/2007
#pragma warning( disable : 4996)   //���ο�strcpy�Ⱦ���
//Header Files
#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "vld.h"
#pragma comment(lib,"ws2_32.lib")   //Winsock Library

//����DNS������IP�б�
char dns_servers[10][100] = {0};

//Type field of Query and Answer
#define T_A		    1		/* host address IP��ַ*/
#define T_NS		2		/* authoritative server ���ַ�����*/
#define T_CNAME		5		/* canonical name �淶����*/
#define T_SOA		6		/* start of authority zone */
#define T_PTR		12		/* domain name pointer */
#define T_MX		15		/* mail routing information */

//Function Prototypes
void           ngethostbyname                (unsigned char*);
void           ConvertDomainToDNSName         (BYTE*, BYTE*);//ת��������ʽ: www.google.com ����>  3www6google3com
int ReadName(BYTE* reader, BYTE* buffer, BYTE* pQueryName, int* count);
void           RetrieveDnsServersFromRegistry(void);  //��ע����ȡDNS������IP
unsigned char* PrepareDnsQueryPacket         (unsigned char*);

//DNS ͷ�ṹ
typedef struct _DNS_HEADER
{
	WORD    Xid;                    // ��ʶ
	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////
	BYTE    RecursionDesired : 1;   // �ݹ��ѯģʽ 
	BYTE    Truncation : 1;         // ���������Ϣ��Ϊ1ʱ��ʾ�ɽضϵģ�ʹ��UDPʱ����ʾ��Ӧ�𳤶ȳ���512�ֽ�ʱ��ֻ����ǰ512�ֽڣ�
	BYTE    Authoritative : 1;      // ��֤��Ӧ 1
	BYTE    Opcode : 4;             // ������{ͨ��ֵΪ0����׼��ѯ��������ֵΪ1�������ѯ����2��������״̬����}
	BYTE    IsResponse : 1;         // 0��ѯ/1��Ӧ��־

	BYTE    ResponseCode : 4;       // ��Ӧ�� 0û�в��3���ֲ��
	BYTE    CheckingDisabled : 1;   // �����Ч
	BYTE    AuthenticatedData : 1;  // ��֤����
	BYTE    Reserved : 1;           // Ԥ��
	BYTE    RecursionAvailable : 1; // �ܷ���еݹ��ѯ
	///////////////////////////�����ײ��ı�־�ֶ�16λ/////////////////////////////////

	WORD    QuestionCount;          // ������Ŀ
	WORD    AnswerCount;            // �ش���Ŀ
	WORD    NameServerCount;        // Ȩ���ش���Ŀ
	WORD    AdditionalCount;        // ������Ŀ
}DNS_HEADER, *PDNS_HEADER;

//��ѯ����ṹ
typedef struct _DNS_WIRE_QUESTION
{
	WORD    QuestionType;  // ��ѯ����Դ����
	WORD    QuestionClass; // ��ѯ��������
}DNS_WIRE_QUESTION, *PDNS_WIRE_QUESTION;

//��ѯ��Ӧ�ṹ
#pragma pack(push, 1)

typedef struct _DNS_WIRE_RECORD
{
	WORD    RecordType;  // ��ѯ��Ӧ����
	WORD    RecordClass; // ��ѯ��Ӧ����������
	DWORD   TimeToLive;  // ��DNS�������Ĵ��ʱ��
	WORD    DataLength;  // ���ݵĳ���
}DNS_WIRE_RECORD, *PDNS_WIRE_RECORD;
#pragma pack(pop)

//��Դ��¼����ָ��
typedef struct _RES_RECORD
{
	BYTE *ResName;                        // ��Դ����
	PDNS_WIRE_RECORD ResRecord;           // ��Դ����
	BYTE *ResData;                        // ��ѯ���
}RES_RECORD, *PRES_RECORD;

//��ѯ�ṹ
typedef struct _QUERY
{
	BYTE *QueryName;                        // ��Դ����
	PDNS_WIRE_QUESTION QueryQuesTion;       // ����
} QUERY, *PQUERY;



int main()  //do you know what is int main() ?
{
	BYTE hostname[MAX_PATH] = {0};
	WSADATA firstsock;
	
	RetrieveDnsServersFromRegistry();
	printf("��ʼ���׽���...\n");
	if (WSAStartup(MAKEWORD(2,2),&firstsock) != 0)
	{
		printf("��ʼ���׽��ִ����룺%d",WSAGetLastError());
		return 1;
	} 
	printf("��ʼ�����.\n");
    
	printf("��������������(����QNA�˳�)��");
	gets((char*)hostname);
	while (strcmp((char*)hostname, "QNA"))
	{
		if (!strlen((char*)hostname))
		{
			strcpy((char*)hostname, "www.baidu.com");
			printf("www.baidu.com\n");
		}
		ngethostbyname(hostname);
		printf("��������������(����QNA�˳�)��");
		ZeroMemory(hostname, sizeof(hostname));
		gets((char*)hostname);
	}
    
	return 0;
}

void ngethostbyname(BYTE *host)
{
	BYTE buf[65536] = {0}, *pQuestionName = NULL,*pReader = NULL;
	int i = 0, j = 0, stop = 0;
	int iLen = 0;
	
	SOCKET NDS_Socket = 0;
	struct sockaddr_in a;
	struct sockaddr_in dest;  //ָ�룬ָ��Ŀ���׽��ֵĵ�ַ 

	RES_RECORD answers[20] = {0},auth[20] = {0},addit[20] = {0};  //��Դ��¼����ָ��	
	PDNS_HEADER pDnsHeader = NULL;                                //DNS ͷ�ṹ
	PDNS_WIRE_QUESTION pDnsQusetion = NULL;                       //��ѯ����ṹ

	NDS_Socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //DNS�����ѯ��ΪUDP���͵�

	dest.sin_family = AF_INET;
	dest.sin_port = htons(53);
	dest.sin_addr.s_addr = inet_addr(dns_servers[0]);  //DNS������IP

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
	ConvertDomainToDNSName(pQuestionName, host); 
	pDnsQusetion = (PDNS_WIRE_QUESTION)&buf[sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName) + 1)]; //fill it

	pDnsQusetion->QuestionType = htons(1);  //��ѯ����we are requesting the ipv4 address
	pDnsQusetion->QuestionClass = htons(1); //��ѯ�� its internet (lol)

	//���Ͳ�ѯ���ݰ�
	if(sendto(
		NDS_Socket,
		(char*)buf,
		sizeof(DNS_HEADER) + (strlen((const char*)pQuestionName)+1) + sizeof(DNS_WIRE_QUESTION),
		0,
		(struct sockaddr*)&dest,
		sizeof(dest))==SOCKET_ERROR)
	{
		printf("���ʹ����룺%d\n",WSAGetLastError());
	}
	printf("�ѷ��Ͳ�ѯ��\n");

	ZeroMemory(buf, 65536);
	iLen = sizeof(dest);
	printf("���ջظ�...\r\n");
	int iRecvLen = recvfrom (NDS_Socket, (char*)buf, 65536, 0, (struct sockaddr*)&dest, &iLen); //buf��������DNS��
	if(iRecvLen==SOCKET_ERROR)
	{
		printf("���մ����룺 %d\r\n", WSAGetLastError());
	}
	printf("�ѽ���\n");
	
	pDnsHeader = (PDNS_HEADER)buf;

	pReader = &buf[sizeof(DNS_HEADER)];
	BYTE szDomain[MAX_PATH] = {0};

	ReadName(pReader, buf, szDomain, &stop);  //����ƫ�Ƶ�ַ
	//www.baidu.com
	//��ȥDNS��ѯͷ�������ֶ� �õ��ظ�����ṹ
	pReader = &buf[sizeof(DNS_HEADER) + stop + sizeof(DNS_WIRE_QUESTION)];

    printf("DNS��������Ӧ������\n");
	printf(" %d ������.\n", ntohs(pDnsHeader->QuestionCount));
	printf(" %d ���ش�.\n", ntohs(pDnsHeader->AnswerCount));
	printf(" %d ����Ȩ������.\n", ntohs(pDnsHeader->NameServerCount));
	printf(" %d ��������Ϣ.\n\n", ntohs(pDnsHeader->AdditionalCount));
	
	//�����ظ��ṹ
	stop = 0;	
	//ntohs ��һ���޷��Ŷ��������������ֽ�˳��ת��Ϊ�����ֽ�˳��  ѭ�� ����ظ�����
	for(i=0; i<ntohs(pDnsHeader->AnswerCount); i++)
	{		
		answers[i].ResName = new BYTE[MAX_PATH];  //��Դ�� ���ڴ����ַ
		ZeroMemory(answers[i].ResName, MAX_PATH);
		ReadName(pReader, buf, answers[i].ResName, &stop); //�õ���ַ ��ֹͣ��ȡλ��
		//answers[i].ResName = ReadName(reader, buf, &stop);
		pReader = pReader + stop;  //��ַ�����һ�ṹ
		
		answers[i].ResRecord = (PDNS_WIRE_RECORD)(pReader);
		pReader = pReader + sizeof(DNS_WIRE_RECORD);  //����ָ�� ��ѯ��Ӧ�ṹ ��Դ����
	
		if(ntohs(answers[i].ResRecord->RecordType) == T_A) //if its an ipv4 address ����IP��� A��¼
		{
			//_asm int 3;
			int iLen2 = ntohs(answers[i].ResRecord->DataLength);
			answers[i].ResData = new BYTE[iLen2];  //�ĸ��ֽ�
			ZeroMemory(answers[i].ResData, iLen2);
			
			for(j=0 ; j<ntohs(answers[i].ResRecord->DataLength) ; j++)
				answers[i].ResData[j]=pReader[j]; //�����ֽڶ����һ�� IP��һ����
			
			answers[i].ResData[ntohs(answers[i].ResRecord->DataLength)] = '\0';
			
			pReader = pReader + ntohs(answers[i].ResRecord->DataLength);
		
		}
		else  //�����������ѯ
		{
			answers[i].ResData = new BYTE[MAX_PATH];
			ZeroMemory(answers[i].ResData, MAX_PATH);
			ReadName(pReader, buf, answers[i].ResData, &stop);
		    pReader = pReader + stop;
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
		printf("������%s \r",answers[i].ResName);
		delete answers[i].ResName;
		
		//A��¼
		if(ntohs(answers[i].ResRecord->RecordType)==1)   //IPv4 address
		{		
			long *p = NULL;
			p = (long*)answers[i].ResData;
			a.sin_addr.s_addr = (*p);    //working without ntohl
			printf("IP��ַ��%s\n",inet_ntoa(a.sin_addr));
			//delete answers[i].ResData;			
		}
		//������¼
		if(ntohs(answers[i].ResRecord->RecordType)==5)
		{
			printf("������%s\n",answers[i].ResData);
			delete answers[i].ResData;
		}				
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

//BYTE* ReadName(BYTE* reader, BYTE* buffer, int* count)
//{
//	BYTE *pQueryName = NULL;
//	WORD p=0, jumped=0, offset;
//	int i = 0, j = 0;
//
//	*count = 1;
//	pQueryName = (BYTE*)malloc(MAX_PATH);
//	ZeroMemory(pQueryName, MAX_PATH);
//
//	//��ȡ��3www6google3com��ʽ������
//	//baidu.com: type A, class IN, addr 220.181.111.86
//	//c0 0c 00 01 00 01 00 00 00 99 00 04 dc b5 6f 56  
//	while(*reader!=0)
//	{
//		if(*reader>=192)
//		{
//			DWORD dwtem = *reader;
//			dwtem = *(reader+1);
//			offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000  ;ƫ��
//			reader = buffer + offset - 1;
//			jumped = 1;  //we have jumped to another location so counting wont go up!
//		}
//		else 
//			pQueryName[p++]=*reader;
//		
//		reader=reader+1;		
//		if(jumped==0)
//			*count = *count + 1; //if we havent jumped to another location then we can count up
//	}
//	
//	pQueryName[p]='\0';    //string complete
//	if(jumped==1)
//		*count = *count + 1;  //number of steps we actually moved forward in the packet
//	
//	//now convert 3www6google3com0 to www.google.com
//	for(i=0;i<(int)strlen((const char*)pQueryName);i++)
//	{
//		p= pQueryName[i];
//		for(j=0;j<(int)p;j++)
//		{
//			pQueryName[i]= pQueryName[i+1];
//			i=i+1;
//		}
//		pQueryName[i]='.';
//	}
//	pQueryName[i-1]='\0';	  //remove the last dot
//	return pQueryName;		
//}

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



//��ע����ȡDNS������IP
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

	printf("������ϵͳ�з������µ�DNS������ ...\n");
	for(i=0;i<dns_count;i++)
	{
		printf("%d)  %s \n",i+1,dns_servers[i]);
	}
}


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



