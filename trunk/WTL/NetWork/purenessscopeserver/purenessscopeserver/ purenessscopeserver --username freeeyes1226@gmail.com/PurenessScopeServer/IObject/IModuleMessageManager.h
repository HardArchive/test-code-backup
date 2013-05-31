#ifndef _IMODULEMESSAGEMANAGER_H
#define _IMODULEMESSAGEMANAGER_H

#include "IBuffPacket.h"

class IModuleMessageManager
{
public:
	virtual ~IModuleMessageManager() {};
	virtual int SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket) = 0; //����ģ���ĵ��ù�ϵ
};

#endif