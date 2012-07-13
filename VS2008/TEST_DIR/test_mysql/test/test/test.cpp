// test.cpp : 定义控制台应用程序的入口点。
//
// MySqlTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
//#include "stdafx.h"
//#include <mysql_connection.h>
//#include <mysql_driver.h>
//#include <statement.h>
//using namespace sql;
//using namespace std;
//void RunConnectMySQL() 
//{
//	mysql::MySQL_Driver *driver;
//	Connection *con;
//	Statement *state;
//	ResultSet *result;
//	// 初始化驱动
//	driver = sql::mysql::get_mysql_driver_instance();
//	// 建立链接
//	con = driver->connect("tcp://127.0.0.1:3306", "root", "tiptop");
//	state = con->createStatement();
//	state->execute("use test");
//	// 查询
//	result = state->executeQuery("select * from testuser where id < 1002");
//	// 输出查询
//	while(result->next())
//	{
//		int id = result->getInt("ID");
//		string name = result->getString("name");
//		cout << id << " : " << name << endl;
//	}
//	delete state;
//	delete con;
//}
//int _tmain(int argc, _TCHAR* argv[])
//{
//	RunConnectMySQL();
//	getchar();
//	return 0;
//}



#include "stdafx.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <statement.h>
using namespace sql;
using namespace std;
#pragma comment(lib, "mysqlcppconn.lib");
#pragma comment(lib, "mysqlcppconn-static.lib");
void RunConnectMySQL() 
{
	mysql::MySQL_Driver *driver;
	Connection *con;
	Statement *state;
	ResultSet *result;
	// 初始化驱动
	driver = sql::mysql::get_mysql_driver_instance();
	// 建立链接
	con = driver->connect("tcp://10.0.100.202:3306", "root", "tiptop");
	state = con->createStatement();
	state->execute("use confidentialcirculation");
	// 查询
	result = state->executeQuery("select * from leadership where lsid < 12");
	// 输出查询
	while(result->next())
	{
		int id = result->getInt("lsid");
		string name = result->getString("lsname");
		cout << id << ":    " << name << endl;
	}
	delete state;
	delete con;
}
int _tmain(int argc, _TCHAR* argv[])
{
	RunConnectMySQL();
	getchar();
	return 0;
}