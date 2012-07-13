// Connect.cpp: implementation of the CConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "mysql.h"
#include <mysql.h>
#include "Connect.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/************************************************************************/
/*                                                                      */
/************************************************************************/

// 
CConnect::CConnect()
{
	m_ptrCon = NULL;
}
//////////////////////////////////////////////////////////////////////////
//

CConnect::~CConnect()
{
	if (m_ptrCon !=NULL)
	{
		DisConnect();
	}
}
//////////////////////////////////////////////////////////////////////////
//�ر����Ӷ���

bool CConnect::DisConnect()
{
	if (m_ptrCon != NULL)
	{
		mysql_close(m_ptrCon);
		m_ptrCon = NULL;
	}
	std::cout<<"�Ͽ����ݿ�����!!!"<<endl;
	return true;
}
//////////////////////////////////////////////////////////////////////////
//   �������ݿ�             

bool CConnect::Connect( string host, string user, string pwd, string dbname, WORD port )
{
	m_ptrCon = mysql_init(NULL);	//��ʼ�����ݽṹ
	if (NULL == m_ptrCon)
	{
		return false;
	}
	//������
	if (NULL == mysql_real_connect(m_ptrCon,host.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),port,NULL,0))
	{
		//m_sqlerr = mysql_error(m_ptrCon);
		strcpy(m_szSqlerr, mysql_error(m_ptrCon));
		return false;
	}

	////���������ַ�����Ĭ��Ϊgb2312
	//if(mysql_set_character_set(m_ptrCon, "gb2312"))
	//{
	//	//��������ַ������ɹ�����ر�����
	//	//TRACE(TEXT("TIPTOP-mysql_set_character_set���������ַ���ʧ�ܣ�������Ϣ:%s\r\n"), mysql_error(&myData));
	//	DisConnect();
	//	return false;
	//}

	////������������֧��
	//mysql_set_server_option(m_ptrCon, MYSQL_OPTION_MULTI_STATEMENTS_ON);

	return true;
}
//////////////////////////////////////////////////////////////////////////
//  ��ȡ���Ӷ���                                                                

void *CConnect::GetConObject()
{
	if (m_ptrCon ==NULL)
	{
		return NULL;
	}
	if (mysql_ping(m_ptrCon) != 0)	//�Զ�����
	{
		//m_sqlerr = mysql_error(m_ptrCon);
		strcpy(m_szSqlerr, mysql_error(m_ptrCon));

		TRACEA(m_szSqlerr);
		return NULL;
	}
	std::cout<<"ȡ�����ݿ�����!!!"<<endl;
	return m_ptrCon;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/


CManagerPool *CManagerPool::m_PtrInstance = NULL;

//////////////////////////////////////////////////////////////////////////
//		���ӳع��캯��,
//		����ģʽ

CManagerPool::CManagerPool()
{
	m_state = false;
}
//////////////////////////////////////////////////////////////////////////

CManagerPool::~CManagerPool()
{
	m_state = false;
	delete m_PtrInstance;
	m_PtrInstance = NULL;
}
//////////////////////////////////////////////////////////////////////////
//  ��ʼ��Ӧ���ó������ӳ�ʵ��

CManagerPool &CManagerPool::GetInitInstance()
{
	CCriticalSection  Mutex;
	if(m_PtrInstance == NULL)	
	{
		//mutex.Lock();
		Mutex.Lock(); 
		m_PtrInstance = new CManagerPool();	
		Mutex.Unlock(); 
	}
	return *m_PtrInstance;
}
//////////////////////////////////////////////////////////////////////////
// �����Ӷ���������ӳ�


void CManagerPool::AddConObject(CConnect * conn)
{
	m_Mutex.Lock();
	m_PoolCon.push(conn);
	m_Mutex.Unlock();
	m_state = true;
}
//////////////////////////////////////////////////////////////////////////
// �����ӳػ�ȡ���Ӷ���                                                                  


CConnect * CManagerPool::GetConObject()
{
	if(!m_state || m_PoolCon.size() == 0)
	{
		return NULL;
	}

	CConnect * conn = NULL;

	m_Mutex.Lock();	
	conn = m_PoolCon.front();
	m_PoolCon.pop();
	m_Mutex.Unlock();	//���� m_mutex
	
	if(conn)
	{
		return conn;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
//   �ͷ�����

void CManagerPool::Release(CConnect * conn)
{
	m_Mutex.Lock(); 				//���� m_mutex
	m_PoolCon.push(conn);			//���
	m_Mutex.Unlock();				//���� m_mutex
}

int CManagerPool::GetPoolSize()
{
	return m_PoolCon.size();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

//�ṹ��Ĺ��캯��
typeset_t::typeset_t()
{
	m_name		=	"";
	m_type		=	(filetype_t)6;
	m_length	=	0;
	m_index		=	0;
}
//////////////////////////////////////////////////////////////////////////
//



COperMysql::COperMysql()
{
	m_PtrCon		=	NULL;						//ָ��  mysql ������ָ��
	m_PtrRes		=	NULL;						//ָ��  mysql �Ĳ�ѯ���ݼ�
	m_Row			=	0;							//����Ӱ��ļ�¼��
	ZeroMemory(m_szErr, sizeof(m_szErr));           //����ԭ��	
	m_SeqID			=	0;							//�µ����������к�
	m_Commit		=	FALSE;						//�����ύģʽ
	m_State			=	FALSE;						//ȡ����Ϣ״̬
	m_Fieldtype.clear();							//�ֶ���Ϣ��
	m_Recordset.clear();

}
//////////////////////////////////////////////////////////////////////////

COperMysql::~COperMysql()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////


void COperMysql::Clear()
{
	m_Fieldtype.clear();							//�ֶ���Ϣ��
	m_Recordset.clear();
	SetIncreaseID(0);
}
//////////////////////////////////////////////////////////////////////////


void COperMysql::SetIncreaseID(long id)
{
	m_SeqID = id;
}
//////////////////////////////////////////////////////////////////////////


row_t * COperMysql::FindRow(DWORD findrow)
{
	if (findrow >= this->RowCount() || m_Recordset.size() == 0)
	{
		return NULL;
	}
	return &m_Recordset[findrow];
}	
//////////////////////////////////////////////////////////////////////////
//        �õ���ѯ��¼��                                          

DWORD COperMysql::RowCount()
{
	return m_Row;
}						
//////////////////////////////////////////////////////////////////////////
//        �õ������������к�
		
DWORD COperMysql::GetIncreaseID()
{
	return m_SeqID;
}

//////////////////////////////////////////////////////////////////////////
//     ��ȡ״̬

bool COperMysql::GetStatus()
{
	return m_State;
}


//////////////////////////////////////////////////////////////////////////
//	 ��ȡ������

char* COperMysql::What()
{
	return m_szErr;
}


//////////////////////////////////////////////////////////////////////////


bool COperMysql::Exec(string &ddl)
{
	
	Clear();								//�������
	
	if(0 == mysql_query(m_PtrCon,ddl.c_str()))
	{
		m_Row	= (WORD)mysql_affected_rows(m_PtrCon);			//�õ���Ӱ�������
		m_SeqID = (WORD)mysql_insert_id(m_PtrCon);
	}
	else
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		//m_Err = mysql_error(m_PtrCon);

		TRACEA(m_szErr);
		return FALSE;
	}
	return TRUE;
}	
//////////////////////////////////////////////////////////////////////////


bool COperMysql::RollBack()
{
	if(m_Commit)	//�ύ����״̬
	{
		return true;
	}
	if(mysql_rollback(m_PtrCon) == 0)
	{
		return true;
	}	
	return false;
}
//////////////////////////////////////////////////////////////////////////

//ִ�����ݲ�������(DML)�����
bool COperMysql::Query(const std::string &dml)
{
	//�������
	Clear();

	if(mysql_query(m_PtrCon, dml.c_str()) != 0)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		//m_Err = mysql_error(m_PtrCon);
		return false;
	}

	m_PtrRes = mysql_store_result(m_PtrCon);

	//�õ���ѯ���ص�����
	m_Row = (WORD)mysql_affected_rows(m_PtrCon);

	//ָ��  mysql �Ĳ�ѯ�ֶμ�
	MYSQL_FIELD *fieldptr = NULL;

	//ȡ�ø��ֶ���������
	while(fieldptr = mysql_fetch_field(m_PtrRes))
	{
		typeset_t typeset;
		typeset.m_index = m_Fieldtype.size();
		typeset.m_length = (WORD)fieldptr->length;
		typeset.m_name = fieldptr->name;
		typeset.m_type = SetFieldType(fieldptr->type);
		m_Fieldtype.push_back(typeset);
	}

	MYSQL_ROW currrow = NULL;
	while((currrow = mysql_fetch_row(m_PtrRes)))
	{
		//���еļ�¼
		const unsigned int colcount = mysql_num_fields(m_PtrRes);
		row_t rows(colcount);
		for(unsigned int i = 0; i < colcount; ++i)
		{
			rows[i] = currrow[i] ? currrow[i] : "NULL";
		}
		m_Recordset.push_back(rows);
	}
	mysql_free_result(m_PtrRes); 
	m_PtrRes = NULL;

	return true;
}
//////////////////////////////////////////////////////////////////////////


bool COperMysql::Select(string &dml)
{
	
	Clear();//�������
	
	if(mysql_query(m_PtrCon,dml.c_str()) != 0)
	{
		//m_Err = mysql_error(m_PtrCon);
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		TRACE(TEXT(" ����ʧ��........."));
		return false;
	}
	m_PtrRes = mysql_store_result(m_PtrCon);	//����ѯ��ȫ�������ȡ���ͻ���		
												//�����ѯδ���ؽ���� ������Nullָ��
												//�����ȡ�����ʧ��
	if (m_PtrCon == NULL)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		//m_Err = mysql_error(m_PtrCon);
		TRACEA(m_szErr);
	}

	m_Row = (WORD)mysql_affected_rows(m_PtrCon);	//�õ���ѯ���ص�����
	MYSQL_FIELD *fieldptr = NULL;			//ָ��  mysql �Ĳ�ѯ�ֶμ�

	while(fieldptr = mysql_fetch_field(m_PtrRes))//ȡ�ø��ֶ���������
	{												//�ӽ������ȡ������Ϣ����Ϊ���󷵻�
		typeset_t typeset;
		typeset.m_index		= m_Fieldtype.size();
		typeset.m_length	= (WORD)fieldptr->length;
		typeset.m_name		= fieldptr->name;
		typeset.m_type		= SetFieldType(fieldptr->type);//
		m_Fieldtype.push_back(typeset);
	}
	
	MYSQL_ROW currrow = NULL;

	while((currrow = mysql_fetch_row(m_PtrRes)))	//���еļ�¼
	{	
		const unsigned int colcount = mysql_num_fields(m_PtrRes);//ȡ�÷����ֶε���Ŀ
		row_t rows(colcount);
		for(unsigned int i = 0; i < colcount; ++i)
		{
			rows[i] = currrow[i] ? currrow[i] : "NULL";
		}
		m_Recordset.push_back(rows);
	}
	mysql_free_result(m_PtrRes);//�����ͷŽ���ڴ�
	m_PtrRes = NULL;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////


bool COperMysql::Commit()
{
	if(m_Commit)
	{
		return TRUE;
	}
	
	if(mysql_commit(m_PtrCon) == 0)
	{
		return TRUE;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////


filetype_t COperMysql::SetFieldType(enum_field_types fieldtype)
{
	filetype_t type;
	switch(fieldtype)
	{
	case MYSQL_TYPE_STRING:
		//
	case MYSQL_TYPE_VAR_STRING:
		//
		//	case MYSQL_TYPE_TEXT:
		//
	case MYSQL_TYPE_BLOB:
		//
	case MYSQL_TYPE_SET:
		//
	case MYSQL_TYPE_GEOMETRY:
		//
	case MYSQL_TYPE_NULL:
		type = ENUM_F_T_CHAR;
		break;
	case MYSQL_TYPE_TINY:
		//
	case MYSQL_TYPE_SHORT:
		//
	case MYSQL_TYPE_LONG:
		//
	case MYSQL_TYPE_INT24:
		//
// 	case MYSQL_TYPE_BIT:
		//
	case MYSQL_TYPE_ENUM:
		//
	case MYSQL_TYPE_YEAR:
		//
	case MYSQL_TYPE_LONGLONG:
		type = ENUM_F_T_INT;
		break;
	case MYSQL_TYPE_DECIMAL:
		//
// 	case MYSQL_TYPE_NEWDECIMAL:
		type = ENUM_F_T_DEC;
		break;
	case MYSQL_TYPE_FLOAT:
		//
	case MYSQL_TYPE_DOUBLE:
		type = ENUM_F_T_DOUBLE;
		break;
	case MYSQL_TYPE_TIMESTAMP:
		//
	case MYSQL_TYPE_DATE:
		//
	case MYSQL_TYPE_TIME:
		type = ENUM_F_T_DATETIME;
		break;
	default:
		type = ENUM_F_T_UNKNOWN;
		break;
	}
	return type;


}
//////////////////////////////////////////////////////////////////////////
//			�������Ӷ���

bool COperMysql::SetTransAction(CConnect * conn)
{
	if (conn == NULL)
	{
		TRACE(TEXT("COperMysql::SetTransAction  conn ����Ϊ��"));
		return false;
	}
	m_PtrCon = (MYSQL *)conn->GetConObject();

	if ( NULL == m_PtrCon )
	{
		TRACE(TEXT("conn->GetConObject() ����Ϊ��"));
		return false;
	}
	mysql_autocommit(m_PtrCon,m_Commit);  //��������
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                  �ҵ���Ӧ�������                        
WORD COperMysql::GetFieldIndex(string &fieldname)
{
	DWORD index = 0;

	for(int i = 0; i < (int)m_Fieldtype.size(); ++i)
	{
		if(0 == strcmp(m_Fieldtype[i].m_name.c_str(),fieldname.c_str()))
		{
			index = m_Fieldtype[i].m_index;
			break;
		}
	}
	return (WORD)index;
}
//////////////////////////////////////////////////////////////////////////
//

string COperMysql::GetItemValue(DWORD row,WORD index)
{
	if(index >= m_Fieldtype.size())
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "�������Ѿ���������");
		//m_Err = "�������Ѿ���������";
		
		m_State = false;//�õ���ǰִ��״̬
		return "";
	}	

	row_t * rowvalue = FindRow(row);
	
	if(rowvalue ==  NULL)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "�������Ѿ���������");
		//m_Err = "�������Ѿ���������";
		m_State = false;
		return "";
	}
	
	return (*rowvalue)[index];

}
//////////////////////////////////////////////////////////////////////////


string COperMysql::GetItemValue(DWORD row,string &fieldname)
{
	DWORD index = 0;
	if((index = GetFieldIndex(fieldname)) >= 10000)
	{
		//m_Err = "�������Ѿ���������";
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "�������Ѿ���������");
		m_State = false;
		return "";
	}
	
	row_t * rowvalue = FindRow(row);
	if(rowvalue ==  NULL)
	{
		//m_Err = "�������Ѿ���������";
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "�������Ѿ���������");
		m_State = false;
		return "";
	}
	
	return (*rowvalue)[index];
}

//////////////////////////////////////////////////////////////////////////


string COperMysql::GetItemString(DWORD row,WORD index)
{

	return GetItemValue(row,index);

}
//////////////////////////////////////////////////////////////////////////

string COperMysql::GetItemString(DWORD row,string &fieldname)
{
	return GetItemValue(row,fieldname);
}

//////////////////////////////////////////////////////////////////////////



float COperMysql::GetItemFloat(DWORD row,WORD index)
{
	return (float)atof(GetItemValue(row,index).c_str());	
}
//////////////////////////////////////////////////////////////////////////


float COperMysql::GetItemFloat(DWORD row,string &fieldname)
{
	return (float)atof(GetItemValue(row,fieldname).c_str());
}
//////////////////////////////////////////////////////////////////////////
//�õ�ָ����ĳ���ֶε�����ֵ

long COperMysql::GetItemLong(DWORD row,WORD index)			
{
	return atol(GetItemValue(row,index).c_str());
}


//////////////////////////////////////////////////////////////////////////


long COperMysql::GetItemLong(DWORD row,string &fieldname)
{
	return atol(GetItemValue(row,fieldname).c_str());
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//�Զ��ͷ����ݿ����ӳص���
CSqlClient::CSqlClient()
{
	m_PtrCon = NULL;//m_PtrCon
}

CSqlClient::~CSqlClient()
{
	if(m_PtrCon)
	{
		CManagerPool::GetInitInstance().Release(m_PtrCon);
	}
}

CConnect * CSqlClient::GetConnect()
{
	if(m_PtrCon == NULL)
	{
		m_PtrCon = CManagerPool::GetInitInstance().GetConObject();
	}
	
	if(m_PtrCon == NULL)
	{
		return m_PtrCon;
	}
	
	return m_PtrCon;
}
