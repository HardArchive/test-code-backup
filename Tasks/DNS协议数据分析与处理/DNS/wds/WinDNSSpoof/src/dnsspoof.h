#ifndef _DNSSPOOF_H
#define _DNSSPOOF_H


/* Checksum stuff */
#define LIBNET_CKSUM_CARRY(x) \
    (x = (x >> 16) + (x & 0xffff), (~(x + (x >> 16)) & 0xffff))


/* Ethernet header */
#define ETH_HDRSIZE      sizeof(struct ether_header)
#define	ETH_TYPE_ARP     0x0806  /* Ether type ARP            */
#define	ETH_TYPE_IP      0x0800  /* Ether type IP             */
#define ETH_ADD_LEN      6       /* Ethernet address length   */
#define ETH_PADDING_ARP  18      /* 18 bytes ethernet padding */

/* IP header */
#define IP_HDRSIZE       sizeof(struct ip_header)
#define IP_PRO_ICMP      0x01    /* IP protocol ICMP          */
#define IP_PRO_UDP       0x11    /* IP protocol ICMP          */

/* UDP header */
#define UDP_HDRSIZE      sizeof(struct udp_header)

/* DNS header */
#define DNS_HDRSIZE      sizeof(struct dns_header)


struct ether_header {
  /* Ethernet header */
  unsigned char      eth_dst_addr[ETH_ADD_LEN]; /* Destination ethernet address	 */
  unsigned char      eth_src_addr[ETH_ADD_LEN]; /* Source ethernet address       */
  unsigned short int eth_type;                  /* EtherType                     */
};

struct ip_header {
  /* IP header */
  unsigned char      ip_hl:4,                   /* Header length                 */
                     ip_v:4;                    /* Version                       */                     
  unsigned char      ip_tos;                    /* Type of service               */
  unsigned short int ip_len;                    /* Total length                  */
  unsigned short int ip_id;                     /* Identification                */  
  unsigned short int ip_flags_off;              /* Flags & offset                */                     
  unsigned char      ip_ttl;                    /* Time to live                  */
  unsigned char      ip_p;                      /* Protocol                      */
  unsigned short int ip_sum;                    /* Checksum                      */
  unsigned char      ip_src[4];                 /* Source address                */
  unsigned char      ip_dst[4];                 /* Destination address           */   	
};

struct udp_header {
  /* UDP header */
  unsigned short int udp_src_port;              /* Source port                   */
  unsigned short int udp_dst_port;              /* Destination port              */
  unsigned short int udp_len;                   /* Header length                 */
  unsigned short int udp_sum;                   /* Checksum                      */
};

struct dns_header {
  /* DNS header */
  unsigned short int dns_id;                    /* Identification                */       
  unsigned char  dns_rd:1;                      /* Recursion desired             */
  unsigned char  dns_tc:1;                      /* Truncation                    */
  unsigned char  dns_aa:1;                      /* Authoritative answer          */
  unsigned char  dns_op:4;                      /* Kind of query in the message  */
  unsigned char  dns_qr:1;                      /* Query/Response flag           */    
  unsigned char  dns_rc:4;                      /* Response code                 */
  unsigned char  dns_z:3;                       /* Reserved for future use       */
  unsigned char  dns_ra:1;                      /* Recursion available           */
  unsigned short int dns_qdc;                   /* Number of entries in question */
  unsigned short int dns_anc;                   /* Number of rr in answer        */
  unsigned short int dns_nsc;                   /* Number of rr in authority     */
  unsigned short int dns_arc;                   /* Number of rr in additional    */
};


/* Prototypes */
void usage(char *name);
int read_dns(LPPACKET lpPacket, unsigned char *etherdst, unsigned char *ethersrc,
			 unsigned char *ipsrc, unsigned char *ipdst,
			 unsigned short int *udp_src_port, unsigned short int *dns_id, 
			 unsigned char **dns_data, char *dnsname, int *datalen, int verbose);
void dns_extract_query(char *dns_query, unsigned char *dns_data);
void dns_build(unsigned char *packet, unsigned char *dst_etheraddr, 
			   unsigned char *src_etheraddr, unsigned char *ip_dst_addr, 
			   unsigned char *ip_src_addr, unsigned short int udp_dst_port, 
			   unsigned short int dns_id, unsigned char *spoofip, 
			   unsigned char *dns_data, int datalen);
int libnet_in_cksum(u_short *addr, int len);


#endif /* _DNSSPOOF_H */