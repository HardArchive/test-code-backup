#ifndef _USERPLAYER_H
#define _USERPLAYER_H

//���︺���¼�û��ṹ�壬�ⲿ�ֽṹ�彫����빲���ڴ���
//add by freeeyes

#include "define.h"

#define PLAYER_INVALID_ID         -1     //��Ч��PlayerID
#define ROOM_INVALID_ID           -1     //��Ч��RoomID

#define PLAYER_SMPOOL_COUNT       5000   //�����ڴ�ص�Player�ĸ��� 

struct _UserPlayer
{
	uint32 m_u4PlayerID;                 //��ҵ�ID 
	char   m_szPlayerNick[MAX_BUFF_20];  //��ҵ�����
	uint32 m_u4Money;                    //������ϵĽ�Ǯ
	uint8  m_u1PlayerState;              //���Ŀǰ״̬��0���ڴ�����1���ڷ�����
	uint8  m_u1DataState;                //��ǰ����״̬��0������ʹ�ã�1���Ѿ��뿪��2�ǿ���ɾ�� ��3�ǿ���
	uint32 m_u4RoomID;                   //��ǰ������ڵ�RoomID

	_UserPlayer()
	{
		Clear();
	}

	void Clear()
	{
		m_u4PlayerID      = PLAYER_INVALID_ID;
		m_szPlayerNick[0] = '\0';
		m_u4Money         = 0;
		m_u1PlayerState   = 0;
		m_u1DataState     = 3;
		m_u4RoomID        = ROOM_INVALID_ID;
	}
};

#endif
