// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

void f(int i)
{
	int iAddr = (int)&i;
	int iTem2 = (int)(&i - 1);
	int iTem = *(&i - 1);
	*(&i - 1) += 3;
	int iTem1 = *(&i - 1);
}

int main2()
{
	register int i = 1;

	int iLen = sizeof(i);
	int iAddr = (int)&i;
	int iAddr1 = iAddr-1;
	int iaddr2 = iAddr1 + 3;
	iAddr1 += 3;
	f(i);
	++i;

	int iAddr3 = (int)&i;
	printf("%d\n", i);
	return 1;
}


struct A
{
	int num;
};

struct B
{
	int num;
	char type;
	int age;
};

int main()
{
	//struct A a;
	//a.num=1;

	//char* tmp1=(char *)(&(a.num));
	//tmp1=tmp1+4;
	//*tmp1='a';
	//int *tmp2=(int *)(&(a.num));
	//tmp2=tmp2+2;
	//*tmp2=100;

	//struct B *b=(struct B *)(&a);

	//printf(" b->num=%d  b->type=%c  b->age=%d \n",b->num,b->type,b->age);

	//struct B c;
	//c.num = 1;
	//c.type = 'a';
	//c.age = 100;

	//tmp1=(char *)(&(c.num));


	//printf(" b->num=%d  b->type=%c  b->age=%d \n",c.num,c.type,c.age);

	struct A a;  
	struct B *b=(struct B *)(&a);  
	b->num = 1;
	b->type = 'a';
	b->age = 100;
	printf(" b->num=%d  b->type=%c  b->age=%d \n",b->num,b->type,b->age);
	getchar();
	return 1;
}