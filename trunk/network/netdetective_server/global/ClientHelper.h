#pragma once
/***************************************************************************************************
* 1、 File       ： 客户端通信辅助类
* 2、 Version    ： 1.0
* 3、 Description： 服务器业务对外接口
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-3-20 14:49:54
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/

#include "socket_module/socket/SocketClient.h"
#include "socket_module/socket/SocketHelper.h"

#include "safestl.h"
#include "socket_module/Pool.h"
#include "comm_protocol.h"

class CClientHelper :
	public IClientSocketListener
{

public:
	CClientHelper(void);
	~CClientHelper(void);

public:
	bool Start(LPCTSTR pszBindAddress, USHORT usPort);	
	bool Stop();

	// 发送数据
	bool Send(const BYTE* pBuffer, int iLen);      //发送数据 连接ID 内容 长度
private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnConnect(DWORD dwConnectionID);

private:
	CSocketClient m_Client;
public:
	bool m_bRevcFlag;
};
