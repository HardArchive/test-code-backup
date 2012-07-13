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
//关闭连接对象

bool CConnect::DisConnect()
{
	if (m_ptrCon != NULL)
	{
		mysql_close(m_ptrCon);
		m_ptrCon = NULL;
	}
	std::cout<<"断开数据库连接!!!"<<endl;
	return true;
}
//////////////////////////////////////////////////////////////////////////
//   连接数据库             

bool CConnect::Connect( string host, string user, string pwd, string dbname, WORD port )
{
	m_ptrCon = mysql_init(NULL);	//初始化数据结构
	if (NULL == m_ptrCon)
	{
		return false;
	}
	//做链接
	if (NULL == mysql_real_connect(m_ptrCon,host.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),port,NULL,0))
	{
		//m_sqlerr = mysql_error(m_ptrCon);
		strcpy(m_szSqlerr, mysql_error(m_ptrCon));
		return false;
	}

	////设置连接字符集，默认为gb2312
	//if(mysql_set_character_set(m_ptrCon, "gb2312"))
	//{
	//	//如果设置字符集不成功，则关闭连接
	//	//TRACE(TEXT("TIPTOP-mysql_set_character_set设置连接字符集失败，错误信息:%s\r\n"), mysql_error(&myData));
	//	DisConnect();
	//	return false;
	//}

	////设置允许多语句支持
	//mysql_set_server_option(m_ptrCon, MYSQL_OPTION_MULTI_STATEMENTS_ON);

	return true;
}
//////////////////////////////////////////////////////////////////////////
//  获取链接对象                                                                

void *CConnect::GetConObject()
{
	if (m_ptrCon ==NULL)
	{
		return NULL;
	}
	if (mysql_ping(m_ptrCon) != 0)	//自动连接
	{
		//m_sqlerr = mysql_error(m_ptrCon);
		strcpy(m_szSqlerr, mysql_error(m_ptrCon));

		TRACEA(m_szSqlerr);
		return NULL;
	}
	std::cout<<"取得数据库连接!!!"<<endl;
	return m_ptrCon;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/


CManagerPool *CManagerPool::m_PtrInstance = NULL;

//////////////////////////////////////////////////////////////////////////
//		连接池构造函数,
//		单件模式

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
//  初始化应该用程序连接池实例

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
// 把链接对象加入连接池


void CManagerPool::AddConObject(CConnect * conn)
{
	m_Mutex.Lock();
	m_PoolCon.push(conn);
	m_Mutex.Unlock();
	m_state = true;
}
//////////////////////////////////////////////////////////////////////////
// 从连接池获取链接对象                                                                  


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
	m_Mutex.Unlock();	//解锁 m_mutex
	
	if(conn)
	{
		return conn;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
//   释放连接

void CManagerPool::Release(CConnect * conn)
{
	m_Mutex.Lock(); 				//锁定 m_mutex
	m_PoolCon.push(conn);			//入队
	m_Mutex.Unlock();				//解锁 m_mutex
}

int CManagerPool::GetPoolSize()
{
	return m_PoolCon.size();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/

//结构体的构造函数
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
	m_PtrCon		=	NULL;						//指向  mysql 的连接指针
	m_PtrRes		=	NULL;						//指向  mysql 的查询数据集
	m_Row			=	0;							//操作影响的记录数
	ZeroMemory(m_szErr, sizeof(m_szErr));           //错误原因	
	m_SeqID			=	0;							//新的自增的序列号
	m_Commit		=	FALSE;						//事务提交模式
	m_State			=	FALSE;						//取得信息状态
	m_Fieldtype.clear();							//字段信息表
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
	m_Fieldtype.clear();							//字段信息表
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
//        得到查询记录数                                          

DWORD COperMysql::RowCount()
{
	return m_Row;
}						
//////////////////////////////////////////////////////////////////////////
//        得到新自增的序列号
		
DWORD COperMysql::GetIncreaseID()
{
	return m_SeqID;
}

//////////////////////////////////////////////////////////////////////////
//     获取状态

bool COperMysql::GetStatus()
{
	return m_State;
}


//////////////////////////////////////////////////////////////////////////
//	 获取错误码

char* COperMysql::What()
{
	return m_szErr;
}


//////////////////////////////////////////////////////////////////////////


bool COperMysql::Exec(string &ddl)
{
	
	Clear();								//清除缓冲
	
	if(0 == mysql_query(m_PtrCon,ddl.c_str()))
	{
		m_Row	= (WORD)mysql_affected_rows(m_PtrCon);			//得到受影响的行数
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
	if(m_Commit)	//提交事务状态
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

//执行数据操作语言(DML)类语句
bool COperMysql::Query(const std::string &dml)
{
	//清除缓冲
	Clear();

	if(mysql_query(m_PtrCon, dml.c_str()) != 0)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		//m_Err = mysql_error(m_PtrCon);
		return false;
	}

	m_PtrRes = mysql_store_result(m_PtrCon);

	//得到查询返回的行数
	m_Row = (WORD)mysql_affected_rows(m_PtrCon);

	//指向  mysql 的查询字段集
	MYSQL_FIELD *fieldptr = NULL;

	//取得各字段名和类型
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
		//读行的记录
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
	
	Clear();//清除缓冲
	
	if(mysql_query(m_PtrCon,dml.c_str()) != 0)
	{
		//m_Err = mysql_error(m_PtrCon);
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		TRACE(TEXT(" 操作失败........."));
		return false;
	}
	m_PtrRes = mysql_store_result(m_PtrCon);	//将查询的全部结果读取到客户端		
												//如果查询未返回结果集 将返回Null指针
												//如果读取结果集失败
	if (m_PtrCon == NULL)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, mysql_error(m_PtrCon));
		//m_Err = mysql_error(m_PtrCon);
		TRACEA(m_szErr);
	}

	m_Row = (WORD)mysql_affected_rows(m_PtrCon);	//得到查询返回的行数
	MYSQL_FIELD *fieldptr = NULL;			//指向  mysql 的查询字段集

	while(fieldptr = mysql_fetch_field(m_PtrRes))//取得各字段名和类型
	{												//从结果集中取得列信息并作为对象返回
		typeset_t typeset;
		typeset.m_index		= m_Fieldtype.size();
		typeset.m_length	= (WORD)fieldptr->length;
		typeset.m_name		= fieldptr->name;
		typeset.m_type		= SetFieldType(fieldptr->type);//
		m_Fieldtype.push_back(typeset);
	}
	
	MYSQL_ROW currrow = NULL;

	while((currrow = mysql_fetch_row(m_PtrRes)))	//读行的记录
	{	
		const unsigned int colcount = mysql_num_fields(m_PtrRes);//取得返回字段的数目
		row_t rows(colcount);
		for(unsigned int i = 0; i < colcount; ++i)
		{
			rows[i] = currrow[i] ? currrow[i] : "NULL";
		}
		m_Recordset.push_back(rows);
	}
	mysql_free_result(m_PtrRes);//函数释放结果内存
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
//			设置连接对象

bool COperMysql::SetTransAction(CConnect * conn)
{
	if (conn == NULL)
	{
		TRACE(TEXT("COperMysql::SetTransAction  conn 对象为空"));
		return false;
	}
	m_PtrCon = (MYSQL *)conn->GetConObject();

	if ( NULL == m_PtrCon )
	{
		TRACE(TEXT("conn->GetConObject() 对象为空"));
		return false;
	}
	mysql_autocommit(m_PtrCon,m_Commit);  //尝试连接
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                  找到对应的列序号                        
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
		strcpy(m_szErr, "列索引已经到达上限");
		//m_Err = "列索引已经到达上限";
		
		m_State = false;//得到当前执行状态
		return "";
	}	

	row_t * rowvalue = FindRow(row);
	
	if(rowvalue ==  NULL)
	{
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "行索引已经到达上限");
		//m_Err = "行索引已经到达上限";
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
		//m_Err = "列索引已经到达上限";
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "列索引已经到达上限");
		m_State = false;
		return "";
	}
	
	row_t * rowvalue = FindRow(row);
	if(rowvalue ==  NULL)
	{
		//m_Err = "行索引已经到达上限";
		ZeroMemory(m_szErr, sizeof(m_szErr));
		strcpy(m_szErr, "行索引已经到达上限");
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
//得到指定行某个字段的整数值

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
//自动释放数据库连接池的类
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
