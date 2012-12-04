#include "StdAfx.h"
#include "DataHelper.h"

CDataHelper::CDataHelper(void)
{
	ZeroMemory(szConn, 512);
}

CDataHelper::~CDataHelper(void)
{
}

//初始化连接
BOOL CDataHelper::InitConnection()
{
	//获取数据库连接字符串
	char szServer[32] = {0};
	char szDatabase[128] = {0};
	char szUId[64] = {0};
	char szPwd[64] = {0};
	GetRegConfig(TEXT("Server"), szServer);
	GetRegConfig(TEXT("Database"), szDatabase);
	GetRegConfig(TEXT("UId"), szUId);
	GetRegConfig(TEXT("Pwd"), szPwd);
	if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	{
		::OutputDebugString(_T("AuthenCenterService服务 从注册表获取数据库连接参数失败!\r\n"));
		return FALSE;
	}
	//获取端口号
	char szTCPPort[16] = {0};
	char szUDPPort[16] = {0};
	GetRegConfig(TEXT("TcpPort"), szTCPPort);
	GetRegConfig(TEXT("UdpPort"), szUDPPort);

	if(strlen(szTCPPort)==0 || strlen(szUDPPort)==0)
	{
		::OutputDebugString(_T("AuthenCenterService服务 从注册表获取端口号参数失败!\r\n"));
		return FALSE;
	}
	SEVER_PORT = atoi(szTCPPort);
	HEART_PORT = atoi(szUDPPort);


	sprintf_s(szConn,512,"driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",szServer,szDatabase,szUId,szPwd);

	//创建并打开数据连接对象-----------------------------------------------------------------------------
	if(!m_pConnection.CreateConnection(szConn))
	{
		::OutputDebugString(_T("AuthenCenterService服务create ADODB.Connection failure !\r\n"));
		return FALSE;
	}
	//----------------------------------------------------------------------------------------------------

	//初始化成功
	return TRUE;
}

//关闭连接
void CDataHelper::ExitConnection()
{
	m_pConnection.ReleaseConnection();
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

	BOOL bRet=TRUE;
	_CommandPtr spCmdPtr=NULL;

	//创建Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 create ADODB.Command failure ! \n"));
		return FALSE;
	}

	//为Command添加参数
	spCmdPtr->CommandType=adCmdText;
	spCmdPtr->CommandText=lpSql;

	//执行存储过程
	if(!m_pConnection.Execute(spCmdPtr))
	{
		TracePrint(LOG_ERROR,TEXT("AuthenCenterService服务 ExecuteSql failure ! \n"));
		bRet=FALSE;
	}
	spCmdPtr.Release();
	spCmdPtr=NULL;
	return bRet;
}