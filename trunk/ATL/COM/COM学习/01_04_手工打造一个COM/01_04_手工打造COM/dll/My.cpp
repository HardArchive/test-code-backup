//My.cpp
#include "StdAfx.h"
#include ".\my.h"
#include <cstring>
using namespace std;

//��ָ�������ʼ��Ϊ0
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
	//	��������ʵ��dynamic_cast�Ĺ��ܣ�������dynamic_cast���������ء�
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

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();	//����Ҫ��������Ϊ���ü�������������
	return S_OK;
}

STDMETHODIMP_(ULONG) CMy::AddRef ()
{
   //ָ�븴���ˣ����Ӽ���
	return ++m_iRes;
}
STDMETHODIMP_(ULONG) CMy::Release()
{
	//ָ������ʱ������������
	if(--m_iRes==0)
       delete this;
	return m_iRes;
}