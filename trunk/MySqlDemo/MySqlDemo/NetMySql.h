//#include "include/mysql.h"
#include <Afxmt.h>
#include <afxsock.h>
#include <mysql.h>

//*************************************************************
//*************************************************************
//**************使用时请根据需要修改以下数据*******************
//*************************************************************
//*************************************************************
#pragma comment(lib,"libmysql.lib")
//#pragma comment(lib,"lib\\mysqlclient.lib")
//#pragma comment(lib,"lib\\mysys-max.lib")
//#pragma comment(lib,"lib\\mysys-nt.lib")
//#pragma comment(lib,"lib\\regex.lib")
//#pragma comment(lib,"lib\\strings.lib")
//#pragma comment(lib,"lib\\zlib.lib")

#define DEFAULT_FIELD        16  //你所要操作的库中，数据表的最大的字段数。假如student表有5个字段，
                                 //分别是name,old,sex,profession,class，而teacher表有10个字段，则
                                 //此值应添10。最好设置为最大字段数，设置太大会占用非常大的内存。

#define DEFAULT_FIELD_SIZE   50  //每个字段名称的最大字节数。如：name为4个字节，profession为10个字节
                                 //此处应该添10。注意：一个汉字等于2个字节，不建议使用汉字作为字段名。
                                 //同样，此值也不应该设置过大，否则也会占用非常大的内存。
//**************************************************************
//**************************************************************
//*******************使用环境需要MFC****************************
//**************************************************************
//**************************************************************

class CNetMySql
{
private:
	//MySql的连接句柄
	MYSQL myData;

	//MySql预处理句柄
	MYSQL_STMT *myStmt;

	//MySql的结果集
    MYSQL_RES *result;
    //存储查询结果集的HashTable
	CMapWordToPtr res;

	char m_pwd[32];
public:
	char m_host[32],m_name[32];
	unsigned m_port;

public:
	CNetMySql();
	~CNetMySql();

	//连接MySql服务器,
	BOOL Connect(char * host,char * name,char *pwd,char * db,unsigned port);

	//执行非查询语句,返回成功或失败
	BOOL ExecuteNoQuery(char ** sqlArray,int count);
	//执行查询语句，返回查询到的行数，处理完结果后，要调用FreeResult释放结果
	//如果不调用FreeResult，则在下次查询时自动释放上次的结果集
	unsigned ExecuteQuery(char * sql);
	//释放查询的结果集
	void FreeResult();
     
	//改变用户，使用却省db，如果db为空，则不使用
	BOOL ChangeUser(char * name,char *pwd,char *db);

	//以当前用户的身份，连接其他数据库
	BOOL SelectDB(char * db);

	//关闭连接
	void Close();

	//执行带参数的非SELECT SQL语句
	//以下四个函数无法在4.1以下的版本中使用
	BOOL ExecutePrepare(char * sql);
	BOOL BindParam(MYSQL_BIND * bind);
    unsigned Execute();
	BOOL ClosePrepare();


	//获得指定行和指定列的整形值
	//行列都从0开始编号
        // row     ：行号
        // fd_name ：列名称
        // fd_num  ：列字段号
        // 注意    ：Get**Value系列函数的参数要确保正确，否则不保证返回值将正确
	int GetIntValue(int row, char * fd_name);
	int GetIntValue(int row, int fd_num);

	//获得符点型值
	//从0开始编号
	float GetFloatValue(int row,char * fd_name);
	float GetFloatValue(int row,int fd_num);

	//获得高精度符点型值
	//从0开始编号
	double GetDoubleValue(int row,char * fd_name);
	double GetDoubleValue(int row,int fd_num);

	//获得布尔型值
	//从0开始编号
	BOOL GetBoolValue(int row,char * fd_name);
	BOOL GetBoolValue(int row,int fd_num);

	//获得字符串值，返回字符串长度
	//从0开始编号
	char * GetStringValue(int row,char * fd_name, unsigned * len);
    char * GetStringValue(int row,int fd_num,unsigned * len);

	//获得日期时间型数据，以字符串形式表示
	//从0开始编号
	//格式如下：0000-00-00 00:00:00
	char * GetDataTimeValue(int row,char * fd_name);
	char * GetDataTimeValue(int row, int fd_num);

	//获得二进制数据的缓冲区
	//从0开始编号，当传递给pBuf==NULL时，获得缓冲区长度
	int GetBinaryValue(int row,char * fd_name,char * pBuf);
	int GetBinaryValue(int row,int fd_num,char * pBuf);
};