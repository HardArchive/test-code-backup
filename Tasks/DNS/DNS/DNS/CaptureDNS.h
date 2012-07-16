#include "WinpCap.h"

/******************************************************************************* 
* 1�� �ļ����ƣ� CaptureDNS
* 2�� �桡������ Version *.*
* 3�� ��    ���� DNS�ظ��������� -��ͨ���ص���ʽ����
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-4-26 14:05:22
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

class CCaptureDNS:public CWinpCap, CAnalyticDNS
{
public:

	CCaptureDNS(void)
	{
		m_bExitThread = false;             //�߳��˳���־�� Ϊtrueʱ�˳��̣߳�Ϊflaseʱ�߳�����
		m_hThread = NULL;           //�߳̾��
		m_hExitEvent = NULL;        //�߳��˳��¼�����
	}

	~CCaptureDNS(void)
	{
		DWORD dwExitCode = 0;
		//�ȴ��߳��˳�
		m_bExitThread = false;
		WaitForSingleObject(m_hExitEvent, INFINITE);
		m_hExitEvent = NULL;
		m_hThread = NULL;
	}

public:
	int Init()
	{
		//_asm int 3;
		HANDLE m_hAdapter = OpenAdapter(0);
		if (!SetFilter(PACKET_FILTER, m_hAdapter))
		{
			return -1;
		}
		return 1;
	}
	int start()
	{
		//_asm int 3;
		m_bExitThread = true;
		m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, TEXT("QNA_RESULT"));   //����һ���Զ���ԭ����ʼ״̬Ϊ���źŵ��¼�����
		m_hThread = CreateThread(NULL, 0, CaptureProc, (PVOID)this, 0, NULL);
		return 1;
	}

	int stop()
	{
		//�ȴ��߳��˳�
		m_bExitThread = false;
		WaitForSingleObject(m_hExitEvent, INFINITE);
		m_hExitEvent = NULL;
		m_hThread = NULL;

		return 1;
	}

private:
	//�����߳�
	static DWORD WINAPI CaptureProc(LPVOID lp_Thread)
	{
		TRACE("�����̡߳���-QNA\n");
		CCaptureDNS* pobj = (CCaptureDNS*)lp_Thread;
		while(pobj->m_bExitThread)
		{
			//���￪ʼ����
			TRACE("�߳��������С���-QNA\n");
			pobj->AnalyticDNS();


			Sleep(5);
		}
		SetEvent(pobj->m_hExitEvent);
		pobj = NULL;
		return 1;
	}

	int AnalyticDNS()
	{
		struct pcap_pkthdr *header;
		const u_char *pkt_data;
		BYTE* byPacketDNS = NULL;
		int iPacketLen = 0;

		if (!GetNextPacket(&header, &pkt_data))
		{
			TRACE("AnalyticDNS����δ��ȡ��������-QNA\n");
			return 0;
		}
		_asm int 3;
		byPacketDNS = (BYTE*)pkt_data+42;  //�õ�DNS�׵�ַ	 
		if (AnalyticQueryDNS(byPacketDNS, iPacketLen))
		{
			TRACE("AnalyticDNS����������ʧ�ܡ���-QNA\n");
			return -1;
		}
		return 1;
	}

	//����Ƿ�ΪDNS�ظ��� �Ƿ���true �񷵻�false 
	bool CheckIsDNS(BYTE* pbyPacket)
	{
		//m_DnsServers[]
	}
private:
	bool m_bExitThread;             //�߳��˳���־�� Ϊtrueʱ�˳��̣߳�Ϊflaseʱ�߳�����
	HANDLE m_hThread;           //�߳̾��
	HANDLE m_hExitEvent;        //�߳��˳��¼�����
};
