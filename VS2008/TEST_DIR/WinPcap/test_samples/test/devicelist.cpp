#include "stdafx.h"
#include <pcap.h>
#pragma comment (lib, "wpcap.lib")

int _tmain(int argc, _TCHAR* argv[])
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i=0;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    /* Retrieve the device list */
    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }
    
    /* Print the list */
    for(d=alldevs;d;d=d->next)
    {
        printf("%d. %s\n", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else            printf(" (No description available)\n");
    }
    
    if(i==0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return 0;
    }

    /* We don't need any more the device list. Free it */
    pcap_freealldevs(alldevs);
	return 1;
}

