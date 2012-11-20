// Ping.h: interface for the CPing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PING_H__BCB312A6_83FC_4AE1_8D1C_A01DD73EFEB4__INCLUDED_)
#define AFX_PING_H__BCB312A6_83FC_4AE1_8D1C_A01DD73EFEB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPing  
{
public:
        //����ping����
        void SetConfigure(char * host,BOOL recordrout=FALSE,int size=DEF_PACKET_SIZE);
        //����IP��ѡ����
        void DecodeIPOptions(char *buf, int bytes);
        //���ICMP��
        void Cleanup();
        void Ping(int timeout =1000);
        SOCKET m_hSocket;        //����ICMP����socket
        IpOptionHeader   m_ipopt;
        SOCKADDR_IN m_addrDest;                //Ŀ�ĵ�ַ
        SOCKADDR_IN m_addrFrom;                //Դ��ַ
        char              *icmp_data;
		char              *recvbuf;
        USHORT             seq_no ;
        char *lpdest;
        int   datasize;
        BOOL m_bRecordRout;
        //CMyPingDlg  * m_dlg;
        CPing();
        virtual ~CPing();
       
private:
        //����ICMPͷ�ṹ
        void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN* from);
        //У������
        USHORT checksum(USHORT *buffer, int size);
        //���ICMP����
        void FillICMPData(char *icmp_data, int datasize);
};

#endif // !defined(AFX_PING_H__BCB312A6_83FC_4AE1_8D1C_A01DD73EFEB4__INCLUDED_)
