// DNSQuery1.cpp: implementation of the CDNSQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dnsquery.h"
#include "DNSQuery.h"

#include "winsock2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib,"ws2_32.lib")   //Winsock Library

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDNSQuery::CDNSQuery()
{

	WSADATA firstsock;
	
	RetrieveDnsServersFromRegistry();

	TRACE("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&firstsock) != 0)
	{
		TRACE("Failed. Error Code : %d",WSAGetLastError());
		return;
	} 
	TRACE("Initialised.");

}

CDNSQuery::~CDNSQuery()
{
	WSACleanup();
}

CString CDNSQuery::DNSQuery(unsigned char *host, char *dnsserver)
{
	unsigned char buf[65536],*qname,*reader;
	int i , j , stop;
	
	SOCKET s;
	struct sockaddr_in a;

	struct RES_RECORD answers[20],auth[20],addit[20];  //DNS�������Ļ�Ӧ
	struct sockaddr_in dest;
	
	struct DNS_HEADER1 *dns = NULL;
	struct QUESTION   *qinfo = NULL;

	CString sResult = "", sTemp;

	s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  //����UDP����DNS��ѯ

	dest.sin_family=AF_INET;
	dest.sin_port=htons(53);                    //DNS�˿�
	dest.sin_addr.s_addr=inet_addr(dnsserver);  //DNS������

	//����DNS��׼��ѯ�ṹ
	dns = (struct DNS_HEADER1 *)&buf;
	
	dns->id = (unsigned short) (htons((unsigned short) GetCurrentProcessId()));//��ǰ����ID
	dns->qr = 0;      //����һ����ѯ
	dns->opcode = 0;  //����һ����׼��ѯ
	dns->aa = 0;      //������֤
	dns->tc = 0;      //����һ���������
	dns->rd = 1;      //ϣ���ݹ��ѯ
	dns->ra = 0;      
	dns->z  = 0;
	dns->ad = 0;
	dns->cd = 0;
	dns->rcode = 0;
	dns->q_count = htons(1);   //ֻ��һ������
	dns->ans_count  = 0;
	dns->auth_count = 0;
	dns->add_count  = 0;
	
	//ָ��ָ���ѯ���ļ������ⲿ�֣���DNSͷ�ṹ֮��
	qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER1)];
    //ת��������ʽ,�����䣨����ѯ����������DNS֮��
	ChangetoDnsNameFormat(qname,host);
	//ָ��ָ���ѯ��֮��
	qinfo =(struct QUESTION*)&buf[sizeof(struct DNS_HEADER1) + (strlen((const char*)qname) + 1)]; //fill it
	//��ѯ���ͺͲ�ѯ��
	qinfo->qtype = htons(1);  //ֻ��ѯipv4��ַ
	qinfo->qclass = htons(1); //����internet����

	TRACE("\nSending Packet...");
	if(sendto(s,(char*)buf,sizeof(struct DNS_HEADER1) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION),0,(struct sockaddr*)&dest,sizeof(dest))==SOCKET_ERROR)
	{
		TRACE("%d  error",WSAGetLastError());
	}
	TRACE("Sent");
	
	i = sizeof(dest);
	TRACE("\nReceiving answer...");
	if(recvfrom (s,(char*)buf,65536,0,(struct sockaddr*)&dest,&i)==SOCKET_ERROR)
	{
		TRACE("Failed. Error Code : %d",WSAGetLastError());
	}
	TRACE("Received.");
	
	dns=(struct DNS_HEADER1*)buf;
	
	//ָ��Խ��DNS��ͷ�Ͳ�ѯ���ⲿ��
	reader=&buf[sizeof(struct DNS_HEADER1) + (strlen((const char*)qname)+1) + sizeof(struct QUESTION)];

    sTemp.Format("The response contains : ");
	sResult += sTemp;
	sTemp.Format("\r\n %d Questions.",ntohs(dns->q_count));
	sResult += sTemp;
	sTemp.Format("\r\n %d Answers.",ntohs(dns->ans_count));
	sResult += sTemp;
	sTemp.Format("\r\n %d Authoritative Servers.",ntohs(dns->auth_count));
	sResult += sTemp;
	sTemp.Format("\r\n %d Additional records.\r\n",ntohs(dns->add_count));
	sResult += sTemp;

	//������ش�
	stop=0;
	
	for(i=0;i<ntohs(dns->ans_count);i++)
	{
		answers[i].name=ReadName(reader,buf,&stop);
		reader = reader + stop;
		
		answers[i].resource = (struct R_DATA*)(reader);
		reader = reader + sizeof(struct R_DATA);
	
		if(ntohs(answers[i].resource->type) == 1) //if its an ipv4 address
		{
			answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->data_len));
			
			for(j=0 ; j<ntohs(answers[i].resource->data_len) ; j++)
				answers[i].rdata[j]=reader[j];
			
			answers[i].rdata[ntohs(answers[i].resource->data_len)] = '\0';
			
			reader = reader + ntohs(answers[i].resource->data_len);
		
		}
		else
		{
			answers[i].rdata = ReadName(reader,buf,&stop);
		    reader = reader + stop;
		}
	}
	
	//����֤��Ӧ
	for(i=0;i<ntohs(dns->auth_count);i++)
	{
		auth[i].name=ReadName(reader,buf,&stop);
		reader+=stop;
		
		auth[i].resource=(struct R_DATA*)(reader);
		reader+=sizeof(struct R_DATA);
	
		auth[i].rdata=ReadName(reader,buf,&stop);
		reader+=stop;
	}

	//��������Դ����
	for(i=0;i<ntohs(dns->add_count);i++)
	{
		addit[i].name=ReadName(reader,buf,&stop);
		reader+=stop;
		
		addit[i].resource=(struct R_DATA*)(reader);
		reader+=sizeof(struct R_DATA);
	
		if(ntohs(addit[i].resource->type)==1)
		{
			addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->data_len));
			for(j=0;j<ntohs(addit[i].resource->data_len);j++)
				addit[i].rdata[j]=reader[j];

			addit[i].rdata[ntohs(addit[i].resource->data_len)]='\0';
			reader+=ntohs(addit[i].resource->data_len);
		
		}
		else
		{
			addit[i].rdata=ReadName(reader,buf,&stop);
		    reader+=stop;
		}
	}

	
	//����ش𣬼���ѯ���
	for(i=0;i<ntohs(dns->ans_count);i++)
	{
		sTemp.Format("Name : %s ",answers[i].name);
		sResult += sTemp;
			
		if(ntohs(answers[i].resource->type)==1)   //IPv4��ַ
		{
			
			long *p;
			p=(long*)answers[i].rdata;
			a.sin_addr.s_addr=(*p);    
			sTemp.Format("has \r\nIPv4 address :  %s",inet_ntoa(a.sin_addr));
			sResult += sTemp;
		}
		if(ntohs(answers[i].resource->type)==5)   //����
		{
			sTemp.Format("has alias name : %s",answers[i].rdata);
			sResult += sTemp;
		}
				
		sTemp.Format("\r\n");
		sResult += sTemp;
	}

	//�����֤��Ϣ
	for(i=0;i<ntohs(dns->auth_count);i++)
	{
		sTemp.Format("Name : %s ",auth[i].name);
		sResult += sTemp;
		if(ntohs(auth[i].resource->type)==2)
		{
			sTemp.Format("has authoritative nameserver : %s",auth[i].rdata);
			sResult += sTemp;
		}
		sTemp.Format("\r\n");
		sResult += sTemp;
	}

	//���������Դ����
	for(i=0;i<ntohs(dns->add_count);i++)
	{
		sTemp.Format("Name : %s ",addit[i].name);
		sResult += sTemp;
		if(ntohs(addit[i].resource->type)==1)
		{
			long *p;
			p=(long*)addit[i].rdata;
			a.sin_addr.s_addr=(*p);    
			sTemp.Format("has \r\nIPv4 address :  %s",inet_ntoa(a.sin_addr));
			sResult += sTemp;
		}
		sTemp.Format("\r\n");
		sResult += sTemp;
	}

    return sResult;
}

unsigned char* CDNSQuery::ReadName(unsigned char* reader,unsigned char* buffer,int* count)
{
	unsigned char *name;
	unsigned int p=0,jumped=0,offset;
	int i , j;

	*count = 1;
	name   = (unsigned char*)malloc(256);
	
	name[0]='\0';

	//read the names in 3www6google3com format
	while(*reader!=0)
	{
		if(*reader>=192)
		{
			offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000  ;)
			reader = buffer + offset - 1;
			jumped = 1;  //we have jumped to another location so counting wont go up!
		}
		else 
			name[p++]=*reader;
		
		reader=reader+1;
		
		if(jumped==0) *count = *count + 1; //if we havent jumped to another location then we can count up
	}
	
	name[p]='\0';    //string complete
	if(jumped==1) *count = *count + 1;  //number of steps we actually moved forward in the packet
	
	//now convert 3www6google3com0 to www.google.com
	for(i=0;i<(int)strlen((const char*)name);i++)
	{
		p=name[i];
		for(j=0;j<(int)p;j++)
		{
			name[i]=name[i+1];
			i=i+1;
		}
		name[i]='.';
	}
	name[i-1]='\0';	  //remove the last dot
	return name;		
}


// ��ע������DNS������
void CDNSQuery::RetrieveDnsServersFromRegistry()
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
		
		if(err==ERROR_SUCCESS && strlen(name)>0) strcpy(m_DnsServers[dns_count++],name);
	}

	for(i=0;i<dns_count;i++)
	{
		for(j=0;j<(int)strlen(m_DnsServers[i]);j++)
		{
			if(m_DnsServers[i][j]==',' || m_DnsServers[i][j]==' ')
			{
				strcpy(m_DnsServers[dns_count++],m_DnsServers[i]+j+1);
				m_DnsServers[i][j]=0;
			}
		}
	}

	TRACE("\nThe following DNS Servers were found on your system...");
	for(i=0;i<dns_count;i++)
	{
		TRACE("\n%d)  %s",i+1,m_DnsServers[i]);
	}
}


//ת����ʽ�� www.google.com -> www google com ;
void CDNSQuery::ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host)
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
