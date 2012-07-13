// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ConnMySql.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TRACE(TEXT("TIPTOP-正在执行初始化…\r\n"));
	CConnMySql objConnMySql;
	//_asm int 3;
	if (!objConnMySql.ConnectMysql(g_szServerIP, g_szMysqlUserName, g_szMysqlPassword))
	{
		TRACE(TEXT("TIPTOP-连接数据库失败…\r\n"));
		return FALSE;
	}

	if (!objConnMySql.CheckUser())
	{
		TRACE(TEXT("TIPTOP-未查找到用户…\r\n"));
	}
	TRACE(TEXT("TIPTOP-查找到用户…\r\n"));
	return 0;
}

