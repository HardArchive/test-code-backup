#ifndef _ILOGMANAGER_H
#define _ILOGMANAGER_H

#include "ILogObject.h"

class ILogManager
{
public:
	virtual ~ILogManager() {};
	virtual int WriteLog(int nLogType, const char* fmt, ...)              = 0;
};

#endif
