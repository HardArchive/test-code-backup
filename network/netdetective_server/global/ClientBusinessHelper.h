#pragma once
#include "socket_module/socket/IocpServer.h"
#include "socket_module/Pool.h"
#include "socket_module/CriticalSection.h"
#include "comm_protocol.h"
//#include "IOCPServerHelper.h"
/***************************************************************************************************
* 1、 File       ： 客户端业务处理操作类
* 2、 Version    ： 1.0
* 3、 Description： 
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-3-20 14:49:54
* 6、 History    ： 
* 7、 Remark     ： 只负责处理数据 不负责不送  发送由CIOCPServerHelper负责
****************************************************************************************************/
//客户端业务处理操作类

class CClientBusinessHelper
{
public:
	CClientBusinessHelper(TPool<DATAPACKET>* pobjRecvPacketPool, CIocpServer* pclsIocpServer, CLIENTBASEINFO& stuClientBaseInfo);
	~CClientBusinessHelper(void);
public:
	void Release();
	int Decode(const char* pRecv, const int nRcvLen);

private:
	inline bool CheckSerialNo(DWORD dwInSerialNo);
	int CheckHeadPaceket(const char* pRecv, const int nRcvLen);       //包头检查
	inline void ReceiveReset();	                                      // 数据复位
	bool Send(const BYTE* pBuffer, int iLen);                         //发送数据 连接ID 内容 长度

	int HandlePacket();        //处理包数据

	bool GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo);


public:
	DWORD m_dwInSerialNo;                     //最后收到数据包流水号
	PCLIENTBASEINFO m_pstuClientBaseInfo;     //客户端信息指针 由调用者 CIOCPServerHelper传入
	PDATAPACKET m_pstuDataPacket;             //客户端收发数据指针  指向接收缓冲区  一次只存放一个数据包
	TPool<DATAPACKET>* m_pobjRecvPacketPool;  //接收包对象池
	CIocpServer* m_pclsIOCPServer;
};