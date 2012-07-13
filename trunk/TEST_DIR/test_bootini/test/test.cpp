// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define BOOTLOGO L"/bootlogo"
#define NOGUIBOOT L"/noguiboot"

void XpModifyIni(bool bFlag);
int _tmain(int argc, _TCHAR* argv[])
{
	XpModifyIni(false);

	return 0;
}

//系统启动参数分析，判断boot.ini中是否添加了开机LOGO，
//未添加"/bootlogo"返回1， 未添加"/noguiboot"返回2，两都都未添加返回3
//两者都已经添加返回0
int AnalysisParameter(WCHAR* pszParameter)
{
	int iRet = 0;
	WCHAR *pTemp = NULL;

	pTemp = wcswcs(pszParameter, BOOTLOGO);
	if (!pTemp)
	{
		//未添加"/bootlogo"返回1
		iRet = 1;
	}

	pTemp = wcswcs(pszParameter, NOGUIBOOT);
	if (!pTemp)
	{
		if (1==iRet)
			iRet = 3;   //两者都未添加返回3
		else
			iRet = 2;   //未添加"/noguiboot"返回2
	}

	return iRet;
}

/******************************************************************************* 
1、 函数名称： ChangeParam(WCHAR* pszParam, int iInPut)
2、 功能描述： 
3、 输入参数： WCHAR* pszParam系统启动参数
			 int iInPut 控制命令
				1：添加BOOTLOGO
				2：添加NOGUIBOOT
				3：添加BOOTLOGO和NOGUIBOOT
				4:去除BOOTLOGO和NOGUIBOOT
4、 返 回 值： 无
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-30 15:45:56
8、 备    注： 传入参数pszParam必需有足够大的空间用添加BOOTLOGO和NOGUIBOOT否则很可能出错
********************************************************************************/ 
void ChangeParam(WCHAR* pszParam, int iInPut)
{
	int iNum = 0;
	WCHAR* pTemp = NULL;
	WCHAR wszTemp[MAX_PATH] = {0};
	BOOL bLogo = FALSE;
	BOOL bNogui = FALSE;
	bLogo = (1 == iInPut)?TRUE:((3 == iInPut)?TRUE:FALSE);
	bNogui = (2 == iInPut)?TRUE:((3 == iInPut)?TRUE:FALSE);
	//添加BOOTLOGO和NOGUIBOOT
	if (bLogo)
	{
		wcscat(pszParam, BOOTLOGO);
	}

	if (bNogui)
	{
		wcscat(pszParam, NOGUIBOOT);
	}

	if (4 == iInPut)
	{
		//移除BOOTLOGO
		pTemp = wcswcs(pszParam, BOOTLOGO);	//查找第一次出现的位置	
		if (pTemp)
		{
			iNum = wcslen(pTemp);
			wcsncpy(wszTemp, pszParam, wcslen(pszParam)-iNum);//copy出现的前一部分
			iNum = wcslen(wszTemp) + wcslen(BOOTLOGO);
			wcscat(wszTemp, pszParam+iNum);  //复制出现的后面部分
			wcscpy(pszParam, wszTemp);

			pTemp = NULL;
			iNum = 0;
			ZeroMemory(wszTemp, sizeof(WCHAR)*wcslen(wszTemp));
		}
		//移除NOGUIBOOT
	   	pTemp = wcswcs(pszParam, NOGUIBOOT);	//查找第一次出现的位置	
		if (pTemp)
		{
			iNum = wcslen(pTemp);
			wcsncpy(wszTemp, pszParam, wcslen(pszParam)-iNum);//copy出现的前一部分
			iNum = wcslen(wszTemp) + wcslen(NOGUIBOOT);
			wcscat(wszTemp, pszParam+iNum);  //复制出现的后面部分
			wcscpy(pszParam, wszTemp);

			pTemp = NULL;
			iNum = 0;
			ZeroMemory(wszTemp, sizeof(WCHAR)*wcslen(wszTemp));
		}
	}
}

/******************************************************************************* 
1、 函数名称： XpModifyIni(bool bFlag)
2、 功能描述： 在BOOT.INI中添加移除或检测BOOTLOGO和NOGUIBOOT开机参数
3、 输入参数： bool bAddFlag, 添加标志，当其为true时,当其为false时,移除BOOTLOGO和NOGUIBOOT
4、 返 回 值： 成功返回true，失败返回false
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-30 16:25:22
8、 备    注： 
********************************************************************************/ 
void XpModifyIni(bool bAddFlag)
{
	int iNum = 0;	
	WCHAR wszIniStrOS[MAX_PATH] = {0};     //wszIniStrLoader =的参数
	WCHAR wszIniStrTEMP[MAX_PATH] = {0};
	WCHAR wszBootIniPath[MAX_PATH] = {0};

	//获取系统所在盘符
	GetSystemDirectory(wszBootIniPath, MAX_PATH);
	memcpy(wszIniStrTEMP, wszBootIniPath, 3*sizeof(WCHAR));
	ZeroMemory(wszBootIniPath, sizeof(WCHAR));
	swprintf(wszBootIniPath, L"%sboot.ini", wszIniStrTEMP);
	ZeroMemory(wszIniStrTEMP, sizeof(WCHAR));

	//得到default=的字符串，即开机启用的系统
	GetPrivateProfileString(
		L"boot loader",
		L"default",
		L"",
		wszIniStrTEMP,
		MAX_PATH,
		wszBootIniPath);

	//得到系统启用所需的参数
	GetPrivateProfileString(
		L"operating systems",
		wszIniStrTEMP,
		L"",
		wszIniStrOS,
		MAX_PATH,
		wszBootIniPath);

	iNum = AnalysisParameter(wszIniStrOS);

	if(bAddFlag)
	{
		if (!iNum)
		{
			ZeroMemory(wszIniStrOS, sizeof(wszIniStrOS));
			ZeroMemory(wszIniStrTEMP, sizeof(wszIniStrTEMP));
			ZeroMemory(wszBootIniPath, sizeof(wszBootIniPath));
			return;
		}
		
		ChangeParam(wszIniStrOS, iNum);
	}
	else
	{
		ChangeParam(wszIniStrOS, 4);
	}

	::WritePrivateProfileString(
		L"operating systems",
		wszIniStrTEMP,
		wszIniStrOS,
		wszBootIniPath);	

	ZeroMemory(wszIniStrOS, sizeof(wszIniStrOS));
	ZeroMemory(wszIniStrTEMP, sizeof(wszIniStrTEMP));
}

