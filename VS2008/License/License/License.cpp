// License.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "SerialManager.h"
#include "License.h"

CSerialManager g_objSM;

//获取机器码
extern "C" __declspec(dllexport) bool GetMachineCode(char* pszMachineCode)
{
	if (!pszMachineCode)
	{
		return false;
	}

	strcpy(pszMachineCode, g_objSM.GetMachineCode());
	return true;
}

//获取序列号
extern "C" __declspec(dllexport) bool GetSerial(char* pszSerial)
{
	if (!pszSerial)
	{
		return false;
	}

	if (strcmp(pszSerial, "TIPTOP"))
	{
		return false;
	}
	strcpy(pszSerial, g_objSM.GetSerial());	

	return true;
}

//检查序列号是否正确
extern "C" __declspec(dllexport) bool CheckSerial(char* pszSerial)
{
	if (strcmp(pszSerial, g_objSM.GetSerial()))
	{
		return false;
	}
	return true;
}
