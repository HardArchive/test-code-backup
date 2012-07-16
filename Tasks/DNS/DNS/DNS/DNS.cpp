// DNS.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ProcessDNS.h"
#include "DNS.h"

//全局变量
bool g_bIsInit = false;          //是否初始化，
CProcessDNS g_ProcessDNS;

//初始化函数
extern "C" __declspec(dllexport) int InitDNS()
{
	//g_ProcessDNS = new CProcessDNS;
	g_ProcessDNS.Init();

	g_bIsInit = true;

	return 1;
}
/******************************************************************************* 
1、 函数名称： DNSVerify
2、 功能描述： DNS验证
3、 输入参数： char* pszDomain 域名, char* pszIp IP地址
4、 返 回 值： 验证正确返回1， 错误返回-1， 失败返回0
5、 动态内存： 
6、 代码设计：  阿毛
7、 开发日期： 2012-4-20 15:39:49
8、 备    注： 
********************************************************************************/ 
extern "C" __declspec(dllexport) int DNSVerify(char* pszDomain, char* pszIp)
{
	TRACE("测试-进入DNS验证函数-QNA\n");
	if (!g_bIsInit)
	{
		TRACE("DNSVerify未初始化-QNA\n");
		return -1;
	}
	return g_ProcessDNS.VerifyDNS(pszDomain, pszIp);
}

/******************************************************************************* 
1、 函数名称： CapPacketDNS
2、 功能描述： 开启或关闭DNS数据包捕获
3、 输入参数： bool bIsStart 为true时开启， false时关闭
4、 返 回 值： 成功返回1， 失败返回0， 出错返回负值
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2012-4-26 13:11:24
8、 备    注： 
********************************************************************************/ 
extern "C" __declspec(dllexport) int CapPacketDNS(bool bIsStart)
{
	if (!g_bIsInit)
	{
		TRACE("DNSVerify未初始化-QNA\n");
		return -1;
	}
	g_ProcessDNS.CapPacketDNS(bIsStart);

	return 1;
}



