#pragma once

#include "ADOConnection.h"
#include <vector>
using namespace std;

class CDataHelper
{
public:
	CDataHelper(void);
	~CDataHelper(void);

public:
	bool InitConnection();          //初始化连接 数据库	
	bool OpenConnection();          //先检查连接状态如果，正常则返回true， 否则重连	
	void ExitConnection();          //断开连接 释放资源

public:
	void GetRegConfig(LPCTSTR pKey,char *pValue);
	bool ExecuteSql(LPCTSTR lpSql);

public:
	bool GetRemoteControl();

private:
	CADOConnection m_pConnection;
	TCHAR m_tszConn[512];
};
