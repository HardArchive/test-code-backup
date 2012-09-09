//IMy2.h
#pragma once
#include "IMyUnkown.h"

class  IMy2:public IMyUnkown
{
public:
	virtual const char* SayHello()=0;
		
};
