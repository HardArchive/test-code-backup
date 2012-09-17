//file TraceUtil.h
/***************************************************************************************************
* 1、 Class      ： PathUtil
* 2、 Version    ： 1.0
* 3、 Description： 
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 16:27:44
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __PATH_UTIL_H_
#define __PATH_UTIL_H_
#include <Windows.h>

namespace QNA
{
	//删除文件夹
	//检查当前目录需要的文件夹是否存在,如果不存在返回false，存在返回true
	bool ChickDirExist(TCHAR* ptszInPath)
	{
		if (!ptszInPath || _tcsclen(ptszInPath)<2)
			return false;

		//检验路径是否存在	如果不存在则创建
		if (GetFileAttributes(ptszInPath) != FILE_ATTRIBUTE_DIRECTORY)
			return false;
		

		return true;
	}

	//创建多级目录，成功返回true， 失败返回false
	bool CreateMultipleDirectory(TCHAR* ptszInPath)
	{
		int iLen = 0;    
		TCHAR* ptTemp = NULL;		
		TCHAR tszPath[MAX_PATH] = {0};
		TCHAR tszTemPath[MAX_PATH] = {0};
		_tcscpy_s(tszPath, ptszInPath);         //存放要创建的目录字符串

		//检查参数是否正确
		if (!tszPath || _tcsclen(tszPath)<4)
			return false;

		//在这里去掉尾部为'\\'的字符
		if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
		{
			tszPath[_tcsclen(tszPath)-1] = 0;
		}

		ptTemp = _tcsrchr(tszPath, _T('\\'));  //从尾部查找字符
		iLen = _tcsclen(ptTemp);
		_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //得到父目录路径

		//检验父级路径是否存在  		
		if (ChickDirExist(tszTemPath))
		{   //如果存在则创建子目录
			if (!CreateDirectory(tszPath, NULL))
			{	//创建文件夹失败
				::MessageBox(NULL, tszPath, _T("创建文件夹失败!!!!!"), MB_OK);
				return false;
			}
			else
			{
				SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);	
				return true;
			}
			return true;
		}	
		else
		{	//如果不存在则递归
			if (!CreateMultipleDirectory(tszTemPath))
			{
				return false;
			}
			else
			{   //父目录创建完毕则创建子目录
				if (!CreateMultipleDirectory(tszPath))
					return false;		
			}
		}

		return true;
	}

	//获取当前程序所在目录 成功返回true，失败返回false
	bool GetExePath(TCHAR* ptszInPath)
	{
		TCHAR* ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//获取当前目录  //这里是获取当前进程文件的完整路径 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptszInPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, _T('\\'));
		memcpy(ptszInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}

	//获取指定文件长度（单位：字节） 
	DWORD GetFileLen(WCHAR* ptszInFilePath)
	{
		//第一种方法
		DWORD dwFileLen = 0;	
		HANDLE hFile = NULL;     
		hFile = CreateFile(ptszInFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);       
		dwFileLen = GetFileSize(hFile, NULL);       
		CloseHandle(hFile);     

		////第二种方法
		//FILE* pFile = NULL;
		//pFile = ::fopen(ptszInFilePath, "rb");
		//if(pFile == NULL)
		//	return 0;
		//
		//::fseek(pFile, 0, SEEK_SET);    //定位开始
		//dwFileLen = ::ftell(pFile);
		//fseek(pFile, 0, SEEK_END);  
		//dwFileLen = ::ftell(pFile) - dwFileLen;
		//fclose(pFile);
		//pFile = NULL;
		
		return dwFileLen;
	}
	//获取桌面路径
	char *GetDesktopPath()
{
	// Get the directory for Windows Desktop. This is
    // stored in the Registry under HKEY_CURRENT_USER\Software\
    // Microsoft\Windows\CurrentVersion\Explorer\Shell Folders\Desktop.
    HKEY hCU;
    DWORD lpType;
    ULONG ulSize = MAX_PATH;
	static char szPath[MAX_PATH];
	const char szRegKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
	
    if(RegOpenKeyEx(HKEY_CURRENT_USER, szRegKey, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
    {
		RegQueryValueEx(hCU, "Desktop", NULL, &lpType, (BYTE *)&szPath, &ulSize);
	}
	
	RegCloseKey(hCU);

	return szPath;
}

}
#endif

