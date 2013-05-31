#ifndef _IMESSAGEMANAGER_H
#define _IMESSAGEMANAGER_H

#include "ClientCommand.h"
#include "IMessage.h"

class IMessageManager
{
public:
	virtual ~IMessageManager() {};
	virtual bool DoMessage(IMessage* pMessage, uint16& u2CommandID)    = 0;
	virtual void Close()                                               = 0;

	virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName) = 0;
	virtual bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand)                          = 0;
};

#endif
