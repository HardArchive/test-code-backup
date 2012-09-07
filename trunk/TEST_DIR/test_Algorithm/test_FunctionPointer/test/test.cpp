// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int fun1(int);

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 12;
	

	//指针函数是指带指针的函数，即本质是一个函数。函数返回类型是某一类型的指针 
	//类型标识符    *函数名(参数表)eg:  int *f(x，y);
	
	//这是第一种方法   函数指针的方法  注意与指针函数的区别
	//函数指针，是指向函数的指针变量，即本质是一个指针变量
	//fp是一个指针，可以指向一个函数的首地址
	int (*fp)(int);                  //fp没有申明
	//给fp赋值，fun1 函数名，函数名即可代表一个函数的首地址，这时fp指向fun1这个函数
	fp=fun1;                         
	//指针函数的调用方式，i 是参数。
	cout<<"1、fun1:"<<(*fp)(i+1)<<endl;   

	//第二种方法  自定义函数类型
	typedef int (*FUN)(int);
	FUN pFun = fun1;
	cout<<"2、fun1:"<<pFun(i+1)<<endl;
	 
	//两者常在DLL导出函数中使用
}

int fun1(int k)
{
	k=k*2+1;
	return k;
}