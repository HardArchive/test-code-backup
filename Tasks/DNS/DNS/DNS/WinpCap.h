#pragma once
/******************************************************************************* 
* 1�� �ļ����ƣ� WinpCap
* 2�� �桡������ Version *.*
* 3�� ��    ���� ��װ winpcap ��һЩ���� ����
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-4-20 16:43:40
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

#include <pcap.h>
//#include <WinSock2.h>
#include <WinSock.h>
#pragma comment (lib, "wpcap.lib")
#pragma comment (lib, "wsock32.lib")

//typedef unsigned char byte;

//ʹ�ûص���ʽ�������ݰ�ʹ�õĻص�������������
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
	//������  ������Ҫ�򿪵��������кţ�
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

	//��ָ�������� 
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

	//����豸�б�
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

	//��ȡ�豸���� ����������
	u_int GetAdapterCount()
	{
		if(!m_Adapter_Handle)
			GetAdapterList();
		return m_AdapterCount;
	}

	//��ʼ���������ϵ����ݰ�  ������������ͨ���ص��ķ�ʽ�������ݰ�
	virtual bool StartCapture(HandlePackets Handler)
	{
		if(!m_Adapter_Handle)
			return false;
		m_bIsCaptureStart = true;
		pcap_loop(m_Adapter_Handle, 0, Handler, NULL);
		return true;
	}

	//ֹͣ����
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

	//ͨ���ǻص��ķ�ʽ�������ݰ�
	bool GetNextPacket(struct pcap_pkthdr **header, const u_char **pkt_data)
	{
		if(!m_Adapter_Handle)
			return false;
		return pcap_next_ex( m_Adapter_Handle, &(*header), &(*pkt_data)) >= 0 ? true : false;
	}

	//���ù����� szFilter�ؼ��֣� hAdapter�豸���
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
	//���Ͱ� buffer�����ݣ� ��С
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
	pcap_t*    m_Adapter_Handle;   //�����ľ��
	u_int      m_AdapterCount;     //��������������Ŀ
	pcap_if_t* m_pAdapters;        //���ػ����豸�б�
	bool       m_bIsCaptureStart;  //�ǲ��ǿ�ʼ�����ˡ���
};

char CWinpCap::buff[PCAP_ERRBUF_SIZE]={0};