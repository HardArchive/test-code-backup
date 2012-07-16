#pragma once
/******************************************************************************* 
* 1、 文件名称： WinpCap
* 2、 版　　本： Version *.*
* 3、 描    述： 封装 winpcap 的一些函数 。。
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-4-20 16:43:40
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

#include <pcap.h>
//#include <WinSock2.h>
#include <WinSock.h>
#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "wsock32.lib")

//typedef unsigned char byte;

//使用回调方式捕获数据包使用的回调函数类型声明
typedef void ( *HandlePackets)(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

class CWinpCap
{
public:

	CWinpCap(void)
	{
		m_Adapter_Handle = NULL;
		m_AdapterCount = 0;
		m_pAdapters = NULL;
		m_bIsCaptureStart = false;
	}

	~CWinpCap(void)
	{
		Free();
	}

public:
	//打开网卡  参数是要打开的网卡序列号；
	HANDLE OpenAdapter(int idx) 
	{
		if(m_Adapter_Handle)
			Free();
		GetAdapterList();
		if(idx > (int)m_AdapterCount)
		{
			return NULL;
		}
		pcap_if_t *p = GetAdapterList();
		for (int i = 0;i<idx;i++)
			p = p->next;
		if((m_Adapter_Handle = pcap_open_live(p->name, 65536, 1, 1000, buff)) == NULL)
		{
			return NULL;
		}
		return p;
	}

	//打开指定的网卡 
	HANDLE OpenAdapter(const char *szAdapterName)
	{
		if(m_Adapter_Handle)
			Free();
		GetAdapterList();
		if((m_Adapter_Handle = pcap_open_live(szAdapterName, 65536, 1, 1000, buff)) == NULL)
		{
			return NULL;
		}
		pcap_if_t* p = NULL;
		for (p=GetAdapterList(); p; p=p->next)
		{
			if(strcmp(p->name, szAdapterName) == 0)
				break;
		}
		return p;
	}

	//获得设备列表
	pcap_if_t * GetAdapterList()
	{
		if(!m_pAdapters)
		{
			if(pcap_findalldevs(&m_pAdapters, buff) == -1)
			{
				return NULL;
			}
			pcap_if *p = m_pAdapters;
			m_AdapterCount = 0;
			for (p=m_pAdapters; p; p=p->next)
				m_AdapterCount++;
		}
		return m_pAdapters;
	}

	//获取设备数量 即网卡个数
	u_int GetAdapterCount()
	{
		if(!m_Adapter_Handle)
			GetAdapterList();
		return m_AdapterCount;
	}

	//开始捕获网卡上的数据包  会阻塞。。。通过回调的方式捕获数据包
	virtual bool StartCapture(HandlePackets Handler)
	{
		if(!m_Adapter_Handle)
			return false;
		m_bIsCaptureStart = true;
		pcap_loop(m_Adapter_Handle, 0, Handler, NULL);
		return true;
	}

	//停止捕获
	virtual bool StopCapture()
	{
		if(m_bIsCaptureStart  && m_Adapter_Handle)
		{
			pcap_breakloop(m_Adapter_Handle);
			return true;
		}
		else
			return false;
	}

	//通过非回调的方式捕获数据包
	bool GetNextPacket(struct pcap_pkthdr **header, const u_char **pkt_data)
	{
		if(!m_Adapter_Handle)
			return false;
		return pcap_next_ex( m_Adapter_Handle, &(*header), &(*pkt_data)) >= 0 ? true : false;
	}

	//设置过滤器 szFilter关键字， hAdapter设备句柄
	bool SetFilter(const char *szFilter, HANDLE hAdapter) 
	{
		if(!m_Adapter_Handle)
			return false;
		struct bpf_program fcode;
		if(pcap_compile(m_Adapter_Handle, &fcode, szFilter, 1,
			((sockaddr_in *)((pcap_if_t *)hAdapter)->addresses->netmask)->sin_addr.S_un.S_addr)<0)
		{
			return false;
		}
		pcap_setfilter(m_Adapter_Handle, &fcode);
		return true;
	}
	//发送包 buffer包内容， 大小
	bool SendPacket(const BYTE* buffer, u_int m_size)
	{
		if(!m_Adapter_Handle)
			return false;
		return pcap_sendpacket(m_Adapter_Handle, buffer, m_size) <0 ? false : true;
	}

	//virtual void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data) = 0;

private:
	void Free()
	{
		if(m_bIsCaptureStart)
		{
			StopCapture();
			m_bIsCaptureStart = false;
		}
		if(m_Adapter_Handle)
			pcap_close(m_Adapter_Handle);
		m_Adapter_Handle = NULL;
		if(m_pAdapters)
			pcap_freealldevs(m_pAdapters);
		m_pAdapters = NULL;
	}

public:
	static char buff[PCAP_ERRBUF_SIZE];  //pcap error buff

private:
	pcap_t*    m_Adapter_Handle;   //网卡的句柄
	u_int      m_AdapterCount;     //网卡驱动器的数目
	pcap_if_t* m_pAdapters;        //本地机器设备列表
	bool       m_bIsCaptureStart;  //是不是开始捕获了。。
};

char CWinpCap::buff[PCAP_ERRBUF_SIZE]={0};