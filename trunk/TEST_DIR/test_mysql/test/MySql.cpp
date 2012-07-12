#include "StdAfx.h"
#include "MySql.h"

//一个字段结构
typedef struct _FIELDS
{
	char szFieldName[DEFAULT_FIELD_SIZE]; //字段名
	void * pFiledValue;                   //字段值的指针
	unsigned uValueLen;                   //字段的长度，只适用于字符串类型
}FIELDS,*PFIELDS;

//一行数据的结构数组
typedef struct _ROWS{
	FIELDS stuFields[DEFAULT_FIELD];     //字段,根据不同的应用程序可以修改此值
	int iFieldNum;                        //多少个字段，指示field中元素的有效个数
}ROWS,*PROWS;

CMySql::CMySql(void)
{
	m_pstuResult=NULL;
	memset(m_szhostIP, 0, 32);
	memset(m_szUserName, 0, 32);
	memset(m_szpwd, 0, 32);
	m_uPort = 0;

	//调用mysql_library_init()和mysql_library_end()的目的在于，为MySQL库提供恰当的初始化和结束处理。
	mysql_library_init(NULL,NULL,NULL);

	mysql_init(&m_stuMySql);

	//设置连接超时
	unsigned timeout=5*60;
	mysql_options(&m_stuMySql, MYSQL_OPT_CONNECT_TIMEOUT, (char*)&timeout);

	//启动再连接
	my_bool reconnect=TRUE;
	mysql_options(&m_stuMySql, MYSQL_OPT_RECONNECT, (char*)&reconnect);

	//初始化HashTable大小为100
	//m_objRes.InitHashTable(100);
}

CMySql::~CMySql(void)
{
	Close();
}

//关闭连接
void CMySql::Close()
{	
	mysql_close(&m_stuMySql);
}

void CMySql::FreeResult()
{
	//int iLen = 0;
	//PROWS pstuRows = NULL;

	//iLen = m_pstuResult->GetCount();
	////释放结果集的内存
	//for(int i=0;i<iLen;i++)
	//{
	//	pstuRows = (PROWS)m_pstuResult[i];
	//	if(pstuRows != NULL)
	//	{
	//		free(pstuRows);
	//		pstuRows = NULL;
	//	}
	//}

	//释放result
	if(m_pstuResult!=NULL)
	{
		mysql_free_result(m_pstuResult);
	}
	//m_pstuResult = NULL;
}

//连接MySql服务器
BOOL CMySql::Connect(char* pszHostIp, char* pszUserName, char* pszpwd, char* pszDbName, unsigned uPort)
{
	strcpy(m_szhostIP, pszHostIp);
	strcpy(m_szUserName, pszUserName);
	strcpy(m_szpwd, pszpwd);
	m_uPort = uPort;

	//连接服务器
	if(mysql_real_connect(&m_stuMySql, m_szhostIP, m_szUserName, m_szpwd, pszDbName, m_uPort, NULL, 0)==NULL)
	{
		return FALSE;
	}

	//设置连接字符集，默认为gb2312
	if(mysql_set_character_set(&m_stuMySql, "gb2312"))
	{
		//如果设置字符集不成功，则关闭连接
		Close();
		return FALSE;
	}
	//设置允许多语句支持
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