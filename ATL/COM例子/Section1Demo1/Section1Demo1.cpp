// Section1Demo1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

//预定义interface
#define interface struct

//接口IX
interface IX
{	
	virtual void Fx1() = 0;
	virtual void Fx2() = 0;
};

//接口IY
interface IY
{	
	virtual void Fy1() = 0;
	virtual void Fy2() = 0;
};

//组件CA
class CA: public IX, public IY
{
public:
	virtual void Fx1()
	{
		cout << "Fx1" << endl;
	}

	virtual void Fx2()
	{
		cout << "Fx2" << endl;
	}

	virtual void Fy1()
	{
		cout << "Fy1" << endl;
	}

	virtual void Fy2()
	{
		cout << "Fy2" << endl;
	}
};



int _tmain(int argc, _TCHAR* argv[])
{
	//创建组件的实例
	CA *pA= new CA();


	//获取IX的接口
	IX *pIX = pA;

	//使用IX的接口
	pIX->Fx1();
	pIX->Fx2();

	//获取IY的接口
	IY *pIY = pA;

	//使用IY的接口
	pIY->Fy1();
	pIY->Fy2();

	//销毁组件
	delete pA;
	pA = NULL;

	return 0;
}