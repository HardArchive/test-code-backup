/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: WorkThread.cpp
 * Author: 李  照    Version: 1.0    Date: 2011-08-18
 * Description: 协议解析线程类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-08-18    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include "direct.h"
#include ".\AppConfig.h"
#include "Common.h"

CAppConfig* CAppConfig::m_instance = NULL;
char CAppConfig::m_empty = '\0';

// 注册表位置
static const LPCTSTR lpSubKey = "SOFTWARE\\RainSoft\\ManageCenter2011\\ReceiveSave";

// 构造函数
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

// 构造函数
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
	
	// 加载数据
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

#pragma region 加密相关
	
#pragma endregion

#pragma region 相关的目录配置
	// 源文件目录、输出文件目录、 原文件保存目录、错误目录
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
		// 读取失败，默认为单进程模式
		m_RunMode = 0;
	}
	////// 抓包网卡
	////uCount = MAX_PATH * sizeof(TCHAR);
	////ZeroMemory(m_Adapter, uCount);
	////if(Reg.QueryStringValue(_T("Adapter"),m_Adapter,&uCount) != ERROR_SUCCESS) m_Adapter[0] = '\0';
	//////else m_Adapter = szData;
	////StrReplace(m_Adapter, "Packet_", "NPF_");

	// 关闭注册表项
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
	// 从注册表加载数据
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
		// 计算一组数据
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

		// 添加有效数据
		if (bIsValidNum)
		{
			*p1 = *p;
			p1++;
		}

		// 字符串结束
		if (*p == '\0')	break;
	}

	// 计算最后一组数据s
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