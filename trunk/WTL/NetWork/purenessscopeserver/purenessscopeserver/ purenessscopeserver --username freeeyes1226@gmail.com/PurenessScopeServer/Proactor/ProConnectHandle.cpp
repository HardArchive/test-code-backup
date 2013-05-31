#include "ProConnectHandle.h"

Mutex_Allocator _msg_prosend_mb_allocator;

CProConnectHandle::CProConnectHandle(void)
{
	m_szError[0]          = '\0';
	m_u4ConnectID         = 0;
	m_u4AllRecvCount      = 0;
	m_u4AllSendCount      = 0;
	m_u4AllRecvSize       = 0;
	m_u4AllSendSize       = 0;
	m_nIOCount            = 0;
	m_u4HandlerID         = 0;
	m_u2MaxConnectTime    = 0;
	m_u4SendThresHold     = MAX_MSG_SNEDTHRESHOLD;
	m_u2SendQueueMax      = MAX_MSG_SENDPACKET;
	m_u1ConnectState      = CONNECT_INIT;
	m_u1SendBuffState     = CONNECT_SENDNON;
	m_pPacketParse        = NULL;
	m_u4MaxPacketSize     = MAX_MSG_PACKETLENGTH;
	m_u8RecvQueueTimeCost = 0;
	m_u4RecvQueueCount    = 0;
	m_u8SendQueueTimeCost = 0;
	m_pBlockMessage       = NULL;
	m_u2SendQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000;  //Ŀǰ��Ϊ��¼����΢�룬����������Ӧ������1000��
	m_u2RecvQueueTimeout  = MAX_QUEUE_TIMEOUT * 1000;  //Ŀǰ��Ϊ��¼����΢�룬����������Ӧ������1000��
}

CProConnectHandle::~CProConnectHandle(void)
{
	if(NULL != m_pBlockMessage)
	{
		m_pBlockMessage->release();
		m_pBlockMessage = NULL;
	}
}

void CProConnectHandle::Init(uint16 u2HandlerID)
{
	m_u4HandlerID      = u2HandlerID;
	m_u2MaxConnectTime = App_MainConfig::instance()->GetMaxConnectTime();
	m_u4SendThresHold  = App_MainConfig::instance()->GetSendThresHold();
	m_u2SendQueueMax   = App_MainConfig::instance()->GetSendQueueMax();
	m_u4MaxPacketSize  = App_MainConfig::instance()->GetRecvBuffSize();

	m_u2SendQueueTimeout = App_MainConfig::instance()->GetSendQueueTimeout() * 1000;
	if(m_u2SendQueueTimeout == 0)
	{
		m_u2SendQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
	}

	m_u2RecvQueueTimeout = App_MainConfig::instance()->GetRecvQueueTimeout() * 1000;
	if(m_u2RecvQueueTimeout <= 0)
	{
		m_u2RecvQueueTimeout = MAX_QUEUE_TIMEOUT * 1000;
	}

	m_pBlockMessage      = new ACE_Message_Block(m_u4MaxPacketSize);
}

const char* CProConnectHandle::GetError()
{
	return m_szError;
}

bool CProConnectHandle::Close(int nIOCount)
{
	m_ThreadWriteLock.acquire();
	if(nIOCount > m_nIOCount)
	{
		m_nIOCount = 0;
	}

	//OUR_DEBUG((LM_DEBUG, "[CProConnectHandle::Close]ConnectID=%d, m_nIOCount = %d, nIOCount = %d.\n", GetConnectID(), m_nIOCount, nIOCount));

	if(m_nIOCount > 0)
	{
		m_nIOCount -= nIOCount;
	}
	m_ThreadWriteLock.release();

	if(m_nIOCount == 0)
	{
		m_ThreadWriteLock.acquire();

		m_Reader.cancel();
		m_Writer.cancel();

		if(this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket(this->handle());
			this->handle(ACE_INVALID_HANDLE);
		}

		m_ThreadWriteLock.release();

		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::Close] Close(%d) delete OK.\n", GetConnectID()));

		//ɾ�������б��еĶ�������
		App_ProConnectManager::instance()->Close(GetConnectID());

		//������ָ�����ճ���
		App_ProConnectHandlerPool::instance()->Delete(this);

		return true;
	}

	return false;
}

bool CProConnectHandle::ServerClose()
{
	OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::ServerClose] Close(%d) delete OK.\n", GetConnectID()));

	//���ͷ����������ӶϿ���Ϣ��
	if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
		return false;
	}

	m_Reader.cancel();
	m_Writer.cancel();

	if(this->handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::closesocket(this->handle());
		this->handle(ACE_INVALID_HANDLE);
	}

	m_u1ConnectState = CONNECT_SERVER_CLOSE;

	return true;
}

void CProConnectHandle::SetConnectID(uint32 u4ConnectID)
{
	m_u4ConnectID = u4ConnectID;
}

uint32 CProConnectHandle::GetConnectID()
{
	return m_u4ConnectID;
}

void CProConnectHandle::addresses (const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address)
{
	m_addrRemote = remote_address;
}

void CProConnectHandle::open(ACE_HANDLE h, ACE_Message_Block&)
{
	ACE_Time_Value tvOpenBegin(ACE_OS::gettimeofday());

	OUR_DEBUG((LM_INFO, "[CProConnectHandle::open] [0x%08x]Connection from [%s:%d]\n", this, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

	m_atvConnect      = ACE_OS::gettimeofday();
	m_atvInput        = ACE_OS::gettimeofday();
	m_atvOutput       = ACE_OS::gettimeofday();
	m_atvSendAlive    = ACE_OS::gettimeofday();

	m_u4AllRecvCount      = 0;
	m_u4AllSendCount      = 0;
	m_u4AllRecvSize       = 0;
	m_u4AllSendSize       = 0;
	m_blCanWrite          = true;
	m_blTimeClose         = false;
	m_u4RecvPacketCount   = 0;
	m_nIOCount            = 1;
	m_u8RecvQueueTimeCost = 0;
	m_u4RecvQueueCount    = 0;
	m_u8SendQueueTimeCost = 0;

	ACE_Time_Value tvOpenEnd(ACE_OS::gettimeofday());
	ACE_Time_Value tvOpen(tvOpenEnd - tvOpenBegin);

	if(App_ForbiddenIP::instance()->CheckIP(m_addrRemote.get_host_addr()) == false)
	{
		//�ڽ�ֹ�б��У����������
		OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]IP Forbidden(%s).\n", m_addrRemote.get_host_addr()));
		return;
	}

	//��鵥λʱ�����Ӵ����Ƿ�ﵽ����
	if(false == App_IPAccount::instance()->AddIP((string)m_addrRemote.get_host_addr()))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open]IP connect frequently.\n", m_addrRemote.get_host_addr()));
		App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), App_MainConfig::instance()->GetForbiddenTime());
		Close();
		return;
	}

	//��ʼ�������
	m_TimeConnectInfo.Init(App_MainConfig::instance()->GetValid(), App_MainConfig::instance()->GetValidPacketCount(), App_MainConfig::instance()->GetValidRecvSize());


	AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] DisposeTime = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), tvOpen.msec());

	this->handle(h);

	if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1 || 
		this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader or m_reader == 0.\n"));	
		Close();
		return;
	}

	//ACE_Sig_Action writeAction((ACE_SignalHandler)SIG_IGN);
	//writeAction.register_action(SIGPIPE, 0);

	//int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
	//ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
	//ACE_OS::setsockopt(h, SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));

	//��������ӷ������ӿ�
	if(false == App_ProConnectManager::instance()->AddConnect(this))
	{
		OUR_DEBUG((LM_ERROR, "%s.\n", App_ProConnectManager::instance()->GetError()));
		sprintf_safe(m_szError, MAX_BUFF_500, "%s", App_ProConnectManager::instance()->GetError());
		Close();
		return;
	}

	m_u1ConnectState = CONNECT_OPEN;

	OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] Open(%d).\n", GetConnectID()));	

	m_pPacketParse = App_PacketParsePool::instance()->Create();
	if(NULL == m_pPacketParse)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
		Close();
		return;
	}

	//�������ӽ�����Ϣ��
	if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CONNECT, NULL))
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
	}

	RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());

	return;
}

void CProConnectHandle::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_Message_Block& mb = result.message_block();
	uint32 u4PacketLen = (uint32)result.bytes_transferred();
	int nTran = (int)result.bytes_transferred();

	if(!result.success() || result.bytes_transferred() == 0)
	{
		//���ӶϿ�
		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
			m_pPacketParse->SetMessageHead(NULL);
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
			m_pPacketParse->SetMessageBody(NULL);
		}

		if(&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
		{
			//OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Message_block release.\n"));
			App_MessageBlockManager::instance()->Close(&mb);
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);

		OUR_DEBUG((LM_DEBUG,"[%tCConnectHandler::handle_read_stream]Connectid=[%d] error(%d)...\n", GetConnectID(), errno));
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
		//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

		if(m_u1ConnectState != CONNECT_SERVER_CLOSE)
		{
			//���Ϳͻ������ӶϿ���Ϣ��
			if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CDISCONNECT, NULL))
			{
				OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
			}
		}

		Close(2);

		return;
	}

	m_atvInput = ACE_OS::gettimeofday();

	if(result.bytes_transferred() < result.bytes_to_read())
	{
		//�̶���������
		int nRead = (int)result.bytes_to_read() - (int)result.bytes_transferred();
		if(-1 == m_Reader.read(mb, nRead))
		{
			if(m_pPacketParse->GetMessageHead() != NULL)
			{
				App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
				m_pPacketParse->SetMessageHead(NULL);
			}

			if(m_pPacketParse->GetMessageBody() != NULL)
			{
				App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
				m_pPacketParse->SetMessageBody(NULL);
			}

			if(&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
			{
				App_MessageBlockManager::instance()->Close(&mb);
			}
			App_PacketParsePool::instance()->Delete(m_pPacketParse);

			OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]Read Shoter error(%d).", errno));
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
			//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

			if(m_u1ConnectState != CONNECT_SERVER_CLOSE)
			{
				//���Ϳͻ������ӶϿ���Ϣ��
				if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_CDISCONNECT, NULL))
				{
					OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
				}
			}

			Close(2);
			return;
		}

	}
	else if(mb.length() == m_pPacketParse->GetPacketHeadLen() && m_pPacketParse->GetIsHead() == false)
	{
		//�ж�ͷ�ĺϷ���
		m_pPacketParse->SetPacketHead(mb.rd_ptr(), (uint32)mb.length());
		uint32 u4PacketBodyLen = m_pPacketParse->GetPacketDataLen();

		//����������������ȣ�Ϊ�Ƿ�����
		if(u4PacketBodyLen >= m_u4MaxPacketSize || u4PacketBodyLen <= 0)
		{
			OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than %d.\n", u4PacketBodyLen, m_u4MaxPacketSize));

			if(m_pPacketParse->GetMessageHead() != NULL)
			{
				App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageHead());
				m_pPacketParse->SetMessageHead(NULL);
			}

			if(m_pPacketParse->GetMessageBody() != NULL)
			{
				App_MessageBlockManager::instance()->Close(m_pPacketParse->GetMessageBody());
				m_pPacketParse->SetMessageBody(NULL);
			}

			if(&mb != m_pPacketParse->GetMessageHead() && &mb != m_pPacketParse->GetMessageBody())
			{
				App_MessageBlockManager::instance()->Close(&mb);
			}
			App_PacketParsePool::instance()->Delete(m_pPacketParse);

			//���ͷ����������ӶϿ���Ϣ��
			if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
			{
				OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
			}

			Close(2);
			return;
		}
		else
		{
			m_pPacketParse->SetMessageHead(&mb);
			Close();
			RecvClinetPacket(u4PacketBodyLen);
		}
	}
	else
	{
		//��������������ɣ���ʼ�����������ݰ�
		m_pPacketParse->SetPacketData(mb.rd_ptr(), (uint32)mb.length());
		m_pPacketParse->SetMessageBody(&mb);

		if(false == CheckMessage())
		{
			Close(2);
			return;
		}

		m_pPacketParse = App_PacketParsePool::instance()->Create();
		if(NULL == m_pPacketParse)
		{
			OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
			//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ
			//���ͷ����������ӶϿ���Ϣ��
			if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_SDISCONNECT, NULL))
			{
				OUR_DEBUG((LM_ERROR, "[CProConnectHandle::open] ConnectID = %d, PACKET_CONNECT is error.\n", GetConnectID()));
			}

			Close(2);
			return;
		}
		Close();

		//������һ�����ݰ�
		RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());
	}

	return;
}

void CProConnectHandle::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	if(!result.success() || result.bytes_transferred()==0)
	{
		//���ӶϿ�
		OUR_DEBUG ((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), errno));

		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64dns, SendQueueTimeCost=%I64dns.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);

		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		Close();
		return;
	}
	else
	{
		//�̶�������
		m_ThreadWriteLock.acquire();
		m_blCanWrite = true;
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		m_u4AllSendSize += (uint32)result.bytes_to_write();
		m_ThreadWriteLock.release();
		Close();
		return;
	}
}

bool CProConnectHandle::SetRecvQueueTimeCost(uint32 u4TimeCost)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();

	//���������ֵ�����¼����־��ȥ
	if((uint32)(m_u2RecvQueueTimeout * 1000) <= u4TimeCost)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_RECVQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, u4TimeCost);
	}

	//OUR_DEBUG((LM_ERROR, "[CProConnectHandle::SetRecvQueueTimeCost]m_u4RecvQueueCount=%d.\n", m_u4RecvQueueCount));
	m_u4RecvQueueCount++;

	//�����˼��죬�о����ʱ�����壬��Ϊ��ȡ���еĴ���ʱ��Ƭ���ܺܺ�ʱ������һ�����ݵĽ׶���ʱ������
	//ֻҪ��¼��ʱ�����ݼ���
	//m_u8RecvQueueTimeCost += u4TimeCost;

	Close();
	return true;
}

bool CProConnectHandle::SetSendQueueTimeCost(uint32 u4TimeCost)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();

	//���������ֵ�����¼����־��ȥ
	if((uint32)(m_u2SendQueueTimeout * 1000) <= u4TimeCost)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_SENDQUEUEERROR, "[TCP]IP=%s,Prot=%d,Timeout=[%d].", GetClientIPInfo().m_szClientIP, GetClientIPInfo().m_nPort, u4TimeCost);
	}

	//m_u8SendQueueTimeCost += u4TimeCost;

	Close();
	return true;
}

uint8 CProConnectHandle::GetConnectState()
{
	return m_u1ConnectState;
}

uint8 CProConnectHandle::GetSendBuffState()
{
	return m_u1SendBuffState;
}

bool CProConnectHandle::GetIsClosing()
{
	return m_blTimeClose;
}

bool CProConnectHandle::SendMessage(IBuffPacket* pBuffPacket, bool blState, uint8 u1SendType, uint32& u4PacketSize)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();	
	//OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage]Connectid=%d,m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	CPacketParse PacketParse;
	ACE_Message_Block* pMbData = NULL;

	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
		Close();
		return false;
	}

	//�������ֱ�ӷ������ݣ���ƴ�����ݰ�
	if(u1SendType == SENDMESSAGE_NOMAL)
	{
		//���ж�Ҫ���͵����ݳ��ȣ������Ƿ���Է��뻺�壬�����Ƿ��Ѿ�������
		uint32 u4SendPacketSize = 0;
		if(u1SendType == SENDMESSAGE_NOMAL)
		{
			u4SendPacketSize = PacketParse.MakePacketLength(pBuffPacket->GetPacketLen());
		}
		else
		{
			u4SendPacketSize = (uint32)m_pBlockMessage->length();
		}

		if(u4SendPacketSize + (uint32)m_pBlockMessage->length() >= m_u4MaxPacketSize)
		{
			OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] m_pBlockMessage is not enougth.\n", GetConnectID()));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			Close();
			return false;
		}
		else
		{
			//��ӽ�������
			ACE_Message_Block* pMbBufferData = NULL;

			//SENDMESSAGE_NOMAL����Ҫ��ͷ��ʱ�򣬷��򣬲����ֱ�ӷ���
			if(u1SendType == SENDMESSAGE_NOMAL)
			{
				//������ɷ������ݰ�
				PacketParse.MakePacket(pBuffPacket->GetData(), pBuffPacket->GetPacketLen(), m_pBlockMessage);
			}
			else
			{
				//�������SENDMESSAGE_NOMAL����ֱ�����
				ACE_OS::memcpy(m_pBlockMessage->wr_ptr(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen());
				m_pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());
			}
		}

		//����������̷��ͣ�������ݿ���������������ȥ
		if(false == blState)
		{
			//ɾ���������ݰ� 
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			Close();
			return true;
		}

		//��Ϊ���첽���ͣ����͵�����ָ�벻���������ͷţ�������Ҫ�����ﴴ��һ���µķ������ݿ飬�����ݿ���
		pMbData = App_MessageBlockManager::instance()->Create((uint32)m_pBlockMessage->length());
		if(NULL == pMbData)
		{
			OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pMbData is NULL.\n", GetConnectID()));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			Close();
			return false;
		}

		ACE_OS::memcpy(pMbData->wr_ptr(), m_pBlockMessage->rd_ptr(), m_pBlockMessage->length());
		pMbData->wr_ptr(m_pBlockMessage->length());
		//������ɣ�����ջ������ݣ�ʹ�����
		m_pBlockMessage->reset();

		//ɾ���������ݰ� 
		App_BuffPacketManager::instance()->Delete(pBuffPacket);
	}
	else
	{
		//���֮ǰ�л������ݣ���ͻ�������һ����
		if(m_pBlockMessage->length() > 0)
		{
			ACE_OS::memcpy(m_pBlockMessage->wr_ptr(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen());
			m_pBlockMessage->wr_ptr(pBuffPacket->GetPacketLen());

			//��Ϊ���첽���ͣ����͵�����ָ�벻���������ͷţ�������Ҫ�����ﴴ��һ���µķ������ݿ飬�����ݿ���
			pMbData = App_MessageBlockManager::instance()->Create((uint32)m_pBlockMessage->length());
			if(NULL == pMbData)
			{
				OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pMbData is NULL.\n", GetConnectID()));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				Close();
				return false;
			}

			ACE_OS::memcpy(pMbData->wr_ptr(), m_pBlockMessage->rd_ptr(), m_pBlockMessage->length());
			pMbData->wr_ptr(m_pBlockMessage->length());
			//������ɣ�����ջ������ݣ�ʹ�����
			m_pBlockMessage->reset();
		}
		else
		{
			pMbData = App_MessageBlockManager::instance()->Create((uint32)m_pBlockMessage->length());
			if(NULL == pMbData)
			{
				OUR_DEBUG((LM_DEBUG,"[CConnectHandler::SendMessage] Connectid=[%d] pMbData is NULL.\n", GetConnectID()));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				Close();
				return false;
			}

			ACE_OS::memcpy(pMbData->wr_ptr(), pBuffPacket->GetData(), pBuffPacket->GetPacketLen());
			pMbData->wr_ptr(pBuffPacket->GetPacketLen());
		}

		//ɾ���������ݰ� 
		App_BuffPacketManager::instance()->Delete(pBuffPacket);
	}

	return PutSendPacket(pMbData);
}

bool CProConnectHandle::CheckAlive()
{
	ACE_Time_Value tvNow = ACE_OS::gettimeofday();
	ACE_Time_Value tvIntval(tvNow - m_atvInput);
	if(tvIntval.sec() > m_u2MaxConnectTime)
	{
		//������������ʱ�䣬��������ر�����
		OUR_DEBUG ((LM_ERROR, "[CProConnectHandle::CheckAlive] Connectid=%d Server Close!\n", GetConnectID()));
		ServerClose();
		return false;
	}
	else
	{
		return true;
	}
}

bool CProConnectHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
	int nSendSize = m_u4AllSendSize;

	//OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d, m_nIOCount=%d!\n", GetConnectID(), m_nIOCount));

	//�첽���ͷ���
	if(NULL != pMbData)
	{
		if(0 != m_Writer.write(*pMbData, pMbData->length()))
		{
			OUR_DEBUG ((LM_ERROR, "[CConnectHandler::PutSendPacket] Connectid=%d mb=%d m_writer.write error(%d)!\n", GetConnectID(),  pMbData->length(), errno));
			pMbData->release();
			Close();
			return false;
		}
		else
		{
			m_ThreadWriteLock.acquire();
			m_u4AllSendCount += 1;
			m_atvOutput      = ACE_OS::gettimeofday();
			m_ThreadWriteLock.release();
			//Close();
			return true;
		}
		//pMbData->release();
	}
	else
	{
		OUR_DEBUG ((LM_ERROR,"[CConnectHandler::PutSendPacket] Connectid=%d mb is NULL!\n", GetConnectID()));
		Close();
		return false;
	}
}

bool CProConnectHandle::RecvClinetPacket(uint32 u4PackeLen)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();
	//OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket]Connectid=%d, m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	ACE_Message_Block* pmb = NULL;
	pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);
	if(pmb == NULL)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket] pmb new is NULL.\n"));
		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);
		Close(2);
		return false;
	}

	if(m_Reader.read(*pmb, u4PackeLen) == -1)
	{
		//�����ʧ�ܣ���ر����ӡ�
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d, RecvQueueCount=%d, RecvQueueTimeCost=%I64d, SendQueueTimeCost=%I64d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount, m_u4RecvQueueCount, m_u8RecvQueueTimeCost, m_u8SendQueueTimeCost);
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));
		pmb->release();
		if(m_pPacketParse->GetMessageHead() != NULL)
		{
			m_pPacketParse->GetMessageHead()->release();
		}

		if(m_pPacketParse->GetMessageBody() != NULL)
		{
			m_pPacketParse->GetMessageBody()->release();
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);
		Close(2);
		return false;
	}

	return true;
}

bool CProConnectHandle::CheckMessage()
{
	if(m_pPacketParse->GetMessageHead() != NULL && m_pPacketParse->GetMessageBody() != NULL)
	{
		m_ThreadWriteLock.acquire();
		m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
		m_u4AllRecvCount++;
		m_ThreadWriteLock.release();

		ACE_Date_Time dtNow;
		if(false == m_TimeConnectInfo.Check((uint8)dtNow.minute(), 1, m_u4AllRecvSize))
		{
			//�������޶��ķ�ֵ����Ҫ�ر����ӣ�����¼��־
			AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECTABNORMAL, "[TCP]IP=%s,Prot=%d,PacketCount=%d, RecvSize=%d.", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_TimeConnectInfo.m_u4PacketCount, m_TimeConnectInfo.m_u4RecvSize);
			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			//���÷��ʱ��
			App_ForbiddenIP::instance()->AddTempIP(m_addrRemote.get_host_addr(), App_MainConfig::instance()->GetForbiddenTime());
			OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is check invalid.\n", GetConnectID()));
			return false;
		}

		//������Buff������Ϣ���У����ݸ�MakePacket����
		if(false == App_MakePacket::instance()->PutMessageBlock(GetConnectID(), PACKET_PARSE, m_pPacketParse))
		{
			App_PacketParsePool::instance()->Delete(m_pPacketParse);
			OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, PutMessageBlock is error.\n", GetConnectID()));
		}
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectHandle::CheckMessage] ConnectID = %d, m_pPacketParse is NULL.\n", GetConnectID()));
	}

	return true;
}

_ClientConnectInfo CProConnectHandle::GetClientInfo()
{
	_ClientConnectInfo ClientConnectInfo;

	ClientConnectInfo.m_blValid             = true;
	ClientConnectInfo.m_u4ConnectID         = GetConnectID();
	ClientConnectInfo.m_addrRemote          = m_addrRemote;
	ClientConnectInfo.m_u4RecvCount         = m_u4AllRecvCount;
	ClientConnectInfo.m_u4SendCount         = m_u4AllSendCount;
	ClientConnectInfo.m_u4AllRecvSize       = m_u4AllSendSize;
	ClientConnectInfo.m_u4AllSendSize       = m_u4AllSendSize;
	ClientConnectInfo.m_u4BeginTime         = (uint32)m_atvConnect.sec();
	ClientConnectInfo.m_u4AliveTime         = (uint32)(ACE_OS::gettimeofday().sec()  -  m_atvConnect.sec());
	ClientConnectInfo.m_u4RecvQueueCount    = m_u4RecvQueueCount;
	ClientConnectInfo.m_u8RecvQueueTimeCost = m_u8RecvQueueTimeCost;
	ClientConnectInfo.m_u8SendQueueTimeCost = m_u8SendQueueTimeCost;

	return ClientConnectInfo;
}

_ClientIPInfo CProConnectHandle::GetClientIPInfo()
{
	_ClientIPInfo ClientIPInfo;
	sprintf_safe(ClientIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_addrRemote.get_host_addr());
	ClientIPInfo.m_nPort = (int)m_addrRemote.get_port_number();
	return ClientIPInfo;
}

//***************************************************************************
CProConnectManager::CProConnectManager(void)
{
	m_u4TimeCheckID      = 0;
	m_u4ConnectCurrID    = 0;
	m_szError[0]         = '\0';
	m_blRun              = false;

	m_tvCheckConnect     = ACE_OS::gettimeofday();

	m_SendMessagePool.Init();
}

CProConnectManager::~CProConnectManager(void)
{
	CloseAll();
}

void CProConnectManager::CloseAll()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	KillTimer();
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
		if(pConnectHandler != NULL)
		{
			pConnectHandler->Close();
		}
	}

	m_mapConnectManager.clear();
}

bool CProConnectManager::Close(uint32 u4ConnectID)
{
	//�ͻ��˹ر�
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		m_mapConnectManager.erase(f);
		return true;
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::Close] ConnectID[%d] is not find.", u4ConnectID);
		return true;
	}
}

bool CProConnectManager::CloseConnect(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	//�������ر�
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		m_mapConnectManager.erase(f);
		if(pConnectHandler != NULL)
		{
			pConnectHandler->ServerClose();
		}
		return true;
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::CloseConnect] ConnectID[%d] is not find.", u4ConnectID);
		return true;
	}
}

bool CProConnectManager::AddConnect(uint32 u4ConnectID, CProConnectHandle* pConnectHandler)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	if(pConnectHandler == NULL)
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] pConnectHandler is NULL.");
		return false;		
	}

	m_ThreadWriteLock.acquire();
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);
	if(f != m_mapConnectManager.end())
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::AddConnect] ConnectID[%d] is exist.", u4ConnectID);
		m_ThreadWriteLock.release();
		return false;
	}

	pConnectHandler->SetConnectID(u4ConnectID);
	//����map
	m_mapConnectManager.insert(mapConnectManager::value_type(u4ConnectID, pConnectHandler));
	//m_u4ConnectCurrID++;
	m_ThreadWriteLock.release();

	return true;
}

bool CProConnectManager::SendMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint16 u2CommandID, bool blSendState, uint8 u1SendType, ACE_hrtime_t& tvSendBegin)
{
	//ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	//OUR_DEBUG((LM_ERROR,"[CProConnectManager::SendMessage]BEGIN.\n"));

	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		uint32 u4CommandSize = pBuffPacket->GetPacketLen();

		if(NULL != pConnectHandler)
		{
			uint32 u4PacketSize  = 0;
			pConnectHandler->SendMessage(pBuffPacket, blSendState, u1SendType, u4PacketSize);

			//��¼��Ϣ��������ʱ��
			//uint32 u4SendCost = (uint32)(ACE_OS::gethrtime() - tvSendBegin);
			uint32 u4SendCost = 0;
			//pConnectHandler->SetSendQueueTimeCost(u4SendCost);
			App_CommandAccount::instance()->SaveCommandData(u2CommandID, (uint8)u4SendCost, PACKET_TCP, u4PacketSize, u4CommandSize, COMMAND_TYPE_OUT);
			return true;
		}
		else
		{
			sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::SendMessage] ConnectID[%d] is not find.", u4ConnectID);
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			return true;
		}
	}
	else
	{
		sprintf_safe(m_szError, MAX_BUFF_500, "[CProConnectManager::SendMessage] ConnectID[%d] is not find.", u4ConnectID);
		//OUR_DEBUG((LM_ERROR,"[CProConnectManager::SendMessage]%s.\n", m_szError));
		App_BuffPacketManager::instance()->Delete(pBuffPacket);
		return true;
	}

	return true;
}

bool CProConnectManager::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState)
{
	//OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage]BEGIN.\n"));
	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] pBuffPacket is NULL.\n"));
		return false;
	}

	//ACE_Message_Block* mb = App_MessageBlockManager::instance()->Create(sizeof(_SendMessage*));
	ACE_Message_Block* mb = NULL;

	ACE_NEW_MALLOC_NORETURN(mb, 
		static_cast<ACE_Message_Block*>(_msg_prosend_mb_allocator.malloc(sizeof(ACE_Message_Block))),
		ACE_Message_Block(sizeof(_SendMessage*), // size
		ACE_Message_Block::MB_DATA, // type
		0,
		0,
		&_msg_prosend_mb_allocator, // allocator_strategy
		0, // locking strategy
		ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
		ACE_Time_Value::zero,
		ACE_Time_Value::max_time,
		&_msg_prosend_mb_allocator,
		&_msg_prosend_mb_allocator
		));

	if(NULL != mb)
	{
		//���뷢�Ͷ���
		_SendMessage* pSendMessage = m_SendMessagePool.Create();

		if(NULL == pSendMessage)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] new _SendMessage is error.\n"));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			return false;
		}

		pSendMessage->m_u4ConnectID = u4ConnectID;
		pSendMessage->m_pBuffPacket = pBuffPacket;
		pSendMessage->m_nEvents     = u1SendType;
		pSendMessage->m_u2CommandID = u2CommandID;
		pSendMessage->m_blSendState = blSendState;
		pSendMessage->m_tvSend      = ACE_OS::gethrtime();

		_SendMessage** ppSendMessage = (_SendMessage **)mb->base();
		*ppSendMessage = pSendMessage;

		//�ж϶����Ƿ����Ѿ����
		int nQueueCount = (int)msg_queue()->message_count();
		if(nQueueCount >= (int)MAX_MSG_THREADQUEUE)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			mb->release();
			return false;
		}

		ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, m_u4SendQueuePutTime);
		if(this->putq(mb, &xtime) == -1)
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			mb->release();
			return false;
		}
	}
	else
	{
		OUR_DEBUG((LM_ERROR,"[CMessageService::PutMessage] mb new error.\n"));
		App_BuffPacketManager::instance()->Delete(pBuffPacket);
		return false;
	}

	return true;
}

const char* CProConnectManager::GetError()
{
	return m_szError;
}

bool CProConnectManager::StartTimer()
{
	//���������߳�
	if(0 != open())
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectManager::StartTimer]Open() is error.\n"));
		return false;
	}

	//���ⶨʱ���ظ�����
	KillTimer();
	OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()-->begin....\n"));

	//������ӷ��ʹ�����
	uint16 u2SendAlive = App_MainConfig::instance()->GetSendAliveTime();
	m_u4TimeCheckID = App_TimerManager::instance()->schedule(this, (void *)NULL, ACE_OS::gettimeofday() + ACE_Time_Value(u2SendAlive), ACE_Time_Value(u2SendAlive));
	if(-1 == m_u4TimeCheckID)
	{
		OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread m_u4TimeCheckID error.\n"));
		return false;
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "CProConnectManager::StartTimer()--> Start thread time OK.\n"));
		return true;
	}
}

bool CProConnectManager::KillTimer()
{
	if(m_u4TimeCheckID > 0)
	{
		App_TimerManager::instance()->cancel(m_u4TimeCheckID);
		m_u4TimeCheckID = 0;
	}

	return true;
}

int CProConnectManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

	//Ϊ�˷�ֹ���߳��µ�����ɾ�����⣬�Ȱ����е�����ID���������������������������߳̾����Ļ��ᡣ
	if(m_mapConnectManager.size() != 0)
	{
		for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end();)
		{
			CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
			if(pConnectHandler != NULL)
			{
				if(false == pConnectHandler->CheckAlive())
				{
					//ɾ���ͷŶ���
					m_mapConnectManager.erase(b++);
				}
				else
				{
					b++;
				}
			}
			else
			{
				b++;
			}
		}
	}

	//�ж��Ƿ�Ӧ�ü�¼������־
	ACE_Time_Value tvNow = ACE_OS::gettimeofday();
	ACE_Time_Value tvInterval(tvNow - m_tvCheckConnect);
	if(tvInterval.sec() >= MAX_MSG_HANDLETIME)
	{
		AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "[CProConnectManager]CurrConnectCount = %d.", GetCount());
		m_tvCheckConnect = tvNow;
	}

	return 0;
}

int CProConnectManager::GetCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	return (int)m_mapConnectManager.size(); 
}

int CProConnectManager::open(void* args)
{
	m_blRun = true;
	msg_queue()->high_water_mark(MAX_MSG_MASK);
	msg_queue()->low_water_mark(MAX_MSG_MASK);

	OUR_DEBUG((LM_INFO,"[CProConnectManager::open] m_u4HighMask = [%d] m_u4LowMask = [%d]\n", MAX_MSG_MASK, MAX_MSG_MASK));
	if(activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED | THR_SUSPENDED, MAX_MSG_THREADCOUNT) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CProConnectManager::open] activate error ThreadCount = [%d].", MAX_MSG_THREADCOUNT));
		m_blRun = false;
		return -1;
	}

	m_u4SendQueuePutTime = App_MainConfig::instance()->GetSendQueuePutTime() * 1000;

	resume();

	return 0;
}

int CProConnectManager::svc (void)
{
	ACE_Message_Block* mb = NULL;
	ACE_Time_Value xtime;

	while(IsRun())
	{
		mb = NULL;
		//xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
		if(getq(mb, 0) == -1)
		{
			OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] getq is error[%d]!\n", errno));
			m_blRun = false;
			break;
		}
		if (mb == NULL)
		{
			continue;
		}
		_SendMessage* msg = *((_SendMessage**)mb->base());
		if (! msg)
		{
			mb->release();
			continue;
		}

		//����������
		SendMessage(msg->m_u4ConnectID, msg->m_pBuffPacket, msg->m_u2CommandID, msg->m_blSendState, msg->m_nEvents, msg->m_tvSend);
		m_SendMessagePool.Delete(msg);

		mb->release();
	}

	OUR_DEBUG((LM_INFO,"[CProConnectManager::svc] svc finish!\n"));
	return 0;
}

bool CProConnectManager::IsRun()
{
	return m_blRun;
}

int CProConnectManager::close(u_long)
{
	m_blRun = false;
	OUR_DEBUG((LM_INFO,"[CProConnectManager::close] close().\n"));
	return 0;
}

void CProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )b->second;
		if(pConnectHandler != NULL)
		{
			VecClientConnectInfo.push_back(pConnectHandler->GetClientInfo());
		}
	}
}

void CProConnectManager::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			pConnectHandler->SetRecvQueueTimeCost(u4TimeCost);
		}
	}
}

_ClientIPInfo CProConnectManager::GetClientIPInfo(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);
	mapConnectManager::iterator f = m_mapConnectManager.find(u4ConnectID);

	if(f != m_mapConnectManager.end())
	{
		CProConnectHandle* pConnectHandler = (CProConnectHandle* )f->second;
		if(NULL != pConnectHandler)
		{
			return pConnectHandler->GetClientIPInfo();
		}
		else
		{
			_ClientIPInfo ClientIPInfo;
			return ClientIPInfo;
		}
	}
	else
	{
		_ClientIPInfo ClientIPInfo;
		return ClientIPInfo;
	}
}

bool CProConnectManager::PostMessageAll( IBuffPacket* pBuffPacket, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	m_ThreadWriteLock.acquire();
	vecConnectManager objveCProConnectManager;
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		objveCProConnectManager.push_back((uint32)b->first);
	}
	m_ThreadWriteLock.release();

	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)objveCProConnectManager.size(); i++)
	{
		IBuffPacket* pCurrBuffPacket = App_BuffPacketManager::instance()->Create();
		if(NULL == pCurrBuffPacket)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManager::PostMessage]pCurrBuffPacket is NULL.\n"));
			App_BuffPacketManager::instance()->Delete(pBuffPacket);
			return false;
		}

		pCurrBuffPacket->WriteStream(pBuffPacket->GetData(), pBuffPacket->GetPacketLen());

		u4ConnectID = objveCProConnectManager[i];
		ACE_Message_Block* mb = NULL;

		ACE_NEW_MALLOC_NORETURN(mb, 
			static_cast<ACE_Message_Block*>(_msg_prosend_mb_allocator.malloc(sizeof(ACE_Message_Block))),
			ACE_Message_Block(sizeof(_SendMessage*), // size
			ACE_Message_Block::MB_DATA, // type
			0,
			0,
			&_msg_prosend_mb_allocator, // allocator_strategy
			0, // locking strategy
			ACE_DEFAULT_MESSAGE_BLOCK_PRIORITY, // priority
			ACE_Time_Value::zero,
			ACE_Time_Value::max_time,
			&_msg_prosend_mb_allocator,
			&_msg_prosend_mb_allocator
			));

		if(NULL != mb)
		{
			//���뷢�Ͷ���
			_SendMessage* pSendMessage = m_SendMessagePool.Create();

			if(NULL == pSendMessage)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] new _SendMessage is error.\n"));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				return false;
			}

			pSendMessage->m_u4ConnectID = u4ConnectID;
			pSendMessage->m_pBuffPacket = pCurrBuffPacket;
			pSendMessage->m_nEvents     = u1SendType;
			pSendMessage->m_u2CommandID = u2CommandID;
			pSendMessage->m_blSendState = blSendState;
			pSendMessage->m_tvSend      = ACE_OS::gethrtime();

			_SendMessage** ppSendMessage = (_SendMessage **)mb->base();
			*ppSendMessage = pSendMessage;

			//�ж϶����Ƿ����Ѿ����
			int nQueueCount = (int)msg_queue()->message_count();
			if(nQueueCount >= (int)MAX_MSG_THREADQUEUE)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue is Full nQueueCount = [%d].\n", nQueueCount));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				m_SendMessagePool.Delete(pSendMessage);
				mb->release();
				return false;
			}

			ACE_Time_Value xtime = ACE_OS::gettimeofday() + ACE_Time_Value(0, MAX_MSG_PUTTIMEOUT);
			if(this->putq(mb, &xtime) == -1)
			{
				OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] Queue putq  error nQueueCount = [%d] errno = [%d].\n", nQueueCount, errno));
				App_BuffPacketManager::instance()->Delete(pBuffPacket);
				m_SendMessagePool.Delete(pSendMessage);
				mb->release();
				return false;
			}
		}
		else
		{
			OUR_DEBUG((LM_ERROR,"[CProConnectManager::PutMessage] mb new error.\n"));
			return false;
		}
	}

	return true;
}

//*********************************************************************************

CProConnectHandlerPool::CProConnectHandlerPool(void)
{
	m_u4CurrMaxCount = 0;
}

CProConnectHandlerPool::~CProConnectHandlerPool(void)
{
	Close();
}

void CProConnectHandlerPool::Init(int nObjcetCount)
{
	Close();

	for(int i = 0; i < nObjcetCount; i++)
	{
		CProConnectHandle* pPacket = new CProConnectHandle();
		if(NULL != pPacket)
		{
			//��ӵ�Free map����
			mapHandle::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				pPacket->Init(m_u4CurrMaxCount);
				m_mapMessageFree.insert(mapHandle::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
	}
}

void CProConnectHandlerPool::Close()
{
	//���������Ѵ��ڵ�ָ��
	mapHandle::iterator itorFreeB = m_mapMessageFree.begin();
	mapHandle::iterator itorFreeE = m_mapMessageFree.end();

	for(itorFreeB; itorFreeB != itorFreeE; itorFreeB++)
	{
		CProConnectHandle* pObject = (CProConnectHandle* )itorFreeB->second;
		SAFE_DELETE(pObject);
	}

	mapHandle::iterator itorUsedB = m_mapMessageUsed.begin();
	mapHandle::iterator itorUsedE = m_mapMessageUsed.end();

	for(itorUsedB; itorUsedB != itorUsedE; itorUsedB++)
	{
		CProConnectHandle* pPacket = (CProConnectHandle* )itorUsedB->second;
		SAFE_DELETE(pPacket);
	}

	m_u4CurrMaxCount = 0;
	m_mapMessageFree.clear();
	m_mapMessageUsed.clear();
}

int CProConnectHandlerPool::GetUsedCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageUsed.size();
}

int CProConnectHandlerPool::GetFreeCount()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	return (int)m_mapMessageFree.size();
}

CProConnectHandle* CProConnectHandlerPool::Create()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	//���free�����Ѿ�û���ˣ�����ӵ�free���С�
	if(m_mapMessageFree.size() <= 0)
	{
		CProConnectHandle* pPacket = new CProConnectHandle();

		if(pPacket != NULL)
		{
			//��ӵ�Free map����
			mapHandle::iterator f = m_mapMessageFree.find(pPacket);
			if(f == m_mapMessageFree.end())
			{
				pPacket->Init(m_u4CurrMaxCount);
				m_mapMessageFree.insert(mapHandle::value_type(pPacket, pPacket));
				m_u4CurrMaxCount++;
			}
		}
		else
		{
			return NULL;
		}
	}

	//��free�����ó�һ��,���뵽used����
	mapHandle::iterator itorFreeB = m_mapMessageFree.begin();
	CProConnectHandle* pPacket = (CProConnectHandle* )itorFreeB->second;
	m_mapMessageFree.erase(itorFreeB);
	//��ӵ�used map����
	mapHandle::iterator f = m_mapMessageUsed.find(pPacket);
	if(f == m_mapMessageUsed.end())
	{
		m_mapMessageUsed.insert(mapHandle::value_type(pPacket, pPacket));
	}

	return (CProConnectHandle* )pPacket;
}

bool CProConnectHandlerPool::Delete(CProConnectHandle* pObject)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);

	if(NULL == pObject)
	{
		return false;
	}

	mapHandle::iterator f = m_mapMessageUsed.find(pObject);
	if(f != m_mapMessageUsed.end())
	{
		m_mapMessageUsed.erase(f);

		//��ӵ�Free map����
		mapHandle::iterator f = m_mapMessageFree.find(pObject);
		if(f == m_mapMessageFree.end())
		{
			m_mapMessageFree.insert(mapHandle::value_type(pObject, pObject));
		}
	}

	return true;
}

//==============================================================
CProConnectManagerGroup::CProConnectManagerGroup()
{
	m_u4CurrMaxCount     = 0;
	m_u2ThreadQueueCount = SENDQUEUECOUNT;
}

CProConnectManagerGroup::~CProConnectManagerGroup()
{
	OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::~CProConnectManagerGroup].\n"));
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e;b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		SAFE_DELETE(pConnectManager);
	}

	m_mapConnectManager.clear();
}

void CProConnectManagerGroup::Init(uint16 u2SendQueueCount)
{
	for(int i = 0; i < u2SendQueueCount; i++)
	{
		CProConnectManager* pConnectManager = new CProConnectManager();
		if(NULL != pConnectManager)	
		{
			//����map
			m_mapConnectManager.insert(mapConnectManager::value_type(i, pConnectManager));
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::Init]Creat %d SendQueue OK.\n", i));
		}
	}

	m_u2ThreadQueueCount = (uint16)m_mapConnectManager.size();
}

uint32 CProConnectManagerGroup::GetGroupIndex()
{
	//�������ӻ�����У��������������㷨��
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
	m_u4CurrMaxCount++;
	return m_u4CurrMaxCount;
}

bool CProConnectManagerGroup::AddConnect(CProConnectHandle* pConnectHandler)
{
	uint32 u4ConnectID = GetGroupIndex();

	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::AddConnect]No find send Queue object.\n"));
		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::Init]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));

	return pConnectManager->AddConnect(u4ConnectID, pConnectHandler);
}

bool CProConnectManagerGroup::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState)
{
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));

	return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
}

bool CProConnectManagerGroup::PostMessage( uint32 u4ConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
		return false;		
	}

	//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));
	IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
	if(NULL != pBuffPacket)
	{
		pBuffPacket->WriteStream(pData, nDataLen);
		return pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));
		return false;
	}

}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
	{
		//�ж����е���һ���߳�������ȥ
		uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

		mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
		if(f == m_mapConnectManager.end())
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
			return false;
		}

		CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
	}

	return true;
}

bool CProConnectManagerGroup::PostMessage( vector<uint32> vecConnectID, const char* pData, uint32 nDataLen, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	uint32 u4ConnectID = 0;
	for(uint32 i = 0; i < (uint32)vecConnectID.size(); i++)
	{
		//�ж����е���һ���߳�������ȥ
		uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

		mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
		if(f == m_mapConnectManager.end())
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]Out of range Queue ID.\n"));
			return false;
		}

		CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		//OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]u4ConnectID=%d, u2ThreadIndex=%d.\n", u4ConnectID, u2ThreadIndex));
		IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
		if(NULL != pBuffPacket)
		{
			pBuffPacket->WriteStream(pData, nDataLen);
			pConnectManager->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
		} 
		else
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]pBuffPacket is NULL.\n"));
			return false;
		}
	}

	return true;
}

bool CProConnectManagerGroup::CloseConnect(uint32 u4ConnectID)
{
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::CloseConnect]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::CloseConnect]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->CloseConnect(u4ConnectID);
}

_ClientIPInfo CProConnectManagerGroup::GetClientIPInfo(uint32 u4ConnectID)
{
	_ClientIPInfo objClientIPInfo;
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return objClientIPInfo;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return objClientIPInfo;		
	}	

	return pConnectManager->GetClientIPInfo(u4ConnectID);	
}

void CProConnectManagerGroup::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	VecClientConnectInfo.clear();

	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->GetConnectInfo(VecClientConnectInfo);
		}
	}
}

int CProConnectManagerGroup::GetCount()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			u4Count += pConnectManager->GetCount();
		}
	}	

	return u4Count;
}

void CProConnectManagerGroup::CloseAll()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();

	for(b; b != e; b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->CloseAll();
		}
	}	
}

bool CProConnectManagerGroup::StartTimer()
{
	uint32 u4Count = 0;
	mapConnectManager::iterator b = m_mapConnectManager.begin();
	mapConnectManager::iterator e = m_mapConnectManager.end();	

	for(b; b != e;b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL != pConnectManager)
		{
			pConnectManager->StartTimer();
		}
	}

	return true;	
}

bool CProConnectManagerGroup::Close(uint32 u4ConnectID)
{
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return false;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return false;		
	}	

	return pConnectManager->Close(u4ConnectID);
}

const char* CProConnectManagerGroup::GetError()
{
	return (char* )"";
}

void CProConnectManagerGroup::SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost)
{
	//�ж����е���һ���߳�������ȥ
	uint16 u2ThreadIndex = u4ConnectID % m_u2ThreadQueueCount;

	mapConnectManager::iterator f = m_mapConnectManager.find(u2ThreadIndex);
	if(f == m_mapConnectManager.end())
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]Out of range Queue ID.\n"));
		return;
	}

	CProConnectManager* pConnectManager = (CProConnectManager* )f->second;
	if(NULL == pConnectManager)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::GetClientIPInfo]No find send Queue object.\n"));
		return;		
	}		

	pConnectManager->SetRecvQueueTimeCost(u4ConnectID, u4TimeCost);
}

bool CProConnectManagerGroup::PostMessageAll( IBuffPacket* pBuffPacket, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	//ȫ��Ⱥ��
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, blSendState);
	}

	//�����˾�ɾ��
	App_BuffPacketManager::instance()->Delete(pBuffPacket);

	return true;
}

bool CProConnectManagerGroup::PostMessageAll( const char* pData, uint32 nDataLen, uint8 u1SendType /*= SENDMESSAGE_NOMAL*/, uint16 u2CommandID /*= 0*/, bool blSendState /*= true*/ )
{
	IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();
	if(NULL == pBuffPacket)
	{
		OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessageAll]pBuffPacket is NULL.\n"));
		//SAFE_DELETE(pData);
		return false;
	}
	else
	{
		pBuffPacket->WriteStream(pData, nDataLen);
	}

	//ȫ��Ⱥ��
	for(mapConnectManager::iterator b = m_mapConnectManager.begin(); b != m_mapConnectManager.end(); b++)
	{
		CProConnectManager* pConnectManager = (CProConnectManager* )b->second;
		if(NULL == pConnectManager)
		{
			OUR_DEBUG((LM_INFO, "[CProConnectManagerGroup::PostMessage]No find send Queue object.\n"));
			return false;		
		}

		pConnectManager->PostMessageAll(pBuffPacket, u1SendType, u2CommandID, blSendState);
	}

	//�����˾�ɾ��
	App_BuffPacketManager::instance()->Delete(pBuffPacket);

	return true;
}


