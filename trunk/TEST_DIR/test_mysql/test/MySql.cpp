#include "StdAfx.h"
#include "MySql.h"

//һ���ֶνṹ
typedef struct _FIELDS
{
	char szFieldName[DEFAULT_FIELD_SIZE]; //�ֶ���
	void * pFiledValue;                   //�ֶ�ֵ��ָ��
	unsigned uValueLen;                   //�ֶεĳ��ȣ�ֻ�������ַ�������
}FIELDS,*PFIELDS;

//һ�����ݵĽṹ����
typedef struct _ROWS{
	FIELDS stuFields[DEFAULT_FIELD];     //�ֶ�,���ݲ�ͬ��Ӧ�ó�������޸Ĵ�ֵ
	int iFieldNum;                        //���ٸ��ֶΣ�ָʾfield��Ԫ�ص���Ч����
}ROWS,*PROWS;

CMySql::CMySql(void)
{
	m_pstuResult=NULL;
	memset(m_szhostIP, 0, 32);
	memset(m_szUserName, 0, 32);
	memset(m_szpwd, 0, 32);
	m_uPort = 0;

	//����mysql_library_init()��mysql_library_end()��Ŀ�����ڣ�ΪMySQL���ṩǡ���ĳ�ʼ���ͽ�������
	mysql_library_init(NULL,NULL,NULL);

	mysql_init(&m_stuMySql);

	//�������ӳ�ʱ
	unsigned timeout=5*60;
	mysql_options(&m_stuMySql, MYSQL_OPT_CONNECT_TIMEOUT, (char*)&timeout);

	//����������
	my_bool reconnect=TRUE;
	mysql_options(&m_stuMySql, MYSQL_OPT_RECONNECT, (char*)&reconnect);

	//��ʼ��HashTable��СΪ100
	//m_objRes.InitHashTable(100);
}

CMySql::~CMySql(void)
{
	Close();
}

//�ر�����
void CMySql::Close()
{	
	mysql_close(&m_stuMySql);
}

void CMySql::FreeResult()
{
	//int iLen = 0;
	//PROWS pstuRows = NULL;

	//iLen = m_pstuResult->GetCount();
	////�ͷŽ�������ڴ�
	//for(int i=0;i<iLen;i++)
	//{
	//	pstuRows = (PROWS)m_pstuResult[i];
	//	if(pstuRows != NULL)
	//	{
	//		free(pstuRows);
	//		pstuRows = NULL;
	//	}
	//}

	//�ͷ�result
	if(m_pstuResult!=NULL)
	{
		mysql_free_result(m_pstuResult);
	}
	//m_pstuResult = NULL;
}

//����MySql������
BOOL CMySql::Connect(char* pszHostIp, char* pszUserName, char* pszpwd, char* pszDbName, unsigned uPort)
{
	strcpy(m_szhostIP, pszHostIp);
	strcpy(m_szUserName, pszUserName);
	strcpy(m_szpwd, pszpwd);
	m_uPort = uPort;

	//���ӷ�����
	if(mysql_real_connect(&m_stuMySql, m_szhostIP, m_szUserName, m_szpwd, pszDbName, m_uPort, NULL, 0)==NULL)
	{
		return FALSE;
	}

	//���������ַ�����Ĭ��Ϊgb2312
	if(mysql_set_character_set(&m_stuMySql, "gb2312"))
	{
		//��������ַ������ɹ�����ر�����
		Close();
		return FALSE;
	}
	//������������֧��
	mysql_set_server_option(&m_stuMySql, MYSQL_OPTION_MULTI_STATEMENTS_ON);

	return TRUE;

}

void CMySql::QueryItem(char* pszTableName, char* pInbuffer)
{
	char szSqlText[MAX_PATH]= {0}; 
	if (strlen(pInbuffer))
		sprintf(szSqlText,"select * from %s where %s", pszTableName, pInbuffer);
	else
		sprintf(szSqlText,"select * from %s", pszTableName);

	if(!mysql_query(&m_stuMySql, szSqlText))
	{
		m_pstuResult = mysql_store_result(&m_stuMySql);
		DealReasult();
	}
}