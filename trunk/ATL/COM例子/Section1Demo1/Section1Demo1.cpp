// Section1Demo1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;

//Ԥ����interface
#define interface struct

//�ӿ�IX
interface IX
{	
	virtual void Fx1() = 0;
	virtual void Fx2() = 0;
};

//�ӿ�IY
interface IY
{	
	virtual void Fy1() = 0;
	virtual void Fy2() = 0;
};

//���CA
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
	//���������ʵ��
	CA *pA= new CA();


	//��ȡIX�Ľӿ�
	IX *pIX = pA;

	//ʹ��IX�Ľӿ�
	pIX->Fx1();
	pIX->Fx2();

	//��ȡIY�Ľӿ�
	IY *pIY = pA;

	//ʹ��IY�Ľӿ�
	pIY->Fy1();
	pIY->Fy2();

	//�������
	delete pA;
	pA = NULL;

	return 0;
}