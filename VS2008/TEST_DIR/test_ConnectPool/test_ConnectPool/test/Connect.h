
/************************************************************************/
/* 
�������Ա����Ҫ�ĵ� ���ؿ��ļ���ͷ�ļ�
  
	
	  
		
		  
#pragma comment(lib,"../lib/mysql/debug/libmySQL.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma warning (disable:4786)

  
#include <WINSOCK2.H>
#include <windows.h> 
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;
	  
                                                                     */
/************************************************************************/




// Connect.h: interface for the CConnect class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "Muxtex/MyOsMutex.h"
#include "MyMutex.h"
using namespace QNA;

#pragma once
/*****************************************************************/
/*
����			:	CConnect
����			:	����	��	��ȡ	���Ӷ���
���ݳ�Ա		:		
	MYSQL *m_ptrCon;	���Ӷ���
����ӿ�˵��	:
�ṩ���Ӷ��������
		������		�û���		����	���ݿ���	�˿�			
bool Connect(string	host,string	user,string	pwd,string dbname,	WORD port);

�õ�һ�����Ӷ���  
void *GetConObject();						
�ú�������ֵΪ		MYSQL *m_ptrCon;	���Ӷ���

bool DisConnect();	�ж�����
�ⲿ���øú�����Ҫ����ǿת�� MYSQL*
/*****************************************************************/
#include <mysql.h>

class CConnect
{
public:
	CConnect();										//���캯��
	virtual ~CConnect();							//��������
	
	bool Connect(
		string	host,								//����
		string	user,								//�û���
		string	pwd,								//����
		string	dbname,								//���ݿ���
		WORD	port								//�˿ں�
		);											//�������ݿ�
	bool DisConnect();							//�ж�����
	void *GetConObject();						//�õ����Ӷ���
	
private:
	//string	m_sqlerr;
	char    m_szSqlerr[MAX_PATH];
	MYSQL	*m_ptrCon;									//���Ӷ���
	CConnect(const CConnect &tmp);						//��������
	CConnect &operator= (const CConnect &);				//��ֵ�����

};
/************************************************************************/
/*                                                                      
����		:		���ӳصĹ�����		����Ϊ����ģʽ
���ݳ�Ա	:			
	queue<CConnect*>	m_PoolCon		���ӳصĹ������ ���δʹ�õ����Ӷ���
	CCriticalSection	m_Mutex;	    �ٽ�������	�ṩ����ͬ������
	CManagerPool *		m_PtrInstance;	���ݿ����ӳصľ�̬�������
  
����ӿ�˵��	:				
��Ա����	:
	GetInitInstance();          ��ʼ�����ӳع������	
	m_PtrInstance
	AddConObject();             ��ӽ��������	
	CConnect * GetConObject();  ��ȡһ�����Ӷ���
	GetPoolSize();              ��ȡ���ӳ��п������Ӷ���
	Release();                  �ͷ����Ӷ���,���¼�����и����ӳ�ȥ����
					
/************************************************************************/

class CManagerPool  
{
private:
	CManagerPool();
public:
	static CManagerPool &GetInitInstance();   // ��ʼ��Ӧ���ó������ӳ�ʵ��
	void AddConObject(CConnect * conn);       // �����Ӷ���������ӳ�
	CConnect * GetConObject();                // �����ӳػ�ȡ���Ӷ���  
	void Release(CConnect * conn);            // �ͷ�����
	int GetPoolSize();                        // ��ȡ��������
	virtual ~CManagerPool();
	
private:	
	queue<CConnect*>				m_PoolCon;		//���ӳ�����
	CCriticalSection        		m_Mutex;		//�ٽ�������
	bool							m_state;		//�Ƿ�����ṩ���ӷ���	
	static CManagerPool *			m_PtrInstance;	//���ݿ����ӳصĹ������
	
};
/************************************************************************/
/*                                                                      */
/************************************************************************/


//�ֶ��������ֶ����͵Ķ�Ӧ��
enum  filetype_t
{
	ENUM_F_T_CHAR = 1,	//		char ����
	ENUM_F_T_INT,		//		int����
	ENUM_F_T_DATETIME ,	//ʱ��	DATETIME 
	ENUM_F_T_DOUBLE ,	//double
	ENUM_F_T_DEC ,		//dec??? 
	ENUM_F_T_UNKNOWN    //
};
//////////////////////////////////////////////////////////////////////////

struct typeset_t
{
	string			m_name;			//	�ֶ���	
	filetype_t		m_type;			//	�ֶ�����
	WORD			m_length;		//	�ֶγ���
	WORD			m_index;		//	��ѯ�б��е���λ��
	typeset_t();					//	Ĭ�ϵĳ�ʼ������
};
//////////////////////////////////////////////////////////////////////////

typedef vector<string> row_t;

//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*
����		:	COperMysql
����		:   ��ȡһ�����Ӷ��� Ȼ����˽���������Ӧ�� ���ݿ����(���,ɾ��,�޸�,��ѯ)
��Ҫ���ݳ�Ա˵��:
MYSQL		*m_PtrCon;					//ָ��  mysql ������ָ��
MYSQL_RES	*m_PtrRes;					//ָ��  mysql �Ĳ�ѯ���ݼ�
vector<typeset_t>		m_Fieldtype;	//�ֶ���Ϣ��

�ṩ����ӿ�˵��:
��Ա����:
1: bool SetTransAction(CConnect * conn);	//�������Ӷ���
����˵��:
	CConnect * conn	�ӹ�����л�ȡΪʹ�õ����Ӷ���;
	��ֵ��MYSQL *m_PtrCon	���ҳ������Ӳ���	
2: bool Exec(string &ddl);	
3: bool Select(string &dml);
4: bool Commit();
5: bool RollBack();
                                                                    
/************************************************************************/
class COperMysql 
{
public:
	COperMysql();
	virtual ~COperMysql();	
	
public:	
	DWORD RowCount();											//�õ���ѯ��¼����
	DWORD GetIncreaseID();										//�õ������������к�
	bool SetTransAction(CConnect * conn);						//�������Ӷ���
	bool GetStatus();											//�õ���ǰִ��״̬
	char* What();												//����ԭ��	
	bool Exec(string &ddl);										//ִ�����ݶ�������(DDL)�����
	bool Select(string &dml);								    //ִ�����ݲ�������(DML)�����
	bool Commit();												//�����ύ
	bool RollBack();											//����ع�

	//ִ�����ݲ�������(DML)�����
	virtual bool Query(const std::string &dml);

	filetype_t SetFieldType(enum_field_types fieldtype);
								
	string GetItemString(DWORD row,WORD index);					//�õ�ָ����ĳ���ֶε��ַ�������ֵ
	string GetItemString(DWORD row,string &fieldname);
	
	float GetItemFloat(DWORD row,WORD index);					//�õ�ָ����ĳ���ֶε���ֵ
	float GetItemFloat(DWORD row,string &fieldname);

	long GetItemLong(DWORD row,WORD index);						//�õ�ָ����ĳ���ֶε�����ֵ
	long GetItemLong(DWORD row,string &fieldname);
	

private:	
	void Clear();											//�����ʱ����
	void SetIncreaseID(long id);							//�����������к�
	row_t * FindRow(DWORD findrow);							//�ҵ���
	
	WORD GetFieldIndex(string &fieldname);					//�ҵ���Ӧ�������
	string GetItemValue(DWORD row,WORD index);
	string GetItemValue(DWORD row,string &fieldname);
	
private:	
	MYSQL					*m_PtrCon;					//ָ��  mysql ������ָ��
	MYSQL_RES				*m_PtrRes;					//ָ��  mysql �Ĳ�ѯ���ݼ�
	WORD					m_Row;							//����Ӱ��ļ�¼��
	//string					m_Err;							//����ԭ��	
	char                    m_szErr[MAX_PATH];               //����ԭ��
	DWORD					m_SeqID;						//�µ����������к�
	bool					m_Commit;						//�����ύģʽ
	// 	my_bool					auto_mode;
	bool					m_State;						//ȡ����Ϣ״̬
	vector<typeset_t>		m_Fieldtype;					//�ֶ���Ϣ��
	vector<row_t>			m_Recordset;
};


/************************************************************************/
/*                                                                      */
/************************************************************************/
//�����ӳػ�ȡһ������
//����֮�����Զ��ͷ�����
class CSqlClient
{
private:
	CConnect * m_PtrCon;
public:
	CSqlClient();
	
	~CSqlClient();	
	
	CConnect *GetConnect();	
};

/************************************************************************/
/*                                                                      */
/************************************************************************/

