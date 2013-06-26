// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//
//

//任意的东西！
//
//考虑三个类，A, B, 和 C, 它们没有共同的基类，而我们想把它们存入一个 std::vector.
// 如果它们没有共同基类，看起来我们不得不把它们当成 void* 来保存，对吗？唔，not any more (相关语，没有更多的了)，
// 因为类型 any 没有改变对所存值的类型的依赖。以下代码示范了如何解决这个问题。

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
	//any_cast 让你访问any中存放的值。参数为需要取回值的 any 对象。
	// 如果类型 ValueType 与所存值不符，any 抛出一个 bad_any_cast 异常。
	// 请注意，这个语法有点象 dynamic_cast.
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
		//异常
		// bad_any_cast
		// 当试图将一个any对象转换为该对象所存类型以外的其它类型，将抛出该异常。
		// bad_any_cast 派生自 std::bad_cast. 注意，使用指针参数调用 any_cast 时，将不抛出异常(类似于对指针使用 dynamic_cast 时返回空指针一样)，
		// 反之对引用类型使用 dynamic_cast 则会在失败时抛出异常。
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

    // 我们再来，再加一些别的东西
    store_anything.push_back(std::string("This is fantastic!\r\n"));
    store_anything.push_back(3);
    store_anything.push_back(std::make_pair(true, 7.92));

	//算法类的遍历方法
    std::for_each(
        store_anything.begin(),
        store_anything.end(),
        print_any);
}