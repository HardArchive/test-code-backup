#include "stdafx.h"


//获取机器码
extern "C" __declspec(dllexport) bool GetMachineCode(char* pszMachineCode);

//获取序列号
extern "C" __declspec(dllexport) bool GetSerial(char* pszSerial);

//检查序列号是否正确
extern "C" __declspec(dllexport) bool CheckSerial(char* pszSerial);
