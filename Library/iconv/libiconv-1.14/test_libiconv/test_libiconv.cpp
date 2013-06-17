// test_libiconv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <iconv.h>
//#define LIBICONV_PLUG
#include "iconv.h"
#pragma comment(lib, "libiconv.lib");
#define OUTLEN 255

int covert(char *desc, char *src, const char *input, size_t ilen, char *output, size_t olen)
{
	const char **pin = &input;
	char **pout = &output;
	iconv_t cd = iconv_open(desc, src);
	if (cd == (iconv_t)-1)
	{
		return -1;
	}
	memset(output, 0, olen);
	if (iconv(cd, pin, &ilen, pout, &olen)) return -1;
	iconv_close(cd);
	return 0;
}

int main1(int argc, char *argv[])
{
	char *input = "中国";
	size_t len = strlen(input);
	char *output = (char *)malloc(OUTLEN);
	covert("UTF-8", "GBK", input, len, output, OUTLEN);
	printf("%s\n", output);
	return 0;
}


