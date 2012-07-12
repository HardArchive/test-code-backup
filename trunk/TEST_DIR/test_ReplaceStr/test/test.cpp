// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int ReplaceStr(char* sSrc, char* sMatchStr, char* sReplaceStr)
{
	int StringLen;
	char caNewString[64];
	char* FindPos;
	FindPos =(char *)strstr(sSrc, sMatchStr);
	if( (!FindPos) || (!sMatchStr) )
		return -1;

	while( FindPos )
	{
		memset(caNewString, 0, sizeof(caNewString));
		StringLen = FindPos - sSrc;
		strncpy(caNewString, sSrc, StringLen);
		strcat(caNewString, sReplaceStr);
		strcat(caNewString, FindPos + strlen(sMatchStr));
		strcpy(sSrc, caNewString);

		FindPos =(char *)strstr(sSrc, sMatchStr);
	}
	free(FindPos);
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	char szTemp1[MAX_PATH] = {"abcd-efgh-ijklm-nopq"};
	char szTemp2[MAX_PATH] = {"D:\\360data\\��Ҫ����\\����\\���������Ŀ"};
	ReplaceStr(szTemp1,"-","");//���ַ����еġ�-��ɾ������

	ReplaceStr(szTemp2,"\\","/");
	return 0;
}



