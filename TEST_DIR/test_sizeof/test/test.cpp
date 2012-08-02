// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIP;//IP值
	//域名信息 可以根据实际域名的个数长及长度定义此缓冲区,域名信息的存放格式为 域名长度:域名
	//域名长度占一个字节,紧接着域名长度的是域名信息 然后是下一个域名长度和域名信息 按此格式一直排下去
	char buff[32];  //这个buff长度不固定
}DomainInfo;

int _tmain(int argc, _TCHAR* argv[])
{
	char szbuf[1024] = {0};
	int iLen1 = sizeof(DWORD);
	int iLen2 = sizeof(DomainInfo);
	DomainInfo* pDomainInfo = (DomainInfo*)szbuf;

	pDomainInfo->dwIPId = 100;
	pDomainInfo->dwIP = 115;

	strcpy(pDomainInfo->buff, "asdgkladfjghkladfjgkldfgorthdfkalgafklgkldfgkldf");
	strcpy(pDomainInfo->buff, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");

	//有效长度
	int iLen3 = sizeof(DomainInfo)-32 + strlen("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");



	return 0;
}

