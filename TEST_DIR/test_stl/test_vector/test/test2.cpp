// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class A
{
public:
	char b[256];
	char a[256];//主键
};

bool comT(A a_com)
{
	return  (!strcmp(a_com.a,"02"));      
}

int main2() 
{
	A a;
	strcpy(a.a,"01");
	strcpy(a.b,"b");
	A b;
	strcpy(b.a,"02");
	strcpy(b.b,"bb");
	vector<A> vect;
	vect.push_back(a);
	vect.push_back(b);

	vector<A>::iterator iter = find_if(vect.begin(),vect.end(),&comT);

	if(iter!=vect.end())
	{
		//找到了
	}
	else
	{
		//找不到
	}

	A c;
	c=*iter;
	cout<<c.a<<endl;
	cout<<c.b<<endl;

	getchar();
	return 0;
} 