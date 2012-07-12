// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

int main()
{
	const int ENDNUMBER = -1;//哨兵，在遍历数组时遇到此数表示已经到达了数组的最后。
	//设置这个后，你可以在nFolders数组中任意添加删除数据而不用在遍历数组时考虑其中有多少个数据。

	CoInitialize(NULL);
	int nFolders[] = 
	{
		CSIDL_BITBUCKET, CSIDL_CONTROLS, CSIDL_DESKTOP, CSIDL_DESKTOPDIRECTORY, 
		CSIDL_DRIVES, CSIDL_FONTS, CSIDL_NETHOOD, CSIDL_NETWORK, CSIDL_PERSONAL, 
		CSIDL_PRINTERS, CSIDL_PROGRAMS, CSIDL_RECENT, CSIDL_SENDTO, CSIDL_STARTMENU, 
		CSIDL_STARTUP, CSIDL_TEMPLATES, ENDNUMBER
	};

	HRESULT       hr;
	LPITEMIDLIST  ppidl; 
	WCHAR wszPath[MAX_PATH] = {0};
	int i = 0;  

	while (nFolders[i] != ENDNUMBER)
	{
		//获得系统特殊路径
		hr = SHGetSpecialFolderLocation(NULL, nFolders[i], &ppidl); 
		if (hr != S_OK)
		{
			printf("SHGetSpecialFolderLocation Error\r\n");
		}
		else
		{
			if (SHGetPathFromIDList(ppidl, wszPath))
				printf("%ws\r\n", wszPath);
			else
				printf("SHGetPathFromIDList Error\r\n");
			CoTaskMemFree(ppidl);
		}
		i++;
	}
	CoUninitialize();
	return 0;
}

