// testWin7.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
//#include <ImageHlp.h>
//#pragma comment(lib,"ImageHlp.lib") 


typedef bool (/*__stdcall*/ *UnLogonImageEx)();
UnLogonImageEx g_UnLogonImage = NULL;
typedef bool (/*__stdcall*/ *LogonImageEx)(WCHAR*);
LogonImageEx g_LogonImage = NULL;


BOOL UpdateProcessPrivilege( HANDLE hProcess, LPCTSTR lpPrivilegeName);  

int _tmain(int argc, _TCHAR* argv[])
{
	//_asm int 3;
	HMODULE g_hDll = NULL;
	

	//if (UpdateProcessPrivilege(GetCurrentProcess(), SE_DEBUG_NAME))
	//{
	//	MessageBox(NULL, L"提权成功！", L"Error", MB_OK);
	//}
	
	g_hDll = LoadLibrary(L"LogonImage.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, L"加载动态库失败！", L"Error", MB_OK);
		return 0;
	}

	//g_UnLogonImage = (UnLogonImageEx)GetProcAddress(g_hDll, "UnLogonImage");

	//if (!g_UnLogonImage())
	//{
	//	MessageBox(NULL, L"获取DLL函数失败！", L"Error", MB_OK);
	//	FreeLibrary(g_hDll);
	//	return 0;
	//}

	g_LogonImage = (LogonImageEx)GetProcAddress(g_hDll, "LogonImage");
	if (!g_LogonImage)
	{
		MessageBox(NULL, L"获取函数失败！！！", L"Error", MB_OK);
	}

	if (!g_LogonImage(L"D:\\boot.bmp"))
	{
		MessageBox(NULL, L"替换图片失败！！！", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);
	return 0;
}

//
  
// 返回值 : TRUE : 成功; FALSE : 失败   
/******************************************************************************* 
1、 函数名称： UpdateProcessPrivilege
2、 功能描述： 提升进程权限   
3、 输入参数： hProcess [in] : 要提升的进程，目标进程   ,本进程可用GetCurrentProcess(）
			 lpPrivilegeName [in] : 要提升到的特权，目标特权 可以为以下参数
			 //#define            SE_BACKUP_NAME               TEXT("SeBackupPrivilege")     //备份数据权限
			 //#define            SE_RESTORE_NAME              TEXT("SeRestorePrivilege")    //恢复数据权限
			 //#define            SE_SHUTDOWN_NAME             TEXT("SeShutdownPrivilege")   //关机权限
			 //#define            SE_DEBUG_NAME                TEXT("SeDebugPrivilege")      //读、写控制权限
4、 返 回 值： TRUE : 成功; FALSE : 失败 
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-2 10:17:55
8、 备    注： 
********************************************************************************/ 
BOOL UpdateProcessPrivilege(HANDLE hProcess, LPCTSTR lpPrivilegeName)   
{   
	HANDLE hToken; 
	LUID destLuid;  
	if (!::OpenProcessToken( hProcess, TOKEN_ALL_ACCESS, &hToken )) 
	{  
		return FALSE;
	}
	  
	if (!::LookupPrivilegeValue( NULL, lpPrivilegeName, &destLuid )) 
	{ 
		CloseHandle(hToken);
		return FALSE;
	}

	TOKEN_PRIVILEGES TokenPrivileges;   
	TokenPrivileges.PrivilegeCount = 1;   
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;   
	TokenPrivileges.Privileges[0].Luid = destLuid;   

	if (!::AdjustTokenPrivileges( hToken, FALSE, &TokenPrivileges, 0, NULL, NULL )) 
	{
		CloseHandle(hToken);
		return FALSE;       
	}     

	return TRUE;   
}  





