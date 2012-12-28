#pragma once

#include "../../Common/Src/socket/SocketClient.h"

class CWorker :	public IClientSocketListener
{
public:
	CWorker(void);
	~CWorker(void);
};

