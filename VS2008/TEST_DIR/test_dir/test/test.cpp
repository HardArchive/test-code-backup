// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

bool WriteDirectory(const WCHAR* pwDir);

int _tmain(int argc, _TCHAR* argv[])
{
	WCHAR wszDir[MAX_PATH] = {0};
	wcscpy(wszDir, L"C:\\WINDOWS\\system32\\oobe\\info\\backgrounds\\");
	bool bflag = WriteDirectory(wszDir);
	return 0;
}


bool WriteDirectory(const WCHAR* pwInDir)
{
	int iNum = 0;
	WCHAR* pwDir = NULL;
	WCHAR wszParentDir[MAX_PATH] = {0};
	WCHAR wszTempDir[MAX_PATH] = {0};

	//检验路径是否存在
	if (GetFileAttributes(pwInDir) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return TRUE;
	}

	pwDir = (WCHAR*)wcsrchr(pwInDir, L'\\');  //从尾部查找字符
	iNum = wcslen(pwDir);
	if (1 == iNum)
	{
		wcsncpy(wszTempDir, pwInDir, wcslen(pwInDir)-1);
		pwDir = (WCHAR*)wcsrchr(wszTempDir, L'\\');
		//pwDir += 1;
		iNum += wcslen(pwDir);
	}

	wcsncpy(wszParentDir, pwInDir, (wcslen(pwInDir)-iNum));

	//检验父级路径是否存在  如果存在则创建子目录
	if (GetFileAttributes(wszParentDir) == FILE_ATTRIBUTE_DIRECTORY)
	{		
		if (!CreateDirectory(pwInDir, NULL))
		{
			//创建文件夹失败
			//FindClose(hParentFile);
			::MessageBox(NULL, pwInDir, L"创建文件夹失败", MB_OK);
			return false;
		}
		else
		{
			SetFileAttributes(pwInDir, FILE_ATTRIBUTE_NORMAL);	
			return true;
		}
	}
	else
	{
		//如果不存在则递归
		if (!WriteDirectory(wszParentDir))
		{
			return false;
		}
		else
		{//父目录创建完毕则创建子目录
			if (!WriteDirectory(pwInDir))
			{
				return false;
			}
			return true;
		}
	}

	return true;
}

BOOL createMultiDir(const TCHAR* pszDir)
{
	TCHAR szTmp[MAX_PATH] = {0};

	BOOL bSuccess = TRUE;

	TCHAR* pszHeader = szTmp;

	wsprintf(szTmp, _T("%s"), pszDir);

	int nLen = lstrlen(szTmp);

	for(int i = 0;  i < nLen; i++)
	{
		TCHAR* pch = _tcschr(szTmp + i, _T('\\'));

		if(pch)
		{
			i = (pch - szTmp);

			*pch = 0;

			// directory is exist
			if(::GetFileAttributes(szTmp) == FILE_ATTRIBUTE_DIRECTORY)
			{
				*pch = _T('\\');

				continue;
			}
			else
			{
				if(!::CreateDirectory(szTmp, NULL))
				{
					bSuccess = FALSE;

					break;
				}
			}

			*pch = _T('\\');
		}
	}

	return bSuccess;
}
