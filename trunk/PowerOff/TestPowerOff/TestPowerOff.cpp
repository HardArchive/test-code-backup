// TestPowerOff.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
typedef	BOOL (/*__stdcall*/ *RebootWindowsEx)(void);
typedef	BOOL (/*__stdcall*/ *ShutDownWindowsEx)(void);
typedef	BOOL (/*__stdcall*/ *LogoffWindowsEx)(void);
typedef	BOOL (/*__stdcall*/ *HibernateEx)(void);
typedef	BOOL (/*__stdcall*/ *ForceWindowsEx)(void);
typedef BOOL (/*__stdcall*/ *ForceShutDownEx)(void);
typedef BOOL (/*__stdcall*/ *ForceRebootEx)(void);

RebootWindowsEx g_RebootWindows = NULL;
ShutDownWindowsEx g_ShutDownWindows = NULL;
LogoffWindowsEx g_LogoffWindows = NULL;
HibernateEx g_Hibernate = NULL;
ForceWindowsEx g_ForceWindows = NULL;
ForceShutDownEx g_ForceShutDown = NULL;
ForceRebootEx g_ForceReboot = NULL;
HMODULE g_hDll = NULL;

BOOL Init()
{
	g_hDll = LoadLibrary("PowerOff.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, "加载动态库失败！", "Error", MB_OK);
		return FALSE;
	}
	g_RebootWindows = (RebootWindowsEx)GetProcAddress(g_hDll, "Reboot");
	g_ShutDownWindows = (ShutDownWindowsEx)GetProcAddress(g_hDll, "ShutDown");
	g_LogoffWindows = (LogoffWindowsEx)GetProcAddress(g_hDll, "Logoff");
	g_Hibernate = (HibernateEx)GetProcAddress(g_hDll, "Hibernate");
	g_ForceWindows = (ForceWindowsEx)GetProcAddress(g_hDll, "ForceLogoff");
    g_ForceShutDown = (ForceShutDownEx)GetProcAddress(g_hDll, "ForceShutDown");
	g_ForceReboot = (ForceRebootEx)GetProcAddress(g_hDll, "ForceReboot");

	if (!(g_RebootWindows&&g_ShutDownWindows&&g_LogoffWindows&&g_Hibernate&&g_ForceWindows&&g_ForceShutDown&&g_ForceReboot))
	{
		MessageBox(NULL, "获取DLL函数失败！", "Error", MB_OK);
        return FALSE;
	}
    return TRUE;
}



int _tmain(int argc, _TCHAR* argv[])
{
	int iInput = 0;
	Init();
	while(TRUE)
	{
		printf("退出请输入0:\r\n");
		printf("重启请输入1:\r\n");
		printf("关机请输入2:\r\n");
		printf("注销请输入3:\r\n");
		printf("侍机请输入4:\r\n");
		printf("强制注销请输入5:\r\n");
		printf("快速关机请输入6:\r\n");
		printf("快速重启请输入7:\r\n");		
		printf("请输入0-7之间的数字:");
		scanf("%d", &iInput);
		switch (iInput)
		{
		case 0:
			return 0;
			break;
		case 1:
			if (!g_RebootWindows())
			{
				MessageBox(NULL, "重启失败！！！", "Error", MB_OK);
			}
			break;
		case 2:
			if (!g_ShutDownWindows())
			{
				MessageBox(NULL, "关机失败！！！", "Error", MB_OK);
			}
			break;
		case 3:
			if (!g_LogoffWindows())
			{
				MessageBox(NULL, "注销失败！！！", "Error", MB_OK);
			}
			break;
		case 4:
			if (!g_Hibernate())
			{
				MessageBox(NULL, "休眠失败！！！", "Error", MB_OK);
			}
			break;
		case 5:			
			if (!g_ForceWindows())
			{
				MessageBox(NULL, "强制注销失败！！！", "Error", MB_OK);
			}
			break;
		case 6:
			if (!g_ForceShutDown())
			{
				MessageBox(NULL, "快速关机失败！！！", "Error", MB_OK);
			}
			break;
		case 7:
			if (!g_ForceReboot())
			{
				MessageBox(NULL, "快速重启失败！！！", "Error", MB_OK);
			}
			break;
		default:
			system("cls");  //清屏
			break;
		}
	}
	FreeLibrary(g_hDll);
	return 0;
}

