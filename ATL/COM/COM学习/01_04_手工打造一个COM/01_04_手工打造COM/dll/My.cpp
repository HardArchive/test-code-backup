//My.cpp
#include "StdAfx.h"
#include ".\my.h"
#include <cstring>
using namespace std;

//将指针计数初始化为0
CMy::CMy(void):m_iRes(0)
{
}

CMy::~CMy(void)
{
}

STDMETHODIMP CMy::Add (int x1,int x2)
{
   m_iResult=x1+x2;
   return S_OK;
}



STDMETHODIMP CMy::QueryInterface(REFIID riid, void **ppv)
{
	//	这里这是实现dynamic_cast的功能，但由于dynamic_cast与编译器相关。
	if(riid == IID_IMy)
		*ppv = static_cast<IMy *>(this);
	else if(riid == IID_IMy2)
		*ppv = static_cast<IMy2 *>(this);
	else if(riid == IID_IUnknown)
		*ppv = static_cast<IMy *>(this);
	else {
		*ppv = 0;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();	//这里要这样是因为引用计数是针对组件的
	return S_OK;
}

STDMETHODIMP_(ULONG) CMy::AddRef ()
{
   //指针复制了，增加计数
	return ++m_iRes;
}
STDMETHODIMP_(ULONG) CMy::Release()
{
	//指针销毁时，将对象销毁
	if(--m_iRes==0)
       delete this;
	return m_iRes;
}