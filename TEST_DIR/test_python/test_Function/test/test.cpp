// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//


#include <python.h> //包含头文件，在c++中嵌入python，这是必须的
int main() 
{ 
	Py_Initialize(); //初始化python
	
	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	pModule = PyImport_ImportModule("test2"); //获取test1.py文件句柄
	//pFunc   = PyObject_GetAttrString(pModule, "Hello"); //获取Hello函数
	pFunc   = PyObject_GetAttrString(pModule, "you"); 
	PyEval_CallObject(pFunc, NULL);  //调用函数
	Py_Finalize();  //释放资源
	return 0; 
} 