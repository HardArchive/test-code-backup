#include "StdAfx.h"
#include "SharedMemory.h"
#include "MMF.h"

SharedMemory::SharedMemory()
{
	
}
SharedMemory& SharedMemory::Instance()
{
	static SharedMemory sm;
	return sm;
}
SharedMemory::~SharedMemory(void)
{
	for(map<string,MMF*>::iterator i = m_mapMMF.begin(); i != m_mapMMF.end(); i++)
	{
		MMF* pMMF = i->second;
		if(pMMF)
		{
			pMMF->CloseMapFile();
			delete pMMF;
			pMMF = NULL;
		}
	}
	m_mapMMF.clear();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool SharedMemory::WriteSharedMemory(const string& szName,const string& szValue)
{
	map<string,MMF*>::iterator iter = m_mapMMF.find(szName);
	bool bRet = false;
	if(iter != m_mapMMF.end())
	{
		MMF* pMMF = iter->second;
		if(!pMMF)
		{
			return false;
		}
		bRet= pMMF->WriteData(szValue.c_str(),szValue.size());

	}
	else
	{
		MMF* pMMF = new MMF(szName);
		bRet = pMMF->CreateMapFile();
		if(!bRet)
		{
			return false;
		}
		bRet = pMMF->WriteData(szValue.c_str(),szValue.size());
		if(bRet)
		{
			m_mapMMF.insert(make_pair(szName,pMMF));
		}
	}
	return bRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
bool SharedMemory::ReadSharedMemory(const string& szName,string& szValue)
{
	map<string,MMF*>::iterator iter = m_mapMMF.find(szName);
	bool bRet = false;
	if(iter != m_mapMMF.end())
	{
		MMF* pMMF = iter->second;
		if(!pMMF)
		{
			return false;
		}
		bRet= pMMF->ReadData(szValue);
	}
	else
	{
		MMF* pMMF = new MMF(szName);
		bRet = pMMF->OpenMapFile();
		if(!bRet)
		{
			return false;
		}
		bRet = pMMF->ReadData(szValue);
		if(bRet)
		{
			m_mapMMF.insert(make_pair(szName,pMMF));
		}
	}
	return bRet;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void SharedMemory::DeleteSharedMemory(const string& szName)
{
	map<string,MMF*>::iterator iter = m_mapMMF.find(szName);
	if(iter != m_mapMMF.end())
	{
		MMF* pMMF = iter->second;
		if(pMMF)
		{
			pMMF->CloseMapFile();
			delete pMMF;
			pMMF = NULL;
		}
		m_mapMMF.erase(iter);
	}
}