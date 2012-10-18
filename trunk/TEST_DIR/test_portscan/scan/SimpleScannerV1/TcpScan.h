// TcpScan.h: interface for the CTcpScan class.
// author Asian Chai
// date   2005-5-22
// funtion, Other scanning method such as TCP syn Scan, ACK scan, Null Scan
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSCAN_H__9BBD6CDC_DBF6_4D8A_8D4F_486D28B7AD43__INCLUDED_)
#define AFX_TCPSCAN_H__9BBD6CDC_DBF6_4D8A_8D4F_486D28B7AD43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	IP_ALEN	4		/* IP address length in bytes (octets)	*/
typedef	unsigned long IPaddr;	/*  internet address			*/

/* Some Assigned Protocol Numbers */

#define	IPT_ICMP	1	/* protocol type for ICMP packets	*/
#define	IPT_IGMP	2	/* protocol type for IGMP packets	*/
#define	IPT_TCP		6	/* protocol type for TCP packets	*/
#define IPT_EGP		8	/* protocol type for EGP packets	*/
#define	IPT_UDP		17	/* protocol type for UDP packets	*/
#define	IPT_OSPF	89	/* protocol type for OSPF packets	*/

/*the struct
of the ip packet*/
typedef struct _ip
{
	unsigned char	ip_verlen;	/* IP version & header length (in longs)*/
	unsigned char	ip_tos;		/* type of service			*/
	unsigned short	ip_len;		/* total packet length (in octets)	*/
	short			ip_id;		/* datagram id				*/
	short 			ip_fragoff;	/* fragment offset (in 8-octet's)	*/
	unsigned char	ip_ttl;		/* time to live, in gateway hops	*/
	unsigned char	ip_proto;	/* IP protocol (see IPT_* above)	*/
	short			ip_cksum;	/* header checksum 			*/
	IPaddr			ip_src;		/* IP address of source			*/
	IPaddr			ip_dst;		/* IP address of destination		*/
	unsigned char	ip_data[1];	/* variable length data			*/
}
IP;

#define	IP_VERSION	4	/* current version value		*/
#define	IP_MINHLEN	5	/* minimum IP header length (in longs)	*/
//#define	IP_TTL		255	/* Initial time-to-live value		*/

#define	IP_MF		0x2000	/* more fragments bit			*/
#define	IP_DF		0x4000	/* don't fragment bit			*/
#define	IP_FRAGOFF	0x1fff	/* fragment offset mask			*/
#define	IP_PREC		0xe0	/* precedence portion of TOS		*/

/* IP Precedence values */

#define	IPP_NETCTL	0xe0	/* network control			*/
#define	IPP_INCTL	0xc0	/* internet control			*/
#define	IPP_CRIT	0xa0	/* critical				*/
#define	IPP_FLASHO	0x80	/* flash over-ride			*/
#define	IPP_FLASH	0x60	/* flash 				*/
#define	IPP_IMMED	0x40	/* immediate				*/
#define	IPP_PRIO	0x20	/* priority				*/
#define	IPP_NORMAL	0x00	/* normal				*/

/* macro to compute a datagram's header length (in bytes)		*/
#define	IP_HLEN(pip)	((pip->ip_verlen & 0xf)<<2)
#define	IPMHLEN		20	/* minimum IP header length (in bytes)	*/

/* IP options */
#define	IPO_COPY	0x80	/* copy on fragment mask		*/
#define IPO_CLASS	0x60	/* option class				*/
#define	IPO_NUM		0x17	/* option number			*/

#define	IPO_EOOP	0x00	/* end of options			*/
#define	IPO_NOP		0x01	/* no operation				*/
#define	IPO_SEC		0x82	/* DoD security/compartmentalization	*/
#define	IPO_LSRCRT	0x83	/* loose source routing			*/
#define	IPO_SSRCRT	0x89	/* strict source routing		*/
#define	IPO_RECRT	0x07	/* record route				*/
#define	IPO_STRID	0x88	/* stream ID				*/
#define	IPO_TIME	0x44	/* internet timestamp			*/

#define	IP_MAXLEN	BPMAXB-EP_HLEN	/* Maximum IP datagram length	*/

/* IP process info */

#define	IPSTK		512	/* stack size for IP process	*/
#define	IPPRI		100	/* IP runs at high priority		*/
#define	IPNAM		"ip"/* name of IP process			*/
#define	IPARGC		0	/* count of args to IP 			*/
#define MAX_PACKET_SIZE		4096
#define DATA_DIRECTION_IN	0
#define DATA_DIRECTION_OUT	1


#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)   //定义SIO_RCVALL

//定义RawSocket包结构，用于接收分析数据包
typedef enum _RAW_PACKET_TYPE
{
	PACKET_TYPE_SYN = 0,
		PACKET_TYPE_SYN_ACK,
		PACKET_TYPE_ACK,
		PACKET_TYPE_FIN,
		PACKET_TYPE_RST,
		PACKET_TYPE_DATA
}
RAW_PACKET_TYPE;

/* 

*/
typedef struct _raw_packet
{
	SYSTEMTIME time;
	int direction;
	RAW_PACKET_TYPE type;
	unsigned long packet_id;
	unsigned int size;
	unsigned char* buff;
}
RAW_PACKET;

extern IPaddr	ip_maskall;		/* = 255.255.255.255	*/
extern IPaddr	ip_anyaddr;		/* = 0.0.0.0			*/
extern IPaddr	ip_loopback;	/* = 127.0.0.1			*/

#include <winsock2.h>
#include <queue>
using namespace std;

typedef queue<unsigned char*> IP_PACKET_QUEUE;


class CTcpScan  
{
public:
	CTcpScan();
	virtual ~CTcpScan();


	// Implementions
public:
	
	BOOL SendIPFrame(TCP* tcp, LPTSTR szData, int dataLen);
	
private:
	int InitSendSock();
	int InitRecvSock();
	UINT InitWsaDll();
	
// Attributes
public:
  	BOOL CheckPortConnect(CString strHost,int port,int Scanflag);
	IPaddr	ip_local;
	IPaddr  ip_remote;
	

private:
	BOOL AnalyzeFlag(TCP *RecvTcpHead,int ScanFlag);
	SOCKET	m_sockSend;
	SOCKET	m_sockRecv;	

};

#endif // !defined(AFX_TCPSCAN_H__9BBD6CDC_DBF6_4D8A_8D4F_486D28B7AD43__INCLUDED_)
