#include "stdafx.h"
#include <iostream>
using namespace std;
//����������ຯ���Ĺ�Ч(��ͨ���麯��)
//ʹ���������������������ָ����         �Ҿ��������϶��������  
class A3;
class B3;

union Helper
{
	A3 *pa;
	B3 *pb;
};

class A3
{
public:
	void test();

};
class B3:public A3
{
public:
	void speek()
	{
		cout<<"speek"<<endl;
	}

};

void A3::test()
{
	Helper hp;
	hp.pa = this;
	hp.pb->speek();

}


int main()
{
	A3 a3;
	a3.test();

	return 0;
}