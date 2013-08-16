// Test_ATL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include "../Simple_ATL/Simple_ATL_i.h"
//#include "../Simple_ATL/First_ATL.h"

// 把以下的内容从Simple_ATL工程目录的Simple_ATL_i.c文件中复制过来
// 注意：你也可以直接包含Simple_ATL_i.c文件，我在此只想清楚地表明这些const常量来自何处以及它们的样子

const IID IID_IFirst_ATL = {0x8048195A,0xF56F,0x4085,{0x9E,0xB5,0x36,0x06,0x56,0x7C,0x12,0x6D}};
const CLSID CLSID_First_ATL = {0x48CB9DA1,0xD7E3,0x4527,{0xB3,0xCF,0x27,0x06,0xA0,0x76,0x8A,0x28}};


int _tmain(int argc, _TCHAR* argv[])
{
	// 声明一个HRESULT变量以及一个Simple_ATL接口的指针
	HRESULT          hr;
	IFirst_ATL       *IFirstATL = NULL;

	// 现在初始化COM
	hr = CoInitialize(0);

	// 使用SUCCEEDED宏来看看我们是否能够获得接口的指针
	if(SUCCEEDED(hr))
	{
		hr = CoCreateInstance( CLSID_First_ATL, NULL,
			CLSCTX_INPROC_SERVER,
			IID_IFirst_ATL, (void**) &IFirstATL);

		// 如果成功了，那么调用AddNumbers方法
		// 否则给用户显示一条适当的信息
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
	// 卸载COM
	CoUninitialize();
	return 0;
}

