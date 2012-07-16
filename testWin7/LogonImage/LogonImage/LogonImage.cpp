// LogonImage.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <stdio.h>
#include <shlwapi.h>  
#pragma comment(lib,"Shlwapi.lib") 


//判断系统版本
//typedef OSVERSIONINFOW OSVERSIONINFO;
//typedef struct _OSVERSIONINFOW {
//    DWORD dwOSVersionInfoSize;     //指定该数据结构的字节大小
//    DWORD dwMajorVersion;          //操作系统的主版本号   5代表2000以上版本
//    DWORD dwMinorVersion;          //操作系统的副版本号    0代表win2000. 1代表winxp
//    DWORD dwBuildNumber;           //操作系统的创建号
//    DWORD dwPlatformId;            //操作系统ID号
//    WCHAR szCSDVersion[ 128 ];     // Maintenance string for PSS usage    关于操作系统的一些附加信息
//} OSVERSIONINFOW, *POSVERSIONINFOW, *LPOSVERSIONINFOW, RTL_OSVERSIONINFOW, *PRTL_OSVERSIONINFOW;
/******************************************************************************* 
1、 函数名称： GetOSVer
2、 功能描述： 获取操作系统的版本号
3、 输入参数： 无
4、 返 回 值： 失败返回 0； xp 返回 1｜ windows 2003 返回 2 ｜windows 2003 返回 3｜vista and 2008 返回 4｜2008 R2 and Windows 7 返回  5｜
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-8-30 14:15:56
8、 备    注： 
********************************************************************************/ 
int GetOSVer()  
{  
	OSVERSIONINFO osver = {0};     
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);     
	GetVersionEx(&osver);     
	//VER_PLATFORM_WIN32_NT：标识为WindowsNT。
	if(osver.dwPlatformId == 2)  
	{  //操作系统的主版本号  操作系统的副版本号
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

//获取文件长度
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

	//::fseek(pFile, 0, SEEK_SET);    //定位开始
	//lFileLen = ::ftell(pFile);

	//::fseek(pFile, 0, SEEK_END);    //定位结尾
	//lFileLen = ::ftell(pFile) - lFileLen;

	//::fclose(pFile);

	return dwFileLen;
}

/******************************************************************************* 
1、 函数名称： SetWin7Reg
2、 功能描述： 设置启动界面修改的注册表值HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Authentication\LogonUI\Background
	注册表修改值“OEMBackground”为“1”
3、 输入参数： bool bFlag  //为true时设置修改，为false时取消
4、 返 回 值： 成功返回true，失败返回false
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-8-30 16:30:51
8、 备    注： 
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
		::MessageBox(NULL, L"图片文件过大，必需小于250Kb……", L"ERROR", MB_OK);
		return false;
	}
	if (!SetWin7Reg(true))
	{
	    ::MessageBox(NULL, L"设置注册表项失败……", L"ERROR", MB_OK);
		return false;
	}
	CopyFile(pwszImagePath, L"C:\\WINDOWS\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg", FALSE);  //拷贝文件
	
	return true;
}

bool LogonImage(WCHAR* pwszImagePath)
{
	::MessageBox(NULL, L"进入设置……", L"ERROR", MB_OK);
	int iOsVer = 0;
	if ((!pwszImagePath) || (!wcslen(pwszImagePath)))
	{
		OutputDebugString(L"XP或Win2003系统……\r\n");
		return false;
	}
	if (!PathFileExists(pwszImagePath))
	{
		::MessageBox(NULL, L"图片文件不存在……", L"ERROR", MB_OK);
		return false;
	}
	

	iOsVer = GetOSVer();

	if ((2==iOsVer) || (3==iOsVer))
	{
		OutputDebugString(L"Vista或Win7系统……\r\n");
		return LogonForXp(pwszImagePath);
	}
	else if ((4==iOsVer) || (5==iOsVer))
	{
		OutputDebugString(L"Vista或Win7系统……\r\n");
		return LogonForWin7(pwszImagePath);
	}
	else
	{
		OutputDebugString(L"操作系统板本错误……\r\n");
		return false;
	}
}

//移除启动界面修改
bool UnLogonImage()
{
	::MessageBox(NULL, L"卸载设置……", L"ERROR", MB_OK);
	DeleteFile(L"C:\\WINDOWS\\system32\\oobe\\info\\backgrounds\\backgroundDefault.jpg");
	if (!SetWin7Reg(FALSE))
	{
		::MessageBox(NULL, L"设置注册表项失败……", L"ERROR", MB_OK);
		return false;
	}

	return true;
}