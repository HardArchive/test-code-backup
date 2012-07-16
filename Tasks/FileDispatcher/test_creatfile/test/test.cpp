// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

//创建多级目录，如不存在则创建 失败返回false，其他返回true
bool CreatePath(char* pDirPath)
{
	int iNum = 0;
	char* pDir = NULL;
	char szParentDir[MAX_PATH] = {0};
	char szTempDir[MAX_PATH] = {0};

	//检验路径是否存在
	if (GetFileAttributes(pDirPath) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	pDir = (char*)strrchr(pDirPath, '\\');  //从尾部查找字符
	iNum = strlen(pDir);
	if (1 == iNum)
	{
		strncpy(szTempDir, pDirPath, strlen(pDirPath)-1);  //父目录名
		pDir = (char*)strrchr(szTempDir, '\\');
		//pwDir += 1;
		iNum += strlen(pDir);
	}

	strncpy(szParentDir, pDirPath, strlen(pDirPath)-iNum);

	//检验父级路径是否存在  如果存在则创建子目录
	if (GetFileAttributes(szParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(pDirPath, NULL))
		{
			//创建文件夹失败
			//FindClose(hParentFile);
			::MessageBox(NULL, pDirPath, "创建文件夹失败", MB_OK);
			return false;
		}
		else
		{
			SetFileAttributes(pDirPath, FILE_ATTRIBUTE_NORMAL);	
			return true;
		}
	}
	else
	{
		//如果不存在则递归
		if (!CreatePath(szParentDir))
		{
			return false;
		}
		else
		{//父目录创建完毕则创建子目录
			if (!CreatePath(pDirPath))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}

bool CopyFolder(char* pExistingFolderPath, char* pNewFolderPath)
{
	int iRet = 0;
	int iFileNum = 0;   //当前文件夹下文件数量 为负值时遍历出错
	HANDLE hFind = NULL;
	WIN32_FIND_DATA stuWFD = {0};	
	char szFullPath[MAX_PATH] = {0};
	char szNewFullPath[MAX_PATH] = {0};
	char szFindFilter[MAX_PATH] = {0};
	DWORD dwAttributes = 0;

	if (!(pExistingFolderPath && pNewFolderPath))
	{
		return -1;
	}
	//检查创建新文件夹路径
	if (!CreatePath(pNewFolderPath))
	{
		return -2;
	}

	sprintf(szFindFilter, "%s\\*.*", pExistingFolderPath);    //查找文件类型

	if ((hFind = FindFirstFile(szFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
	{
		return -3;
	}

	do
	{
		//过滤本级目录和父目录
		if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
			continue;

		//如果找到的是目录，则进入此目录进行递归
		if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			ZeroMemory(szFullPath, sizeof(szFullPath));
			sprintf(szFullPath, "%s\\%s", pExistingFolderPath, stuWFD.cFileName);  //得到文件夹完整路径
			sprintf(szNewFullPath, "%s\\%s", pNewFolderPath, stuWFD.cFileName);    //新文件夹完整路径
			if (1 != CopyFolder(szFullPath, szNewFullPath))
			{
				FindClose(hFind);
				return -4;
			}
		}
		else 
		{ 
			//if (!bIsRoot)
			{
				//找到的是文件
				sprintf(szFullPath, "%s\\%s", pExistingFolderPath, stuWFD.cFileName); //得到文件完整路径
				sprintf(szNewFullPath, "%s\\%s", pNewFolderPath, stuWFD.cFileName);
				if (!CopyFile(szFullPath, szNewFullPath, FALSE))
				{
					FindClose(hFind);
					return -4;
				}
			}
		}	
		//iFileNum++;
	}while (FindNextFile(hFind, &stuWFD));

	FindClose(hFind);

	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iCount = 10000;
	int wMinute = 0;
	char szFolderName[MAX_PATH] = {0};
	char szNewFolderPath[MAX_PATH] = {0};
	SYSTEMTIME stuSystemTime = {0};
	//D:\WinPath\desktop\test\dest\201204280922  已存在文件夹
	//D:\WinPath\desktop\test  目标位置
	GetLocalTime(&stuSystemTime);
	
	while(iCount)
	{
		wMinute = stuSystemTime.wMinute;
		while(true)
		{
			if (wMinute+1 == stuSystemTime.wMinute)
			{
				break;
			}
			if (wMinute-59 == stuSystemTime.wMinute)
			{
				break;
			}

			GetLocalTime(&stuSystemTime);
			Sleep(1000);
		}
		sprintf(szFolderName, "%04d%02d%02d%02d%02d", stuSystemTime.wYear, stuSystemTime.wMonth, stuSystemTime.wDay, stuSystemTime.wHour, stuSystemTime.wMinute);
		sprintf(szNewFolderPath, "D:\\WinPath\\desktop\\test\\%s", szFolderName);
		printf(szNewFolderPath);
		printf("\r\n");

		if (1!=CopyFolder("D:\\WinPath\\desktop\\test\\dest\\201204280922", szNewFolderPath))
		{
			printf("文件夹拷贝失败！！！！！");
			printf("\r\n");
			return FALSE;
		}

		iCount--;
	}
	return 0;
}

