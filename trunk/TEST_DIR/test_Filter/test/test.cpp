// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>

//过滤特殊符号 '和,
void FilterSpecialsymbols(BYTE* pbInOutBuf, int iBufLen)
{
	int iTemLen = 0;
	int iCount = iBufLen;	
	PBYTE pbTempBuf = new BYTE[iBufLen];
	PBYTE pbTemp = pbTempBuf;
	PBYTE pbTemp2 = pbInOutBuf;
	memset(pbTempBuf, 0, iBufLen);
	while (iCount)
	{
		if (','!=*pbTemp2 && '\''!=*pbTemp2)
		{
			*pbTemp = *pbTemp2;
			iTemLen++;
			pbTemp++;
		}
		iCount--;
		pbTemp2++;
	}
	memset(pbInOutBuf, 0, iBufLen);
	memcpy(pbInOutBuf, pbTempBuf, iTemLen);
};


int _tmain(int argc, _TCHAR* argv[])
{
	char szTem[MAX_PATH] = {0};
	strcpy(szTem, "你好,张三！！！'sql执行代码……', abc '123456'\r\n");

	FilterSpecialsymbols((BYTE*)szTem, strlen(szTem));
	printf("%s", szTem);
	return 0;
}

