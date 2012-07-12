// PowerOff.cpp : ���� DLL Ӧ�ó������ڵ㡣
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

//�ж�ϵͳ�汾
BOOL IsWIN32_NT()
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	return osv.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

//���ػ���Ԥ������  ȡ��Ȩ��
BOOL PrepareShutDown()
{
	//  �˴������õĺ������ΪAPI������
	HANDLE hToken = NULL;                   //������йصķ��ʱ�Ǿ����
	HANDLE hProcess =  NULL;				//������̾����	
	LPVOID lpMsgBuf = NULL;
	TOKEN_PRIVILEGES  tp;			// �������ȼ���ǽṹ���һ��ʵ����
	LUID luid;						//  �������ID�ṹ���һ��ʵ��;
	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("��ǰϵͳΪ��WIN_32ϵͳ"), _T("Error"), MB_OK | MB_ICONINFORMATION);
        return FALSE;
	}

	hProcess = GetCurrentProcess();
	if(!hProcess)
	{
		::MessageBox(0, "processHandle is NULL", "processHandle", MB_OK);
		return FALSE;
	}
	//  �õ�һ��������йصķ��ʱ�Ǿ����	
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

	//  ��luid����ֵ��
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

	tp.PrivilegeCount = 1;       //  ���ṹ��ʵ����ֵ��
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


//EWX_FORCE  ǿ��ע��
//����ǿ����ֹ���̡�
//	���˱�־���ã�Windows���ᷢ����ϢWM_QUERYENDSESSION��WM_ENDSESSION����Ϣ��Ŀǰ��ϵͳ�����еĳ�������ܻᵼ��Ӧ�ó���ʧ���ݡ�
//	��ˣ���Ӧ��ֻ�ڽ��������ʹ�ô˱�־��
//EWX_LOGOFF �����ر����н��̣�Ȼ��ע���û���
//EWX_POWEROFF ����
//	�ر�ϵͳ���رյ�Դ����ϵͳ����֧�ֶϵ硣 ����
//	WindowsҪ�� ����Windows NT�е��ý��̱����� SE_SHUTDOWN_NAME ��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//EWX_REBOOT ����
//	�ر�ϵͳ��Ȼ����������ϵͳ�� ����
//	WindowsҪ�� ����Windows NT�У����ý��̱�����SE_SHUTDOWN_NAME��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//EWX_SHUTDOWN ����
//	�ر�ϵͳ����ȫ�عرյ�Դ�������ļ��������Ѿ�ˢ�µ������ϣ������������еĽ����Ѿ�ֹͣ�� ���� //�̻�
//	WindowsҪ�� ����Windows NT�У����ý��̱�����SE_SHUTDOWN_NAME��Ȩ�� ����Windows 9X�У�����ֱ�ӵ��á�
//�̻� paternity machine

//�������������
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

//�رռ����
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

//ע��log off
BOOL Logoff()
{
	if (!PoweroffEx(EWX_LOGOFF))
	{
		return FALSE;
	}
	return TRUE;
}

//����Ƿ���������  ����ֵΪTRUEʱ����
BOOL IsHibernate()
{
	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("��ǰϵͳΪ��WIN_32ϵͳ"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	return IsPwrHibernateAllowed();		
}


//��������
BOOL Hibernate()
{
	if (!IsHibernate())
	{
		MessageBox(NULL, "��������ϵͳ���߹��ܡ���!", "TIP", MB_OK);	
		return FALSE;
	}
	if(!PrepareShutDown())
	{
		MessageBox(NULL, "PrepareShutDown", "SHUTDOWN", MB_OK);	
		return FALSE;
	}
/*
Private Declare Function SetSuspendState Lib "Powrprof" (ByVal Hibernate As Boolean, ByVal ForceCritical As Boolean, ByVal DisableWakeEvent As Boolean) As Boolean

ʾ��:
SetSuspendState True, True,True

'����Hibernate:  =False:����; =True:����
'����ForceCritical:  =False:��ǿ����ֹ�����еĽ���; =True:ǿ����ֹ�����еĽ���
'����DisableWakeEvent  =False:����ϵͳ�¼����Ѽ����; =True:ϵͳ���ܻ��Ѽ����

*/

	if (!::SetSystemPowerState(FALSE, TRUE))
	{
		return FALSE;
	}
	return TRUE;
}

//ǿ��ע��forced
BOOL ForceLogoff()
{
	if (!PoweroffEx(EWX_FORCE))
	{
		return FALSE;
	}
	return TRUE;
}

//���ٹػ�API ��Windows7��XP�²���ͨ������2000���ϵ�ϵͳ�У�Ӧ�ö����������á�

///*NTSYSAPI NTSTATUS NTAPI NtSetSystemPowerState(
//	IN POWER_ACTION SystemAction,
//	IN SYSTEM_POWER_STATE MinSystemState,
//	IN ULONG Flags); */ 
////����POWER_ACTION��ʾ�ػ�������
////SYSTEM_POWER_STATE��ʾϵͳ��Դ״̬��
////Flags���ʾϵͳ�ػ�ԭ��
////��������MSDN�ж�����ϸ���أ�Flags��ֵ����Windows SDK��Reason.h�ж��塣
////���ǵ�ֵ��������

////���� 
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
	//i=2; //���ٹػ�
	//i=1; //�������� 

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
	//i=2; //���ٹػ�
	//i=1; //�������� 
    
	return TRUE;	
}