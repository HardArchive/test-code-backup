#include "StdAfx.h"
#include "MMF.h"
#include "ShareRestrictedSD.h"

MMF::MMF(const string& szMMName):m_szMMName(szMMName),m_hMapFile(NULL)
{
}

MMF::~MMF(void)
{
	CloseMapFile();
}
bool MMF::CreateMapFile()
{
	CShareRestrictedSD ShareRestrictedSD;
	if(NULL == m_hMapFile)
	{
		m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, 
			ShareRestrictedSD.GetSA(),
			PAGE_READWRITE,
			0,
			1000,
			m_szMMName.c_str());
	}
	return NULL != m_hMapFile;
}
bool MMF::OpenMapFile()
{
	if(NULL == m_hMapFile)
	{
		m_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
			TRUE,
			m_szMMName.c_str());
	}
	return NULL != m_hMapFile;
}
void MMF::CloseMapFile()
{
	if(m_hMapFile)
	{
		::CloseHandle(m_hMapFile);
		m_hMapFile = NULL;
	}
}
bool MMF::WriteData(const char* szData,int nLen)
{
	LPVOID lpRet = MapViewOfFile(m_hMapFile,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
	if(lpRet)
	{
		memcpy((char*)lpRet,szData,nLen);
		::UnmapViewOfFile(lpRet);
		return true;
	}
	else
	{
		return false;
	}
}
bool MMF::ReadData(string& szData)
{
	LPVOID lpRet = MapViewOfFile(m_hMapFile,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
	if(lpRet)
	{
		int nCopyLen = strlen((char*)lpRet);
		char *pBuffer = new char[nCopyLen];
		memset(pBuffer,0,nCopyLen);
		memcpy(pBuffer,lpRet,nCopyLen);
		szData = string(pBuffer,nCopyLen);
		::UnmapViewOfFile(lpRet);
		return true;
	}
	else
	{
		return false;
	}
}
