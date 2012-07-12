// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <pcap.h>
//#include <remote-ext.h>
#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "wsock32.lib")
//int _tmain(int argc, _TCHAR* argv[])
//{
//	pcap_if_t *alldevs;
//	pcap_if_t *d;
//	int i=0;
//	char errbuf[PCAP_ERRBUF_SIZE];
//	/* 取得列表 */
//	if  (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL  /* auth is not needed 
//													   */, &alldevs, errbuf) == -1) 
//	//if (pcap_findalldevs(&alldevs, errbuf) == -1)
//	{
//		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
//		exit(1);
//	}
//	/* 输出列表 */
//	for(d=alldevs;d;d=d->next)
//	{
//		printf("设备名称：%d、%s\n", ++i, d->name);
//		if (d->description)
//			printf("设备描述：(%s)\n", d->description);
//		else
//			/* Y- 没有有效的描述 */
//			printf(" (No description available)\n");
//	}
//	if(i==0)
//	{
//		/* Y- 没有有效的接口 , 可能是因为没有安装 WinPcap */
//		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
//		return 0;
//	}
//	/* 我们不再需要列表了 , 释放 */
//	pcap_freealldevs(alldevs);
//
//	getchar();
//	return 1;
//}




#include "pcap.h" 
#ifndef WIN32 
#include <sys/socket.h> 
#include <netinet/in.h> 
#else 
#include <winsock.h> 
#endif 
void ifprint(pcap_if_t *d); 
char *iptos(u_long in); 
int main() 
{ 
	pcap_if_t *alldevs; 
	pcap_if_t *d; 
	char errbuf[PCAP_ERRBUF_SIZE+1]; 
	/* 取得列表 */ 
	if (pcap_findalldevs(&alldevs, errbuf) == -1) 
	{ 
		fprintf(stderr,"Error in pcap_findalldevs: %s\n",errbuf); 
		exit(1); 
	} 
	/* Scan the list printing every entry */ 
	for(d=alldevs;d;d=d->next) 
	{ 
		ifprint(d); 
	} 
	getchar();
	return 1; 
} 
/* 输出列表 */ 
void ifprint(pcap_if_t *d) 
{ 
	pcap_addr_t *a; 
	/* Name */ 
	printf("设备名称：%s\n",d->name); 
	/* Description */ 
	if (d->description) 
		printf("Description(设备描述): %s\n",d->description); 
	/* Loopback Address*/ 
	printf("\tLoopback: %s\n",(d->flags & PCAP_IF_LOOPBACK)?"yes":"no"); 
	/* IP addresses */ 
	for(a=d->addresses;a;a=a->next) 
	{ 
		printf("\tAddress Family: #%d\n",a->addr->sa_family); 
		switch(a->addr->sa_family) 
		{ 
		case AF_INET: 
			printf("\tAddress Family Name: AF_INET\n"); 
			if (a->addr) 
				printf("\tAddress(IP): %s\n",iptos(((struct sockaddr_in*)a->addr)->sin_addr.s_addr)); 
			if (a->netmask) 
				printf("\tNetmask(掩码): %s\n",iptos(((struct sockaddr_in*)a->netmask)->sin_addr.s_addr)); 
			if (a->broadaddr) 
				printf("\tBroadcast Address(广播地址): %s\n",iptos(((struct sockaddr_in*)a->broadaddr)->sin_addr.s_addr)); 
			if (a->dstaddr) 
				printf("\tDestination Address(目标地址): %s\n",iptos(((struct sockaddr_in*)a->dstaddr)->sin_addr.s_addr)); 
			break; 
		default: 
			printf("\tAddress Family Name: Unknown\n"); 
			break; 
		} 
	} 
	printf("\n"); 
} 
//将数字类型的IP地址转换成字符串类型的 
#define IPTOSBUFFERS    12 
char *iptos(u_long in) 
{ 
	static char output[IPTOSBUFFERS][3*4+3+1]; 
	static short which; 
	u_char *p; 
	p = (u_char *)&in; 
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1); 
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]); 
	return output[which]; 
} 