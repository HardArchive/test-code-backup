// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <python.h> //����ͷ�ļ�����c++��Ƕ��python�����Ǳ����

int main() 
{ 
	Py_Initialize(); //��ʼ��python

	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	PyObject * pArg    = NULL; 
	PyObject * pClass  = NULL; 
	PyObject * pObject = NULL; 

	pModule = PyImport_ImportModule("test2");               //��ȡtest2.py�ļ����

	pClass  = PyObject_GetAttrString(pModule, "TestClass"); //��ȡ����� 
	pArg = PyTuple_New(1);                                  //newһ��Ԫ�飬����size 
	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", "��ë,����˧��"));  //setԪ���ֵ����һ������ΪԪ�飬�ڶ���Ϊindex����0��ʼ����������Ϊvalue 
	pObject = PyEval_CallObject(pClass, pArg);              //����һ�����󣬻��߽���ʵ�� 
	
	pFunc = PyObject_GetAttrString(pObject, "printName");   //�õ���ʵ���ĳ�Ա���� 
	PyEval_CallObject(pFunc, NULL);                         //ִ�и�ʵ���ĳ�Ա���� 


	Py_Finalize();                                         //�ͷ���Դ
	getchar();

	return 0; 
} 

