// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "boost/any.hpp"
#include "boost/shared_ptr.hpp"

class A
{
public:
    virtual ~A()
    {
        std::cout << "A::~A()\r\n";
    }

    void not_virtual()
    {
        std::cout << "A::not_virtual()\r\n";
    }

    virtual void is_virtual ()
    {
        std::cout << "A:: is_virtual ()\r\n";
    }
};

class B : public A
{
public:

    void not_virtual()
    {
        std::cout << "B::not_virtual()\r\n";
    }

    virtual void is_virtual ()
    {
        std::cout << "B:: is_virtual ()\r\n";
    }
};

void foo(boost::any &a)
{

    std::cout << "\r\n";

    // ��һ�� boost::shared_ptr<A>
    try
    {
        boost::shared_ptr<A> ptr =
            boost::any_cast<boost::shared_ptr<A> >(a);

        std::cout << "This any contained a boost::shared_ptr<A>\r\n";
        ptr-> is_virtual ();
        ptr->not_virtual();
        return;
    }
    catch(boost::bad_any_cast &e) {}

    // ��һ�� boost::shared_ptr<B>
    try
    {
        boost::shared_ptr<B> ptr =
            boost::any_cast<boost::shared_ptr<B> >(a);

        std::cout << "This any contained a boost::shared_ptr<B>\r\n";
        ptr-> is_virtual ();
        ptr->not_virtual();
        return;
    }
    catch(boost::bad_any_cast &e) {}

    // �������������(��һ���ַ���), �������
    std::cout <<
              "The any didn't contain anything that concerns this function!\r\n";
}


int main()
{
    std::cout << "Example of any and shared_ptr\r\n";

    boost::any a1(boost::shared_ptr<A>(new A));
    boost::any a2(std::string("Just a string"));

    {
        boost::any b1(boost::shared_ptr<A>(new B));
        boost::any b2(boost::shared_ptr<B>(new B));
        std::vector<boost::any> vec;
        vec.push_back(a1);
        vec.push_back(a2);
        vec.push_back(b1);
        vec.push_back(b2);

        std::for_each(vec.begin(), vec.end(), foo);
        std::cout << "\r\n";
    }

    std::cout << "any's b1 and b2 have been destroyed which means\r\n"
              "that the shared_ptrs' reference counts became zero\r\n";

}