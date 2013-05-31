#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"
#include "CommandAccount.h"

//�����޸�һ�£����һ�������Ӧһ��ģ���������Ƶġ�
//�����Ϊһ��������Զ�Ӧ���������Ĵ���ģ�飬�����ͱȽϺ��ˡ�
//add by freeeyes

using namespace std;

//��������ߵĸ�ʽ
struct _ClientCommandInfo
{
	CClientCommand* m_pClientCommand;              //��ǰ����ָ��
	uint16          m_u2CommandID;                 //��ǰ�����Ӧ��ID
	uint32          m_u4Count;                     //��ǰ������õĴ���
	uint32          m_u4TimeCost;                  //��ǰ������ʱ������
	char            m_szModuleName[MAX_BUFF_100];  //����ģ������
	ACE_Date_Time   m_dtLoadTime;                  //��ǰ�������ʱ��
	uint32          m_u4CurrUsedCount;             //��ǰ����ʹ�õ����ô���
	uint8           m_u1State;                     //��ǰ�����״̬��0Ϊ������1Ϊ���ڹر�

	_ClientCommandInfo()
	{
		m_pClientCommand  = NULL;
		m_u4Count         = 0;
		m_u4TimeCost      = 0;
		m_szModuleName[0] = '\0';
		m_u4CurrUsedCount = 0;
		m_u1State         = 0;
	}
};

//ģ���_ClientCommandInfo֮��Ķ�Ӧ��ϵ
struct _ModuleClient
{
	vector<_ClientCommandInfo*> m_vecClientCommandInfo;    //һ��ģ�����ж�Ӧ�����б�
	uint8                       m_u1ModuleState;           //ģ��״̬��0Ϊ������1Ϊ����ж����
};

//��������Ҫ�����ݽṹ�����ڻش�����ģ����Ϣ
typedef map<string, _ModuleClient*> mapModuleClient;

//һ����Ϣ���Զ�Ӧһ��CClientCommand*�����飬����Ϣ�����ʱ��ַ�����Щ������
class CClientCommandList
{
private:
	typedef vector<_ClientCommandInfo*> vecClientCommandList;
	vecClientCommandList m_vecClientCommandList;

public:
	CClientCommandList()
	{
	};

	~CClientCommandList()
	{
		Close();
	};

	void Close()
	{
		for(int i = 0; i < (int)m_vecClientCommandList.size(); i++)
		{
			_ClientCommandInfo* pClientCommandInfo = m_vecClientCommandList[i];
			SAFE_DELETE(pClientCommandInfo);
		}
		m_vecClientCommandList.clear();
	}

	_ClientCommandInfo* AddClientCommand(CClientCommand* pClientCommand, const char* pMuduleName)
	{
		_ClientCommandInfo* pClientCommandInfo = new _ClientCommandInfo();
		if(NULL != pClientCommandInfo)
		{
			pClientCommandInfo->m_pClientCommand = pClientCommand;
			m_vecClientCommandList.push_back(pClientCommandInfo);
			sprintf_safe(pClientCommandInfo->m_szModuleName, MAX_BUFF_100, "%s", pMuduleName);
			return pClientCommandInfo;
		}
		else
		{
			return NULL;
		}

	};

	//�������Ϊtrue��֤�������Ϣ�Ѿ�û�ж�Ӧ���Ҫ��Χmap�г�ȥ
	bool DelClientCommand(CClientCommand* pClientCommand)
	{
		for(vecClientCommandList::iterator b = m_vecClientCommandList.begin(); b!= m_vecClientCommandList.end(); b++)
		{
			_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )(*b);
			if(NULL != pClientCommandInfo && pClientCommand == pClientCommandInfo->m_pClientCommand)
			{
				SAFE_DELETE(pClientCommandInfo);
				m_vecClientCommandList.erase(b);
				break;
			}
		}

		if((int)m_vecClientCommandList.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//�õ�����
	int GetCount()
	{
		return (int)m_vecClientCommandList.size();
	}

	//�õ�ָ��λ�õ�ָ��
	_ClientCommandInfo* GetClientCommandIndex(int nIndex)
	{
		if(nIndex >= (int)m_vecClientCommandList.size())
		{
			return NULL;
		}
		else
		{
			return (_ClientCommandInfo* )m_vecClientCommandList[nIndex];
		}
	}
};

class CMessageManager : public IMessageManager
{
public:
	CMessageManager(void);
	~CMessageManager(void);

	bool DoMessage(IMessage* pMessage, uint16& u2CommandID);   //ִ������
	void Close();

	bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName);   //ע������
	bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand);                            //ж������
 
	bool UnloadModuleCommand(const char* pModuleName, uint8 u1State);  //ж��ָ��ģ���¼�

	int  GetCommandCount();                                            //�õ���ǰע������ĸ���
	CClientCommandList* GetClientCommandList(uint16 u2CommandID);      //�õ���ǰ�����ִ���б�
	
	mapModuleClient* GetModuleClient();                                //��������ģ���ע��������Ϣ

private:
	bool CloseCommandInfo(const char* pModuleName);

private:
	typedef map<uint16, CClientCommandList*> mapClientCommand;

	mapClientCommand            m_mapClientCommand;
	mapModuleClient             m_mapModuleClient;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //���ڿ��Ƽ�����������
};

typedef ACE_Singleton<CMessageManager, ACE_Null_Mutex> App_MessageManager; 
#endif
