// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//


#include <python.h> //����ͷ�ļ�����c++��Ƕ��python�����Ǳ����
int main() 
{ 
	Py_Initialize(); //��ʼ��python
	
	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	pModule = PyImport_ImportModule("test2"); //��ȡtest1.py�ļ����
	//pFunc   = PyObject_GetAttrString(pModule, "Hello"); //��ȡHello����
	pFunc   = PyObject_GetAttrString(pModule, "you"); 
	PyEval_CallObject(pFunc, NULL);  //���ú���
	Py_Finalize();  //�ͷ���Դ
	return 0; 
} 