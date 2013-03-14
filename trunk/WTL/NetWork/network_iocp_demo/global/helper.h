#pragma once

#include "../common/com_module/socket/SocketHelper.h"

#define USER_INFO_MSG			(WM_USER + 100)
#define MAX_LOG_RECORD_LENGT	1000

#define EVT_ON_SEND			_T("OnSend")
#define EVT_ON_RECEIVE		_T("OnReceive")
#define EVT_ON_CLOSE		_T("OnClose")
#define EVT_ON_ERROR		_T("OnError")
#define EVT_ON_ACCEPT		_T("OnAccept")
#define EVT_ON_CONNECT		_T("OnConnect")
#define EVT_ON_SHUTDOWN		_T("OnShutdown")
#define EVT_ON_END_TEST		_T("END TEST")

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

void SetMainWnd(CWnd* pWnd);
void SetInfoList(CListBox* pInfoList);
void LogServerStart(LPCTSTR lpszAddress, USHORT port);
void LogServerStartFail(DWORD code, LPCTSTR lpszDesc);
void LogServerStop();
void LogServerStopFail(DWORD code, LPCTSTR lpszDesc);
void LogClientStart(LPCTSTR lpszAddress, USHORT port);
void LogClientStartFail(DWORD code, LPCTSTR lpszDesc);
void LogClientStop(DWORD dwConnectionID);
void LogClientStopFail(DWORD code, LPCTSTR lpszDesc);
void LogSend(DWORD dwConnectionID, LPCTSTR lpszContent);
void LogClientSendFail(int iSequence, int iSocketIndex, DWORD code, LPCTSTR lpszDesc);
void LogSendFail(DWORD dwConnectionID, DWORD code, LPCTSTR lpszDesc);
void LogOnConnect(DWORD dwConnectionID);
void LogMsg(const CString& msg);

void PostOnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
void PostOnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
void PostOnClose(DWORD dwConnectionID);
void PostOnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
void PostOnAccept(DWORD dwConnectionID);
void PostOnConnect(DWORD dwConnectionID);
void PostOnShutdown();
void PostTimeConsuming(DWORD dwTickCount);
void PostInfoMsg(info_msg* msg);
void LogInfoMsg(info_msg* pInfoMsg);