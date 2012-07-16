
#ifndef  _DEFINE_STRUCT_HEAD_H_
#define  _DEFINE_STRUCT_HEAD_H_
/////////////////////////////////////////
///// define_struct_head.h
/////////////////////////////////////////

#include "stdafx.h"
struct		arppkt
{
	unsigned short		hdtyp;
	unsigned short		protyp;
	unsigned char		hdsize;
	unsigned char		prosize;
	unsigned short		op;
	u_char				smac[6];
	u_char				sip[4];
	u_char				dmac[6];
	u_char				dip[4];
};


//�Զ��崦�������
// pcap_pkthdr�ṹ�Ǵ�WINPCAP�����õ����ݰ���ͷ
// pkt_data�Ǳ�ʾMAC֡����ʼλ��
// out�������
void packet_handler(const pcap_pkthdr *header, const  u_char *pkt_data, ostream& out);
#endif