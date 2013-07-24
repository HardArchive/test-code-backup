#pragma once
/***************************************************************************************************
* 1�� File       �� HandleCreakDLP.h
* 2�� Version    �� *.*
* 3�� Description�� ���ܴ��������
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-24 17:41:51
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#include <tchar.h>
#include "comm_protocol.h"
#include "safestl.h"
#include "NPipe.h"

class CHandleCreakDLP
{
public:

	CHandleCreakDLP(void)
	{
		Reset();
		//_asm int 3;
		m_clsNamePipe.Open(_T("RG_DecryptDLP"));
		
	}

	~CHandleCreakDLP(void)
	{
		Reset();
	}

	bool HandleMessage()
	{
		//::MessageBox(NULL, "DLL�Ѿ�����", "SendFile", MB_OK);
		//_asm int 3;
		m_bExitFlag = true;
		while(m_bExitFlag)
		{
			Recv();
			Sleep(1000);
		}

		return true;		
	}
	//�����߳� һ������Ŀ¼
	//һ�����շ�����Ϣ

private:
	//��������
	int Recv()
	{
		DATAPACKET stuDataPacket;
		stuDataPacket.Reset();
		stuDataPacket.pstuDataHead->Reset();

		int iReadLen = m_clsNamePipe.Read(stuDataPacket.pstuDataHead, sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;
		iReadLen = m_clsNamePipe.Read(stuDataPacket.szbyData+sizeof(DATAHEAD), stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;

		//��������ַ�·���Ϣ
		if (TYPE_PACKET_PATH == stuDataPacket.pstuDataHead->dwPacketType)
		{
			_tcscpy_s(m_tszECodePath, MAX_PATH, (TCHAR*)(stuDataPacket.szbyData+sizeof(DATAHEAD)));
			::MessageBox(NULL, m_tszECodePath, _T("��Ϣ��ʾ"), MB_OK);
		}

		//������������·���Ϣ
		if (TYPE_PACKET_COMMAND == stuDataPacket.pstuDataHead->dwPacketType)
		{
			::MessageBox(NULL, _T("�յ�������������·���Ϣ"), _T("��Ϣ��ʾ"), MB_OK);
			m_bExitFlag = false;
		}

		//�ļ�ӳ����Ϣ�ظ� 
		if (TYPE_PACKET_REPLAY_MEMSHARE == stuDataPacket.pstuDataHead->dwPacketType)
		{
			::MessageBox(NULL, _T("�յ��ļ�ӳ����Ϣ�ظ�"), _T("��Ϣ��ʾ"), MB_OK);
		}
		return iReadLen;
	}

	void Reset()
	{
		m_dwSerialNo = 0;
		m_bExitFlag = false;
		m_clsNamePipe.Close();
		memset(m_tszECodePath, 0, MAX_PATH*sizeof(TCHAR));
		m_mapSerialNoPacket.Lock();
		m_mapSerialNoPacket.clear();
		m_mapSerialNoPacket.Unlock();
	}

private:
	RG::CNamedPipe                          m_clsNamePipe;
	bool                                    m_bExitFlag;                   //�߳��˳���־
	DWORD                                   m_dwSerialNo;                  //��ˮ��
	TCHAR                                   m_tszECodePath[MAX_PATH];      //���ܴ���·��
	RG::SAFESTL::CMap<DWORD, DATAPACKET>    m_mapSerialNoPacket;           //��ˮ�������ݰ���Ӧ map
};
