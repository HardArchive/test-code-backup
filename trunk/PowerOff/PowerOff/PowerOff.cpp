// PowerOff.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"

#include "PowerOff.h"
//#include <Shellapi.h>
#pragma comment(lib,"Powrprof.lib")

extern "C" { 
#include <powrprof.h> 
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//判断系统版本
BOOL IsWIN32_NT()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	return osv.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

//作关机的预备工作  取得权限
BOOL PrepareShutDown()
{
	//  此处所调用的函数大多为API函数；
	HANDLE hToken = NULL;                   //与进程有关的访问标记句柄；
	HANDLE hProcess =  NULL;				//定义进程句柄；	
	LPVOID lpMsgBuf = NULL;
	TOKEN_PRIVILEGES  tp;			// 定义优先级标记结构体的一个实例；
	LUID luid;						//  定义查找ID结构体的一个实例;
	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("当前系统为非WIN_32系统"), _T("Error"), MB_OK | MB_ICONINFORMATION);
        return FALSE;
	}

	hProcess = GetCurrentProcess();
	if(!hProcess)
	{
		::MessageBox(0, "processHandle is NULL", "processHandle", MB_OK);
		return FALSE;
	}
	//  得到一个与进程有关的访问标记句柄；	
	if(!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{		
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);
		::MessageBox(0, (LPCTSTR)lpMsgBuf, _T("GetLastError"), MB_OK | MB_ICONINFORMATION);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}		

	//  给luid赋制值；
	if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid))  
	{
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);
		::MessageBox(0, (LPCTSTR)lpMsgBuf, _T("GetLastError"), MB_OK | MB_ICONINFORMATION);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	

	tp.PrivilegeCount = 1;       //  给结构体实例赋值；
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);
		::MessageBox(0, (LPCTSTR)lpMsgBuf, _T("GetLastError"), MB_OK | MB_ICONINFORMATION);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	
	return TRUE;
}


BOOL PoweroffEx(UINT uFlags)
{
	LPVOID lpMsgBuf = NULL;
	if(0==ExitWindowsEx(uFlags, 0))
	{		
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			0,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);

		::MessageBox(0,(LPCTSTR)lpMsgBuf,_T("GetLastError"),MB_OK | MB_ICONINFORMATION);
		//OutputDebugString(lpMsgBuf);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}
	return TRUE;
}


//EWX_FORCE  强制注销
//　　强制终止进程。
//	当此标志设置，Windows不会发送消息WM_QUERYENDSESSION和WM_ENDSESSION的消息给目前在系统中运行的程序。这可能会导致应用程序丢失数据。
//	因此，你应该只在紧急情况下使用此标志。
//EWX_LOGOFF 　　关闭所有进程，然后注销用户。
//EWX_POWEROFF 　　
//	关闭系统并关闭电源。该系统必须支持断电。 　　
//	Windows要求： 　　Windows NT中调用进程必须有 SE_SHUTDOWN_NAME 特权。 　　Windows 9X中：可以直接调用。
//EWX_REBOOT 　　
//	关闭系统，然后重新启动系统。 　　
//	Windows要求： 　　Windows NT中：调用进程必须有SE_SHUTDOWN_NAME特权。 　　Windows 9X中：可以直接调用。
//EWX_SHUTDOWN 　　
//	关闭系统，安全地关闭电源。所有文件缓冲区已经刷新到磁盘上，所有正在运行的进程已经停止。 　　 //侍机
//	Windows要求： 　　Windows NT中：调用进程必须有SE_SHUTDOWN_NAME特权。 　　Windows 9X中：可以直接调用。
//侍机 paternity machine

//重新启动计算机
BOOL Reboot()
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
		return FALSE;
	}
	if (!PoweroffEx(EWX_REBOOT))
	{
		return FALSE;
	}	
	return TRUE;
}

//关闭计算机
BOOL ShutDown() 
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
        return FALSE;
	}
	if (!PoweroffEx(EWX_SHUTDOWN))
	{
		return FALSE;
	}
	return TRUE;
}

//注销log off
BOOL Logoff()
{
	if (!PoweroffEx(EWX_LOGOFF))
	{
		return FALSE;
	}
	return TRUE;
}

//检测是否启用休眠  返回值为TRUE时启用
BOOL IsHibernate()
{
	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("当前系统为非WIN_32系统"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	return IsPwrHibernateAllowed();		
}


//待机休眠
BOOL Hibernate()
{
	if (!IsHibernate())
	{
		MessageBox(NULL, "请先启用系统休眠功能……!", "TIP", MB_OK);	
		return FALSE;
	}
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
		return FALSE;
	}
/*
Private Declare Function SetSuspendState Lib "Powrprof" (ByVal Hibernate As Boolean, ByVal ForceCritical As Boolean, ByVal DisableWakeEvent As Boolean) As Boolean

示例:
SetSuspendState True, True,True

'参数Hibernate:  =False:待机; =True:休眠
'参数ForceCritical:  =False:不强行中止运行中的进程; =True:强行中止运行中的进程
'参数DisableWakeEvent  =False:允许系统事件唤醒计算机; =True:系统不能唤醒计算机

*/

	if (!::SetSystemPowerState(FALSE, TRUE))
	{
		return FALSE;
	}
	return TRUE;
}

//强制注销forced
BOOL ForceLogoff()
{
	if (!PoweroffEx(EWX_FORCE))
	{
		return FALSE;
	}
	return TRUE;
}

//快速关机API 在Windows7和XP下测试通过，在2000以上的系统中，应该都可以起作用。

///*NTSYSAPI NTSTATUS NTAPI NtSetSystemPowerState(
//	IN POWER_ACTION SystemAction,
//	IN SYSTEM_POWER_STATE MinSystemState,
//	IN ULONG Flags); */ 
////其中POWER_ACTION表示关机动作，
////SYSTEM_POWER_STATE表示系统电源状态，
////Flags则表示系统关机原因。
////这三者在MSDN中都有详细记载，Flags的值则在Windows SDK的Reason.h中定义。
////他们的值是这样的

////代码 
//enum SYSTEM_POWER_STATE{
//	PowerSystemUnspecified = 0,
//	PowerSystemWorking     = 1,
//	PowerSystemSleeping1   = 2, 
//	PowerSystemSleeping2   = 3, 
//	PowerSystemSleeping3   = 4, 
//	PowerSystemHibernate   = 5, 
//	PowerSystemShutdown    = 6, 
//	PowerSystemMaximum     = 7
//}
//enum POWER_ACTION{
//	PowerActionNone = 0,
//	PowerActionReserved,
//	PowerActionSleep,
//	PowerActionHibernate,
//	PowerActionShutdown,
//	PowerActionShutdownReset,
//	PowerActionShutdownOff,
//	PowerActionWarmEject
//} 



BOOL ForceShutDown()
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
		return FALSE;
	}

	typedef HANDLE (PASCAL *OPEN)(int); 
	HMODULE hKernel32 = GetModuleHandle("ntdll.dll");
	OPEN pLibRemote = (OPEN)GetProcAddress( hKernel32,	"NtShutdownSystem");
	pLibRemote(2);
	//i=2; //快速关机
	//i=1; //快速重启 

	return TRUE;
}

BOOL ForceReboot()
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
		return FALSE;
	}

	typedef HANDLE (PASCAL *OPEN)(int); 
	HMODULE hKernel32 = GetModuleHandle("ntdll.dll");
	OPEN pLibRemote = (OPEN)GetProcAddress( hKernel32,	"NtShutdownSystem");
	pLibRemote(1);
	//i=2; //快速关机
	//i=1; //快速重启 
    
	return TRUE;	
}