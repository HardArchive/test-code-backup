#pragma once
/***************************************************************************************************
* 1�� File       �� IOCP��������������
* 2�� Version    �� 1.0
* 3�� Description�� ������ҵ�����ӿ�
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-3-20 14:49:54
* 6�� History    �� 
* 7�� Remark     �� 
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
		CRITICAL_SECTION       stuCriSecLock;                //�ٽ�������  ����pclsClientBusinessHelper��ͬ������
		bool                   bIsDelete;                    //�ͻ����Ƿ�ɾ��
		DATE                   dtLastActiveTime;             //�ͻ������ͨ��ʱ��
		CLIENTBASEINFO         stuClientBaseInfo;            //�ͻ��˻�����Ϣ		
		CClientBusinessHelper* pclsClientBusinessHelper;     //�ͻ���ҵ�����ָ��						

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

		//�������ͨ��ʱ��
		inline void UpdateActiveTime()
		{
			dtLastActiveTime = COleDateTime::GetCurrentTime();
		}

		//��������Ƿ�ʱ
		inline bool CheckTimeout(const DWORD dwMinute)
		{
			COleDateTime dtCurrent = COleDateTime::GetCurrentTime();
			if (dtLastActiveTime + COleDateTimeSpan(0, 0, dwMinute, 0) > dtCurrent) return true;

			TRACE(_T("�ͻ������ӳ�ʱ; connID:%d;IP:%s; ���ͨ��ʱ��:%s; ��ǰʱ��:%s\r\n"), 
				stuClientBaseInfo.dwConnID,
				stuClientBaseInfo.szIP,
				DateTimeToString(dtLastActiveTime), dtCurrent.Format(_T("%Y-%m-%d %H:%M:%S")));
			return false;
		}

		inline void Reset()
		{			
			stuClientBaseInfo.Reset();
			bIsDelete = false;                    //�ͻ����Ƿ�ɾ��
			dtLastActiveTime = 0;             //�ͻ������ͨ��ʱ��					
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
	bool CloseLink(DWORD dwConnID);        //�ر�ָ������ ��ʵ��

	// ��������
	bool Send(DWORD dwConnID, const BYTE* pBuffer, int iLen);      //�������� ����ID ���� ����
private:

	//�˳��������
	void Reset();

	//���ӶϿ�ʱ���ĳ����������
	void Reset(DWORD dwConnID);

private:
	virtual ISocketListener::EnHandleResult OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength);
	virtual ISocketListener::EnHandleResult OnClose(DWORD dwConnectionID);
	virtual ISocketListener::EnHandleResult OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode);
	virtual ISocketListener::EnHandleResult OnAccept(DWORD dwConnectionID, DWORD dwIPValue);
	virtual ISocketListener::EnHandleResult OnServerShutdown();	

public:
	// ���ն����
	TPool<DATAPACKET> m_objRecvPacketPool;          //���հ������	

private:
	CIocpServer m_clsIOCPServer;	
	RG::SAFESTL::CMap<DWORD, PCLIENTINFO> m_mapConnIDClient;   //connID��ͻ��˵Ķ�Ӧ��ϵ  ��������ͬ������(CLIENTINFO�Ĵ�����CClientBusinessHelper֮ǰ��ɾ��CClientBusinessHelper��֮��)
	RG::SAFESTL::CQueue<PCLIENTINFO>      m_queueDeleteClient; //��ɾ���ͻ���ָ����Ϣ����  ����ĵ���Ҳ�账��ͬ��

};
