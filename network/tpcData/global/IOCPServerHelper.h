#pragma once
/***************************************************************************************************
* 1、 File       ： IOCP服务器管理辅助类
* 2、 Version    ： 1.0
* 3、 Description： 服务器业务对外接口
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-3-20 14:49:54
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/

#include "socket_module/socket/IocpServer.h"
#include "socket_module/socket/SocketHelper.h"
#include "ClientBusinessHelper.h"
#include "safestl.h"
#include "socket_module/Pool.h"
#include "comm_protocol.h"

class CIOCPServerHelper :
	public IServerSocketListener
{
	typedef struct CLIENT_INFO_TAG
	{
		CRITICAL_SECTION       stuCriSecLock;                //临界区对象  用于pclsClientBusinessHelper的同步操作
		bool                   bIsDelete;                    //客户端是否删除
		DATE                   dtLastActiveTime;             //客户端最后通信时间
		CLIENTBASEINFO         stuClientBaseInfo;            //客户端基本信息		
		CClientBusinessHelper* pclsClientBusinessHelper;     //客户端业务操作指针						

		CLIENT_INFO_TAG()
		{
			::InitializeCriticalSection(&stuCriSecLock);
			Reset();
		}
		~CLIENT_INFO_TAG()
		{
			Reset();
			::DeleteCriticalSection(&stuCriSecLock);
		}

		//更新最后通信时间
		inline void UpdateActiveTime()
		{
			dtLastActiveTime = COleDateTime::GetCurrentTime();
		}

		//检查连接是否超时
		inline bool CheckTimeout(const DWORD dwMinute)
		{
			COleDateTime dtCurrent = COleDateTime::GetCurrentTime();
			if (dtLastActiveTime + COleDateTimeSpan(0, 0, dwMinute, 0) > dtCurrent) return true;

			TRACE(_T("客户端连接超时; connID:%d;IP:%s; 最后通信时间:%s; 当前时间:%s\r\n"), 
				stuClientBaseInfo.dwConnID,
				stuClientBaseInfo.szIP,
				DateTimeToString(dtLastActiveTime), dtCurrent.Format(_T("%Y-%m-%d %H:%M:%S")));
			return false;
		}

		inline void Reset()
		{			
			stuClientBaseInfo.Reset();
			bIsDelete = false;                    //客户端是否删除
			dtLastActiveTime = 0;             //客户端最后通信时间					
			pclsClientBusinessHelper = NULL;
		}

		inline void Lock()		{::EnterCriticalSection(&stuCriSecLock);}
		inline void Unlock()	{::LeaveCriticalSection(&stuCriSecLock);}
	}CLIENTINFO, *PCLIENTINFO;

public:
	CIOCPServerHelper(void);
	~CIOCPServerHelper(void);

	bool Start(LPCTSTR pszBindAddress, USHORT usPort);	
	bool Stop();
	bool CloseLink(DWORD dwConnID);        //关闭指定连接 待实现

	// 发送数据
	bool Send(DWORD dwConnID, const BYTE* pBuffer, int iLen);      //发送数据 连接ID 内容 长度
private:

	//退出清空数据
	void Reset();

	//连接断开时清空某个连接数据
	void Reset(DWORD dwConnID);

private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnAccept(DWORD dwConnectionID, DWORD dwIPValue);
	virtual ISocketListener::EnHandleResult OnServerShutdown();	

public:
	// 接收对象池
	TPool<DATAPACKET> m_objRecvPacketPool;          //接收包对象池	

private:
	CIocpServer m_clsIOCPServer;	
	RG::SAFESTL::CMap<DWORD, PCLIENTINFO> m_mapConnIDClient;   //connID与客户端的对应关系  调用需作同步处理(CLIENTINFO的创建在CClientBusinessHelper之前，删除CClientBusinessHelper在之后)
	RG::SAFESTL::CQueue<PCLIENTINFO>      m_queueDeleteClient; //已删除客户端指针信息队列  这里的调用也需处理同步

};
