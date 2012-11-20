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
#define DEF_PACKET_SIZE  32                        //ȱʡ������
#define MAX_PACKET       1024      // ���ICMP������
#define MAX_IP_HDR_SIZE  60        // ���IPͷ����
#define ICMP_ECHO        8                        //ICMP���ͣ�8����Ӧ����
#define ICMP_ECHOREPLY   0
#define ICMP_MIN         8                        //��С8�ֽڱ�ͷ

typedef struct _iphdr //IPͷ�ṹ
{
    unsigned int   h_len:4;        // ͷ����
    unsigned int   version:4;      // IP�汾
    unsigned char  tos;            // ��������
    unsigned short total_len;      // �����ܳ���
    unsigned short ident;          // ����ʶ���
    unsigned short frag_and_flags; // ��־
    unsigned char  ttl;            // ����������
    unsigned char  proto;          // Э������(TCP, UDP ��)
    unsigned short checksum;       // IPУ���
       
    unsigned int   sourceIP;                //ԴIP
    unsigned int   destIP;                        //Ŀ��IP
} IpHeader;


typedef struct _icmphdr //ICMPͷ�ṹ
{
    BYTE   i_type;                        //ICMP����(8λ)
    BYTE   i_code;          // ICMP����(8λ)
    USHORT i_cksum;                        //ICMPУ���(16λ)
    USHORT i_id;                        //��ʶ��
    USHORT i_seq;                        //���к�
        // ����Ͳ��Ǳ�׼ͷ�Ĳ��֣�������ʱ����ı����ֶ�
    ULONG  timestamp;                //ʱ���
} IcmpHeader;

//
//IPͷѡ��ṹ����socket��ѡ������ΪIP_OPTIONSʱʹ�øýṹ
typedef struct _ipoptionhdr
{
    unsigned char        code;        // ��ѡ����
    unsigned char        len;         // ͷ����
    unsigned char        ptr;         // ָ���һ����ַ��ƫ��
    unsigned long        addr;        // IP��ַ�б�
} IpOptionHeader;

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__489DB0B6_AAB2_4E33_9394_5FCA4872D2CA__INCLUDED_)
