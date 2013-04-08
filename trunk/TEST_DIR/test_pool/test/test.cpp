// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Pool.h"
#include "TraceUtil.h"
#include "comm_protocol.h"
#include <math.h>
//��������
#define TYPE_POLICY_GAME                 0x01       //��Ϸ����
#define TPYE_POLICY_HTTP                 0x02       //URL����
#define TYPE_POLICY_PERIOD               0x03       //ʱ�β���

#define POLICY_BUFSIZE_PERIOD            64*4         //ʱ�β��Ի�������󳤶� 4��
#define POLICY_BUFSIZE_HTTP              256*1024*8   //URL���Ի�������󳤶� 1024*8��
#define POLICY_BUFSIZE_GAME              1024*1024*4  //��Ϸ���Ի�������󳤶� 1024*4��

int GetTypeSize(int iType)
{
	return TYPE_POLICY_PERIOD==iType ? POLICY_BUFSIZE_PERIOD:(TPYE_POLICY_HTTP==iType ? POLICY_BUFSIZE_HTTP:POLICY_BUFSIZE_GAME);
}

int _tmain(int argc, _TCHAR* argv[])
{
	TPool<DATAPACKET> m_objRecvPacketPool;          //���հ������
	DWORD dwBufLen = 1314; 
	double dbCount = (double)dwBufLen/(double)sizeof(DATAPACKET);
	dbCount = ceil(dbCount);     //����ȡ��
	int iCount = (int)dbCount;
	PDATAPACKET pstuDataPacket = m_objRecvPacketPool.New();
	pstuDataPacket->Init(sizeof(DATAPACKET));
	pstuDataPacket->pstuDataHead->dwSerialNo = 5;
	pstuDataPacket->pstuDataHead->dwPacketLen = 128;
	pstuDataPacket->pstuDataHead->dwReturn = 0;
	pstuDataPacket->pstuDataHead->wHeadLen = 32;
	pstuDataPacket->pstuDataHead->wEncode = 0;
	pstuDataPacket->pstuDataHead->dwVersion = 1;


	strcpy((char*)pstuDataPacket->szbyData+32, "��һ���ǳ��������������ͨ����ȡ�����ļ���ȷ��category��appender��layout�ȶ���Ҳ�����Ƿǳ��Ƽ���ʹ�÷�ʽ����������ͨ�������ļ��������еض��������ԣ��������±��룬��̬������־��¼�Ĳ��ԡ�");
	char* pPoint = (char*)pstuDataPacket->szbyData+32;
	pstuDataPacket = m_objRecvPacketPool.New(2);
	pstuDataPacket->Init(2*sizeof(DATAPACKET));

	int iSize1 = GetTypeSize(1);
	int iSize2 = GetTypeSize(2);
	int iSize3 = GetTypeSize(3);
	
	//pstuDataPacket->Init();

	m_objRecvPacketPool.Delete(pstuDataPacket);

	pstuDataPacket = m_objRecvPacketPool.New(2000);
	for (int i=0; i<10;i++)
	{
		pstuDataPacket = m_objRecvPacketPool.New(200);
		//pstuDataPacket->Init();
	}

	//Sleep(5000);
	//pstuDataPacket->Init();
	pstuDataPacket->Reset();
	m_objRecvPacketPool.Delete(pstuDataPacket);

	pstuDataPacket = m_objRecvPacketPool.New();
	m_objRecvPacketPool.Free();


	DWORD dwLen1 = sizeof(BARBASEINFO);         //Ӫ����������Ϣ
	DWORD dwLen2 = sizeof(BARTERMINALINFO);     //��Ӫ����������Ϣ 
	DWORD dwLen3 = sizeof(CUSTOMERINFO);        //�����û���Ϣ
	DWORD dwLen4 = sizeof(CUSTOMERLOGINOUTLOG); //�û����»���־
	DWORD dwLen5 = sizeof(BARBUSINESSCONTROL);  //��Ӫ��������

	
	DWORD dwLen6 = sizeof(DATAPACKET);         //�������ݻ�����
	DWORD dwLen7 = sizeof(POLICYALERT);        //���Ա��� ������ײ��¼
	DWORD dwLen8 = sizeof(POLICYTIME);         //��Ӫʱ�����
	DWORD dwLen9 = sizeof(POLICYHTTP);         //��ֹ��ҳ����
	DWORD dwLen10 = sizeof(POLICYGAME);        //��ֹ��Ϸ����
	DWORD dwLen11 = sizeof(POLICYCONTENT);     //���ز��Ի�����
	DWORD dwLen12 = sizeof(ISSUEDMESSAGE);     //�·���Ϣ
	DWORD dwLen13 = sizeof(COMMONDISSUED);     //�·�(�ظ�)����
	return 0;
}

