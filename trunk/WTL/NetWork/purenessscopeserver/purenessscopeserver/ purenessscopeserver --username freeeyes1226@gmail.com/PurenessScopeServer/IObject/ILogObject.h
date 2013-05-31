#ifndef _ILOGOBJECT
#define _ILOGOBJECT

#include "ace/SString.h"

//日志类对象(是一个虚类，由别的类继承实现内部)
class CServerLogger 
{
public:
	virtual ~CServerLogger() {};

	virtual int DoLog(int nLogType, ACE_TString* pLogText) = 0;
	virtual int GetLogTypeCount()                          = 0;
	virtual int GetLogType(int nIndex)                     = 0;
};
#endif
