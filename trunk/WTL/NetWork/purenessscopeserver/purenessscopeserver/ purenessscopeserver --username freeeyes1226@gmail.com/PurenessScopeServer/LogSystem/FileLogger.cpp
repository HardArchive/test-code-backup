#include "FileLogger.h"

CFileLogger::CFileLogger()
{
	m_nCount       = 0;
  m_szLogRoot[0] = '\0';
}

CFileLogger::~CFileLogger()
{
	OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger].\n"));
	for(mapLogFile::iterator b = m_mapLogFile.begin(); b!= m_mapLogFile.end(); b++)
	{
		CLogFile* pLogFile = (CLogFile* )b->second;
		SAFE_DELETE(pLogFile);
	}

	m_mapLogFile.clear();
	m_vecLogType.clear();
	OUR_DEBUG((LM_INFO, "[CFileLogger::~CFileLogger]End.\n"));
}

int CFileLogger::DoLog(int nLogType, ACE_TString* pLogText)
{
	mapLogFile::iterator f = m_mapLogFile.find(nLogType);
	if(f == m_mapLogFile.end())
	{
		return -1;
	}
	else
	{
		CLogFile* pLogFile = (CLogFile* )f->second;
		pLogFile->doLog(pLogText);
	}		

	return 0;
}

int CFileLogger::GetLogTypeCount()
{
	return (int)m_vecLogType.size();
}

int CFileLogger::GetLogType(int nIndex)
{
	if(nIndex >= (int)m_vecLogType.size())
	{
		return 0;
	}

	return (int)m_vecLogType[nIndex];
}

bool CFileLogger::Init()
{
	CXmlOpeation objXmlOpeation;
  uint16 u2LogID                  = 0;
	uint8 u1FileClass               = 0;
	char szFile[MAX_BUFF_1024]      = {'\0'};
  char szFileName[MAX_BUFF_100]   = {'\0'};
  char szServerName[MAX_BUFF_100] = {'\0'};
  char* pData = NULL;

	m_vecLogType.clear();

	sprintf_safe(szFile, MAX_BUFF_1024, "%s%s", App_MainConfig::instance()->GetModulePath(), FILELOG_CONFIG);
	if(false == objXmlOpeation.Init(szFile))
	{
		OUR_DEBUG((LM_ERROR,"[CFileLogger::Init] Read Configfile[%s] failed\n", szFile));
		return false; 
	}

	//得到服务器名称
	//conf.GetValue("ServferName",strServerName,"\\ServerInfo");
	pData = objXmlOpeation.GetData("ServerLogHead", "Text");
  if(pData != NULL)
  {
    sprintf_safe(szServerName, MAX_BUFF_100, "%s", pData);
  }
  OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strServerName=%s\n", szServerName));	

	//得到绝对路径
	pData = objXmlOpeation.GetData("LogPath", "Path");
  if(pData != NULL)
  {
	  sprintf_safe(m_szLogRoot, MAX_BUFF_100, "%s", pData);
  }
	OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]m_strRoot=%s\n", m_szLogRoot));

	//添加子类的个数
  TiXmlElement* pNextTiXmlElement    = NULL;
  TiXmlElement* pNextTiXmlElementPos = NULL;
  TiXmlElement* pNextTiXmlElementIdx = NULL;

	while(true)
	{
    pData = objXmlOpeation.GetData("LogInfo", "logid", pNextTiXmlElementIdx);  
    if(pData != NULL)
    {
      u2LogID = (uint16)atoi(pData);                                                      
      OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u2LogID=%d\n", u2LogID));
    }
    else
    {
      break;
    }

		pData = objXmlOpeation.GetData("LogInfo", "logname", pNextTiXmlElement);
    if(pData != NULL)
    {
      sprintf_safe(szFileName, MAX_BUFF_100, "%s", pData);
		  OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]strFileValue=%s\n", szFileName));
    }
    else
    {
      break;
    }

		pData = objXmlOpeation.GetData("LogInfo", "logtype", pNextTiXmlElementPos);  
    if(pData != NULL)
    {
		  u1FileClass = (uint8)atoi(pData);                                                      
		  OUR_DEBUG((LM_ERROR, "[CFileLogger::readConfig]u1FileClass=%d\n", u1FileClass));
    }
    else
    {
      break;
    }

		//添加到管理日志文件对象map中
		mapLogFile::iterator f = m_mapLogFile.find(u2LogID);

		if(f != m_mapLogFile.end())
		{
			continue;
		}

		CLogFile* pLogFile = new CLogFile();

		pLogFile->SetLoggerName(szFileName);
		pLogFile->SetLoggerType((int)u2LogID);
		pLogFile->SetLoggerClass((int)u1FileClass);
		pLogFile->SetServerName(szServerName);
		pLogFile->Run();

		m_mapLogFile.insert(mapLogFile::value_type(pLogFile->GetLoggerType(), pLogFile));
		m_vecLogType.push_back(u2LogID);

	}

	return true;
}

bool CFileLogger::Close()
{
	for(mapLogFile::iterator b = m_mapLogFile.begin(); b != m_mapLogFile.end(); b++)
	{
		CLogFile* pLogFile = (CLogFile* )b->second;
		delete pLogFile;
	}

	m_mapLogFile.clear();
	m_vecLogType.clear();
	m_nCount = 0;

	return true;
}
