// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int fun1(int);

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 12;
	

	//ָ�뺯����ָ��ָ��ĺ�������������һ����������������������ĳһ���͵�ָ�� 
	//���ͱ�ʶ��    *������(������)eg:  int *f(x��y);
	
	//���ǵ�һ�ַ���   ����ָ��ķ���  ע����ָ�뺯��������
	//����ָ�룬��ָ������ָ���������������һ��ָ�����
	//fp��һ��ָ�룬����ָ��һ���������׵�ַ
	int (*fp)(int);                  //fpû������
	//��fp��ֵ��fun1 �����������������ɴ���һ���������׵�ַ����ʱfpָ��fun1�������
	fp=fun1;                         
	//ָ�뺯���ĵ��÷�ʽ��i �ǲ�����
	cout<<"1��fun1:"<<(*fp)(i+1)<<endl;   

	//�ڶ��ַ���  �Զ��庯������
	typedef int (*FUN)(int);
	FUN pFun = fun1;
	cout<<"2��fun1:"<<pFun(i+1)<<endl;
	 
	//���߳���DLL����������ʹ��
}

int fun1(int k)
{
	k=k*2+1;
	return k;
}