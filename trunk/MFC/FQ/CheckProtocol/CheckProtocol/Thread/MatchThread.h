#pragma once
#include "Threadbase.h"
typedef void (__stdcall *CALLMATCHSTATE)(char *pItemName);

class CMatchThread : public CThreadBase
{
public:
	CMatchThread(void);
	~CMatchThread(void);

public:

    virtual  bool Initialization();
	virtual	 void WorkThread(LPVOID pParam = NULL);
	CALLMATCHSTATE CalMatchState;
	bool     CloseWebProcess(char *ItemName);
	void     Close();
};
