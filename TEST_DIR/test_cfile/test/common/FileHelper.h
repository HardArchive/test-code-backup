#pragma once

#include "file1.h"
#include "comm_protocol.h"
#include "RegHelper.h"
#include <sstream> 
using namespace RG;
class CFileHelper
{
public:

	CFileHelper(void)
	{
		InitializeCriticalSection(&m_lock);
		CreateAllFile();
	}

	~CFileHelper(void)
	{
		CloseAllFile();
		DeleteCriticalSection(&m_lock);
	}

public:
	// 获取实例
	static CFileHelper* GetInstance()
	{
		EnterCriticalSection(&m_lock);
		if (NULL == m_pInstance)
		{
			m_pInstance = new CFileHelper;
		}
		LeaveCriticalSection(&m_lock);
		return m_pInstance;
	}
	////增加用户信息
	//int AddCustomerInfo(CUSTOMERINFO& stuCustomerInfo)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////增加用户上下机日志
	//int AddCustomerLoginOutLog(CUSTOMERLOGINOUTLOG& stuCustomerLoginoutLog)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}	
	////增加单个主机信息
	//int AddBarterminalInfo(BARTERMINALINFO& stuBarterminalInfo)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////增加所有主机信息
	//int AddBarterminalInfoAll(PBYTE  pszOutBuf, int iBufLen)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////增加报警-策略碰撞记录
	//int AddTacticsMatchRecord()
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////增加命令截图
	//int AddScreenshot(PBYTE  pszOutBuf, int iBufLen)
	//{
	//	int iRet = 0;
	//	PSCREENSHOT pstuSereenshot = (PSCREENSHOT)pszOutBuf;

	//	return iRet;
	//}

private:

	bool Init()
	{		
		return CreateAllFile();		
	}

	bool CreateAllFile()
	{
		CloseAllFile();

		if (!CreateFile(m_clsFileBarterminalInfo, "BarterminalInfo")) return false;
		if (!CreateFile(m_clsFileCustomerInfo, "CustomerInfo")) return false;
		if (!CreateFile(m_clsFileCustomerLoginOutLog, "CustomerLoginOutLog")) return false;
		if (!CreateFile(m_clsFileScreenshot, "Screenshot")) return false;
		if (!CreateFile(m_clsFileTacticsMatchRecord, "TacticsMatchRecord")) return false;
		return true;
	}

	bool CreateFile(CFile& clsFile, string& strFileNameContent)
	{
		string strFileName;
		stringstream strStream;  
		DATE dtCurrentTime = GetTickCount();
		strStream << m_strPath<< strFileNameContent<<"_"<<(int)dtCurrentTime<<".txt";  
		strFileName = strStream.str(); 
		return clsFile(clsFile, strFileName);
	}

	bool OpenFile(CFile& clsFile, string& strFileName)
	{
		CloseFile(clsFile);
		return clsFile.Open(strFileName, "wt+");
	}

	void CloseFile(CFile& clsFile)
	{
		clsFile.Close();
	}

	void CloseAllFile()
	{
		m_clsFileCustomerInfo.Close();
		m_clsFileCustomerLoginOutLog.Close();
		m_clsFileBarterminalInfo.Close();
		m_clsFileTacticsMatchRecord.Close();
		m_clsFileScreenshot.Close();
	}

	bool CheckIsFull()
	{

	}

	bool GetSavePath()
	{
		TCHAR* ptTem = NULL;
		RG::CRegUtil clsRegUtil;
		clsRegUtil.GetRegistryValue(_T("SOFTWARE\\RainSoft\\RSDataToDb"), _T("SourcePath"), REG_SZ, (PVOID*)&ptTem);
		m_strPath = ptTem;
		delete ptTem;
		ptTem = NULL;
	}

private:
	// 互斥对象
	CRITICAL_SECTION m_lock;	
	static CFileHelper* m_pInstance;
	//文件读写句柄
	CFile m_clsFileCustomerInfo;         //增加用户信息
	CFile m_clsFileCustomerLoginOutLog;  //增加用户上下机日志
	CFile m_clsFileBarterminalInfo;      //增加单个主机信息
	CFile m_clsFileTacticsMatchRecord;   //增加报警-策略碰撞记录
	CFile m_clsFileScreenshot;           //增加命令截图
	//文件保存路径
	string m_strPath;            
};
