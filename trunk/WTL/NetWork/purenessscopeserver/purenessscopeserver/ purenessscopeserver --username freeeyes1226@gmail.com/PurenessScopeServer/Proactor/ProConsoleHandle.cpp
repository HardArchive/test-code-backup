#include "ProConsoleHandle.h"

CProConsoleHandle::CProConsoleHandle(void)
{
	m_szError[0]       = '\0';
	m_u4ConnectID      = 0;
	m_u4AllRecvCount   = 0;
	m_u4AllSendCount   = 0;
	m_u4AllRecvSize    = 0;
	m_u4AllSendSize    = 0;
	m_nIOCount         = 0;
	m_u4HandlerID      = 0;
	m_u2MaxConnectTime = 0;
	m_u4SendThresHold  = MAX_MSG_SNEDTHRESHOLD;
	m_u2SendQueueMax   = MAX_MSG_SENDPACKET;
	m_u1ConnectState   = CONNECT_INIT;
	m_u1SendBuffState  = CONNECT_SENDNON;
	m_pPacketParse     = NULL;
}

CProConsoleHandle::~CProConsoleHandle(void)
{
}

const char* CProConsoleHandle::GetError()
{
	return m_szError;
}

bool CProConsoleHandle::Close(int nIOCount)
{
	m_ThreadWriteLock.acquire();
	if(nIOCount > m_nIOCount)
	{
		m_nIOCount = 0;
	}

	if(m_nIOCount > 0)
	{
		m_nIOCount -= nIOCount;
	}
	m_ThreadWriteLock.release();

	//OUR_DEBUG((LM_DEBUG, "[CProConsoleHandle::Close]ConnectID=%d, m_nIOCount = %d.\n", GetConnectID(), m_nIOCount));

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

		OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::Close] Close(%d) delete OK.\n", GetConnectID()));

		delete this;
		return true;
	}

	return false;
}

bool CProConsoleHandle::ServerClose()
{
	OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::ServerClose] Close(%d) delete OK.\n", GetConnectID()));

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

void CProConsoleHandle::SetConnectID(uint32 u4ConnectID)
{
	m_u4ConnectID = u4ConnectID;
}

uint32 CProConsoleHandle::GetConnectID()
{
	return m_u4ConnectID;
}

void CProConsoleHandle::addresses (const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address)
{
	m_addrRemote = remote_address;
}

void CProConsoleHandle::open(ACE_HANDLE h, ACE_Message_Block&)
{
	ACE_Time_Value tvOpenBegin(ACE_OS::gettimeofday());

  //��ʼ��keyֵ�б�
  m_ConsoleMessage.SetConsoleKey(App_MainConfig::instance()->GetConsoleKey());

	OUR_DEBUG((LM_INFO, "[CProConsoleHandle::open] [0x%08x]Connection from [%s:%d]\n", this, m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

	m_atvConnect      = ACE_OS::gettimeofday();
	m_atvInput        = ACE_OS::gettimeofday();
	m_atvOutput       = ACE_OS::gettimeofday();
	m_atvSendAlive    = ACE_OS::gettimeofday();

	m_u4AllRecvCount    = 0;
	m_u4AllSendCount    = 0;
	m_u4AllRecvSize     = 0;
	m_u4AllSendSize     = 0;
	m_blCanWrite        = true;
	m_blTimeClose       = false;
	m_u4RecvPacketCount = 0;
	m_nIOCount          = 1;

	ACE_Time_Value tvOpenEnd(ACE_OS::gettimeofday());
	ACE_Time_Value tvOpen(tvOpenEnd - tvOpenBegin);

	//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Connection from [%s:%d] DisposeTime = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), tvOpen.msec());

	this->handle(h);

	if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1 || 
		this->m_Writer.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE)) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] m_reader or m_reader == 0.\n"));	
		Close();
		return;
	}

	//ACE_Sig_Action writeAction((ACE_SignalHandler)SIG_IGN);
	//writeAction.register_action(SIGPIPE, 0);

	//int nTecvBuffSize = MAX_MSG_SOCKETBUFF;
	//ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));
	//ACE_OS::setsockopt(h, SOL_SOCKET, SO_SNDBUF, (char* )&nTecvBuffSize, sizeof(nTecvBuffSize));

	m_u1ConnectState = CONNECT_OPEN;

	OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] Open(%d).\n", GetConnectID()));	

	m_pPacketParse = App_PacketParsePool::instance()->Create();
	if(NULL == m_pPacketParse)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::open] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
		Close();
		return;
	}

	RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());

	return;
}

void CProConsoleHandle::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
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
			//OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::handle_read_stream] Message_block release.\n"));
			App_MessageBlockManager::instance()->Close(&mb);
		}
		App_PacketParsePool::instance()->Delete(m_pPacketParse);

		OUR_DEBUG((LM_DEBUG,"[%tCConnectHandler::handle_read_stream]Connectid=[%d] error(%d)...\n", GetConnectID(), errno));
		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
		//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

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
			//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
			//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ

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
		if(u4PacketBodyLen >= MAX_MSG_PACKETLENGTH || u4PacketBodyLen <= 0)
		{
			OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than MAX_MSG_PACKETLENGTH.", u4PacketBodyLen));

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

		CheckMessage();
		//App_PacketParsePool::instance()->Delete(m_pPacketParse);

		m_pPacketParse = App_PacketParsePool::instance()->Create();
		if(NULL == m_pPacketParse)
		{
			OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::handle_read_stream] Open(%d) m_pPacketParse new error.\n", GetConnectID()));
			//��Ϊ��Ҫ�ر����ӣ�����Ҫ��ر�һ��IO����ӦOpen���õ�1�ĳ�ʼֵ
			Close(2);
			return;
		}
		Close();

		//������һ�����ݰ�
		RecvClinetPacket(m_pPacketParse->GetPacketHeadLen());
	}

	return;
}

void CProConsoleHandle::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	if(!result.success() || result.bytes_transferred()==0)
	{
		//���ӶϿ�
		OUR_DEBUG ((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] begin(%d)...\n",GetConnectID(), errno));

		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);

		OUR_DEBUG((LM_DEBUG,"[CConnectHandler::handle_write_stream] Connectid=[%d] finish ok...\n", GetConnectID()));
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		Close();
		return;
	}
	else
	{
		//�̶�������
		m_blCanWrite = true;
		m_atvOutput = ACE_OS::gettimeofday();
		App_MessageBlockManager::instance()->Close(&result.message_block());
		m_u4AllSendSize += (uint32)result.bytes_to_write();
		Close();
		return;
	}
}

uint8 CProConsoleHandle::GetConnectState()
{
	return m_u1ConnectState;
}

uint8 CProConsoleHandle::GetSendBuffState()
{
	return m_u1SendBuffState;
}

bool CProConsoleHandle::GetIsClosing()
{
	return m_blTimeClose;
}

bool CProConsoleHandle::SendMessage(IBuffPacket* pBuffPacket)
{
	CPacketParse PacketParse;

	if(NULL == pBuffPacket)
	{
		//OUR_DEBUG((LM_DEBUG,"[CProConsoleHandle::SendMessage] Connectid=[%d] pBuffPacket is NULL.\n", GetConnectID()));
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

bool CProConsoleHandle::PutSendPacket(ACE_Message_Block* pMbData)
{
	int nSendSize = m_u4AllSendSize;

	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();
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
			m_u4AllSendCount += 1;
			m_atvOutput      = ACE_OS::gettimeofday();
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

bool CProConsoleHandle::RecvClinetPacket(uint32 u4PackeLen)
{
	m_ThreadWriteLock.acquire();
	m_nIOCount++;
	m_ThreadWriteLock.release();
	//OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket]Connectid=%d, m_nIOCount=%d.\n", GetConnectID(), m_nIOCount));

	ACE_Message_Block* pmb = NULL;
	pmb = App_MessageBlockManager::instance()->Create(u4PackeLen);
	if(pmb == NULL)
	{
		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
		OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] pmb new is NULL.\n"));
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
		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_CONNECT, "Close Connection from [%s:%d] RecvSize = %d, RecvCount = %d, SendSize = %d, SendCount = %d.",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number(), m_u4AllRecvSize, m_u4AllRecvCount, m_u4AllSendSize, m_u4AllSendCount);
		OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::RecvClinetPacket] m_reader.read is error(%d)(%d).\n", GetConnectID(), errno));
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

bool CProConsoleHandle::CheckMessage()
{
	if(m_pPacketParse->GetMessageHead() != NULL && m_pPacketParse->GetMessageBody() != NULL)
	{
		m_u4AllRecvSize += (uint32)m_pPacketParse->GetMessageHead()->length() + (uint32)m_pPacketParse->GetMessageBody()->length();
		m_u4AllRecvCount++;

		//���ͻظ���Ϣ
		IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create();

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
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CProConsoleHandle::CheckMessage] ConnectID = %d, m_pPacketParse is NULL.\n", GetConnectID()));
	}

	return true;
}
