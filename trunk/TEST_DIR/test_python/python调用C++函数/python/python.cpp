// python.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Python.h>

#include <iostream>
#include <windows.h>
using namespace std;

// ����һ����python���õĺ���
static PyObject* mb_showMsg(PyObject* self, PyObject *args);   

extern "C" __declspec(dllexport) void initmb()              //ע�����������   
{   
    static PyMethodDef mbMethods[] = {                  //python�޷�ֱ����mb_showMsg����   
        {"showMsg", mb_showMsg, METH_VARARGS},          // ���һ����python���õĺ�����python������Ϊ��showMsg
        {NULL, NULL, NULL} /*sentinel���ڱ���������ʶ����*/  
    };     
  
    PyObject *m = Py_InitModule("mb", mbMethods);             //"mb"�����ⲿ���ļ���   
}   

int _tmain(int argc, _TCHAR* argv[])
{
     Py_Initialize();           //ʹ��python֮ǰ��Ҫ����Py_Initialize();����������г�ʼ��   
	 if (!Py_IsInitialized())  
		 return -1;  

	 initmb();
     PyObject * pModule = NULL; //��������   
     PyObject * pFunc = NULL;       //��������   
     pModule =PyImport_ImportModule("pythonTest");  //������Ҫ���õ��ļ���   
     pFunc= PyObject_GetAttrString(pModule, "Hello"); //������Ҫ���õĺ�����   
     PyEval_CallObject(pFunc, NULL);            //���ú���   
     Py_Finalize();                     //����Py_Finalize�������Py_Initialize���Ӧ�ġ�  

	 system("pause");
     return 0;
}

  
  
static PyObject* mb_showMsg(PyObject* self, PyObject *args)   
{   
    //���ǵ�showMsg������Ҫ����һ���ַ�������   
    const char* msg = NULL;   
   // LPWSTR wmsg = NULL;   
       
    if (!PyArg_ParseTuple(args, "s", &msg))   
        return NULL;   
  
	int r = MessageBox(NULL, msg, "���ص���ʾ", MB_OK);
	// �����Ƕ����ĵ�ת����
    /*int r;   
    DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, msg, -1, NULL, 0); //���ַ���ת��,�ҵ�VC����Ĭ����UNICODE   
    wmsg = new wchar_t[dwNum];   
  
    if(!wmsg)   
    {   
      delete []wmsg;   
      r = ::MessageBox(NULL, _T("conversion failed") , _T("Caption:Form C module"), MB_ICONINFORMATION | MB_OK);   
    }   
    else  
    {   
        MultiByteToWideChar (CP_ACP, 0, msg, -1, wmsg, dwNum);   
        //����MB   
        r = ::MessageBox(NULL, wmsg , _T("Caption:Form C module"), MB_ICONINFORMATION | MB_OK);   
        delete []wmsg ;        
    }   
  */
    //����ֵ   
    return Py_BuildValue("i", r);   
}  
