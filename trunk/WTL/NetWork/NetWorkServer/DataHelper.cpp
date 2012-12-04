#include "StdAfx.h"
#include "DataHelper.h"

CDataHelper::CDataHelper(void)
{
	ZeroMemory(szConn, 512);
}

CDataHelper::~CDataHelper(void)
{
}

//��ʼ������
BOOL CDataHelper::InitConnection()
{
	//��ȡ���ݿ������ַ���
	char szServer[32] = {0};
	char szDatabase[128] = {0};
	char szUId[64] = {0};
	char szPwd[64] = {0};
	GetRegConfig(TEXT("Server"), szServer);
	GetRegConfig(TEXT("Database"), szDatabase);
	GetRegConfig(TEXT("UId"), szUId);
	GetRegConfig(TEXT("Pwd"), szPwd);
	if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	{
		::OutputDebugString(_T("AuthenCenterService���� ��ע����ȡ���ݿ����Ӳ���ʧ��!\r\n"));
		return FALSE;
	}
	//��ȡ�˿ں�
	char szTCPPort[16] = {0};
	char szUDPPort[16] = {0};
	GetRegConfig(TEXT("TcpPort"), szTCPPort);
	GetRegConfig(TEXT("UdpPort"), szUDPPort);

	if(strlen(szTCPPort)==0 || strlen(szUDPPort)==0)
	{
		::OutputDebugString(_T("AuthenCenterService���� ��ע����ȡ�˿ںŲ���ʧ��!\r\n"));
		return FALSE;
	}
	SEVER_PORT = atoi(szTCPPort);
	HEART_PORT = atoi(szUDPPort);


	sprintf_s(szConn,512,"driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",szServer,szDatabase,szUId,szPwd);

	//���������������Ӷ���-----------------------------------------------------------------------------
	if(!m_pConnection.CreateConnection(szConn))
	{
		::OutputDebugString(_T("AuthenCenterService����create ADODB.Connection failure !\r\n"));
		return FALSE;
	}
	//----------------------------------------------------------------------------------------------------

	//��ʼ���ɹ�
	return TRUE;
}

//�ر�����
void CDataHelper::ExitConnection()
{
	m_pConnection.ReleaseConnection();
}

//�����Ӷ�����ִ��SQL���
BOOL CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//������ӹر��������
	if(!OpenConnection())
	{
		return FALSE;
	}

	BOOL bRet=TRUE;
	_CommandPtr spCmdPtr=NULL;

	//����Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� create ADODB.Command failure ! \n"));
		return FALSE;
	}

	//ΪCommand��Ӳ���
	spCmdPtr->CommandType=adCmdText;
	spCmdPtr->CommandText=lpSql;

	//ִ�д洢����
	if(!m_pConnection.Execute(spCmdPtr))
	{
		TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� ExecuteSql failure ! \n"));
		bRet=FALSE;
	}
	spCmdPtr.Release();
	spCmdPtr=NULL;
	return bRet;
}