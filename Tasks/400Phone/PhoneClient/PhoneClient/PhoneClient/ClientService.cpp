#include "stdafx.h"
#include "ClientService.h"
#include <winsock2.h>
// �Ƿ����ģʽ����
extern BOOL g_bRunInDebugMode;
 
const LPCTSTR ServiceName = _T("400PhoneClientService");
const LPCTSTR PROGRAM_MUTEX_NAME=_T("400PhoneClientrService");


// CAuthenCenterServiceApp ����

CClientService::CClientService():CNTService(ServiceName, ServiceName)
{

}

// CAuthenCenterServiceApp ��ʼ��

BOOL CClientService::InitInstance()
{
	WSAData wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata))
	{
		printf("WSAStartup failure! \n");
		return FALSE;
	}

	// ��������
	RegisterService(__argc, __argv);
	return TRUE;
}


//�����˳�
int CClientService::ExitInstance()
{
	WSACleanup();
	return TRUE;
}

//���з���
void CClientService::Run(DWORD argc, LPTSTR * argv)
{
	g_bRunInDebugMode = this->m_bDebug;
	TracePrint(LOG_INFO, "400Phone client����ʼ���� ! \n");
	ReportStatus(SERVICE_RUNNING);

	//fileSearch.SetPath("F:\\GycProject\\c++Pro\\PhoneClient\\pwd\\");
	//fileSearch.SetPath(szPath/*"c:\\yunlong\\"*/);
	fileSearch.Start();

}

//ֹͣ����
void CClientService::Stop()
{
	fileSearch.Stop();
	ReportStatus(SERVICE_STOP_PENDING);
	TracePrint(LOG_INFO, _T("400Phone client���������Stopֹͣ����\r\n"));
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
}