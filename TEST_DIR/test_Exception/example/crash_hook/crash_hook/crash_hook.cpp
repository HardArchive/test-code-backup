// (c) 2011 Cristian Adam

#include <iostream>
#include <ctime>
#include <vector>
#include <windows.h>
#include "APIHook.h"

bool showCrashDialog = false;

LONG WINAPI RedirectedSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
    // When the CRT calls SetUnhandledExceptionFilter with NULL parameter
    // our handler will not get removed.
    std::cout << "12345!" << std::endl;
    return 0;
}

LONG WINAPI OurSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
{
    std::cout << "Gotcha!" << std::endl;

    return showCrashDialog ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}

void MemoryAccessCrash()
{
    std::cout << "Normal null pointer crash" << std::endl;

    char *p = 0;
    *p = 5;
}

void OutOfBoundsVectorCrash()
{
    std::cout << "std::vector out of bounds crash!" << std::endl;

    std::vector<int> v;
    v[0] = 5;
}

void AbortCrash()
{
	//terminate();
    std::cout << "Calling Abort" << std::endl;
    abort();
}

void VirtualFunctionCallCrash()
{
    struct B
    {
        B()
        {
            std::cout << "Pure Virtual Function Call crash!" << std::endl;
            Bar();
        }

        virtual void Foo() = 0;

        void Bar()
        {
            Foo();
        }
    };

    struct D: public B
    {
        void Foo()
        {			
        }
    };

    B* b = new D;
    // Just to silence the warning C4101: 'VirtualFunctionCallCrash::B::Foo' : unreferenced local variable
    b->Foo(); 
}

int main()
{
    ::SetUnhandledExceptionFilter(OurSetUnhandledExceptionFilter);

    CAPIHook apiHook("kernel32.dll", "SetUnhandledExceptionFilter", (PROC)RedirectedSetUnhandledExceptionFilter);

    srand(static_cast<unsigned int>(std::time(0)));
    int option = rand() % 4;
	//int option = 2;

    switch (option)
    {
    case 0:
        MemoryAccessCrash();
        break;
    case 1:
        OutOfBoundsVectorCrash();
        break;
    case 2:
        AbortCrash();
        break;
    case 3:
        VirtualFunctionCallCrash();
        break;
    }
	Sleep(5000);

    return 0;
}
