#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#include "stdafx.h"
#include <Windows.h>
#include <lm.h>
#include <assert.h>

#pragma comment(lib, "Netapi32.lib")
#pragma comment( lib, "Ws2_32.lib")
#pragma comment( lib, "wpcap.lib")


//��̫���ײ�  14=6+6+2
struct ethernet_head
{
	unsigned char dest_mac[6];   //dest MAC  ��̫��Ŀ�ĵ�ַ
	unsigned char source_mac[6]; //source MAC ��̫��Դ��ַ
	unsigned short eh_type;      //ethernet type ֡����  ARP(0x0806)
};

//ARP����Ӧ��
struct arp_head
{
	//APR�ײ� 8=2+2+1+1+2
	unsigned short hardware_type; //hardware type  Ӳ������
	unsigned short protocol_type; //format of hardware adress Э������ 
	unsigned char add_len;        //length of hardware addrdss Ӳ����ַ���� - mac����
	unsigned char pro_len;        //length of protocol type Э���ַ���� - ip����
	unsigned short option;        //request or ack �����Ӧ��

	unsigned char sour_addr[6];  //source MAC address ������(Դ)Ӳ����ַ
	unsigned char sour_ip[4];    //source proco addr ԴIP
    unsigned char dest_addr[6];  //target hardware address ������(Ŀ��)Ӳ����ַ
    unsigned char dest_ip[4];    //target proco addr Ŀ��IP


	unsigned char padding[18];   //
};

//APR���ṹ
struct arp_packet//���հ��ṹ
{
	ethernet_head eth;//��̫��ͷ��
	arp_head arp;//ARP���ݰ�ͷ��
};


