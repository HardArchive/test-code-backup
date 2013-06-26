// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//
//

//����Ķ�����
//
//���������࣬A, B, �� C, ����û�й�ͬ�Ļ��࣬������������Ǵ���һ�� std::vector.
// �������û�й�ͬ���࣬���������ǲ��ò������ǵ��� void* �����棬������not any more (����û�и������)��
// ��Ϊ���� any û�иı������ֵ�����͵����������´���ʾ������ν��������⡣

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <boost/any.hpp>


class A
{
public:
    void some_function()
    {
        std::cout << "A::some_function()\n";
    }
};

class B
{
public:
    void some_function()
    {
        std::cout << "B::some_function()\n";
    }
};

class C
{
public:
    void some_function()
    {
        std::cout << "C::some_function()\n";
    }
};

void print_any(boost::any &a)
{
	//any_cast �������any�д�ŵ�ֵ������Ϊ��Ҫȡ��ֵ�� any ����
	// ������� ValueType ������ֵ������any �׳�һ�� bad_any_cast �쳣��
	// ��ע�⣬����﷨�е��� dynamic_cast.
    if (A *pA = boost::any_cast<A>(&a))
    {
        pA->some_function();
    }
    else if (B *pB = boost::any_cast<B>(&a))
    {
        pB->some_function();
    }
    else if (C *pC = boost::any_cast<C>(&a))
    {
        pC->some_function();
    }
	else 
	{
		//�쳣
		// bad_any_cast
		// ����ͼ��һ��any����ת��Ϊ�ö�����������������������ͣ����׳����쳣��
		// bad_any_cast ������ std::bad_cast. ע�⣬ʹ��ָ��������� any_cast ʱ�������׳��쳣(�����ڶ�ָ��ʹ�� dynamic_cast ʱ���ؿ�ָ��һ��)��
		// ��֮����������ʹ�� dynamic_cast �����ʧ��ʱ�׳��쳣��
		try
		{
			std::cout << boost::any_cast<std::string>(a) << '\r\n';
		}
		catch(boost::bad_any_cast&) 
		{
			std::cout << "Oops!\r\n";
		}
	}
}

int main()
{
    std::cout << "Example of using any.\n\n";

    std::vector<boost::any> store_anything;

    store_anything.push_back(A());
    store_anything.push_back(B());
    store_anything.push_back(C());

    // �����������ټ�һЩ��Ķ���
    store_anything.push_back(std::string("This is fantastic!\r\n"));
    store_anything.push_back(3);
    store_anything.push_back(std::make_pair(true, 7.92));

	//�㷨��ı�������
    std::for_each(
        store_anything.begin(),
        store_anything.end(),
        print_any);
}