// Section1Demo2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <Unknwn.h>

using namespace std;

// {A348FBDD-E765-4b41-8477-6D8B7038FCC6}
static const IID IID_IX = 
{ 0xa348fbdd, 0xe765, 0x4b41, { 0x84, 0x77, 0x6d, 0x8b, 0x70, 0x38, 0xfc, 0xc6 } };

// {10A90ED2-FCDE-4067-92DA-ABA38F5C1B12}
static const IID IID_IY = 
{ 0x10a90ed2, 0xfcde, 0x4067, { 0x92, 0xda, 0xab, 0xa3, 0x8f, 0x5c, 0x1b, 0x12 } };


//接口IX
interface IX : public IUnknown
{	
	virtual void Fx1() = 0;
	virtual void Fx2() = 0;
};

//接口IY
interface IY : public IUnknown
{	
	virtual void Fy1() = 0;
	virtual void Fy2() = 0;
};

//组件CA
class CA: public IX, public IY
{
//实现
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppv)
	{
		//查看PPT中CA的内存结构讲解如下的转换过程

		if (iid == IID_IUnknown)
		{
			//即使CA继承了两个IUnknown接口，其中一个来自于IX，另一个来自于IY。我们一般返回第一个被继承的IX接口。
			*ppv = static_cast<IX*>(this);		
		}
		else if (iid == IID_IX)
		{
			//返回IX接口
			*ppv = static_cast<IX*>(this);		
		}
		else if (iid == IID_IY)
		{
			//返回IY接口
			*ppv = static_cast<IY*>(this);
		}
		else
		{
			//查询不到IID，*ppv返回NULL。
			*ppv = NULL;
			return E_NOINTERFACE;	//函数返回值返回E_NOINTERFACE，表示组件不支持iid的接口。
		}

		AddRef();		//这个第二讲会讲

		return S_OK;	//返回S_OK
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		//暂时不实现，暂时返回0
		return 0;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		//暂时不实现，暂时返回0
		return 0;
	}

	virtual void Fx1()
	{
		cout << "Fx1" << endl;
	}

	virtual void Fx2()
	{
		cout << "Fx2" << endl;
	}

	virtual void Fy1()
	{
		cout << "Fy1" << endl;
	}

	virtual void Fy2()
	{
		cout << "Fy2" << endl;
	}

//数据
public:
	long m_lAA;
	long m_lAB;
	long m_lAC;

};


int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;

	//创建组件
	CA *pA = new CA();

	//从组件查询IUnknown接口
	IUnknown *pIUnknown = NULL;
	hr = pA->QueryInterface(IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr))		//对HRESULT返回值的判断，一般采用SUCCEEDED
	{
		//从IUnknown查询IX接口
		IX *pIX = NULL;
		hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);
		if (SUCCEEDED(hr))
		{
			//调用IX接口的方法
			pIX->Fx1();
			pIX->Fx2();
		}

		//从IUnknown查询IY接口
		IY *pIY = NULL;
		hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);
		if (SUCCEEDED(hr))
		{
			//调用IY接口的方法
			pIY->Fy1();
			pIY->Fy1();
		}

		if ((void*)pIX != (void*)pIY)
		{
			cout << "pIX != pIY" <<endl;
		}

		if ((void*)pIUnknown != (void*)pIY)
		{
			cout << "pIUnknown != pIY" <<endl;
		}

		if ((void*)pIUnknown == (void*)pIX)
		{
			cout << "pIUnknown == pIX" <<endl;
		}

		//从IX查询IY
		IY *pIY2 = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY2);
		if (SUCCEEDED(hr))
		{
			pIY2->Fy1();
			pIY2->Fy2();
		}

		//从IX也可以查询到IUnknown
		//从IY也可以查询到IX
		//从IY也可以查询到IUnknown
		//从CA也可以查询到IX
		//从CA也可以查询到IY
		//总结：
			//只要是CA所继承的接口，从CA的组件里都可以查询到；
			//只要是CA所继承的接口，从CA所继承的其它接口里都可以查询到。
	}

	//释放组件
	delete pA;

	return 0;
}

