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
        //设置ping参数
        void SetConfigure(char * host,BOOL recordrout=FALSE,int size=DEF_PACKET_SIZE);
        //解析IP可选参数
        void DecodeIPOptions(char *buf, int bytes);
        //清除ICMP包
        void Cleanup();
        void Ping(int timeout =1000);
        SOCKET m_hSocket;        //发送ICMP包的socket
        IpOptionHeader   m_ipopt;
        SOCKADDR_IN m_addrDest;                //目的地址
        SOCKADDR_IN m_addrFrom;                //源地址
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
        //解析ICMP头结构
        void DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN* from);
        //校验数据
        USHORT checksum(USHORT *buffer, int size);
        //填充ICMP数据
        void FillICMPData(char *icmp_data, int datasize);
};

#endif // !defined(AFX_PING_H__BCB312A6_83FC_4AE1_8D1C_A01DD73EFEB4__INCLUDED_)
