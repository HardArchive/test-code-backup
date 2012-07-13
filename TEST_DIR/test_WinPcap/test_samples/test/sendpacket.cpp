#include <stdlib.h>
#include <stdio.h>

#include "stdafx.h"
#include <pcap.h>
#pragma comment (lib, "wpcap.lib")
//3. sendpacket.c 发送自己填充的一个数据包 
void usage();

int main(int argc, char **argv) 
{
	pcap_t *fp;
	char error[PCAP_ERRBUF_SIZE];
	u_char packet[100] = {0};
	int i = 0;

	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* 获得设备列表 */
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* 打印列表 */
	for(d=alldevs; d; d=d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):",i);
	scanf("%d", &inum);

	if(inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* 释放设备列表 */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* 跳转到已选设备 */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);

	
	argc= 2;
	/* Check the validity of the command line */
	if (argc != 2)
	{
		printf("usage: %s inerface\n", d->name);
		return -1;
	}

	/* Open the output adapter */
	if((fp = pcap_open_live(d->name, 100, 1, 1000, error) ) == NULL)
	{
		fprintf(stderr,"\nError opening adapter: %s\n", error);
		return -1;
	}

	/* Supposing to be on ethernet, set mac destination to 1:1:1:1:1:1 */
	packet[0]=1;
	packet[1]=1;
	packet[2]=1;
	packet[3]=1;
	packet[4]=1;
	packet[5]=1;

	/* set mac source to 2:2:2:2:2:2 */
	packet[6]=2;
	packet[7]=2;
	packet[8]=2;
	packet[9]=2;
	packet[10]=2;
	packet[11]=2;

	/* Fill the rest of the packet */
	for(i=12;i<100;i++){
		packet[i]=i%256;
	}

	/* Send down the packet */
	pcap_sendpacket(fp,
		packet,
		100);

	return 1;
}

