#pragma once
/******************************************************************************* 
* 1、 文件名称： ProcessDNS
* 2、 版　　本： Version *.*
* 3、 描    述： DNS
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-4-20 16:45:35
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

//#include "struct_define.h"
#include "WinpCap.h"

#include "QueryDNS.h"
#include "CaptureDNS.h"
class CProcessDNS
{
public:

	CProcessDNS(void)
	{
	}

	~CProcessDNS(void)
	{
	}
public:
	int VerifyDNS(char* pszDomain, char* pszIp)
	{		
		return m_objQueryDns.VerifyDNS(pszDomain, pszIp);
	}

	int CapPacketDNS(bool bIsStart)
	{
		if (bIsStart)
		{
			TRACE("开始拦截……-QNA\n");
			m_objCaptureDns.start();
		}
		else
		{
			TRACE("停止拦截……-QNA\n");
			m_objCaptureDns.stop();
		}
		return 1;
	}

	void Init()
	{
		m_objQueryDns.Init();
		//m_objWinpCap.OpenAdapter(1);
		m_objCaptureDns.Init();

	}

    //捕获DNS回复包
	int CaptureDNS(char* pszDomain)
	{
	}
	//获取数据
	int GetData();
private:
	CQueryDNS m_objQueryDns;
	CCaptureDNS m_objCaptureDns;
};
