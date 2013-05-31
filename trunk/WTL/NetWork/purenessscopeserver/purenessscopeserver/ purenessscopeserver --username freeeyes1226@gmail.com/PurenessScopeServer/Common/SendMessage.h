#ifndef _SENDMESSAGE_H
#define _SENDMESSAGE_H

#include "define.h"
#include "IBuffPacket.h"
#include <map>

using namespace std;

//����һ���������������������첽���Ͷ���
struct _SendMessage
{
	uint32          m_u4ConnectID;    //Ҫ���͵�Զ��ID
	uint16          m_u2CommandID;    //Ҫ���͵�����ID������ͳ�ƹ���
	bool            m_blSendState;    //Ҫ���͵�״̬��0���������ͣ�1���Ȼ��治����
	IBuffPacket*    m_pBuffPacket;    //���ݰ�����
	uint8           m_nEvents;        //�������ͣ�0���������ݰ����ͣ�1��������������
	ACE_hrtime_t    m_tvSend;         //���ݰ����͵�ʱ���

	_SendMessage()
	{
		m_u4ConnectID = 0;
		m_nEvents     = 0;
		m_u2CommandID = 0;
		m_blSendState = 0;
	}

	~_SendMessage()
	{
	}
};

class CSendMessagePool
{
public:
	CSendMessagePool(void);
	~CSendMessagePool(void);

	void Init(int nObjcetCount = MAX_MSG_THREADQUEUE);
	void Close();

	_SendMessage* Create();
	bool Delete(_SendMessage* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<_SendMessage*, _SendMessage*> mapSendMessage;
	mapSendMessage              m_mapMessageUsed;                      //��ʹ�õ�
	mapSendMessage              m_mapMessageFree;                      //û��ʹ�õ�
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ƶ��߳���
	uint32                      m_u4CurrMaxCount;
};
#endif

