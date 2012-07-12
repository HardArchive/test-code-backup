// TestPowerOff.cpp : �������̨Ӧ�ó������ڵ㡣
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
		MessageBox(NULL, "���ض�̬��ʧ�ܣ�", "Error", MB_OK);
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
		MessageBox(NULL, "��ȡDLL����ʧ�ܣ�", "Error", MB_OK);
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
		printf("�˳�������0:\r\n");
		printf("����������1:\r\n");
		printf("�ػ�������2:\r\n");
		printf("ע��������3:\r\n");
		printf("�̻�������4:\r\n");
		printf("ǿ��ע��������5:\r\n");
		printf("���ٹػ�������6:\r\n");
		printf("��������������7:\r\n");		
		printf("������0-7֮�������:");
		scanf("%d", &iInput);
		switch (iInput)
		{
		case 0:
			return 0;
			break;
		case 1:
			if (!g_RebootWindows())
			{
				MessageBox(NULL, "����ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 2:
			if (!g_ShutDownWindows())
			{
				MessageBox(NULL, "�ػ�ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 3:
			if (!g_LogoffWindows())
			{
				MessageBox(NULL, "ע��ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 4:
			if (!g_Hibernate())
			{
				MessageBox(NULL, "����ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 5:			
			if (!g_ForceWindows())
			{
				MessageBox(NULL, "ǿ��ע��ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 6:
			if (!g_ForceShutDown())
			{
				MessageBox(NULL, "���ٹػ�ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		case 7:
			if (!g_ForceReboot())
			{
				MessageBox(NULL, "��������ʧ�ܣ�����", "Error", MB_OK);
			}
			break;
		default:
			system("cls");  //����
			break;
		}
	}
	FreeLibrary(g_hDll);
	return 0;
}

