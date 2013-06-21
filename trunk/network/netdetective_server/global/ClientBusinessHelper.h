#pragma once
#include "socket_module/socket/IocpServer.h"
#include "socket_module/Pool.h"
#include "socket_module/CriticalSection.h"
#include "comm_protocol.h"
//#include "IOCPServerHelper.h"
/***************************************************************************************************
* 1�� File       �� �ͻ���ҵ���������
* 2�� Version    �� 1.0
* 3�� Description�� 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-3-20 14:49:54
* 6�� History    �� 
* 7�� Remark     �� ֻ���������� ��������  ������CIOCPServerHelper����
****************************************************************************************************/
//�ͻ���ҵ���������

class CClientBusinessHelper
{
public:
	CClientBusinessHelper(TPool<DATAPACKET>* pobjRecvPacketPool, CIocpServer* pclsIocpServer, CLIENTBASEINFO& stuClientBaseInfo);
	~CClientBusinessHelper(void);
public:
	void Release();
	int Decode(const char* pRecv, const int nRcvLen);

private:
	inline bool CheckSerialNo(DWORD dwInSerialNo);
	int CheckHeadPaceket(const char* pRecv, const int nRcvLen);       //��ͷ���
	inline void ReceiveReset();	                                      // ���ݸ�λ
	bool Send(const BYTE* pBuffer, int iLen);                         //�������� ����ID ���� ����

	int HandlePacket();        //���������

	bool GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo);


public:
	DWORD m_dwInSerialNo;                     //����յ����ݰ���ˮ��
	PCLIENTBASEINFO m_pstuClientBaseInfo;     //�ͻ�����Ϣָ�� �ɵ����� CIOCPServerHelper����
	PDATAPACKET m_pstuDataPacket;             //�ͻ����շ�����ָ��  ָ����ջ�����  һ��ֻ���һ�����ݰ�
	TPool<DATAPACKET>* m_pobjRecvPacketPool;  //���հ������
	CIocpServer* m_pclsIOCPServer;
};