// LogonImage.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <stdio.h>
#include <shlwapi.h>  
#pragma comment(lib,"Shlwapi.lib") 


//�ж�ϵͳ�汾
//typedef OSVERSIONINFOW OSVERSIONINFO;
//typedef struct _OSVERSIONINFOW {
//    DWORD dwOSVersionInfoSize;     //ָ�������ݽṹ���ֽڴ�С
//    DWORD dwMajorVersion;          //����ϵͳ�����汾��   5����2000���ϰ汾
//    DWORD dwMinorVersion;          //����ϵͳ�ĸ��汾��    0����win2000. 1����winxp
//    DWORD dwBuildNumber;           //����ϵͳ�Ĵ�����
//    DWORD dwPlatformId;            //����ϵͳID��
//    WCHAR szCSDVersion[ 128 ];     // Maintenance string for PSS usage    ���ڲ���ϵͳ��һЩ������Ϣ
//} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
/******************************************************************************* 
1�� �������ƣ� GetOSVer
2�� ���������� ��ȡ����ϵͳ�İ汾��
3�� ��������� ��
4�� �� �� ֵ�� ʧ�ܷ��� 0�� xp ���� 1�� windows 2003 ���� 2 ��windows 2003 ���� 3��vista and 2008 ���� 4��2008 R2 and Windows 7 ����  5��
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-8-30 14:15:56
8�� ��    ע�� 
********************************************************************************/ 
int GetOSVer()  
{  
	OSVERSIONINFO osver = {0};     
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);     
	//VER_PLATFORM_WIN32_NT����ʶΪWindowsNT��
	if(osver.dwPlatformId == 2)  
	{  //����ϵͳ�����汾��  ����ϵͳ�ĸ��汾��
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)  
		{  
			//printf("xp\n");  
			return(2);  
		}  
		if(osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)  
		{  
			//printf("windows 2003\n");  
			return(3);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 0)  
		{  
			//printf("vista and 2008\n");  
			return(4);  
		}  
		if(osver.dwMajorVersion ==  6 && osver.dwMinorVersion == 1)  
		{  
			//printf("2008 R2 and Windows 7\n");  
			return(5);  
		}  
	}  
	return 0;  
} 

//��ȡ�ļ�����
DWORD GetFileLen(WCHAR* pwszImagePath)
{
	DWORD dwFileLen = 0;	
	HANDLE hFile = NULL;     
	hFile = CreateFile(pwszImagePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);       
	dwFileLen = GetFileSize(hFile, NULL);       
	CloseHandle(hFile);     


	//FILE* pFile = NULL;
	//

	//pFile = ::fopen(pwszImagePath, "r");
	//if(pFile == NULL)
	//{
	//	return 0;
	//}

	//::fseek(pFile, 0, SEEK_SET);    //��λ��ʼ
	//lFileLen = ::ftell(pFile);

	//::fseek(pFile, 0, SEEK_END);    //��λ��β
	//lFileLen = ::ftell(pFile) - lFileLen;

	//::fclose(pFile);

	return dwFileLen;
}

/******************************************************************************* 
1�� �������ƣ� SetWin7Reg
2�� ���������� �������������޸ĵ�ע���ֵHKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Authentication\LogonUI\Background
	ע����޸�ֵ��OEMBackground��Ϊ��1��
3�� ��������� bool bFlag  //Ϊtrueʱ�����޸ģ�Ϊfalseʱȡ��
4�� �� �� ֵ�� �ɹ�����true��ʧ�ܷ���false
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2011-8-30 16:30:51
8�� ��    ע�� 
********************************************************************************/ 
bool SetWin7Reg(bool bFlag)
{
	HKEY hKey = 0; 
	LONG Regcode = 0;	
	DWORD dwValue = 0;
	
	if (bFlag)
		dwValue = 1;
	else
		dwValue = 0;
	//RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background", &hKey); 
	RegCreateKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Authentication\\LogonUI\\Background", &hKey); 
	Regcode = RegSetValueEx(hKey, L"OEMBackground", 0, REG_SZ, (CONST BYTE*)&dwValue, 4); 
	
	if (ERROR_SUCCESS != Regcode)
	{
		return FALSE;	
	}
	
	RegCloseKey(hKey); 
	return TRUE;
}


bool LogonForXp(WCHAR* pwszImagePath)
{


	return true;
}

bool LogonForWin7(WCHAR* pwszImagePath)
{
	DWORD dwImageLen = GetFileLen(pwszImagePath);
	if (dwImageLen>256*1024)
	{
		::MessageBox(NULL, L"ͼƬ�ļ����󣬱���С��250Kb����", L"ERROR", MB_OK);
		return false;
	}
	if (!SetWin7Reg(true))
	{
	    ::MessageBox(NULL, L"����ע�����ʧ�ܡ���", L"ERROR", MB_OK);
		return false;
	}
	CopyFile(pwszImagePath, L"C:\\WINDOWS\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg", FALSE);  //�����ļ�
	
	return true;
}

bool LogonImage(WCHAR* pwszImagePath)
{
	::MessageBox(NULL, L"�������á���", L"ERROR", MB_OK);
	int iOsVer = 0;
	if ((!pwszImagePath) || (!wcslen(pwszImagePath)))
	{
		OutputDebugString(L"XP��Win2003ϵͳ����\r\n");
		return false;
	}
	if (!PathFileExists(pwszImagePath))
	{
		::MessageBox(NULL, L"ͼƬ�ļ������ڡ���", L"ERROR", MB_OK);
		return false;
	}
	

	iOsVer = GetOSVer();

	if ((2==iOsVer) || (3==iOsVer))
	{
		OutputDebugString(L"Vista��Win7ϵͳ����\r\n");
		return LogonForXp(pwszImagePath);
	}
	else if ((4==iOsVer) || (5==iOsVer))
	{
		OutputDebugString(L"Vista��Win7ϵͳ����\r\n");
		return LogonForWin7(pwszImagePath);
	}
	else
	{
		OutputDebugString(L"����ϵͳ�屾���󡭡�\r\n");
		return false;
	}
}

//�Ƴ����������޸�
bool UnLogonImage()
{
	::MessageBox(NULL, L"ж�����á���", L"ERROR", MB_OK);
	DeleteFile(L"C:\\WINDOWS\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg");
	if (!SetWin7Reg(FALSE))
	{
		::MessageBox(NULL, L"����ע�����ʧ�ܡ���", L"ERROR", MB_OK);
		return false;
	}

	return true;
}