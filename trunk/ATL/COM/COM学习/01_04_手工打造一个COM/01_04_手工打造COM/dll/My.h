//My.h
#pragma once
#include "IMy_h.h"
#include <cstring>
using namespace std;

class  CMy:public IMy,public IMy2
{
	int m_iResult;
	ULONG m_iRes;
public:
	CMy(void);
	~CMy(void);
	//IUnknown方法
	STDMETHOD(QueryInterface)(REFIID riid, void **ppv);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	//IMy的方法
	STDMETHOD (Add)(int x1,int x2);
	STDMETHOD (Get)(int * pret)
	{
		*pret= m_iResult;
		return S_OK;
	}
	//IMy2的方法
	STDMETHOD ( SayHello)(CHAR * pret)
	{		
		strcpy(pret,"Hello world");
		return S_OK;
	}

	
};
