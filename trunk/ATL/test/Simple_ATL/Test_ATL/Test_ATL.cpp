// Test_ATL.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include "../Simple_ATL/Simple_ATL_i.h"
//#include "../Simple_ATL/First_ATL.h"

// �����µ����ݴ�Simple_ATL����Ŀ¼��Simple_ATL_i.c�ļ��и��ƹ���
// ע�⣺��Ҳ����ֱ�Ӱ���Simple_ATL_i.c�ļ������ڴ�ֻ������ر�����Щconst�������Ժδ��Լ����ǵ�����

const IID IID_IFirst_ATL = {0x8048195A,0xF56F,0x4085,{0x9E,0xB5,0x36,0x06,0x56,0x7C,0x12,0x6D}};
const CLSID CLSID_First_ATL = {0x48CB9DA1,0xD7E3,0x4527,{0xB3,0xCF,0x27,0x06,0xA0,0x76,0x8A,0x28}};


int _tmain(int argc, _TCHAR* argv[])
{
	// ����һ��HRESULT�����Լ�һ��Simple_ATL�ӿڵ�ָ��
	HRESULT          hr;
	IFirst_ATL       *IFirstATL = NULL;

	// ���ڳ�ʼ��COM
	hr = CoInitialize(0);

	// ʹ��SUCCEEDED�������������Ƿ��ܹ���ýӿڵ�ָ��
	if(SUCCEEDED(hr))
	{
		hr = CoCreateInstance( CLSID_First_ATL, NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFirst_ATL, (void**) &IFirstATL);

		// ����ɹ��ˣ���ô����AddNumbers����
		// ������û���ʾһ���ʵ�����Ϣ
		if(SUCCEEDED(hr))
		{
			long ReturnValue;

			IFirstATL->MinusNumbers(5, 7, &ReturnValue);
			cout << "The answer for 5 + 7 is: "
				<< ReturnValue << endl;
			IFirstATL->Release();
		}
		else
		{
			cout << "CoCreateInstance Failed." << endl;
		}
	}
	// ж��COM
	CoUninitialize();
	return 0;
}

