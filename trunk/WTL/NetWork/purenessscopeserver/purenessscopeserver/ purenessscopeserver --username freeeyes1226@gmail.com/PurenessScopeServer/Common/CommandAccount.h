#ifndef _COMMANDACCOUNT_K
#define _COMMANDACCOUNT_K

// ͳ�����н�����ܵ�����ִ�����
// add by freeeyes
// 2012-03-19

#include "ace/Date_Time.h"
#include "define.h"
#include "LogManager.h"
#include <map>

using namespace std;

//ͳ����Ϣ��������Ҫͳ�Ƶ�������Ϣ����
struct _CommandData
{
	uint16 m_u2CommandID;                  //�����ID
	uint32 m_u4CommandCount;               //������ܵ��ô���
	uint64 m_u8CommandCost;                //�����ִ�кķ���ʱ��
	uint8  m_u1CommandType;                //��������ͣ�0���յ������1�Ƿ���������
	uint32 m_u4PacketSize;                 //���������������(δ����)
	uint32 m_u4CommandSize;                //���������������(����)
	uint8  m_u1PacketType;                 //���ݰ���Դ����  
	ACE_Time_Value m_tvCommandTime;        //����������ʱ��

	_CommandData()
	{
		m_u2CommandID    = 0;
		m_u4CommandCount = 1;
		m_u8CommandCost  = 0;
		m_u4PacketSize   = 0;
		m_u4CommandSize  = 0;
		m_u1PacketType   = PACKET_TCP;
		m_u1CommandType  = COMMAND_TYPE_IN;
	}
};


struct _CommandTimeOut
{
	uint16         m_u2CommandID;                  //�����ID
	ACE_Time_Value m_tvTime;                       //����ʱ��
	uint32         m_u4TimeOutTime;                //��ʱʱ��

	_CommandTimeOut()
	{
		m_u2CommandID   = 0;
		m_tvTime        = ACE_OS::gettimeofday();
		m_u4TimeOutTime = 0;
	}
};

typedef vector<_CommandTimeOut> vecCommandTimeOut;   //��¼���г�ʱ���������

//ͳ�����н�����ܵ�����ִ�������Ŀǰ�����������������������ͳ�ƣ���Ϊ�ⲿ��Э���޷�ͳһ��
class CCommandAccount
{
public:
	CCommandAccount();
	~CCommandAccount();

	void Init(uint8 u1CommandAccount, uint16 u2RecvTimeout);

	bool   SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType = PACKET_TCP, uint32 u4PacketSize = 0, uint32 u4CommandSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN, ACE_Time_Value tvTime = ACE_OS::gettimeofday());   //��¼����ִ����Ϣ
	bool   SaveCommandDataLog();                       //�洢����ִ����Ϣ����־
	void   ClearTimeOut();                             //����ʱʱ���������־
	uint32 GetTimeoutCount();                          //�õ���ʱ������־����
	_CommandTimeOut* GetTimeoutInfo(uint32 u4Index);   //�õ���ʱ���ݿ�

	void   Close();

public:
	typedef map<uint16, _CommandData*> mapCommandDataList;
	vecCommandTimeOut  m_vecCommandTimeOut;
	mapCommandDataList m_mapCommandDataList;
	uint8              m_u1CommandAccount;
	uint64             m_u8PacketTimeout;
};

typedef ACE_Singleton<CCommandAccount, ACE_Recursive_Thread_Mutex> App_CommandAccount; 

#endif
