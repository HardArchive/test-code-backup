#include "stdafx.h"
#include <iostream>
using namespace std;


//һ��	���캯�������������ĵ���˳��
//	1��	���Ӳ��ܼ̳и��׵Ĺ������������
//	2��	ִ��˳��:
//		a)	��������Ķ��󣬹��캯����ִ�к�����û�й�ϵ��
//		b)	��������Ķ��󣬹��캯����ִ��˳��
//			i.	����ִ�и���Ĺ��캯��
//			ii.	���ǳ�Ա����������Ĺ��캯��
//			iii.���ִ������Ĺ��캯��
//			iv.	����������ִ��˳��͹��캯���෴
//			
//			

//����
class Base
{
public:
	Base()
	{ cout<<"����Ĺ��캯��ִ��!"<<endl; }

	~Base()
	{ cout<<"�������������ִ��!"<<endl ;}
};

//����ĳ�Ա������
class Member 
{
public:
	Member()
	{ cout<<"Member��Ĺ��캯��"<<endl; }

	~Member()
	{ cout<<"Member�����������"<<endl; }
};

//����
class Son:public Base
{
private:
	Member m_clsMember;
public:
	Son()
	{ cout<<"����Ĺ��캯�� ��" <<endl;}

	~Son()
	{ cout<<"������������� ��"<<endl; }
};

void test1()
{
	Son clsSon;
	cout<<endl<<"#####��ʼ��������"<<endl;
}

//����	���๹�캯���ĸ�ʽ
//	1��	���������ֻ���޲εĹ��캯�������಻��Ҫ���ø���Ĺ��캯����
//	2��	����������д������Ĺ��캯�����������Ҫ���ø���Ĺ��캯�������ഫ�ݲ�����
//	3��	���ݲ����ĸ�ʽ������Ĺ��캯�������๹�캯����(�����б�)
//	4��	һ������Ĺ��캯���������ĸ���=�̳������ĳ�Ա����+�����Լ���Ա������

class Person
{
protected:
	int m_iage;
	char m_szName[128];

public:
	Person(char* pNameInput,int iage)
	{
		strcpy(m_szName, pNameInput);
		m_iage = iage;
	}

	void ShowPerson()
	{ cout<<"������"<<m_szName<<" ���䣺"<<m_iage<<endl;}
};

//Student: ��Ա����name��age��g
class Student:public Person
{
protected:
	int m_iResult;
public:
	Student(char* pNameInput,int iage, int iResult):Person(pNameInput, iage)
	{
		this->m_iResult = iResult;
	}

	void ShowStudent()
	{
		cout<<"������"<<m_szName<<" ���䣺"<<m_iage
			<<" �ɼ���"<<m_iResult<<endl;
	}
};

void test2()
{
	Student clsStudent("����", 33, 89);
	clsStudent.ShowStudent();
}



void main()
{
	test1();
	cout<<endl<<"#####���๹�캯����ʼ������"<<endl;
	test2();
	system("pause");
}
