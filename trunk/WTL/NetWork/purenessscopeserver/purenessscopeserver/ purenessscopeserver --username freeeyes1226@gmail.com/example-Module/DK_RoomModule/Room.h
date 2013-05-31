#ifndef _ROOM_H
#define _ROOM_H

#include "ace/OS_NS_string.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"

#include "define.h"

//���������Ϣ�����������ڵ������Ϣ
//add by freeeyes

//Ŀǰ֧�ֵ�����
#define COMMAND_ENTERROOM    0x2003    //���뷿��
#define COMMAND_EXITROOM     0x2004    //�뿪����
#define COMMAND_BEGINROOM    0x2005    //��ʼ����
#define COMMAND_ENDROOM      0x2006    //��������
#define COMMAND_SETCARDROOM  0x2007    //������
#define COMMAND_OUTOLAYER    0x2008    //����
#define COMMAND_CHANGEROOM   0x2009    //�ı䷿��
#define COMMAND_CLEARROOM    0x2010    //������
#define COMMAND_INITROOM     0x2011    //��ʼ������
#define COMMAND_TIMERPLAYER  0x2012    //��Ҷ�ʱ����ʱ
#define COMMAND_TIMERROOM    0x2013    //���䶨ʱ����ʱ
#define COMMAND_SETROOM      0x2014    //����

#define COMMAND_RETURN_USERINROOM       0xf003    //��ҽ��뷿��
#define COMMAND_RETURN_USEROUTROOM      0xf004    //����뿪����
#define COMMAND_RETURN_USERSIT          0xf005    //�������
#define COMMAND_RETURN_USERINBET        0xf034    //�����ע
#define COMMAND_RETURN_USEROVERGAME     0xf035    //�������
#define COMMAND_RETURN_GAMEFINISH       0xf036    //��ҵ�ǰ��Ϸ����
#define COMMAND_RETURN_ROOMTIMER        0xf037    //���䶨ʱ������
#define COMMAND_RETURN_ROOMPLAYERTIMER  0xf038    //������Ҷ�ʱ������
#define COMMAND_RETURN_ROOMMONEY        0xf039    //��ǰ������ע״̬
#define COMMAND_RETURN_ROOMTABLECARD    0xf040    //��ǰ�����ڿ�����Ϣ

#define PLAYER_CODE_SIZE          5    //��ҿ��Ƹ���
#define ROOM_PLAYER_SIZE          5    //��������Ҳμӵ��������
#define ROOM_PLAYER_VISITOR_SIZE  5    //�۲�����ҵ�������
#define ROOM_TABLE_CARD_SIZE      5    //�����������ϵ���
#define ROOM_CARD_SIZE            52   //�����ڱ��Ƶ�����
#define ROOM_COUNT                50   //��󷿼���

#define ROOM_ERROR_NULL           0    //�����ɹ�
#define ROOM_ERROR_NOEXIST        -1   //����ID������
#define ROOM_ERROR_NOBEGIN        -2   //�����Ѿ���ʼ�����������
#define ROOM_ERROR_PLAYERFULL     -3   //�������Ѿ�����
#define ROOM_ERROR_PLAYERNOEXIST  -4   //ָ��ID����Ҳ��ڷ���
#define ROOM_ERROR_ISBEGIN        -5   //�����Ѿ���ʼ�������ظ���ʼ
#define ROOM_ERROR_ISNOBEGIN      -6   //����û�п�ʼ�����ܽ���
#define ROOM_ERROR_PLAYERCARDFULL -7   //�û����ϵĿ�Ƭ�������������
#define ROOM_ERROR_PLAYERNOCARD   -8   //�Ҳ����������ָ��λ�õĿ�Ƭ
#define ROOM_ERROR_PLAYEREXIST    -9   //����Ѿ�������ָ��λ����
#define ROOM_ERROR_PLAYERLOCATION -10  //������λ��ҷ�Χ
#define ROOM_ERROR_PLAYERSITEXIST -11  //����Ѿ�����
#define ROOM_ERROR_PLAYERVISEXIST -12  //����Ѿ��ڹ۲�����
#define ROOM_ERROR_PLAYERNOBET    -13  //����Ӧ����������ע
#define ROOM_ERROR_PLAYERMONONT   -14  //������Ͻ�Ǯ����

#define GAMEINFO_BOTTOMONEY       1000 //Ĭ�ϵ׳�
#define GAMEINFO_SMALLBLIND       1000 //Ĭ��Сäע

#define ROOM_BUFFPOLL_INVALID     -1   //��Ч��BuffID��BuffPool�����Ѿ�û�п���Ķ���
#define TIMER_ERROR_IN            -1   //��Ч�Ķ�ʱ��

//����״̬
enum
{
	ROOMSTATE_REDAY = 0,    //����װ����
	ROOMSTATE_ERROR,        //�������쳣
	ROOMSTATE_XIAZHU,       //����������Ҫ��ע
	ROOMSTATE_FINISH        //�����ƾֽ���
};

#define PLAYER_INVALID_ID         -1   //��Ч��PlayerID
#define ROOM_INVALID_ID           -1   //��Ч��RoomID
#define CARD_INVALID_ID           -1   //��Ч�Ŀ�Ƭ

//��ʱ�������ṹ��
struct _TimerInfo
{
	uint32 m_u4CommandID;
	uint32 m_u4RoomID;
	uint32 m_u4PlayerID;
	uint32 m_u4Event;

	_TimerInfo()
	{
		m_u4CommandID = 0;
		m_u4RoomID    = 0;
		m_u4PlayerID  = 0;
		m_u4Event     = 0;
	}
};

//��Ϸ��ز�����¼��Ϣ
struct _GameInfo
{
	uint16 m_u2Postion;                                   //���Ƶĵ�ǰλ��
	uint16 m_u2GameState;                                 //�����ƾ�״̬
	uint16 m_u2CurrentSmallBlind;                         //��ǰСäע
	uint32 m_u4BottomMoney;                               //��ǰ�׳� 
	uint16 m_u2RoundCount;                                //�ƾֵ�ǰ����
	uint32 m_u4TempBet;                                   //�ƾ����ע��
	uint32 m_u4Money;                                     //�����ϵ�Ǯ
	uint32 m_u4CurrPlayerID;                              //��ǰ���ID
	uint32 m_u4BeginPlayerID;                             //��ʼ�����ID
	uint32 m_objTableCard[ROOM_TABLE_CARD_SIZE];          //����������Ϣ
	uint32 m_objBankCard[ROOM_CARD_SIZE];                 //�ƿ�

	_GameInfo()
	{
		Clear();
	}

	void Clear()
	{
		m_u2Postion           = 0;
		m_u2GameState         = 0;
		m_u2CurrentSmallBlind = GAMEINFO_SMALLBLIND;
		m_u2RoundCount        = 0;
		m_u4TempBet           = 0;
		m_u4Money             = 0;
		m_u4CurrPlayerID      = PLAYER_INVALID_ID;
		m_u4BeginPlayerID     = PLAYER_INVALID_ID;
		m_u4BottomMoney       = GAMEINFO_BOTTOMONEY;

		for(int i = 0; i < ROOM_CARD_SIZE; i++)
		{
			m_objTableCard[i] = CARD_INVALID_ID;
		}

		for(int i = 0; i < ROOM_CARD_SIZE; i++)
		{

			m_objBankCard[i] = CARD_INVALID_ID;
		}
	}

};

//�����Ϣ
struct _Player
{
	uint32 m_u4PlayerID;                  //���ID
	char   m_szPlayerNick[MAX_BUFF_20];   //����ǳ� 
	uint32 m_u4Money;                     //��ҽ�Ǯ
	uint32 m_objCard[PLAYER_CODE_SIZE];   //������ϵ���
	uint32 m_u4CardCount;                 //��ǰӵ���Ƶĸ���
	uint32 m_u4TimerID;                   //��ǰ������ϵĶ�ʱ��
	_TimerInfo* m_pTimerInfo;             //��ǰ����Ҷ�ʱ��������Ϣ
	ACE_Time_Value m_tvUpdate;            //��ǰ��Ҹ������ʱ��
	ACE_Time_Value m_tvTimerArrived;      //��ʱ������ִ�������ʱ��

	//����û���ʱ������ʱ��
	ACE_Time_Value GetTimerArrived()
	{
		return m_tvTimerArrived;
	}

	//�����û���ʱ������ʱ��
	void SetTimerArrived(uint16 u2Seconds)
	{
		m_tvTimerArrived = ACE_OS::gettimeofday() + ACE_Time_Value(u2Seconds);
	}

	//����û���ʱ��ʱ������0Ϊû�м��
	uint16 GetTimerInterval()
	{
		uint16 n2Second = 0;
		if(m_tvTimerArrived > ACE_OS::gettimeofday())
		{
			ACE_Time_Value tvInterval = (m_tvTimerArrived - ACE_OS::gettimeofday());
			n2Second = (uint16)tvInterval.sec();
		}
		return n2Second;
	}

	_Player()
	{
		m_u4TimerID       = TIMER_ERROR_IN;
		m_pTimerInfo      = NULL;
		Clear();
	}

	void Clear()
	{
		m_u4PlayerID      = PLAYER_INVALID_ID;
		m_u4Money         = 0;
		m_u4CardCount     = 0;
		m_szPlayerNick[0] = '\0';
		m_tvUpdate        = ACE_OS::gettimeofday();

		for(int i = 0; i < PLAYER_CODE_SIZE; i++)
		{
			m_objCard[i] = CARD_INVALID_ID;
		}
		m_u4CardCount = 0;

		if(m_pTimerInfo != NULL)
		{
			//���Timer���ӵ�ָ��û�еõ��ͷţ����������������
			OUR_DEBUG((LM_ERROR, "[_Player::Clear]m_pTimerInfo is not NULL.\n"));
		}

		m_u4TimerID       = TIMER_ERROR_IN;
		m_pTimerInfo      = NULL;
	}
};

//����ṹ��
struct _Room
{
	uint32      m_u4RoomID;                                    //����ID
	char        m_szRoomName[MAX_BUFF_20];                     //��������
	char        m_szRoomDesc[MAX_BUFF_200];                    //������Ϣ���� 
	_Player     m_objPlayer[ROOM_PLAYER_SIZE];                 //��󷿼��ڵ����
	_Player     m_objVisitPlayer[ROOM_PLAYER_VISITOR_SIZE];    //��󷿼�ι������
	bool        m_blState;                                     //�Ƿ񿪷ţ�true���ţ�false������
	uint32      m_u4State;                                     //����״̬,0Ϊ׼����ʼ��1Ϊ�Ѿ���ʼ
	uint32      m_u4PlayerCount;                               //��Ҹ���
	uint32      m_u4VisitorCount;                              //�ι���Ҹ���
	uint32      m_u4TimerID;                                   //���䶨ʱ��ID
	_TimerInfo* m_pTimerInfo;                                  //���䶨ʱ���ĸ��Ӳ���
	ACE_Time_Value m_tvUpdate;                                 //��ǰ����������ʱ��
	ACE_Time_Value m_tvTimerArrived;                           //��ʱ������ִ�������ʱ��

	_GameInfo   m_objGameInfo;                                 //������Ϸ�����Ϣ

	_Room()
	{
		m_u4RoomID = ROOM_ERROR_NOEXIST;
		m_u4TimerID  = TIMER_ERROR_IN;
		m_pTimerInfo = NULL;
		Clear();
	}

	//������
	void Clear()
	{
		for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
		{
			m_objPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
		}

		for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
		{
			m_objVisitPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
		}

		if(m_pTimerInfo != NULL)
		{
			//���Timer���ӵ�ָ��û�еõ��ͷţ����������������
			OUR_DEBUG((LM_ERROR, "[_Room::Clear]m_pTimerInfo is not NULL.\n"));
		}

		m_u4TimerID  = TIMER_ERROR_IN;
		m_pTimerInfo = NULL;

		m_u4PlayerCount  = 0;
		m_u4VisitorCount = 0;
		m_blState        = true;
		m_u4State        = 0;
		m_tvUpdate       = ACE_OS::gettimeofday();
	}

	//��÷��䶨ʱ������ʱ��
	ACE_Time_Value GetTimerArrived()
	{
		return m_tvTimerArrived;
	}

	//���÷��䶨ʱ������ʱ��
	void SetTimerArrived(uint16 u2Seconds)
	{
		m_tvTimerArrived = ACE_OS::gettimeofday() + ACE_Time_Value(u2Seconds);
	}

	//��÷��䶨ʱ��ʱ������0Ϊû�м��
	uint16 GetTimerInterval()
	{
		uint16 n2Second = 0;
		if(m_tvTimerArrived > ACE_OS::gettimeofday())
		{
			ACE_Time_Value tvInterval = (m_tvTimerArrived - ACE_OS::gettimeofday());
			n2Second = (uint16)tvInterval.sec();
		}
		return n2Second;
	}
};

//��������࣬�������еķ���
struct _RoomManager
{
	_Room m_objRoom[ROOM_COUNT];

	_RoomManager()
	{
		for(int i = 0; i < ROOM_COUNT; i++)
		{
			m_objRoom[i].m_u4RoomID = i;
		}
	}

	_Room* GetRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return &m_objRoom[u4RoomID];
		}
	}

	//���ÿ�ʼ�����ID
	uint32 SetRoomGame_BenginePlayerID(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID = u4PlayerID;
			return ROOM_ERROR_NULL;
		}
	}

	//��ÿ�ʼ�����ID
	uint32 GetRoomGame_BenginePlayerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID;
		}
	}

	//�õ���ǰ��Ҫ���������ID
	uint32 GetCurrPlayerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID;
		}
	}

	//��ע
	uint32 SetBet(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//�������Ӧ����ע���ˣ���ʲô������
			if(m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID != u4PlayerID)
			{
				return ROOM_ERROR_PLAYERNOBET;
			}
			else
			{
				//��ʼ��ע
				uint32 u4PlayerMoney = GetPlayerMoney(u4RoomID, u4PlayerID);
				if(u4PlayerMoney < u4Money)
				{
					return ROOM_ERROR_PLAYERMONONT;
				}
				else
				{
					//��������Ͽ۳���Ǯ�������ƾֽ�Ǯ����ȥ
					SetPlayerMoney(u4RoomID, u4PlayerID, u4PlayerMoney - u4Money);
					m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4Money;
					return ROOM_ERROR_NULL;
				}	
			}
		}
	}

	//���ƶ���
	uint32 DealCode(uint32 u4RoomID, uint32 u4BottomMoney, int32 n4PlayerID, int32 n4SmallBlind)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//��ʼ���׳أ�Сäע
			m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney = u4BottomMoney;
			if(n4SmallBlind != -1)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind = n4SmallBlind;
			}

			int i = 0;
			//ϴ��
			vector<uint32> vecCodeBank;
			for (i = 0 ;i < ROOM_TABLE_CARD_SIZE; i++)
			{
				vecCodeBank.push_back(i);
			}
			random_shuffle(vecCodeBank.begin(), vecCodeBank.end());

			//�����ƿ�
			for (i = 0 ;i < ROOM_TABLE_CARD_SIZE; i++)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] = (uint32)vecCodeBank[i];
			}

			int nCodeBankIndex = 0;
			//��ʼ����(�����������)
			for(i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[0] = m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
					nCodeBankIndex++;
					m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[1] = m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
					nCodeBankIndex++;
					m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount = 2;
				}
			}

			//��ʼ���ƣ��������ϵ���
			for(i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] =  m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[nCodeBankIndex];
				nCodeBankIndex++;
			}

			//���õ�һ��������
			if(n4PlayerID != -1)
			{
				int16 n2Location = GetRoomPlayerLocation(u4RoomID, n4PlayerID);
				if(n2Location > 0)
				{
					//���ÿ�ʼλ��
					m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = n2Location;
				}
				else
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}

			//��ʼ�۳��׳�
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money >= m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney)
					{
						m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money -= m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney;
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u4BottomMoney;
					}
					else
					{
						m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money = 0;
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money;
					}
				}
			}

			//��ʼ��˳��ۼ�Сäע����λ�����ƶ�һ��
			uint32 u4BeginPlayerID = GetRoomNextPlayer(u4RoomID);
			if(u4BeginPlayerID >= 0)
			{
				//�۳�Сäע
				uint32 u4CurrPlayerMoney = GetPlayerMoney(u4RoomID, u4BeginPlayerID);
				if(u4CurrPlayerMoney > 0)
				{
					if(u4CurrPlayerMoney < m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind)
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, 0);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4CurrPlayerMoney;
					}
					else
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, u4CurrPlayerMoney - m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind;
					}
				}
			}

			//��ʼ�ۼ���äע����λ�����ƶ�һ��
			u4BeginPlayerID = GetRoomNextPlayer(u4RoomID);
			if(u4BeginPlayerID >= 0)
			{
				//�۳���äע
				uint32 u4CurrPlayerMoney = GetPlayerMoney(u4RoomID, u4BeginPlayerID);
				if(u4CurrPlayerMoney > 0)
				{
					if(u4CurrPlayerMoney < (uint32)(m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2))
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, 0);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += u4CurrPlayerMoney;
					}
					else
					{
						SetPlayerMoney(u4RoomID, u4BeginPlayerID, u4CurrPlayerMoney - m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2);
						m_objRoom[u4RoomID].m_objGameInfo.m_u4Money += m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind * 2;
					}
				}
			}

			//���÷���Ϊ��ע״̬
			m_objRoom[u4RoomID].m_u4State = ROOMSTATE_XIAZHU;

			return ROOM_ERROR_NULL;
		}
	}

	//�õ���һ����������ң��������ĩβ�����ɵ�һ��
	uint32 GetRoomNextPlayer(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			//ֻ��һ����ҵ�ʱ�򣬷���-1
			if(m_objRoom[u4RoomID].m_u4PlayerCount <= 1)
			{
				return PLAYER_INVALID_ID;
			}

			uint32 u4PlayerID = PLAYER_INVALID_ID;
			if(m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion == 0)
			{
				//�жϵ�0���Ƿ������
				if(m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID == PLAYER_INVALID_ID)
				{
					//Ѱ����һ���û��±�
					bool blState = false;
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if( m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
							blState                                       = true;
							u4PlayerID                                    = m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID;
						}
					}

					//��ǰ������û�����
					if(blState == false)
					{
						return PLAYER_INVALID_ID;
					}
				}
				else
				{
					u4PlayerID = m_objRoom[u4RoomID].m_objPlayer[0].m_u4PlayerID;
				}
			}
			else
			{
				u4PlayerID = m_objRoom[u4RoomID].m_objPlayer[m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion].m_u4PlayerID;
			}

			//Ѱ����һ���û��±�
			bool blState = false;
			for(int i = m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion + 1; i < ROOM_PLAYER_SIZE; i++)
			{
				if( m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
					blState                                       = true;
					break;
				}
			}

			if(blState == false)
			{
				//Ѱ����һ���û��±�
				bool blState = false;
				for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
				{
					if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID != PLAYER_INVALID_ID)
					{
						m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = i;
						blState                                       = true;
						break;
					}
				}

				//��ǰ������û�����
				if(blState == false)
				{
					return PLAYER_INVALID_ID;
				}
			}

			//�����ǰID��BeginID��ͬ�����ִ�+1
			if(m_objRoom[u4RoomID].m_objGameInfo.m_u4BeginPlayerID == u4PlayerID)
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount++;
			}

			return u4PlayerID;
		}
	}

	uint32 SetRoomUpdateTime(uint32 u4RoomID)    //���õ�ǰ����������ʱ��
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_tvUpdate = ACE_OS::gettimeofday();
			return ROOM_ERROR_NULL;
		}
	}

	bool CheckRoomUpdateTime(uint32 u4RoomID, uint32 u4MillSeconds)    //���õ�ǰ����������ʱ��,u4MillSeconds��λ�Ǻ���
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return false;
		}
		else
		{
			ACE_Time_Value tvDispose = ACE_OS::gettimeofday() - m_objRoom[u4RoomID].m_tvUpdate;
			if(u4MillSeconds >= (uint32)tvDispose.msec())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}

	uint32 SetRoomPlayerUpdateTime(uint32 u4RoomID, uint32 u4PlayerID)    //���õ�ǰ�������������ʱ��
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					m_objRoom[u4RoomID].m_objPlayer[i].m_tvUpdate = ACE_OS::gettimeofday();
					return ROOM_ERROR_NULL;
				}
			}
			
			return ROOM_ERROR_PLAYERNOEXIST;
		}
	}

	bool CheckRoomPlayerUpdateTime(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4MillSeconds)    //���õ�ǰ����������ʱ��,u4MillSeconds��λ�Ǻ���
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return false;
		}
		else
		{
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					ACE_Time_Value tvDispose = ACE_OS::gettimeofday() - m_objRoom[u4RoomID].m_objPlayer[i].m_tvUpdate;
					if(u4MillSeconds >= (uint32)tvDispose.msec())
					{
						return false;
					}
					else
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	uint32 GetRoomGame_Postion(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion;
		}
	}

	uint32 SetRoomGame_Postion(uint32 u4RoomID, uint16 u2Postion)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2Postion = u2Postion;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_GameState(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2GameState;
		}
	}

	uint32 SetRoomGame_GameState(uint32 u4RoomID, uint16 u2GameState)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2GameState = u2GameState;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_CurrentSmallBlind(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind;
		}
	}

	uint32 SetRoomGame_CurrentSmallBlind(uint32 u4RoomID, uint16 u2CurrentSmallBlind)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2CurrentSmallBlind = u2CurrentSmallBlind;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_RoundCount(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount;
		}
	}

	uint32 SetRoomGame_RoundCount(uint32 u4RoomID, uint16 u2RoundCount)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u2RoundCount = u2RoundCount;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_TempBet(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4TempBet;
		}
	}

	uint32 SetRoomGame_TempBet(uint32 u4RoomID, uint32 u4TempBat)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4TempBet = u4TempBat;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_Money(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4Money;
		}
	}

	uint32 SetRoomGame_Money(uint32 u4RoomID, uint32 u4Money)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4Money = u4Money;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomGame_CurrPlayerID(uint32 u4RoomID)     //�õ���ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_objGameInfo.m_u4CurrPlayerID;
		}
	}

	uint32 SetRoomGame_CurrPlayerID(uint32 u4RoomID, uint32 u4CurrPlayerID)     //���õ�ǰ������Ϸ��Ϣ
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_objGameInfo.m_u4CurrPlayerID = u4CurrPlayerID;
			return ROOM_ERROR_NULL;
		}
	}

	uint32 GetRoomPlayerCount(uint32 u4RoomID)     //�õ���ǰ��ҵ�����
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4PlayerCount;
		}
	}

	uint32 GetRoomVisitorCount(uint32 u4RoomID)     //�õ��ι��������
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4VisitorCount;
		}
	}

	//�õ�ָ���������Ϣ(�����u4Index�Ǹ��ӵ��±�)
	char* GetRoomPlayerName(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4Index < 0 || u4Index >= ROOM_PLAYER_SIZE)
		{
			return NULL;
		}

		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_objPlayer[u4Index].m_u4PlayerID != PLAYER_INVALID_ID)
			{
				return m_objRoom[u4RoomID].m_objPlayer[u4Index].m_szPlayerNick;
			}
			else
			{
				return NULL;
			}
		}
	}

	//�õ���������
	const char* GetRoomName(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_szRoomName;
		}
	}

	//���÷�������
	uint32 SetRoomName(uint32 u4RoomID, const char* pRoomName)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			sprintf_safe(m_objRoom[u4RoomID].m_szRoomName, MAX_BUFF_20, "%s", pRoomName);
			return ROOM_ERROR_NULL;
		}
	}

	//�õ���������
	const char* GetRoomDesc(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_szRoomDesc;
		}
	}

	//�õ�����״̬
	uint32 GetRoomState(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4State;
		}
	}

	//���÷���״̬
	uint32 SetRoomState(uint32 u4RoomID, uint32 u4State)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_u4State = u4State;
			return ROOM_ERROR_NULL;
		}
	}

	//���÷���������Ϣ
	uint32 SetRoomDesc(uint32 u4RoomID, const char* pRoomDesc)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			sprintf_safe(m_objRoom[u4RoomID].m_szRoomDesc, MAX_BUFF_200, "%s", pRoomDesc);
			return ROOM_ERROR_NULL;
		}
	}

	//�õ�Room�Ķ�ʱ��ID
	uint32 GetRoomTimerID(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			return m_objRoom[u4RoomID].m_u4TimerID;
		}
	}

	//�ӹ۲�����ָ�����̶�λ����Ϸ�����,���n2Location��-1,���ǰ�˳�����£�������ָ����λ�á�
	uint32 SetRoomPlayerSitDown(uint32 u4RoomID, uint32 u4PlayerID, int16 n2Location)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(n2Location < -1 || n2Location >= ROOM_PLAYER_SIZE)
			{
				return ROOM_ERROR_PLAYERLOCATION;
			}
			else
			{
				for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
				{
					if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
					{
						if(n2Location == -1)
						{
							uint32 uRet = JoinRoom(u4RoomID, u4PlayerID, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money);
							if(uRet == ROOM_ERROR_NULL)
							{
								m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
								m_objRoom[u4RoomID].m_u4VisitorCount--;
								return ROOM_ERROR_NULL;
							}
							else
							{
								return uRet;
							}
						}
						else
						{
							uint32 uRet = JoinRoom(u4RoomID, u4PlayerID, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money, (uint16)n2Location);
							if(uRet == ROOM_ERROR_NULL)
							{
								m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
								m_objRoom[u4RoomID].m_u4VisitorCount--;
								return ROOM_ERROR_NULL;
							}
							else
							{
								return uRet;
							}
						}
					}
				}

				return ROOM_ERROR_PLAYERNOEXIST;
			}
		}
	}

	//����Room�Ķ�ʱ��ID
	uint32 SetRoomTimerID(uint32 u4RoomID, uint32 u4TimerID, uint16 u2Seconds, _TimerInfo* pTimerInfo)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			m_objRoom[u4RoomID].m_u4TimerID  = u4TimerID;
			m_objRoom[u4RoomID].m_pTimerInfo = pTimerInfo;
			if(u2Seconds > 0)
			{
				m_objRoom[u4RoomID].SetTimerArrived(u2Seconds);
			}
			return ROOM_ERROR_NULL;
		}
	}

	//�õ�Room�Ķ�ʱ��������Ϣ
	_TimerInfo*  GetRoomTimerInfo(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			return m_objRoom[u4RoomID].m_pTimerInfo;
		}
	}

	//�õ���ҵĶ�ʱ��ID
	uint32 GetRoomPlayerTimerID(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4TimerID;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//�õ�������ϵ�TimerInfo;
	_TimerInfo*  GetRoomPlayerTimerInfo(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return NULL;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return NULL;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_pTimerInfo;
						}
					}

					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
	}

	//����������ϵĶ�ʱ��ID
	uint32  SetRoomPlayerTimerID(uint32 u4RoomID, uint32 u4PlayerID, uint16 u2Seconds, uint32 u4TimerID, _TimerInfo* pTimerInfo)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4TimerID  = u4TimerID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_pTimerInfo = pTimerInfo;
							if(u2Seconds > 0)
							{
								m_objRoom[u4RoomID].m_objPlayer[i].SetTimerArrived(u2Seconds);
							}
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//����RoomID��PlayerID��õ�ǰ�����λ��
	uint32 GetRoomPlayerLocation(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			int nIndex = 0;
			for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
				{
					return nIndex;
				}
				nIndex++;
			}

			return ROOM_ERROR_PLAYERNOEXIST;
		}
	}

	//���뷿��(�̶�λ��)
	uint32 JoinRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money, uint16 nIndex)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4PlayerID == PLAYER_INVALID_ID)
			{
				m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4PlayerID = u4PlayerID;
				m_objRoom[u4RoomID].m_objPlayer[nIndex].m_u4Money    = u4Money;
				m_objRoom[u4RoomID].m_u4PlayerCount++;
				sprintf_safe(m_objRoom[u4RoomID].m_objPlayer[nIndex].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_PLAYEREXIST;
			}
		}
	}

	//���뷿��۲���
	uint32 EnterRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4VisitorCount >= ROOM_PLAYER_VISITOR_SIZE)
				{
					return ROOM_ERROR_PLAYERFULL;
				}
				else
				{
					//���ҵ�ǰ�����ظ���PlayerID����(�۲���)
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return ROOM_ERROR_PLAYERVISEXIST;
						}
					}

					//���ҵ�ǰ����Ƿ��Ѿ�����
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return ROOM_ERROR_PLAYERSITEXIST;
						}
					}

					//Ѱ�ҿ�λ
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID = u4PlayerID;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money    = u4Money;
							m_objRoom[u4RoomID].m_u4VisitorCount++;
							sprintf_safe(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERFULL;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//���뷿��(���λ��)
	uint32 JoinRoom(uint32 u4RoomID, uint32 u4PlayerID, const char* pPlayerName, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount >= ROOM_PLAYER_SIZE)
				{
					return ROOM_ERROR_PLAYERFULL;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == PLAYER_INVALID_ID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID = u4PlayerID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = u4Money;
							m_objRoom[u4RoomID].m_u4PlayerCount++;
							sprintf_safe(m_objRoom[u4RoomID].m_objPlayer[i].m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerName);
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERFULL;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//�뿪����
	uint32 ExitRoom(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					//�ȼ��һ���Ƿ��Ѿ�����
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = 0;
							m_objRoom[u4RoomID].m_objPlayer[i].Clear();
							m_objRoom[u4RoomID].m_u4PlayerCount--;
							return ROOM_ERROR_NULL;
						}
					}

					//���û�����£������Ƿ��ǹ۲���
					for(int i = 0; i < ROOM_PLAYER_VISITOR_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4PlayerID = PLAYER_INVALID_ID;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].m_u4Money    = 0;
							m_objRoom[u4RoomID].m_objVisitPlayer[i].Clear();
							m_objRoom[u4RoomID].m_u4VisitorCount--;
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//��ʼ����
	uint32 BeginRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				m_objRoom[u4RoomID].m_u4State = 1;
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_ISBEGIN;
			}
		}
	}

	//��������
	uint32 EndRoom(uint32 u4RoomID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 1)
			{
				m_objRoom[u4RoomID].m_u4State = 0;
				return ROOM_ERROR_NULL;
			}
			else
			{
				return ROOM_ERROR_ISNOBEGIN;
			}
		}
	}

	//���÷�������ҵĽ�Ǯ
	uint32 SetPlayerMoney(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Money)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money    = u4Money;
							return ROOM_ERROR_NULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//�õ���ҽ�Ǯ
	uint32 GetPlayerMoney(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4Money;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//������ҵ���
	uint32 SetPlayerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//�ҵ�һ���ո񣬷���
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] == CARD_INVALID_ID)
								{
									m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] = u4CardID;
									m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount++;
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERCARDFULL;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//�õ����ָ��λ�õ���
	uint32 GetPlayerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Index)
	{
		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//�ҵ�һ���ո񣬷���
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] != CARD_INVALID_ID)
								{
									if(u4CurrIndex == u4Index)
									{
										return m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j];
									}
									else
									{
										u4CurrIndex++;
									}
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERNOCARD;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//ɾ��������ϵ�һ�ſ���
	uint32 DelPalyerCard(uint32 u4RoomID, uint32 u4PlayerID, uint32 u4Index)
	{
		uint32 u4CurrIndex = 0;
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							for(int j = 0; j < PLAYER_CODE_SIZE; j++)
							{
								//�ҵ�һ���ո񣬷���
								if(m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] != CARD_INVALID_ID)
								{
									if(u4CurrIndex == u4Index)
									{
										m_objRoom[u4RoomID].m_objPlayer[i].m_objCard[j] = CARD_INVALID_ID;
										m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount--;
										return ROOM_ERROR_NULL;
									}
									else
									{
										u4CurrIndex++;
									}
									return ROOM_ERROR_NULL;
								}
							}

							return ROOM_ERROR_PLAYERNOCARD;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//�õ���ҵĿ�������
	uint32 GetPlayerCardCount(uint32 u4RoomID, uint32 u4PlayerID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(m_objRoom[u4RoomID].m_u4State == 0)
			{
				if(m_objRoom[u4RoomID].m_u4PlayerCount <= 0)
				{
					return ROOM_ERROR_PLAYERNOEXIST;
				}
				else
				{
					for(int i = 0; i < ROOM_PLAYER_SIZE; i++)
					{
						if(m_objRoom[u4RoomID].m_objPlayer[i].m_u4PlayerID == u4PlayerID)
						{
							return m_objRoom[u4RoomID].m_objPlayer[i].m_u4CardCount;
						}
					}

					return ROOM_ERROR_PLAYERNOEXIST;
				}
			}
			else
			{
				return ROOM_ERROR_NOBEGIN;
			}
		}
	}

	//���÷��������ϵ�һ�ſ���
	uint32 SetRoomTableCard(uint32 u4RoomID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] == CARD_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[i] = u4CardID;
				}
			}

			return ROOM_ERROR_PLAYERCARDFULL;
		}
	}

	//�õ�����������ָ��λ�õ�һ����
	uint32 GetRoomTableCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_TABLE_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				return m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[u4Index];
			}
		}
	}

	//ɾ������ָ��λ�õ�һ�ſ���
	uint32 DelRoomTableCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_TABLE_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objTableCard[u4Index] = CARD_INVALID_ID;
				return ROOM_ERROR_NULL;
			}
		}
	}

	//�����ƿ��е�һ����
	uint32 SetRoomBankCard(uint32 u4RoomID, uint32 u4CardID)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			for(int i = 0; i < ROOM_TABLE_CARD_SIZE; i++)
			{
				if(m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] == CARD_INVALID_ID)
				{
					m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[i] = u4CardID;
				}
			}

			return ROOM_ERROR_PLAYERCARDFULL;
		}
	}

	//�õ������ƿ��е�һ����
	uint32 GetRoomBankCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				return m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[u4Index];
			}
		}
	}

	//ɾ�������ƿ��е�һ�ſ���
	uint32 DelRoomBankCard(uint32 u4RoomID, uint32 u4Index)
	{
		if(u4RoomID < 0 || u4RoomID >= ROOM_COUNT)
		{
			return ROOM_ERROR_NOEXIST;
		}
		else
		{
			if(u4Index < 0 || u4Index >= ROOM_CARD_SIZE)
			{
				return ROOM_ERROR_PLAYERNOCARD;
			}
			else
			{
				m_objRoom[u4RoomID].m_objGameInfo.m_objBankCard[u4Index] = CARD_INVALID_ID;
				return ROOM_ERROR_NULL;
			}
		}
	}
};

typedef ACE_Singleton<_RoomManager, ACE_Null_Mutex> App_RoomManager;

#endif
