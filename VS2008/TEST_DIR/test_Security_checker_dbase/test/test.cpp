// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ConnMySql.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE(TEXT("TIPTOP-����ִ�г�ʼ����\r\n"));
	CConnMySql objConnMySql;
	//_asm int 3;
	if (!objConnMySql.ConnectMysql(g_szServerIP, g_szMysqlUserName, g_szMysqlPassword))
	{
		TRACE(TEXT("TIPTOP-�������ݿ�ʧ�ܡ�\r\n"));
		return FALSE;
	}

	if (!objConnMySql.CheckUser())
	{
		TRACE(TEXT("TIPTOP-δ���ҵ��û���\r\n"));
	}
	TRACE(TEXT("TIPTOP-���ҵ��û���\r\n"));
	return 0;
}

