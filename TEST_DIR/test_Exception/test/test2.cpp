//Version 3:
#include "stdafx.h"
#include <cstdlib>
#include <iostream>

using namespace std;

template<class T>class Stack
{
    T *v;
    int max_size;
    int top;
public:
    class stack_error
    {
    public:
        virtual void error(void) = 0;
    };

    class Underflow: public stack_error       //下溢
    {
    public:
        void error(void)
        {
            cerr << "stack Underflow" << endl;
        }
    };
    class Overflow: public stack_error        //上溢
    {
    public:
        void error(void)
        {
            cerr << "stack Overflow" << endl;
        }
    };
    Stack(int s): max_size(s), top(0)
    {
        v = new T[max_size];   // construct function. determine the size
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

void f2()
{
    Stack<string> ss(0);
    try
    {
        ss.push("Quiz");
        string s = ss.pop();
        ss.pop();
    }
    catch(Stack<string>::stack_error &e)
    {
        e.error();
    }

}

int main2(int argc, char *argv[])
{
    f2();
    system("PAUSE");
    return EXIT_SUCCESS;
}


