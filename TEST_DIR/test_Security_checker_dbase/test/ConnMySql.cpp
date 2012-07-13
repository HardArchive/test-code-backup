#include "StdAfx.h"
#include "ConnMySql.h"

CConnMySql::CConnMySql(void)
{
	m_bConnectFlag = false;
}

CConnMySql::~CConnMySql(void)
{
	if (m_bConnectFlag)
	{
		m_objNetMysql.Close();
	}
}

//连接数据库  char* host, char* name, char* pwd 服务器IP，用户名密码
BOOL CConnMySql::ConnectMysql(char* host, char* name, char* pwd)
{
	m_bConnectFlag = m_objNetMysql.Connect(host, name, pwd, "securitycheckerdb", 3306);
	CheckUser();
	return m_bConnectFlag;	
}

BOOL CConnMySql::CheckUser()
{
	DWORD dwSize = MAX_PATH;
	TCHAR tszMac[32] = {0};
	TCHAR tszIP[32] = {0};
	TCHAR tszUserName[MAX_PATH] = {0};
	TCHAR tszCoumputerName[MAX_PATH] = {0};
	char szSql[MAX_PATH] = {0};
	char* pTemp = NULL;
    unsigned uRow = 0;               //行数
	if (!m_bConnectFlag)
	{
		return FALSE;
	}
	if (!GetNetInfo(tszMac, tszIP)&&!GetUser(tszUserName)&&GetComputerName(tszCoumputerName, &dwSize))
	{
		return FALSE;
	}
	//_asm int 3;
	USES_CONVERSION;
	char szTemp[64] = {0};
	//strcpy_s(tszMac, "50-e5-49-5b-d4-f9");
	strcpy_s(szTemp, tszMac);
	sprintf(szSql, "SELECT * FROM securitycheckerdb.user WHERE mid='%s'", szTemp);
//_asm int 3;ExecuteQuery
	//toupper();
	
	uRow = m_objNetMysql.ExecuteQuery(szSql);  //uRow是行数 但实际下标是从零开始的，所以减1
	UINT uiLen = 0;
	pTemp = m_objNetMysql.GetStringValue(0, "ipaddress", &uiLen);

	if (!uRow)
	{
		return FALSE;
	}

	return TRUE;
}