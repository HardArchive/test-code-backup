//#include "include/mysql.h"
#include <Afxmt.h>
#include <afxsock.h>
#include <mysql.h>

//*************************************************************
//*************************************************************
//**************ʹ��ʱ�������Ҫ�޸���������*******************
//*************************************************************
//*************************************************************
#pragma comment(lib,"libmysql.lib")
//#pragma comment(lib,"lib\\mysqlclient.lib")
//#pragma comment(lib,"lib\\mysys-max.lib")
//#pragma comment(lib,"lib\\mysys-nt.lib")
//#pragma comment(lib,"lib\\regex.lib")
//#pragma comment(lib,"lib\\strings.lib")
//#pragma comment(lib,"lib\\zlib.lib")

#define DEFAULT_FIELD        16  //����Ҫ�����Ŀ��У����ݱ�������ֶ���������student����5���ֶΣ�
                                 //�ֱ���name,old,sex,profession,class����teacher����10���ֶΣ���
                                 //��ֵӦ��10���������Ϊ����ֶ���������̫���ռ�÷ǳ�����ڴ档

#define DEFAULT_FIELD_SIZE   50  //ÿ���ֶ����Ƶ�����ֽ������磺nameΪ4���ֽڣ�professionΪ10���ֽ�
                                 //�˴�Ӧ����10��ע�⣺һ�����ֵ���2���ֽڣ�������ʹ�ú�����Ϊ�ֶ�����
                                 //ͬ������ֵҲ��Ӧ�����ù��󣬷���Ҳ��ռ�÷ǳ�����ڴ档
//**************************************************************
//**************************************************************
//*******************ʹ�û�����ҪMFC****************************
//**************************************************************
//**************************************************************

class CNetMySql
{
private:
	//MySql�����Ӿ��
	MYSQL myData;

	//MySqlԤ������
	MYSQL_STMT *myStmt;

	//MySql�Ľ����
    MYSQL_RES *result;
    //�洢��ѯ�������HashTable
	CMapWordToPtr res;

	char m_pwd[32];
public:
	char m_host[32],m_name[32];
	unsigned m_port;

public:
	CNetMySql();
	~CNetMySql();

	//����MySql������,
	BOOL Connect(char * host,char * name,char *pwd,char * db,unsigned port);

	//ִ�зǲ�ѯ���,���سɹ���ʧ��
	BOOL ExecuteNoQuery(char ** sqlArray,int count);
	//ִ�в�ѯ��䣬���ز�ѯ��������������������Ҫ����FreeResult�ͷŽ��
	//���������FreeResult�������´β�ѯʱ�Զ��ͷ��ϴεĽ����
	unsigned ExecuteQuery(char * sql);
	//�ͷŲ�ѯ�Ľ����
	void FreeResult();
     
	//�ı��û���ʹ��ȴʡdb�����dbΪ�գ���ʹ��
	BOOL ChangeUser(char * name,char *pwd,char *db);

	//�Ե�ǰ�û�����ݣ������������ݿ�
	BOOL SelectDB(char * db);

	//�ر�����
	void Close();

	//ִ�д������ķ�SELECT SQL���
	//�����ĸ������޷���4.1���µİ汾��ʹ��
	BOOL ExecutePrepare(char * sql);
	BOOL BindParam(MYSQL_BIND * bind);
    unsigned Execute();
	BOOL ClosePrepare();


	//���ָ���к�ָ���е�����ֵ
	//���ж���0��ʼ���
        // row     ���к�
        // fd_name ��������
        // fd_num  �����ֶκ�
        // ע��    ��Get**Valueϵ�к����Ĳ���Ҫȷ����ȷ�����򲻱�֤����ֵ����ȷ
	int GetIntValue(int row, char * fd_name);
	int GetIntValue(int row, int fd_num);

	//��÷�����ֵ
	//��0��ʼ���
	float GetFloatValue(int row,char * fd_name);
	float GetFloatValue(int row,int fd_num);

	//��ø߾��ȷ�����ֵ
	//��0��ʼ���
	double GetDoubleValue(int row,char * fd_name);
	double GetDoubleValue(int row,int fd_num);

	//��ò�����ֵ
	//��0��ʼ���
	BOOL GetBoolValue(int row,char * fd_name);
	BOOL GetBoolValue(int row,int fd_num);

	//����ַ���ֵ�������ַ�������
	//��0��ʼ���
	char * GetStringValue(int row,char * fd_name, unsigned * len);
    char * GetStringValue(int row,int fd_num,unsigned * len);

	//�������ʱ�������ݣ����ַ�����ʽ��ʾ
	//��0��ʼ���
	//��ʽ���£�0000-00-00 00:00:00
	char * GetDataTimeValue(int row,char * fd_name);
	char * GetDataTimeValue(int row, int fd_num);

	//��ö��������ݵĻ�����
	//��0��ʼ��ţ������ݸ�pBuf==NULLʱ����û���������
	int GetBinaryValue(int row,char * fd_name,char * pBuf);
	int GetBinaryValue(int row,int fd_num,char * pBuf);
};