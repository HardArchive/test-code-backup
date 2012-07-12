//����������Ҫ����
/*
1. K_Database  ���ݿ�
2. k_Record    ������¼
3. k_RecordSet ��¼����
3. k_Field     �ֶ���Ϣ

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
	��Ҫ����:�����µļ�¼
	����ֵ:0�ɹ� -1 ִ�в�ѯʧ�� 
	˵��:���� m_value m_field �ṩ����Ϣ ����sql ����ٲ�ѯ
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
	��Ҫ����:�༭�ƶ��ļ�¼
	����ֵ:0�ɹ� -1 ִ�в�ѯʧ��
	˵��:ɾ�� m_where�ƶ������� �ļ�¼ ����sql ����ٲ�ѯ
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
	��Ҫ����:�������Ƶ�����ת��δ�Ϸ���sql�ı�
	ʧ�ܷ���:"-11"
	��Ҫ:��������1m���µ��ļ�
	*/
	string k_SreamRead(string filename,MYSQL* myData)
	{
		char buf[1024*300];//����������
		char sql[1024*300*2];//sql������δʵ�ʴ�С��2
		int fleng;//�ļ�����
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
1 �ֶβ���
2 
*/
///////////////////////////////////////////////////////////////////////////////////////////

class K_field
{
public ://����
	vector<string> m_name;//�ֶ�����
	vector<enum_field_types> m_type;//�ֶ�����


public ://����
	bool IsNum(int num)//�Ƿ�������
	{
		if(IS_NUM(m_type[num])) return true;
		else return false;
	};
	bool IsNum(string num)//�Ƿ�������
	{
		if(IS_NUM(m_type[GetField_NO(num)])) return true;
		else return false;

	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsDate(int num)//�Ƿ�������
	{
		if(m_type[num]==FIELD_TYPE_DATE ||m_type[num]==FIELD_TYPE_DATETIME  )  return true;
		else return false;
	};
	bool IsDate(string num)//�Ƿ�������
	{
		int temp;
		temp=this->GetField_NO (num);

		if(m_type[temp]==FIELD_TYPE_DATE ||m_type[temp]==FIELD_TYPE_DATETIME  )  return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsChar(int num)//�Ƿ����ַ�
	{
		if(m_type[num]==FIELD_TYPE_STRING  ||m_type[num]==FIELD_TYPE_VAR_STRING || m_type[num]==FIELD_TYPE_CHAR )  return true;
		else return false;


	};
	bool IsChar(string num)//�Ƿ����ַ�
	{
		int temp;
		temp=this->GetField_NO (num);
		if(m_type[temp]==FIELD_TYPE_STRING  ||m_type[temp]==FIELD_TYPE_VAR_STRING || m_type[temp]==FIELD_TYPE_CHAR )  return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	bool IsBlob(int num)//�Ƿ�Ϊ����������
	{
		if(IS_BLOB(m_type[num])) return true;
		else return false;
	};
	bool IsBlob(string num)//�Ƿ�Ϊ����������
	{
		if(IS_BLOB(m_type[GetField_NO(num)])) return true;
		else return false;
	};
	//-----------------------------------------------------------------------------------------------------------
	int GetField_NO(string field_name)//�õ��ƶ��ֶε����
	{   

		for(unsigned int i=0;i<m_name.size ();i++)
		{
			if(!m_name[i].compare (field_name)) return i;

		}

		throw "�ֶβ�����";

	};

};

///////////////////////////////////////////////////////////////////////////////////////////

/*
1 ������¼
2 [int ]���� [""]����
*/
///////////////////////////////////////////////////////////////////////////////////////////
class K_record
{
public://����
	vector<string> m_rs;//�����
	K_field* m_field;//�ֶ���Ϣ ��ռ��4�ֽڵ��ڴ� ����¼���ܴ��ǻز�����������!!!!!!!!!!!!!!!!!!!
public ://����

	void k_record(K_field* m_f)
	{
		m_field =m_f;
	};
	void SetData(string value)
	{
		m_rs.push_back (value);
	}

	string operator[](string s)//[""]����
	{
		return m_rs[m_field->GetField_NO (s)];
	};
	string operator[](int num)
	{
		return m_rs[num];
	};
	bool IsNull(int num)//nullֵ�ж�
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
	��Ҫ-����:�� value tab value ����ʽ ���ؽ��
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
1 ��¼����
2 [int ]���� [""]����
3 ��ṹ����
4 ���ݵĲ����޸�
*/
///////////////////////////////////////////////////////////////////////////////////////////

class K_recordSet
{
public ://����
	vector<K_record> m_s;//��¼��
	int m_recordcount;//��¼��
	int m_field_num;//�ֶ���
	K_field  m_field;//�ֶ���Ϣ
	//----------------------------------------------------//

	MYSQL_RES * res ;
	MYSQL_FIELD * fd ;
	MYSQL_ROW row ;
	MYSQL* myData ;

	//-----------------------------------------------------//
public ://����
	K_recordSet()
	{

	};

	int GetRecordCount(){return m_recordcount;};//�õ���¼��Ŀ
	int GetFieldNum(){return m_field_num;};//�õ��ֶ���Ŀ
	K_field* GetField(){return &m_field;};//�����ֶ�
	//---------------------------------------------------------------------------//

	//----------------------------------------------------------------------------------------

	/*
	��Ҫ����:�����ƶ��Ľ����
	����:no. ��¼���

	*/
	K_record operator[](int num)//�����ƶ���ŵļ�¼
	{
		return this->m_s[num];
	};

	//-----------------------------------------------------------------------------------------



	//--------------------------------------------------------------------------------------------
};


///////////////////////////////////////////////////////////////////////////////////////////

/*
1 �������ݿ�����ӹر�
2 ִ��sql ���(�����ؽ��)
3 ��������
*/
///////////////////////////////////////////////////////////////////////////////////////////
class K_Database
{

public :
	K_Database(){};

public ://����
	MYSQL* myData;//msyql ���Ӿ��

public ://����

	//-----------------------------------------------------------------------
	/*
	��Ҫ����:�������ݿ�
	����˵��:
	1 host ����ip��ַ����ʱ��������
	2 user �û���
	3 passwd ����
	4 db �����ӵ����ݿ�����
	5 port �˿ں� 
	6 uinx Ƕ����
	7 client_flag �ͻ����Ӳ���
	����ֵ: 0�ɹ� -1 ʧ��
	*/
	int Connect(string host, string user, string passwd, string db, unsigned int port, unsigned long client_flag)
	{
		if ( (myData = mysql_init((MYSQL*) 0)) &&  mysql_real_connect( myData, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(),port , NULL, client_flag ) )
		{
			//���������ַ�����Ĭ��Ϊgb2312
			if(mysql_set_character_set(myData, "gb2312"))
			{
				//��������ַ������ɹ�����ر�����
				mysql_close( myData ) ;
				return -1 ;
			}

			if ( mysql_select_db( myData, db.c_str () ) < 0 ) //ѡ���ƶ������ݿ�ʧ��
			{
				mysql_close( myData ) ;
				return -1 ;
			}
		}
		else 
		{

			mysql_close( myData ) ;//��ʼ��mysql�ṹʧ��
			return -1 ;
		}
		return 0;//�ɹ�
	};
	//---------------------------------------------------------------------
	/*
	��Ҫ����:�ر����ݿ�����
	*/
	void DisConnect( )
	{
		mysql_close( myData ) ;
	};
	//---------------------------------------------------------------------
	/*
	��Ҫ����: ִ�зǷ��ؽ����ѯ
	����:
	1 sql ��ִ�еĲ�ѯ���
	����ֵ; nΪ�ɹ� ��ʾ�ܵ�Ӱ������� -1 Ϊִ��ʧ��

	*/
	int ExecQuery(string sql)
	{
		if(!mysql_real_query( myData, sql.c_str () ,(unsigned long)sql.length ()) )
		{
			return (int)mysql_affected_rows(myData) ;//�õ���Ӱ�������
		}
		else      {return -1;}//ִ�в�ѯʧ��

	};
	//----------------------------------------------------------------------------------
	/*
	��Ҫ����:����mysql�������Ƿ���
	����ֵ:0 ��ʾ�ɹ� -1 ʧ��
	*/
	int Ping()
	{
		if(! mysql_ping(myData))  return 0;
		else return -1;

	};
	//-----------------------------------------------------------------------
	/*
	��Ҫ����:�ر�mysql ������
	����ֵ;0�ɹ� -1 ʧ��
	*/
	int ShutDown()
	{
		if(! mysql_shutdown(myData, SHUTDOWN_DEFAULT)) return 0;
		else return -1;
	};
	//-----------------------------------------------------------------------
	/*
	��Ҫ����:��������mysql ������
	����ֵ;0��ʾ�ɹ� -1 ��ʾʧ��
	*/
	int Reload()
	{
		if(!mysql_reload(myData)) return 0;
		else return -1;
	};
	//-----------------------------------------------------------------------
	// ˵��:����֧��InnoDB or BDB������
	//-----------------------------------------------------------------------
	/*
	��Ҫ����:��ʼ����
	����ֵ:0 ��ʾ�ɹ� -1 ��ʾʧ��
	*/
	int Start_Transaction()
	{
		if(!mysql_real_query( myData, "START TRANSACTION" ,(unsigned long)strlen("START TRANSACTION") ))
		{
			return 0;
		}
		else      {return -1;}//ִ�в�ѯʧ��


	}
	//-----------------------------------------------------------------------
	/*
	��Ҫ����:�ύ����
	����ֵ:0 ��ʾ�ɹ� -1 ��ʾʧ��
	*/
	int Commit()
	{

		if(!mysql_real_query( myData, "COMMIT",(unsigned long)strlen("COMMIT") ) )
		{
			return 0;
		}
		else      {return -1;}//ִ�в�ѯʧ��

	}
	//--------------------------------------------------------------------------
	/*
	��Ҫ����:�ع�����
	����ֵ:0 ��ʾ�ɹ� -1 ��ʾʧ��
	*/
	int Rollback()
	{

		if(!mysql_real_query( myData, "ROLLBACK",(unsigned long)strlen("ROLLBACK") ) ) return 0;

		else      {return -1;}//ִ�в�ѯʧ��

	};
	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ��ͻ���Ϣ*/
	string Get_client_info()  { return mysql_get_client_info() ; };
	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ��ͻ��汾��Ϣ*/
	//   int Get_client_version() { return mysql_get_client_version(); };
	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ�������Ϣ*/
	string Get_host_info()     { return mysql_get_host_info( myData ) ;};
	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ���������Ϣ*/
	string Get_server_info() { return mysql_get_server_info( myData ) ; };
	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ��������汾��Ϣ*/
	//   int  Get_server_version() {return mysql_get_server_version(myData) ; };

	//--------------------------------------------------------------------------
	/*��Ҫ����:�õ� ��ǰ���ӵ�Ĭ���ַ���*/
	string  Get_character_set_name() {return mysql_character_set_name(myData) ; };
	//---------------------------------------------------------------------------
	//��Ҫ���ܷ��ص�ֵ��ѯ 
	string ExecQueryGetSingValue(string sql)
	{
		MYSQL_RES * res ;
		MYSQL_ROW row ;

		if(!mysql_real_query( myData, sql.c_str (),(unsigned long)sql.length () ) )
		{

			res = mysql_store_result( myData ) ;//�����ѯ���
			row = mysql_fetch_row( res ) ; 

			return ((row[0]==NULL)||(!strlen(row[0])))?"--1":row[0];


			mysql_free_result( res ) ;


		}
		else      {return "--1";}//ִ�в�ѯʧ��
	};
	//------------------------------------------------------------------------------------
	//�õ�ϵͳʱ�� 
	string GetSysTime()
	{
		return ExecQueryGetSingValue("select now()");
	}
	;
	//---------------------------------------------------------------------------------
	/*
	��Ҫ����:���������ݿ�
	����:name Ϊ�����ݿ������
	����:0�ɹ� -1 ʧ��
	*/
	int Create_db(string name)
	{
		string temp ;
		temp="CREATE DATABASE ";
		temp+=name;
		if(!mysql_real_query( myData,temp.c_str () ,(unsigned long)temp.length ()) ) return 0;

		else      {return -1;}//ִ�в�ѯʧ��

	};
	//----------------------------------------------------------------------------
	/*
	��Ҫ����:ɾ���ƶ������ݿ�
	����:name Ϊ��ɾ�����ݿ������
	����:0�ɹ� -1 ʧ��
	*/
	int Drop_db(string name)
	{
		string temp ;
		temp="DROP DATABASE ";
		temp+=name;
		if(!mysql_real_query( myData,temp.c_str () ,(unsigned long)temp.length ()) ) return 0;

		else      {return -1;}//ִ�в�ѯʧ��

	};
	int  ExecQueryGetRecord(string sql,K_recordSet& Record)
	{
		int i,j;

		if ( ! mysql_real_query( myData, sql.c_str () ,(unsigned long)sql.length ()) )//ִ�в�ѯ
		{


			Record.res  = mysql_store_result( myData ) ;//�����ѯ���

			i = (int) mysql_num_rows( Record.res ) ; //�õ���¼����
			Record.m_recordcount =i;//�����¼��
			j = mysql_num_fields( Record.res ) ;//�õ��ֶ�����
			Record.m_field_num =j;//�����ֶ���


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

				Record.m_s.push_back (temp); //����¼�¼
			}
			mysql_free_result( Record.res ) ;
			return (int)Record.m_s.size ();
		}
		else
			return -1 ;//��ѯִ��ʧ��;

	};


};
