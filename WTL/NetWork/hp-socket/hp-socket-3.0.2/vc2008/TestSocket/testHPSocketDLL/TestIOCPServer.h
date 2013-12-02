#pragma once

//#include "../../../Common/Src/socket/HPSocket.h"
//#include "../Global/helper.h"
#include "HPSocket.h"
//#include "../Global/helper.h"

class CTestIOCPServer :	public CTcpPullServerListener
{
public:
	CTestIOCPServer(void);
	~CTestIOCPServer(void);

	void Start();
	void Stop();

private:
	virtual EnHandleResult OnPrepareListen(SOCKET soListen);   
	virtual ISocketListener::EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);                 //连接消息响应
	virtual ISocketListener::EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);    //发送消息响应
	virtual ISocketListener::EnHandleResult OnReceive(CONNID dwConnID, int iLength);                    //接收消息响应
	virtual ISocketListener::EnHandleResult OnClose(CONNID dwConnID);                                   //关闭消息响应 
	virtual ISocketListener::EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode); //出错响应
	virtual ISocketListener::EnHandleResult OnServerShutdown();                                         //服务器关闭响应
//private: 
//	TPkgInfo* FindPkgInfo(CONNID dwConnID);
//	void RemovePkgInfo(CONNID dwConnID);
private:
	CTcpPullServerWrapper m_Server;
};
