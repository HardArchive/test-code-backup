#pragma once
#include "My.h"

class CMy;
class __declspec(dllexport) CHandleClass
{
	
	CMy* m_pThis;
public:
	CHandleClass(void);
	~CHandleClass(void);
	void Add(int x1,int x2);
	int Get();
	
};
