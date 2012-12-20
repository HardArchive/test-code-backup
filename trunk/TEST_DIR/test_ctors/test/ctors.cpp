#include "stdafx.h"
#include <iostream>
using namespace std;


//一、	构造函数和析构函数的调用顺序
//	1、	儿子不能继承父亲的构造和析构函数
//	2、	执行顺序:
//		a)	创建父类的对象，构造函数的执行和子类没有关系。
//		b)	创建子类的对象，构造函数的执行顺序：
//			i.	首先执行父类的构造函数
//			ii.	再是成员对象所在类的构造函数
//			iii.最后执行子类的构造函数
//			iv.	析构函数的执行顺序和构造函数相反
//			
//			

//基类
class Base
{
public:
	Base()
	{ cout<<"基类的构造函数执行!"<<endl; }

	~Base()
	{ cout<<"基类的析构函数执行!"<<endl ;}
};

//子类的成员对象类
class Member 
{
public:
	Member()
	{ cout<<"Member类的构造函数"<<endl; }

	~Member()
	{ cout<<"Member类的析构函数"<<endl; }
};

//子类
class Son:public Base
{
private:
	Member m_clsMember;
public:
	Son()
	{ cout<<"子类的构造函数 ！" <<endl;}

	~Son()
	{ cout<<"子类的析构函数 ！"<<endl; }
};

void test1()
{
	Son clsSon;
	cout<<endl<<"#####开始析构……"<<endl;
}

//二、	子类构造函数的格式
//	1、	如果父类中只有无参的构造函数，子类不需要调用父类的构造函数。
//	2、	如果父类中有带参数的构造函数，子类必须要调用父类的构造函数，向父类传递参数。
//	3、	传递参数的格式：子类的构造函数：父类构造函数名(参数列表)
//	4、	一般子类的构造函数带参数的个数=继承下来的成员变量+子类自己成员变量。

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
	{ cout<<"姓名："<<m_szName<<" 年龄："<<m_iage<<endl;}
};

//Student: 成员变量name，age，g
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
		cout<<"姓名："<<m_szName<<" 年龄："<<m_iage
			<<" 成绩："<<m_iResult<<endl;
	}
};

void test2()
{
	Student clsStudent("张三", 33, 89);
	clsStudent.ShowStudent();
}



void main()
{
	test1();
	cout<<endl<<"#####子类构造函数初始化……"<<endl;
	test2();
	system("pause");
}
