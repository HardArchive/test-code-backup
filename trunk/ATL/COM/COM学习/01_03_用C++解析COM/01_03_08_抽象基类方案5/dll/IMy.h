//IMy.h
#pragma once
#include "IMyUnkown.h"

class  IMy:public IMyUnkown
{
public:
	virtual void Add(int x1,int x2)=0;
	virtual int Get()=0;	
};
