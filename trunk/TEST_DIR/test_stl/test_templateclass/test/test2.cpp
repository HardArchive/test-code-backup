
//����������ຯ���Ĺ�Ч(��ͨ���麯��)
//ͨ����Ա����ʵ�� ----�ⷽ�����������⣿

#include "stdafx.h"
#include <iostream>
using namespace std;

class B2;

class A2
{
private:
	B2* b2;
public:
	A2(){};
	A2(B2* b);

	void fun();
};

class B2 : public A2
{
public:
	B2(){};
	void speek()
	{
		printf("OK B2\n");
	}
};

A2::A2(B2* b2)
{
	this->b2 = b2;
}

void A2::fun()
{
	b2->speek();
}

int main2()
{
	A2 a2(new B2);
	a2.fun();

	return 0;
}