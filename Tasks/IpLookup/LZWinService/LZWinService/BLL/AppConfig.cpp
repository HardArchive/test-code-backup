/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: WorkThread.cpp
 * Author: ��  ��    Version: 1.0    Date: 2011-08-18
 * Description: Э������߳���
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-08-18    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include "direct.h"
#include ".\AppConfig.h"
#include "Common.h"

CAppConfig* CAppConfig::m_instance = NULL;
char CAppConfig::m_empty = '\0';

// ע���λ��
static const LPCTSTR lpSubKey = "SOFTWARE\\RainSoft\\ManageCenter2011\\ReceiveSave";

// ���캯��
CAppConfig::CAppConfig(): m_RunMode(0)
{
	////memset(SourcePath, 0, sizeof(SourcePath));
	////memset(FilePath, 0, sizeof(FilePath));
	////memset(DestPath, 0, sizeof(DestPath));
	////memset(ErrorPath,0, sizeof(ErrorPath));

	memset(AppDir, 0, sizeof(AppDir));
	int nLen = GetModuleFileName(NULL, this->AppDir, sizeof(this->AppDir));
	for (int i=nLen-1;i>0;i--)
	{
		if (AppDir[i]=='\\')
		{
			AppDir[i] = '\0';
			break;
		}
	}
}

// ���캯��
CAppConfig::~CAppConfig()
{

}

CAppConfig** CAppConfig::GetInstance()
{
	return &m_instance;
}

void CAppConfig::Free()
{
	if(m_instance != NULL)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

bool CAppConfig::Init()
{
	m_instance = NULL;
	if(m_instance == NULL)
	{
		m_instance = new CAppConfig();
	}
	
	// ��������
	m_instance->LoadData();

	return true;
}

char* CAppConfig::Empty()
{
	return &m_empty;
}

bool CAppConfig::LoadDataFromRegister()
{
	HKEY  hKey, hSubKey, hDbSubKey;
	LONG  lRet;

	TCHAR szData[MAX_PATH] = {0};
	ULONG uCount = 0;

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T(lpSubKey), 
		0, KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS, &hKey);
	if (lRet != ERROR_SUCCESS) 
	{
		TracePrint(LOG_INFO, "RSRcvSave CGlobalData::LoadData--> Open Registery error!\r\n");
		return FALSE;
	}

#pragma region �������
	
#pragma endregion

#pragma region ��ص�Ŀ¼����
	// Դ�ļ�Ŀ¼������ļ�Ŀ¼�� ԭ�ļ�����Ŀ¼������Ŀ¼
	////GetRegPath(hKey, "SourcePath", this->SourcePath, "d:\\testout\\Source\\");
	////GetRegPath(hKey, "DestPath", this->DestPath, "d:\\testout\\Out\\");
	////GetRegPath(hKey, "FilePath", this->FilePath, "d:\\testout\\File\\");
	////GetRegPath(hKey, "ErrorPath", this->ErrorPath, "d:\\testout\\Error\\");
	//////GetRegPath(hKey, "OtherPath", this->OtherPath, "d:\\testout\\Other\\");
#pragma endregion

	DWORD dwBufLen = sizeof(m_RunMode);
	lRet = RegQueryValueEx(hKey, _T("RunMode"), NULL, NULL,
		(LPBYTE)&(m_RunMode), &dwBufLen);
	if ((lRet != ERROR_SUCCESS) || (dwBufLen > sizeof(m_RunMode)))
	{
		// ��ȡʧ�ܣ�Ĭ��Ϊ������ģʽ
		m_RunMode = 0;
	}
	////// ץ������
	////uCount = MAX_PATH * sizeof(TCHAR);
	////ZeroMemory(m_Adapter, uCount);
	////if(Reg.QueryStringValue(_T("Adapter"),m_Adapter,&uCount) != ERROR_SUCCESS) m_Adapter[0] = '\0';
	//////else m_Adapter = szData;
	////StrReplace(m_Adapter, "Packet_", "NPF_");

	// �ر�ע�����
	RegCloseKey(hKey);

	return true;
}

void CAppConfig::WriteStringRegistry( LPCTSTR strName, const string & strValue )
{
	CRegKey Reg;
	if (Reg.Open(HKEY_LOCAL_MACHINE,lpSubKey)!=ERROR_SUCCESS)
	{
		Reg.Close();
		return ;
	}
	Reg.SetStringValue(strName,strValue.c_str());
	Reg.Close();
}


bool CAppConfig::LoadData()
{
	// ��ע����������
	return LoadDataFromRegister();
}

DWORD CAppConfig::IpStr2Dword( const char* pSrc )
{
	DWORD dwIp = 0;
	char szTemp[10]={0};
	char *p1 = szTemp;
	bool bIsValidNum = false;

	char *p = (char*)pSrc;
	for (int i=0;i<15; i++, p++)
	{
		// ����һ������
		if (*p == '.') 
		{
			bIsValidNum = false;
			dwIp = dwIp * 256;
			if (szTemp[0] != '\0') dwIp += atoi(szTemp);
			memset(szTemp, 0, sizeof(szTemp));
			p1 = szTemp;
			continue;
		}

		// 
		if (!bIsValidNum && *p != '0') bIsValidNum = true;

		// �����Ч����
		if (bIsValidNum)
		{
			*p1 = *p;
			p1++;
		}

		// �ַ�������
		if (*p == '\0')	break;
	}

	// �������һ������s
	dwIp = dwIp * 256;
	if (szTemp[0] != '\0') dwIp += atoi(szTemp);

	return dwIp;
}

int CAppConfig::GetRegPath( HKEY hKey, const char* pKeyName, char* pValue, const char* pDefault )
{
	DWORD dwBufLen = MAX_PATH;
	int lRet = RegQueryValueEx(hKey, pKeyName, NULL, NULL,
		(LPBYTE)pValue, &dwBufLen);
	if ((lRet != ERROR_SUCCESS) || (dwBufLen < 4) || (dwBufLen > MAX_PATH))
	{
		if (pDefault != NULL) sprintf_s(pValue, MAX_PATH, "%s", pDefault);
	}
	if (pValue[_tcslen(pValue)-1] != '\\')
		_tcsncat(pValue, "\\", 1);

	return lRet;
}