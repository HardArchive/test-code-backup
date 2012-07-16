// PhoneClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ClientService.h"
 
#pragma comment(lib,"ws2_32.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	//取得日志文件路径
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
	CTool::Get400RegConfig(TEXT("logpath"), g_szMinitorFolderPah);
	if (strlen(g_szMinitorFolderPah)<3)
	{
		char szTemp[MAX_PATH] = {0};
		//获取当前目录  //这里是获取当前进程文件的完整路径 
		if (!GetModuleFileName(NULL, szTemp, MAX_PATH))
		{
			return 0;
		} 
		//GetCurrentDirectory(MAX_PATH, g_szMinitorFolderPah);
		memcpy(g_szMinitorFolderPah, szTemp, (strlen(szTemp)-strlen("/PhoneClient.exe")));
		strcat(g_szMinitorFolderPah, "\\log");
		CTool::SetRegConfig(TEXT("logpath"), g_szMinitorFolderPah);
	}
	g_objLog.SetLogPath(g_szMinitorFolderPah);

	//获取监控路径
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
	CTool::Get400RegConfig(TEXT("path"), g_szMinitorFolderPah);
	if (!strlen(g_szMinitorFolderPah))
	{
		TracePrint(LOG_ERROR, "从注册表获取监控路径失败…%s;\r\n", g_szMinitorFolderPah);
		return 0;
	}

	CClientService service;
	if(!service.InitInstance())
	{
		return 1;
	}
	service.ExitInstance();
	return 0;
}
