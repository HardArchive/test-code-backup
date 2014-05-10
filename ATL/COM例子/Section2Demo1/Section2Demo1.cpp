// Section2Demo1.cpp : 定义控制台应用程序的入口点。
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
//构造与析构
public:
	CA()
	{
		m_lCount = 0;

		//构造时，需要自增引用计数
		AddRef();
	}
	virtual ~CA()		//析构函数一般采用虚函数
	{
		cout << "我被释放啦！" << endl;
	}

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

		//查询成功时，需要自增引用计数
		AddRef();		

		return S_OK;	//返回S_OK
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		//简单实现方法
		return ++m_lCount;

		//多线程编程采用如下方法，这种方法确保同一个时刻只会有一个线程来访问成员变量
		//return InterlockedIncrement(&m_lCount);
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		//简单实现方法
		if (--m_lCount == 0)
		{
			delete this;	//销毁自己
			return 0;
		}
		return m_lCount;

		////多线程编程采用如下方法，这种方法确保同一个时刻只会有一个线程来访问成员变量
		//if (InterlockedDecrement(&m_lCount) == 0)
		//{
		//	delete this;		//销毁自己
		//	return 0;
		//}
		//return m_lCount;
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
private:
	long m_lCount;		//引用计数，该计数只被该类管理，外界不可访问，访问权限设置为private

};


int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;

	CA *pA = new CA();		//引用计数1

	//从组件查询IUnknown接口
	IUnknown *pIUnknown = NULL;
	hr = pA->QueryInterface(IID_IUnknown, (void**)&pIUnknown);		//引用计数2
	if (SUCCEEDED(hr))		//对HRESULT返回值的判断，一般采用SUCCEEDED
	{
		pA->Release();	//pA不再使用，引用计数1
		pA = NULL;		//访止再不小心使用m_pA

		//从IUnknown查询IX接口
		IX *pIX = NULL;
		hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX);	//引用计数2
		if (SUCCEEDED(hr))
		{
			//调用IX接口的方法
			pIX->Fx1();
			pIX->Fx2();
		}

		//从IUnknown查询IY接口
		IY *pIY = NULL;
		hr = pIUnknown->QueryInterface(IID_IY, (void**)&pIY);	//引用计数3
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

		pIY->Release();		//pIY不再使用，引用计数2
		pIY = NULL;

		if ((void*)pIUnknown == (void*)pIX)
		{
			cout << "pIUnknown == pIX" <<endl;
		}

		//从IX查询IY
		IY *pIY2 = NULL;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY2);		//引用计数，引用计数3

		pIX->Release();		//pIX不再使用，引用计数2
		pIX = NULL;

		if (SUCCEEDED(hr))
		{
			pIY2->Fy1();
			pIY2->Fy2();
		}

		pIY2->Release();	//pIY不再使用，引用计数1
		pIY2 = NULL;

	}

	//目前引用计数为1，因为pIUnknown还在使用。

	IX *pIX2 = NULL;
	hr = pIUnknown->QueryInterface(IID_IX, (void**)&pIX2);		//引用计数为2
	if (SUCCEEDED(hr))
	{
		IX *pIX3 = NULL;
		pIX3 = pIX2;		//执行了赋值
		pIX3->AddRef();		//由于上句执行了赋值，所以引用计数需要自增，引用计数为3

		pIX3->Fx1();
		pIX3->Fx2();

		pIX3->Release();		//pIX3不再使用，引用计数为2
		pIX3 = NULL;
	}
	pIX2->Release();		//pIX2不再使用，引用计数为1
	pIX2 = NULL;

	pIUnknown->Release();		//pIUnknown不再使用，引用计数为0，Release函数里执行了delete this，销毁组件的内存资源
	pIUnknown = NULL;

	//释放组件? no!
	//delete pA;		//不再需要写delete代码


	return 0;
}

