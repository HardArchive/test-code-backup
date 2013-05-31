#ifndef _LOADMODULE_H
#define _LOADMODULE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"

#include "MapTemplate.h"
#include "IObject.h"

#include <string>
#include <vector>

using namespace std;

struct _ModuleInfo
{
	string           strModuleName;         //ģ���ļ�����
	string           strModulePath;         //ģ��·��
	ACE_Date_Time    dtCreateTime;          //ģ�鴴��ʱ��
	ACE_SHLIB_HANDLE hModule;
	int (*LoadModuleData)(CServerObject* pServerObject);
	int (*UnLoadModuleData)(void);
	const char* (*GetDesc)(void);
	const char* (*GetName)(void);
	const char* (*GetModuleKey)(void);
	int (*DoModuleMessage)(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

	_ModuleInfo()
	{
	}
};

class CLoadModule
{
public:
	CLoadModule(void);
	~CLoadModule(void);

	void Close();

	bool LoadModule(const char* szModulePath, const char* szResourceName);
	bool UnLoadModule(const char* szResourceName);

	int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

	int  GetCurrModuleCount();
	_ModuleInfo* GetModuleIndex(int nIndex);
	_ModuleInfo* GetModuleInfo(const char* pModuleName);


private:
	bool ParseModule(const char* szResourceName, vector<string>& vecModuleName);     //���ַ�������������
	bool LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo);             //��ʼ����ģ��Ľӿں�����

private:
	CMapTemplate<string, _ModuleInfo>  m_mapModuleInfo;
	char                               m_szModulePath[MAX_BUFF_200];
	ACE_Recursive_Thread_Mutex         m_tmModule;
};

typedef ACE_Singleton<CLoadModule, ACE_Null_Mutex> App_ModuleLoader; 
#endif
