#include <windows.h>
#include "../MathCOM.h"
#include "../MathCOM_i.c"
#include <iostream>
using namespace std;

void main(void)
{
	//初始化COM库
	HRESULT hr=::CoInitialize(0);
	ISimpleMath * pSimpleMath=NULL;
	IAdvancedMath * pAdvancedMath=NULL;

	int nReturnValue=0;
/*@**#---2003-10-30 10:58:25 (tulip)---#**@
	hr=::CoCreateInstance(CLSID_MATHCOM,NULL,CLSCTX_INPROC,
						IID_ISimpleMath,(void**)&pSimpleMath);
*/
	//如果还想用CoCreateInstance()得到接口指针，请如下使用先传一个IClassFactory接口

	IClassFactory * pClassFactory=NULL;
	hr=::CoGetClassObject(CLSID_MATHCOM,CLSCTX_INPROC,NULL,
		IID_IClassFactory,(void**)&pClassFactory);
	pClassFactory->CreateInstance(NULL,IID_ISimpleMath,(void**)&pSimpleMath);

//@**#---2003-10-30 11:26:15 (tulip)---#**@
////上面的创建COM组件实例的方法可用下面替换
//	hr=::CoGetClassObject(CLSID_MATHCOM,CLSCTX_INPROC,
//							NULL,IID_ISimpleMath,
//							(void **)&pSimpleMath);
	if(SUCCEEDED(hr))
	{
		hr=pSimpleMath->Add(10,4,&nReturnValue);
		if(SUCCEEDED(hr))
			cout << "10 + 4 = " <<nReturnValue<< endl;
		nReturnValue=0;
	}
	hr=pSimpleMath->QueryInterface(IID_IAdvancedMath, (void **)&pAdvancedMath);	//	查询对象实现的接口IAdvancedMath
	if(SUCCEEDED(hr))
	{
		hr=pAdvancedMath->Fabonacci(10,&nReturnValue);
		if(SUCCEEDED(hr))
			cout << "10 Fabonacci is " << nReturnValue << endl;	
	}
	pAdvancedMath->Release();
	pSimpleMath->Release();

	::CoUninitialize();

	::system("pause");
	return ;

}