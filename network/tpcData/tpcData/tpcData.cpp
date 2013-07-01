// tpcData.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tpcData.h"
#include "../global/IOCPServerHelper.h"
#include "../global/file.h"
#include "../global/safestl.h"
// 这是导出变量的一个示例
TPCDATA_API int ntpcData=0;

// 这是导出函数的一个示例。
TPCDATA_API int fntpcData(void)
{
	return 42;
}

//// 这是已导出类的构造函数。
//// 有关类定义的信息，请参阅 tpcData.h
//CtpcData::CtpcData()
//{
//	return;
//}


RG::SAFESTL::CQueue<USERSTATUSINFO> g_queueUserStatusInfo;

CIOCPServerHelper* g_pclsIOCPServerHelper = NULL;

TPCDATA_API bool Init(LPCTSTR pszBindAddress, USHORT usPort)
{
	if (g_pclsIOCPServerHelper)	UnInit();

	g_pclsIOCPServerHelper = new CIOCPServerHelper;
	return g_pclsIOCPServerHelper->Start(pszBindAddress, usPort);
}


TPCDATA_API bool UnInit()
{
	bool bRet = false;
	 bRet = g_pclsIOCPServerHelper->Stop();
	 delete g_pclsIOCPServerHelper;
	 g_pclsIOCPServerHelper = NULL;

	 
	 g_queueUserStatusInfo.Lock();
	 while (!g_queueUserStatusInfo.empty())
	 {
		 g_queueUserStatusInfo.pop();
	 }
	 g_queueUserStatusInfo.Unlock();

	 return true;
}


TPCDATA_API int GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo)
{
	int iRet = 0;
	PUSERSTATUSINFO pstuTem = NULL;
	if (NULL == pstuOutUserStatusInfo)
	{
		return iRet;
	}

	g_queueUserStatusInfo.Lock();
	if (!g_queueUserStatusInfo.empty())
	{
		try
		{
			memcpy(pstuOutUserStatusInfo, &(g_queueUserStatusInfo.front()), sizeof(USERSTATUSINFO));
		}
		catch (PUSERSTATUSINFO )
		{
			TRACE("输入参数pstuOutUserStatusInfo异常；可能分配内存空间太小!!!\r\n");
			g_queueUserStatusInfo.Unlock();
			iRet = -2;
			return iRet;
		}
		g_queueUserStatusInfo.pop();
	}
	else
	{
		iRet = -1;
	}
	
	g_queueUserStatusInfo.Unlock();

	return 1;
}