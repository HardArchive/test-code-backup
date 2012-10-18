#include <stdio.h>
#include "pcap.h"
#pragma comment(lib,"ws2_32.lib")
#define MAX_PROTO_TEXT_LEN 16 // 子协议名称最大长度
#define MAX_PROTO_NUM 12 // 子协议数量
// 定义IP地址结构
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

// 定义IP首部格式
typedef struct ip_header
{
	u_char	ver_ihl;		// 版本和首部长度
	u_char	tos;			// 服务类型 
	u_short tlen;			// 总长度 
	u_short identification; // 标识号
	u_short flags_fo;		// 段偏移量
	u_char	ttl;			// 生存时间
	u_char	proto;			// 协议
	u_short crc;			// 首部校验和
	ip_address	saddr;		// 源IP地址
	ip_address	daddr;		// 目的地址
	u_int	op_pad;			// 选项和填补位
}ip_header;

// 定义TCP首部格式
typedef struct tcp_header 
{ 
    unsigned short th_sport;  // 源端口号        
    unsigned short th_dport;  // 目的端口号       
    unsigned int th_seq;  // SEQ序号    
    unsigned int th_ack;  // ACK序号
    unsigned char th_lenres;  //  首部长度    
    unsigned char th_flag;   // 控制位  
    unsigned short th_win;  // 窗口大小        
    unsigned short th_sum;  // 校验和        
    unsigned short th_urp;  // 紧急指针         
}tcp_header;

// 定义UDP首部格式
typedef struct udp_header
{
	u_short sport;			// 16位源端口
	u_short dport;			// 16位目的端口
	u_short len;			// 16位长度
	u_short crc;			// 16位校验和
}udp_header;

// 定义ICMP首部格式
typedef struct icmp_header
{
	BYTE i_type; // 8位类型
	BYTE i_code; // 8位代码
	unsigned short i_cksum; // 16位校验和
	unsigned short i_id; // 识别号
	unsigned short i_seq; // 报文序列号
	unsigned long timestamp; // 时间戳
}icmp_header;

// 定义子协议映射表
typedef struct _protomap 
{
	int ProtoNum;
	char ProtoText[MAX_PROTO_TEXT_LEN];
}PROTOMAP;

// 为子协议映射表赋值
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


char TcpFlag[6]={'F','S','R','P','A','U'}; // TCP标志位



// 将一个unsigned long 型的IP转换为字符串类型的IP
#define IPTOSBUFFERS 12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;
	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	// 格式化字符串
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
} 

// 输出网卡信息
void ifprint(pcap_if_t *d, int num)
{
	pcap_addr_t *a;
	
	printf("\n\n************网卡%d信息************\n",num);
	// 输出网卡名称
	printf("网卡名  : %s \n",d->name);
	
	// 网卡描述信息
	if (d->description)
	{
		printf("网卡描述: %s \n",d->description);
	}
	
	// 反馈
	printf("反馈    : %s \n",(d->flags & PCAP_IF_LOOPBACK)?"yes":"no");
	
	// IP地址
	for(a=d->addresses;a;a=a->next) 
	{
		switch(a->addr->sa_family)
		{
		case AF_INET:
			printf("IP地址类型: AF_INET\n");//打印网络地址类型
			if (a->addr)//打印IP地址
				printf("IP地址  : %s\n",
				iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->netmask)//打印掩码
				printf("掩码    : %s\n",
				iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			if (a->broadaddr)//打印广播地址
				printf("广播地址: %s\n",
				iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			if (a->dstaddr)//目的地址
				printf("Destination Address: %s\n",
				iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
			break;
		default:
			printf("Address Family Name: Unknown\n");
			break;
		}
	}
}

// 协议识别函数
char *check_protocol(int iProtocol)
{
	for(int i=0; i<MAX_PROTO_NUM;i++)
	{
		// 如果找到对应的子协议，则返回名称
		if(ProtoMap[i].ProtoNum == iProtocol)
		{
			return ProtoMap[i].ProtoText;
		}
	}
	return "";
}

// TCP解包函数
int decode_tcp(char *tcpbuf)
{
	tcp_header *ptcpheader;
	int i;
	// 转换成TCP首部格式
	ptcpheader = (tcp_header*)tcpbuf;
	// 输出源端口和目的端口
	printf("  Port : %d-->%d  ",ntohs(ptcpheader->th_sport),ntohs(ptcpheader->th_dport));
	unsigned char FlagMask = 1;
	// 输出标志位
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

// UDP 解包函数
int decode_udp(char *udpbuf)
{
	udp_header *pudpheader;
	pudpheader = (udp_header *)udpbuf;
	// 输出端口和数据长度
	printf("  Port : %d-->%d",ntohs(pudpheader->sport),ntohs(pudpheader->dport));
	return true;
}

// ICMP 解包函数
int decode_icmp(char *icmpbuf)
{
	icmp_header *picmpheader;
	picmpheader = (icmp_header *)icmpbuf;
	// 输出ICMP数据包类型、ID和SEQ
	printf("  Type : %d,%d",picmpheader->i_type,picmpheader->i_code);
	printf("  ID = %d SEQ = %d",picmpheader->i_id,picmpheader->i_seq);
	return true;
}

// IP解包函数
int decode_ip(char *ipbuf, int protocol)
{
	switch(protocol)
	{
	case IPPROTO_TCP: // TCP类型数据包
		decode_tcp(ipbuf);
		break;
	case IPPROTO_UDP: // UDP类型数据包
		decode_udp(ipbuf);
		break;
	case IPPROTO_ICMP: // ICMP类型数据包
		decode_icmp(ipbuf);
		break;
	default:
		break;
	}
	return true;
}
// 包处理回调函数，对于每个嗅探到的数据包
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header *ih;
	u_int ip_len;

	// 返回IP首部的位置
	ih = (ip_header *) (pkt_data +
		14); //以太网的首部长度是14

	// IP首部长度
	ip_len = (ih->ver_ihl & 0xf) * 4;

	printf("%s  ",check_protocol(ih->proto));

	// 输出源地址IP和目的地址IP
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
	
	// 获取网卡列表
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"pcap_findalldevs发生错误: %s\n", errbuf);
		exit(1);
	}	
	// 输出网卡信息
	for(i=0,d=alldevs; d; d=d->next,i++)
	{
		ifprint(d,i+1);
		
	}
	if(i==0)
	{
		printf("\n没有找到任何网卡，请确认Winpcap已经安装.\n");
		return -1;
	}
	
	printf("\n\n请输入网卡编号 (1-%d):",i);
	scanf("%d", &inum);
	
	// 检测用户是否指定了有效网卡
	if(inum < 1 || inum > i)
	{
		printf("\n网卡编号超出范围.\n");
		
		// 释放设备列表
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	// 跳转到选择的网卡
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
	// 打开网卡设备
	if ((adhandle= pcap_open_live(d->name,	// 设备名称
							 65536,			// 捕获全部的数据包 
							 1,				// 设置网卡为混杂模式
							 1000,			// 读超时为1秒
							 errbuf			// 错误缓存
							 )) == NULL)
	{
		fprintf(stderr,"\n不能打开网卡. %s 不被Winpcap支持\n");
		// 释放设备列表
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	// 检测链接层，只支持以太网模式
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\n此程序只能运行在以太网上.\n");
		// 释放设备列表
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if(d->addresses != NULL)
	{   // 返回接口的第一个地址的掩码
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{   // 如果没有掩码，则默认设置为C类
		netmask=0xffffff; 
	}
	
	// 选择过滤包类型
	int protocol_type; // 0->ip 1->tcp 2->udp 3->icmp
	printf("\n请选择监听的数据包协议类型(0->ip 1->tcp 2->udp 3->icmp) : ");
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
	// 编译过滤器
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\n不能编译过滤器. 请检测语法.\n");
		// 释放设备列表
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	//设置过滤器
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\n设置过滤器出错.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	printf("\n在网卡 %s 开始监听...\n", d->description);
	
	// 不再需要任何设备列表，进行释放
	pcap_freealldevs(alldevs);
	
	// 开始嗅探
	pcap_loop(adhandle, 0, packet_handler, NULL);
	
	return 0;
}

