#pragma once
#include "netmysql.h"
/******************************************************************************* 
* 1�� �ļ����ƣ� ConnMySql
* 2�� �桡������ Version *.*
* 3�� ��    ���� ���ݿ�ͨ��
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-2-9 15:03:43
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

class CConnMySql
{
public:
	CConnMySql(void);
	~CConnMySql(void);

public:
	BOOL ConnectMysql(char* host,char* name,char*pwd);
	BOOL CheckUser();            //����û������Ƿ����Լ��ļ�¼ ���û�������
private:
	
	
private:
	CNetMySql m_objNetMysql;

	BOOL m_bConnectFlag;              //MySql���ӱ�־
};
