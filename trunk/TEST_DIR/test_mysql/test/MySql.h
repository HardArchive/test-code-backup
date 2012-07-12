#pragma once

#include <WinSock2.h> //��windows��
#include <mysql.h>
#pragma comment(lib,"libmysql.lib") //unix�¿ɾ�̬����libmysqlclient.a

#define DEFAULT_FIELD        16  //����Ҫ�����Ŀ��У����ݱ�������ֶ���������student����5���ֶΣ�
//�ֱ���name,old,sex,profession,class����teacher����10���ֶΣ���
//��ֵӦ��10���������Ϊ����ֶ���������̫���ռ�÷ǳ�����ڴ档

#define DEFAULT_FIELD_SIZE   50  //ÿ���ֶ����Ƶ�����ֽ������磺nameΪ4���ֽڣ�professionΪ10���ֽ�
//�˴�Ӧ����10��ע�⣺һ�����ֵ���2���ֽڣ�������ʹ�ú�����Ϊ�ֶ�����
//ͬ������ֵҲ��Ӧ�����ù��󣬷���Ҳ��ռ�÷ǳ�����ڴ档

class CMySql
{
public:
	CMySql(void);
	~CMySql(void);
	//CMySql(char* pszHostIp, unsigned uPort, char* pszDbName);     //��ʼ�����캯��
public:
	char m_szhostIP[32];                //���ݿ�IP
	//char m_szDbName[32];                //���ݿ���
	char m_szUserName[32];              //���ݿ������û���
	char m_szpwd[32];                   //���ݿ���������
	unsigned m_uPort;                   //���Ӷ˿�
private:
	MYSQL m_stuMySql;                   //MySql�����Ӿ��
	
	MYSQL_RES* m_pstuResult;//MySql�Ľ����
	//typedef int(* DealData)(void); // ����CallBack ���͵ĺ���ָ��
	
	//CMapWordToPtr m_objRes;//�洢��ѯ�������HashTable
public:
	BOOL Connect(char* pszHostIp, char* pszUserName, char* pszpwd, char* pszDbName, unsigned uPort);    //����CMySql������
	void Close();                      //�ر�����
	//����ɾ�����ݿ�ָ���ֶ�����
	void QueryItem(char* pszTableName, char* pInbuffer);//��ѯ���ݿ�  ���� �ζ�����ѯ������eg��lsid=2 and lssex=1��	
	void ModifyItem(char* pszTableName, char* pInbuffer);//�޸�һ����¼
	void AddItem(char* pszTableName, char* pInbuffer);   //����һ����¼
	void DeleteItem(MYSQL *myData);                      //ɾ��һ����¼
	//ָ��������
	
	virtual int DealReasult() = 0;           //�������ݼ�����  ��
private:		
	void FreeResult();//�ͷŲ�ѯ�Ľ����
};
