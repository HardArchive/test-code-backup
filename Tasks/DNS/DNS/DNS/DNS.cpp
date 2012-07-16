// DNS.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ProcessDNS.h"
#include "DNS.h"

//ȫ�ֱ���
bool g_bIsInit = false;          //�Ƿ��ʼ����
CProcessDNS g_ProcessDNS;

//��ʼ������
extern "C" __declspec(dllexport) int InitDNS()
{
	//g_ProcessDNS = new CProcessDNS;
	g_ProcessDNS.Init();

	g_bIsInit = true;

	return 1;
}
/******************************************************************************* 
1�� �������ƣ� DNSVerify
2�� ���������� DNS��֤
3�� ��������� char* pszDomain ����, char* pszIp IP��ַ
4�� �� �� ֵ�� ��֤��ȷ����1�� ���󷵻�-1�� ʧ�ܷ���0
5�� ��̬�ڴ棺 
6�� ������ƣ�  ��ë
7�� �������ڣ� 2012-4-20 15:39:49
8�� ��    ע�� 
********************************************************************************/ 
extern "C" __declspec(dllexport) int DNSVerify(char* pszDomain, char* pszIp)
{
	TRACE("����-����DNS��֤����-QNA\n");
	if (!g_bIsInit)
	{
		TRACE("DNSVerifyδ��ʼ��-QNA\n");
		return -1;
	}
	return g_ProcessDNS.VerifyDNS(pszDomain, pszIp);
}

/******************************************************************************* 
1�� �������ƣ� CapPacketDNS
2�� ���������� ������ر�DNS���ݰ�����
3�� ��������� bool bIsStart Ϊtrueʱ������ falseʱ�ر�
4�� �� �� ֵ�� �ɹ�����1�� ʧ�ܷ���0�� �����ظ�ֵ
5�� ��̬�ڴ棺 ��
6�� ������ƣ�  ��ë
7�� �������ڣ� 2012-4-26 13:11:24
8�� ��    ע�� 
********************************************************************************/ 
extern "C" __declspec(dllexport) int CapPacketDNS(bool bIsStart)
{
	if (!g_bIsInit)
	{
		TRACE("DNSVerifyδ��ʼ��-QNA\n");
		return -1;
	}
	g_ProcessDNS.CapPacketDNS(bIsStart);

	return 1;
}



