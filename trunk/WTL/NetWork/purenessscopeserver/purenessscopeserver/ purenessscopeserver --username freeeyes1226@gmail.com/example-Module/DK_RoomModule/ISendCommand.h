#ifndef _ISENDCOMMAND_H
#define _ISENDCOMMAND_H

//�����������ӿڣ���ӦBaseCommand�ļ̳й�ϵ��
//Ϊlua�ڲ���̬�˿��ṩ�������ݵ�֧��

#include "define.h"
#include "IBuffPacket.h"
#include "Room.h"

//���ͷ�������ҽṹ��
struct _Send_RoomPlayer
{
	uint32 m_u4PlayerID;                  //���ID
	uint8  m_u1Location;                  //��ҵ���λID
	char   m_szPlayerNick[MAX_BUFF_20];   //����ǳ�
	uint32 m_u4CardCount;                 //��ǰӵ���Ƶĸ���
	uint32 m_objCard[PLAYER_CODE_SIZE];   //������ϵ���

	_Send_RoomPlayer()
	{
		m_u4CardCount = 0;
		for(int i = 0; i < PLAYER_CODE_SIZE; i++)
		{
			m_objCard[i] = CARD_INVALID_ID;
		}
	}
};

//��ҷ�����Ϣ
struct _Send_RoomInfo
{
	uint32            m_u4RoomID;                                    //����ID
	char              m_szRoomName[MAX_BUFF_20];                     //��������
	char              m_szRoomDesc[MAX_BUFF_200];                    //������Ϣ����
	uint32            m_u4PlayerCount;                               //��ǰ�������µ�����
	_Send_RoomPlayer  m_objSendRoomPlayer[ROOM_PLAYER_SIZE];         //��ǰ���������Ϣ

	_Send_RoomInfo()
	{
		m_u4PlayerCount = 0;
	}

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			VCHARS_STR ss_Data;
			ss_Data.text  = (char* )m_szRoomName;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szRoomName);
			(*pBuffPacket) << ss_Data;

			ss_Data.text = (char* )m_szRoomDesc;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szRoomName);
			(*pBuffPacket) << ss_Data;

			(*pBuffPacket) << m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u4PlayerID;
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u1Location;
				ss_Data.text  = (char* )m_objSendRoomPlayer[i].m_szPlayerNick;
				ss_Data.u1Len =(uint8) ACE_OS::strlen(m_objSendRoomPlayer[i].m_szPlayerNick);
				(*pBuffPacket) << ss_Data;
				(*pBuffPacket) << m_objSendRoomPlayer[i].m_u4CardCount;

				for(int j = 0; j < (int)m_objSendRoomPlayer[i].m_u4CardCount; j++)
				{
					(*pBuffPacket) << m_objSendRoomPlayer[i].m_objCard[j];
				}
			}

			return true;
		}
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			VCHARS_STR ss_Data;
			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szRoomName, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szRoomDesc, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u4PlayerID;
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u1Location;
				(*pBuffPacket) >> ss_Data;
				ACE_OS::memcpy(m_objSendRoomPlayer[i].m_szPlayerNick, ss_Data.text, ss_Data.u1Len);
				(*pBuffPacket) >> m_objSendRoomPlayer[i].m_u4CardCount;

				for(int j = 0; j < (int)m_objSendRoomPlayer[i].m_u4CardCount; j++)
				{
					(*pBuffPacket) >> m_objSendRoomPlayer[i].m_objCard[j];
				}
			}

			return true;
		}
	}
};

//���ͷ��䶨ʱ��Ϣ
struct _Send_RoomTimer
{
	uint32 m_u4RoomID;
	uint16 m_u2RoomSeconds;

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u2RoomSeconds;
		}

		return true;
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u2RoomSeconds;
		}

		return true;
	}
};

//������Ҷ�ʱ����Ϣ
struct _Send_RoomPlayerTimer
{
	uint32 m_u4RoomID;
	uint32 m_u4PlayerID;
	uint16 m_u2RoomSeconds;

	bool SetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;
			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4PlayerID;
			(*pBuffPacket) << m_u2RoomSeconds;
		}

		return true;
	}

	bool GetStream(uint16& u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4PlayerID;
			(*pBuffPacket) >> m_u2RoomSeconds;
		}

		return true;
	}
};

//���������Ϣ
struct _Send_RoomPlayerInfo
{
	uint32 m_u4RoomID;                    //������ڷ���ID
	uint32 m_u4PlayerID;                  //���ID
	uint8  m_u1Location;                  //��ҵ���λID
	char   m_szPlayerNick[MAX_BUFF_20];   //����ǳ�
	uint32 m_u4CardCount;                 //��ǰӵ���Ƶĸ���
	uint32 m_objCard[PLAYER_CODE_SIZE];   //������ϵ���

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4PlayerID;
			(*pBuffPacket) << m_u1Location;

			VCHARS_STR ss_Data;
			ss_Data.text  = (char* )m_szPlayerNick;
			ss_Data.u1Len = (uint8)ACE_OS::strlen(m_szPlayerNick);
			(*pBuffPacket) << ss_Data;

			(*pBuffPacket) << m_u4CardCount;

			for(int i = 0; i < (int)m_u4CardCount; i++)
			{
				(*pBuffPacket) << m_objCard[i];
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;
			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4PlayerID;
			(*pBuffPacket) >> m_u1Location;

			VCHARS_STR ss_Data;
			(*pBuffPacket) >> ss_Data;
			ACE_OS::memcpy(m_szPlayerNick, ss_Data.text, ss_Data.u1Len);

			(*pBuffPacket) >> m_u4CardCount;

			for(int i = 0; i < (int)m_u4CardCount; i++)
			{
				(*pBuffPacket) >> m_objCard[i];
			}

			return true;
		}
	}
};

//��ǰ�����ע��Ϣ
struct _Send_RoomPlayerMoneyInfo
{
	uint32 m_u4PlayerID;    //��ҵ�ID
	uint32 m_u4PlayerMoney; //��ҵĽ�Ǯ
};

//�����ע��Ϣ
struct _Send_RoomMoney
{
	uint32                    m_u4RoomID;                      //����ID
	uint32                    m_u4RoomMoney;                   //��ǰ������Ǯ��
	uint32                    m_u4PlayerCount;                 //��ǰ��������Ҹ���
	_Send_RoomPlayerMoneyInfo m_objSendRoomPlayerMoneyInfo[ROOM_PLAYER_SIZE];

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u4RoomMoney;
			(*pBuffPacket) << m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) << m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerID;
				(*pBuffPacket) << m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerMoney;
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;

			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u4RoomMoney;
			(*pBuffPacket) >> m_u4PlayerCount;

			for(int i = 0; i < (int)m_u4PlayerCount; i++)
			{
				(*pBuffPacket) >> m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerID;
				(*pBuffPacket) >> m_objSendRoomPlayerMoneyInfo[i].m_u4PlayerMoney;
			}

			return true;
		}
	}
};

//�ƾַ��俨����Ϣ
struct _Send_RoomCodeInfo
{
	uint32 m_u4RoomID;          //����ID
	uint8  m_u1TableCardCount;  //��ǰ������Ҫ��ʾ�Ŀ�������
	uint32 m_objCard[ROOM_TABLE_CARD_SIZE];

	bool SetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)    //����ǰ������ѹ���ɶ�������
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) << u2CommandID;

			(*pBuffPacket) << m_u4RoomID;
			(*pBuffPacket) << m_u1TableCardCount;

			for(int i = 0; i < m_u1TableCardCount; i++)
			{
				(*pBuffPacket) << m_objCard[i];
			}

			return true;
		}
	}

	bool GetStream(uint16 u2CommandID, IBuffPacket* pBuffPacket)   //����ǰ��������ת��Ϊ��ṹ
	{
		if(NULL == pBuffPacket)
		{
			return false;
		}
		else
		{
			(*pBuffPacket) >> u2CommandID;

			(*pBuffPacket) >> m_u4RoomID;
			(*pBuffPacket) >> m_u1TableCardCount;

			for(int i = 0; i < m_u1TableCardCount; i++)
			{
				(*pBuffPacket) >> m_objCard[i];
			}

			return true;
		}
	}

};

class ISendCommand
{
public:
	virtual bool SendRoomState(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomInfo& objSendRoomInfo)                     = 0;         //���ͷ����ڵ������Ϣ
	virtual bool SendRoomSitdown(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerInfo& objSendRoomPlayerInfo)       = 0;         //���������Ϣ
	virtual bool SendRoomTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomTimer& objSendRoomTimer)                   = 0;         //���ͷ��䶨ʱ��������Ϣ
	virtual bool SendRoomPlayerTimer(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomPlayerTimer objSendRoomPlayerTimer)  = 0;         //������Ҷ�ʱ��������Ϣ
	virtual bool SendRoomPlayerMoney(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomMoney objSendRoomMoney)              = 0;         //���ͷ�������ҽ�Ǯ��Ϣ
	virtual bool SendRoomTableCard(uint16 u2CommandID, uint32 u4PlayerID, _Send_RoomCodeInfo objSendRoomCodeInfo)          = 0;         //���ͷ����ڵĿ�����Ϣ
};

#endif
