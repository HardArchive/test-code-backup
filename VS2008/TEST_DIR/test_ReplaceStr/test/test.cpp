// test.cpp : 定义控制台应用程序的入口点。
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
	char szTemp2[MAX_PATH] = {"D:\\360data\\重要数据\\桌面\\工作相关项目"};
	ReplaceStr(szTemp1,"-","");//把字符串中的“-”删除掉！

	ReplaceStr(szTemp2,"\\","/");
	return 0;
}



