//包含三个主要的类
/*
1. K_Database  数据库
2. k_Record    单条记录
3. k_RecordSet 记录集合
3. k_Field     字段信息

K_mysql.h

*/
#include "mysql.h"
#include <vector>
using namespace std;
////////////////////////////////////

class k_sqlfactory
{
public:
	k_sqlfactory()
	{
		clean();
	}
	vector<string> name_list;
	vector<string> value_list;

	string where;
	string tablename;

	void clean(){name_list.clear (); value_list.clear();where="";tablename=""; };

	/*
	主要功能:插入新的记录
	返回值:0成功 -1 执行查询失败 
	说明:以来 m_value m_field 提供的信息 生成sql 语句再查询
	*/
	void add_value(string newname,string newvalue)
	{
		name_list.push_back (newname);
		value_list.push_back(newvalue);
	};
	string get_add()
	{

		if((tablename.c_str ()=="") ||( this->tablename.empty ()) )
			return "-1";
		string sqltext="INSERT INTO ";
		sqltext+=tablename;
		sqltext+="(";
		int j=(int)this->name_list.size ();
		for(int i=0;i<j;i++)
		{

			sqltext+=name_list[i];
			if(i<j-1) sqltext+=",";
		}
		sqltext+=") VALUES (";

		j=(int)value_list.size ();
		for(int i=0;i<j;i++)
		{
			sqltext+="'";
			sqltext+=value_list[i];
			sqltext+="'";

			if(i<j-1) sqltext+=",";

		}
		sqltext+=")";
		return sqltext;
	}
	string get_edit()
	{
		if((tablename.c_str ()=="") ||( this->tablename.empty ()) ||( this->where.c_str()=="")||(this->where.empty ())) return "-1";
		string sqltext="UPDATE ";
		sqltext+=tablename;
		sqltext+=" SET ";
		int j=(int)this->name_list.size ();
		for(int i=0;i<j;i++)
		{

			sqltext+=name_list[i];
			sqltext+="='";
			sqltext+=value_list[i];
			sqltext+="'";

			if(i<j-1) sqltext+=",";


		}
		if(sqltext.find_last_of (",")==sqltext.length ()) sqltext.erase (sqltext.end ());

		sqltext+=" where (";
		sqltext+=where;
		sqltext+=")";

		return sqltext;

	}
	/*
	主要功能:编辑制定的记录
	返回值:0成功 -1 执行查询失败
	说明:删除 m_where制定的条件 的记录 生成sql 语句再查询
	*/
	string get_del()
	{
		string sqltext;

		if((tablename.c_str ()=="") ||( this->tablename.empty ()) ||( this->where.c_str()=="")||(this->where.empty ()))
			return "-1";
		else
		{
			sqltext ="Delete  from " ;
			sqltext+= tablename;
			sqltext+=" Where " ;
			sqltext+=where;

			return sqltext;

		}

	}
	/*
	主要功能:将二进制的数据转换未合法的sql文本
	失败返回:"-11"
	主要:仅仅限于1m以下的文件
	*/
	string k_SreamRead(string filename,MYSQL* myData)
	{
		char buf[1024*300];//基本缓冲区
		char sql[1024*300*2];//sql缓冲区未实际大小的2
		int fleng;//文件长度
		int realleng;
		FILE* fp;
		fp=fopen(filename.c_str (),"rb");
		if(fp==NULL) return "--1";


		fseek(fp,0,SEEK_END );
		fleng=ftell(fp);

		fseek(fp,0,SEEK_SET);

		realleng=(int)fread(buf,sizeof(char),fleng,fp);


		mysql_real_escape_string(myData,sql,buf,realleng);

		fclose(fp);


		string temp;
		temp=sql;

		return temp;
	};


};
///////////////////////////////////////////////////////////////////////////////////////////

/*
1 字段操作
2 
*/
///////////////////////////////////////////////////////////////////////////////////////////

class K_field
{
public ://属性
	vector<string> m_name;//字段名称
	vector<enum_field_types> m_type;//字段类型


public ://方法
	bool IsNum(int num)//是否是数字
	{
		if(IS_NUM(m_type[num])) return true;
		else return false;
	};
	bool IsNum(string num)//是否是数字
	{
		if(IS_NUM(m_type[GetField_NO(num)])) return true;
		else return false;

	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsDate(int num)//是否是日期
	{
		if(m_type[num]==FIELD_TYPE_DATE ||m_type[num]==FIELD_TYPE_DATETIME  )  return true;
		else return false;
	};
	bool IsDate(string num)//是否是日期
	{
		int temp;
		temp=this->GetField_NO (num);

		if(m_type[temp]==FIELD_TYPE_DATE ||m_type[temp]==FIELD_TYPE_DATETIME  )  return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsChar(int num)//是否是字符
	{
		if(m_type[num]==FIELD_TYPE_STRING  ||m_type[num]==FIELD_TYPE_VAR_STRING || m_type[num]==FIELD_TYPE_CHAR )  return true;
		else return false;


	};
	bool IsChar(string num)//是否是字符
	{
		int temp;
		temp=this->GetField_NO (num);
		if(m_type[temp]==FIELD_TYPE_STRING  ||m_type[temp]==FIELD_TYPE_VAR_STRING || m_type[temp]==FIELD_TYPE_CHAR )  return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsBlob(int num)//是否为二进制数据
	{
		if(IS_BLOB(m_type[num])) return true;
		else return false;
	};
	bool IsBlob(string num)//是否为二进制数据
	{
		if(IS_BLOB(m_type[GetField_NO(num)])) return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	int GetField_NO(string field_name)//得到制定字段的序号
	{   

		for(unsigned int i=0;i<m_name.size ();i++)
		{
			if(!m_name[i].compare (field_name)) return i;

		}

		throw "字段不存在";

	};

};

///////////////////////////////////////////////////////////////////////////////////////////

/*
1 单条记录
2 [int ]操作 [""]操作
*/
///////////////////////////////////////////////////////////////////////////////////////////
class K_record
{
public://属性
	vector<string> m_rs;//结果集
	K_field* m_field;//字段信息 回占用4字节的内存 当记录数很大是回产生性能问题!!!!!!!!!!!!!!!!!!!
public ://方法

	void k_record(K_field* m_f)
	{
		m_field =m_f;
	};
	void SetData(string value)
	{
		m_rs.push_back (value);
	}

	string operator[](string s)//[""]操作
	{
		return m_rs[m_field->GetField_NO (s)];
	};
	string operator[](int num)
	{
		return m_rs[num];
	};
	bool IsNull(int num)//null值判断
	{
		if(m_rs[num].c_str ()=="") return true;
		else return false;

	};
	bool IsNull(string s)
	{
		if(m_rs[m_field->GetField_NO (s)].c_str ()=="") return true;
		else return false;
	};
	//------------------------------------------------------------------------------
	/*
	主要-功能:用 value tab value 的形式 返回结果
	*/
	std::string GetTabText()
	{
		string temp;
		for(unsigned int i=0 ;i<m_rs.size ();i++)
		{
			temp+=m_rs[i];
			if(i<m_rs.size ()-1) temp+="\t";
		}
		return temp;
	};


	//----------------------------------------------------------------------------------
};
///////////////////////////////////////////////////////////////////////////////////////////

/*
1 记录集合
2 [int ]操作 [""]操作
3 表结构操作
4 数据的插入修改
*/
///////////////////////////////////////////////////////////////////////////////////////////

class K_recordSet
{
public ://属性
	vector<K_record> m_s;//记录集
	int m_recordcount;//记录数
	int m_field_num;//字段数
	K_field  m_field;//字段信息
	//----------------------------------------------------//

	MYSQL_RES * res ;
	MYSQL_FIELD * fd ;
	MYSQL_ROW row ;
	MYSQL* myData ;

	//-----------------------------------------------------//
public ://方法
	K_recordSet()
	{

	};

	int GetRecordCount(){return m_recordcount;};//得到记录数目
	int GetFieldNum(){return m_field_num;};//得到字段数目
	K_field* GetField(){return &m_field;};//返回字段
	//---------------------------------------------------------------------------//

	//----------------------------------------------------------------------------------------

	/*
	主要功能:返回制定的结果集
	参数:no. 记录序号

	*/
	K_record operator[](int num)//返回制定序号的记录
	{
		return this->m_s[num];
	};

	//-----------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------------
};


///////////////////////////////////////////////////////////////////////////////////////////

/*
1 负责数据库的连接关闭
2 执行sql 语句(不返回结果)
3 处理事务
*/
///////////////////////////////////////////////////////////////////////////////////////////
class K_Database
{

public :
	K_Database(){};

public ://属性
	MYSQL* myData;//msyql 连接句柄

public ://方法

	//-----------------------------------------------------------------------
	/*
	主要功能:连接数据库
	参数说明:
	1 host 主机ip地址或者时主机名称
	2 user 用户名
	3 passwd 密码
	4 db 欲连接的数据库名称
	5 port 端口号 
	6 uinx 嵌套字
	7 client_flag 客户连接参数
	返回值: 0成功 -1 失败
	*/
	int Connect(string host, string user, string passwd, string db, unsigned int port, unsigned long client_flag)
	{
		if ( (myData = mysql_init((MYSQL*) 0)) &&  mysql_real_connect( myData, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(),port , NULL, client_flag ) )
		{
			//设置连接字符集，默认为gb2312
			if(mysql_set_character_set(myData, "gb2312"))
			{
				//如果设置字符集不成功，则关闭连接
				mysql_close( myData ) ;
				return -1 ;
			}

			if ( mysql_select_db( myData, db.c_str () ) < 0 ) //选择制定的数据库失败
			{
				mysql_close( myData ) ;
				return -1 ;
			}
		}
		else 
		{

			mysql_close( myData ) ;//初始化mysql结构失败
			return -1 ;
		}
		return 0;//成功
	};
	//---------------------------------------------------------------------
	/*
	主要功能:关闭数据库连接
	*/
	void DisConnect( )
	{
		mysql_close( myData ) ;
	};
	//---------------------------------------------------------------------
	/*
	主要功能: 执行非返回结果查询
	参数:
	1 sql 待执行的查询语句
	返回值; n为成功 表示受到影响的行数 -1 为执行失败

	*/
	int ExecQuery(string sql)
	{
		if(!mysql_real_query( myData, sql.c_str () ,(unsigned long)sql.length ()) )
		{
			return (int)mysql_affected_rows(myData) ;//得到受影响的行数
		}
		else      {return -1;}//执行查询失败

	};
	//----------------------------------------------------------------------------------
	/*
	主要功能:测试mysql服务器是否存活
	返回值:0 表示成功 -1 失败
	*/
	int Ping()
	{
		if(! mysql_ping(myData))  return 0;
		else return -1;

	};
	//-----------------------------------------------------------------------
	/*
	主要功能:关闭mysql 服务器
	返回值;0成功 -1 失败
	*/
	int ShutDown()
	{
		if(! mysql_shutdown(myData, SHUTDOWN_DEFAULT)) return 0;
		else return -1;
	};
	//-----------------------------------------------------------------------
	/*
	主要功能:重新启动mysql 服务器
	返回值;0表示成功 -1 表示失败
	*/
	int Reload()
	{
		if(!mysql_reload(myData)) return 0;
		else return -1;
	};
	//-----------------------------------------------------------------------
	// 说明:事务支持InnoDB or BDB表类型
	//-----------------------------------------------------------------------
	/*
	主要功能:开始事务
	返回值:0 表示成功 -1 表示失败
	*/
	int Start_Transaction()
	{
		if(!mysql_real_query( myData, "START TRANSACTION" ,(unsigned long)strlen("START TRANSACTION") ))
		{
			return 0;
		}
		else      {return -1;}//执行查询失败


	}
	//-----------------------------------------------------------------------
	/*
	主要功能:提交事务
	返回值:0 表示成功 -1 表示失败
	*/
	int Commit()
	{

		if(!mysql_real_query( myData, "COMMIT",(unsigned long)strlen("COMMIT") ) )
		{
			return 0;
		}
		else      {return -1;}//执行查询失败

	}
	//--------------------------------------------------------------------------
	/*
	主要功能:回滚事务
	返回值:0 表示成功 -1 表示失败
	*/
	int Rollback()
	{

		if(!mysql_real_query( myData, "ROLLBACK",(unsigned long)strlen("ROLLBACK") ) ) return 0;

		else      {return -1;}//执行查询失败

	};
	//--------------------------------------------------------------------------
	/*主要功能:得到客户信息*/
	string Get_client_info()  { return mysql_get_client_info() ; };
	//--------------------------------------------------------------------------
	/*主要功能:得到客户版本信息*/
	//   int Get_client_version() { return mysql_get_client_version(); };
	//--------------------------------------------------------------------------
	/*主要功能:得到主机信息*/
	string Get_host_info()     { return mysql_get_host_info( myData ) ;};
	//--------------------------------------------------------------------------
	/*主要功能:得到服务器信息*/
	string Get_server_info() { return mysql_get_server_info( myData ) ; };
	//--------------------------------------------------------------------------
	/*主要功能:得到服务器版本信息*/
	//   int  Get_server_version() {return mysql_get_server_version(myData) ; };

	//--------------------------------------------------------------------------
	/*主要功能:得到 当前连接的默认字符集*/
	string  Get_character_set_name() {return mysql_character_set_name(myData) ; };
	//---------------------------------------------------------------------------
	//主要功能返回单值查询 
	string ExecQueryGetSingValue(string sql)
	{
		MYSQL_RES * res ;
		MYSQL_ROW row ;

		if(!mysql_real_query( myData, sql.c_str (),(unsigned long)sql.length () ) )
		{

			res = mysql_store_result( myData ) ;//保存查询结果
			row = mysql_fetch_row( res ) ; 

			return ((row[0]==NULL)||(!strlen(row[0])))?"--1":row[0];


			mysql_free_result( res ) ;


		}
		else      {return "--1";}//执行查询失败
	};
	//------------------------------------------------------------------------------------
	//得到系统时间 
	string GetSysTime()
	{
		return ExecQueryGetSingValue("select now()");
	}
	;
	//---------------------------------------------------------------------------------
	/*
	主要功能:建立新数据库
	参数:name 为新数据库的名称
	返回:0成功 -1 失败
	*/
	int Create_db(string name)
	{
		string temp ;
		temp="CREATE DATABASE ";
		temp+=name;
		if(!mysql_real_query( myData,temp.c_str () ,(unsigned long)temp.length ()) ) return 0;

		else      {return -1;}//执行查询失败

	};
	//----------------------------------------------------------------------------
	/*
	主要功能:删除制定的数据库
	参数:name 为欲删除数据库的名称
	返回:0成功 -1 失败
	*/
	int Drop_db(string name)
	{
		string temp ;
		temp="DROP DATABASE ";
		temp+=name;
		if(!mysql_real_query( myData,temp.c_str () ,(unsigned long)temp.length ()) ) return 0;

		else      {return -1;}//执行查询失败

	};
	int  ExecQueryGetRecord(string sql,K_recordSet& Record)
	{
		int i,j;

		if ( ! mysql_real_query( myData, sql.c_str () ,(unsigned long)sql.length ()) )//执行查询
		{


			Record.res  = mysql_store_result( myData ) ;//保存查询结果

			i = (int) mysql_num_rows( Record.res ) ; //得到记录数量
			Record.m_recordcount =i;//保存记录数
			j = mysql_num_fields( Record.res ) ;//得到字段数量
			Record.m_field_num =j;//保存字段数


			for (int  x = 0 ; Record.fd = mysql_fetch_field( Record.res ) ; x++ )
			{
				Record.m_field.m_name.push_back (Record.fd->name );
				Record.m_field.m_type .push_back (Record.fd->type );


			}


			//....
			while ( Record.row = mysql_fetch_row( Record.res ) ) 
			{
				K_record temp;

				for ( int k = 0 ; k < j ; k++ )
				{

					if(Record.row[k]==NULL||(!strlen(Record.row[k])))
					{
						temp.SetData ("");
					}
					else
					{

						temp.SetData (Record.row[k]);
					}

				}

				Record.m_s.push_back (temp); //添加新记录
			}
			mysql_free_result( Record.res ) ;
			return (int)Record.m_s.size ();
		}
		else
			return -1 ;//查询执行失败;

	};


};
