#include "ProConnectClient.h"
#include "ClientProConnectManager.h"

CProConnectClient::CProConnectClient(void)
{
	m_nIOCount          = 1;
	m_nServerID         = 0;
	m_mbRecv            = NULL;
	m_pClientParse      = NULL;
	m_pClientMessage    = NULL;
  m_u4MaxPacketSize   = MAX_MSG_PACKETLENGTH;

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
}

CProConnectClient::~CProConnectClient(void)
{
}

void CProConnectClient::Close()
{
	m_ThreadWritrLock.acquire();
	if(m_nIOCount > 0)
	{
		m_nIOCount--;
	}
	m_ThreadWritrLock.release();

	if(m_nIOCount == 0)
	{
		m_Reader.cancel();
		m_Writer.cancel();

		if(this->handle() != ACE_INVALID_HANDLE)
		{
			ACE_OS::closesocket(this->handle());
			this->handle(ACE_INVALID_HANDLE);
		}

		App_ClientProConnectManager::instance()->CloseByClient(m_nServerID);

		OUR_DEBUG((LM_DEBUG, "[CProConnectClient::Close]delete OK.\n"));
		delete this;
	}
}

void CProConnectClient::ClientClose()
{
	if(this->handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::shutdown(this->handle(), SD_SEND);
		ACE_OS::closesocket(this->handle());
		this->handle(ACE_INVALID_HANDLE);
	}
}

void CProConnectClient::SetServerID(int nServerID)
{
	m_nServerID = nServerID;
}

void CProConnectClient::SetClientMessage(IClientMessage* pClientMessage)
{
	m_pClientMessage = pClientMessage;
}

int CProConnectClient::GetServerID()
{
	return m_nServerID;
}

void CProConnectClient::open(ACE_HANDLE h, ACE_Message_Block&)
{
  //从配置文件获取数据
  m_u4MaxPacketSize  = App_MainConfig::instance()->GetRecvBuffSize();

	m_nIOCount = 1;
	this->handle(h);
	if(this->m_Reader.open(*this, h, 0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1||this->m_Writer.open(*this, h,  0, App_ProactorManager::instance()->GetAce_Proactor(REACTOR_POSTDEFINE)) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader or m_reader == 0.\n"));	
		Close();
		return;
	}

	m_u4SendSize        = 0;
	m_u4SendCount       = 0;
	m_u4RecvSize        = 0;
	m_u4RecvCount       = 0;
	m_u4CostTime        = 0;
	m_atvBegin          = ACE_OS::gettimeofday();

	m_pClientParse = App_ClientParsePool::instance()->Create();
	if(m_pClientParse == NULL)
	{
		Close();
		return;
	}

	App_ClientProConnectManager::instance()->SetHandler(m_nServerID, this);
	m_pClientMessage = App_ClientProConnectManager::instance()->GetClientMessage(m_nServerID);

	RecvData(m_pClientParse->GetPacketHeadLen());
}


void CProConnectClient::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_Message_Block& mb = result.message_block();
	uint32 u4PacketLen = (uint32)result.bytes_transferred();
	
	if(!result.success() || u4PacketLen == 0)
	{
		mb.release();
		if(NULL != m_pClientMessage)
		{
			m_pClientMessage->ConnectError((int)ACE_OS::last_error());
		}
		Close();
		return;
	}
	else if(result.bytes_transferred() < result.bytes_to_read())    //如果是短读，继续读。
	{
		if(m_Reader.read(mb ,result.bytes_to_read() - result.bytes_transferred()) == -1)
		{
			mb.release();
			if(NULL != m_pClientMessage)
			{
				m_pClientMessage->ConnectError((int)ACE_OS::last_error());
			}
			Close();
			return;
		}
	}
	else if(mb.length() == m_pClientParse->GetPacketHeadLen() && m_pClientParse->GetIsHead() == false)
	{
		//判断头的合法性
		m_pClientParse->SetPacketHead(mb.rd_ptr(), (uint32)mb.length());
		uint32 u4PacketBodyLen = m_pClientParse->GetPacketDataLen();

		//如果超过了最大包长度，为非法数据
		if(u4PacketBodyLen >= m_u4MaxPacketSize || u4PacketBodyLen <= 0)
		{
			OUR_DEBUG((LM_ERROR, "[CConnectHandler::handle_read_stream]u4PacketHeadLen(%d) more than MAX_MSG_PACKETLENGTH.", u4PacketBodyLen));

			if(m_pClientParse->GetMessageHead() != NULL)
			{
				m_pClientParse->GetMessageHead()->release();
				m_pClientParse->SetMessageHead(NULL);
			}

			if(m_pClientParse->GetMessageBody() != NULL)
			{
				m_pClientParse->GetMessageBody()->release();
				m_pClientParse->SetMessageBody(NULL);
			}

			if(&mb != m_pClientParse->GetMessageHead() && &mb != m_pClientParse->GetMessageBody())
			{
				mb.release();
			}
			App_ClientParsePool::instance()->Delete(m_pClientParse);
			if(NULL != m_pClientMessage)
			{
				//包错误，包长度非法，错误ID是100
				m_pClientMessage->ConnectError(100);
			}
			Close();
			return;
		}
		else
		{
			m_pClientParse->SetMessageHead(&mb);
			RecvData(u4PacketBodyLen);
		}
	}
	else
	{
		//接受完整数据完成，开始分析完整数据包
		m_pClientParse->SetPacketData(mb.rd_ptr(), (uint32)mb.length());
		m_pClientParse->SetMessageBody(&mb);

		//处理接收数据包
		DoMessage();

		//App_ClientParsePool::instance()->Delete(m_pClientParse);
		m_pClientParse = App_ClientParsePool::instance()->Create();
		if(NULL == m_pClientParse)
		{
			Close();
			if(NULL != m_pClientMessage)
			{
				//包错误，包长度非法，错误ID是101
				m_pClientMessage->ConnectError(101);
			}
			return;
		}

		//接受下一个数据包
		RecvData(m_pClientParse->GetPacketHeadLen());
	}
}

void CProConnectClient::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	ACE_Message_Block& mblk = result.message_block();
	if(!result.success() || result.bytes_transferred() == 0)
	{
		//处理数据发送出错
		OUR_DEBUG((LM_DEBUG, "[CProConnectClient::handle_write_stream]Write error(%d).\n", ACE_OS::last_error()));
		mblk.release();
		if(NULL != m_pClientMessage)
		{
			m_pClientMessage->ConnectError((int)ACE_OS::last_error());
		}
		Close();
	}
	else
	{
		mblk.release();
	}
}

void CProConnectClient::addresses(const ACE_INET_Addr &remote_address, const ACE_INET_Addr &local_address)
{
	m_AddrRemote = remote_address;
}

bool CProConnectClient::RecvData(uint32 u4PacketLen)
{
	ACE_NEW_NORETURN(m_mbRecv, ACE_Message_Block(u4PacketLen));
	if(this->m_Reader.read(*m_mbRecv, m_mbRecv->space()) == -1)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectHandle::open] m_reader is error(%d).\n", (int)ACE_OS::last_error()));	
		m_mbRecv->release();
		if(NULL != m_pClientMessage)
		{
			m_pClientMessage->ConnectError((int)ACE_OS::last_error());
		}
		Close();
		return false;
	}

	return true;
}

bool CProConnectClient::DoMessage()
{
	//接收到完整的数据包，开始解析
	if(NULL == m_pClientParse)
	{
		OUR_DEBUG((LM_DEBUG,"[CProConnectClient::DoMessage] m_pClientParse is NULL.\n"));	
		return false;
	}

	m_ThreadWritrLock.acquire();
	m_nIOCount++;
	m_ThreadWritrLock.release();

	m_u4RecvSize = m_pClientParse->GetPacketHeadLen() + m_pClientParse->GetPacketDataLen();
	m_u4RecvCount++;

	ACE_Time_Value tvBegin = ACE_OS::gettimeofday();
	//回调处理方法，处理方法由外围继承
	if(NULL != m_pClientMessage)
	{
		m_pClientMessage->RecvData(m_pClientParse);
	}
	ACE_Time_Value tvEnd = ACE_OS::gettimeofday();
	m_u4CostTime += (uint32)(tvEnd.msec() - tvBegin.msec());

	//用完了，就删除
	m_pClientParse->Close();
	App_ClientParsePool::instance()->Delete(m_pClientParse);

	m_ThreadWritrLock.acquire();
	m_nIOCount--;
	m_ThreadWritrLock.release();

	return true;
}

bool CProConnectClient::SendData(ACE_Message_Block* pmblk)
{
	OUR_DEBUG((LM_DEBUG, "[CProConnectClient::SendData]Begin.\n"));
	//发送信息
	if(NULL != pmblk)
	{
		int nLen = (int)pmblk->length();
		if (m_Writer.write(*pmblk, pmblk->length()) == -1)
		{
			OUR_DEBUG((LM_DEBUG,"[CProConnectClient::SendData] Send Error(%d).\n", ACE_OS::last_error()));	
			pmblk->release();
			if(NULL != m_pClientMessage)
			{
				m_pClientMessage->ConnectError((int)ACE_OS::last_error());
			}
			Close();
			return false;
		}

		m_u4SendSize += (uint32)nLen;
		m_u4SendCount++;		
	}

	return true;
}

_ClientConnectInfo CProConnectClient::GetClientConnectInfo()
{
	_ClientConnectInfo ClientConnectInfo;
	ClientConnectInfo.m_blValid       = true;
	//ClientConnectInfo.m_addrRemote    = m_AddrRemote;
	ClientConnectInfo.m_u4AliveTime   = (uint32)(ACE_OS::gettimeofday().sec() - m_atvBegin.sec());
	ClientConnectInfo.m_u4AllRecvSize = m_u4RecvSize;
	ClientConnectInfo.m_u4RecvCount   = m_u4RecvCount;
	ClientConnectInfo.m_u4AllSendSize = m_u4SendSize;
	ClientConnectInfo.m_u4SendCount   = m_u4SendCount;
	ClientConnectInfo.m_u4ConnectID   = m_nServerID;
	ClientConnectInfo.m_u4BeginTime   = (uint32)m_atvBegin.sec();
	return ClientConnectInfo;
}

