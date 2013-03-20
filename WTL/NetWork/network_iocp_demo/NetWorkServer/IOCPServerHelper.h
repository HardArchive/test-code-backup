#pragma once
#include "../common/com_module/socket/IocpServer.h"
#include "../common/com_module/socket/SocketHelper.h"
#include "../global/helper.h"
class CIOCPServerHelper :
	public IServerSocketListener
{
public:
	CIOCPServerHelper(void);
	~CIOCPServerHelper(void);

	bool Start(LPCTSTR pszBindAddress, USHORT usPort);
	bool Send(DWORD dwConnID, const BYTE* pBuffer, int iLen);  //发送数据 连接ID 内容 长度
	bool Stop();
	bool CloseLink(DWORD dwConnID);        //关闭指定连接

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
