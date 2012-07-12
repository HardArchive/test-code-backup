// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <python.h> //����ͷ�ļ�����c++��Ƕ��python�����Ǳ����

int main() 
{ 
	Py_Initialize(); //��ʼ��python
	if (!Py_IsInitialized())  
		return -1;  

	// ��ӵ�ǰ·��
	//��������ַ�����ΪPython����ֱ�����У�����0
	//��ʾ�ɹ���-1��ʾ�д����ʱ���������Ϊ�ַ���
	//�����﷨����
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')"); 
	PyRun_SimpleString("print('PyRun_SimpleString���������')");

	PyObject * pModule = NULL; 
	PyObject * pFunc   = NULL;
	PyObject * pArg    = NULL; 

	PyObject *pName = NULL;
	
	// ������Ϊtest2�Ľű�
	pName = PyString_FromString("test2");
	pModule = PyImport_Import(pName);

	//pModule = PyImport_ImportModule("test2");              //��ȡtest1.py�ļ����
	if (!pModule)
	{  
		printf("Cant open python file!\n");  
		return -1;  
	}  


	pFunc = PyObject_GetAttrString(pModule, "Hello");      //��ȡHello����
	pArg = Py_BuildValue("(s)", "���뺯����������"); //���ú�������
	PyEval_CallObject(pFunc, pArg);                        //���ú���

	pFunc = PyObject_GetAttrString(pModule, "Add");     
	pArg = Py_BuildValue("(i, i)", 5, 10);
	PyEval_CallObject(pFunc, pArg);                        //���ú���
	Py_Finalize();                                         //�ͷ���Դ
	getchar();

	return 0; 
} 
//
//����Py_BuildValue��ʹ�����ӣ�����python documentation��
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

