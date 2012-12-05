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
	bool InitConnection();          //��ʼ������ ���ݿ�	
	bool OpenConnection();          //�ȼ������״̬����������򷵻�true�� ��������	
	void ExitConnection();          //�Ͽ����� �ͷ���Դ

public:
	void GetRegConfig(LPCTSTR pKey,char *pValue);
	bool ExecuteSql(LPCTSTR lpSql);

public:
	bool GetRemoteControl();

private:
	CADOConnection m_pConnection;
	TCHAR m_tszConn[512];
};
