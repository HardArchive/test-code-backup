#include "ConsoleHandler.h"

CConsoleHandler::CConsoleHandler(void)
{
	m_szError[0]       = '\0';
	m_u4ConnectID      = 0;
	m_u2SendCount      = 0;
	m_u4AllRecvCount   = 0;
	m_u4AllSendCount   = 0;
	m_u4AllRecvSize    = 0;
	m_u4AllSendSize    = 0;
	m_nIOCount         = 1;
	m_u4SendThresHold  = MAX_MSG_SNEDTHRESHOLD;
	m_u2SendQueueMax   = MAX_MSG_SENDPACKET;
	m_u1ConnectState   = CONNECT_INIT;
	m_u1SendBuffState  = CONNECT_SENDNON;
	m_pTCClose         = NULL;
	m_u1IsClosing      = HANDLE_ISCLOSE_NO;
	m_pCurrMessage     = NULL;
	m_pPacketParse     = NULL;
	m_u4CurrSize       = 0;
	m_u4HandlerID      = 0;
	m_u2MaxConnectTime = 0;
}

CConsoleHandler::~CConsoleHandler(void)
{
	OUR_DEBUG((LM_ERROR, "[CConsoleHandler::~CConsoleHandler].\n"));
	SAFE_DELETE(m_pTCClose);
	OUR_DEBUG((LM_ERROR, "[CConsoleHandler::~CConsoleHandler]End.\n"));
}

const char* CConsoleHandler::GetError()
{
	return m_szError;
}

bool CConsoleHandler::Close(int nIOCount)
{
	m_ThreadLock.acquire();
	if(nIOCount > m_nIOCount)
	{
		m_nIOCount = 0;
	}

	if(m_nIOCount > 0)
	{
		m_nIOCount -= nIOCount;
	}
	m_ThreadLock.release();

	//OUR_DEBUG((LM_ERROR, "[CConsoleHandler::Close]ConnectID=%d,m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	//�ӷ�Ӧ��ע���¼�
	if(m_nIOCount == 0)
	{
		//ɾ�����󻺳��PacketParse
		if(m_pCurrMessage != NULL)
		{
			m_pCurrMessage->release();
		}

		//msg_queue()->deactivate();
		shutdown();
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::Close]Close(%d) OK.\n", GetConnectID()));
		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);

		delete this;
		return true;
	}

	return false;
}

bool CConsoleHandler::ServerClose()
{
	OUR_DEBUG((LM_ERROR, "[CConsoleHandler::ServerClose]Close(%d) OK.\n", GetConnectID()));
	//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);

	//msg_queue()->deactivate();
	shutdown();

	m_u1ConnectState = CONNECT_SERVER_CLOSE;

	return true;
}

void CConsoleHandler::SetConnectID(uint32 u4ConnectID)
{
	m_u4ConnectID = u4ConnectID;
}

uint32 CConsoleHandler::GetConnectID()
{
	return m_u4ConnectID;
}

int CConsoleHandler::open(void*)
{
	m_nIOCount = 1;

  //��ʼ��keyֵ�б�
  m_ConsoleMessage.SetConsoleKey(App_MainConfig::instance()->GetConsoleKey());

	int nRet = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open();
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].\n", nRet));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].", nRet);
		return -1;
	}

	//��������Ϊ������ģʽ
	if (this->peer().enable(ACE_NONBLOCK) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]this->peer().enable  = ACE_NONBLOCK error.");
		return -1;
	}

	//���Զ�����ӵ�ַ�Ͷ˿�
	if(this->peer().get_remote_addr(m_addrRemote) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this->peer().get_remote_addr error.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::open]this->peer().get_remote_addr error.");
		return -1;
	}

	//�ж��Ƿ��ڷ����������IP��Χ��
	if(App_MainConfig::instance()->CompareConsoleClinetIP(m_addrRemote.get_host_addr()) == false)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::open]this IP is abort.\n"));
		return -1;
	}

	OUR_DEBUG((LM_INFO, "[CConsoleHandler::open] Connection from [%s:%d]\n",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

	m_atvConnect      = ACE_OS::gettimeofday();
	m_atvInput        = ACE_OS::gettimeofday();
	m_atvOutput       = ACE_OS::gettimeofday();
	m_atvSendAlive    = ACE_OS::gettimeofday();

	m_u4AllRecvCount  = 0;
	m_u4AllSendCount  = 0;
	m_u4AllRecvSize   = 0;
	m_u4AllSendSize   = 0;

	//���ý��ջ���صĴ�С
	int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
	//ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
	ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
	//int nOverTime = MAX_MSG_SENDTIMEOUT;
	//ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_SNDTIMEO, (char* )&nOverTime, sizeof(nOverTime));

	m_pPacketParse = App_PacketParsePool::instance()->Create();
	if(NULL == m_pPacketParse)
	{
		OUR_DEBUG((LM_DEBUG,"[%t|CConnectHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
		return -1;
	}

	//����ͷ�Ĵ�С��Ӧ��mb
	m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadLen());
	if(m_pCurrMessage == NULL)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
		OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));

		return -1;
	}

	//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d].",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number());

	m_u1ConnectState = CONNECT_OPEN;


	return nRet;
}

//��������
int CConsoleHandler::handle_input(ACE_HANDLE fd)
{
	m_ThreadLock.acquire();
	m_nIOCount++;
	m_ThreadLock.release();
	//OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]ConnectID=%d,m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);

	m_atvInput = ACE_OS::gettimeofday();

	if(fd == ACE_INVALID_HANDLE)
	{
		m_u4CurrSize = 0;
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input]fd == ACE_INVALID_HANDLE.");

		return -1;
	}

	//�ж����ݰ��ṹ�Ƿ�ΪNULL
	if(m_pPacketParse == NULL)
	{
		m_u4CurrSize = 0;
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]m_pPacketParse == NULL.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input]m_pPacketParse == NULL.");

		return -1;
	}

	//�жϻ����Ƿ�ΪNULL
	if(m_pCurrMessage == NULL)
	{
		m_u4CurrSize = 0;
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input]m_pCurrMessage == NULL.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input]m_pCurrMessage == NULL.");

		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);

		return -1;
	}

	int nCurrCount = (uint32)m_pCurrMessage->size() - m_u4CurrSize;
	//������Ҫ��m_u4CurrSize���м�顣
	if(nCurrCount < 0)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input][%d] nCurrCount < 0 m_u4CurrSize = %d.\n", GetConnectID(), m_u4CurrSize));
		m_u4CurrSize = 0;

		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);

		return -1;
	}

	int nDataLen = this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);
	if(nDataLen <= 0)
	{
		m_u4CurrSize = 0;
		uint32 u4Error = (uint32)errno;
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_input] ConnectID = %d, recv data is error nDataLen = [%d] errno = [%d].\n", GetConnectID(), nDataLen, u4Error));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::handle_input] ConnectID = %d, recv data is error[%d].\n", GetConnectID(), nDataLen);

		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);

		return -1;
	}

	m_u4CurrSize += nDataLen;

	m_pCurrMessage->wr_ptr(nDataLen);

	//���û�ж��꣬�̶�
	if(m_pCurrMessage->size() > m_u4CurrSize)
	{
		Close();
		return 0;
	}
	else if(m_pCurrMessage->length() == m_pPacketParse->GetPacketHeadLen() && m_pPacketParse->GetIsHead() == false)
	{
		m_pPacketParse->SetPacketHead(m_pCurrMessage->rd_ptr(), (uint32)m_pCurrMessage->length());
		uint32 u4PacketBodyLen = m_pPacketParse->GetPacketDataLen();
		m_u4CurrSize = 0;

		//����������������ȣ�Ϊ�Ƿ�����
		if(u4PacketBodyLen >= MAX_MSG_PACKETLENGTH || u4PacketBodyLen <= 0)
		{
			m_u4CurrSize = 0;
			OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_read_stream]u4PacketHeadLen(%d) more than MAX_MSG_PACKETLENGTH.", u4PacketBodyLen));
			if(m_pPacketParse->GetMessageHead() != NULL)
			{
				m_pPacketParse->GetMessageHead()->release();
			}

			if(m_pPacketParse->GetMessageBody() != NULL)
			{
				m_pPacketParse->GetMessageBody()->release();
			}

			if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageBody() != m_pCurrMessage)
			{
				m_pCurrMessage->release();
				m_pCurrMessage = NULL;
			}
			App_PacketParsePool::instance()->Delete(m_pPacketParse);

			return -1;
		}
		else
		{
			m_pPacketParse->SetMessageHead(m_pCurrMessage);

			//����ͷ�Ĵ�С��Ӧ��mb
			m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketDataLen());
			if(m_pCurrMessage == NULL)
			{
				m_u4CurrSize = 0;
				AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
				OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));

				if(m_pPacketParse->GetMessageHead() != NULL)
				{
					m_pPacketParse->GetMessageHead()->release();
				}

				if(m_pPacketParse->GetMessageBody() != NULL)
				{
					m_pPacketParse->GetMessageBody()->release();
				}

				if(m_pCurrMessage != NULL && m_pPacketParse->GetMessageBody() != m_pCurrMessage && m_pPacketParse->GetMessageBody() != m_pCurrMessage)
				{
					m_pCurrMessage->release();
					m_pCurrMessage = NULL;
				}
				App_PacketParsePool::instance()->Delete(m_pPacketParse);

				return -1;
			}
			Close();
		}
	}
	else
	{
		//��������������ɣ���ʼ�����������ݰ�
		m_pPacketParse->SetPacketData(m_pCurrMessage->rd_ptr(), (uint32)m_pCurrMessage->length());
		m_pPacketParse->SetMessageBody(m_pCurrMessage);

		CheckMessage();

		m_u4CurrSize = 0;

		//�����µİ�
		m_pPacketParse = App_PacketParsePool::instance()->Create();
		if(NULL == m_pPacketParse)
		{
			OUR_DEBUG((LM_DEBUG,"[%t|CConnectHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
			return -1;
		}

		//����ͷ�Ĵ�С��Ӧ��mb
		m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pPacketParse->GetPacketHeadLen());
		if(m_pCurrMessage == NULL)
		{
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
			OUR_DEBUG((LM_ERROR, "[CConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));

			return -1;
		}

		Close();
	}

	return 0;
}

//�ر�����
int CConsoleHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	if(h == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_DEBUG,"[CConsoleHandler::handle_close] h is NULL mask=%d.\n", (int)mask));
	}

	OUR_DEBUG((LM_DEBUG,"[CConsoleHandler::handle_close]Connectid=[%d] begin(%d)...\n",GetConnectID(), errno));
	Close(2);

	return 0;
}

uint8 CConsoleHandler::GetConnectState()
{
	return m_u1ConnectState;
}

uint8 CConsoleHandler::GetSendBuffState()
{
	return m_u1SendBuffState;
}

uint8 CConsoleHandler::GetIsClosing()
{
	return m_u1IsClosing;
}

bool CConsoleHandler::SendMessage(IBuffPacket* pBuffPacket)
{
	m_ThreadLock.acquire();
	m_nIOCount++;
	m_ThreadLock.release();	
	//OUR_DEBUG((LM_DEBUG,"[CConsoleHandler::SendMessage]Connectid=%d,m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	CPacketParse PacketParse;

	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_DEBUG,"[CConsoleHandler::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
		Close();
		return false;
	}

	ACE_Message_Block* pMbData = NULL;

	int nSendLength = PacketParse.MakePacketLength(pBuffPacket->GetPacketLen());
	pMbData = App_MessageBlockManager::instance()->Create(nSendLength);

	//������ɷ������ݰ�
	PacketParse.MakePacket(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), pMbData);

	App_BuffPacketManager::instance()->Delete(pBuffPacket);

	PutSendPacket(pMbData);

	return true;
}

bool CConsoleHandler::PutSendPacket(ACE_Message_Block* pMbData)
{
	ACE_Time_Value     nowait(MAX_MSG_PACKETTIMEOUT);

	if(NULL == pMbData)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
		Close();
		return false;
	}

	if(get_handle() == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID()));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConsoleHandler::SendPacket] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetConnectID());
		pMbData->release();
		Close();
		return false;
	}

	//��������
	char* pData = pMbData->rd_ptr();
	if(NULL == pData)
	{
		OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, pData is NULL.\n", GetConnectID()));
		pMbData->release();
		Close();
		return false;
	}

	int nIsSendSize = 0;

	//ѭ�����ͣ�ֱ�����ݷ�����ɡ�
	while(true)
	{
		int nCurrSendSize = (int)(pMbData->length() - nIsSendSize);
		if(nCurrSendSize <= 0)
		{
			OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, nCurrSendSize error is %d.\n", GetConnectID(), nCurrSendSize));
			pMbData->release();
			return false;
		}

		int nDataLen = this->peer().send(pMbData->rd_ptr(), nCurrSendSize, &nowait);
		int nErr = ACE_OS::last_error();
		if(nDataLen <= 0)
		{
			if(nErr == EWOULDBLOCK)
			{
				//������Ͷ��������10������ٷ��͡�
				ACE_Time_Value tvSleep(0, 10 * MAX_BUFF_1000);
				ACE_OS::sleep(tvSleep);
				continue;
			}

			OUR_DEBUG((LM_ERROR, "[CConsoleHandler::SendPacket] ConnectID = %d, error = %d.\n", GetConnectID(), errno));
			pMbData->release();
			m_atvOutput      = ACE_OS::gettimeofday();
			Close();
			return false;
		}
		else if(nDataLen >= nCurrSendSize)   //�����ݰ�ȫ��������ϣ���ա�
		{
			//OUR_DEBUG((LM_ERROR, "[CConsoleHandler::handle_output] ConnectID = %d, send (%d) OK.\n", GetConnectID(), msg_queue()->is_empty()));
			m_u4AllSendCount += 1;
			m_u4AllSendSize  += (uint32)pMbData->length();
			pMbData->release();
			m_atvOutput      = ACE_OS::gettimeofday();
			Close();
			return true;
		}
		else
		{
			pMbData->rd_ptr(nDataLen);
			nIsSendSize      += nDataLen;
			m_atvOutput      = ACE_OS::gettimeofday();
			continue;
		}
	}

	return true;
}

bool CConsoleHandler::CheckMessage()
{
	m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
	m_u4AllRecvCount++;

	//���ͻظ���Ϣ
	IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();

	//������Buff������Ϣ����
	//������Buff������Ϣ���У����ݸ������ࡣ
	uint32 u4Return = (uint32)m_ConsoleMessage.Dispose(m_pPacketParse->GetMessageBody(), pBuffPacket);

	if(CONSOLE_MESSAGE_SUCCESS == u4Return)
	{
		if(pBuffPacket->GetPacketLen() > 0)
		{
			SendMessage((IBuffPacket* )pBuffPacket);
		}
	}

	App_PacketParsePool::instance()->Delete(m_pPacketParse);

	return true;
}
