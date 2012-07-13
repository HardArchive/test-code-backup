#pragma once
#include "MySql.h"

class CConnMySql
{
public:
	CConnMySql(void);
	~CConnMySql(void);

private:
	MySql m_objMysql;

public:
	int CheckUserPW(char* pszUserName, char* pszPW);
};
