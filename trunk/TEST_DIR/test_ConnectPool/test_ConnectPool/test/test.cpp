// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Connect.h"

typedef unsigned long  APIRETURN;

APIRETURN insert(void * para)
{
	for(;;)
	{
		for(int i = 0;i < 200;++i)
		{
			CSqlClient client;
			COperMysql store;
			if(!store.SetTransAction(client.GetConnect()))
			{
				//std::cout<<"db connect pool busy on insert"<<endl;
				break;
			}
			std::string s_sql = "";
			s_sql = "insert into processingtransactioninterface";
			s_sql += "(acttype,resourceid,actresult,memo) values(";
			s_sql += "1,1,1,'插入数据')";
			if(!store.Exec(s_sql))
			{
				//执行SQL语句出错 
				store.RollBack();
				std::cout<<"execute insert syntax fail,"<<store.What()<<endl;
			}
			else
			{
				store.Commit();
			}
		}

		Sleep(DWORD(1000 * 0.01));
	}
	return 0;
}

APIRETURN update(void * para)
{
	for(;;)
	{
		for(;;)
		{
			CSqlClient client;
			COperMysql store;
			if(!store.SetTransAction(client.GetConnect()))
			{
				//std::cout<<"db connect pool busy on update"<<endl;
				break;
			}
			std::string s_sql = "";
			s_sql = "update processingtransactioninterface";
			s_sql += " set acttype=2,memo='更新数据',actresult=3";
			s_sql += " where acttype = 1";
			if(!store.Exec(s_sql))
			{
				//执行SQL语句出错
				store.RollBack();
				std::cout<<"execute update syntax fail,"<<store.What()<<endl;
			}
			else
			{
				store.Commit();
			}
			break;
		}

		Sleep(DWORD(1000 * 0.01));
	}

	return 0;
}

APIRETURN show(void * para)
{
	std::cout<<"进入显示线程…"<<endl;
	for(;;)
	{		
		for(;;)
		{
			CSqlClient client;
			COperMysql store;
			if(!store.SetTransAction(client.GetConnect()))
			{
				std::cout<<"在线程连接池忙，等待中…"<<endl;
				break;
			}

			/*
			这可能发生，例如，如果你正在使用mysql_use_result()
			并且在你已经调用了mysql_free_result()之前试图
			执行新查询。如果你在mysql_use_result()或
			mysql_store_result()之间试图执行返回数
			据的2个查询，它也可能发生。*/

			std::string s_sql = "select * from processingtransactioninterface";  //获取表头
			if(!store.Query(s_sql))
			{
				//删除表失败
				std::cout<<"query table fail,"<<store.What()<<endl;

			}
			else
			{
				unsigned long ulTem = store.RowCount();
				for(unsigned long i = 0;i <store.RowCount();++i)
				{
					std::string s_out1 = "actresult";

					std::string s_out2 = "memo";
					;
					std::cout<<"result:"<<store.GetItemLong(i, s_out1)
						<<"memo:"<<store.GetItemString(i, s_out2).c_str()
						<<";actid:"<<store.GetItemLong(i,0)<<endl;
				}

				s_sql = "delete from processingtransactioninterface";   //删除表数据
				if(!store.Exec(s_sql))
				{
					//删除表失败
					store.RollBack();
					std::cout<<"delete table fail,"<<store.What()<<endl;
				}
				else
				{
					store.Commit();
				}
			}
			break;
		}

		Sleep(DWORD(1000 * 0.1));
	}
	return 0;
}

typedef unsigned long (__stdcall * funcptr)(void *);


int _tmain(int argc, _TCHAR* argv[])
{
	bool bReturn = false;
	for(int i = 0;i < 5;++i)
	{
		//std::string host = "10.0.0.218";
		//std::string user = "root";
		//std::string password = "tiptip";
		//std::string dbname = "test";
		//WORD port = MYSQL_PORT;
		CConnect * conn = new CConnect();
		bReturn = false;
		//bReturn = conn->Connect(host, user, password, dbname, port);
		bReturn = conn->Connect("localhost", "root", "tiptop", "test", MYSQL_PORT);
		if (bReturn)
		{
			std::cout<<"连接数据库成功!!!"<<endl;
		}
		else
		{
			std::cout<<"连接数据库失败!!!"<<endl;
		}					
		
		//bReturn = conn->Connect("10.0.0.218", "root", "tiptop", "test", MYSQL_PORT);
		CManagerPool::GetInitInstance().AddConObject(conn);
	}

	CSqlClient client;

	COperMysql store;
	store.SetTransAction(client.GetConnect());

	//先删除数据表
	std::string s_sql = "drop table processingtransactioninterface";//删除表
	if(!store.Exec(s_sql))
	{
		//删除表失败
		std::cout<<"drop table fail"<<endl<<store.What()<<endl;   
	}
	else
	{
		std::cout<<"drop table success"<<endl;
	}

	//创建数据表,字段 actid 设置了自增列属性
	s_sql = "create table processingtransactioninterface(";
	s_sql += "actid integer not null AUTO_INCREMENT,";
	s_sql += "acttype integer not null,resourceid integer not null,";
	s_sql += " subresoureid  integer, actdate varchar(19), actresult integer,";
	s_sql += " memo varchar(50) null,primary key(actid))";
	if(!store.Exec(s_sql))
	{
		//创建表失败
		std::cout<<"create table fail"<<endl;
		return -1;
	}
	else
	{
		std::cout<<"create table success"<<endl;
		std::cout<<s_sql.c_str()<<endl;
	}
	std::cout<<"开始运行线程……"<<endl;

	unsigned long p1 = 0;	
	::CreateThread(0,0,(funcptr)show,0,0,&p1);

	unsigned long p2 = 0;	
	::CreateThread(0,0,(funcptr)update,0,0,&p2);
	
	unsigned long p3 = 0;
	::CreateThread(0,0,(funcptr)insert,0,0,&p3);

	system("pause");
	return 0;
}

