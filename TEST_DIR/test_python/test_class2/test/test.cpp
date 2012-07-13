// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <python.h> //包含头文件，在c++中嵌入python，这是必须的

int main() 
{ 
	Py_Initialize(); //初始化python

	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	PyObject * pArg    = NULL; 
	PyObject * pClass  = NULL; 
	PyObject * pObject = NULL; 

	pModule = PyImport_ImportModule("test2");               //获取test2.py文件句柄

	pClass  = PyObject_GetAttrString(pModule, "TestClass"); //获取类对象 
	pArg = PyTuple_New(1);                                  //new一个元组，传入size 
	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", "阿毛,你真帅！"));  //set元组的值，第一个参数为元组，第二个为index（从0开始），第三个为value 
	pObject = PyEval_CallObject(pClass, pArg);              //生成一个对象，或者叫作实例 
	
	pFunc = PyObject_GetAttrString(pObject, "printName");   //得到该实例的成员函数 
	PyEval_CallObject(pFunc, NULL);                         //执行该实例的成员函数 


	Py_Finalize();                                         //释放资源
	getchar();

	return 0; 
} 

