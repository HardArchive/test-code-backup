#include <stdio.h>
#include "pcap.h"
#pragma comment(lib,"ws2_32.lib")
#define MAX_PROTO_TEXT_LEN 16 // ��Э��������󳤶�
#define MAX_PROTO_NUM 12 // ��Э������
// ����IP��ַ�ṹ
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

// ����IP�ײ���ʽ
typedef struct ip_header
{
	u_char	ver_ihl;		// �汾���ײ�����
	u_char	tos;			// �������� 
	u_short tlen;			// �ܳ��� 
	u_short identification; // ��ʶ��
	u_short flags_fo;		// ��ƫ����
	u_char	ttl;			// ����ʱ��
	u_char	proto;			// Э��
	u_short crc;			// �ײ�У���
	ip_address	saddr;		// ԴIP��ַ
	ip_address	daddr;		// Ŀ�ĵ�ַ
	u_int	op_pad;			// ѡ����λ
}ip_header;

// ����TCP�ײ���ʽ
typedef struct tcp_header 
{ 
    unsigned short th_sport;  // Դ�˿ں�        
    unsigned short th_dport;  // Ŀ�Ķ˿ں�       
    unsigned int th_seq;  // SEQ���    
    unsigned int th_ack;  // ACK���
    unsigned char th_lenres;  //  �ײ�����    
    unsigned char th_flag;   // ����λ  
    unsigned short th_win;  // ���ڴ�С        
    unsigned short th_sum;  // У���        
    unsigned short th_urp;  // ����ָ��         
}tcp_header;

// ����UDP�ײ���ʽ
typedef struct udp_header
{
	u_short sport;			// 16λԴ�˿�
	u_short dport;			// 16λĿ�Ķ˿�
	u_short len;			// 16λ����
	u_short crc;			// 16λУ���
}udp_header;

// ����ICMP�ײ���ʽ
typedef struct icmp_header
{
	BYTE i_type; // 8λ����
	BYTE i_code; // 8λ����
	unsigned short i_cksum; // 16λУ���
	unsigned short i_id; // ʶ���
	unsigned short i_seq; // �������к�
	unsigned long timestamp; // ʱ���
}icmp_header;

// ������Э��ӳ���
typedef struct _protomap 
{
	int ProtoNum;
	char ProtoText[MAX_PROTO_TEXT_LEN];
}PROTOMAP;

// Ϊ��Э��ӳ���ֵ
PROTOMAP ProtoMap[MAX_PROTO_NUM]={
	{IPPROTO_IP,"IP"},
	{IPPROTO_ICMP,"ICMP"},
	{IPPROTO_IGMP,"IGMP"},
	{IPPROTO_GGP,"GGP"},
	{IPPROTO_TCP,"TCP"},
	{IPPROTO_PUP,"PUP"},
	{IPPROTO_UDP,"UDP"},
	{IPPROTO_IDP,"IDP"},
	{IPPROTO_ND,"ND"},
	{IPPROTO_RAW,"RAW"},
	{IPPROTO_MAX,"MAX"},
	{NULL,""}
};


char TcpFlag[6]={'F','S','R','P','A','U'}; // TCP��־λ



// ��һ��unsigned long �͵�IPת��Ϊ�ַ������͵�IP
#define IPTOSBUFFERS 12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;
	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	// ��ʽ���ַ���
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
} 

// ���������Ϣ
void ifprint(pcap_if_t *d, int num)
{
	pcap_addr_t *a;
	
	printf("\n\n************����%d��Ϣ************\n",num);
	// �����������
	printf("������  : %s \n",d->name);
	
	// ����������Ϣ
	if (d->description)
	{
		printf("��������: %s \n",d->description);
	}
	
	// ����
	printf("����    : %s \n",(d->flags & PCAP_IF_LOOPBACK)?"yes":"no");
	
	// IP��ַ
	for(a=d->addresses;a;a=a->next) 
	{
		switch(a->addr->sa_family)
		{
		case AF_INET:
			printf("IP��ַ����: AF_INET\n");//��ӡ�����ַ����
			if (a->addr)//��ӡIP��ַ
				printf("IP��ַ  : %s\n",
				iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->netmask)//��ӡ����
				printf("����    : %s\n",
				iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			if (a->broadaddr)//��ӡ�㲥��ַ
				printf("�㲥��ַ: %s\n",
				iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			if (a->dstaddr)//Ŀ�ĵ�ַ
				printf("Destination Address: %s\n",
				iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
			break;
		default:
			printf("Address Family Name: Unknown\n");
			break;
		}
	}
}

// Э��ʶ����
char *check_protocol(int iProtocol)
{
	for(int i=0; i<MAX_PROTO_NUM;i++)
	{
		// ����ҵ���Ӧ����Э�飬�򷵻�����
		if(ProtoMap[i].ProtoNum == iProtocol)
		{
			return ProtoMap[i].ProtoText;
		}
	}
	return "";
}

// TCP�������
int decode_tcp(char *tcpbuf)
{
	tcp_header *ptcpheader;
	int i;
	// ת����TCP�ײ���ʽ
	ptcpheader = (tcp_header*)tcpbuf;
	// ���Դ�˿ں�Ŀ�Ķ˿�
	printf("  Port : %d-->%d  ",ntohs(ptcpheader->th_sport),ntohs(ptcpheader->th_dport));
	unsigned char FlagMask = 1;
	// �����־λ
	for(i=0;i<6;i++)
	{
		if((ptcpheader->th_flag) & FlagMask)
		{
			printf("%c",TcpFlag[i]);
		}
		else
		{
			printf("-");
		}
		FlagMask = FlagMask<<1;
	}
	return true;
}

// UDP �������
int decode_udp(char *udpbuf)
{
	udp_header *pudpheader;
	pudpheader = (udp_header *)udpbuf;
	// ����˿ں����ݳ���
	printf("  Port : %d-->%d",ntohs(pudpheader->sport),ntohs(pudpheader->dport));
	return true;
}

// ICMP �������
int decode_icmp(char *icmpbuf)
{
	icmp_header *picmpheader;
	picmpheader = (icmp_header *)icmpbuf;
	// ���ICMP���ݰ����͡�ID��SEQ
	printf("  Type : %d,%d",picmpheader->i_type,picmpheader->i_code);
	printf("  ID = %d SEQ = %d",picmpheader->i_id,picmpheader->i_seq);
	return true;
}

// IP�������
int decode_ip(char *ipbuf, int protocol)
{
	switch(protocol)
	{
	case IPPROTO_TCP: // TCP�������ݰ�
		decode_tcp(ipbuf);
		break;
	case IPPROTO_UDP: // UDP�������ݰ�
		decode_udp(ipbuf);
		break;
	case IPPROTO_ICMP: // ICMP�������ݰ�
		decode_icmp(ipbuf);
		break;
	default:
		break;
	}
	return true;
}
// ������ص�����������ÿ����̽�������ݰ�
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header *ih;
	u_int ip_len;

	// ����IP�ײ���λ��
	ih = (ip_header *) (pkt_data +
		14); //��̫�����ײ�������14

	// IP�ײ�����
	ip_len = (ih->ver_ihl & 0xf) * 4;

	printf("%s  ",check_protocol(ih->proto));

	// ���Դ��ַIP��Ŀ�ĵ�ַIP
	printf("%d.%d.%d.%d -> %d.%d.%d.%d  ",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4
		);

	decode_ip((char*)(pkt_data+14+ip_len),ih->proto);
	printf("\n");
}


int main()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[20];
	struct bpf_program fcode;
	
	// ��ȡ�����б�
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"pcap_findalldevs��������: %s\n", errbuf);
		exit(1);
	}	
	// ���������Ϣ
	for(i=0,d=alldevs; d; d=d->next,i++)
	{
		ifprint(d,i+1);
		
	}
	if(i==0)
	{
		printf("\nû���ҵ��κ���������ȷ��Winpcap�Ѿ���װ.\n");
		return -1;
	}
	
	printf("\n\n������������� (1-%d):",i);
	scanf("%d", &inum);
	
	// ����û��Ƿ�ָ������Ч����
	if(inum < 1 || inum > i)
	{
		printf("\n������ų�����Χ.\n");
		
		// �ͷ��豸�б�
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	// ��ת��ѡ�������
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
	// �������豸
	if ((adhandle= pcap_open_live(d->name,	// �豸����
							 65536,			// ����ȫ�������ݰ� 
							 1,				// ��������Ϊ����ģʽ
							 1000,			// ����ʱΪ1��
							 errbuf			// ���󻺴�
							 )) == NULL)
	{
		fprintf(stderr,"\n���ܴ�����. %s ����Winpcap֧��\n");
		// �ͷ��豸�б�
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	// ������Ӳ㣬ֻ֧����̫��ģʽ
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\n�˳���ֻ����������̫����.\n");
		// �ͷ��豸�б�
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if(d->addresses != NULL)
	{   // ���ؽӿڵĵ�һ����ַ������
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{   // ���û�����룬��Ĭ������ΪC��
		netmask=0xffffff; 
	}
	
	// ѡ����˰�����
	int protocol_type; // 0->ip 1->tcp 2->udp 3->icmp
	printf("\n��ѡ����������ݰ�Э������(0->ip 1->tcp 2->udp 3->icmp) : ");
	scanf("%d",&protocol_type);
	switch(protocol_type)
	{
	case 0:
		strcpy(packet_filter,"ip");
		break;
	case 1:
		strcpy(packet_filter,"ip and tcp");
		break;
	case 2:
		strcpy(packet_filter,"ip and udp");
		break;
	case 3:
		strcpy(packet_filter,"ip and icmp");
		break;
	default:
		break;
	}
	// ���������
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\n���ܱ��������. �����﷨.\n");
		// �ͷ��豸�б�
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	//���ù�����
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\n���ù���������.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	printf("\n������ %s ��ʼ����...\n", d->description);
	
	// ������Ҫ�κ��豸�б������ͷ�
	pcap_freealldevs(alldevs);
	
	// ��ʼ��̽
	pcap_loop(adhandle, 0, packet_handler, NULL);
	
	return 0;
}

