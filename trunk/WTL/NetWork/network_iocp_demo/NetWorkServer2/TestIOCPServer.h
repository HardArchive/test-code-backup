#pragma once
#include "../common/com_module/socket/IocpServer.h"
#include "../common/com_module/socket/SocketHelper.h"
#include "../global/helper.h"
class CTestIOCPServer :
	public IServerSocketListener
{
public:
	CTestIOCPServer(void);
	~CTestIOCPServer(void);

	void Start();
	void Stop();

private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnAccept(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnServerShutdown();
private:
	CIocpServer m_Server;
};
