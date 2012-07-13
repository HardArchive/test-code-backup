// License.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"

#include "SerialManager.h"
#include "License.h"

CSerialManager g_objSM;

//��ȡ������
extern "C" __declspec(dllexport) bool GetMachineCode(char* pszMachineCode)
{
	if (!pszMachineCode)
	{
		return false;
	}

	strcpy(pszMachineCode, g_objSM.GetMachineCode());
	return true;
}

//��ȡ���к�
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

//������к��Ƿ���ȷ
extern "C" __declspec(dllexport) bool CheckSerial(char* pszSerial)
{
	if (strcmp(pszSerial, g_objSM.GetSerial()))
	{
		return false;
	}
	return true;
}
