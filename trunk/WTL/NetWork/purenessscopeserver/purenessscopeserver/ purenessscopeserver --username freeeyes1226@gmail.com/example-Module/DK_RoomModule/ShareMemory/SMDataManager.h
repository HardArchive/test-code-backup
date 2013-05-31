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



typedef map<uint32, _UserPlayer*> mapOnlineUserPlayer;
typedef list<_UserPlayer*> listDeleteUserPlayer;

class CSMDataManager
{
public:
	CSMDataManager(void);
	~CSMDataManager(void);

	bool         Init();                                    //�򿪹����ڴ�
	_UserPlayer* GetUserPlayer(uint32 u4Pos);               //����ƫ�ƻ������ָ��


private:
	CSMPool<_UserPlayer> m_objUserPlayerPool;     //����û���
	SMKey                m_objKey;                //�����ڴ�key

};
#endif

