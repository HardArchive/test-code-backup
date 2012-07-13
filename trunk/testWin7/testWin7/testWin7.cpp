// testWin7.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//	MessageBox(NULL, L"��Ȩ�ɹ���", L"Error", MB_OK);
	//}
	
	g_hDll = LoadLibrary(L"LogonImage.dll");
	if (!g_hDll)
	{
		MessageBox(NULL, L"���ض�̬��ʧ�ܣ�", L"Error", MB_OK);
		return 0;
	}

	//g_UnLogonImage = (UnLogonImageEx)GetProcAddress(g_hDll, "UnLogonImage");

	//if (!g_UnLogonImage())
	//{
	//	MessageBox(NULL, L"��ȡDLL����ʧ�ܣ�", L"Error", MB_OK);
	//	FreeLibrary(g_hDll);
	//	return 0;
	//}

	g_LogonImage = (LogonImageEx)GetProcAddress(g_hDll, "LogonImage");
	if (!g_LogonImage)
	{
		MessageBox(NULL, L"��ȡ����ʧ�ܣ�����", L"Error", MB_OK);
	}

	if (!g_LogonImage(L"D:\\boot.bmp"))
	{
		MessageBox(NULL, L"�滻ͼƬʧ�ܣ�����", L"Error", MB_OK);
		FreeLibrary(g_hDll);
		return 0;
	}

	FreeLibrary(g_hDll);
	return 0;
}

//
  
// ����ֵ : TRUE : �ɹ�; FALSE : ʧ��   
/******************************************************************************* 
1�� �������ƣ� UpdateProcessPrivilege
2�� ���������� ��������Ȩ��   
3�� ��������� hProcess [in] : Ҫ�����Ľ��̣�Ŀ�����   ,�����̿���GetCurrentProcess(��
			 lpPrivilegeName [in] : Ҫ����������Ȩ��Ŀ����Ȩ ����Ϊ���²���
			 //#define            SE_BACKUP_NAME               TEXT("SeBackupPrivilege")     //��������Ȩ��
			 //#define            SE_RESTORE_NAME              TEXT("SeRestorePrivilege")    //�ָ�����Ȩ��
			 //#define            SE_SHUTDOWN_NAME             TEXT("SeShutdownPrivilege")   //�ػ�Ȩ��
			 //#define            SE_DEBUG_NAME                TEXT("SeDebugPrivilege")      //����д����Ȩ��
4�� �� �� ֵ�� TRUE : �ɹ�; FALSE : ʧ�� 
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-9-2 10:17:55
8�� ��    ע�� 
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





