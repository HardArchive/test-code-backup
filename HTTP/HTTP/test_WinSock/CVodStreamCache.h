
#ifndef CVodStreamCache_H_
#define CVodStreamCache_H_

#include <iostream>
#include <WinSock2.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;


class Cache
{
public:
	Cache();
	~Cache();
	void setTotalLen(long totalLen);
	void setRecvLen(long recvLen);
	long getTotalLen();
	long getRecvLen();

protected:

private:
	char *filename;
	long totalLen;
	long recvLen;

};


class CVodStreamCache
{
public:
	CVodStreamCache();
	~CVodStreamCache();

	HANDLE Create(char token[], char path[]);
	int ReadBlock(long offset, int len, char *buff);
	int WriteBlock(long offset, int len, char *buff);
	void Destroy();
	void CacheInfo();	// 获取缓存信息，状态等等

	void InitSocketLib();	// 初始化Socket函数库
	void ParseURL(char* url);	// 解析URL地址
	void FormatRequestHeader(char* pHost, char* pGet, long lFrom=0, long lTo=0);	// 格式化请求头
	void Socket();	// 创建SOCKET对象
	void Connect(char* pHost, int port=80);	// 建立连接
	void SendRequest(const char* requestHeader);	// 发送请求头
	void GetResponseHeader();	// 获取完整的返回头
	void SetTimeOut(int nTime, int type=0);	// 设置接收或发送的最长时间
	void InitCache();	// 初始化Cache，获取文件长度

protected:

private:
	BOOL err;
	char *pHost;	// 主机地址
	char *pGet;	// GET 相对路径
	char *requestHeader;	// 请求头
	SOCKET sock;
	char *responseHeader;	// 回应头
	fstream cacheFile;	// 输入输出文件流

	Cache cache;

};


#endif