#include "stdafx.h"
#include "DataHelper.h"

//��������ָ��
_ConnectionPtr CDataHelper::m_pConnPtr;
//���ݿ������ַ���
char CDataHelper::szConn[512]={0};

int SEVER_PORT;

//��ȡע�����Ϣ
void CDataHelper::GetRegConfig(LPCTSTR pKey,char *pValue)
{
	HKEY	hkey;
	DWORD	dwType = REG_SZ, dwLenth = 128;
	char	sTemp[128] = {0};
	
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\RainSoft\\400phone"), 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hkey) != ERROR_SUCCESS )
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
BOOL CDataHelper::InitConnection()
{
	::CoInitialize(NULL);
	//��ȡ���ݿ������ַ���
	char szServer[32]={0};
	char szDatabase[128]={0};
	char szUId[64]={0};
	char szPwd[64]={0};
	GetRegConfig(TEXT("Server"),szServer);
	GetRegConfig(TEXT("Database"),szDatabase);
	GetRegConfig(TEXT("UId"),szUId);
	GetRegConfig(TEXT("Pwd"),szPwd);
	if(strlen(szServer)==0 || strlen(szDatabase)==0 || strlen(szUId)==0 || strlen(szPwd)==0)
	{
		::OutputDebugString(_T("400Phone���� ��ע����ȡ���ݿ����Ӳ���ʧ��!\r\n"));
		return FALSE;
	}
	

	sprintf_s(szConn,512,"driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",szServer,szDatabase,szUId,szPwd);

	//���������������Ӷ���-----------------------------------------------------------------------------
	HRESULT hr;
	hr=m_pConnPtr.CreateInstance(__uuidof(Connection));
	if(FAILED(hr))
	{
		::OutputDebugString(_T("400Phone����create ADODB.Connection failure !\r\n"));
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr.Release();
		}
		return FALSE;
	}
	try
	{
		hr=m_pConnPtr->Open(szConn,"","",adModeUnknown);
		if(FAILED(hr))
		{
			m_pConnPtr.Release();
			TracePrint(LOG_ERROR,TEXT("400Phone���� ADODB.Connection open failure ! \n"));
			return FALSE;
		}
	}
	catch(...)
	{
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr.Release();
		}
		TracePrint(LOG_ERROR,TEXT("400Phone���� ADODB.Connection open failure ! \n"));
		return FALSE;
	}
	 
	::OutputDebugString(_T("400Phone����ADODB.Connection open success !\r\n"));
	//----------------------------------------------------------------------------------------------------
	
	//��ʼ���ɹ�
	return TRUE;
}

//�ر�����
void CDataHelper::ExitConnection()
{
	if(m_pConnPtr!=NULL)
	{
		m_pConnPtr->Close();
	    m_pConnPtr.Release();
	}
	::CoUninitialize();
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

	BOOL bRet=FALSE;
	_CommandPtr spCmdPtr=NULL;

	//����Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("400Phone���� create ADODB.Command failure ! \n"));
		if(spCmdPtr!=NULL)
		{
			spCmdPtr.Release();
		}
		return FALSE;
	}

	try
	{
		//ΪCommand��Ӳ���
		spCmdPtr->CommandType=adCmdText;
		spCmdPtr->CommandText=lpSql;

		//ִ�д洢����
		spCmdPtr->ActiveConnection=m_pConnPtr;
		spCmdPtr->Execute(NULL,NULL,adCmdText);
		bRet=TRUE;
	}
	catch(_com_error ex)
	{ 
		TracePrint(LOG_ERROR,TEXT("400Phone���� ExecuteSql failure !%d \n"), GetLastError());
		bRet=FALSE;
	}
	if(spCmdPtr!=NULL)
	{
		spCmdPtr.Release();
	}
	return bRet;
}

/*
//�����Ӷ�����ִ��SQL���
BOOL CDataHelper::ExecuteSql(LPCTSTR lpSql)
{
	HRESULT hr;
	//������ӹر��������
	if(!OpenConnection())
	{
		return FALSE;
	}

	BOOL bRet=FALSE;
	_CommandPtr spCmdPtr=NULL;
	//_RecordsetPtr pRecordset = NULL;

	//����Command
	hr=spCmdPtr.CreateInstance(__uuidof(Command));
	if(FAILED(hr))
	{
		TracePrint(LOG_ERROR,TEXT("400Phone���� create ADODB.Command failure ! \n"));
		if(spCmdPtr!=NULL)
		{
			spCmdPtr.Release();
		}
		return FALSE;
	}

	try
	{
		//ΪCommand��Ӳ���
		spCmdPtr->CommandType=adCmdText;
		spCmdPtr->CommandText=lpSql;

		//ִ�д洢����
		spCmdPtr->ActiveConnection=m_pConnPtr;
		spCmdPtr->Execute(NULL,NULL,adCmdText);
		bRet=TRUE;
	}
	catch(_com_error ex)
	{ 
		TracePrint(LOG_ERROR,TEXT("400Phone���� ExecuteSql failure ! \n"));
		bRet=FALSE;
	}
	//if (iType)
	//{
	//	iType = pRecordset->GetRecordCount();

	//	if (iType<0)
	//	{
	//		bRet=FALSE;
	//	}
	//	else
	//	{
	//		bRet = TRUE;
	//	}
	//}
	//pRecordset = NULL;

	if(spCmdPtr!=NULL)
	{
		spCmdPtr.Release();
	}

	
	return bRet;
}
*/

//�򿪵����ݿ������
BOOL CDataHelper::OpenConnection()
{
	//������ӹر��������
	if(!TestConnection())
	{
		if(m_pConnPtr !=NULL)
		{
			m_pConnPtr->Close();
			m_pConnPtr.Release();
		}
		 
		HRESULT hr=m_pConnPtr.CreateInstance(__uuidof(Connection));
		if(FAILED(hr))
		{
			::OutputDebugString(_T("400Phone����create ADODB.Connection failure !\r\n"));
			return FALSE;
		}
		try
		{
			hr=m_pConnPtr->Open(szConn,"","",adModeUnknown);
			if(FAILED(hr))
			{
				TracePrint(LOG_ERROR,TEXT("400Phone���� ADODB.Connection open failure ! \n"));
				if(m_pConnPtr !=NULL)
				{
				    m_pConnPtr.Release();
				}
				return FALSE;
			}
		}
		catch(...)
		{
			TracePrint(LOG_ERROR,TEXT("400Phone���� ADODB.Connection open failure ! \n"));
			if(m_pConnPtr !=NULL)
			{
			    m_pConnPtr.Release();
			}
			return FALSE;
		}
	}
	return TRUE;
}

//��������
BOOL CDataHelper::TestConnection()
{
	_RecordsetPtr spRecord=NULL;
	BOOL bRet=FALSE;
	CString str("select 1 as t");
	
	try
	{
		_variant_t vt;
		spRecord=m_pConnPtr->Execute(_bstr_t(str),&vt,adCmdText);
		if(spRecord!=NULL)
		{
			_variant_t vtRet=spRecord->GetCollect("t");
			if(vtRet.intVal ==1)
			{
				bRet=TRUE;
			}
		}
	}
	catch(...)
	{
		 bRet=FALSE;
	}
	if(spRecord !=NULL)
	{
		spRecord->Close();
		spRecord.Release();
	}
	return bRet;
}

BOOL CDataHelper::AddPhoneAndPwd(const char *pszPhone,const char *pszPwd)
{
	char szSql[512]={0};

	////��ѯ����
	////SELECT * FROM tUser WHERE sAccount='13621607084'
	//sprintf_s(szSql,512,"SELECT * FROM tUser WHERE sAccount='%s'", pszPhone);
	//if (CDataHelper::ExecuteSql((LPCTSTR)szSql, 1))
	//{//��ѯ����¼
	//	ZeroMemory(szSql, sizeof(szSql));
	//	// ���´���
	//	// UPDATE tUser SET sPwd='%s' WHERE sAccount='%s'  UPDATE tUser SET sPwd='1111' WHERE sAccount='13621607084'
	//	sprintf_s(szSql,512,"UPDATE tUser SET sPwd='%s' WHERE sAccount='%s'", pszPwd, pszPhone);
	//}
	//else
	//{//δ��ѯ��
	//	ZeroMemory(szSql, sizeof(szSql));
	//	sprintf_s(szSql,512,"insert into tUser(sAccount,sPwd) values('%s','%s')",pszPhone,pszPwd);		
	//}
	
	//sprintf_s(szSql,512,"insert into tUser(sAccount,sPwd) values('%s','%s')",pszPhone,pszPwd);	
	sprintf_s(szSql,512,"exec dbo.dt_Save400Phone '%s','%s'",pszPhone, pszPwd);	  //ִ�д洢����
	return CDataHelper::ExecuteSql((LPCTSTR)szSql);
}