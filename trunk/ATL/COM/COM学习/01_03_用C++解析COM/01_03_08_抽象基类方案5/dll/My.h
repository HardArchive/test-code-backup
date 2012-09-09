//My.h
#pragma once
#include "IMy.h"
#include "IMy2.h"

class  CMy:public IMy,public IMy2
{
	int m_iResult;
	int m_iRes;
public:
	CMy(void);
	~CMy(void);
	void Add(int x1,int x2);
	int Get()
	{
		return m_iResult;
	}
	
	const char* SayHello(){ return "Hello world";}

	void * QueryInterface(const char* pszType);
	void AddRef();
	void Release();
};
