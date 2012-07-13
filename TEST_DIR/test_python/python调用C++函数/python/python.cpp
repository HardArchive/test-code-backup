// python.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Python.h>

#include <iostream>
#include <windows.h>
using namespace std;

// 声明一个被python调用的函数
static PyObject* mb_showMsg(PyObject* self, PyObject *args);   

extern "C" __declspec(dllexport) void initmb()              //注意这里的名字   
{   
    static PyMethodDef mbMethods[] = {                  //python无法直接用mb_showMsg访问   
        {"showMsg", mb_showMsg, METH_VARARGS},          // 添加一个被python调用的函数，python调用名为：showMsg
        {NULL, NULL, NULL} /*sentinel，哨兵，用来标识结束*/  
    };     
  
    PyObject *m = Py_InitModule("mb", mbMethods);             //"mb"是你外部的文件名   
}   

int _tmain(int argc, _TCHAR* argv[])
{
     Py_Initialize();           //使用python之前，要调用Py_Initialize();这个函数进行初始化   
	 if (!Py_IsInitialized())  
		 return -1;  

	 initmb();
     PyObject * pModule = NULL; //声明变量   
     PyObject * pFunc = NULL;       //声明变量   
     pModule =PyImport_ImportModule("pythonTest");  //这里是要调用的文件名   
     pFunc= PyObject_GetAttrString(pModule, "Hello"); //这里是要调用的函数名   
     PyEval_CallObject(pFunc, NULL);            //调用函数   
     Py_Finalize();                     //调用Py_Finalize，这个跟Py_Initialize相对应的。  

	 system("pause");
     return 0;
}

  
  
static PyObject* mb_showMsg(PyObject* self, PyObject *args)   
{   
    //我们的showMsg函数需要的是一个字符串参数   
    const char* msg = NULL;   
   // LPWSTR wmsg = NULL;   
       
    if (!PyArg_ParseTuple(args, "s", &msg))   
        return NULL;   
  
	int r = MessageBox(NULL, msg, "返回的提示", MB_OK);
	// 以下是对中文的转换。
    /*int r;   
    DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, msg, -1, NULL, 0); //宽字符的转换,我的VC环境默认是UNICODE   
    wmsg = new wchar_t[dwNum];   
  
    if(!wmsg)   
    {   
      delete []wmsg;   
      r = ::MessageBox(NULL, _T("conversion failed") , _T("Caption:Form C module"), MB_ICONINFORMATION | MB_OK);   
    }   
    else  
    {   
        MultiByteToWideChar (CP_ACP, 0, msg, -1, wmsg, dwNum);   
        //调用MB   
        r = ::MessageBox(NULL, wmsg , _T("Caption:Form C module"), MB_ICONINFORMATION | MB_OK);   
        delete []wmsg ;        
    }   
  */
    //返回值   
    return Py_BuildValue("i", r);   
}  
