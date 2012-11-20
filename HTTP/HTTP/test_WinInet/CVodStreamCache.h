
#ifndef _CVodStreamCache_H
#define _CVodStreamCache_H

#include <iostream>
#include <Windows.h>
#include <WinInet.h>
#include <tchar.h>
#include <string>
using namespace std;

#pragma comment(lib, "wininet.lib")


class CVodStreamCache
{
public:
	HANDLE Create(char token[], char path[]);
	int ReadBlock(long offset, int len, char *buffer);
	int WriteBlock();
	void Destory();
	void CacheInfo();
protected:
private:
	HINTERNET internetOpen;
	HINTERNET internetOpenUrl;

	DWORD dwStatusCode;
	DWORD dwContentLength;

	HANDLE createFile;
	static const int BUFFERSIZE = 1024;
};


#endif