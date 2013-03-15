#pragma once


//使用者线程状态
enum EnAppState
{
	ST_STARTING, ST_STARTED, ST_STOPING, ST_STOPED
};

struct info_msg
{
	DWORD connID;
	LPCTSTR evt;
	int contentLength;
	LPCTSTR content;

	static info_msg* Construct(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent);
	static void Destruct(info_msg* pMsg);

private:
	info_msg(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent);
	~info_msg();
};
