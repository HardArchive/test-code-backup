#include "stdafx.h"
#include "helper.h"
#include "../../Common/Src/FuncHelper.h"

CWnd*		g_pMainWnd;
CListBox*	g_pInfoList;

info_msg* info_msg::Construct(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent)
{
	return new info_msg(dwConnectionID, lpszEvent, iContentLength, lpszContent);
}

void info_msg::Destruct(info_msg* pMsg)
{
	delete pMsg;
}

info_msg::info_msg(DWORD dwConnectionID, LPCTSTR lpszEvent, int iContentLength, LPCTSTR lpszContent)
	: connID(dwConnectionID), evt(lpszEvent), contentLength(iContentLength), content(lpszContent)
{

}

info_msg::~info_msg()
{
	if(contentLength > 0)
		delete[] content;
}

void SetMainWnd(CWnd* pWnd)
{
	g_pMainWnd = pWnd;
}

void SetInfoList(CListBox* pInfoList)
{
	g_pInfoList = pInfoList;
}

void LogServerStart(LPCTSTR lpszAddress, USHORT port)
{
	CString msg;
	msg.Format(_T("$ Server Start OK --> (%s : %d)"), lpszAddress, port);
	LogMsg(msg);
}

void LogServerStartFail(DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ Server Start Fail --> %s (%d)"), lpszDesc, code);
	LogMsg(msg);
}

void LogServerStop()
{
	CString msg = _T("$ Server Stop");
	LogMsg(msg);
}

void LogServerStopFail(DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ Server Stop Fail --> %s (%d)"), lpszDesc, code);
	LogMsg(msg);
}

void LogClientStart(LPCTSTR lpszAddress, USHORT port)
{
	CString msg;
	msg.Format(_T("$ Client Start OK --> (%s : %d)"), lpszAddress, port);
	LogMsg(msg);
}

void LogClientStartFail(DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ Client Start Fail --> %s (%d)"), lpszDesc, code);
	LogMsg(msg);
}

void LogClientStop(DWORD dwConnectionID)
{
	CString msg;
	msg.Format(_T("$ Client Stop --> (%d)"), dwConnectionID);
	LogMsg(msg);
}

void LogClientStopFail(DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ Client Stop Fail --> %s (%d)"), lpszDesc, code);
	LogMsg(msg);
}

void LogClientSendFail(int iSequence, int iSocketIndex, DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ Client Send Fail [SOCK: %d, SEQ: %d] --> %s (%d)"), iSocketIndex, iSequence, lpszDesc, code);
	LogMsg(msg);
}

void LogSend(DWORD dwConnectionID, LPCTSTR lpszContent)
{
	CString msg;
	msg.Format(_T("$ (%d) Send OK --> %s"), dwConnectionID, lpszContent);
	LogMsg(msg);
}

void LogSendFail(DWORD dwConnectionID, DWORD code, LPCTSTR lpszDesc)
{
	CString msg;
	msg.Format(_T("$ (%d) Send Fail --> %s (%d)"), dwConnectionID, lpszDesc, code);
	LogMsg(msg);
}

void LogOnConnect(DWORD dwConnectionID)
{
	CString msg;
	msg.Format(_T("  > [ %d, %s ]"), dwConnectionID, EVT_ON_CONNECT);
	LogMsg(msg);
}

void PostOnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	/*
	LPTSTR lpszContent = NULL;
	int content_len = ::BytesToHex(pData, iLength, &lpszContent);
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_SEND, content_len, lpszContent);
	*/

	LPTSTR lpszContent = new TCHAR[20];
	wsprintf(lpszContent, _T("(%d bytes)"), iLength);
	int content_len = lstrlen(lpszContent);
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_SEND, content_len, lpszContent);

	PostInfoMsg(msg);
}

void PostOnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	/*
	LPTSTR lpszContent = NULL;
	int content_len = ::BytesToHex(pData, iLength, &lpszContent);
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_RECEIVE, content_len, lpszContent);
	*/

	LPTSTR lpszContent = new TCHAR[20];
	wsprintf(lpszContent, _T("(%d bytes)"), iLength);
	int content_len = lstrlen(lpszContent);
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_RECEIVE, content_len, lpszContent);

	PostInfoMsg(msg);
}

void PostOnClose(DWORD dwConnectionID)
{
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_CLOSE, 0, NULL);

	PostInfoMsg(msg);
}

void PostOnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	LPTSTR lpszContent = new TCHAR[100];
	wsprintf(lpszContent, _T("OP: %d, CODE: %d"), enOperation, iErrorCode);
	int content_len = lstrlen(lpszContent);
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_ERROR, content_len, lpszContent);

	PostInfoMsg(msg);
}

void PostOnAccept(DWORD dwConnectionID)
{
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_ACCEPT, 0, NULL);

	PostInfoMsg(msg);
}

void PostOnConnect(DWORD dwConnectionID)
{
	info_msg* msg = info_msg::Construct(dwConnectionID, EVT_ON_CONNECT, 0, NULL);

	PostInfoMsg(msg);
}

void PostOnShutdown()
{
	info_msg* msg = info_msg::Construct(0, EVT_ON_SHUTDOWN, 0, NULL);

	PostInfoMsg(msg);
}

void PostTimeConsuming(DWORD dwTickCount)
{
	LPTSTR lpszContent = new TCHAR[100];
	wsprintf(lpszContent, _T("Total Time Consuming: %u"), dwTickCount);
	int content_len = lstrlen(lpszContent);
	info_msg* msg = info_msg::Construct(0, EVT_ON_END_TEST, content_len, lpszContent);

	PostInfoMsg(msg);
}

void PostInfoMsg(info_msg* msg)
{
	if(g_pMainWnd->GetSafeHwnd() == NULL || !g_pMainWnd->PostMessage(USER_INFO_MSG, (WPARAM)msg))
		info_msg::Destruct(msg);
}

void LogInfoMsg(info_msg* pInfoMsg)
{
	CString msg;

	if(pInfoMsg->connID > 0)
	{
		if(pInfoMsg->contentLength > 0)
			msg.Format(_T("  > [ %d, %s ] -> %s"), pInfoMsg->connID, pInfoMsg->evt, pInfoMsg->content);
		else
			msg.Format(_T("  > [ %d, %s ]"), pInfoMsg->connID, pInfoMsg->evt);
	}
	else
	{
		if(pInfoMsg->contentLength > 0)
			msg.Format(_T("  > [ %s ] -> %s"), pInfoMsg->evt, pInfoMsg->content);
		else
			msg.Format(_T("  > [ %s ]"), pInfoMsg->evt);
	}

	LogMsg(msg);
}

void LogMsg(const CString& msg)
{
	if(!g_pInfoList || !g_pInfoList->GetSafeHwnd())
		return;

	int iCurIndex	= g_pInfoList->GetCurSel();
	int iCount		= g_pInfoList->GetCount();
	BOOL bCurLast	= (iCurIndex != LB_ERR && iCurIndex == iCount - 1);

	g_pInfoList->SetRedraw(FALSE);

	if(iCount > MAX_LOG_RECORD_LENGT)
		g_pInfoList->DeleteString(0);

	g_pInfoList->AddString(msg);

	iCount = g_pInfoList->GetCount();

	if(bCurLast)
		g_pInfoList->SetCurSel(iCount - 1);

	iCurIndex = g_pInfoList->GetCurSel();
	if(iCurIndex == LB_ERR)
	{
		iCurIndex = 0;
		g_pInfoList->SetCurSel(iCurIndex);
	}

	g_pInfoList->SetAnchorIndex(iCurIndex);
	g_pInfoList->SetRedraw(TRUE);
}