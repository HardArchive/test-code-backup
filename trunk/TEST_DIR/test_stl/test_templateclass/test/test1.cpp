// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

//����������ຯ���Ĺ�Ч(��ͨ���麯��)
//ͨ��ģ��ʵ�� 

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