#include "StdAfx.h"
#include "DataHelper.h"

//把_variant_t类型转换成char *,但不需要动态分配内存
BOOL WideStringToCharString(_variant_t &var,char *Dest,int destLen)
{
	if(var.vt !=VT_BSTR)
	{
		Dest[0]=0;
		return FALSE;
	}
	char szTem[68]={0};
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)(var.bstrVal),-1,NULL,0,NULL,FALSE);
	if(dwNum>=68)
	{
		Dest[0]=0;
		return FALSE;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,(LPCWSTR)(var.bstrVal),-1,szTem,dwNum,NULL,FALSE);
	strcpy_s(Dest,destLen,szTem);
	return TRUE;
}

CDataHelper::CDataHelper(void)
{
	::CoInitialize(NULL);
	ZeroMemory(m_tszConn, 512);
}

CDataHelper::~CDataHelper(void)
{
	ZeroMemory(m_tszConn, 512);
	ExitConnection();
	::CoUninitialize();
}

//获取注册表信息
void CDataHelper::GetRegConfig(LPCTSTR pKey, char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	char	sTemp[128] = {0};

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\AuthenCenter"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
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
bool CDataHelper::InitConnection(/*PTCHAR ptszServer, PTCHAR ptszDatabase, PTCHAR ptszUId, PTCHAR ptszPwd*/)
{
	////获取数据库连接字符串
	TCHAR tszServer[32] = {_T("192.168.30.138")};
	TCHAR tszDatabase[128] = {_T("BJCultureSys")};
	TCHAR tszUId[64] = {_T("sa")};
	TCHAR tszPwd[64] = {_T("sa")};
	//GetRegConfig(TEXT("Server"), szServer);
	//GetRegConfig(TEXT("Database"), szDatabase);
	//GetRegConfig(TEXT("UId"), szUId);
	//GetRegConfig(TEXT("Pwd"), szPwd);
	//if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	//{
	//	::OutputDebugString(_T("AuthenCenterService服务 从注册表获取数据库连接参数失败!\r\n"));
	//	return false;
	//}

	_stprintf_s(m_tszConn, 512, _T("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s"),
		tszServer, tszDatabase, tszUId, tszPwd
		/*ptszServer, ptszDatabase, ptszUId, ptszPwd*/);

	//创建并打开数据连接对象-----------------------------------------------------------------------------
	if(!m_pConnection.CreateConnection(m_tszConn))
	{
		::OutputDebugString(_T("CDataHelper::InitConnection create ADODB.Connection failure !\r\n"));
		return false;
	}
	//----------------------------------------------------------------------------------------------------

	//初始化成功
	return true;
}

//关闭连接
void CDataHelper::ExitConnection()
{
	m_pConnection.ReleaseConnection();
}


//打开到数据库的连接
bool CDataHelper::OpenConnection()
{
	//如果连接关闭则打开连接
	if(m_pConnection.CheckConnection())
	{
		return true;
	}

	if(!m_pConnection.CreateConnection(m_tszConn))
	{
		::OutputDebugString(_T("CDataHelper::OpenConnection create ADODB.Connection failure !\r\n"));
		return false;
	}
	return true;
}


//在连接对象上执行SQL语句
bool CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//如果连接关闭则打开连接
	if(!OpenConnection())
	{
		return false;
	}

	bool bRet = true;
	_CommandPtr spCmdPtr = NULL;

	//创建Command
	hr = spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 create ADODB.Command failure ! \n"));
		return false;
	}


	//为Command添加参数
	spCmdPtr->CommandType = adCmdText;
	spCmdPtr->CommandText = lpSql;

	//执行存储过程
	if(!m_pConnection.Execute(spCmdPtr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 ExecuteSql failure ! \n"));
		bRet=false;
	}
	spCmdPtr.Release();
	spCmdPtr = NULL;
	return bRet;
}


//读取存储过程
bool CDataHelper::GetRemoteControl()
{
	HRESULT hr;
	//如果连接关闭则打开连接
	if(!OpenConnection())
		return false;

	bool bRet = true;
	_CommandPtr spCmdPtr = NULL;

	//创建Command
	hr = spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 create ADODB.Command failure ! \n"));
		return false;
	}

	//char spSql[128] = {"EXEC GetRemoteControl"};
	////为Command添加参数
	//spCmdPtr->CommandType = adCmdText;
	//spCmdPtr->CommandText = lpSql;

	_RecordsetPtr spRecord = NULL;
	_variant_t vt;
	//执行存储过程
	if(m_pConnection.Execute(spRecord, _bstr_t(_T("EXEC GetRemoteControl")), &vt, adCmdText))
	{
		DWORD dwTem = 0;
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 ExecuteSql failure ! \n"));
		_variant_t vtRet = spRecord->GetCollect(_T("CommandID"));
		dwTem = vtRet.intVal;
		vtRet = spRecord->GetCollect(_T("CommandType"));
		dwTem = vtRet.intVal;
		
		//sCompanyID
		//var=spLogined->GetCollect("sCompanyID");
		//bRet=WideStringToCharString(var,item.szCompanyID,16);
		//if(!bRet){spLogined->MoveNext();continue;}

		////sComputerIP
		//var=spLogined->GetCollect("sComputerIP");
		//bRet=WideStringToCharString(var,item.szComputerIp,32);
		//if(!bRet){spLogined->MoveNext();continue;}
	}
	spCmdPtr.Release();
	spCmdPtr = NULL;
	return bRet;

}