// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//



#include <windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib, "Psapi.Lib");

/*这是一个很低效的算法*/
unsigned char *memstr(char * dst , int dst_len, char *src , int src_len )
{
	int i;
	char *cp = src;
	if (src_len < dst_len)
	{
		return NULL;
	}
	for (i = 0; i <= src_len - dst_len; i++)
	{
		if (memcmp(cp , dst , dst_len) == 0)
		{
			return (unsigned char *)cp;
		}
		cp++;
	}
	return   NULL;
}

/*sunday算法*/
#define MAX_CHAR_SIZE 257
long *setCharStep(const unsigned char *subStr, long subStrLen)
{
	long i;
	static long charStep[MAX_CHAR_SIZE];
	for (i = 0; i < MAX_CHAR_SIZE; i++)
		charStep[i] = subStrLen + 1;
	for (i = 0; i < subStrLen; i++)
	{
		charStep[(unsigned char)subStr[i]] = subStrLen - i;
	}
	return charStep;
}
/*
算法核心思想，从左向右匹配，遇到不匹配的看大串中匹配范围之外的右侧第一个字符在小串中的最右位置
根据事先计算好的移动步长移动大串指针，直到匹配
*/
long sundaySearch(const unsigned char *mainStr, const unsigned char *subStr, long *charStep, long mainStrLen, int subStrLen)
{
	long main_i = 0;
	long sub_j = 0;
	while (main_i < mainStrLen)
	{
		//保存大串每次开始匹配的起始位置，便于移动指针
		long tem = main_i;
		while (sub_j < subStrLen)
		{
			if (mainStr[main_i] == subStr[sub_j])
			{
				main_i++;
				sub_j++;
				continue;
			}
			else
			{
				//如果匹配范围外已经找不到右侧第一个字符，则匹配失败
				if (tem + subStrLen > mainStrLen)
					return -1;
				//否则 移动步长 重新匹配
				unsigned char firstRightChar = mainStr[tem + subStrLen];
				main_i += charStep[(unsigned char)firstRightChar];
				sub_j = 0;
				break; //退出本次失败匹配 重新一轮匹配
			}
		}
		if (sub_j == subStrLen)
			return main_i - subStrLen;
	}
	return -1;
}

unsigned char getHex(unsigned char hex)
{
	if (hex >= '0' && hex <= '9') return hex - '0';
	if (hex >= 'A' && hex <= 'F') return hex - 'A' + 10;
	if (hex >= 'a' && hex <= 'f') return hex - 'a' + 10;
	return 0;
}

int GetHexValue(char *src)
{
	int i, j, flag;
	static char temp[1024];
	for (i = 0, j = 0; src[i] != 0; i++)
	{
		if ((src[i] <= 'F' && src[i] >= 'A') || (src[i] <= 'f' && src[i] >= 'a') || (src[i] <= '9' && src[i] >= '0'))
		{
			if (src[i] != ' ')
			{
				temp[j++] = src[i];
			}
		}
	}
	temp[j] = 0;
	src[0] = 0;
	for (i = 0, j = 0, flag = 1; temp[i] != 0; i++)
	{

		char ch = getHex(temp[i]);
		if (ch != -1)
		{
			if (flag == 1) src[j] = ch << 4;
			else src[j++] += ch;
			flag *= -1;
		}
	}
	src[j] = 0;
	return j;
}

DWORD ReadPage(HANDLE m_hProcess, DWORD dwBaseAddr, char* Value)
{
	//读取1页内存
	BYTE arBytes[4096];
	if (!::ReadProcessMemory(m_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
	{
		//此页不可读
		return (DWORD) - 1;
	}
	else
	{
		//
		//unsigned char key[] = {0x80, 0x7f, 0x49, 0x00};
		unsigned char Value2[1024];
		strcpy((char*)Value2, Value);
		int len = GetHexValue((char*)Value2);
		//getchar();
		//注释这两行是低效的算法
		//char key[] = {0x80, 0x7f, 0x49, 0x00};
		//int len = 4;
		//if (memstr(key, len, (char*)arBytes, 4096) != 0) return memstr(key, len, (char*)arBytes, 4096) - (unsigned char*)arBytes;
		//else return -1;

		//开始sunday算法
		long *charStep = setCharStep(Value2, len);
		return sundaySearch(arBytes, Value2, charStep, 4096, len);
	}
	return (DWORD) - 1;    //不会执行到此处
}

DWORD aobscan(DWORD dwPid, char* Value)
{
	if (dwPid == 0) return (DWORD) - 1;

	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (hProcess != NULL)
	{
		//获得内存大小
		PROCESS_MEMORY_COUNTERS pmc;
		pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
		::GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));

		//遍历内存
		for (int i = 0; i < pmc.WorkingSetSize; i += 4096)
		{
			DWORD dwValue = ReadPage(hProcess, i, Value);
			if (dwValue != -1)
			{
				printf("Found:0x%X\n", i + dwValue);
				return i + dwValue;
			}
		}
		::CloseHandle(hProcess);
	}
	printf("Nothing Found!\n");
	return (DWORD) - 1;
}

DWORD GetGameID()
{
	HWND hwnd = FindWindow("MainWindow", NULL);
	if (hwnd)
	{
		DWORD dwPid;
		GetWindowThreadProcessId(hwnd, &dwPid);
		return dwPid;
	}
	return 0;
}

int main()
{
	double begin = clock();

	aobscan(GetGameID(), "807f49 00");

	printf("time:%.0fms", clock() - begin);
	getchar();
}
