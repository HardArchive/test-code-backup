// dll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "dll.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// ���ǵ���������һ��ʾ��
DLL_API int ndll=0;

// ���ǵ���������һ��ʾ����
DLL_API int fndll(void)
{
	return 42;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� dll.h
Cdll::Cdll()
{ 
	return; 
}
