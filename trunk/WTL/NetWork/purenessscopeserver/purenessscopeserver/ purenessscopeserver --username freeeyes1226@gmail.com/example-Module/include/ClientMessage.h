#ifndef _ICLIENTMESSAGE_H
#define _ICLIENTMESSAGE_H

//����һ���ӿ��࣬�������ݷ��ͳ�ȥ����������ݷ��أ��ṩ���Ӵ������
//��������RecvMessage()�������ݺ�ConnectError()���ʹ������ַ�����
//add by freeeyes 2010-12-31

#include "ace/Message_Block.h"
#include "ClientParse.h"

class IClientMessage
{
public:
	virtual ~IClientMessage() {};
	
	virtual bool RecvData(CClientParse* pClientParse) = 0;    //�������ݵĺ���
	virtual bool ConnectError(int nError)             = 0;    //�������ʱ�򣬵��ô˽ӿڷ��ش�����Ϣ  
};

#endif
