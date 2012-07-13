// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <python.h> //包含头文件，在c++中嵌入python，这是必须的

int main() 
{ 
	Py_Initialize(); //初始化python
	if (!Py_IsInitialized())  
		return -1;  

	// 添加当前路径
	//把输入的字符串作为Python代码直接运行，返回0
	//表示成功，-1表示有错。大多时候错误都是因为字符串
	//中有语法错误。
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')"); 
	PyRun_SimpleString("print('PyRun_SimpleString函数输出…')");

	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	PyObject * pArg    = NULL; 

	PyObject *pName = NULL;
	
	// 载入名为test2的脚本
	pName = PyString_FromString("test2");
	pModule = PyImport_Import(pName);

	//pModule = PyImport_ImportModule("test2");              //获取test1.py文件句柄
	if (!pModule)
	{  
		printf("Cant open python file!\n");  
		return -1;  
	}  


	pFunc = PyObject_GetAttrString(pModule, "Hello");      //获取Hello函数
	pArg = Py_BuildValue("(s)", "传入函数参数……"); //设置函数参数
	PyEval_CallObject(pFunc, pArg);                        //调用函数

	pFunc = PyObject_GetAttrString(pModule, "Add");     
	pArg = Py_BuildValue("(i, i)", 5, 10);
	PyEval_CallObject(pFunc, pArg);                        //调用函数
	Py_Finalize();                                         //释放资源
	getchar();

	return 0; 
} 
//
//附：Py_BuildValue的使用例子，来自python documentation：
//
//Py_BuildValue("")                        None
//Py_BuildValue("i", 123)                  123
//Py_BuildValue("iii", 123, 456, 789)      (123, 456, 789)
//Py_BuildValue("s", "hello")              'hello'
//Py_BuildValue("ss", "hello", "world")    ('hello', 'world')
//Py_BuildValue("s#", "hello", 4)          'hell'
//Py_BuildValue("()")                      ()
//Py_BuildValue("(i)", 123)                (123,)
//Py_BuildValue("(ii)", 123, 456)          (123, 456)
//Py_BuildValue("(i,i)", 123, 456)         (123, 456)
//Py_BuildValue("[i,i]", 123, 456)         [123, 456]
//Py_BuildValue("{s:i,s:i}", "abc", 123, "def", 456)    {'abc': 123, 'def': 456}
//Py_BuildValue("((ii)(ii)) (ii)",1, 2, 3, 4, 5, 6)          (((1, 2), (3, 4)), (5, 6))

