// TestServer.cpp: implementation of the TestServer class.
//
//////////////////////////////////////////////////////////////////////

#include "TestServer.h"
#include <stdio.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TestServer::TestServer()
{
	SetIOThreadCount(4);//��������IO�߳�����
	SetWorkThreadCount(4);//���ù����߳���
	Listen(8888);
	Listen(6666);
	Listen(9999);
//	Connect("127.0.0.1",8888);//��������δ���ԣ���������ô��
}

TestServer::~TestServer()
{

}

/*
 *	���������߳�
 */
void* TestServer::Main(void* pParam)
{
	while ( IsOk() )
	{
		//ִ��ҵ��
#ifndef WIN32
		usleep( 1000 * 1000 );
#else
		Sleep( 1000 );
#endif
	}
	
	return 0;
}

/**
 * �������¼��ص�����
 * 
 * ������ʵ�־�������ҵ����
 * 
 */
void TestServer::OnConnect(mdk::NetHost& host)
{
	printf( "new client(%d) connect in\n", host.ID() );
}

/**
 * ���ӹر��¼����ص�����
 * 
 * ������ʵ�־���Ͽ�����ҵ����
 * 
 */
void TestServer::OnCloseConnect(mdk::NetHost &host)
{
	printf( "client(%d) close connect\n", host.ID() );
}

void TestServer::OnMsg(mdk::NetHost &host)
{
	//���豨�ĽṹΪ��2byte��ʾ���ݳ���+��������
	unsigned char c[256];
	/*
		��ȡ���ݳ��ȣ����Ȳ���2byteֱ�ӷ��أ��ȴ��´����ݵ���ʱ�ٶ�ȡ
		ֻ��ȡ2byte��false��ʾ��������ȡ�������ݴӻ�����ɾ�����´λ��ǿ��Զ���
	*/
	if ( !host.Recv( c, 2, false ) ) return;
	unsigned short len = 0;
	memcpy( &len, c, 2 );//�õ����ݳ���
	len += 2;//���ĳ��� = ����ͷ����+���ݳ���
	if ( len > 256 ) 
	{
		printf( "close client:invaild fromat msg\n" );
		host.Close();
		return;
	}
	if ( !host.Recv(c, len) ) return;//�����Ķ��������ӽ��ջ�����ɾ�������Բ����ܳ��Ȳ�������ʹ�����Ѿ��Ͽ���Ҳ���Զ�������
	host.Send( c, len );//�յ���Ϣԭ���ظ�
}
