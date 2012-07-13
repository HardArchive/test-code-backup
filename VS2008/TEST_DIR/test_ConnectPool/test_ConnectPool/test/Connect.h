
/************************************************************************/
/* 
以下类成员所需要的的 加载库文件和头文件
  
	
	  
		
		  
#pragma comment(lib,"../lib/mysql/debug/libmySQL.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma warning (disable:4786)

  
#include <WINSOCK2.H>
#include <windows.h> 
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;
	  
                                                                     */
/************************************************************************/




// Connect.h: interface for the CConnect class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Muxtex/MyOsMutex.h"
#include "MyMutex.h"
using namespace QNA;

#pragma once
/*****************************************************************/
/*
类名			:	CConnect
功能			:	设置	或	获取	连接对象
数据成员		:		
	MYSQL *m_ptrCon;	连接对象
对外接口说明	:
提供链接对象的设置
		主机名		用户名		密码	数据库名	端口			
bool Connect(string	host,string	user,string	pwd,string dbname,	WORD port);

得到一个连接对象  
void *GetConObject();						
该函数返回值为		MYSQL *m_ptrCon;	连接对象

bool DisConnect();	中断连接
外部调用该函数需要给他强转成 MYSQL*
/*****************************************************************/
#include <mysql.h>

class CConnect
{
public:
	CConnect();										//构造函数
	virtual ~CConnect();							//析构函数
	
	bool Connect(
		string	host,								//主机
		string	user,								//用户名
		string	pwd,								//密码
		string	dbname,								//数据库名
		WORD	port								//端口号
		);											//连接数据库
	bool DisConnect();							//中断连接
	void *GetConObject();						//得到连接对象
	
private:
	//string	m_sqlerr;
	char    m_szSqlerr[MAX_PATH];
	MYSQL	*m_ptrCon;									//连接对象
	CConnect(const CConnect &tmp);						//拷贝函数
	CConnect &operator= (const CConnect &);				//赋值运算符

};
/************************************************************************/
/*                                                                      
类名		:		连接池的管理类		此类为单件模式
数据成员	:			
	queue<CConnect*>	m_PoolCon		连接池的管理对象 存放未使用的连接对象
	CCriticalSection	m_Mutex;	    临界区对象	提供函数同步操作
	CManagerPool *		m_PtrInstance;	数据库连接池的静态管理对象
  
对外接口说明	:				
成员函数	:
	GetInitInstance();          初始化连接池管理对象	
	m_PtrInstance
	AddConObject();             添加接连对象的	
	CConnect * GetConObject();  获取一个连接对象
	GetPoolSize();              获取连接池中可用连接对象
	Release();                  释放连接对象,重新加入队列给连接池去管理
					
/************************************************************************/

class CManagerPool  
{
private:
	CManagerPool();
public:
	static CManagerPool &GetInitInstance();   // 初始化应该用程序连接池实例
	void AddConObject(CConnect * conn);       // 把链接对象加入连接池
	CConnect * GetConObject();                // 从连接池获取链接对象  
	void Release(CConnect * conn);            // 释放连接
	int GetPoolSize();                        // 获取连接数量
	virtual ~CManagerPool();
	
private:	
	queue<CConnect*>				m_PoolCon;		//连接池数据
	CCriticalSection        		m_Mutex;		//临界区对象
	bool							m_state;		//是否对外提供连接服务	
	static CManagerPool *			m_PtrInstance;	//数据库连接池的管理对象
	
};
/************************************************************************/
/*                                                                      */
/************************************************************************/


//字段索引和字段类型的对应表
enum  filetype_t
{
	ENUM_F_T_CHAR = 1,	//		char 类型
	ENUM_F_T_INT,		//		int类型
	ENUM_F_T_DATETIME ,	//时间	DATETIME 
	ENUM_F_T_DOUBLE ,	//double
	ENUM_F_T_DEC ,		//dec??? 
	ENUM_F_T_UNKNOWN    //
};
//////////////////////////////////////////////////////////////////////////

struct typeset_t
{
	string			m_name;			//	字段名	
	filetype_t		m_type;			//	字段类型
	WORD			m_length;		//	字段长度
	WORD			m_index;		//	查询列表中的列位置
	typeset_t();					//	默认的初始化函数
};
//////////////////////////////////////////////////////////////////////////

typedef vector<string> row_t;

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*
类名		:	COperMysql
功能		:   获取一个连接对象 然后对了解对象进行相应的 数据库操作(添加,删除,修改,查询)
重要数据成员说明:
MYSQL		*m_PtrCon;					//指向  mysql 的连接指针
MYSQL_RES	*m_PtrRes;					//指向  mysql 的查询数据集
vector<typeset_t>		m_Fieldtype;	//字段信息表

提供对外接口说明:
成员函数:
1: bool SetTransAction(CConnect * conn);	//设置连接对象
函数说明:
	CConnect * conn	从管理池中获取为使用的连接对象;
	赋值给MYSQL *m_PtrCon	并且尝试连接操作	
2: bool Exec(string &ddl);	
3: bool Select(string &dml);
4: bool Commit();
5: bool RollBack();
                                                                    
/************************************************************************/
class COperMysql 
{
public:
	COperMysql();
	virtual ~COperMysql();	
	
public:	
	DWORD RowCount();											//得到查询记录行数
	DWORD GetIncreaseID();										//得到新自增的序列号
	bool SetTransAction(CConnect * conn);						//设置连接对象
	bool GetStatus();											//得到当前执行状态
	char* What();												//错误原因	
	bool Exec(string &ddl);										//执行数据定义语言(DDL)类语句
	bool Select(string &dml);								    //执行数据操作语言(DML)类语句
	bool Commit();												//事务提交
	bool RollBack();											//事务回滚

	//执行数据操作语言(DML)类语句
	virtual bool Query(const std::string &dml);

	filetype_t SetFieldType(enum_field_types fieldtype);
								
	string GetItemString(DWORD row,WORD index);					//得到指定行某个字段的字符串类型值
	string GetItemString(DWORD row,string &fieldname);
	
	float GetItemFloat(DWORD row,WORD index);					//得到指定行某个字段的数值
	float GetItemFloat(DWORD row,string &fieldname);

	long GetItemLong(DWORD row,WORD index);						//得到指定行某个字段的整数值
	long GetItemLong(DWORD row,string &fieldname);
	

private:	
	void Clear();											//清除临时对象
	void SetIncreaseID(long id);							//设置自增序列号
	row_t * FindRow(DWORD findrow);							//找到行
	
	WORD GetFieldIndex(string &fieldname);					//找到对应的列序号
	string GetItemValue(DWORD row,WORD index);
	string GetItemValue(DWORD row,string &fieldname);
	
private:	
	MYSQL					*m_PtrCon;					//指向  mysql 的连接指针
	MYSQL_RES				*m_PtrRes;					//指向  mysql 的查询数据集
	WORD					m_Row;							//操作影响的记录数
	//string					m_Err;							//错误原因	
	char                    m_szErr[MAX_PATH];               //错误原因
	DWORD					m_SeqID;						//新的自增的序列号
	bool					m_Commit;						//事务提交模式
	// 	my_bool					auto_mode;
	bool					m_State;						//取得信息状态
	vector<typeset_t>		m_Fieldtype;					//字段信息表
	vector<row_t>			m_Recordset;
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
//从连接池获取一个对象
//用完之后并且自动释放连接
class CSqlClient
{
private:
	CConnect * m_PtrCon;
public:
	CSqlClient();
	
	~CSqlClient();	
	
	CConnect *GetConnect();	
};

/************************************************************************/
/*                                                                      */
/************************************************************************/

