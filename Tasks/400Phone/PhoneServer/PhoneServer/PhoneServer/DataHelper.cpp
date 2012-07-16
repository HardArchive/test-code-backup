#include "stdafx.h"
#include "DataHelper.h"

//连接智能指针
_ConnectionPtr CDataHelper::m_pConnPtr;
//数据库连接字符串
char CDataHelper::szConn[512]={0};

int SEVER_PORT;

//获取注册表信息
void CDataHelper::GetRegConfig(LPCTSTR pKey,char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	char	sTemp[128] = {0};
	
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\400phone"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
	{
		RegCloseKey(hkey);
		return;
	}
	if (RegQueryValueEx(hkey,pKey, 0, &dwType, (LPBYTE)&sTemp, (LPDWORD)&dwLenth) != 0 )
	{
		RegCloseKey(hkey);
		return;
	}
	RegCloseKey(hkey);
	strcpy(pValue, sTemp);
}

//初始化连接
BOOL CDataHelper::InitConnection()
{
	::CoInitialize(NULL);
	//获取数据库连接字符串
	char szServer[32]={0};
	char szDatabase[128]={0};
	char szUId[64]={0};
	char szPwd[64]={0};
	GetRegConfig(TEXT("Server"),szServer);
	GetRegConfig(TEXT("Database"),szDatabase);
	GetRegConfig(TEXT("UId"),szUId);
	GetRegConfig(TEXT("Pwd"),szPwd);
	if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	{
		::OutputDebugString(_T("400Phone服务 从注册表获取数据库连接参数失败!\r\n"));
		return FALSE;
	}
	

	sprintf_s(szConn,512,"driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",szServer,szDatabase,szUId,szPwd);

	//创建并打开数据连接对象-----------------------------------------------------------------------------
	HRESULT hr;
	hr=m_pConnPtr.CreateInstance(__uuidof(Connection));
	if(FAILED(hr))
	{
		::OutputDebugString(_T("400Phone服务create ADODB.Connection failure !\r\n"));
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr.Release();
		}
		return FALSE;
	}
	try
	{
		hr=m_pConnPtr->Open(szConn,"","",adModeUnknown);
		if(FAILED(hr))
		{
			m_pConnPtr.Release();
			TracePrint(LOG_ERROR,TEXT("400Phone服务 ADODB.Connection open failure ! \n"));
			return FALSE;
		}
	}
	catch(...)
	{
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr.Release();
		}
		TracePrint(LOG_ERROR,TEXT("400Phone服务 ADODB.Connection open failure ! \n"));
		return FALSE;
	}
	 
	::OutputDebugString(_T("400Phone服务ADODB.Connection open success !\r\n"));
	//----------------------------------------------------------------------------------------------------
	
	//初始化成功
	return TRUE;
}

//关闭连接
void CDataHelper::ExitConnection()
{
	if(m_pConnPtr!=NULL)
	{
		m_pConnPtr->Close();
	    m_pConnPtr.Release();
	}
	::CoUninitialize();
}
//在连接对象上执行SQL语句
BOOL CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//如果连接关闭则打开连接
	if(!OpenConnection())
	{
		return FALSE;
	}

	BOOL bRet=FALSE;
	_CommandPtr spCmdPtr=NULL;

	//创建Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("400Phone服务 create ADODB.Command failure ! \n"));
		if(spCmdPtr!=NULL)
		{
			spCmdPtr.Release();
		}
		return FALSE;
	}

	try
	{
		//为Command添加参数
		spCmdPtr->CommandType=adCmdText;
		spCmdPtr->CommandText=lpSql;

		//执行存储过程
		spCmdPtr->ActiveConnection=m_pConnPtr;
		spCmdPtr->Execute(NULL,NULL,adCmdText);
		bRet=TRUE;
	}
	catch(_com_error ex)
	{ 
		TracePrint(LOG_ERROR,TEXT("400Phone服务 ExecuteSql failure !%d \n"), GetLastError());
		bRet=FALSE;
	}
	if(spCmdPtr!=NULL)
	{
		spCmdPtr.Release();
	}
	return bRet;
}

/*
//在连接对象上执行SQL语句
BOOL CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//如果连接关闭则打开连接
	if(!OpenConnection())
	{
		return FALSE;
	}

	BOOL bRet=FALSE;
	_CommandPtr spCmdPtr=NULL;
	//_RecordsetPtr pRecordset = NULL;

	//创建Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("400Phone服务 create ADODB.Command failure ! \n"));
		if(spCmdPtr!=NULL)
		{
			spCmdPtr.Release();
		}
		return FALSE;
	}

	try
	{
		//为Command添加参数
		spCmdPtr->CommandType=adCmdText;
		spCmdPtr->CommandText=lpSql;

		//执行存储过程
		spCmdPtr->ActiveConnection=m_pConnPtr;
		spCmdPtr->Execute(NULL,NULL,adCmdText);
		bRet=TRUE;
	}
	catch(_com_error ex)
	{ 
		TracePrint(LOG_ERROR,TEXT("400Phone服务 ExecuteSql failure ! \n"));
		bRet=FALSE;
	}
	//if (iType)
	//{
	//	iType = pRecordset->GetRecordCount();

	//	if (iType<0)
	//	{
	//		bRet=FALSE;
	//	}
	//	else
	//	{
	//		bRet = TRUE;
	//	}
	//}
	//pRecordset = NULL;

	if(spCmdPtr!=NULL)
	{
		spCmdPtr.Release();
	}

	
	return bRet;
}
*/

//打开到数据库的连接
BOOL CDataHelper::OpenConnection()
{
	//如果连接关闭则打开连接
	if(!TestConnection())
	{
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr->Close();
			m_pConnPtr.Release();
		}
		 
		HRESULT hr=m_pConnPtr.CreateInstance(__uuidof(Connection));
		if(FAILED(hr))
		{
			::OutputDebugString(_T("400Phone服务create ADODB.Connection failure !\r\n"));
			return FALSE;
		}
		try
		{
			hr=m_pConnPtr->Open(szConn,"","",adModeUnknown);
			if(FAILED(hr))
			{
				TracePrint(LOG_ERROR,TEXT("400Phone服务 ADODB.Connection open failure ! \n"));
				if(m_pConnPtr !=NULL)
				{
				    m_pConnPtr.Release();
				}
				return FALSE;
			}
		}
		catch(...)
		{
			TracePrint(LOG_ERROR,TEXT("400Phone服务 ADODB.Connection open failure ! \n"));
			if(m_pConnPtr !=NULL)
			{
			    m_pConnPtr.Release();
			}
			return FALSE;
		}
	}
	return TRUE;
}

//测试连接
BOOL CDataHelper::TestConnection()
{
	_RecordsetPtr spRecord=NULL;
	BOOL bRet=FALSE;
	CString str("select 1 as t");
	
	try
	{
		_variant_t vt;
		spRecord=m_pConnPtr->Execute(_bstr_t(str),&vt,adCmdText);
		if(spRecord!=NULL)
		{
			_variant_t vtRet=spRecord->GetCollect("t");
			if(vtRet.intVal ==1)
			{
				bRet=TRUE;
			}
		}
	}
	catch(...)
	{
		 bRet=FALSE;
	}
	if(spRecord !=NULL)
	{
		spRecord->Close();
		spRecord.Release();
	}
	return bRet;
}

BOOL CDataHelper::AddPhoneAndPwd(const char *pszPhone,const char *pszPwd)
{
	char szSql[512]={0};

	////查询代码
	////SELECT * FROM tUser WHERE sAccount='13621607084'
	//sprintf_s(szSql,512,"SELECT * FROM tUser WHERE sAccount='%s'", pszPhone);
	//if (CDataHelper::ExecuteSql((LPCTSTR)szSql, 1))
	//{//查询到记录
	//	ZeroMemory(szSql, sizeof(szSql));
	//	// 更新代码
	//	// UPDATE tUser SET sPwd='%s' WHERE sAccount='%s'  UPDATE tUser SET sPwd='1111' WHERE sAccount='13621607084'
	//	sprintf_s(szSql,512,"UPDATE tUser SET sPwd='%s' WHERE sAccount='%s'", pszPwd, pszPhone);
	//}
	//else
	//{//未查询到
	//	ZeroMemory(szSql, sizeof(szSql));
	//	sprintf_s(szSql,512,"insert into tUser(sAccount,sPwd) values('%s','%s')",pszPhone,pszPwd);		
	//}
	
	//sprintf_s(szSql,512,"insert into tUser(sAccount,sPwd) values('%s','%s')",pszPhone,pszPwd);	
	sprintf_s(szSql,512,"exec dbo.dt_Save400Phone '%s','%s'",pszPhone, pszPwd);	  //执行存储过程
	return CDataHelper::ExecuteSql((LPCTSTR)szSql);
}