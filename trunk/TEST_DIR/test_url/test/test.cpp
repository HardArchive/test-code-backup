// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

int main()
{
	const int ENDNUMBER = -1;//�ڱ����ڱ�������ʱ����������ʾ�Ѿ���������������
	//����������������nFolders�������������ɾ�����ݶ������ڱ�������ʱ���������ж��ٸ����ݡ�

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
		//���ϵͳ����·��
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

