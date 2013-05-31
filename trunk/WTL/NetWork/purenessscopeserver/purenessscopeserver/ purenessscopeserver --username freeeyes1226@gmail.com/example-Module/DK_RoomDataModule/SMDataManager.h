#ifndef _SMDATAMANAGER_H
#define _SMDATAMANAGER_H

//���︺��������е�SM�����ڴ�����
//Ŀǰ�������е�Player��Room�������ڴ�����
//add by freeeyes

#include <list>

using namespace std;

#include "UserPlayer.h"
#include "SMPool.h"

#define RANAD_MIN_NUMBER  1
#define RANAD_MAX_NUMBER  1000000

#include <string>

typedef map<string, _UserPlayer*> mapOnlineUserPlayer;
typedef map<string, _UserPlayer*> mapOfflineUserPlayer;
typedef list<_UserPlayer*> listDeleteUserPlayer;

class CSMDataManager
{
public:
	CSMDataManager(void);
	~CSMDataManager(void);

	bool         Init();                                    //�򿪹����ڴ�
	bool         ReadDBData();                              //�����ݿ�����������Ϣ
	int32        ReadDBData(const char* pPlayerNick);       //������ǳƻ�������Ϣ
	int32        ReadPlayerSMPos(const char* pPlayerNick);  //�����ҵ�ǰ�ڴ��ַ��ƫ��λ��
	bool         LeaveDBData(const char* pPlayerNick);      //����뿪
	bool         WriteDBData(const char* pPlayerNick);      //д������Դ��ҵ�����
	_UserPlayer* GetUserPlayer(uint32 u4Pos);               //����ƫ�ƻ������ָ��
	void         ReadSMPool();                              //��ԭ�������ߣ��뿪�Ϳ���ɾ���������б�

private:
	uint32       GetRandNumber();                           //���һ�������
	bool         DeleteUserPlayer();                        //ɾ�����һ��Ӧ��ɾ�������ָ��
	_UserPlayer* FindPlayerLeave(const char* pPlayerNick);  //�������б���Ѱ���Ƿ���������
	_UserPlayer* FindPlayerDelete(const char* pPlayerNick); //�ڿ�ɾ���б���Ѱ���Ƿ���������

private:
	CSMPool<_UserPlayer> m_objUserPlayerPool;     //����û���
	SMKey                m_objKey;                //�����ڴ�key
	mapOnlineUserPlayer  m_mapOnlineUserPlayer;   //������ұ�
	mapOfflineUserPlayer m_mapLeaveUserPlayer;    //�Ѿ��뿪��ұ�
	mapOfflineUserPlayer m_mapDeleteUserPlayer;   //����ɾ������ұ�(���ڸ�Ч����) 
	listDeleteUserPlayer m_listDeleteUserPlayer;  //����ɾ������ұ�(������̭�㷨)
};
#endif

