//IMyUnkown.h
#pragma once

class  IMyUnkown
{
public:
	virtual void * QueryInterface(const char* pszType)=0;
	virtual void AddRef()=0;
	virtual void Release()=0;	
	
};