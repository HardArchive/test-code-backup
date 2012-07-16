#pragma once
/******************************************************************************* 
* 1�� �ļ����ƣ� ProcessDNS
* 2�� �桡������ Version *.*
* 3�� ��    ���� DNS
* 4�� ������ƣ� ��ë
* 5�� �������ڣ� 2012-4-20 16:45:35
* 6�� �� �� �ˣ� 
* 7�� �޸����ڣ� 
********************************************************************************/ 

//#include "struct_define.h"
#include "WinpCap.h"

#include "QueryDNS.h"
#include "CaptureDNS.h"
class CProcessDNS
{
public:

	CProcessDNS(void)
	{
	}

	~CProcessDNS(void)
	{
	}
public:
	int VerifyDNS(char* pszDomain, char* pszIp)
	{		
		return m_objQueryDns.VerifyDNS(pszDomain, pszIp);
	}

	int CapPacketDNS(bool bIsStart)
	{
		if (bIsStart)
		{
			TRACE("��ʼ���ء���-QNA\n");
			m_objCaptureDns.start();
		}
		else
		{
			TRACE("ֹͣ���ء���-QNA\n");
			m_objCaptureDns.stop();
		}
		return 1;
	}

	void Init()
	{
		m_objQueryDns.Init();
		//m_objWinpCap.OpenAdapter(1);
		m_objCaptureDns.Init();

	}

    //����DNS�ظ���
	int CaptureDNS(char* pszDomain)
	{
	}
	//��ȡ����
	int GetData();
private:
	CQueryDNS m_objQueryDns;
	CCaptureDNS m_objCaptureDns;
};
