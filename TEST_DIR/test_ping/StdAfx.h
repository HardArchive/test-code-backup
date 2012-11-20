// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__489DB0B6_AAB2_4E33_9394_5FCA4872D2CA__INCLUDED_)
#define AFX_STDAFX_H__489DB0B6_AAB2_4E33_9394_5FCA4872D2CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"ws2_32.lib")

#define IP_RECORD_ROUTE  0x7
#define DEF_PACKET_SIZE  32                        //缺省包长度
#define MAX_PACKET       1024      // 最大ICMP包长度
#define MAX_IP_HDR_SIZE  60        // 最大IP头长度
#define ICMP_ECHO        8                        //ICMP类型，8即响应请求
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8                        //最小8字节报头

typedef struct _iphdr //IP头结构
{
    unsigned int   h_len:4;        // 头长度
    unsigned int   version:4;      // IP版本
    unsigned char  tos;            // 服务类型
    unsigned short total_len;      // 包的总长度
    unsigned short ident;          // 包标识身份
    unsigned short frag_and_flags; // 标志
    unsigned char  ttl;            // 包生命周期
    unsigned char  proto;          // 协议类型(TCP, UDP 等)
    unsigned short checksum;       // IP校验和
       
    unsigned int   sourceIP;                //源IP
    unsigned int   destIP;                        //目标IP
} IpHeader;


typedef struct _icmphdr //ICMP头结构
{
    BYTE   i_type;                        //ICMP类型(8位)
    BYTE   i_code;          // ICMP代码(8位)
    USHORT i_cksum;                        //ICMP校验和(16位)
    USHORT i_id;                        //标识符
    USHORT i_seq;                        //序列号
        // 下面就不是标准头的部分，是用于时间戳的保留字段
    ULONG  timestamp;                //时间戳
} IcmpHeader;

//
//IP头选项结构，当socket可选项设置为IP_OPTIONS时使用该结构
typedef struct _ipoptionhdr
{
    unsigned char        code;        // 可选类型
    unsigned char        len;         // 头长度
    unsigned char        ptr;         // 指向第一个地址的偏移
    unsigned long        addr;        // IP地址列表
} IpOptionHeader;

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__489DB0B6_AAB2_4E33_9394_5FCA4872D2CA__INCLUDED_)
