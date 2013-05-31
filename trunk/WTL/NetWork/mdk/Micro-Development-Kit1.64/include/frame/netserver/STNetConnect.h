// STNetConnect.h: interface for the STNetConnect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MDK_STNETCONNECT_H
#define MDK_STNETCONNECT_H

#include "STNetHost.h"
#include "../../../include/mdk/Lock.h"
#include "../../../include/mdk/IOBuffer.h"
#include "../../../include/mdk/Socket.h"

#include <time.h>
#include <map>
#include <string>

namespace mdk
{

class NetEventMonitor;
class Socket;
class STNetEngine;
class MemoryPool;	

class STNetConnect  
{
	friend class STNetEngine;
	friend class STNetHost;
public:
	STNetConnect(SOCKET sock, bool bIsServer, NetEventMonitor *pNetMonitor, STNetEngine *pEngine, MemoryPool *pMemoryPool);
	virtual ~STNetConnect();

	/*
	* ׼��Buffer
	* Ϊд��uLength���ȵ�����׼�����壬
	* д�����ʱ�������WriteFinished()��ǿɶ����ݳ���
	*/
	unsigned char* PrepareBuffer(unsigned short uRecvSize);
	/**
	 * д�����
	 * ���д�����д�����ݵĳ���
	 * ������PrepareBuffer()�ɶԵ���
	 */
	void WriteFinished(unsigned short uLength);
	/*
	 *	���ṩbool WriteData( char *data, int nSize );�ӿ�
	 *	��д���ݣ���Ϊ�˱���COPY���������Ч��
	 *	����Ҳͳһ��IOCP��EPOLL������д�뷽ʽ
	 */

	int GetID();//ȡ��ID
	Socket* GetSocket();//ȡ���׽���
	bool IsReadAble();//�ɶ�
	uint32 GetLength();//ȡ�����ݳ���
	//�ӽ��ջ����ж����ݣ����ݲ�����ֱ�ӷ���false��������ģʽ
	//bClearCacheΪfalse���������ݲ���ӽ��ջ���ɾ�����´λ��Ǵ���ͬλ�ö�ȡ
	bool ReadData(unsigned char* pMsg, unsigned short uLength, bool bClearCache = true );
	bool SendData( const unsigned char* pMsg, unsigned short uLength );
	bool SendStart();//��ʼ��������
	void SendEnd();//������������
	void Close();//�ر�����
		
	//ˢ������ʱ��
	void RefreshHeart();
	//ȡ���ϴ�����ʱ��
	time_t GetLastHeart();
	bool IsInGroups( int *groups, int count );//����ĳЩ����
	bool IsServer();//������һ������
	void InGroup( int groupID );//����ĳ���飬ͬһ�������ɶ�ε��ø÷��������������飬���̰߳�ȫ
	void OutGroup( int groupID );//��ĳ����ɾ�������̰߳�ȫ
	void Release();
	/*
		������ַ
		NetConnect��ʾ���ǶԷ������������ַ���ǶԷ���ַ
	 */
	void GetAddress( std::string &ip, int &port );//������ַ
	/*
		��������ַ
	 */
	void GetServerAddress( std::string &ip, int &port );
	
	int m_useCount;//���ʼ���
	IOBuffer m_recvBuffer;//���ջ���
	int m_nReadCount;//���ڽ��ж����ջ�����߳���
	bool m_bReadAble;//io�����������ݿɶ�
	bool m_bConnect;//��������
	int m_nDoCloseWorkCount;//NetServer::OnCloseִ�д���
	
	IOBuffer m_sendBuffer;//���ͻ���
	int m_nSendCount;//���ڽ��з��͵��߳���
	bool m_bSendAble;//io��������������Ҫ����
	Mutex m_sendMutex;//���Ͳ���������
	
	Socket m_socket;//socketָ�룬���ڵ����������
	NetEventMonitor *m_pNetMonitor;//�ײ�Ͷ�ݲ����ӿ�
	STNetEngine *m_pEngine;//���ڹر�����
	int m_id;
	STNetHost m_host;
	time_t m_tLastHeart;//���һ���յ�����ʱ��
	bool m_bIsServer;//�������ͷ�����
	std::map<int,int> m_groups;//��������
	MemoryPool *m_pMemoryPool;
	
};

}
#endif // !defined MDK_STNETCONNECT_H
