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
	// ��ȡʵ��
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
	////�����û���Ϣ
	//int AddCustomerInfo(CUSTOMERINFO& stuCustomerInfo)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////�����û����»���־
	//int AddCustomerLoginOutLog(CUSTOMERLOGINOUTLOG& stuCustomerLoginoutLog)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}	
	////���ӵ���������Ϣ
	//int AddBarterminalInfo(BARTERMINALINFO& stuBarterminalInfo)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////��������������Ϣ
	//int AddBarterminalInfoAll(PBYTE  pszOutBuf, int iBufLen)
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////���ӱ���-������ײ��¼
	//int AddTacticsMatchRecord()
	//{
	//	int iRet = 0;

	//	return iRet;
	//}
	////���������ͼ
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
	// �������
	CRITICAL_SECTION m_lock;	
	static CFileHelper* m_pInstance;
	//�ļ���д���
	CFile m_clsFileCustomerInfo;         //�����û���Ϣ
	CFile m_clsFileCustomerLoginOutLog;  //�����û����»���־
	CFile m_clsFileBarterminalInfo;      //���ӵ���������Ϣ
	CFile m_clsFileTacticsMatchRecord;   //���ӱ���-������ײ��¼
	CFile m_clsFileScreenshot;           //���������ͼ
	//�ļ�����·��
	string m_strPath;            
};
