// CNetMySql.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include "MySql.h"
#include "NetMySql.h"


//һ���ֶνṹ
typedef struct _FIELDS
{
	char field_name[DEFAULT_FIELD_SIZE];//�ֶ���
	void * filed_value; //�ֶ�ֵ��ָ��
	unsigned value_len; //�ֶεĳ��ȣ�ֻ�������ַ�������
}FIELDS,*PFIELDS;

//һ�����ݵĽṹ����
typedef struct _ROWS{
    FIELDS fields[DEFAULT_FIELD];   //�ֶ�,���ݲ�ͬ��Ӧ�ó�������޸Ĵ�ֵ
	int cnt;             //���ٸ��ֶΣ�ָʾfield��Ԫ�ص���Ч����
}ROWS,*PROWS;


CNetMySql::CNetMySql()
{
	result=NULL;
	memset(m_host,0,32);
	memset(m_name,0,32);
	memset(m_pwd,0,32);
	m_port=0;
	mysql_init(&myData);

	//�������ӳ�ʱ
	unsigned timeout=5*60;
	mysql_options(&myData, MYSQL_OPT_CONNECT_TIMEOUT, (char *)&timeout);
	
	//����������
	my_bool reconnect=TRUE;
    mysql_options(&myData, MYSQL_OPT_RECONNECT, (char *)&reconnect);

	//��ʼ��HashTable��СΪ100
	//res.InitHashTable(100);
}
CNetMySql::~CNetMySql()
{
	Close();
}

//����MySql������
BOOL CNetMySql::Connect(char * host,char * name,char *pwd,char * db, unsigned port)
{
	BOOL bRet=TRUE;
	strcpy_s(m_host,host);
	strcpy_s(m_name,name);
	strcpy_s(m_pwd,pwd);
	m_port=port;
	do
	{
		//���ӷ�����
		if(mysql_real_connect(&myData,host,name,pwd,db,port,NULL,0)==NULL)
		{
			//TRACE(TEXT("TIPTOP-mysql_real_connect���ݿ�����ʧ�ܣ�������Ϣ:%s\r\n"), mysql_error(&myData));
			MessageBoxA(NULL, mysql_error(&myData), "���ݿ����Ӵ�����Ϣ", MB_OK);
			bRet=FALSE;
			break;
		}
		
		//���������ַ�����Ĭ��Ϊgb2312
		if(mysql_set_character_set(&myData, "gb2312"))
		{
			bRet=FALSE;
			//��������ַ������ɹ�����ر�����
			TRACE(TEXT("TIPTOP-mysql_set_character_set���������ַ���ʧ�ܣ�������Ϣ:%s\r\n"), mysql_error(&myData));
			Close();
			break;
		}
		//�������������֧��
		mysql_set_server_option(&myData, MYSQL_OPTION_MULTI_STATEMENTS_ON);
	}while(0);
	TRACE(TEXT("TIPTOP-���ݿ����ӳɹ���mysql_error��Ϣ:%s\r\n"), mysql_error(&myData));
	return bRet;
}

//ִ�зǲ�ѯ���
BOOL CNetMySql::ExecuteNoQuery(char ** sqlArray,int count)
{
	BOOL uRet=TRUE;
	//�����Զ��ύģʽ
     mysql_autocommit(&myData, 0); 
	 for(int i=0;i<count;i++)
	 {
		 mysql_query(&myData,sqlArray[i]);
		 //��ѯ��Ӱ�������
		 //uRet=uRet+(unsigned)mysql_affected_rows(&myData); 
	 }
	 //���û�гɹ�
	 if(mysql_commit(&myData))
	 {
		 //�ع�����res
		 mysql_rollback(&myData);
		 uRet=FALSE;
	 }
     //�ָ��Զ��ύģʽ
     mysql_autocommit(&myData, 0); 
	 return uRet;
}
//ִ�в�ѯ���
unsigned CNetMySql::ExecuteQuery(char * sql)
{
	unsigned uLine=0,uField=0;
	PROWS rows;
	MYSQL_ROW row;
	MYSQL_FIELD *pField;
    //�ͷ�ǰһ�εĽ����
	FreeResult();
	//��ѯ
	if (mysql_query(&myData,sql))
	{
		MessageBoxA(NULL, mysql_error(&myData), "��ѯ����", MB_OK);
		return uLine;
	}
    //�洢�����
    result=mysql_store_result(&myData);
    //��������Ϊ�գ��򷵻�0
    if(result==NULL)return 0;
    //�������
    uLine=(unsigned)mysql_num_rows(result);
    //����ֶ���
    uField=(unsigned)mysql_num_fields(result);
   
   //�������
   for(int i=0;i<(int)uLine;i++)
   {
	   //Ϊ�洢һ�����ݷ����ڴ�
      rows=(PROWS)malloc(sizeof(ROWS));
	  //һ���ж��ٸ��ֶ�
	  rows->cnt=uField;
	  //���һ������
	  row=mysql_fetch_row(result);
	  for(int j=0;j<(int)uField;j++)
	  {
		 //��ø��е������Ϣ
		  pField=mysql_fetch_field_direct(result, j);
		  //������
		  strcpy_s(rows->fields[j].field_name,pField->name);
		  //��ֵ
		  rows->fields[j].filed_value=row[j];
		  //�п���
		  rows->fields[j].value_len=pField->max_length;
	  }
	  //��һ�����ݴ洢��HashTable��
	  //res.SetAt(i,(void *)rows);
	  res.Add((void *)rows);
   }
   //��������
   return uLine;
}

//�ͷŲ�ѯ�Ľ����
void CNetMySql::FreeResult()
{
	int len=0;
	PROWS rows=NULL;

	//len=res.GetCount();
	len = res.GetCount();
	//�ͷŽ�������ڴ�
	for(int i=0;i<len;i++)
	{
		//rows=(PROWS)res[i];
		rows = (PROWS)res.GetAt(i);
		if(rows!=NULL)
		{
			free(rows);
			rows=NULL;
		}
	}
   //���HashTable
	res.RemoveAll();

	//�ͷ�result
    if(result!=NULL)
	{
	   mysql_free_result(result);
	}
	result=NULL;
}

//ִ�д�������SQL��亯��
BOOL CNetMySql::ExecutePrepare(char * sql)
{
	BOOL bRet=TRUE;
	do
	{
		//��ʼ�����
		myStmt=mysql_stmt_init(&myData);
		if (!myStmt)
		{
			bRet=FALSE;
			break;
		}
		//׼��SQL
		if (mysql_stmt_prepare(myStmt, sql, strlen(sql)))
		{
			bRet=FALSE;
			break;
		}
	}while(0);
	return bRet;
}
BOOL CNetMySql::BindParam(MYSQL_BIND * bind)
{
	BOOL bRet=TRUE;
	if (mysql_stmt_bind_param(myStmt, bind))
	{
       bRet=FALSE;
	}
	return bRet;

}
unsigned CNetMySql::Execute()
{
	unsigned uRet=0;
	do
	{
		if (mysql_stmt_execute(myStmt))
		{
			uRet=0;
			break;
		}
		//�����Ӱ������� 
		uRet=(unsigned)mysql_stmt_affected_rows(myStmt);
	}while(0);
    return uRet;
}
BOOL CNetMySql::ClosePrepare()
{
	BOOL bRet=TRUE;
	//�رվ��
	if (mysql_stmt_close(myStmt))
	{
       bRet=FALSE;
	}
	return bRet;
}

//�ı��û�
BOOL CNetMySql::ChangeUser(char * name,char *pwd,char *db)
{
	BOOL bRet=TRUE;
	//���ʧ�ܣ�����FALSE
	if(mysql_change_user(&myData, name, pwd, db) )
	{
		bRet=FALSE;
	}
	return bRet;
}
//ѡ�����ݿ�
BOOL CNetMySql::SelectDB(char * db)
{
	BOOL bRet=TRUE;
	if(mysql_select_db(&myData, db))
	{
		bRet=FALSE;
	}
	return bRet;
}


//�ر�
void CNetMySql::Close()
{
   //��ս����
    FreeResult();
	//�ر�����
	mysql_close(&myData);
}


//��ȡ��ѯ��ֵ
int CNetMySql::GetIntValue(int row, char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	int iRet=0;
	char * tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			//������ݵ��ַ�����ʾ
			tmp=(char *)rows->fields[i].filed_value;
			//ת��������
			iRet=atoi(tmp);
			break;
		}
	}
	return iRet;
}
int CNetMySql::GetIntValue(int row, int fd_num)
{
    PROWS rows=NULL;
	int len=0,iRet=0;
	char * tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

    //������ֵ��ַ�����ʾ
	tmp=(char *)rows->fields[fd_num].filed_value;
	//ת��������
	iRet=atoi(tmp);
	return iRet;
}

//��÷�����ֵ
float CNetMySql::GetFloatValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	float iRet=0.0;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
			iRet=(float)atof(tmp);
			break;
		}
	}
	return iRet;
}
float CNetMySql::GetFloatValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	float iRet=0.0;
	char * tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

    tmp=(char *)rows->fields[fd_num].filed_value;
    iRet=(float)atof(tmp);
	return iRet;
}

//��ø߾��ȷ�����ֵ
double CNetMySql::GetDoubleValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	double iRet=0.0;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
		    iRet=atof(tmp);
			break;
		}
	}
	return iRet;
}
double CNetMySql::GetDoubleValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	double iRet=0.0;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

	tmp=(char *)rows->fields[fd_num].filed_value;
	iRet=atof(tmp);
	return iRet;
}

//��ò�����ֵ
BOOL CNetMySql::GetBoolValue(int row,char * fd_name)
{
   	PROWS rows=NULL;
	int len=0;
	char * tmp=NULL;
	BOOL iRet=0;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			tmp=(char *)rows->fields[i].filed_value;
			iRet=(BOOL)atoi(tmp);
			break;
		}
	}
	return iRet;
}
BOOL CNetMySql::GetBoolValue(int row,int fd_num)
{
	PROWS rows=NULL;
	int len=0;
	BOOL iRet=0;
	char * tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

	tmp=(char *)rows->fields[fd_num].filed_value;
	iRet=(BOOL)atoi(tmp);
	return iRet;
}

char * CNetMySql::GetStringValue(int row,char * fd_name,unsigned * length)
{
	PROWS rows=NULL;
	int len=0;
	char *p_Tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			*length=(unsigned)(rows->fields[i].value_len);
			p_Tmp=(char *)(rows->fields[i].filed_value);
			break;
		}
	}
	return p_Tmp;
}
char * CNetMySql::GetStringValue(int row,int fd_num, unsigned * length)
{
	PROWS rows=NULL;
	int len=0;
	char * p_Tmp=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

	//����г���
	*length=(unsigned)rows->fields[fd_num].value_len;
	//ֱ�ӷ��ش��е�ֵ
	p_Tmp=(char *)(rows->fields[fd_num].filed_value);
	return p_Tmp;
}

//�������ʱ��������
char * CNetMySql::GetDataTimeValue(int row,char * fd_name)
{
	PROWS rows=NULL;
	int len=0;
	char * pTime=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
			pTime=(char *)(rows->fields[i].filed_value);
			break;
		}
	}
	return pTime;
}
char * CNetMySql::GetDataTimeValue(int row, int fd_num)
{
   	PROWS rows=NULL;
	int len=0;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);

	//ֱ�ӷ��ش��е�ֵ
	return (char *)(rows->fields[fd_num].filed_value);
}

//��ö��������ݵĻ�����
int CNetMySql::GetBinaryValue(int row,char * fd_name,char * pBuf)
{
	PROWS rows=NULL;
	int len=0;
	char * p=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return 0;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
	//������
	for(int i=0;i<rows->cnt;i++)
	{
		if(!strcmp(rows->fields[i].field_name,fd_name))
		{
		    len=(int)rows->fields[i].value_len;
			if(pBuf==NULL)
				break;
			p=(char *)(rows->fields[i].filed_value);
			memcpy(pBuf,p,len);
			break;
		}
	}
	return len;
}
int CNetMySql::GetBinaryValue(int row,int fd_num,char * pBuf)
{
	PROWS rows=NULL;
	int len=0;
	char * p=NULL;
	len=res.GetCount();
	//����кų����˷�Χ���򷵻�0
	if(row>=len)return NULL;
	//rows=(PROWS)res[row];
	rows = (PROWS)res.GetAt(row);
    
	len=rows->fields[fd_num].value_len;

	if(pBuf==NULL)
		return len;
  
	//ֱ�ӷ��ش��е�ֵ
	 p=(char *)(rows->fields[fd_num].filed_value);
	 memcpy(pBuf,p,len);
	 return len;
}