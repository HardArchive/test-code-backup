#ifndef _FILELOGGER_H
#define _FILELOGGER_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Date_Time.h"

#include "define.h"
#include "MainConfig.h"
#include "ILogObject.h"

#include <map>
#include <vector>

using namespace std;

#define MAX_CMD_NUM   100
#define MAX_TIME_SIZE 100

//����Log��������
#define LOGTYPE_OPERATION "Operation"
#define LOGTYPE_ERROR     "ServerError"

#define FILELOG_CONFIG    "logger.xml"

//��Ԫģʽ��־��
class CLogFile {
public:
	CLogFile()
	{
		m_nType             = 0;
		m_StrServerName     = "";
		m_StrlogType        = "ServerError";
	};

	virtual ~CLogFile()
	{
		OUR_DEBUG((LM_INFO,"[CLogFile::~CLogFile].\n"));
		m_File.close();
		OUR_DEBUG((LM_INFO,"[CLogFile::~CLogFile] End.\n"));
	};	

	virtual int doLog(ACE_TString * pStrLog)
	{
		//ÿ���Զ����
		CheckTime();

		ACE_Date_Time dt;
		char szDateBuff[MAX_TIME_SIZE] = {'\0'};
		ACE_TString strLog;

		sprintf_safe(szDateBuff, MAX_TIME_SIZE, "%04d-%02d-%02d %02d:%02d:%02d%02d,", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dt.microsec()/10000);
		strLog = szDateBuff + *pStrLog + '\n';

		int nLen = m_File.send(strLog.c_str(), strLog.length());
		if(nLen != (int)strLog.length())
		{
			OUR_DEBUG((LM_INFO,"[%s]Write error[%s].\n", m_StrlogName.c_str(), strLog.c_str()));
		}
		return 0;
	};

	ACE_TString& GetLoggerName()
	{
		return m_StrlogName;
	};

	void SetLoggerClass(int nType)
	{
		if(nType == 1)  //�����1��������Ӫ��־��������Ǵ�����־
		{
			m_StrlogType = LOGTYPE_OPERATION;
		}
		else
		{
			m_StrlogType = LOGTYPE_ERROR;
		}
		OUR_DEBUG((LM_INFO,"[ServerLogger](%d)m_StrlogType=%s.\n", nType, m_StrlogType.c_str()));
	}

	void SetLoggerName(const char* szLoggerName)
	{
		m_StrlogName = szLoggerName;
	};

	void SetLoggerType(int nType)
	{
		m_nType = nType;
	}

	int GetLoggerType()
	{
		return m_nType;
	}

	void SetServerName(const char* szServerName)
	{
		m_StrServerName = szServerName;
	}

	bool Run()
	{
		ACE_Date_Time dt;
		char szDateBuff[MAX_TIME_SIZE] = {'\0'};

		dt.update(ACE_OS::gettimeofday());

		CreatePath();       //���Ŀ¼�������򴴽�Ŀ¼

		sprintf_safe(szDateBuff, MAX_TIME_SIZE, "_%04d-%02d-%02d.log", dt.year(), dt.month(), dt.day());
		sprintf_safe(m_szLogTime, MAX_TIME_SIZE, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());

		ACE_TString strLogModulePath = App_MainConfig::instance()->GetModulePath();
		ACE_TString strLogName       = strLogModulePath + "/Log/" + m_StrlogType + "/" + m_StrlogName + "/" + m_StrServerName + m_StrlogName + szDateBuff;

		m_File.close();

		m_FileAddr.set((ACE_TCHAR *)strLogName.c_str());
		if(m_Connector.connect(m_File, m_FileAddr, 0, ACE_Addr::sap_any, 0, O_WRONLY|O_CREAT|O_APPEND) == -1)
		{
			OUR_DEBUG((LM_INFO,"[ServerLogger]Create file error[%s].\n", strLogName.c_str()));
			return false;
		}

		OUR_DEBUG((LM_INFO,"[ServerLogger]Create file ok[%s].\n", strLogName.c_str()));
		//m_File.truncate(0);
		return true;
	}

	void CheckTime()
	{
		ACE_Date_Time dt;
		dt.update(ACE_OS::gettimeofday());
		char szDate[MAX_TIME_SIZE] = {'\0'};
		sprintf_safe(szDate, MAX_TIME_SIZE, "%04d-%02d-%02d", dt.year(), dt.month(), dt.day());

		if(ACE_OS::strcmp(szDate, m_szLogTime) != 0)
		{
			Run();
		}
	}

	void CreatePath()
	{
		char szPath[MAX_CMD_NUM] = {'\0'};
		sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/", App_MainConfig::instance()->GetModulePath());
		ACE_OS::mkdir(szPath);

		sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/%s/", App_MainConfig::instance()->GetModulePath(), m_StrlogType.c_str());
		ACE_OS::mkdir(szPath);

		sprintf_safe(szPath, MAX_CMD_NUM, "%s/Log/%s/%s", App_MainConfig::instance()->GetModulePath(), m_StrlogType.c_str(), m_StrlogName.c_str());
		ACE_OS::mkdir(szPath);
	}

private:
	ACE_TString         m_StrlogName;         //ģ������
	ACE_TString         m_StrlogType;         //��־����
	ACE_TString         m_StrServerName;      //��־����
	int                 m_nType;              //ģ��ID
	ACE_FILE_Connector  m_Connector;          //I/O����������
	ACE_FILE_IO         m_File;
	ACE_FILE_Addr       m_FileAddr; 
	char                m_szLogTime[MAX_TIME_SIZE];

};

class CFileLogger : public CServerLogger
{
public:
	CFileLogger();
	~CFileLogger();

	int DoLog(int nLogType, ACE_TString* pLogText);
	int GetLogTypeCount();
	int GetLogType(int nIndex);

	bool Init();
	bool Close();

private:
	typedef map<uint16, CLogFile*> mapLogFile;
	typedef vector<uint16>         vecLogType; 
	mapLogFile                  m_mapLogFile;
	vecLogType                  m_vecLogType;
	char                        m_szLogRoot[MAX_BUFF_100];	
	int                         m_nCount;
};

#endif
