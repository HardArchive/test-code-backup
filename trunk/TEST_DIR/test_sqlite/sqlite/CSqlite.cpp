//////////////////////////////////////////////////////////////////////////////////////////////////////
 //
 // FileName:    CSqlite.cpp
 // Description: 封装SQLite 的使用类, 当前使用的SQLite 版本是3.4.0
 // Function:    运行SQLite 的继承类
 // CreateTime:	 2009.02.02
 // Author:      Zhang Pengfei
 // History:
 //
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"
#include "CSqlite.h"
#include "AU5800A Permission Configurator.h"
#include "AU5800A Permission ConfiguratorView.h"

//sqlite3的回调函数         
int LoadResult( void * para, int n_column, char ** column_value, char ** column_name );

static const int BUSYTIMEOUTMS  = 1600;		//默认超时毫秒数
static const int SLEEPTIME      = 6400;		//默认休息暂停时间
static const int LIMITNUM       = 16;		//当SQLite忙时，执行SQL语句的最大尝试次数

static const char * STRSYNCHRONOUS_ON  = "PRAGMA SYNCHRONOUS = ON";
static const char * STRSYNCHRONOUS_OFF = "PRAGMA SYNCHRONOUS = OFF";

//static const char * STRPAGESIZE  = "PRAGMA PAGE_SIZE = 8192";
static const char * STRCACHESIZE = "PRAGMA CACHE_SIZE = 8000";
//static const char * STRDEFAULTCACHESIZE = "PRAGMA DEFAULT_CACHE_SIZE = 8000";

const char * STRBEGINTRANS  = "BEGIN TRANSACTION";
const char * STRCOMMITTRANS = "COMMIT TRANSACTION";
const char * STRROLLBACK    = "ROLLBACK TRANSACTION";

/////////////////////////////////// CSQLite 类/////////////////////////////////////////
// standard constructor and destructor
// 构造与析构函数 //
CSQLite::CSQLite( void )
{
	m_pDb = NULL;
	m_pStmt = NULL;
	m_nState = -1;
	m_nTimeoutMs = BUSYTIMEOUTMS;
	m_nCurFieldNum = 0;
	m_nLimitNum = LIMITNUM;
	m_bIsOpen = false;
}

CSQLite::~CSQLite( void )
{
    if (NULL != m_pStmt)
    {
        sqlite3_finalize(m_pStmt);
        m_pStmt = NULL;
    }
    Close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Open DB
// Input:	 inDbName:file name of the database
//             (也就是保存数据库文件的绝对路径或者当前路径)
//inSynFlag:  是否打开同步标志
// Output:
// Return:	 true,Success; false,Failed
bool CSQLite::Open( const char * inDbName , bool inSynFlag )
{
    //check the name of the database, make sure it is not null
    if ( NULL == inDbName )
    {
        m_strErrInfo = "数据库文件名为空";
        return false;
    }

    //open the database according the specefied file name
    m_strFileName = inDbName;
    m_nState = sqlite3_open(inDbName, &m_pDb);
    if ( SQLITE_OK != m_nState )
    {
        m_strErrInfo = sqlite3_errmsg(m_pDb);
        return false;
    }

    //set the time out time
    if(SQLITE_OK != SetBusyTimeout( m_nTimeoutMs ))
		return false;

    //set the flag of synchronization(打开同步开关标志)
    const char * pTmp = NULL;
    if ( inSynFlag )
    {
        pTmp = STRSYNCHRONOUS_ON;
    }
    else
    {
        pTmp = STRSYNCHRONOUS_OFF;
    }
    ExecDML ( pTmp );
    if ( SQLITE_OK != m_nState )
    {
        m_strErrInfo = sqlite3_errmsg(m_pDb);
        m_strErrInfo.append(" . So database had been closed .");
        Close();
        return false;
    }
    //set the max buffer size设置最大缓存
    ExecDML ( STRCACHESIZE );
    if ( SQLITE_OK != m_nState )
    {
        m_strErrInfo = sqlite3_errmsg(m_pDb);
        m_strErrInfo.append(" . So database had been closed .");
        Close();
        return false;
    }

    m_bIsOpen = true;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	重新打开SQLite
// Input:		inSynFlag  是否打开同步标志
// Output:
// Return:		true  Success, false Failed
bool CSQLite::Reopen( bool inSynFlag )
{
    Close();
    return Open( m_strFileName.c_str(), inSynFlag );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	Close DB
// Input:		null
// Output:		null
// Return:		null
void CSQLite::Close( void )
{
    if ( NULL != m_pDb)
    {
       m_nState = sqlite3_close( m_pDb );
       m_pDb = NULL;
    }
    m_bIsOpen = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Set busy  timeout
// Input:   inMS , timeout time in ms(超时的毫秒数)
// Output:
// Return:  ErrorCode
int CSQLite::SetBusyTimeout( unsigned int inMS )
{
    m_nTimeoutMs = inMS;

    m_nState = sqlite3_busy_timeout( m_pDb, m_nTimeoutMs );
    if ( SQLITE_OK != m_nState )
    {
        m_strErrInfo = sqlite3_errmsg(m_pDb);
    }
    return  m_nState;
}

/* -2- **** Execute SQL  information ****/

//Execute SQL  DML  sentence
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	execute the specified DML SQL statment
// Input: inSql SQL statment to be executed
// Output:
// Return:  ErrorCode
int CSQLite::ExecDML( const char * inSql )
{
    if (NULL == inSql)
    {
       m_strErrInfo = "The inputed SQL sentence is empty .";
       return  DEF_INPUTISNULL;
    }
	char * errmsg = NULL;
    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
		// this is the point
        m_nState = sqlite3_exec( m_pDb, inSql, NULL, NULL, &errmsg );
        if ( SQLITE_OK == m_nState )
        {
            break;
        }
        else if ( SQLITE_BUSY == m_nState )
        {
            if ( m_nLimitNum == m+1 )
            {
                m_strErrInfo = sqlite3_errmsg( m_pDb );
                break;
            }
            sqlite3_busy_handler( m_pDb, NULL, NULL );
            Sleep( SLEEPTIME );
            continue;
        }
        else
        {
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            break;
        }
    }
    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Execute transaction
// Input:    inTransaction 输入的事务
// Output:
// Return:  ErrorCode
int CSQLite::ExecDML(const vector<string> &inTransaction)
{
    unsigned int nStrNum = (unsigned int)inTransaction.size();
    if ( 0 ==  nStrNum )
    {
        return SQLITE_OK;
    }

    //以下是事务执行，事务必须成功或者回滚
    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState = sqlite3_exec( m_pDb, STRBEGINTRANS, NULL, NULL, NULL );
        if ( SQLITE_OK == m_nState )
        {
            break;
        }
        else if ( SQLITE_BUSY == m_nState )
        {
            if ( m_nLimitNum == m+1 )
            {
                m_strErrInfo = sqlite3_errmsg( m_pDb );
                return m_nState;
            }
            //等待指定时间间隔继续执行
            sqlite3_busy_handler( m_pDb, NULL,NULL );
            Sleep( SLEEPTIME );
        }
        else
        {
            m_strErrInfo = sqlite3_errmsg(m_pDb);
            return m_nState;
        }
    }

    //执行相关SQL语句
    for ( unsigned int i = 0; i < nStrNum; i++ )
    {
        for( unsigned int m = 0; m < m_nLimitNum; m++ )
        {
            m_nState = sqlite3_exec( m_pDb, inTransaction[i].c_str(), NULL, NULL, NULL );
            if ( SQLITE_OK == m_nState )
            {
                break;
            }
            else if( SQLITE_BUSY == m_nState )
            {
                if ( m_nLimitNum == m+1 )
                {
                    m_strErrInfo = sqlite3_errmsg( m_pDb );
                    sqlite3_exec( m_pDb, STRROLLBACK, NULL, NULL, NULL );
                    return m_nState;
                }
                //等待指定时间间隔继续执行
                sqlite3_busy_handler( m_pDb, NULL,NULL );
                Sleep( SLEEPTIME );
            }
            else
            {
                m_strErrInfo = sqlite3_errmsg( m_pDb );
                sqlite3_exec( m_pDb, STRROLLBACK, NULL, NULL, NULL );
                return m_nState;
            }
        }
    }

    //提交事务
    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState = sqlite3_exec( m_pDb, STRCOMMITTRANS, NULL, NULL, NULL );
        if ( SQLITE_OK == m_nState )
        {
            break;
        }
        else if( SQLITE_BUSY == m_nState )
        {
            if ( m_nLimitNum == m+1 )
            {
                m_strErrInfo = sqlite3_errmsg( m_pDb );
                return m_nState;
            }
            //等待指定时间间隔继续执行
            sqlite3_busy_handler( m_pDb, NULL,NULL );
            Sleep( SLEEPTIME );
        }
        else
        {
            m_strErrInfo = sqlite3_errmsg( m_pDb );                   //调整执行顺序    modified by yinyong 2007.7.17
            sqlite3_exec( m_pDb, STRROLLBACK, NULL, NULL, NULL );
            return m_nState;
        }
    }

    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	查询表中的数据( 专用)
// Input:		inSql: select sql sentence
// Output:		dbResult: 保存查询结果；nRow:结果行数；nClumn: 结果列数；errmsg:错误信息
// Return:		ErrorCode
int CSQLite::ExecQuery(const char * inSql,  char **dbResult, int &nRow, int &nColumn, char *&errmsg)
{
	//检查输入查询语句
	if ( NULL == inSql )
	{
		m_strErrInfo = "The inputed SQL is empty.";
		return  DEF_INPUTISNULL;
	}
	int result;
	int index = 0;
	CString lstring;
	CString resultString = "";

	if (m_pDb == NULL) {
		return SQLITE_ERROR;
	}

	//数据库操作代码
	//假设前面已经创建了 MyTable_1 表
	//开始查询，传入的 dbResult 已经是 char **，这里又加了一个 & 取地址符，传递进去的就成了 char ***
	result = sqlite3_get_table( m_pDb, 
								inSql, 
								&dbResult, 
								&nRow, 
								&nColumn, 
								&errmsg );
	//到这里，不论数据库查询是否成功，都释放 char** 查询结果，使用 sqlite 提供的功能来释放
	//sqlite3_free_table( dbResult );
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	查询表中的数据( 专用)
// Input:		inSql: select sql sentence
//				param: the input parameter,which is an pointer in any type
// Output:		errmsg: the string to hold the error message
// Return:		ErrorCode
int CSQLite::ExecQuery(const char * inSql, void * para, char * errmsg)
{
	//start the query
	if (m_pDb == NULL) {
		return SQLITE_ERROR;
	}
	int result = sqlite3_exec(	m_pDb,		// sqlite data base
								inSql,		// sql statement
								LoadResult,	// call back function
								para,		// parameter can be used in call back function
								&errmsg );	// a string which will hold the error information if some error occurs
	return result;
}

//sqlite3的回调函数         
// sqlite 每查到一条记录，就调用一次这个回调
int LoadResult( void * para, int n_column, char ** column_value, char ** column_name )
{
	//para是你在 sqlite3_exec 里传入的 void * 参数
	//通过para参数，你可以传入一些特殊的指针（比如类指针、结构指针），然后在这里面强制转换成对应的类型（这里面是void*类型，必须强制转换成你的类型才可用）。然后操作这些数据
	//n_column是这一条记录有多少个字段 (即这条记录有多少列)
	// char ** column_value 是个关键值，查出来的数据都保存在这里，它实际上是个1维数组（不要以为是2维数组），每一个元素都是一个 char * 值，是一个字段内容（用字符串来表示，以\0结尾）
	//char ** column_name 跟 column_value是对应的，表示这个字段的字段名称

	//CAU5800APermissionConfiguratorView *pView = (CAU5800APermissionConfiguratorView*)para;
	int i;
	CString result[16];
	for( i = 0 ; i < n_column; i ++ )
	{
		result[i].Format("%s", column_value[i]);
		//lstring.Format( "字段名:%s\t> 字段值:%s\n",  column_name[i], column_value[i] );
		//resultString += lstring;
	}
	CAU5800APermissionConfiguratorView *pView = ((CAU5800APermissionConfiguratorApp*)AfxGetApp())->m_pView;
	pView->AddLogToList(	result[4], result[6], result[7], atoi(result[5].GetBuffer(0)), 
							result[1], result[2], result[3], atoi(result[8].GetBuffer(0)));
	//if (pView != NULL) {
	//	pView->AddLogToList(result[4], result[6], result[7], atoi(result[5].GetBuffer(0)), 
	//						result[1], result[2], result[3], atoi(result[8].GetBuffer(0)));
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:查询表中的数据( 专用)
// Input:   inSql: select sql sentence
// Output:
// Return:	ErrorCode
int CSQLite::ExecQuery(const char * inSql)
{
   //检查输入查询语句
   if ( NULL == inSql )
   {
      m_strErrInfo = "The inputed SQL is empty.";
      return  DEF_INPUTISNULL;
   }
   for( unsigned int m = 0; m < m_nLimitNum; m++ )
   {
       //执行查询语句
       m_nState = sqlite3_prepare( m_pDb, inSql, -1, &m_pStmt, NULL );
       if ( SQLITE_OK == m_nState )
       {
          //获取查询的字段个数
           m_nCurFieldNum = sqlite3_column_count( m_pStmt );
           break;
       }
       else if( SQLITE_BUSY == m_nState )
       {
           if ( m_nLimitNum == m+1 )
           {
               m_strErrInfo = sqlite3_errmsg( m_pDb );
               sqlite3_finalize(m_pStmt);
               m_pStmt = NULL;
               break;
           }
           //等待指定时间间隔继续执行
           sqlite3_busy_handler( m_pDb, NULL,NULL );
           Sleep( SLEEPTIME );
           continue;
       }
       else
       {
           m_strErrInfo = sqlite3_errmsg( m_pDb );
           sqlite3_finalize(m_pStmt);
           m_pStmt = NULL;
           break;
       }
    }
    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:  获取一行数据内容
// Input:     ioValue, 保存一行数据
// Output:
// Return:	SQLITE_BUSY:   数据库被锁
//          SQLITE_ROW:    数据未取完, 需要继续读取数据
//          SQLITE_DONE:   数据已经读取完成
//          other:         读取数据库记录失败
int CSQLite::Fetch( vector<string> & ioValues )
{
    //清空数组
    ioValues.clear();
    bool bRelease = true; //是否释放游标

    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState = sqlite3_step( m_pStmt );

        if ( SQLITE_DONE  == m_nState )
        {
            // no rows
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            goto __Exit;
        }
        else if ( SQLITE_ROW == m_nState )
        {
            //循环获取字段的内容
            for ( unsigned int i = 0; i < m_nCurFieldNum; i++ )
            {
                const char* pTmp = NULL;   //防止空值, modified by yinyong 2007.12.27
                pTmp = (const char*)( sqlite3_column_text( m_pStmt, i ) );
                if ( NULL == pTmp )
                {
                    ioValues.push_back( "" );
                 }
                else
                {
                    ioValues.push_back( pTmp );
                }
            }
            bRelease = false;
            goto __Exit;
        }
        else if ( SQLITE_BUSY == m_nState )
        {
           if ( m_nLimitNum == m+1 )
           {
               m_strErrInfo = sqlite3_errmsg( m_pDb );
               goto __Exit;
           }
           sqlite3_busy_handler( m_pDb,NULL,NULL);
           Sleep( SLEEPTIME );
           continue;
        }
        else
        {
            //other exceptions
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            goto __Exit;
        }
    }
__Exit:
    if ( NULL != m_pStmt && bRelease )
    {
        sqlite3_finalize( m_pStmt );
        m_pStmt = NULL;
    }
    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:获取一行数据内容
// Input:	ioValue, 保存一行数据
//          ioNull,    保存空值的标志 0  为空, 1 非空
// Output:
// Return:	SQLITE_BUSY:   数据库被锁
//          SQLITE_ROW:    数据未取完, 需要继续读取数据
//          SQLITE_DONE:   数据已经读取完成
//          other:         读取数据库记录失败
int CSQLite::Fetch( vector<string> & ioValues, vector<int>& ioNull )
{
    //清空数组
    ioValues.clear();
    ioNull.clear();
    bool bRelease = true; //是否释放游标

    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState = sqlite3_step( m_pStmt );

        if ( SQLITE_DONE  == m_nState )
        {
            // no rows
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            goto __Exit;
        }
        else if ( SQLITE_ROW == m_nState )
        {
            //循环获取字段的内容
            for ( unsigned int i = 0; i < m_nCurFieldNum; i++ )
            {
                const char* pTmp = NULL;   //防止空值, modified by yinyong 2007.12.27
                pTmp = (const char*)( sqlite3_column_text( m_pStmt, i ) );
                 //有空值
                if ( NULL == pTmp )
                {
                    ioValues.push_back( "" );
                    ioNull.push_back(0);
                 }
                else
                {
                    ioValues.push_back( pTmp );
                    ioNull.push_back(1);
                }
            }
            bRelease = false;
            goto __Exit;
        }
        else if ( SQLITE_BUSY == m_nState )
        {
           if ( m_nLimitNum == m+1 )
           {
               m_strErrInfo = sqlite3_errmsg( m_pDb );
               goto __Exit;
           }
           sqlite3_busy_handler( m_pDb,NULL,NULL);
           Sleep( SLEEPTIME );
           continue;
        }
        else
        {
            //other exceptions
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            goto __Exit;
        }
    }
__Exit:
    if ( NULL != m_pStmt && bRelease )
    {
        sqlite3_finalize( m_pStmt );
        m_pStmt = NULL;
    }
    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:Table is or not exist
// Input:	inTabName: Table's name
// Output:
// Return:  ErrorCode
int CSQLite::TableExists( const char * inTabName )
{
    char szSQL[128] = "";
    sprintf( szSQL,"select count(*) from sqlite_master where type='table' and name='%s'",inTabName );

    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState =  sqlite3_prepare( m_pDb, szSQL, -1, &m_pStmt, NULL );
        if ( SQLITE_OK == m_nState )
        {
            break;
        }
        else if( SQLITE_BUSY == m_nState )
        {
           if ( m_nLimitNum == m+1 )
           {
               m_strErrInfo = sqlite3_errmsg( m_pDb );
               goto __Exit;
           }
           //等待指定时间间隔继续执行
           sqlite3_busy_handler( m_pDb, NULL,NULL );
           Sleep( SLEEPTIME );
           continue;
        }
        else
        {
           m_strErrInfo = sqlite3_errmsg( m_pDb );
           goto __Exit;
        }
    }

    for( unsigned int m = 0; m < m_nLimitNum; m++ )
    {
        m_nState = sqlite3_step( m_pStmt );
        if ( SQLITE_ROW == m_nState )
        {
            if ( sqlite3_column_int( m_pStmt, 0 ) > 0 )
            {
                m_nState = DEF_TABLEEXIST;
            }
            else
            {
                m_nState = DEF_TABLENOTEXIST;
            }
            break;
        }
        else if ( SQLITE_BUSY == m_nState )
        {
            if ( m_nLimitNum == m+1 )
            {
                m_strErrInfo = sqlite3_errmsg( m_pDb );
                goto __Exit;
            }
            sqlite3_busy_handler( m_pDb, NULL,NULL );
            Sleep( SLEEPTIME );
        }
        else
        {
            m_strErrInfo = sqlite3_errmsg( m_pDb );
            goto __Exit;
        }
    }

__Exit:
    if ( NULL != m_pStmt )
    {
        sqlite3_finalize( m_pStmt );
        m_pStmt = NULL;
    }
    return m_nState;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: convert error code to strings
// Input:
// Output:
// Return:  ErrorCode
const char* CSQLite::ErrCodeAsString(int inErrCode)
{
  switch (inErrCode)
  {
         case SQLITE_OK           : return "SQLITE_OK";
         case SQLITE_ERROR        : return "SQLITE_ERROR";
         case SQLITE_INTERNAL     : return "SQLITE_INTERNAL";
         case SQLITE_PERM         : return "SQLITE_PERM";
         case SQLITE_ABORT        : return "SQLITE_ABORT";
         case SQLITE_BUSY         : return "SQLITE_BUSY";
         case SQLITE_LOCKED       : return "SQLITE_LOCKED";
         case SQLITE_NOMEM        : return "SQLITE_NOMEM";
         case SQLITE_READONLY     : return "SQLITE_READONLY";
         case SQLITE_INTERRUPT    : return "SQLITE_INTERRUPT";
         case SQLITE_IOERR        : return "SQLITE_IOERR";
         case SQLITE_CORRUPT      : return "SQLITE_CORRUPT";
         case SQLITE_NOTFOUND     : return "SQLITE_NOTFOUND";
         case SQLITE_FULL         : return "SQLITE_FULL";
         case SQLITE_CANTOPEN     : return "SQLITE_CANTOPEN";
         case SQLITE_PROTOCOL     : return "SQLITE_PROTOCOL";
         case SQLITE_EMPTY        : return "SQLITE_EMPTY";
         case SQLITE_SCHEMA       : return "SQLITE_SCHEMA";
         case SQLITE_TOOBIG       : return "SQLITE_TOOBIG";
         case SQLITE_CONSTRAINT   : return "SQLITE_CONSTRAINT";
         case SQLITE_MISMATCH     : return "SQLITE_MISMATCH";
         case SQLITE_MISUSE       : return "SQLITE_MISUSE";
         case SQLITE_NOLFS        : return "SQLITE_NOLFS";
         case SQLITE_AUTH         : return "SQLITE_AUTH";
         case SQLITE_FORMAT       : return "SQLITE_FORMAT";
         case SQLITE_RANGE        : return "SQLITE_RANGE";
         case SQLITE_ROW          : return "SQLITE_ROW";
         case SQLITE_DONE         : return "SQLITE_DONE";

         case DEF_INPUTISNULL     : return "Input string is NULL";

         default                  : return "UNKNOWN_ERROR";
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: 默认释放类成员m_pStmt
// Input:
// Output:
// Return:
void CSQLite::FreeStmt( void )
{
    if ( NULL != m_pStmt )
    {
        sqlite3_finalize( m_pStmt );
        m_pStmt = NULL;
    }
}
