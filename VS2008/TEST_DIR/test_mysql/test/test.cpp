// test.cpp : 定义控制台应用程序的入口点。
//
// CMySqlTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdio.h>
#include "MySql.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[], char* envp[])
{
	CMySql objMysql;
	objMysql.Connect("10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306);

	return 0;
}

/*
#include <WinSock2.h> //仅windows下
#include <mysql.h>
#pragma comment(lib,"libmysql.lib") //unix下可静态链接libmysqlclient.a

int main(int argc, char* argv[], char* envp[])
{

	//调用mysql_library_init()和mysql_library_end()的目的在于，为MySQL库提供恰当的初始化和结束处理。
	mysql_library_init(NULL,NULL,NULL);

	//分配或初始化与mysql_real_connect()相适应的MYSQL对象。
	MYSQL mysql;
	mysql_init(&mysql);   //初始化数据结构
	//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//设置编码方式  

	//尝试与运行在主机上的MySQL数据库引擎建立连接。
	//在你能够执行需要有效MySQL连接句柄结构的任何其他API函数之前，mysql_real_connect()必须成功完成。
	//数据库的连接（定义的数据库对象，本地/MySQL的服务端ip，用户名，密码，数据库名，固定，，，）
	if (!mysql_real_connect(&mysql, "10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306, NULL, NULL))
	{
		fprintf(stderr,"%s",mysql_error(&mysql));
		exit(1);
	}
	//该函数用于为当前连接设置默认的字符集。
	if (mysql_set_character_set(&mysql,"gb2312"))
	{
		fprintf(stderr,"%s",mysql_error(&mysql));     
		exit(1);
	}


	//如果模式为“1”，启用autocommit模式；如果模式为“0”，禁止autocommit模式。
	mysql_autocommit(&mysql, 0);

	//执行由“Null终结的字符串”查询指向的SQL查询。
	//mysql_query()不能用于包含二进制数据的查询，应使用mysql_real_query()取而代之
	//（二进制数据可能包含字符‘\0’，mysql_query()会将该字符解释为查询字符串结束）。
	//if (mysql_query(&mysql,"insert into `Users`(`username`,`password`)values('王五','abc123')"))
	//{
	//	fprintf(stderr,"%s",mysql_error(&mysql));
	//	exit(1);
	//}

	//回滚当前事务。
	//mysql_rollback(&mysql);

	//提交当前事务。
	//mysql_commit(&mysql);

	if (mysql_query(&mysql,"select * from `leadership`"))
	{
		fprintf(stderr, "select:%s", mysql_error(&mysql));
		exit(1);
	}

	//检索完整的结果集至客户端。
	MYSQL_RES *rs = mysql_store_result(&mysql);
	if (rs == NULL)
	{
		fprintf(stderr,"%s",mysql_error(&mysql));
		exit(1);
	}

	//返回结果集中的行数。
	printf("found %u rows.\r\n", mysql_num_rows(rs));
	//printf("found %n rows.\r\n", mysql_num_fields(rs));

	//从结果集中获取下一行
	MYSQL_ROW row;
	while((row = mysql_fetch_row(rs)) != NULL)
	{
		cout<<row[0]<<"\r;";
		cout<<row[1]<<"\r"<<endl;

	}

	//释放结果集使用的内存。
	mysql_free_result(rs);

	//关闭前面打开的连接。
	mysql_close(&mysql);

	mysql_library_end();

	return 0;
}
*/