#include "../../../include/frame/netserver/STNetHost.h"
#include "../../../include/frame/netserver/STNetConnect.h"
#include "../../../include/mdk/Socket.h"
#include "../../../include/mdk/atom.h"
using namespace std;

namespace mdk
{

STNetHost::STNetHost()
:m_pConnect(NULL)
{
}

STNetHost::STNetHost(const STNetHost& obj)
{
	AtomAdd(&obj.m_pConnect->m_useCount, 1);
	m_pConnect = obj.m_pConnect;
}

STNetHost& STNetHost::operator=(const STNetHost& obj)
{
	if ( m_pConnect == obj.m_pConnect ) return *this;
	if ( NULL != m_pConnect ) m_pConnect->Release();
	AtomAdd(&obj.m_pConnect->m_useCount, 1);
	m_pConnect = obj.m_pConnect;
	return *this;
}

STNetHost::~STNetHost()
{
	if ( NULL != m_pConnect ) m_pConnect->Release();
}
 
bool STNetHost::Send(const unsigned char* pMsg, unsigned short uLength)
{
	return m_pConnect->SendData(pMsg, uLength);
	return true;
}

bool STNetHost::Recv( unsigned char* pMsg, unsigned short uLength, bool bClearCache )
{
	return m_pConnect->ReadData( pMsg, uLength, bClearCache );
}

void STNetHost::Close()
{
	m_pConnect->Close();
}

bool STNetHost::IsServer()
{
	return m_pConnect->IsServer();
}

int STNetHost::ID()
{
	if ( NULL == m_pConnect ) return -1;
	return m_pConnect->GetID();
}

//����ĳ����
void STNetHost::InGroup( int groupID )
{
	m_pConnect->InGroup(groupID);
}

//��ĳ����ɾ��
void STNetHost::OutGroup( int groupID )
{
	m_pConnect->OutGroup(groupID);
}

void STNetHost::GetServerAddress( string &ip, int &port )
{
	m_pConnect->GetServerAddress(ip, port);
	return;
}

void STNetHost::GetAddress( string &ip, int &port )
{
	m_pConnect->GetAddress(ip, port);
	return;
}

}  // namespace mdk
