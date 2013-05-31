// STEpoll.cpp: implementation of the STEpoll class.
//
//////////////////////////////////////////////////////////////////////

#include "../../../include/frame/netserver/STEpoll.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace mdk
{

STEpoll::STEpoll()
{
#ifndef WIN32
	m_bStop = true;
	m_events = NULL;
#endif
}

STEpoll::~STEpoll()
{
#ifndef WIN32
	//���õ���Stop��epoll_wait�����أ�����Ӧ�ñ�����
	if ( NULL != m_events )
	{
		delete[]m_events;
		m_events = NULL;
	}
#endif
}

void STEpoll::SheildSigPipe()
{
#ifndef WIN32
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &sa, 0 );
#endif
}

bool STEpoll::Start( int nMaxMonitor )
{
#ifndef WIN32
	m_nMaxMonitor = nMaxMonitor;
	if ( NULL != m_events )
	{
		delete[]m_events;
		m_events = NULL;
	}
	m_events = new epoll_event[m_nMaxMonitor];	//epoll�¼�
	if ( NULL == m_events ) 
	{
		m_initError = "no memory for epoll_event";
		return false;
	}
	SheildSigPipe();
	m_epollExit = socket( PF_INET, SOCK_STREAM, 0 );
	/* ���� epoll ���*/
    m_hEpoll = epoll_create(m_nMaxMonitor);
	if ( -1 == m_hEpoll ) 
	{
		m_initError = "create epoll faild";
		return false;
	}
	m_bStop = false;
#endif

	return true;
}

bool STEpoll::Stop()
{
#ifndef WIN32
	if ( m_bStop ) return true;
	m_bStop = true;
	AddIO(m_epollExit, true, false);
	//�����������ͷ�m_events��epoll_wait���ܻ���ʹ��
#endif
	return true;
}

int STEpoll::WaitEvent( int timeout )
{
#ifndef WIN32
	return epoll_wait(m_hEpoll, m_events, m_nMaxMonitor, timeout );
#endif
	return -1;
}

//�õ���i���¼���socket
int STEpoll::GetSocket( int i )
{
#ifndef WIN32
	if ( m_epollExit == m_events[i].data.fd )
	{
		if ( NULL != m_events )
		{
			delete[]m_events;
			m_events = NULL;
		}
		return INVALID_SOCKET;
	}
	return m_events[i].data.fd;
#endif
	return INVALID_SOCKET;
}

//��i��socket�н��տ�����
bool STEpoll::IsAcceptAble( int i )
{
#ifndef WIN32
	if ( m_listenSockets.end() == m_listenSockets.find(m_events[i].data.fd) ) return false;
	return true;
#endif
	return false;
}

//��i��socket�ɶ�
bool STEpoll::IsReadAble( int i )
{
#ifndef WIN32
	return m_events[i].events&EPOLLIN;
#endif
	return false;
}

//��i��socket��д
bool STEpoll::IsWriteAble( int i )
{
#ifndef WIN32
	return m_events[i].events&EPOLLOUT;
#endif
	return false;
}

//����һ��Accept�������������Ӳ�����WaitEvent�᷵��
bool STEpoll::AddAccept( SOCKET sock )
{
#ifndef WIN32
	if ( m_listenSockets.end() != m_listenSockets.find(sock) ) return false;
	m_listenSockets.insert(map<SOCKET,SOCKET>::value_type(sock,sock));
	epoll_event ev;
    ev.events = EPOLLIN|EPOLLET;
    ev.data.fd = sock;
	if ( 0 > epoll_ctl(m_hEpoll, EPOLL_CTL_MOD, sock, &ev) ) return false;
#endif
	return true;
}

//����һ��IO��������IOʱWaitEvent�᷵��
bool STEpoll::AddIO( SOCKET sock, bool read, bool write )
{
#ifndef WIN32
	epoll_event ev;
	if ( read && write ) ev.events = EPOLLOUT|EPOLLIN|EPOLLONESHOT;
	else if ( read ) ev.events = EPOLLIN|EPOLLONESHOT;
	else ev.events = EPOLLOUT|EPOLLONESHOT;
    ev.data.fd = sock;
	if ( 0 > epoll_ctl(m_hEpoll, EPOLL_CTL_MOD, sock, &ev) ) return false;
#endif	
	return true;
}

bool STEpoll::DelMonitor( SOCKET sock )
{
#ifndef WIN32
	if ( epoll_ctl(m_hEpoll, EPOLL_CTL_DEL, sock, NULL) < 0 ) return false;
	map<SOCKET,SOCKET>::iterator it = m_listenSockets.find(sock);
	if ( it != m_listenSockets.end() ) m_listenSockets.erase(it);
#endif	
	return true;
}

bool STEpoll::AddMonitor( SOCKET sock )
{
#ifndef WIN32
	epoll_event ev;
    ev.events = EPOLLONESHOT;
    ev.data.fd = sock;
    if ( 0 > epoll_ctl(m_hEpoll, EPOLL_CTL_ADD, sock, &ev) ) return false;
#endif	
	return true;
}

}//namespace mdk
