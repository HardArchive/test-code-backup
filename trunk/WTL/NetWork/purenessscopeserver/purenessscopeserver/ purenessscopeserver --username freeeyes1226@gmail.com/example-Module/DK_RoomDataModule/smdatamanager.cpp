#include "SMDataManager.h"

CSMDataManager::CSMDataManager(void)
{
}

CSMDataManager::~CSMDataManager(void)
{
	
}

bool CSMDataManager::Init()
{
#ifdef _WIN32	
	m_objKey = 100020;
#else
	m_objKey = ftok("/home/freeeyes/SMData/", 0);
#endif

	bool blState = m_objUserPlayerPool.Init(m_objKey, PLAYER_SMPOOL_COUNT);
	if(blState == true)
	{
		//�ָ�������Ϣ�б�
		ReadSMPool();
	}

	return blState;
}

void CSMDataManager::ReadSMPool()
{
	int nCount = m_objUserPlayerPool.GetUsedObjectCount();
	for(int i = 0; i < nCount; i++)
	{
		_UserPlayer* pUserPlayer = m_objUserPlayerPool.GetUsedObject(i);
		if(pUserPlayer != NULL)
		{
			if(pUserPlayer->m_u1DataState == 0)
			{
				//��ӵ�����ʹ���б���
				mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapOnlineUserPlayer.end())
				{
					m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}
			else if(pUserPlayer->m_u1DataState == 1)
			{
				//��ӵ��Ѿ��뿪�б���
				mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapLeaveUserPlayer.end())
				{
					m_mapLeaveUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}
			else if(pUserPlayer->m_u1DataState == 2)
			{
				//��ӵ�����ɾ���б���
				m_listDeleteUserPlayer.push_back(pUserPlayer);

				//��ӵ��Ѿ��뿪�б���
				mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
				if(f == m_mapDeleteUserPlayer.end())
				{
					m_mapDeleteUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
				}
			}

		}
	}
}

int32 CSMDataManager::ReadPlayerSMPos(const char* pPlayerNick)
{
	//�����Ժ�Ļ����в��ң����߱����Ƿ����
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		return (int32)m_objUserPlayerPool.GetDataPos(pUserPlayer);
	}

	//���һ��������Ƿ��Ѿ������˴��û�
	_UserPlayer* pLeavePlayer = FindPlayerLeave(pPlayerNick);
	if(pLeavePlayer != NULL)
	{
		return (int32)m_objUserPlayerPool.GetDataPos(pLeavePlayer);
	}

	//���ҿ�ɾ���б����Ƿ�����˴��û�
	_UserPlayer*  pDeletePlayer = FindPlayerDelete(pPlayerNick);
	if(pDeletePlayer != NULL)
	{
		return (int32)m_objUserPlayerPool.GetDataPos(pDeletePlayer);
	}

	//�����û���ҵ��������ݣ���ȥ����Դȥȡ��
	//�жϹ����ڴ����Ƿ��п���
	if(m_objUserPlayerPool.GetUsedObjectCount() >= PLAYER_SMPOOL_COUNT)
	{
		//�Ѿ�û�п����ˣ���Ҫ���һЩ����
		bool blState = DeleteUserPlayer();
		if(blState == false)
		{
			return PLAYER_INVALID_ID;
		}
	}

	//���PlayerID��������һ���򵥵��㷨���Ժ�������Դ�����滻֮
	_UserPlayer* pUserPlayer = m_objUserPlayerPool.NewObject();
	if(NULL != pUserPlayer)
	{
		pUserPlayer->m_u4PlayerID    =  GetRandNumber();
		pUserPlayer->m_u1PlayerState = 0;
		sprintf_safe(pUserPlayer->m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerNick);
		pUserPlayer->m_u4Money       = 10000;
		pUserPlayer->m_u4RoomID      = 0;

		//��ӵ��б���
		m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		return (int32)m_objUserPlayerPool.GetDataPos(pUserPlayer);
	}
	else
	{
		return PLAYER_INVALID_ID;
	}
}

bool CSMDataManager::ReadDBData()
{
	//�����Ԥ�����еĻ������ݿ��еĻ�Ծ���ݣ��������ݲ�ʵ��
	return true;
}

int32 CSMDataManager::ReadDBData(const char* pPlayerNick)
{
	//�����Ժ�Ļ����в��ң����߱����Ƿ����
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		return pUserPlayer->m_u4PlayerID;
	}


	//���һ��������Ƿ��Ѿ������˴��û�
	_UserPlayer* pUserPlayer = FindPlayerLeave(pPlayerNick);
	if(pUserPlayer != NULL)
	{
		return pUserPlayer->m_u4PlayerID;
	}

	//���ҿ�ɾ���б����Ƿ�����˴��û�
	_UserPlayer*  pDeletePlayer = FindPlayerDelete(pPlayerNick);
	if(pDeletePlayer != NULL)
	{
		return pDeletePlayer->m_u4PlayerID;
	}

	//�����û���ҵ��������ݣ���ȥ����Դȥȡ��
	//�жϹ����ڴ����Ƿ��п���
	if(m_objUserPlayerPool.GetUsedObjectCount() >= PLAYER_SMPOOL_COUNT)
	{
		//�Ѿ�û�п����ˣ���Ҫ���һЩ����
		bool blState = DeleteUserPlayer();
		if(blState == false)
		{
			return PLAYER_INVALID_ID;
		}
	}

	//���PlayerID��������һ���򵥵��㷨���Ժ�������Դ�����滻֮
	pUserPlayer = m_objUserPlayerPool.NewObject();
	if(NULL != pUserPlayer)
	{
		pUserPlayer->m_u4PlayerID    =  GetRandNumber();
		pUserPlayer->m_u1PlayerState = 0;
		sprintf_safe(pUserPlayer->m_szPlayerNick, MAX_BUFF_20, "%s", pPlayerNick);
		pUserPlayer->m_u4Money       = 10000;
		pUserPlayer->m_u4RoomID      = 0;

		//��ӵ��б���
		m_mapOnlineUserPlayer.insert(mapOnlineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		return pUserPlayer->m_u4PlayerID;
	}
	else
	{
		return PLAYER_INVALID_ID;
	}
}

bool CSMDataManager::LeaveDBData(const char* pPlayerNick)
{
	//�����ݿ��Ա��Ϊ�뿪״̬
	mapOnlineUserPlayer::iterator f = m_mapOnlineUserPlayer.find((string)pPlayerNick);
	if(f != m_mapOnlineUserPlayer.end())
	{
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		if(NULL != pUserPlayer)
		{
			pUserPlayer->m_u1DataState = 1;
		}
		m_mapOnlineUserPlayer.erase(f);

		//�����뿪��
		mapOfflineUserPlayer::iterator fl = m_mapLeaveUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(fl == m_mapLeaveUserPlayer.end())
		{
			m_mapLeaveUserPlayer.insert(mapOfflineUserPlayer::value_type(pUserPlayer->m_szPlayerNick, pUserPlayer));
		}
	}

	return true;
}

bool CSMDataManager::WriteDBData(const char* pPlayerNick)
{
	//д��DB����Դ�����������ڴ����
	mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find(pPlayerNick);
	if(f != m_mapLeaveUserPlayer.end())
	{
		//����ִ��д������Դ����������ǿ���ɾ��
		_UserPlayer* pUserPlayer = (_UserPlayer* )f->second;
		if(NULL != pUserPlayer)
		{
			//�����������Դ�Ĵ洢������Ŀǰ��ʱ�ǿգ��ɾ���ʵ�ֵĿ������Լ����
			pUserPlayer->m_u1DataState = 2;
			//m_objUserPlayerPool.DeleteObject(pUserPlayer);
		}
		m_mapLeaveUserPlayer.erase(f);

		//��ӵ�����ɾ����map������ȥ
		mapOfflineUserPlayer::iterator fDelete = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(fDelete == m_mapDeleteUserPlayer.end())
		{
			m_mapDeleteUserPlayer.insert(mapOfflineUserPlayer::value_type((string)pUserPlayer->m_szPlayerNick, pUserPlayer));
		}

		m_listDeleteUserPlayer.push_back(pUserPlayer);
	}

	return true;
}

_UserPlayer* CSMDataManager::GetUserPlayer(uint32 u4Pos)
{
	return m_objUserPlayerPool.GetPosToData(u4Pos);
}

uint32 CSMDataManager::GetRandNumber()
{
	uint32 u4RandID = 0;

	ACE_OS::srand((unsigned)time(NULL));
	u4RandID = (uint32)((ACE_OS::rand() % (RANAD_MAX_NUMBER - RANAD_MIN_NUMBER)) + RANAD_MAX_NUMBER);

	return u4RandID;
}

bool CSMDataManager::DeleteUserPlayer()
{
	if(m_listDeleteUserPlayer.size() > 0)
	{
		listDeleteUserPlayer::iterator b = m_listDeleteUserPlayer.begin();
		_UserPlayer* pUserPlayer = (_UserPlayer* )*b;
		m_listDeleteUserPlayer.erase(b);

		//ͬʱɾ��map�е����
		mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pUserPlayer->m_szPlayerNick);
		if(f != m_mapDeleteUserPlayer.end())
		{
			m_mapDeleteUserPlayer.erase(f);
		}

		if(NULL != pUserPlayer)
		{
			m_objUserPlayerPool.DeleteObject(pUserPlayer);
		}

		return true;
	}
	else
	{
		return false;
	}
}

_UserPlayer* CSMDataManager::FindPlayerLeave(const char* pPlayerNick)
{
	//�������뿪���������Ƿ���ڵ�ǰ��ң��������
	mapOfflineUserPlayer::iterator f = m_mapLeaveUserPlayer.find((string)pPlayerNick);
	if(f != m_mapLeaveUserPlayer.end())
	{
		return (_UserPlayer* )f->second;
	}
	else
	{
		return NULL;
	}
}

_UserPlayer* CSMDataManager::FindPlayerDelete(const char* pPlayerNick)
{
	mapOfflineUserPlayer::iterator f = m_mapDeleteUserPlayer.find((string)pPlayerNick);
	if(f != m_mapDeleteUserPlayer.end())
	{
		return (_UserPlayer*)f->second;
	}
	else
	{
		return NULL;
	}
}

