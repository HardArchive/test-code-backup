//My.h
#pragma once
#include "IMy.h"

class  CMy:public IMy
{
	int m_iResult;
public:
	CMy(void);
	~CMy(void);
	void Add(int x1,int x2);
	int Get()
	{
		return m_iResult;
	}
	void Delete();
};
