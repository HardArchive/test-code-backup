#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

#include <pcap.h>
#pragma comment (lib, "wpcap.lib")
//4. statics.c �������е����ݽ���ͳ�� 
void usage();

void dispatcher_handler(u_char *, 
    const struct pcap_pkthdr *, const u_char *);


int main(int argc, char **argv) 
{
    pcap_t *fp;
    char error[PCAP_ERRBUF_SIZE];
    struct timeval st_ts;
    u_int netmask;
    struct bpf_program fcode;



	int i = 0;
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* ����豸�б� */
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* ��ӡ�б� */
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
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* ��ת����ѡ�豸 */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);


  
	argc= 2;
    /* Check the validity of the command line */
    if (argc != 2)
    {
        usage();
        return -1;
    }
        
    /* Open the output adapter */
    if((fp = pcap_open_live(d->name, 100, 1, 1000, error) ) == NULL)
    {
        fprintf(stderr,"\nError opening adapter: %s\n", error);
        return -1;
    }

    /* Don't care about netmask, it won't be used for this filter */
    netmask=0xffffff; 

    //compile the filter
    if(pcap_compile(fp, &fcode, "tcp", 1, netmask) <0 ){
        fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
        /* Free the device list */
        return -1;
    }
    
    //set the filter
    if(pcap_setfilter(fp, &fcode)<0){
        fprintf(stderr,"\nError setting the filter.\n");
        /* Free the device list */
        return -1;
    }

    /* Put the interface in statstics mode */
    pcap_setmode(fp, MODE_STAT);

    printf("TCP traffic summary:\n");

    /* Start the main loop */
    pcap_loop(fp, 0, dispatcher_handler, (PUCHAR)&st_ts);

    return 1;
}

void dispatcher_handler(u_char *state, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    struct timeval *old_ts = (struct timeval *)state;
    u_int delay;
    LARGE_INTEGER Bps,Pps;
    struct tm *ltime;
    char timestr[16];

    /* Calculate the delay in microseconds from the last sample. */
    /* This value is obtained from the timestamp that the associated with the sample. */
    delay=(header->ts.tv_sec - old_ts->tv_sec) * 1000000 - old_ts->tv_usec + header->ts.tv_usec;
    /* Get the number of Bits per second */
    Bps.QuadPart=(((*(LONGLONG*)(pkt_data + 8)) * 8 * 1000000) / (delay));
    /*                                            ^      ^
                                                  |      |
                                                  |      | 
                                                  |      |
                         converts bytes in bits --       |
                                                         |
                    delay is expressed in microseconds --
    */

    /* Get the number of Packets per second */
    Pps.QuadPart=(((*(LONGLONG*)(pkt_data)) * 1000000) / (delay));

	time_t local_tv_sec;

	/* ��ʱ���ת���ɿ�ʶ��ĸ�ʽ */
	local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

    /* Print timestamp*/
    printf("%s ", timestr);

    /* Print the samples */
    printf("BPS=%I64u ", Bps.QuadPart);
    printf("PPS=%I64u\n", Pps.QuadPart);

    //store current timestamp
    old_ts->tv_sec=header->ts.tv_sec;
    old_ts->tv_usec=header->ts.tv_usec;
}


void usage()
{
    
    printf("\nShows the TCP traffic load, in bits per second and packets per second.\nCopyright (C) 2002 Loris Degioanni.\n");
    printf("\nUsage:\n");
    printf("\t tcptop adapter\n");
    printf("\t You can use \"WinDump -D\" if you don't know the name of your adapters.\n");

    exit(0);
}
