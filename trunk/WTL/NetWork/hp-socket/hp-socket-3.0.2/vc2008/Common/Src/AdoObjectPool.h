/*
 * Copyright Bruce Liang (ldcsaa@gmail.com)
 *
 * Version	: 2.3.1
 * Author	: Bruce Liang
 * Website	: http://www.jessma.org
 * Porject	: https://code.google.com/p/ldcsaa
 * Bolg		: http://www.cnblogs.com/ldcsaa
 * WeiBo	: http://weibo.com/u/1402935851
 * QQ Group	: 75375912
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/******************************************************************************

Module:  AdoObjectPool.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: ��װ���̵߳� ADO ���ݿ�����.
Desc:
		 1. CAdoObject ��װһ�����ݿ�����
		 2. CAdoObjectPool ��װ���ݿ����ӳ�

Usage:
		 1. ͨ��Ӧ�ó���ֻʹ�� CAdoObjectPool ����, ���ش��� CAdoObject ����
		 2. ʹ�÷���: �����ݿ���ʺ����ж���һ�� CAdoObjectPool �ֲ�����, Ȼ��
			���øö������Ӧ����
			(��: TestDBAccessor���Գ���� DBAccessor �����ʹ��)

Rule:	1.	��: "DBAccessor.h" ��ͷ��ע��

Example:	��: TestDBAccessor���Գ���
											
******************************************************************************/

#pragma once

#import <msado15.dll> no_namespace rename ("EOF", "adoEOF") rename("BOF","adoBOF")


#include <icrsint.h>
#include <vector>

#include "GeneralHelper.h"
#include "InitCom.h"
#include "CriticalSection.h"
#include "Event.h"
#include "WaitFor.h"
#include "VarConv.h"

/******************* ͨ���ֶ����ƻ�ȡ�ֶ�ֵ *******************/
#define GET_FIELD_ITEM(rs, field)		rs->Fields->Item[_T(#field)]->Value
#define GET_FIELD_CY(rs, field)			vartocy		(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_STR(rs, field)		vartostr	(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_BOOL(rs, field)		vartobool	(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_DATE(rs, field)		vartodate	(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_BYTE(rs, field)		vartoi1		(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_SHORT(rs, field)		vartoi2		(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_LONG(rs, field)		vartoi4		(GET_FIELD_ITEM(rs, field))
#define GET_FIELD_DOUBLE(rs, field)		vartof		(GET_FIELD_ITEM(rs, field))
/******************* ͨ���ֶ�������ȡ�ֶ�ֵ *******************/
#define GET_INDEX_ITEM(rs, field)		rs->Fields->Item[(long)field]->Value
#define GET_INDEX_CY(rs, field)			vartocy		(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_STR(rs, field)		vartostr	(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_BOOL(rs, field)		vartobool	(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_DATE(rs, field)		vartodate	(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_BYTE(rs, field)		vartoi1		(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_SHORT(rs, field)		vartoi2		(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_LONG(rs, field)		vartoi4		(GET_INDEX_ITEM(rs, field))
#define GET_INDEX_DOUBLE(rs, field)		vartof		(GET_INDEX_ITEM(rs, field))


/****************************************************************************************************
											CAdoObject 
****************************************************************************************************/
class CAdoObject
{
	friend class CAdoObjectPool;
public:

	/**********************************
	����: �����ݿ�����
	����: 
			bPreClose		: ���ԭ���д򿪵����ӣ��Ƿ��ȹر�		Ĭ��: ��
			Options			: ����ѡ��								Ĭ��: adConnectUnspecified
	����ֵ		: �������
	�쳣		: _com_error
	**********************************/
	HRESULT OpenConnection(BOOL bPreClose = FALSE, long Options = adConnectUnspecified)
	{
		HRESULT hr		= S_FALSE;
		BOOL isOpened	= IsOpened();

		if(!bPreClose && isOpened)
			return hr;

		if(isOpened)
			hr = CloseConnection();

		if(SUCCEEDED(hr))
		{
			hr = (m_pConnection->Open(m_bstrDSN, _T(""), _T(""), Options));
			if(SUCCEEDED(hr))
				m_pCommand->ActiveConnection = m_pConnection;
		}
		return hr;
	}

	/**********************************
	����: �򿪼�¼��
	����: 
			Source		: Ҫ�򿪵�Դ, ������ SQL �������ݱ���
			Options		: Դ����		Ĭ��: SQL ���
			CursorType	: �α�����		Ĭ��: ��̬�α�
			LockType	: ������		Ĭ��: �ֹ���
			pRecordset	: Ҫ�򿪵ļ�¼������, ������� NULL ���
						  CAdoObject �����ڲ������ļ�¼������ m_pRecordset, 
						  ����򿪵���CAdoObject::m_pRecordset, ����Ҫ����
						  CloseRecordset()�������йر�
										Ĭ��: NULL
	����ֵ: �򿪵ļ�¼��	
	�쳣  : _com_error
	**********************************/
	_RecordsetPtr OpenRecordset(const CString& Source, long Options = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic, _RecordsetPtr pRecordset = NULL)
	{
		if(pRecordset == NULL)
			pRecordset = m_pRecordset;

		HRESULT hr = CloseRecordset(pRecordset);
		if(SUCCEEDED(hr))
			hr = pRecordset->Open((LPCTSTR)Source, (_Connection*)m_pConnection, CursorType, LockType, Options);
		
		ASSERT(SUCCEEDED(hr));

		return pRecordset;
	}

	/**********************************
	����: ִ������
	����: 
			Source			: Ҫִ�е�Դ, ������ SQL ����洢���̵�
			RecordsAffected	: Ӱ�������
			Parameters		: �������
			CmdType			: ��������		Ĭ��: SQL ���
			CmdTimeout		: ��ʱֵ, �������0��ʹ��Ĭ������
											Ĭ��: 0
			pCommand		: Ҫִ�е��������, �������NULL��ִ��
							  CAdoObject �����ڲ�������������� m_pCommand
											Ĭ��: NULL
	����ֵ: �����¼��	
	�쳣  : _com_error
	**********************************/
	_RecordsetPtr ExecuteCommand(const CString& Command, long* RecordsAffected, CommandTypeEnum CmdType = adCmdText, VARIANT* Parameters = NULL, long CmdTimeout = 0, _CommandPtr pCommand = NULL)
	{
		if(pCommand == NULL)
			pCommand = m_pCommand;
		else
			pCommand->ActiveConnection = m_pConnection;

		*RecordsAffected = 0;
		pCommand->CommandText = (LPCTSTR)Command;
		if(CmdTimeout != 0)
			pCommand->CommandTimeout = CmdTimeout;
		_variant_t var(*RecordsAffected);
		_RecordsetPtr rs = pCommand->Execute(&var, Parameters, CmdType);
		if(var.vt != VT_NULL && var.vt != VT_EMPTY)
			*RecordsAffected = var.lVal;

		return rs;
	}

	/**********************************
	����: �رռ�¼��
	����: 
			pRecordset	: Ҫ�رյļ�¼������, �������NULL��ر�
						  CAdoObject �����ڲ������ļ�¼������ m_pRecordset
										Ĭ��: NULL
	����ֵ: �������	
	**********************************/

	HRESULT CloseRecordset(_RecordsetPtr pRecordset = NULL)
	{
		if(pRecordset == NULL)
			pRecordset = m_pRecordset;

		return (pRecordset->GetState() & adStateOpen) ? pRecordset->Close() : S_FALSE;
	}

	/**********************************
	����: ȡ������
	����: 
			pCommand		: Ҫȡ�����������, �������NULL��ȡ��
							  CAdoObject �����ڲ�������������� m_pCommand
											Ĭ��: NULL
	����ֵ: �������	
	**********************************/
	HRESULT CancelCommand(_CommandPtr pCommand = NULL)
	{
		if(pCommand == NULL)
			pCommand = (_CommandPtr*)(&m_pCommand);

		return (pCommand->GetState() & adStateExecuting) ? pCommand->Cancel() : S_FALSE;
	}

	// ȡ�ö����ڲ����������Ӷ���
	const _ConnectionPtr&	GetConnection() {return m_pConnection;}
	// ȡ�ö����ڲ��ļ�¼������
	const _RecordsetPtr&	GetRecordset()	{return m_pRecordset;}
	// ȡ�ö����ڲ����������
	const _CommandPtr&		GetCommand()	{return m_pCommand;}

private:
	// �ر����ݿ����Ӷ���
	HRESULT CloseConnection()	{return IsOpened() ? m_pConnection->Close() : S_FALSE;}

	// ����Ƿ��Ѿ��������ݿ����Ӷ���
	BOOL IsOpened()				{return m_pConnection->GetState() & adStateOpen;}

public:
	CAdoObject(LPCTSTR dsn)
	: m_pConnection	(_T("ADODB.Connection"))
	, m_pRecordset	(_T("ADODB.Recordset"))
	, m_pCommand	(_T("ADODB.Command"))
	, m_bstrDSN		(dsn)
	, m_lReference	(0)
	, m_evtAdo		(NULL, FALSE, TRUE)
	{
	}

	~CAdoObject()
	{
	}

private:
	CAdoObject(const CAdoObject&);
	CAdoObject operator = (const CAdoObject&);

private:
	_bstr_t			m_bstrDSN;		// �����ַ���

	_ConnectionPtr	m_pConnection;	// ���Ӷ���
	_RecordsetPtr	m_pRecordset;	// ��¼������
	_CommandPtr		m_pCommand;		// �������

/******************* ���������� CAdoObjectPool ʹ�úͲ��� *******************/
private:
	long GetReference()			{return m_lReference;}
	long IncReference()			{return ++m_lReference;}
	long DecReference()			{return --m_lReference;}

	HANDLE GetEventHandle()		{return m_evtAdo.GetHandle();}
	BOOL SetEvent()				{return m_evtAdo.Set();}

	long m_lReference;	// ���ü���

	CEvt m_evtAdo;		// ��������¼�

};

typedef vector<CAdoObject*>			CAdoVector;
typedef hash_map<int, CAdoObject*>	CAdoMap;
typedef CAdoVector::iterator		IAdoIteratorV;
typedef CAdoVector::const_iterator	IAdoConstIteratorCV;
typedef CAdoMap::iterator			IAdoIteratorM;
typedef CAdoMap::const_iterator		IAdoConstIteratorCM;

/****************************************************************************************************
											CAdoObjectPool
****************************************************************************************************/
class CAdoObjectPool
{
public:
	/**********************************
	����: ��ʼ�����ӳ�
	����: 
			dsn			: ���ݿ����Ӵ�
			poolsize	: ���ӳش�С	Ĭ��: DEFAULT_CONNECTION_POOL_SIZE
	����ֵ: �������, ���ʧ�ܻ���� _com_error �쳣
	**********************************/
	static BOOL Init(LPCTSTR dsn, DWORD poolsize = DEFAULT_CONNECTION_POOL_SIZE)
	{

		if(sm_bHasInited)
			return FALSE;


		CLocalLock<CCriSec> local(sm_csAdo);

		ASSERT(poolsize >= 0);
		ASSERT(!sm_pevtAdo);

		if(poolsize == 0)
			poolsize = DEFAULT_CONNECTION_POOL_SIZE;

		sm_pevtAdo = new HANDLE[poolsize];

		for(DWORD i = 0; i < poolsize; ++i)
		{
			CAdoObject* ado	= new CAdoObject(dsn);
			sm_pevtAdo[i]	= ado->GetEventHandle();
			sm_vtAdo.push_back(ado);
		}

		return (sm_bHasInited = TRUE);
	}

	/**********************************
	����: �ͷ����ӳ�
	����: 
	����ֵ: �������	
	**********************************/
	static BOOL UnInit()
	{
		if(!sm_bHasInited)
			return FALSE;

		CLocalLock<CCriSec> local(sm_csAdo);

		WaitAllAdoObjectFree();
		ClearPtrArray(sm_vtAdo);

		if(sm_pevtAdo)
		{
			delete[] sm_pevtAdo;
			sm_pevtAdo = NULL;
		}

		return !(sm_bHasInited = FALSE);
	}

	/**********************************
	����: ���ӳ��Ƿ��Ѿ�����ʼ��
	����: 
	����ֵ: �������	
	**********************************/
	static BOOL HasInited() {return sm_bHasInited;}

public:
	/**********************************
	����: ���캯��
			1. �������ǰ�� CAdoObject ����δ����������, ������������
			2. �ѵ�ǰ�߳� ID �󶨵���ǰ CAdoObject ����
			3. ���ӵ�ǰ CAdoObject ��������ü���
	����: 
			Options		: ���������ӵĲ���	Ĭ��: ͬ����ʽ
	����ֵ: 	
	**********************************/
	CAdoObjectPool(DWORD nWaitTime = INFINITE)
	: m_padoCurrent(NULL)
	{
		ASSERT(sm_vtAdo.size() > 0);

		CLocalLock<CCriSec> local(sm_csAdo);

		m_padoCurrent = GetFreeAdoObject(nWaitTime);

		if(m_padoCurrent)
			m_padoCurrent->IncReference();
	}

	/**********************************
	����: ��������
			1. �ݼ���ǰ CAdoObject ��������ü���
			2. �����ǰ CAdoObject ��������ü���Ϊ 0 ��: 
				A. ȡ����ǰ CAdoObject ������߳� ID ��
				B. �رյ�ǰ CAdoObject ����ļ�¼������
				C. �رյ�ǰ CAdoObject ��������ݿ����Ӷ���
	����ֵ: 	
	**********************************/

	~CAdoObjectPool()
	{
		if(m_padoCurrent && m_padoCurrent->DecReference() == 0)
		{
			sm_mpAdo.erase(::GetCurrentThreadId());
			m_padoCurrent->CloseRecordset();
			m_padoCurrent->CloseConnection();
			m_padoCurrent->SetEvent();
		}
	}

	/**********************************
	����:	����ָ��ǰ CAdoObject ���������ָ��
	����ֵ: ��ǰ CAdoObject ����
	**********************************/
	CAdoObject* operator ->() {return m_padoCurrent;}

	// �����Ƿ����
	BOOL IsValid() {return m_padoCurrent != NULL;}

	// Ĭ�����ݿ����ӳش�С
	static const DWORD	DEFAULT_CONNECTION_POOL_SIZE = 10;

private:
	/**********************************
	����:	��ȡ���е� CAdoObject ����
	����ֵ: 
	**********************************/
	static CAdoObject* GetFreeAdoObject(DWORD nWaitTime = INFINITE)
	{
		size_t poolsize = sm_vtAdo.size();

		ASSERT(poolsize > 0);

		long tid = ::GetCurrentThreadId();
		CAdoObject* pCurrent = NULL;

		// �����ڵ�ǰ�߳��в��ұ��򿪵� CAdoObjectPool ����
		IAdoConstIteratorCM it = sm_mpAdo.find(tid);
		if(it != sm_mpAdo.end())
			pCurrent = it->second;

		while(!pCurrent)
		{
			DWORD retval = ::MsgWaitForMultipleObjectsEx(poolsize, sm_pevtAdo, nWaitTime, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
			
			retval -= WAIT_OBJECT_0;

			if(retval < poolsize)
			{
				pCurrent		= sm_vtAdo[retval];
				sm_mpAdo[tid]	= pCurrent;

				ASSERT(!pCurrent->IsOpened());
			}
			else if(retval == poolsize)
				::PeekMessageLoop();
			else if( retval == WAIT_TIMEOUT - WAIT_OBJECT_0)
				break;
		}

		return pCurrent;
	}

	/**********************************
	����:	�ȴ����ӳ������е� CAdoObject �����Ϊ����״̬
	����ֵ: 
	**********************************/
	static void WaitAllAdoObjectFree()
	{
		size_t poolsize = sm_vtAdo.size();

		ASSERT(poolsize > 0);

		while(TRUE)
		{
			DWORD retval = ::MsgWaitForMultipleObjectsEx(poolsize, sm_pevtAdo, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE | MWMO_WAITALL);

			retval -= WAIT_OBJECT_0;

			if(retval < poolsize)
			{
				ASSERT(!sm_vtAdo[retval]->IsOpened());

				break;
			}
			else if(retval == poolsize)
				::PeekMessageLoop();
		}
	}

private:
	CAdoObjectPool(const CAdoObjectPool&);
	CAdoObjectPool operator = (const CAdoObjectPool&);

private:
	static BOOL			sm_bHasInited;
	static CInitCOM		sm_comAdo;

	static CCriSec		sm_csAdo;
	static CAdoVector	sm_vtAdo;
	static CAdoMap		sm_mpAdo;

	static HANDLE*		sm_pevtAdo;

	CAdoObject*			m_padoCurrent;
};

