
/*
 * @time : July 23th,2011
 * @author : BoJing .Wang
 * @goal : Achieve a management that managed mysql connections
 *
 */

#ifndef __MANAGE_H
#define __MANAGE_H
#include "stdafx.h"
#include <iostream>
#include "contrainer.h"

using namespace std;

namespace pool
{
	/* **********************************************
	 * @brief class WBaseManage
	 * @see openConnection
	 * @see closeConnection
	 * @see checkConnections
	 * @see checkThis
	 * @see getUsedConnection
	 */
	class WBaseManage
	{
	protected:
		typedef vector<unsigned int> haveCon;
		/* ******************
		 * A method 
		 * @func openConnection [open the connection of this id]
		 * @param no param
		 * @return [connection of MYSQL] 
		 */
		virtual MYSQL * openConnection() = 0;
		/* *******************
		 * A method
		 * @func closeConnection [close the connection of this id]
		 * @param no param
		 */
		virtual void closeConnection(uint id) = 0;
		/* *******************
		 * A method
		 * @func checkConnections [check all Connections]
		 * @param no param
		 * @return [all connection that able to be connected] 
		 */
		virtual haveCon checkConnections() = 0;
		/* *******************
		 * A method 
		 * @func checkThis [check this connection whether used or not ]
		 * @param no param
		 * @return [false if not used or true]
		 */
		virtual bool checkThis(uint id) = 0;
		/* *******************
		 * @func getUsedConnection [get used connection]
		 * @param no param
		 * @return [the id of Used Connection]
		 */
		virtual unsigned int getUsedConnection() = 0;
		/* *******************
		 * @func addConnections [add 2 connections]
		 * @param no param
		 * @return success if return true,or failed
		 */
		virtual bool addConnections() = 0;
	public:
		WBaseManage(){ }
		virtual ~WBaseManage() { }
	};

// 	template <size_t Number, typename T>
	class WManage : public WBaseManage
	{
		WContrainer *Contrainer;
		typedef unsigned int uint;
		
	public:
		WManage(UserInfo ifo):Contrainer(new WContrainer(ifo))
		{
			assert(Contrainer ->InitContrainer());
		}
		virtual ~WManage(){ delete Contrainer; }
		/* *********************
		 * A method 
		 * @
		 */
		//获取可用连接
		MYSQL* openConnection()
		{
			checkAll();
			//Contrainer ->ofs << "open an connection from pool"<<endl;
			uint id = getUsedConnection();  
			return getConnection(id);         
		}
	private:
		void closeConnection(uint id)
		{
			Contrainer ->Contrain[id].state = true;
		}

		haveCon checkConnections()
		{
			haveCon allCons;
			
			for(MyVector::iterator it = Contrainer->Contrain.begin();
				it != Contrainer->Contrain.end();
				it ++)
			{
				if(it ->state)
					allCons.push_back(it ->id);
			}
			return allCons;
		}
		//检查所有连接是否超时，如果超时则关闭
		void checkAll()
		{
			for(MyVector::iterator it = Contrainer->Contrain.begin();
				it != Contrainer->Contrain.end();
				it ++)
				{
					if(checkTimeOut(it->id))
					{
						//Contrainer->ofs<<"第"<<it->id <<"连接已经超时而关闭"<<endl;
						closeConnection(it->id);
					}						
				}
		}
		bool checkTimeOut(uint id)
		{
			return Contrainer ->Contrain[id].time.IsTimeout();
		}
		bool checkThis(uint id)
		{
			return Contrainer->Contrain[id].state;
		}
		//获取状态为true的连接结构ID
		uint getUsedConnection()
		{
			for(MyVector::iterator it = Contrainer->Contrain.begin();
				it != Contrainer->Contrain.end();
				it ++)
				{
					if(it->state)
						return it->id;
					
				}
		} 
		/*  ***
		 * A method
		 * @func getConnection
		 * @param id [an determin id of connection]
		 * @return [return an connection if success ,or NULL be returned]
		 */
		MYSQL * getConnection(uint id)
		{
			Contrainer ->Contrain[id].time.StartTime();
			Contrainer ->Contrain[id].state = false;
			return Contrainer ->Contrain[id].sql;
		}
		bool addConnections()
		{
			for(int i = 0;i < ADD_CONNECTIONS;i ++)
			{
				Connection con;
				MYSQL* tempmysql = Contrainer ->sql.get();

				if(!(tempmysql = Contrainer->ConnectData(tempmysql)))
					return false;

				assert(tempmysql);
				con.sql = tempmysql;
				con.state = true;
				con.id = i + Contrainer ->current + 1;

				Contrainer -> Contrain.push_back(con);
				int iTem = i+1 + Contrainer->current;
				//Contrainer->ofs << "第"<<iTem<<"个连接正确创建 "<<endl;
			}
			Contrainer->current += ADD_CONNECTIONS;
			return true;
		}
	};
}




#endif 