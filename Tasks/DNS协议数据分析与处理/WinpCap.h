.h
// WinpCap.h: interface for the CWinpCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPCAP_H__97E8FE92_25BC_4259_A63A_B908F07A46F0__INCLUDED_)
#define AFX_WINPCAP_H__97E8FE92_25BC_4259_A63A_B908F07A46F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
  封装 winpcap 的一些函数 。。
*/
#include <pcap.h>
typedef void ( *HandlePackets)(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
class CWinpCap  
{
public:
        CWinpCap();
        virtual ~CWinpCap();
public:
        HANDLE            OpenAdapter(int idx); //打开网卡  参数是要打开的网卡序列号；
        HANDLE            OpenAdapter(const char *szAdapterName); //打开指定的网卡 
        pcap_if_t  *        GetAdapterList();
        u_int                   GetAdapterCount()
        {
                if(!m_Adapter_Handle)
                        GetAdapterList();
                return m_AdapterCount;
        }
        virtual bool         StartCapture(HandlePackets  Handler); //开始捕获网卡上的数据包  会阻塞。。。
        virtual bool         StopCapture();

        bool         GetNextPacket(struct pcap_pkthdr **header, const u_char **pkt_data);

        bool         SetFilter(const char *szFilter,HANDLE hAdapter); //设置过滤器
        bool         SendPacket(const  byte *buffer,u_int m_size);

public:
        static         char buff[PCAP_ERRBUF_SIZE];  //pcap error buff
private:
        void          Free();
private:
        pcap_t      *m_Adapter_Handle; //网卡的句柄
        u_int           m_AdapterCount; //网卡驱动器的数目
        pcap_if_t* pAdapters;
        bool           m_bIsCaptureStart;  //是不是开始捕获了。。
};

#endif // !defined(AFX_WINPCAP_H__97E8FE92_25BC_4259_A63A_B908F07A46F0__INCLUDED_)


。。。。。。。。。。。。。。。。。
.Cpp
// WinpCap.cpp: implementation of the CWinpCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinpCap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinpCap::CWinpCap()
{
        m_Adapter_Handle = NULL;
        m_AdapterCount = 0;
        pAdapters = NULL;
        m_bIsCaptureStart = false;
}

CWinpCap::~CWinpCap()
{
        Free();
}

char CWinpCap::buff[PCAP_ERRBUF_SIZE]={0};  //pcap error buff

HANDLE  CWinpCap::OpenAdapter(int idx)
{
        if(m_Adapter_Handle)
                Free();
        GetAdapterList();
        if(idx > m_AdapterCount)
        {
                return NULL;
        }
        pcap_if_t *p = GetAdapterList();
        for (int i = 0;i<idx;i++)
                p = p->next;
        if((m_Adapter_Handle = pcap_open_live(p->name,65536,1,1000,buff)) == NULL)
        {
                return NULL;
        }
        return p;
}

HANDLE CWinpCap::OpenAdapter(const char *szAdapterName)
{
        if(m_Adapter_Handle)
                Free();
        GetAdapterList();
        if((m_Adapter_Handle = pcap_open_live(szAdapterName,65536,1,1000,buff)) == NULL)
        {
                return NULL;
        }
        for (pcap_if_t *p=GetAdapterList(); p; p=p->next)
        {
                if(strcmp(p->name,szAdapterName) == 0)
                        break;
        }
        return p;
}

pcap_if_t *CWinpCap::GetAdapterList()
{
        if(!pAdapters)
        {
                if(pcap_findalldevs(&pAdapters,buff) == -1)
                {
                        return NULL;
                }
                pcap_if *p = pAdapters;
                m_AdapterCount = 0;
                for (p=pAdapters; p; p=p->next)
                        m_AdapterCount++;
        }
        return pAdapters;
}


void CWinpCap::Free()
{
        if(m_bIsCaptureStart)
        {
                StopCapture();
                m_bIsCaptureStart = false;
        }
        if(m_Adapter_Handle)
                pcap_close(m_Adapter_Handle);
        m_Adapter_Handle = NULL;
        if(pAdapters)
                pcap_freealldevs(pAdapters);
        pAdapters = NULL;
}

bool CWinpCap::StartCapture(HandlePackets Handler)
{
        if(!m_Adapter_Handle)
                return false;
        m_bIsCaptureStart = true;
        pcap_loop(m_Adapter_Handle, 0, Handler, NULL);
        return true;
}

bool CWinpCap::StopCapture()
{
        if(m_bIsCaptureStart  && m_Adapter_Handle)
        {
                pcap_breakloop(m_Adapter_Handle);
            return true;
        }
        else
                return false;
}

bool  CWinpCap::GetNextPacket( struct pcap_pkthdr **header, const u_char **pkt_data)
{
        if(!m_Adapter_Handle)
                return false;
        return pcap_next_ex( m_Adapter_Handle, &(*header), &(*pkt_data)) >= 0 ? true : false;
}

bool CWinpCap::SendPacket(const byte *buffer,u_int m_size)
{
        if(!m_Adapter_Handle)
                return false;
        return pcap_sendpacket(m_Adapter_Handle,buffer,m_size) <0 ? false : true;
}

bool CWinpCap::SetFilter(const char *szFilter,HANDLE hAdapter)
{
        if(!m_Adapter_Handle)
                return false;
        struct bpf_program fcode;
        if(pcap_compile(m_Adapter_Handle,&fcode,szFilter,1,
                ((sockaddr_in *)((pcap_if_t *)hAdapter)->addresses->netmask)->sin_addr.S_un.S_addr)<0)
        {
                return false;
        }
        pcap_setfilter(m_Adapter_Handle,&fcode);
        return true;
}