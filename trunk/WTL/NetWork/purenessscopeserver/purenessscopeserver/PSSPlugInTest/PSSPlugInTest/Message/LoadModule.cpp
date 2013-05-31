// BuffPacket.h
// ����ʵ��ģ�����
// һ��������ɾ���Ϊ�࣬����ʵ��Ŀ�ꡣ
// add by freeeyes
// 2009-02-20

#include "LoadModule.h"

CLoadModule::CLoadModule(void)
{
	m_szModulePath[0] = '\0';
}

CLoadModule::~CLoadModule(void)
{
	OUR_DEBUG((LM_INFO, "[CLoadModule::~CLoadModule].\n"));
	//Close();
}

void CLoadModule::Close()
{
	//�滷�رյ�ǰ��Ծģ��
	for(int i = 0; i < m_mapModuleInfo.GetSize(); i++)
	{
		//ж�ز�ɾ������new��module����
		UnLoadModule(m_mapModuleInfo.GetMapDataKey(i).c_str());
		_ModuleInfo* pModuleInfo = (_ModuleInfo* )m_mapModuleInfo.GetMapData(i);
		SAFE_DELETE(pModuleInfo);
	}

	m_mapModuleInfo.Clear();
}

bool CLoadModule::LoadModule(const char* szModulePath, const char* szResourceName)
{
	vector<string> vecModuleName;
	string strModuleName;

	//���ַ�����������ɵ�һ��ģ������
	ParseModule(szResourceName, vecModuleName);

	sprintf_safe(m_szModulePath, MAX_BUFF_200, "%s", szModulePath);

	for(uint16 i = 0; i < (uint16)vecModuleName.size(); i++)
	{
		strModuleName = vecModuleName[i];
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] Begin Load ModuleName[%s]!\n", strModuleName.c_str()));

		//ȷ�����ģ���Ƿ�ע���
		_ModuleInfo* pCurr = m_mapModuleInfo.SearchMapData(strModuleName);
		if(NULL != pCurr)
		{
			//�����ע�������ж�����еģ�������װ��
			UnLoadModule(strModuleName.c_str());
		}

		_ModuleInfo* pModuleInfo = new _ModuleInfo();

		if(NULL == pModuleInfo)
		{
			OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] new _ModuleInfo is error!\n"));
			return false;
		}

		//��ʼע��ģ��
		if(false == LoadModuleInfo(strModuleName, pModuleInfo))
		{
			SAFE_DELETE(pModuleInfo);
			return false;
		}

        //��ע��ɹ���ģ�飬���뵽map��
		if(false == m_mapModuleInfo.AddMapData(pModuleInfo->GetName(), pModuleInfo))
		{
			OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] m_mapModuleInfo.AddMapData error!\n"));
			SAFE_DELETE(pModuleInfo);
			return false;
		}

		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] Begin Load ModuleName[%s] OK!\n", strModuleName.c_str()));
	}

	return true;
}

bool CLoadModule::UnLoadModule(const char* szResourceName)
{
	string strModuleName = szResourceName;
	_ModuleInfo* pModuleInfo = m_mapModuleInfo.SearchMapData(strModuleName);
	if(NULL == pModuleInfo)
	{
		return false;
	}
	else
	{
		//����ʹ˹��������ж���
		pModuleInfo->UnLoadModuleData();

		//�����ӳ�һ�£���Ϊ�п������ڴ���ǰ��Ϣ�����Ա����������ӳ�һ�£���ֹ�����ı��˴���ģʽ�����ַ�ʽ�Ǻܴ����ģ�������δ������֮��
		//ACE_Time_Value tvSleep(MAX_LOADMODEL_CLOSE, 0);
		//ACE_OS::sleep(tvSleep);
		
		//���ģ���������������
		int nRet = ACE_OS::dlclose(pModuleInfo->hModule);
		m_mapModuleInfo.DelMapData(strModuleName);
		
		OUR_DEBUG((LM_ERROR, "[CLoadModule::UnLoadModule] Close Module=%s, nRet=%d!\n", strModuleName.c_str(), nRet));

		return true;
	}
}

int CLoadModule::GetCurrModuleCount()
{
	return m_mapModuleInfo.GetSize();
}

_ModuleInfo* CLoadModule::GetModuleIndex(int nIndex)
{
	return m_mapModuleInfo.GetMapData(nIndex);
}

_ModuleInfo* CLoadModule::GetModuleInfo(const char* pModuleName)
{
	string strModuleName = pModuleName;
	return m_mapModuleInfo.SearchMapData(strModuleName);
}

bool CLoadModule::ParseModule(const char* szResourceName, vector<string>& vecModuleName)
{
	char szResource[MAX_BUFF_1024]  = {'\0'};
	char szModuleName[MAX_BUFF_100] = {'\0'};
	int  nPos = 0;

	if(szResourceName[ACE_OS::strlen(szResourceName) - 1] != ',')
	{
		sprintf_safe(szResource, MAX_BUFF_1024, "%s,", szResourceName);
	}
	else
	{
		sprintf_safe(szResource, MAX_BUFF_1024, "%s", szResourceName);
	}

	for(uint16 i = 0; i < (uint16)ACE_OS::strlen(szResource); i++)
	{
		if(szResource[i] == ',')
		{
			szModuleName[nPos] = '\0';
			nPos = 0;
			vecModuleName.push_back((string)szModuleName);
		}
		else
		{
			szModuleName[nPos] = szResource[i];
			nPos++;
		}
	}

	return true;
}

bool CLoadModule::LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo)
{
	char szModuleFile[MAX_BUFF_200] = {'\0'};
	if(NULL == pModuleInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, pModuleInfo is NULL!\n", strModuleName.c_str()));
		return false;
	}

	pModuleInfo->strModulePath = m_szModulePath;

	sprintf_safe(szModuleFile, MAX_BUFF_200, "%s%s", m_szModulePath, strModuleName.c_str());

	m_tmModule.acquire();

	pModuleInfo->hModule = ACE_OS::dlopen(szModuleFile, RTLD_NOW);
	if(NULL == pModuleInfo->hModule || !pModuleInfo->hModule)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, pModuleInfo->hModule is NULL(%s)!\n", strModuleName.c_str(), ACE_OS::dlerror()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->LoadModuleData = (int(*)(CServerObject*))ACE_OS::dlsym(pModuleInfo->hModule, "LoadModuleData");
	if(NULL == pModuleInfo->LoadModuleData || !pModuleInfo->LoadModuleData)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function LoadMoudle is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->UnLoadModuleData = (int(*)())ACE_OS::dlsym(pModuleInfo->hModule, "UnLoadModuleData");
	if(NULL == pModuleInfo->UnLoadModuleData || !pModuleInfo->UnLoadModuleData)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function UnloadModule is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->GetDesc = (const char*(*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetDesc");
	if(NULL == pModuleInfo->GetDesc || !pModuleInfo->GetDesc)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetDesc is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->GetName = (const char*(*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetName");
	if(NULL == pModuleInfo->GetName || !pModuleInfo->GetName)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetName is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->GetModuleKey = (const char*(*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetModuleKey");
	if(NULL == pModuleInfo->GetModuleKey || !pModuleInfo->GetModuleKey)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetModuleKey is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->DoModuleMessage = (int(*)(uint16, IBuffPacket*, IBuffPacket*))ACE_OS::dlsym(pModuleInfo->hModule, "DoModuleMessage");
	if(NULL == pModuleInfo->DoModuleMessage || !pModuleInfo->DoModuleMessage)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function DoModuleMessage is error(%d)!\n", strModuleName.c_str(), errno));
		m_tmModule.release();
		return false;
	}

	//����ģ�����
	int nRet = pModuleInfo->LoadModuleData(App_ServerObject::instance());
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Execute Function LoadModuleData is error!\n", strModuleName.c_str()));
		m_tmModule.release();
		return false;
	}

	pModuleInfo->strModuleName = strModuleName;
	m_tmModule.release();
	return true;
}

int CLoadModule::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
	_ModuleInfo* pModuleInfo = m_mapModuleInfo.SearchMapData((string)pModuleName);
	if(NULL != pModuleInfo)
	{
		pModuleInfo->DoModuleMessage(u2CommandID, pBuffPacket, pReturnBuffPacket);
	}

	return 0;
}

