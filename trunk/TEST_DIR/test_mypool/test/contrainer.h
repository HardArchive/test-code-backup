
/*
 * @time : July 23th,2011
 * @author : BoJing .Wang 
 * @goal : Achieve a contraner that contraned mysql connection
 *
 */
#include "stdafx.h"
#ifndef __CONTRAINER_H
#define __CONTRAINER_H

#include <iostream>
#include <string>
#include <cassert>
#include <boost/shared_ptr.hpp>
//#include <shared_ptr.hpp>
#include <vector>
#include <iterator>
#include <exception>
#include <fstream>
#include "mytime.h"

#ifdef __cplusplus
extern "C"
{
#endif

//#include <mysql/mysql.h>
#include <mysql.h>

#ifdef __cplusplus
}
#endif

#define MAX_CONNECTIONS 20
#define ADD_CONNECTIONS 2
#define INIT_NUM 6


using namespace std;

namespace pool
{
	/* *******************************************************
	 * @brief struct Connection comment
	 * @param sql[connection of databases] 
	 * @param state [the state of this connection ,true would be able to
	 * be connected ,false would be not]
	 * @param id [the id of this connection]
	 */
	//数据库连接结构
	struct Connection 
	{
		WTime time;      //连接时间
		MYSQL * sql;     //数据库句柄
		bool state;      //状态
		unsigned int id; //连接ID
	};

	/* **
	 * @brief struct UserInfo Comment
	 * @param DBUser [the user of databases]
	 * @param DBName [the name of databases]
	 * @param DBPwd [the possword of databases]
	 */
	//用户信息
	struct UserInfo
	{
		string DBUser;   //用户名
		string DBName;   //数据库名
		string DBPwd;    //密码
	};

	typedef vector<Connection> MyVector;    //连接向量
	MyVector::iterator MyIterator;          //连接向量迭代器

	/* ********************************************************
	 * @brief class comment
	 */
	class WBaseContrainer
	{
	protected:
		/* **
		 * A pure virtual member.
		 * @see InitContrainer()
		 */
		virtual bool InitContrainer() = 0;
	public:
		WBaseContrainer(){ }
		virtual ~WBaseContrainer(){ }
	};

// 	template <unsigned int num,typename T>
	class WContrainer : public WBaseContrainer
	{
		//fstream ofs;
		FILE* m_pFile;
		boost::shared_ptr<MYSQL> sql;       //MYSQL句柄  智能指针
		//shared_ptr的作用有如同指针，但会记录有多少个shared_ptrs共同指向一个对象。这便是所谓的引用计数（reference counting）。
		//一旦最后一个这样的指针被销毁，也就是一旦某个对象的引用计数变为0，这个对象会被自动删除。
		//这在非环形数据结构中防止资源泄露很有帮助。 
		UserInfo info;                      //用户信息

		friend class WManage;
	public:
		WContrainer():sql(new MYSQL)
		{
			//ofs.open("pool.log",fstream::in | fstream::out | fstream::app);
			m_pFile = _tfopen("pool.log", _T("wb"));
		}

		/* **
		 * @brief func comment
		 * @see WContrainer
		 * @param user [the user of databases]
		 * @param name [the name of databases]
		 * @param pwd [the password of databases]
		 */
		WContrainer(string user,string name,string pwd):sql(new MYSQL)
		{
			//ofs.open("pool.log",fstream::in | fstream::out | fstream::app);
			m_pFile = _tfopen("pool.log", _T("wb"));

			InitValue(user,name,pwd);
		}

		WContrainer(UserInfo ifo):sql(new MYSQL)
		{
			//ofs.open("pool.log",fstream::in | fstream::out | fstream::app);
			m_pFile = _tfopen("pool.log", _T("wb"));
			InitValue(ifo.DBUser,ifo.DBName,ifo.DBPwd);
		}
		
		virtual ~WContrainer() { /*ofs.close();*/fclose(m_pFile); }

		/* **
		 * @brief func comment
		 * @param no param
		 * @return true if success or failed
		 */
		//初始化连接……连接数据库
		bool InitContrainer()
		{			
			for(int i = 0 ;i < INIT_NUM;i ++)
			{
				Connection con;
				MYSQL* tempmysql = sql.get();
				
				if(!(tempmysql = ConnectData(tempmysql)))  //连接数据库
					return false;

				assert(tempmysql);
				con.sql = tempmysql;
				con.state = true;
				con.id = i;
				
				Contrain.push_back(con);
				
				//ofs<< "第"<<++i<<"个连接正确创建"<<endl;
				//fwrite("第", 2, 1, m_pFile);
			}
			current = INIT_NUM;
			return true;
		}
	private: //@function
		MYSQL * ConnectData(MYSQL * ms)
		{
			try
			{
				if(!(ms = mysql_init(NULL)))
				{
					//ofs << "mysql init failed,please again!" << endl;
					return NULL;
				}

				if(!(ms = mysql_real_connect(ms,"localhost",
						info.DBUser.c_str(),info.DBPwd.c_str(),
						info.DBName.c_str(),0,NULL,0)))
				{
					//ofs << "mysql connected failed,please again"<<endl;
					return NULL;
				}
				//ofs <<__DATE__<<"/"<<__TIME__<<" It is success to connect to mysql" << endl;
			}catch(exception & e)
			{
				cerr << "exception caught: " << e.what() << endl;
			}
			return ms;			
		}
		
		//初始化用户信息
		inline void InitValue(string user,string name,string pwd)
		{
			info.DBUser = user;
			info.DBName = name;
			info.DBPwd = pwd;
		}
	private://@var
		MyVector Contrain;
		uint current;          //当前连接数
	};

}
#endif