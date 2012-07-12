// test.cpp : �������̨Ӧ�ó������ڵ㡣
//
// CMySqlTest.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <stdio.h>
#include "MySql.h"
#include <iostream>

using namespace std;


int main(int argc, char* argv[], char* envp[])
{
	CMySql objMysql;
	objMysql.Connect("10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306);

	return 0;
}

/*
#include <WinSock2.h> //��windows��
#include <mysql.h>
#pragma comment(lib,"libmysql.lib") //unix�¿ɾ�̬����libmysqlclient.a

int main(int argc, char* argv[], char* envp[])
{

	//����mysql_library_init()��mysql_library_end()��Ŀ�����ڣ�ΪMySQL���ṩǡ���ĳ�ʼ���ͽ�������
	mysql_library_init(NULL,NULL,NULL);

	//������ʼ����mysql_real_connect()����Ӧ��MYSQL����
	MYSQL mysql;
	mysql_init(&mysql);   //��ʼ�����ݽṹ
	//mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gb2312");//���ñ��뷽ʽ  

	//�����������������ϵ�MySQL���ݿ����潨�����ӡ�
	//�����ܹ�ִ����Ҫ��ЧMySQL���Ӿ���ṹ���κ�����API����֮ǰ��mysql_real_connect()����ɹ���ɡ�
	//���ݿ�����ӣ���������ݿ���󣬱���/MySQL�ķ����ip���û��������룬���ݿ������̶���������
	if (!mysql_real_connect(&mysql, "10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306, NULL, NULL))
	{
		fprintf(stderr,"%s",mysql_error(&mysql));
		exit(1);
	}
	//�ú�������Ϊ��ǰ��������Ĭ�ϵ��ַ�����
	if (mysql_set_character_set(&mysql,"gb2312"))
	{
		fprintf(stderr,"%s",mysql_error(&mysql));     
		exit(1);
	}


	//���ģʽΪ��1��������autocommitģʽ�����ģʽΪ��0������ֹautocommitģʽ��
	mysql_autocommit(&mysql, 0);

	//ִ���ɡ�Null�ս���ַ�������ѯָ���SQL��ѯ��
	//mysql_query()�������ڰ������������ݵĲ�ѯ��Ӧʹ��mysql_real_query()ȡ����֮
	//�����������ݿ��ܰ����ַ���\0����mysql_query()�Ὣ���ַ�����Ϊ��ѯ�ַ�����������
	//if (mysql_query(&mysql,"insert into `Users`(`username`,`password`)values('����','abc123')"))
	//{
	//	fprintf(stderr,"%s",mysql_error(&mysql));
	//	exit(1);
	//}

	//�ع���ǰ����
	//mysql_rollback(&mysql);

	//�ύ��ǰ����
	//mysql_commit(&mysql);

	if (mysql_query(&mysql,"select * from `leadership`"))
	{
		fprintf(stderr, "select:%s", mysql_error(&mysql));
		exit(1);
	}

	//���������Ľ�������ͻ��ˡ�
	MYSQL_RES *rs = mysql_store_result(&mysql);
	if (rs == NULL)
	{
		fprintf(stderr,"%s",mysql_error(&mysql));
		exit(1);
	}

	//���ؽ�����е�������
	printf("found %u rows.\r\n", mysql_num_rows(rs));
	//printf("found %n rows.\r\n", mysql_num_fields(rs));

	//�ӽ�����л�ȡ��һ��
	MYSQL_ROW row;
	while((row = mysql_fetch_row(rs)) != NULL)
	{
		cout<<row[0]<<"\r;";
		cout<<row[1]<<"\r"<<endl;

	}

	//�ͷŽ����ʹ�õ��ڴ档
	mysql_free_result(rs);

	//�ر�ǰ��򿪵����ӡ�
	mysql_close(&mysql);

	mysql_library_end();

	return 0;
}
*/