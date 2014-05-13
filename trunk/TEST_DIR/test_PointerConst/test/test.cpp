// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "vld.h"
//#include "../vld/vld.h"
char* g_pCSS = "<STYLE type=text/css>table { font-family: 'Courier New', Courier, mono;font-size: 12px;color: #336600;border-top-style: none;border-right-style: dashed; border-right-width: 2px; border-bottom-style: dashed;border-left-style: none;border-bottom-width: 2px; border-left-width: 2px;word-break: break-all;}td { border-top-style: none; border-right-style: none;   border-bottom-style: none;  border-left-style: dashed;  border-left-width: 2px;word-break: break-all;} th { font-family: 'Courier New', Courier, mono;  font-size: 14px;    font-weight: 400;   color: #FFFFFF; background-color: #339933;  border: none; border-left-style: dashed; border-left-width: 2px; }.ztitle {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; mso-style-parent: style0}.z1 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ebebeb; BORDER-BOTTOM: windowtext 0.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}.z2 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #ffd165; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}.z3 {BORDER-RIGHT: medium none; BORDER-TOP: medium none; BORDER-LEFT: medium none; COLOR: #c8c840; BORDER-BOTTOM: windowtext 1.5pt solid; TEXT-ALIGN: center; mso-style-parent: style0}</STYLE>";
int _tmain1(int argc, _TCHAR* argv[])
{
	printf("CSS内容:%s", g_pCSS);
	return 0;
}



#include <iostream>
using namespace std;

class CA;

/*指向类的非静态成员函数的指针*/
typedef int (CA::*pClassFun)(int, int);

/*指向一般函数的指针*/
typedef int (*pGeneralFun)(int, int);

class CA
{
public:

	int Max(int a, int b)
	{
		return a > b ? a : b;
	}

	int Min(int a, int b)
	{
		return a < b ? a : b;
	}

	static int Sum(int a, int b)
	{
		return a + b;
	}

	/*类内部的接口函数，实现对类的非静态成员函数的封装*/
	int Result(pClassFun fun, int a, int b)
	{
		return (this->*fun)(a, b);
	}

};

/*类外部的接口函数，实现对类的非静态成员函数的封装*/
int Result(CA* pA, pClassFun fun, int a, int b)
{
	return (pA->*fun)(a, b);
}

/*类外部的接口函数，实现对类的静态成员函数的封装*/
int GeneralResult(pGeneralFun fun, int a, int b)
{
	return (*fun)(a, b);
}


void main()
{
	CA ca;
	int a = 3;
	int b = 4;

	cout<<"Test nonstatic member function pointer from member function:"<<endl;
	cout<<"The maximum number between a and b is "<<ca.Result(&CA::Max, a, b)<<endl;
	cout<<"The minimum number between a and b is "<<ca.Result(&CA::Min, a, b)<<endl;

	cout<<endl;
	cout<<"Test nonstatic member function pointer from external function:"<<endl;
	cout<<"The maximum number between a and b is "<<Result(&ca, &CA::Max, a, b)<<endl;
	cout<<"The minimum number between a and b is "<<Result(&ca, &CA::Min, a, b)<<endl;

	cout<<endl;
	cout<<"Test static member function pointer: "<<endl;
	cout<<"The sum of a and b is "<<GeneralResult(CA::Sum, a, b)<<endl;
}