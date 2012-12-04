#pragma once
#include "ADOConnection.h"

class CDataHelper
{
public:
	CDataHelper(void);
	~CDataHelper(void);

public:
	BOOL InitConnection();
	void ExitConnection();
	void GetRegConfig(LPCTSTR pKey,char *pValue);
	BOOL ExecuteSql(LPCTSTR lpSql);
	BOOL LoadLoginedItem(vector<LoginItem> &loginedItem);

private:
	CADOConnection m_pConnection;
	char szConn[512];
};
