
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
	void CacheInfo();	// ��ȡ������Ϣ��״̬�ȵ�

	void InitSocketLib();	// ��ʼ��Socket������
	void ParseURL(char* url);	// ����URL��ַ
	void FormatRequestHeader(char* pHost, char* pGet, long lFrom=0, long lTo=0);	// ��ʽ������ͷ
	void Socket();	// ����SOCKET����
	void Connect(char* pHost, int port=80);	// ��������
	void SendRequest(const char* requestHeader);	// ��������ͷ
	void GetResponseHeader();	// ��ȡ�����ķ���ͷ
	void SetTimeOut(int nTime, int type=0);	// ���ý��ջ��͵��ʱ��
	void InitCache();	// ��ʼ��Cache����ȡ�ļ�����

protected:

private:
	BOOL err;
	char *pHost;	// ������ַ
	char *pGet;	// GET ���·��
	char *requestHeader;	// ����ͷ
	SOCKET sock;
	char *responseHeader;	// ��Ӧͷ
	fstream cacheFile;	// ��������ļ���

	Cache cache;

};


#endif