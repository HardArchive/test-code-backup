// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
			s_sql += "1,1,1,'��������')";
			if(!store.Exec(s_sql))
			{
				//ִ��SQL������ 
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
			s_sql += " set acttype=2,memo='��������',actresult=3";
			s_sql += " where acttype = 1";
			if(!store.Exec(s_sql))
			{
				//ִ��SQL������
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
	std::cout<<"������ʾ�̡߳�"<<endl;
	for(;;)
	{		
		for(;;)
		{
			CSqlClient client;
			COperMysql store;
			if(!store.SetTransAction(client.GetConnect()))
			{
				std::cout<<"���߳����ӳ�æ���ȴ��С�"<<endl;
				break;
			}

			/*
			����ܷ��������磬���������ʹ��mysql_use_result()
			���������Ѿ�������mysql_free_result()֮ǰ��ͼ
			ִ���²�ѯ���������mysql_use_result()��
			mysql_store_result()֮����ͼִ�з�����
			�ݵ�2����ѯ����Ҳ���ܷ�����*/

			std::string s_sql = "select * from processingtransactioninterface";  //��ȡ��ͷ
			if(!store.Query(s_sql))
			{
				//ɾ����ʧ��
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

				s_sql = "delete from processingtransactioninterface";   //ɾ��������
				if(!store.Exec(s_sql))
				{
					//ɾ����ʧ��
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
			std::cout<<"�������ݿ�ɹ�!!!"<<endl;
		}
		else
		{
			std::cout<<"�������ݿ�ʧ��!!!"<<endl;
		}					
		
		//bReturn = conn->Connect("10.0.0.218", "root", "tiptop", "test", MYSQL_PORT);
		CManagerPool::GetInitInstance().AddConObject(conn);
	}

	CSqlClient client;

	COperMysql store;
	store.SetTransAction(client.GetConnect());

	//��ɾ�����ݱ�
	std::string s_sql = "drop table processingtransactioninterface";//ɾ����
	if(!store.Exec(s_sql))
	{
		//ɾ����ʧ��
		std::cout<<"drop table fail"<<endl<<store.What()<<endl;   
	}
	else
	{
		std::cout<<"drop table success"<<endl;
	}

	//�������ݱ�,�ֶ� actid ����������������
	s_sql = "create table processingtransactioninterface(";
	s_sql += "actid integer not null AUTO_INCREMENT,";
	s_sql += "acttype integer not null,resourceid integer not null,";
	s_sql += " subresoureid  integer, actdate varchar(19), actresult integer,";
	s_sql += " memo varchar(50) null,primary key(actid))";
	if(!store.Exec(s_sql))
	{
		//������ʧ��
		std::cout<<"create table fail"<<endl;
		return -1;
	}
	else
	{
		std::cout<<"create table success"<<endl;
		std::cout<<s_sql.c_str()<<endl;
	}
	std::cout<<"��ʼ�����̡߳���"<<endl;

	unsigned long p1 = 0;	
	::CreateThread(0,0,(funcptr)show,0,0,&p1);

	unsigned long p2 = 0;	
	::CreateThread(0,0,(funcptr)update,0,0,&p2);
	
	unsigned long p3 = 0;
	::CreateThread(0,0,(funcptr)insert,0,0,&p3);

	system("pause");
	return 0;
}

