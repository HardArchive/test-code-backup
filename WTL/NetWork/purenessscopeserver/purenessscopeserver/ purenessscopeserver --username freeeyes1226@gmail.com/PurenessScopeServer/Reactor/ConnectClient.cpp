#include "ConnectClient.h"
#include "ClientReConnectManager.h"

CConnectClient::CConnectClient(void)
{
	m_pClientParse      = NULL;
	m_pCurrMessage      = NULL;
	m_nIOCount          = 1;
	m_nServerID         = 0;

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
  m_u4MaxPacketSize   = MAX_MSG_PACKETLENGTH;
}

CConnectClient::~CConnectClient(void)
{
}

bool CConnectClient::Close()
{
	m_ThreadLock.acquire();
	if(m_nIOCount > 0)
	{
		m_nIOCount--;
	}
	m_ThreadLock.release();

	//从反应器注销事件
	if(m_nIOCount == 0)
	{
		//msg_queue()->deactivate();
		shutdown();
		OUR_DEBUG((LM_ERROR, "[CConnectClient::Close]Close(%s:%d) OK.\n", m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

		//删除链接对象
		App_ClientReConnectManager::instance()->CloseByClient(m_nServerID);

		//回归用过的指针
		delete this;
		return true;
	}

	return false;
}

void CConnectClient::ClinetClose()
{
	//msg_queue()->deactivate();
	shutdown();
}

int CConnectClient::open(void* p)
{
  //从配置文件获取数据
  m_u4MaxPacketSize  = App_MainConfig::instance()->GetRecvBuffSize();

	if(p != NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::open]p is not NULL.\n"));
	}
	
	ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);
	m_nIOCount = 1;
	int nRet = Super::open();
	if(nRet != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].\n", nRet));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::open]ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>::open() error [%d].", nRet);
		return -1;
	}

	//设置链接为非阻塞模式
	if (this->peer().enable(ACE_NONBLOCK) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().enable  = ACE_NONBLOCK error.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::open]this->peer().enable  = ACE_NONBLOCK error.");
		return -1;
	}

	//获得远程链接地址和端口
	if(this->peer().get_remote_addr(m_addrRemote) == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectHandler::open]this->peer().get_remote_addr error.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::open]this->peer().get_remote_addr error.");
		return -1;
	}

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
	m_atvBegin          = ACE_OS::gettimeofday();
	m_u4CurrSize        = 0;

	m_pClientParse = App_ClientParsePool::instance()->Create();
	if(m_pClientParse == NULL)
	{
		return -1;
	}

	//申请当前的MessageBlock
	m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pClientParse->GetPacketHeadLen());
	if(m_pCurrMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::RecvClinetPacket] pmb new is NULL.\n"));
		return -1;
	}

	App_ClientReConnectManager::instance()->SetHandler(m_nServerID, this);
	m_pClientMessage = App_ClientReConnectManager::instance()->GetClientMessage(m_nServerID);
	OUR_DEBUG((LM_INFO, "[CConnectClient::open] Connection from [%s:%d]\n",m_addrRemote.get_host_addr(), m_addrRemote.get_port_number()));

	return 0;
}

int CConnectClient::handle_input(ACE_HANDLE fd)
{
	ACE_Time_Value nowait(MAX_MSG_PACKETTIMEOUT);

	if(fd == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input]fd == ACE_INVALID_HANDLE.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_input]fd == ACE_INVALID_HANDLE.");
		return -1;
	}

	//判断数据包结构是否为NULL
	if(m_pClientParse == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input]m_pPacketParse == NULL.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectHandler::handle_input]m_pPacketParse == NULL.");
		return -1;
	}

	//判断缓冲是否为NULL
	if(m_pCurrMessage == NULL)
	{
		m_u4CurrSize = 0;
		OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input]m_pCurrMessage == NULL.\n"));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::handle_input]m_pCurrMessage == NULL.");

		if(m_pClientParse->GetMessageHead() != NULL)
		{
			m_pClientParse->GetMessageHead()->release();
		}

		if(m_pClientParse->GetMessageBody() != NULL)
		{
			m_pClientParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pClientParse->GetMessageBody() != m_pCurrMessage && m_pClientParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_ClientParsePool::instance()->Delete(m_pClientParse);
		return -1;
	}

	int nCurrCount = (uint32)m_pCurrMessage->size() - m_u4CurrSize;
	if(nCurrCount < 0)
	{
		//如果剩余字节为负，说明程序出了问题
		OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input][%d] nCurrCount < 0 m_u4CurrSize = %d.\n", GetServerID(), m_u4CurrSize));
		m_u4CurrSize = 0;

		if(m_pClientParse->GetMessageHead() != NULL)
		{
			m_pClientParse->GetMessageHead()->release();
		}

		if(m_pClientParse->GetMessageBody() != NULL)
		{
			m_pClientParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pClientParse->GetMessageBody() != m_pCurrMessage && m_pClientParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_ClientParsePool::instance()->Delete(m_pClientParse);
		return -1;
	}

	int nDataLen = this->peer().recv(m_pCurrMessage->wr_ptr(), nCurrCount, MSG_NOSIGNAL, &nowait);
	if(nDataLen <= 0)
	{
		m_u4CurrSize = 0;
		uint32 u4Error = (uint32)errno;
		OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_input] ConnectID = %d, recv data is error nDataLen = [%d] errno = [%d].\n", GetServerID(), nDataLen, u4Error));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::handle_input] ConnectID = %d, recv data is error[%d].\n", GetServerID(), nDataLen);

		if(m_pClientParse->GetMessageHead() != NULL)
		{
			m_pClientParse->GetMessageHead()->release();
		}

		if(m_pClientParse->GetMessageBody() != NULL)
		{
			m_pClientParse->GetMessageBody()->release();
		}

		if(m_pCurrMessage != NULL && m_pClientParse->GetMessageBody() != m_pCurrMessage && m_pClientParse->GetMessageBody() != m_pCurrMessage)
		{
			m_pCurrMessage->release();
			m_pCurrMessage = NULL;
		}
		App_ClientParsePool::instance()->Delete(m_pClientParse);
		return -1;
	}

	m_u4CurrSize += nDataLen;

	m_pCurrMessage->wr_ptr(nDataLen);

	//如果没有读完，短读
	if((uint32)m_pCurrMessage->size() > m_u4CurrSize)
	{
		return 0;
	}
	else if(m_pCurrMessage->length() == m_pClientParse->GetPacketHeadLen() && m_pClientParse->GetIsHead() == false)
	{
		m_pClientParse->SetPacketHead(m_pCurrMessage->rd_ptr(), (uint32)m_pCurrMessage->length());
		uint32 u4PacketBodyLen = m_pClientParse->GetPacketDataLen();
		m_u4CurrSize = 0;

		//如果超过了最大包长度，为非法数据
		if(u4PacketBodyLen >= m_u4MaxPacketSize || u4PacketBodyLen <= 0)
		{
			m_u4CurrSize = 0;
			OUR_DEBUG((LM_ERROR, "[CConnectClient::handle_read_stream]u4PacketHeadLen(%d) more than MAX_MSG_PACKETLENGTH.", u4PacketBodyLen));
			if(m_pClientParse->GetMessageHead() != NULL)
			{
				m_pClientParse->GetMessageHead()->release();
			}

			if(m_pClientParse->GetMessageBody() != NULL)
			{
				m_pClientParse->GetMessageBody()->release();
			}

			if(m_pCurrMessage != NULL && m_pClientParse->GetMessageBody() != m_pCurrMessage && m_pClientParse->GetMessageBody() != m_pCurrMessage)
			{
				m_pCurrMessage->release();
				m_pCurrMessage = NULL;
			}
			App_ClientParsePool::instance()->Delete(m_pClientParse);
			return 0;
		}
		else
		{
			m_pClientParse->SetMessageHead(m_pCurrMessage);

			//申请头的大小对应的mb
			m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pClientParse->GetPacketDataLen());
			if(m_pCurrMessage == NULL)
			{
				m_u4CurrSize = 0;
				OUR_DEBUG((LM_ERROR, "[CConnectClient::RecvClinetPacket] pmb new is NULL.\n"));

				if(m_pClientParse->GetMessageHead() != NULL)
				{
					m_pClientParse->GetMessageHead()->release();
				}

				if(m_pClientParse->GetMessageBody() != NULL)
				{
					m_pClientParse->GetMessageBody()->release();
				}

				if(m_pCurrMessage != NULL && m_pClientParse->GetMessageBody() != m_pCurrMessage && m_pClientParse->GetMessageBody() != m_pCurrMessage)
				{
					m_pCurrMessage->release();
					m_pCurrMessage = NULL;
				}
				App_ClientParsePool::instance()->Delete(m_pClientParse);
				return -1;
			}
			return 0;
		}
	}
	else
	{
		//接受完整数据完成，开始分析完整数据包
		m_pClientParse->SetPacketData(m_pCurrMessage->rd_ptr(), (uint32)m_pCurrMessage->length());
		m_pClientParse->SetMessageBody(m_pCurrMessage);
		m_u4CurrSize = 0;

		//将接收的完整数据包扔给逻辑去处理
		CheckMessage();

		m_u4CurrSize = 0;

		//申请头的大小对应的mb
		m_pCurrMessage = App_MessageBlockManager::instance()->Create(m_pClientParse->GetPacketHeadLen());
		if(m_pCurrMessage == NULL)
		{
			OUR_DEBUG((LM_ERROR, "[CConnectClient::RecvClinetPacket] pmb new is NULL.\n"));
			return -1;
		}

		//申请新的包
		m_pClientParse = App_ClientParsePool::instance()->Create();
		if(NULL == m_pClientParse)
		{
			OUR_DEBUG((LM_DEBUG,"[CConnectClient::open] Open(%d) m_pPacketParse new error.\n", GetServerID()));
			return -1;
		}
	}

	return 0;
}

int CConnectClient::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	if(h == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_DEBUG,"[CConnectClient::handle_close] h is NULL mask=%d.\n", GetServerID(), (int)mask));
	}
	
	Close();
	return 0;
}

void CConnectClient::SetClientMessage(IClientMessage* pClientMessage)
{
	m_pClientMessage = pClientMessage;
}

void CConnectClient::SetServerID(int nServerID)
{
	m_nServerID = nServerID;
}

int CConnectClient::GetServerID()
{
	return m_nServerID;
}

bool CConnectClient::CheckMessage()
{
	if(NULL == m_pClientMessage)
	{
		OUR_DEBUG((LM_DEBUG,"[CConnectClient::CheckMessage](%d)m_pClientMessage is NULL.\n", GetServerID()));
		return false;
	}

	m_ThreadLock.acquire();
	m_nIOCount++;
	m_ThreadLock.release();

	m_u4RecvSize = m_pClientParse->GetPacketHeadLen() + m_pClientParse->GetPacketDataLen();
	m_u4RecvCount++;

	ACE_Time_Value tvBegin = ACE_OS::gettimeofday();
	//处理接收到的数据
	if(NULL != m_pClientMessage)
	{
		m_pClientMessage->RecvData(m_pClientParse);
	}
	ACE_Time_Value tvEnd = ACE_OS::gettimeofday();
	m_u4CostTime += (uint32)(tvEnd.msec() - tvBegin.msec());

	m_pClientParse->Close();
	App_ClientParsePool::instance()->Delete(m_pClientParse);

	m_ThreadLock.acquire();
	m_nIOCount--;
	m_ThreadLock.release();

	return true;
}

bool CConnectClient::SendData(ACE_Message_Block* pmblk)
{
	ACE_Time_Value     nowait(MAX_MSG_PACKETTIMEOUT);

	if(NULL == pmblk)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID()));
		return false;
	}

	if(get_handle() == ACE_INVALID_HANDLE)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID()));
		sprintf_safe(m_szError, MAX_BUFF_500, "[CConnectClient::SendData] ConnectID = %d, get_handle() == ACE_INVALID_HANDLE.\n", GetServerID());
		pmblk->release();
		return false;
	}

	char* pData = pmblk->rd_ptr();
	if(NULL == pData)
	{
		OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, pData is NULL.\n", GetServerID()));
		pmblk->release();
		return false;
	}

	int nSendLen = (int)pmblk->length();   //发送数据的总长度
	int nIsSendSize = 0;

	//循环发送，直到数据发送完成。
	while(true)
	{
		if(nSendLen <= 0)
		{
			OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, nCurrSendSize error is %d.\n", GetServerID(), nSendLen));
			pmblk->release();
			return false;
		}

		int nDataLen = this->peer().send(pmblk->rd_ptr(), nSendLen, &nowait);
		int nErr = ACE_OS::last_error();
		if(nDataLen <= 0)
		{
			if(nErr == EWOULDBLOCK)
			{
				//如果发送堵塞，则等10毫秒后再发送。
				ACE_Time_Value tvSleep(0, 10 * MAX_BUFF_1000);
				ACE_OS::sleep(tvSleep);
				continue;
			}

			OUR_DEBUG((LM_ERROR, "[CConnectClient::SendData] ConnectID = %d, error = %d.\n", GetServerID(), errno));
			pmblk->release();
			Close();
			return false;
		}
		else if(nDataLen >= nSendLen)   //当数据包全部发送完毕，清空。
		{
			//OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_output] ConnectID = %d, send (%d) OK.\n", GetConnectID(), msg_queue()->is_empty()));
			pmblk->release();
			m_u4SendSize += (uint32)nSendLen;
			m_u4SendCount++;	
			return true;
		}
		else
		{
			pmblk->rd_ptr(nDataLen);
			nIsSendSize      += nDataLen;
			continue;
		}
	}

	return true;
}

_ClientConnectInfo CConnectClient::GetClientConnectInfo()
{
	_ClientConnectInfo ClientConnectInfo;
	ClientConnectInfo.m_blValid       = true;
	ClientConnectInfo.m_addrRemote    = m_addrRemote;
	ClientConnectInfo.m_u4AliveTime   = (uint32)(ACE_OS::gettimeofday().sec() - m_atvBegin.sec());
	ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
	ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
	ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
	ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
	ClientConnectInfo.m_u4ConnectID   = m_nServerID;
	ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvBegin.sec();
	return ClientConnectInfo;
}

