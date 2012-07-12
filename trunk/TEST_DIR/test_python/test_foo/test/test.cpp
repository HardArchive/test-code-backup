// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//C++����python��������
#include <Windows.h>
#include <Python.h>

int main(int argc, char** argv)
{
	//��ʼ��Python
	//��ʹ��Pythonϵͳǰ������ʹ��Py_Initialize����
	//���г�ʼ������������Python���ڽ�ģ�鲢���ϵͳ·
	//����ģ������·���С��������û�з���ֵ�����ϵͳ
	//�Ƿ��ʼ���ɹ���Ҫʹ��Py_IsInitialized��

	Py_Initialize();

	// ����ʼ���Ƿ�ɹ�
	if ( !Py_IsInitialized() )
	{
		printf("Init Error\r\n");
		Sleep(10000);
		return -1;
	}

	// ��ӵ�ǰ·��
	//��������ַ�����ΪPython����ֱ�����У�����0
	//��ʾ�ɹ���-1��ʾ�д����ʱ���������Ϊ�ַ���
	//�����﷨����
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	PyObject *pName = NULL, *pModule = NULL, *pDict = NULL, *pFunc = NULL, *pArgs = NULL;

	// ������Ϊpytest�Ľű�
	pName = PyString_FromString("pytest");
	pModule = PyImport_Import(pName);
	if ( !pModule )
	{
		printf("can't find pytest.py\r\n");
		Sleep(10000);
		return -1;
	}
	pDict = PyModule_GetDict(pModule);
	if ( !pDict )
	{
		printf("can't GetDict\r\n");
		Sleep(10000);
		return -1;
	}

	// �ҳ�������Ϊadd�ĺ���
	pFunc = PyDict_GetItemString(pDict, "add");
	if ( !pFunc || !PyCallable_Check(pFunc) )
	{
		printf("can't find function [add]");
		Sleep(10000);
		return -1;
	}

	// ������ջ
	*pArgs;
	pArgs = PyTuple_New(2);

	// PyObject* Py_BuildValue(char *format, ...)
	// ��C++�ı���ת����һ��Python���󡣵���Ҫ��
	// C++���ݱ�����Pythonʱ���ͻ�ʹ������������˺���
	// �е�����C��printf������ʽ��ͬ�����õĸ�ʽ��
	// s ��ʾ�ַ�����
	// i ��ʾ���ͱ�����
	// f ��ʾ��������
	// O ��ʾһ��Python����

	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i",3));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",4));

	// ����Python����
	PyObject_CallObject(pFunc, pArgs);

	//��������ǲ��Һ���foo ��ִ��foo
	pFunc = PyDict_GetItemString(pDict, "foo");
	if ( !pFunc || !PyCallable_Check(pFunc) )
	{
		printf("can't find function [foo]");
		Sleep(10000);
		return -1;
	}

	pArgs = PyTuple_New(1);
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("i",2)); //

	PyObject_CallObject(pFunc, pArgs);


	Py_DECREF(pName);
	Py_DECREF(pArgs);
	Py_DECREF(pModule);

	// �ر�Python
	Py_Finalize();

	getchar();
	return 0;
}

