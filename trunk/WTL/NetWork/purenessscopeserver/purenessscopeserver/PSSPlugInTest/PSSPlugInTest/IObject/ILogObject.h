#ifndef _ILOGOBJECT
#define _ILOGOBJECT

#include "ace/SString.h"

//��־�����(��һ�����࣬�ɱ����̳�ʵ���ڲ�)
class CServerLogger 
{
public:
	virtual ~CServerLogger() {};

	virtual int DoLog(int nLogType, ACE_TString* pLogText) = 0;
	virtual int GetLogTypeCount()                          = 0;
	virtual int GetLogType(int nIndex)                     = 0;
};
#endif
