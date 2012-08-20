// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

//父类调用子类函数的功效(不通过虚函数)
//通过模板实现 

#include <stdio.h>
template <typename T>
class A1
{
public: 
	A1()
	{
		T *pT = (T *) this;
		pT->speek();
	} 
}; 

class B1 : public A1<B1> 
{ 
public: 
	void speek() 
	{
		printf("OK B1\n"); 
	} 
}; 

int main1() 
{
	B1 b1; 
	return 0; 
} 