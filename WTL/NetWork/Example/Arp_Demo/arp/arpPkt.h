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


//以太网首部  14=6+6+2
struct ethernet_head
{
	unsigned char dest_mac[6];   //dest MAC  以太网目的地址
	unsigned char source_mac[6]; //source MAC 以太网源地址
	unsigned short eh_type;      //ethernet type 帧类型  ARP(0x0806)
};

//ARP请求，应答
struct arp_head
{
	//APR首部 8=2+2+1+1+2
	unsigned short hardware_type; //hardware type  硬件类型
	unsigned short protocol_type; //format of hardware adress 协议类型 
	unsigned char add_len;        //length of hardware addrdss 硬件地址长度 - mac长度
	unsigned char pro_len;        //length of protocol type 协议地址长度 - ip长度
	unsigned short option;        //request or ack 请求或应答

	unsigned char sour_addr[6];  //source MAC address 发送者(源)硬件地址
	unsigned char sour_ip[4];    //source proco addr 源IP
    unsigned char dest_addr[6];  //target hardware address 接收者(目标)硬件地址
    unsigned char dest_ip[4];    //target proco addr 目标IP


	unsigned char padding[18];   //
};

//APR包结构
struct arp_packet//最终包结构
{
	ethernet_head eth;//以太网头部
	arp_head arp;//ARP数据包头部
};


