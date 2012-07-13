#pragma once

#include <WinSock2.h> //仅windows下
#include <mysql.h>
#pragma comment(lib,"libmysql.lib") //unix下可静态链接libmysqlclient.a

#define DEFAULT_FIELD        16  //你所要操作的库中，数据表的最大的字段数。假如student表有5个字段，
//分别是name,old,sex,profession,class，而teacher表有10个字段，则
//此值应添10。最好设置为最大字段数，设置太大会占用非常大的内存。

#define DEFAULT_FIELD_SIZE   50  //每个字段名称的最大字节数。如：name为4个字节，profession为10个字节
//此处应该添10。注意：一个汉字等于2个字节，不建议使用汉字作为字段名。
//同样，此值也不应该设置过大，否则也会占用非常大的内存。

class CMySql
{
public:
	CMySql(void);
	~CMySql(void);
	//CMySql(char* pszHostIp, unsigned uPort, char* pszDbName);     //初始化构造函数
public:
	char m_szhostIP[32];                //数据库IP
	//char m_szDbName[32];                //数据库名
	char m_szUserName[32];              //数据库连接用户名
	char m_szpwd[32];                   //数据库连接密码
	unsigned m_uPort;                   //连接端口
private:
	MYSQL m_stuMySql;                   //MySql的连接句柄
	
	MYSQL_RES* m_pstuResult;//MySql的结果集
	//typedef int(* DealData)(void); // 声明CallBack 类型的函数指针
	
	//CMapWordToPtr m_objRes;//存储查询结果集的HashTable
public:
	BOOL Connect(char* pszHostIp, char* pszUserName, char* pszpwd, char* pszDbName, unsigned uPort);    //连接CMySql服务器
	void Close();                      //关闭连接
	//增改删查数据库指定字段内容
	void QueryItem(char* pszTableName, char* pInbuffer);//查询数据库  表名 参二：查询条件（eg：lsid=2 and lssex=1）	
	void ModifyItem(char* pszTableName, char* pInbuffer);//修改一条记录
	void AddItem(char* pszTableName, char* pInbuffer);   //增加一条记录
	void DeleteItem(MYSQL *myData);                      //删除一条记录
	//指定表搜索
	
	virtual int DealReasult() = 0;           //处理数据集函数  纯
private:		
	void FreeResult();//释放查询的结果集
};
