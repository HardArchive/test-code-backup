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
	virtual ISocketListener::EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);                 //������Ϣ��Ӧ
	virtual ISocketListener::EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);    //������Ϣ��Ӧ
	virtual ISocketListener::EnHandleResult OnReceive(CONNID dwConnID, int iLength);                    //������Ϣ��Ӧ
	virtual ISocketListener::EnHandleResult OnClose(CONNID dwConnID);                                   //�ر���Ϣ��Ӧ 
	virtual ISocketListener::EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode); //������Ӧ
	virtual ISocketListener::EnHandleResult OnServerShutdown();                                         //�������ر���Ӧ
//private: 
//	TPkgInfo* FindPkgInfo(CONNID dwConnID);
//	void RemovePkgInfo(CONNID dwConnID);
private:
	CTcpPullServerWrapper m_Server;
};
