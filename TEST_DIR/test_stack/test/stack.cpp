// stack.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

const int MAX_SIZE = 5;

class CStack
{
private:
	int list[MAX_SIZE];
	int top;//标记
public:
	CStack();
	void Push(const int a);//入栈
	int Pop();//出栈
	void Clear();//清空栈
	int GetTop();//获取栈顶元素
	bool IsEmpty()const;//判断堆栈是否为空
	bool IsFull()const;//判断堆栈是否满
};

CStack::CStack()
{
	top =0;
}

void CStack::Push(const int a)
{
	if(top == MAX_SIZE)
		return;
	list[top++] = a;
}

int CStack::Pop()
{
	if(top == 0)
	{
		return 0;
	}
	else
		return list[--top];
}
void CStack::Clear()
{
	top =0;
	list[top]=0;
}

int CStack::GetTop()
{
	int tp = top;
	if(top == 0)
	{
		cout<<"stack is already empty!";
		return 0;
	}

	else
		return list[--tp];
}

bool CStack::IsEmpty()const
{
	return top == 0;
}

bool CStack::IsFull()const
{
	return top == MAX_SIZE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	CStack st;
	int list[5]={1,2,3,4,5};
	for(int i=0;i<5;i++)
	{
		st.Push(list[i]);
	}
	for(int i=0;i<5;i++)
	{
		cout <<st.Pop()<<endl;
	}
	for(int i=0;i<5;i++)
	{
		st.Push(list[i]);
	}
	st.Clear();
	if(st.IsEmpty())
		cout<<"stack is empty!";
	return 0;
}

