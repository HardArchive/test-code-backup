#ifndef MAKEPACKET_H
#define MAKEPACKET_H

//ר�Ŵ������ݽ������
//�����ݰ���ƴ�Ӻͽ������һ��
//add by freeeyes
//2010-06-11
#pragma once

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "PacketParsePool.h"
#include "MessageService.h"
#include "ProfileTime.h"

#include <map>

using namespace std;

struct _MakePacket
{
	uint8          m_PacketType;     //���ݰ����� 
	uint32         m_u4ConnectID;    //����ID
	uint8          m_u1Option;       //��������
	CPacketParse*  m_pPacketParse;   //���ݰ�����ָ��
	ACE_INET_Addr  m_AddrRemote;     //���ݰ�����ԴIP��Ϣ

	_MakePacket()
	{
		m_PacketType        = PACKET_TCP;  //0ΪTCP,1��UDP Ĭ����TCP
		m_u4ConnectID       = 0;
		m_u1Option          = 0;
	}
};

//_MakePacket����أ�ѭ��ʹ�ã���new���ݡ����������ٶȣ�Ӧ����CMakePacket
class CMakePacketPool
{
public:
	CMakePacketPool();
	~CMakePacketPool();

	void Init(uint32 u4PacketCount);
	void Close();

	_MakePacket* Create();
	bool Delete(_MakePacket* pMakePacket);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<_MakePacket*, _MakePacket*> mapPacket;
	mapPacket                  m_mapPacketUsed;                       //��ʹ�õ�
	mapPacket                  m_mapPacketFree;                       //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex m_ThreadWriteLock;                     //���ƶ��߳���
};

class CMakePacket
{
public:
	CMakePacket(void);
	~CMakePacket(void);

	bool Init();

	bool PutMessageBlock(uint32 u4ConnectID, uint8 u1Option, CPacketParse* pPacketParse);                        //����TCP���ݰ�
	bool PutUDPMessageBlock(const ACE_INET_Addr& AddrRemote, uint8 u1Option, CPacketParse* pPacketParse);        //����UDP���ݰ� 
	bool ProcessMessageBlock(_MakePacket* pMakePacket);

private:
	CMessage* SetMessage(CPacketParse* pPacketParse, uint32 u4ConnectID);                                         //һ�����ݰ���Ϣ(TCP)
	CMessage* SetMessage(CPacketParse* pPacketParse, const ACE_INET_Addr& AddrRemote);                            //һ�����ݰ���Ϣ(UDP)
	CMessage* SetMessageConnect(uint32 u4ConnectID);                                                              //�û��������ݰ���Ϣ
	CMessage* SetMessageCDisConnect(uint32 u4ConnectID);                                                          //�û��Ͽ��������ݰ���Ϣ
	CMessage* SetMessageSDisConnect(uint32 u4ConnectID);                                                          //�������Ͽ��������ݰ���Ϣ

private:
	CMakePacketPool                m_MakePacketPool;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket; 

#endif
