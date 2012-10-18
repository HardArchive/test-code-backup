// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__46F49764_3525_4039_8438_39DED3A51009__INCLUDED_)
#define AFX_STDAFX_H__46F49764_3525_4039_8438_39DED3A51009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  ScanDebug true
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxsock.h>		// MFC socket extensions
//#include <Icmpapi.h>        //The ICMP api header file 
#include <WINSOCK2.H>
#include "ws2tcpip.h"
#include "afxmt.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__46F49764_3525_4039_8438_39DED3A51009__INCLUDED_)


// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, *PIPHDR;

//TCP Header 
typedef	long	tcpseq;
typedef tcpseq	TCPSEQ;

typedef struct _tcp 
{
	unsigned short	tcp_sport;	/* source port		*/
	unsigned short	tcp_dport;	/* destination port	*/
	TCPSEQ			tcp_seq;	/* sequence			*/
	TCPSEQ			tcp_ack;	/* acknowledged sequence*/
	unsigned char	tcp_offset;
	unsigned char	tcp_code;	/* control flags		*/
	unsigned short	tcp_window;	/* window advertisement	*/
	unsigned short	tcp_cksum;	/* check sum			*/
	unsigned short	tcp_urgptr;	/* urgent pointer		*/
	unsigned char	tcp_data[1];
}
TCP;

/* TCP Control Bits */

#define	TCPF_URG	0x20	/* urgent pointer is valid			*/
#define	TCPF_ACK	0x10	/* acknowledgement field is valid	*/
#define	TCPF_PSH	0x08	/* this segment requests a push		*/
#define	TCPF_RST	0x04	/* reset the connection				*/
#define	TCPF_SYN	0x02	/* synchronize sequence numbers		*/
#define	TCPF_FIN	0x01	/* sender has reached end of its stream	*/

#define	TCPMHLEN	  20	/* minimum TCP header length		*/
#define	TCPHOFFSET	0x50	/* tcp_offset value for TCPMHLEN	*/
#define	TCP_HLEN(ptcp)		(((ptcp)->tcp_offset & 0xf0)>>2)
#define TCP_IF_URG(ptcp)	((ptcp)->tcp_code & TCPF_URG)
#define TCP_IF_ACK(ptcp)	((ptcp)->tcp_code & TCPF_ACK)
#define TCP_IF_PSH(ptcp)	((ptcp)->tcp_code & TCPF_PSH)
#define TCP_IF_RST(ptcp)	((ptcp)->tcp_code & TCPF_RST)
#define TCP_IF_SYN(ptcp)	((ptcp)->tcp_code & TCPF_SYN)
#define TCP_IF_FIN(ptcp)	((ptcp)->tcp_code & TCPF_FIN)

/* TCP Options */

#define	TPO_EOOL	0	/* end Of Option List		*/
#define	TPO_NOOP	1	/* no Operation				*/
#define	TPO_MSS		2	/* maximum Segment Size		*/



// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
	char	Data;			// Data
}ICMPHDR, *PICMPHDR;


#define REQ_DATASIZE 32		// Echo Request Data size
#define ICMP_ECHOREQ	8

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;



//////////////////////////////////////////////////////////////////////////////////
//                  The golbal fuctions  declare                                        //                   //
//////////////////////////////////////////////////////////////////////////////////

//CheckSum: 计算校验和的子函数 
USHORT checksum(USHORT *buffer, int size);

//获得机器地址
LPSTR GetLocalIpAddr(LPSTR ip);


