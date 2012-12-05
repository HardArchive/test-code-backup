#include "StdAfx.h"
#include "DataHelper.h"

//��_variant_t����ת����char *,������Ҫ��̬�����ڴ�
BOOL WideStringToCharString(_variant_t &var,char *Dest,int destLen)
{
	if(var.vt !=VT_BSTR)
	{
		Dest[0]=0;
		return FALSE;
	}
	char szTem[68]={0};
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)(var.bstrVal),-1,NULL,0,NULL,FALSE);
	if(dwNum>=68)
	{
		Dest[0]=0;
		return FALSE;
	}
	WideCharToMultiByte (CP_OEMCP,NULL,(LPCWSTR)(var.bstrVal),-1,szTem,dwNum,NULL,FALSE);
	strcpy_s(Dest,destLen,szTem);
	return TRUE;
}

CDataHelper::CDataHelper(void)
{
	::CoInitialize(NULL);
	ZeroMemory(m_tszConn, 512);
}

CDataHelper::~CDataHelper(void)
{
	ZeroMemory(m_tszConn, 512);
	ExitConnection();
	::CoUninitialize();
}

//��ȡע�����Ϣ
void CDataHelper::GetRegConfig(LPCTSTR pKey, char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	char	sTemp[128] = {0};

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\AuthenCenter"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
	{
		RegCloseKey(hkey);
		return;
	}

	if (RegQueryValueEx(hkey,pKey, 0, &dwType, (LPBYTE)&sTemp, (LPDWORD)&dwLenth) != 0 )
	{
		RegCloseKey(hkey);
		return;
	}
	RegCloseKey(hkey);
	strcpy(pValue, sTemp);
}

//��ʼ������
bool CDataHelper::InitConnection(/*PTCHAR ptszServer, PTCHAR ptszDatabase, PTCHAR ptszUId, PTCHAR ptszPwd*/)
{
	////��ȡ���ݿ������ַ���
	TCHAR tszServer[32] = {_T("192.168.30.138")};
	TCHAR tszDatabase[128] = {_T("BJCultureSys")};
	TCHAR tszUId[64] = {_T("sa")};
	TCHAR tszPwd[64] = {_T("sa")};
	//GetRegConfig(TEXT("Server"), szServer);
	//GetRegConfig(TEXT("Database"), szDatabase);
	//GetRegConfig(TEXT("UId"), szUId);
	//GetRegConfig(TEXT("Pwd"), szPwd);
	//if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	//{
	//	::OutputDebugString(_T("AuthenCenterService���� ��ע����ȡ���ݿ����Ӳ���ʧ��!\r\n"));
	//	return false;
	//}

	_stprintf_s(m_tszConn, 512, _T("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s"),
		tszServer, tszDatabase, tszUId, tszPwd
		/*ptszServer, ptszDatabase, ptszUId, ptszPwd*/);

	//���������������Ӷ���-----------------------------------------------------------------------------
	if(!m_pConnection.CreateConnection(m_tszConn))
	{
		::OutputDebugString(_T("CDataHelper::InitConnection create ADODB.Connection failure !\r\n"));
		return false;
	}
	//----------------------------------------------------------------------------------------------------

	//��ʼ���ɹ�
	return true;
}

//�ر�����
void CDataHelper::ExitConnection()
{
	m_pConnection.ReleaseConnection();
}


//�򿪵����ݿ������
bool CDataHelper::OpenConnection()
{
	//������ӹر��������
	if(m_pConnection.CheckConnection())
	{
		return true;
	}

	if(!m_pConnection.CreateConnection(m_tszConn))
	{
		::OutputDebugString(_T("CDataHelper::OpenConnection create ADODB.Connection failure !\r\n"));
		return false;
	}
	return true;
}


//�����Ӷ�����ִ��SQL���
bool CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//������ӹر��������
	if(!OpenConnection())
	{
		return false;
	}

	bool bRet = true;
	_CommandPtr spCmdPtr = NULL;

	//����Command
	hr = spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� create ADODB.Command failure ! \n"));
		return false;
	}


	//ΪCommand��Ӳ���
	spCmdPtr->CommandType = adCmdText;
	spCmdPtr->CommandText = lpSql;

	//ִ�д洢����
	if(!m_pConnection.Execute(spCmdPtr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� ExecuteSql failure ! \n"));
		bRet=false;
	}
	spCmdPtr.Release();
	spCmdPtr = NULL;
	return bRet;
}


//��ȡ�洢����
bool CDataHelper::GetRemoteControl()
{
	HRESULT hr;
	//������ӹر��������
	if(!OpenConnection())
		return false;

	bool bRet = true;
	_CommandPtr spCmdPtr = NULL;

	//����Command
	hr = spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� create ADODB.Command failure ! \n"));
		return false;
	}

	//char spSql[128] = {"EXEC GetRemoteControl"};
	////ΪCommand��Ӳ���
	//spCmdPtr->CommandType = adCmdText;
	//spCmdPtr->CommandText = lpSql;

	_RecordsetPtr spRecord = NULL;
	_variant_t vt;
	//ִ�д洢����
	if(m_pConnection.Execute(spRecord, _bstr_t(_T("EXEC GetRemoteControl")), &vt, adCmdText))
	{
		DWORD dwTem = 0;
		//TracePrint(LOG_ERROR,TEXT("AuthenCenterService���� ExecuteSql failure ! \n"));
		_variant_t vtRet = spRecord->GetCollect(_T("CommandID"));
		dwTem = vtRet.intVal;
		vtRet = spRecord->GetCollect(_T("CommandType"));
		dwTem = vtRet.intVal;
		
		//sCompanyID
		//var=spLogined->GetCollect("sCompanyID");
		//bRet=WideStringToCharString(var,item.szCompanyID,16);
		//if(!bRet){spLogined->MoveNext();continue;}

		////sComputerIP
		//var=spLogined->GetCollect("sComputerIP");
		//bRet=WideStringToCharString(var,item.szComputerIp,32);
		//if(!bRet){spLogined->MoveNext();continue;}
	}
	spCmdPtr.Release();
	spCmdPtr = NULL;
	return bRet;

}