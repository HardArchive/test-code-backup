// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#ifdef WIN32  
//#include "Python.h"   
////#include <python.h>
//#else  
//#include "/usr/src/Python-2.4/Include/Python.h" 
//#endif 
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

/* 

 * test.cpp 

 *  Created on: 2010-12-12 

 *      Author: Handy_Zhou 

 */  

#include <Python.h>  
#include <iostream>  
#include <string>  

//��ӡ�ֵ����
void printDict(PyObject* obj) 
{  
    if (!PyDict_Check(obj))  
        return;  

    PyObject *k, *keys;  

    keys = PyDict_Keys(obj);  

    for (int i = 0; i < PyList_GET_SIZE(keys); i++) 
	{  
        k = PyList_GET_ITEM(keys, i);  
        char* c_name = PyString_AsString(k);  
        printf("%s\n", c_name);  
    }  
}  

int main()
{  
    Py_Initialize();  
    if (!Py_IsInitialized())  
        return -1;  

    PyRun_SimpleString("import sys");  
    PyRun_SimpleString("sys.path.append('./')");  

    //����ģ��  
    PyObject* pModule = PyImport_ImportModule("test1");  
    if (!pModule)
	{  
        printf("Cant open python file!\n");  
        return -1;  
    }  

    //ģ����ֵ��б�  
    PyObject* pDict = PyModule_GetDict(pModule);  
    if (!pDict) 
	{  
        printf("Cant find dictionary.\n");  
        return -1;  
    }  

    //��ӡ������һ��  
    printDict(pDict);  

    //��ʾ��������  
    PyObject* pFunHi = PyDict_GetItemString(pDict, "sayhi");  
    PyObject_CallFunction(pFunHi, "s", "lhb");  
    Py_DECREF(pFunHi);  

    //��ʾ����һ��Python���󣬲�����Class�ķ���  
    //��ȡSecond��  
    PyObject* pClassSecond = PyDict_GetItemString(pDict, "Second");  
    if (!pClassSecond)
	{  
        printf("Cant find second class.\n");
        return -1;  
    }  

    //��ȡPerson��  
    PyObject* pClassPerson = PyDict_GetItemString(pDict, "Person");  

    if (!pClassPerson)
	{  
        printf("Cant find person class.\n");
        return -1;  
    }  

    //����Second��ʵ��  
    PyObject* pInstanceSecond = PyInstance_New(pClassSecond, NULL, NULL);  

    if (!pInstanceSecond)
	{  
        printf("Cant create second instance.\n");  
        return -1;  
    }  

    //����Person��ʵ��  
    PyObject* pInstancePerson = PyInstance_New(pClassPerson, NULL, NULL);  

    if (!pInstancePerson)
	{  
        printf("Cant find person instance.\n");  
        return -1;  
    }  

    //��personʵ������second��invoke����  
    PyObject_CallMethod(pInstanceSecond, "invoke", "O", pInstancePerson);  

	//�ͷ�  
	Py_DECREF(pInstanceSecond);  
	Py_DECREF(pInstancePerson);  
	Py_DECREF(pClassSecond);  
	Py_DECREF(pClassPerson);  
	Py_DECREF(pModule);  
	Py_Finalize();  

	getchar();
    return 0;  

} 

