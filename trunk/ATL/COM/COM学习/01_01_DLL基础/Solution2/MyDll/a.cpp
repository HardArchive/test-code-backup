#include <iostream>
using namespace std;
//#include "a.h"

__declspec(dllexport) void fun()
{
	cout<<"Hello world"<<endl;
}