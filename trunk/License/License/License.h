#include "stdafx.h"


//��ȡ������
extern "C" __declspec(dllexport) bool GetMachineCode(char* pszMachineCode);

//��ȡ���к�
extern "C" __declspec(dllexport) bool GetSerial(char* pszSerial);

//������к��Ƿ���ȷ
extern "C" __declspec(dllexport) bool CheckSerial(char* pszSerial);
