#include "stdafx.h"
#include "PhoneService.h"

// �Ƿ����ģʽ����
extern BOOL g_bRunInDebugMode;
 
const LPCTSTR ServiceName = _T("400PhoneServerService");
const LPCTSTR PROGRAM_MUTEX_NAME=_T("400PhoneServerService");


// CAuthenCenterServiceApp ����

CPhoneService::CPhoneService():CNTService(ServiceName, ServiceName)
{
}

// CAuthenCenterServiceApp ��ʼ��

BOOL CPhoneService::InitInstance()
{
	if(!recvData.Init())
	{
		return FALSE;
	}
	// ��������
	RegisterService(__argc, __argv);
	return TRUE;
}


//�����˳�
int CPhoneService::ExitInstance()
{
	recvData.UnInit();
	return TRUE;
}

//���з���
void CPhoneService::Run(DWORD argc, LPTSTR * argv)
{
	g_bRunInDebugMode = this->m_bDebug;
	TracePrint(LOG_INFO, "400Phone����ʼ���� ! \n");
	ReportStatus(SERVICE_RUNNING);
	recvData.Start();
}

//ֹͣ����
void CPhoneService::Stop()
{
	this->recvData.Stop();
	ReportStatus(SERVICE_STOP_PENDING);
	TracePrint(LOG_INFO, _T("400Phone���������Stopֹͣ����\r\n"));
}