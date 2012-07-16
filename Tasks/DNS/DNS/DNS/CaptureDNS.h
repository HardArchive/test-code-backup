#include "WinpCap.h"

/******************************************************************************* 
* 1、 文件名称： CaptureDNS
* 2、 版　　本： Version *.*
* 3、 描    述： DNS回复包拦截类 -不通过回调方式拦截
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-4-26 14:05:22
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

class CCaptureDNS:public CWinpCap, CAnalyticDNS
{
public:

	CCaptureDNS(void)
	{
		m_bExitThread = false;             //线程退出标志， 为true时退出线程，为flase时线程运行
		m_hThread = NULL;           //线程句柄
		m_hExitEvent = NULL;        //线程退出事件对象
	}

	~CCaptureDNS(void)
	{
		DWORD dwExitCode = 0;
		//等待线程退出
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
		m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, TEXT("QNA_RESULT"));   //创建一个自动复原，初始状态为无信号的事件对象
		m_hThread = CreateThread(NULL, 0, CaptureProc, (PVOID)this, 0, NULL);
		return 1;
	}

	int stop()
	{
		//等待线程退出
		m_bExitThread = false;
		WaitForSingleObject(m_hExitEvent, INFINITE);
		m_hExitEvent = NULL;
		m_hThread = NULL;

		return 1;
	}

private:
	//拦截线程
	static DWORD WINAPI CaptureProc(LPVOID lp_Thread)
	{
		TRACE("进入线程……-QNA\n");
		CCaptureDNS* pobj = (CCaptureDNS*)lp_Thread;
		while(pobj->m_bExitThread)
		{
			//这里开始拦截
			TRACE("线程正在运行……-QNA\n");
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
			TRACE("AnalyticDNS——未获取到包……-QNA\n");
			return 0;
		}
		_asm int 3;
		byPacketDNS = (BYTE*)pkt_data+42;  //得到DNS首地址	 
		if (AnalyticQueryDNS(byPacketDNS, iPacketLen))
		{
			TRACE("AnalyticDNS——分析包失败……-QNA\n");
			return -1;
		}
		return 1;
	}

	//检查是否为DNS回复包 是返回true 否返回false 
	bool CheckIsDNS(BYTE* pbyPacket)
	{
		//m_DnsServers[]
	}
private:
	bool m_bExitThread;             //线程退出标志， 为true时退出线程，为flase时线程运行
	HANDLE m_hThread;           //线程句柄
	HANDLE m_hExitEvent;        //线程退出事件对象
};
