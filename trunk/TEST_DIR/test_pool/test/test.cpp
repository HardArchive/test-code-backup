// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Pool.h"
#include "TraceUtil.h"
#include "comm_protocol.h"
#include <math.h>
//策略类型
#define TYPE_POLICY_GAME                 0x01       //游戏策略
#define TPYE_POLICY_HTTP                 0x02       //URL策略
#define TYPE_POLICY_PERIOD               0x03       //时段策略

#define POLICY_BUFSIZE_PERIOD            64*4         //时段策略缓冲区最大长度 4条
#define POLICY_BUFSIZE_HTTP              256*1024*8   //URL策略缓冲区最大长度 1024*8条
#define POLICY_BUFSIZE_GAME              1024*1024*4  //游戏策略缓冲区最大长度 1024*4条

int GetTypeSize(int iType)
{
	return TYPE_POLICY_PERIOD==iType ? POLICY_BUFSIZE_PERIOD:(TPYE_POLICY_HTTP==iType ? POLICY_BUFSIZE_HTTP:POLICY_BUFSIZE_GAME);
}

int _tmain(int argc, _TCHAR* argv[])
{
	TPool<DATAPACKET> m_objRecvPacketPool;          //接收包对象池
	DWORD dwBufLen = 1314; 
	double dbCount = (double)dwBufLen/(double)sizeof(DATAPACKET);
	dbCount = ceil(dbCount);     //向上取整
	int iCount = (int)dbCount;
	PDATAPACKET pstuDataPacket = m_objRecvPacketPool.New();
	pstuDataPacket->Init(sizeof(DATAPACKET));
	pstuDataPacket->pstuDataHead->dwSerialNo = 5;
	pstuDataPacket->pstuDataHead->dwPacketLen = 128;
	pstuDataPacket->pstuDataHead->dwReturn = 0;
	pstuDataPacket->pstuDataHead->wHeadLen = 32;
	pstuDataPacket->pstuDataHead->wEncode = 0;
	pstuDataPacket->pstuDataHead->dwVersion = 1;


	strcpy((char*)pstuDataPacket->szbyData+32, "另一个非常优秀的特征就是通过读取配置文件，确定category、appender、layout等对象。也是我们非常推荐的使用方式，可以灵活地通过配置文件定义所有地对象及其属性，不用重新编码，动态更改日志记录的策略。");
	char* pPoint = (char*)pstuDataPacket->szbyData+32;
	pstuDataPacket = m_objRecvPacketPool.New(2);
	pstuDataPacket->Init(2*sizeof(DATAPACKET));

	int iSize1 = GetTypeSize(1);
	int iSize2 = GetTypeSize(2);
	int iSize3 = GetTypeSize(3);
	
	//pstuDataPacket->Init();

	m_objRecvPacketPool.Delete(pstuDataPacket);

	pstuDataPacket = m_objRecvPacketPool.New(2000);
	for (int i=0; i<10;i++)
	{
		pstuDataPacket = m_objRecvPacketPool.New(200);
		//pstuDataPacket->Init();
	}

	//Sleep(5000);
	//pstuDataPacket->Init();
	pstuDataPacket->Reset();
	m_objRecvPacketPool.Delete(pstuDataPacket);

	pstuDataPacket = m_objRecvPacketPool.New();
	m_objRecvPacketPool.Free();


	DWORD dwLen1 = sizeof(BARBASEINFO);         //营声所基本信息
	DWORD dwLen2 = sizeof(BARTERMINALINFO);     //经营场所主机信息 
	DWORD dwLen3 = sizeof(CUSTOMERINFO);        //上网用户信息
	DWORD dwLen4 = sizeof(CUSTOMERLOGINOUTLOG); //用户上下机日志
	DWORD dwLen5 = sizeof(BARBUSINESSCONTROL);  //经营控制命令

	
	DWORD dwLen6 = sizeof(DATAPACKET);         //接收数据缓冲区
	DWORD dwLen7 = sizeof(POLICYALERT);        //策略报警 策略碰撞记录
	DWORD dwLen8 = sizeof(POLICYTIME);         //经营时间策略
	DWORD dwLen9 = sizeof(POLICYHTTP);         //禁止网页策略
	DWORD dwLen10 = sizeof(POLICYGAME);        //禁止游戏策略
	DWORD dwLen11 = sizeof(POLICYCONTENT);     //本地策略缓冲区
	DWORD dwLen12 = sizeof(ISSUEDMESSAGE);     //下发消息
	DWORD dwLen13 = sizeof(COMMONDISSUED);     //下发(回复)命令
	return 0;
}

