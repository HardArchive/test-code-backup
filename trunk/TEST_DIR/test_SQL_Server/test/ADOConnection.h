#pragma once

#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")

class CADOConnection
{
public:
	CADOConnection(void)
	{
		m_pConnPtr=NULL;
		::InitializeCriticalSection(&m_cs);
	}
	~CADOConnection(void)
	{
		Release();
		::DeleteCriticalSection(&m_cs);
	}

private:
	void Release()
	{
		__try
		{
			if(m_pConnPtr != NULL)
			{
				m_pConnPtr->Close();
				m_pConnPtr.Release();
				m_pConnPtr = NULL;
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			m_pConnPtr = NULL;
		}
	}

public:
	bool CreateConnection(TCHAR tszConn[512])
	{
		::EnterCriticalSection(&m_cs);
		if(m_pConnPtr !=NULL)
		{
			Release();
		}
		//创建并打开数据连接对象
		HRESULT hr = m_pConnPtr.CreateInstance(__uuidof(Connection));
		if(FAILED(hr))
		{
			::LeaveCriticalSection(&m_cs);
			return false;
		}

		try
		{
			hr = m_pConnPtr->Open(tszConn, _T(""), _T(""), adModeUnknown);
			if(FAILED(hr))
			{
				Release();
				::LeaveCriticalSection(&m_cs);
				return false;
			}
		}
		catch (...)
		{
			Release();
			::LeaveCriticalSection(&m_cs);
			return false;
		}
		::LeaveCriticalSection(&m_cs);
		return true;
	}

	void ReleaseConnection()
	{
		::EnterCriticalSection(&m_cs);
		Release();
		::LeaveCriticalSection(&m_cs);
	}

	//检查连接状态 正常返回true， 否则返回false
	bool CheckConnection()
	{
		_RecordsetPtr spRecord = NULL;
		bool bRet = false;

		try
		{
			_variant_t vt;
			if(Execute(spRecord, _bstr_t(_T("select 1 as t")), &vt, adCmdText))
			{
				_variant_t vtRet = spRecord->GetCollect(_T("t"));
				if(vtRet.intVal == 1)
				{
					bRet = true;
				}
			}
		}
		catch(...)
		{
			bRet=false;
		}
		if(spRecord != NULL)
		{
			spRecord->Close();
			spRecord.Release();
			spRecord = NULL;
		}
		return bRet;
	}
	bool Execute(_RecordsetPtr &rsPrt, _bstr_t CommandText, VARIANT* RecordAffects, long Options)
	{
		bool bRet = false;
		::EnterCriticalSection(&m_cs);
		try
		{
			rsPrt = m_pConnPtr->Execute(CommandText, RecordAffects, Options);
			bRet = true;
		}
		catch(...)
		{
			bRet = false;
		}
		::LeaveCriticalSection(&m_cs);
		return bRet;
	}
	bool Execute(_CommandPtr &cmdPrt)
	{
		bool bRet = false;
		::EnterCriticalSection(&m_cs);
		try
		{
			cmdPrt->ActiveConnection = m_pConnPtr;
			cmdPrt->Execute(NULL, NULL, adCmdText);
			bRet = true;
		}
		catch (...)
		{
			bRet = false;
		}
		::LeaveCriticalSection(&m_cs);
		return bRet;
	}

private:
	_ConnectionPtr m_pConnPtr;
	CRITICAL_SECTION m_cs;
};
