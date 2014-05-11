// com_1and2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <Unknwn.h>


// {68F8316B-428A-4543-9154-357EC3534678}
static const IID IID_IX = { 0x68f8316b, 0x428a, 0x4543, { 0x91, 0x54, 0x35, 0x7e, 0xc3, 0x53, 0x46, 0x78 } };
// {29C08E09-5E57-4a5e-BCAB-103124217410}
static const IID IID_IY = { 0x29c08e09, 0x5e57, 0x4a5e, { 0xbc, 0xab, 0x10, 0x31, 0x24, 0x21, 0x74, 0x10 } };


interface IX : public IUnknown
{
	virtual void Fx1() = 0;
	virtual void Fx2() = 0;
};

interface IY : public IUnknown
{
	virtual void Fy1() = 0;
	virtual void Fy2() = 0;
};

class CA : public IX, public IY
{
public: 
	CA()
	{
		m_lCount = 0;
		AddRef();//构造时，需要自增引用计数
	}

	~CA()
	{
		cout << "调用析构函数 释放！！！" <<endl;
	}
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void **ppvObject)
	{
		if (IID_IUnknown == riid)
		{
			*ppvObject = static_cast<IX*>(this);
		}
		else if (IID_IX == riid)
		{
			*ppvObject = static_cast<IX*>(this);
		}
		else if (IID_IY == riid)
		{
			*ppvObject = static_cast<IY*>(this);
		}
		else
		{
			*ppvObject = NULL; 			//查询不到IID，*ppvObject返回NULL。
			return E_NOINTERFACE;	    //函数返回值返回E_NOINTERFACE，表示组件不支持iid的接口。
		}

		AddRef();       //成功则计数器加1

		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return ++m_lCount;
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		if (0 == --m_lCount)
		{
			delete this;
		}
		return m_lCount;
	}


	virtual void Fx1()
	{
		cout << "Fx1 Run……" << endl;
	}

	virtual void Fx2()
	{
		cout << "Fx2 Run……" << endl;
	}

	virtual void Fy1()
	{
		cout << "Fy1 Run……" << endl;
	}

	virtual void Fy2()
	{
		cout << "Fy2 Run……" << endl;
	}

private:
	long m_lCount;
};


int _tmain(int argc, _TCHAR* argv[])
{
	CA* pclsCA = new CA;
	IUnknown* pclsIUnknown = NULL;
	HRESULT hr = pclsCA->QueryInterface(IID_IUnknown, (void**)&pclsIUnknown);

	if (SUCCEEDED(hr))
	{
		IX* pclsIX = NULL;
		hr = pclsIUnknown->QueryInterface(IID_IX, (void**)&pclsIX);
		if (SUCCEEDED(hr))
		{
			pclsIX->Fx1();
			pclsIX->Fx2();
		}


		IY* pclsIY = NULL;
		hr = pclsIUnknown->QueryInterface(IID_IY, (void**)&pclsIY);
		if (SUCCEEDED(hr))
		{
			pclsIY->Fy1();
			pclsIY->Fy2();
		}
	}



	//pclsCA->Fx1();
	//pclsCA->Fx2();
	//pclsCA->Fy1();
	//pclsCA->Fy2();


	//IX* pclsIX = pclsCA;
	//pclsIX->Fx1();
	//pclsIX->Fx2();

	//IY* pclsIY = pclsCA;
	//pclsIY->Fy1();
	//pclsIY->Fy2();

	
	//释放组件? no!
	//delete pclsCA; //不再需要写delete代码
	//pclsCA = NULL;

		
	return 0;
}

