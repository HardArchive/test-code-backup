// Test_ATL.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;

// ��ͷ�ļ���Ŀ¼ָ��Simple_ATL�������ڵ�Ŀ¼
#include "../Simple_ATL/Simple_ATL_i.h"
//#include "..\Simple_ATL\Simple_ATL.h"
//#include 
// ��Simple_ATL ��������Ŀ¼��Simple_ATL_i.c �ļ��п�����������
// ע��: ��Ҳ���Բ�������Щ���������ǰ��ļ�Simple_ATL_i.c����������
// ��֮���Խ���������������������չʾ��Щ��������ʲô�ط�һ�����ǵĴ���

const IID IID_IFirst_ATL = {0x34D3379E,0xDAFE,0x4CE1,{0xBA,0x93,0x85,0x77,0x71,0x39,0x51,0x68}};
const CLSID CLSID_First_ATL = {0x2E334F59,0xCBD8,0x4A4A,{0xA0,0x38,0xD6,0x58,0xDE,0x24,0x0E,0x9F}};

void main(void)
{
	// ����HRESULT��Simple_ATL�ӿ�ָ��
	HRESULT hr;
	IFirst_ATL *IFirstATL = NULL;

	// ��ʼ��COM
	hr = CoInitialize(0);

	// ʹ��SUCCEEDED �겢��������Ƿ��ܵõ�һ���ӿ�ָ�� 
	if(SUCCEEDED(hr))
	{
		hr = CoCreateInstance( CLSID_First_ATL, NULL, CLSCTX_INPROC_SERVER,
			IID_IFirst_ATL, (void**) &IFirstATL);

		// ����ɹ��������AddNumbers������������ʾ��Ӧ�ĳ�����Ϣ
		if(SUCCEEDED(hr))
		{
			long ReturnValue;

			IFirstATL->AddNumbers(5, 7, &ReturnValue);
			cout << "The answer for 5 + 7 is: " << ReturnValue << endl;
			IFirstATL->Release(); 
		}
		else
		{
			cout << "CoCreateInstance Failed." << endl;
		}
	}
	// �ͷ�COM
	CoUninitialize();

	system("VER");
	cout << endl;
	//system("TIME");
	system("pause");
}