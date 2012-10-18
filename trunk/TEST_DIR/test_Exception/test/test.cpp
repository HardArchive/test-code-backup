// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

//version 1
// From: An Overview of the C++ Programming Language

#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

template<class T>class Stack
{
	T *v;
	int max_size;
	int top;
public:
	class Underflow { };  // 下溢
	class Overflow { };   // 上溢
	// construct function. determine the size
	Stack(int s): max_size(s), top(0)
	{
		v = new T[max_size];
	}
	~Stack() {}
	void push(T c)
	{
		if(top == max_size) throw Overflow();
		v[top++] = c;
	}
	T pop()
	{
		if(top == 0) throw Underflow();
		return v[--top];
	}
};

void f()
{
	Stack<string> ss(10);
	try
	{
		ss.push("Quiz");
		string s = ss.pop();
		ss.pop();
	}
	catch(Stack<string>::Overflow)
	{
		cerr << "error: stack overflow" << endl;
	}
	catch(Stack<string>::Underflow)
	{
		cerr << "error: stack underflow" << endl;
	}
}

int main(int argc, char *argv[])
{
	f();
	system("PAUSE");
	return EXIT_SUCCESS;
}
