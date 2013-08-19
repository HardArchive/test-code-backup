// Test_ATL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

// 将头文件的目录指到Simple_ATL工程所在的目录
#include "../Simple_ATL/Simple_ATL_i.h"
//#include "..\Simple_ATL\Simple_ATL.h"
//#include 
// 从Simple_ATL 工程所在目录的Simple_ATL_i.c 文件中拷贝以下内容
// 注意: 你也可以不拷贝这些东西，而是把文件Simple_ATL_i.c包含进来。
// 我之所以将它拷进来，是想更清楚地展示这些敞亮来自什么地方一击它们的代码

const IID IID_IFirst_ATL = {0x34D3379E,0xDAFE,0x4CE1,{0xBA,0x93,0x85,0x77,0x71,0x39,0x51,0x68}};
const CLSID CLSID_First_ATL = {0x2E334F59,0xCBD8,0x4A4A,{0xA0,0x38,0xD6,0x58,0xDE,0x24,0x0E,0x9F}};

void main(void)
{
	// 声明HRESULT和Simple_ATL接口指针
	HRESULT hr;
	IFirst_ATL *IFirstATL = NULL;

	// 初始化COM
	hr = CoInitialize(0);

	// 使用SUCCEEDED 宏并检查我们是否能得到一个接口指针 
	if(SUCCEEDED(hr))
	{
		hr = CoCreateInstance( CLSID_First_ATL, NULL, CLSCTX_INPROC_SERVER,
			IID_IFirst_ATL, (void**) &IFirstATL);

		// 如果成功，则调用AddNumbers方法，否则显示相应的出错信息
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
	// 释放COM
	CoUninitialize();

	system("VER");
	cout << endl;
	//system("TIME");
	system("pause");
}